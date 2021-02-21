// Copyright (C) 2003 Dolphin Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0 or later versions.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official SVN repository and contact information can be found at
// http://code.google.com/p/dolphin-emu/

#include "ppsspp_config.h"

#ifdef VITA

#include <string>

#include "FileUtil.h"
#include "MemoryUtil.h"
#include "MemArena.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

size_t MemArena::roundup(size_t x) {
	return x;
}

bool MemArena::NeedsProbing() {
	return false;
}

void MemArena::GrabLowMemSpace(size_t size) {
	return;
}

void MemArena::ReleaseSpace() {
}

void *MemArena::CreateView(s64 offset, size_t size, void *base)
{
	void *retval = mmap(base, size, PROT_READ | PROT_WRITE, MAP_SHARED |
		((base == 0) ? 0 : MAP_FIXED), -1, 0);
	if (retval == MAP_FAILED) {
		NOTICE_LOG(MEMMAP, "mmap for base %p and size %zx failed", base, size);
		return 0;
	}
	return retval;
}

void MemArena::ReleaseView(void* view, size_t size) {
	munmap(view, size);
}

u8* MemArena::Find4GBBase() {
	return (u8 *)0xC0000000;
}

#endif
