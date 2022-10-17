// ustd_platform.h - adapt platform specific stuff
#pragma once

//================= Platform defines ==========================
// https://docs.platformio.org/en/latest/plus/debugging.html
// MCU       CPU        RAM   Flash   EEPROM      Clock  Family
// ATtiny85            512b      8k     512b      20MHz
// Uno                   2k     32k       1k      16MHz  __ARDUINO__
// Mega                  8k    256k       4k      16MHz  __ARDUINO__
// Feater M0 Cortex M0  32k
// STM32F103C8T6        20k     64k               72MHz  __ARM__
// "Bluepill" Cortex-M3
// STM32F411CEU6       128k    512k              100MHz  __ARM__
// "Blackpill" Cortex-M4F
// Teensy 4.0          512k   1984k       1k     600Mhz  __ARM__
// Esp8266              80k 512k-4M           80-160MHz  __ESP__
// ESP32               320k   2M-4M          160-240MHz  __ESP__

// ATtiny85
#define USTD_FEATURE_MEM_512B 512
// Arduino UNO, ATtiny1614, AT328P
#define USTD_FEATURE_MEM_2K 2048
// Arduino MEGA
#define USTD_FEATURE_MEM_8K 8192
// ESP8266, Bluepill
#define USTD_FEATURE_MEM_32K 32768
// Blackpill
#define USTD_FEATURE_MEM_128K 131072
// ESP32
#define USTD_FEATURE_MEM_512K 524288
// Unixoids
#define USTD_FEATURE_MEM_1M 1048576

/*
A Platform sets USTD_FEATURE_MEMORY to one of the above _MEM_ defines.
// #if USTD_FEATURE_MEMORY >= USTD_FEATURE_MEM_2K
// we have at least 2k RAM
// #endif

// Platforms use the following defines to show feature-availability:

// Filesystem:
#define USTD_FEATURE_FILESYSTEM
#define USTD_FEATURE_FS_SPIFFS
#define USTD_FEATURE_FS_LITTLEFS
#define USTD_FEATURE_FS_SD

// User override-options for FS:
#define USTD_OPTION_FS_FORCE_SPIFFS
#define USTD_OPTION_FS_FORCE_NO_FS

// EEPROM:
#define USTD_FEATURE_EEPROM

// Time:
#define USTD_FEATURE_SYSTEMCLOCK
#define USTD_FEATURE_CLK_READ
#define USTD_FEATURE_CLK_SET

// Network:
#define USTD_FEATURE_NETWORK
*/

// Compatibility-1
#if !defined(DONT_USE_FEATURE_COMPATIBILITY)
#if defined(__USE_OLD_FS__)
#define USTD_FEATURE_FS_FORCE_SPIFFS
#pragma message("Please use USTD_OPTION_FS_FORCE_SPIFFS instead of __USE_OLD_FS__")
#endif
#if defined(__USE_SPIFFS_FS__)
#pragma message("Please use USTD_OPTION_FS_FORCE_SPIFFS instead of __USE_SPIFFS_FS__")
#endif
#if defined(__USE_LITTLE_FS__)
#pragma message("Please do not use __USE_LITTLEFS__")
#endif
#endif  // DONT_USE_FEATURE_COMPATIBILITY
// Compatibility-1 end (see below for more)

// ------------- ATTINY ---------------------------------------
#if defined(__ATTINY__)
#if defined(KNOWN_PLATFORM)
#error "Platform already defined"
#endif
#define KNOWN_PLATFORM 1
#define USTD_FEATURE_MEMORY 512
#define USTD_FEATURE_EEPROM
#include <Arduino.h>
#include <SoftwareSerial.h>
#endif  // ATTINY

// ------------- Arduino UNO ----------------------------------
#if defined(__UNO__)
#if defined(KNOWN_PLATFORM)
#error "Platform already defined"
#endif
#define KNOWN_PLATFORM 1
#define USTD_FEATURE_MEMORY 2048
#define USTD_FEATURE_EEPROM
#define __ARDUINO__ 1
#include <Arduino.h>
#include <new.h>  // New Arduino core new operator
#endif            // Arduino UNO

// ------------- Arduino MEGA ---------------------------------
#if defined(__ATMEGA__)
#if defined(KNOWN_PLATFORM)
#error "Platform already defined"
#endif
#define KNOWN_PLATFORM 1
#define USTD_FEATURE_MEMORY 8192
#define USTD_FEATURE_EEPROM
#define __ARDUINO__ 1
#include <Arduino.h>
#include <new.h>  // New Arduino core new operator
#endif            // Arduino Mega

// ------------- Adafruit M0 Wifi ----------------------------
#if defined(__FEATHER_M0__)
#if defined(KNOWN_PLATFORM)
#error "Platform already defined"
#endif
#define KNOWN_PLATFORM 1
#define USTD_FEATURE_MEMORY 32768
#define __ARM__ 1
#include <Arduino.h>
#endif  // Feather M0

// ------------- Raspberry Pi PICO RP2040 ---------------------
#if defined(__RP2040__)
#if defined(KNOWN_PLATFORM)
#error "Platform already defined"
#endif
#define KNOWN_PLATFORM 1
#define USTD_FEATURE_MEMORY 264000
#define USTD_FEATURE_SUPPORTS_NEW_OPERATOR
#include "pico/stdlib.h"
#include "stdlib.h"
#define __ARM__ 1
#define __RP_PICO__ 1
#endif  // RP2040

// ------------- STM32F103C8T6 Bluepill -----------------------
#if defined(__BLUEPILL__)
#if defined(KNOWN_PLATFORM)
#error "Platform already defined"
#endif
#define KNOWN_PLATFORM 1
#define USTD_FEATURE_MEMORY 20480
#define USTD_FEATURE_SUPPORTS_NEW_OPERATOR
#define __ARM__ 1
#include <Arduino.h>
#endif  // Bluepill

// ------------- Adafruit Bluefruit feather (NRF52832) --------
#if defined(__NRF52__)
#if defined(KNOWN_PLATFORM)
#error "Platform already defined"
#endif
#define KNOWN_PLATFORM 1
#define USTD_FEATURE_MEMORY 65536
#define __ARM__ 1
#include <Arduino.h>
#endif  // NRF52

// ------------- ESP8266 --------------------------------------
#if defined(__ESP__) && !defined(__ESP32__) && !defined(__ESP32DEV__)
#if defined(KNOWN_PLATFORM)
#error "Platform already defined"
#endif
#define KNOWN_PLATFORM 1
#define USTD_FEATURE_MEMORY 80000
#include <ESP8266WiFi.h>
#define USTD_FEATURE_NETWORK
#include <time.h>       // time() ctime()
#include <sys/time.h>   // struct timeval
#include <coredecls.h>  // settimeofday_cb()
#define USTD_FEATURE_SYSTEMCLOCK
#define USTD_FEATURE_CLK_READ
#define USTD_FEATURE_CLK_SET
#if !defined(USTD_OPTION_FS_FORCE_NO_FS)
#define USTD_FEATURE_FILESYSTEM
#define FS_NO_GLOBALS  // see: https://github.com/esp8266/Arduino/issues/3819
#if defined(USTD_OPTION_FS_FORCE_SPIFFS)
#include <SPIFFS.h>
#define USTD_FEATURE_FS_SPIFFS
#else  // Use standard LittleFS
#define USTD_FEATURE_FS_LITTLEFS
#include <LittleFS.h>
#endif  // FORCE_SPIFFS
#endif  // FORCE_NO_FS
#endif  // ESP8266

// ------------- STM32 Black Pill -----------------------------
#if defined(__BLACKPILL__)
#if defined(KNOWN_PLATFORM)
#error "Platform already defined"
#endif
#define KNOWN_PLATFORM 1
#define USTD_FEATURE_MEMORY 128000
#define USTD_FEATURE_SUPPORTS_NEW_OPERATOR
#define __ARM__ 1
#include <Arduino.h>
#endif  // Blackpill

// ------------- Adafruit Feather M4 Express ------------------
#if defined(__FEATHER_M4__)
#if defined(KNOWN_PLATFORM)
#error "Platform already defined"
#endif
#define KNOWN_PLATFORM 1
#define USTD_FEATURE_MEMORY 192000
#define __ARM__ 1
#include <Arduino.h>
#endif  // FEATHER_M4

// ------------- Arduino BLE Sense (NRF52840) -----------------
#if defined(__NANOBLE__)
#if defined(KNOWN_PLATFORM)
#error "Platform already defined"
#endif
#define KNOWN_PLATFORM 1
#define USTD_FEATURE_MEMORY 256000
#define USTD_FEATURE_SUPPORTS_NEW_OPERATOR
#define __ARM__ 1
#include <Arduino.h>
#endif  // NANOBLE

// ------------- Teensy 4.0 (Cortex M7, imxrt1062) -----------------
#if defined(__TEENSY40__)
#if defined(KNOWN_PLATFORM)
#error "Platform already defined"
#endif
#define KNOWN_PLATFORM 1
#define USTD_FEATURE_MEMORY 512000
#define USTD_FEATURE_SUPPORTS_NEW_OPERATOR
#define __ARM__ 1
#include <Arduino.h>
#endif  // TEEENSY40

// ------------- ESP32 and ESP32DEV ---------------------------
#if defined(__ESP32__) || defined(__ESP32DEV__) || defined(__ESP32_RISC__)
#if defined(KNOWN_PLATFORM)
#error "Platform already defined"
#endif
#define KNOWN_PLATFORM 1
// This is probably somewhat inconsistent:
#ifndef __ESP__
#define __ESP__
#endif
#ifdef __ESP32_RISC__
#define __RISC_V__
#define USTD_FEATURE_SUPPORTS_NEW_OPERATOR
#define USTD_OPTION_FS_FORCE_LITTLEFS
#else
#define __TENSILICA__
#endif
#define USTD_FEATURE_MEMORY 320000
#include <WiFi.h>
#define USTD_FEATURE_NETWORK
#include <time.h>      // time() ctime()
#include <sys/time.h>  // struct timeval
#define USTD_FEATURE_SYSTEMCLOCK
#define USTD_FEATURE_CLK_READ
#define USTD_FEATURE_CLK_SET
#if !defined(USTD_OPTION_FS_FORCE_NO_FS)
#define USTD_FEATURE_FILESYSTEM
#define FS_NO_GLOBALS  // see: https://github.com/esp8266/Arduino/issues/3819
#if !defined(USTD_OPTION_FS_FORCE_LITTLEFS)
#include <SPIFFS.h>
#define USTD_FEATURE_FS_SPIFFS
#else
#include <LittleFS.h>
#define USTD_FEATURE_FS_LITTLEFS
#endif  // USTD_OPTION_FS_FORCE_LITTLEFS)
#include <FS.h>
#endif  // FORCE_NO_FS
#endif  // ESP32 || ESP32DEV || ESP32_RISC

// ------ RISC-V Maix Bit -------------------------------------
#if defined(__MAIXBIT__)
#if defined(KNOWN_PLATFORM)
#error "Platform already defined"
#endif
#define KNOWN_PLATFORM 1
#define __RISC_V__
#define USTD_FEATURE_MEMORY 6000000
#include <Arduino.h>
#endif

//------- compatibility-2
#if !defined(DONT_USE_FEATURE_COMPATIBILITY)
#if defined(USTD_FEATURE_FS_LITTLEFS)
#define __USE_LITTLEFS__
#endif
#if defined(USTD_FEATURE_FS_SPIFFS)
#define __USE_SPIFFS_FS__
#define __USE_OLD_FS__
#endif
#endif  // DONT_USE_FEATURE_COMPATIBILITY
//-------- end compatibility-2

// ------------- Raspberry Pico -------------------------------
#if defined(__RP_PICO__)
#include <cstring>
#include <string>
typedef std::string String;
inline unsigned long micros() {
    return time_us_64();
}
inline unsigned long millis() {
    return time_us_64() / 1000;
}
#endif  // __RP_PICO__

// ------------- Unixoids -------------------------------------
#if defined(__linux__) || defined(__APPLE__)
#if defined(KNOWN_PLATFORM)
#error "Platform already defined"
#endif
#define KNOWN_PLATFORM 1
#define USTD_FEATURE_MEMORY 1048576
#define __UNIXOID__ 1
#include <climits>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <sys/time.h>
#include <cassert>

#define USTD_FEATURE_NETWORK
#define USTD_FEATURE_FILESYSTEM
#define USTD_FEATURE_SYSTEMCLOCK
#define USTD_FEATURE_CLK_READ
#define USTD_FEATURE_CLK_SET

// ------------- Compatibility libs for Unixoids --------------
/*
#define USTD_ASSERT 1
#include <cassert>
#ifdef USTD_ASSERT
#define ASSERT(f) assert(f)
#else  // else USTD_ASSERT
#define ASSERT(f)
#endif  // end USTD_ASSERT
*/
typedef std::string String;

inline unsigned long micros() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    unsigned long tu;
    tu = (tv.tv_sec % 1000L) * 1000000L + tv.tv_usec;
    return tu;
}
inline unsigned long millis() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    unsigned long tm;
    tm = (tv.tv_sec % 1000000L) * 1000L + tv.tv_usec / 1000L;
    return tm;
}

class SerialSim {
  public:
    void Serial() {
    }
    void begin(unsigned long baud) {
    }
    void println() {
        std::cout << std::endl;
    }

    void println(String s) {
        std::cout << s << std::endl;
    }
    void println(const char *sz) {
        std::cout << sz << std::endl;
    }
    void println(uint8_t b) {
        std::cout << b << std::endl;
    }
    void println(int l) {
        std::cout << l << std::endl;
    }
    void println(unsigned long l) {
        std::cout << l << std::endl;
    }
    void println(double d) {
        std::cout << d << std::endl;
    }

    void print(String s) {
        std::cout << s;
    }
    void print(const char *sz) {
        std::cout << sz;
    }
    void print(uint8_t b) {
        std::cout << b;
    }
    void print(int l) {
        std::cout << l;
    }
    void print(unsigned long l) {
        std::cout << l;
    }
    void print(double d) {
        std::cout << d;
    }
};

SerialSim Serial;

#else   // else linux, apple
// ------------- Debug helpers and small tools for MCUs -------
/*
#if defined(USTD_ASSERT) && !defined(__NANOBLE__)
#ifdef USE_SERIAL_DBG
bool assertFailedLine(const char *filename, int line) {
    Serial.print("Assertion Failed: File ");
    Serial.print(filename);
    Serial.print(", Line ");
    Serial.println(line);
    return false;
}

#define ASSERT(f)                                                                                  \
    if (!(f))                                                                                      \
    assertFailedLine(__FILE__, __LINE__)
#else  // else USE_SERIAL_DBG
#define ASSERT(f)
#endif  // end USE_SERIAL_DBG
#else   // else USTD_ASSERT
#define ASSERT(f)
#endif  // end USTD_ASSERT
*/
#endif  // end linux, apple

#if defined(__ARDUINO__) || defined(__ARM__)
#define USTD_FEATURE_FREE_MEMORY
// from: https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char *sbrk(int incr);
#else   // __ARM__
extern char *__brkval;
#endif  // __arm__

inline int freeMemory() {
    char top;
#ifdef __arm__
    return &top - reinterpret_cast<char *>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
    return &top - __brkval;
#else   // __arm__
    return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
#elif defined(__ESP__)
#define USTD_FEATURE_FREE_MEMORY
inline int freeMemory() {
    return (int)ESP.getFreeHeap();
}
#elif defined(__UNIXOID__)
#define USTD_FEATURE_FREE_MEMORY
// To keep the API compatible, this function gives back max INT_MAX as free memory.
inline int freeMemory() {
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    long long memfree = pages * page_size;
    if (memfree > INT_MAX) {
        return INT_MAX;
    } else {
        return (int)memfree;
    };
}
#endif

#ifdef USE_SERIAL_DBG

#define DBG_INIT(f) Serial.begin(f)
#define DBG_ONLY(f) f
#define DBG(f) Serial.println(f)
#define DBGP(f) Serial.print(f)
#define DBGF(...) Serial.printf(__VA_ARGS__)
#if USE_SERIAL_DBG > 1
#define DBG2(f) Serial.println(f)
#define DBGP2(f) Serial.print(f)
#if USE_SERIAL_DBG > 2
#define DBG3(f) Serial.println(f)
#define DBGP3(f) Serial.print(f)
#else
#define DBG3(f)
#define DBGP3(f)
#endif  // end USE_SERIAL_DBG > 2
#else
#define DBG2(f)
#define DBG3(f)
#define DBGP2(f)
#define DBGP3(f)
#endif  // end USE_SERIAL_DBG > 1

#else

#define DBG_INIT(f)
#define DBG_ONLY(f)
#define DBG(f)
#define DBGP(f)
#define DBGF(...)
#define DBG2(f)
#define DBG3(f)
#define DBGP2(f)
#define DBGP3(f)

#endif  // USE_SERIAL_DBG

#ifndef UINT_MAX
#define UINT_MAX (65535)  // or 4294967295 (mostly)
#endif

#ifndef KNOWN_PLATFORM
#error Unkown platform. Please use a platform define, e.g. __ATMEGA__, __ESP__, __linux__, __APPLE__. Check https://github.com/muwerk/ustd#platform-defines for complete list.
#endif
