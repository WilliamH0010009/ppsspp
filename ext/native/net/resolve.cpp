#include "net/resolve.h"
#include "base/logging.h"
#include "base/timeutil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>


#ifdef _WIN32
#include <WinSock2.h>
#include <Ws2tcpip.h>
#ifndef AI_ADDRCONFIG
#define AI_ADDRCONFIG 0x0400
#endif
#undef min
#undef max
#else
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#endif


namespace net {

void Init()
{
#ifdef _WIN32
	// WSA does its own internal reference counting, no need to keep track of if we inited or not.
	WSADATA wsaData = {0};
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

void Shutdown()
{
#ifdef _WIN32
	WSACleanup();
#endif
}

bool DNSResolve(const std::string &host, const std::string &service, addrinfo **res, std::string &error, DNSType type) {
#if PPSSPP_PLATFORM(SWITCH) || PPSSPP_PLATFORM(VITA)
	// Force IPv4 lookups.
	if (type == DNSType::IPV6) {
		*res = nullptr;
		return false;
	} else if (type == DNSType::ANY) {
		type = DNSType::IPV4;
	}
#endif

	addrinfo hints = {0};
	// TODO: Might be uses to lookup other values.
	hints.ai_socktype = SOCK_STREAM;
#ifdef __ANDROID__
	hints.ai_flags = AI_ADDRCONFIG;
#else
	// AI_V4MAPPED seems to have issues on some platforms, not sure we should include it:
	// http://stackoverflow.com/questions/1408030/what-is-the-purpose-of-the-ai-v4mapped-flag-in-getaddrinfo
	hints.ai_flags = /*AI_V4MAPPED |*/ AI_ADDRCONFIG;
#endif
	hints.ai_protocol = 0;
	if (type == DNSType::IPV4)
		hints.ai_family = AF_INET;
	else if (type == DNSType::IPV6)
		hints.ai_family = AF_INET6;

	const char *servicep = service.length() == 0 ? nullptr : service.c_str();

	*res = nullptr;
	int result = getaddrinfo(host.c_str(), servicep, &hints, res);
	if (result == EAI_AGAIN) {
		// Temporary failure.  Since this already blocks, let's just try once more.
		sleep_ms(1);
		result = getaddrinfo(host.c_str(), servicep, &hints, res);
	}

	if (result != 0) {
#if !PPSSPP_PLATFORM(VITA)
#ifdef _WIN32
		error = gai_strerrorA(result);
#else
		error = gai_strerror(result);
#endif
#endif
		if (*res != nullptr)
			freeaddrinfo(*res);
		*res = nullptr;
		return false;
	}

	return true;
}

void DNSResolveFree(addrinfo *res)
{
	if (res != NULL)
		freeaddrinfo(res);
}

int inet_pton(int af, const char* src, void* dst)
{
	if (af == AF_INET)
	{
		unsigned char *ip = (unsigned char *)dst;
		int k = 0, x = 0;
		char ch;
		for (int i = 0; (ch = src[i]) != 0; i++)
		{
			if (ch == '.')
			{
				ip[k++] = x;
				if (k == 4)
					return 0;
				x = 0;
			}
			else if (ch < '0' || ch > '9')
				return 0;
			else
				x = x * 10 + ch - '0';
			if (x > 255)
				return 0;
		}
		ip[k++] = x;
		if (k != 4)
			return 0;
	}
	else if (af == AF_INET6)
	{
		unsigned short* ip = ( unsigned short* )dst;
		int i;
		for (i = 0; i < 8; i++) ip[i] = 0;
		int k = 0;
		unsigned int x = 0;
		char ch;
		int marknum = 0;
		for (i = 0; src[i] != 0; i++)
		{
			if (src[i] == ':')
				marknum++;
		}
		for (i = 0; (ch = src[i]) != 0; i++)
		{
			if (ch == ':')
			{
				x = ((x & 0xFF00) >> 8) | ((x & 0x00FF) << 8);
				ip[k++] = x;
				if (k == 8)
					return 0;
				x = 0;
				if (i > 0 && src[i - 1] == ':')
					k += 7 - marknum;
			}
			else if (ch >= '0' && ch <= '9')
				x = x * 16 + ch - '0';
			else if (ch >= 'a' && ch <= 'f')
				x = x * 16 + ch - 'a' + 10;
			else if (ch >= 'A' && ch <= 'F')
				x = x * 16 + ch - 'A' + 10;
			else
				return 0;
			if (x > 0xFFFF)
				return 0;
		}
		x = ((x & 0xFF00) >> 8) | ((x & 0x00FF) << 8);
		ip[k++] = x;
		if (k != 8)
			return 0;
	}
	return 1;
}

}
