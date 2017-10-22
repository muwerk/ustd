// platform.h - adapt platform specific stuff
#pragma once

#ifdef __linux__
#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <climits>
#include <string>
#include <cstring>
#include <sys/time.h>
#define USTD_ASSERTS 1
typedef std::string String;
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

#ifdef __linux__
unsigned long micros() {
    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_usec;
}
#endif

}  // namespace ustd
