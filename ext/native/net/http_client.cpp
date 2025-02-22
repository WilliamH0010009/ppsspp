#include "net/http_client.h"
#include "base/timeutil.h"

#ifndef _WIN32
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define closesocket close
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#include <io.h>
#endif

#include <cmath>
#include <stdio.h>
#include <stdlib.h>

#include "base/logging.h"
#include "base/buffer.h"
#include "base/stringutil.h"
#include "data/compression.h"
#include "file/fd_util.h"
#include "net/resolve.h"
#include "net/url.h"
#include "thread/threadutil.h"

namespace net {

Connection::Connection()
		: port_(-1), resolved_(NULL), sock_(-1) {
}

Connection::~Connection() {
	Disconnect();
	if (resolved_ != NULL)
		DNSResolveFree(resolved_);
}

// For whatever crazy reason, htons isn't available on android x86 on the build server. so here we go.

// TODO: Fix for big-endian
inline unsigned short myhtons(unsigned short x) {
	return (x >> 8) | (x << 8);
}

bool Connection::Resolve(const char *host, int port, DNSType type) {
	if ((intptr_t)sock_ != -1) {
		ELOG("Resolve: Already have a socket");
		return false;
	}
	if (!host || port < 1 || port > 65535) {
		ELOG("Resolve: Invalid host or port (%d)", port);
		return false;
	}

	host_ = host;
	port_ = port;

	char port_str[16];
	snprintf(port_str, sizeof(port_str), "%d", port);

	std::string err;
	if (!net::DNSResolve(host, port_str, &resolved_, err, type)) {
		ELOG("Failed to resolve host %s: %s", host, err.c_str());
		// So that future calls fail.
		port_ = 0;
		return false;
	}

	return true;
}

bool Connection::Connect(int maxTries, double timeout, bool *cancelConnect) {
	if (port_ <= 0) {
		ELOG("Bad port");
		return false;
	}
	sock_ = -1;

	for (int tries = maxTries; tries > 0; --tries) {
		std::vector<uintptr_t> sockets;
		fd_set fds;
		int maxfd = 1;
		FD_ZERO(&fds);
		for (addrinfo *possible = resolved_; possible != nullptr; possible = possible->ai_next) {
			if (possible->ai_family != AF_INET && possible->ai_family != AF_INET6)
				continue;

			int sock = socket(possible->ai_family, SOCK_STREAM, 6);
			if ((intptr_t)sock == -1) {
				ELOG("Bad socket");
				continue;
			}
			fd_util::SetNonBlocking(sock, true);

			// Start trying to connect (async with timeout.)
			connect(sock, possible->ai_addr, (int)possible->ai_addrlen);
			sockets.push_back(sock);
			FD_SET(sock, &fds);
			if (maxfd < sock + 1) {
				maxfd = sock + 1;
			}
		}

		int selectResult = 0;
		long timeoutHalfSeconds = floor(2 * timeout);
		while (timeoutHalfSeconds >= 0 && selectResult == 0) {
			struct timeval tv;
			tv.tv_sec = 0;
			if (timeoutHalfSeconds > 0) {
				// Wait up to 0.5 seconds between cancel checks.
				tv.tv_usec = 500000;
			} else {
				// Wait the remaining <= 0.5 seconds.  Possibly 0, but that's okay.
				tv.tv_usec = (timeout - floor(2 * timeout) / 2) * 1000000.0;
			}
			--timeoutHalfSeconds;

			selectResult = select(maxfd, nullptr, &fds, nullptr, &tv);
			if (cancelConnect && *cancelConnect) {
				break;
			}
		}
		if (selectResult > 0) {
			// Something connected.  Pick the first one that did (if multiple.)
			for (int sock : sockets) {
				if ((intptr_t)sock_ == -1 && FD_ISSET(sock, &fds)) {
					fd_util::SetNonBlocking(sock, false);
					sock_ = sock;
				} else {
					closesocket(sock);
				}
			}

			// Great, now we're good to go.
			return true;
		}

		if (cancelConnect && *cancelConnect) {
			break;
		}

		sleep_ms(1);
	}

	// Nothing connected, unfortunately.
	return false;
}

void Connection::Disconnect() {
	if ((intptr_t)sock_ != -1) {
		closesocket(sock_);
		sock_ = -1;
	}
}

}	// net

namespace http {

// TODO: do something sane here
#define USERAGENT "NATIVEAPP 1.0"

Client::Client() {
	httpVersion_ = "1.1";
	userAgent_ = USERAGENT;
}

Client::~Client() {
	Disconnect();
}

// Ignores line folding (deprecated), but respects field combining.
// Don't use for Set-Cookie, which is a special header per RFC 7230.
bool GetHeaderValue(const std::vector<std::string> &responseHeaders, const std::string &header, std::string *value) {
	std::string search = header + ":";
	bool found = false;

	value->clear();
	for (const std::string &line : responseHeaders) {
		auto stripped = StripSpaces(line);
		if (startsWithNoCase(stripped, search)) {
			size_t value_pos = search.length();
			size_t after_white = stripped.find_first_not_of(" \t", value_pos);
			if (after_white != stripped.npos)
				value_pos = after_white;

			if (!found)
				*value = stripped.substr(value_pos);
			else
				*value += "," + stripped.substr(value_pos);
			found = true;
		}
	}

	return found;
}

void DeChunk(Buffer *inbuffer, Buffer *outbuffer, int contentLength, float *progress) {
	int dechunkedBytes = 0;
	while (true) {
		std::string line;
		inbuffer->TakeLineCRLF(&line);
		if (!line.size())
			return;
		unsigned int chunkSize;
		sscanf(line.c_str(), "%x", &chunkSize);
		if (chunkSize) {
			std::string data;
			inbuffer->Take(chunkSize, &data);
			outbuffer->Append(data);
		} else {
			// a zero size chunk should mean the end.
			inbuffer->clear();
			return;
		}
		dechunkedBytes += chunkSize;
		if (progress && contentLength) {
			*progress = (float)dechunkedBytes / contentLength;
		}
		inbuffer->Skip(2);
	}
}

int Client::GET(const char *resource, Buffer *output, std::vector<std::string> &responseHeaders, float *progress, bool *cancelled) {
	const char *otherHeaders =
		"Accept: */*\r\n"
		"Accept-Encoding: gzip\r\n";
	int err = SendRequest("GET", resource, otherHeaders, progress, cancelled);
	if (err < 0) {
		return err;
	}

	Buffer readbuf;
	int code = ReadResponseHeaders(&readbuf, responseHeaders, progress, cancelled);
	if (code < 0) {
		return code;
	}

	err = ReadResponseEntity(&readbuf, responseHeaders, output, progress, cancelled);
	if (err < 0) {
		return err;
	}
	return code;
}

int Client::GET(const char *resource, Buffer *output, float *progress, bool *cancelled) {
	std::vector<std::string> responseHeaders;
	int code = GET(resource, output, responseHeaders, progress,  cancelled);
	return code;
}

int Client::POST(const char *resource, const std::string &data, const std::string &mime, Buffer *output, float *progress) {
	char otherHeaders[2048];
	if (mime.empty()) {
		snprintf(otherHeaders, sizeof(otherHeaders), "Content-Length: %lld\r\n", (long long)data.size());
	} else {
		snprintf(otherHeaders, sizeof(otherHeaders), "Content-Length: %lld\r\nContent-Type: %s\r\n", (long long)data.size(), mime.c_str());
	}
	int err = SendRequestWithData("POST", resource, data, otherHeaders, progress);
	if (err < 0) {
		return err;
	}

	Buffer readbuf;
	std::vector<std::string> responseHeaders;
	int code = ReadResponseHeaders(&readbuf, responseHeaders, progress);
	if (code < 0) {
		return code;
	}

	err = ReadResponseEntity(&readbuf, responseHeaders, output, progress);
	if (err < 0) {
		return err;
	}
	return code;
}

int Client::POST(const char *resource, const std::string &data, Buffer *output, float *progress) {
	return POST(resource, data, "", output, progress);
}

int Client::SendRequest(const char *method, const char *resource, const char *otherHeaders, float *progress, bool *cancelled) {
	return SendRequestWithData(method, resource, "", otherHeaders, progress, cancelled);
}

int Client::SendRequestWithData(const char *method, const char *resource, const std::string &data, const char *otherHeaders, float *progress, bool *cancelled) {
	if (progress) {
		*progress = 0.01f;
	}

	Buffer buffer;
	const char *tpl =
		"%s %s HTTP/%s\r\n"
		"Host: %s\r\n"
		"User-Agent: %s\r\n"
		"Connection: close\r\n"
		"%s"
		"\r\n";

	buffer.Printf(tpl,
		method, resource, httpVersion_,
		host_.c_str(),
		userAgent_,
		otherHeaders ? otherHeaders : "");
	buffer.Append(data);
	bool flushed = buffer.FlushSocket(sock(), dataTimeout_);
	if (!flushed) {
		return -1;  // TODO error code.
	}
	return 0;
}

int Client::ReadResponseHeaders(Buffer *readbuf, std::vector<std::string> &responseHeaders, float *progress, bool *cancelled) {
	// Snarf all the data we can into RAM. A little unsafe but hey.
	static constexpr float CANCEL_INTERVAL = 0.25f;
	bool ready = false;
	double leftTimeout = dataTimeout_;
	while (!ready) {
		if (cancelled && *cancelled)
			return -1;
		ready = fd_util::WaitUntilReady(sock(), CANCEL_INTERVAL, false);
		if (!ready && leftTimeout >= 0.0) {
			leftTimeout -= CANCEL_INTERVAL;
			if (leftTimeout < 0) {
				ELOG("HTTP headers timed out");
				return -1;
			}
		}
	};
	// Let's hope all the headers are available in a single packet...
	if (readbuf->Read(sock(), 4096) < 0) {
		ELOG("Failed to read HTTP headers :(");
		return -1;
	}

	// Grab the first header line that contains the http code.

	std::string line;
	readbuf->TakeLineCRLF(&line);

	int code;
	size_t code_pos = line.find(' ');
	if (code_pos != line.npos) {
		code_pos = line.find_first_not_of(' ', code_pos);
	}

	if (code_pos != line.npos) {
		code = atoi(&line[code_pos]);
	} else {
		return -1;
	}

	while (true) {
		int sz = readbuf->TakeLineCRLF(&line);
		if (!sz)
			break;
		responseHeaders.push_back(line);
	}

	if (responseHeaders.size() == 0) {
		return -1;
	}

	return code;
}

int Client::ReadResponseEntity(Buffer *readbuf, const std::vector<std::string> &responseHeaders, Buffer *output, float *progress, bool *cancelled) {
	bool gzip = false;
	bool chunked = false;
	int contentLength = 0;
	for (std::string line : responseHeaders) {
		if (startsWithNoCase(line, "Content-Length:")) {
			size_t size_pos = line.find_first_of(' ');
			if (size_pos != line.npos) {
				size_pos = line.find_first_not_of(' ', size_pos);
			}
			if (size_pos != line.npos) {
				contentLength = atoi(&line[size_pos]);
				chunked = false;
			}
		} else if (startsWithNoCase(line, "Content-Encoding:")) {
			// TODO: Case folding...
			if (line.find("gzip") != std::string::npos) {
				gzip = true;
			}
		} else if (startsWithNoCase(line, "Transfer-Encoding:")) {
			// TODO: Case folding...
			if (line.find("chunked") != std::string::npos) {
				chunked = true;
			}
		}
	}

	if (contentLength < 0) {
		// Just sanity checking...
		contentLength = 0;
	}

	if (!contentLength && progress) {
		// Content length is unknown.
		// Set progress to 1% so it looks like something is happening...
		*progress = 0.1f;
	}

	if (!contentLength || !progress) {
		// No way to know how far along we are. Let's just not update the progress counter.
		if (!readbuf->ReadAllWithProgress(sock(), contentLength, nullptr, cancelled))
			return -1;
	} else {
		// Let's read in chunks, updating progress between each.
		if (!readbuf->ReadAllWithProgress(sock(), contentLength, progress, cancelled))
			return -1;
	}

	// output now contains the rest of the reply. Dechunk it.
	if (chunked) {
		DeChunk(readbuf, output, contentLength, progress);
	} else {
		output->Append(*readbuf);
	}

	// If it's gzipped, we decompress it and put it back in the buffer.
	if (gzip) {
		std::string compressed, decompressed;
		output->TakeAll(&compressed);
		bool result = decompress_string(compressed, &decompressed);
		if (!result) {
			ELOG("Error decompressing using zlib");
			if (progress)
				*progress = 0.0f;
			return -1;
		}
		output->Append(decompressed);
	}

	if (progress) {
		*progress = 1.0f;
	}
	return 0;
}

Download::Download(const std::string &url, const std::string &outfile)
	: url_(url), outfile_(outfile) {
}

Download::~Download() {
	if (!joined_) {
		FLOG("Download destructed without join");
	}
}

void Download::Start() {
	thread_ = std::thread(std::bind(&Download::Do, this));
}

void Download::Join() {
	if (joined_) {
		ELOG("Already joined thread!");
	}
	thread_.join();
	joined_ = true;
}

void Download::SetFailed(int code) {
	failed_ = true;
	progress_ = 1.0f;
	completed_ = true;
}

int Download::PerformGET(const std::string &url) {
	Url fileUrl(url);
	if (!fileUrl.Valid()) {
		return -1;
	}

	http::Client client;
	if (!client.Resolve(fileUrl.Host().c_str(), fileUrl.Port())) {
		ELOG("Failed resolving %s", url.c_str());
		return -1;
	}

	if (cancelled_) {
		return -1;
	}

	if (!client.Connect(2, 20.0, &cancelled_)) {
		ELOG("Failed connecting to server or cancelled.");
		return -1;
	}

	if (cancelled_) {
		return -1;
	}

	return client.GET(fileUrl.Resource().c_str(), &buffer_, responseHeaders_, &progress_, &cancelled_);
}

std::string Download::RedirectLocation(const std::string &baseUrl) {
	std::string redirectUrl;
	if (GetHeaderValue(responseHeaders_, "Location", &redirectUrl)) {
		Url url(baseUrl);
		url = url.Relative(redirectUrl);
		redirectUrl = url.ToString();
	}

	return redirectUrl;
}

void Download::Do() {
	setCurrentThreadName("Downloader::Do");
	resultCode_ = 0;

	std::string downloadURL = url_;
	while (resultCode_ == 0) {
		int resultCode = PerformGET(downloadURL);
		if (resultCode == -1) {
			SetFailed(resultCode);
			return;
		}

		if (resultCode == 301 || resultCode == 302 || resultCode == 303 || resultCode == 307 || resultCode == 308) {
			std::string redirectURL = RedirectLocation(downloadURL);
			if (redirectURL.empty()) {
				ELOG("Could not find Location header for redirect");
				resultCode_ = resultCode;
			} else if (redirectURL == downloadURL || redirectURL == url_) {
				// Simple loop detected, bail out.
				resultCode_ = resultCode;
			}

			// Perform the next GET.
			if (resultCode_ == 0)
				ILOG("Download of %s redirected to %s", downloadURL.c_str(), redirectURL.c_str());
			downloadURL = redirectURL;
			continue;
		}

		if (resultCode == 200) {
			ILOG("Completed downloading %s to %s", url_.c_str(), outfile_.empty() ? "memory" : outfile_.c_str());
			if (!outfile_.empty() && !buffer_.FlushToFile(outfile_.c_str())) {
				ELOG("Failed writing download to %s", outfile_.c_str());
			}
		} else {
			ELOG("Error downloading %s to %s: %i", url_.c_str(), outfile_.c_str(), resultCode);
		}
		resultCode_ = resultCode;
	}

	progress_ = 1.0f;

	// Set this last to ensure no race conditions when checking Done. Users must always check
	// Done before looking at the result code.
	completed_ = true;
}

std::shared_ptr<Download> Downloader::StartDownload(const std::string &url, const std::string &outfile) {
	std::shared_ptr<Download> dl(new Download(url, outfile));
	downloads_.push_back(dl);
	dl->Start();
	return dl;
}

std::shared_ptr<Download> Downloader::StartDownloadWithCallback(
	const std::string &url,
	const std::string &outfile,
	std::function<void(Download &)> callback) {
	std::shared_ptr<Download> dl(new Download(url, outfile));
	dl->SetCallback(callback);
	downloads_.push_back(dl);
	dl->Start();
	return dl;
}

void Downloader::Update() {
	restart:
	for (size_t i = 0; i < downloads_.size(); i++) {
		if (downloads_[i]->Progress() == 1.0f || downloads_[i]->Failed()) {
			downloads_[i]->RunCallback();
			downloads_[i]->Join();
			downloads_.erase(downloads_.begin() + i);
			goto restart;
		}
	}
}

std::vector<float> Downloader::GetCurrentProgress() {
	std::vector<float> progress;
	for (size_t i = 0; i < downloads_.size(); i++) {
		if (!downloads_[i]->IsHidden())
			progress.push_back(downloads_[i]->Progress());
	}
	return progress;
}

void Downloader::CancelAll() {
	for (size_t i = 0; i < downloads_.size(); i++) {
		downloads_[i]->Cancel();
	}
	for (size_t i = 0; i < downloads_.size(); i++) {
		downloads_[i]->Join();
	}
	downloads_.clear();
}

}	// http
