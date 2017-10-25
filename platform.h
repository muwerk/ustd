// platform.h - adapt platform specific stuff
#pragma once

#if defined(__linux__) || defined(__APPLE__)
#include <cassert>
#include <climits>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/time.h>

#define USTD_ASSERTS 1
typedef std::string String;
//#define String std::string
#pragma message("Unixoid")

unsigned long micros() {
    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_usec;
}

#else
#define __ARDUINO__ 1
#endif

#ifndef UINT_MAX
#define UINT_MAX (65535)  // or 4294967295 (mostly)
#endif

namespace ustd {
unsigned long timeDiffMicros(unsigned long first, unsigned long second) {
    if (second >= first)
        return second - first;
    return (unsigned long)-1 - first + second + 1;
}

}  // namespace ustd
