// Check if platform define is already supplied by build system (Travis-CI defines ATMEGA for tests):
#ifndef __ATMEGA__
// Not yet defined? Make a platform define for ESP32:
#define __ESP32__
#endif

#include "ustd_platform.h"

#include "ustd_array.h"
#include "ustd_queue.h"
#include "ustd_map.h"

void setup() {
    ustd::array<int> ar = ustd::array<int>(1, 100, 1);
    ustd::queue<int> qu = ustd::queue<int>(128);
    ustd::map<int, int> mp = ustd::map<int, int>(7, 100, 1);
}

void loop() {
}
