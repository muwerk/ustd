// platform.h - adapt platform specific stuff
#pragma once

// https://docs.platformio.org/en/latest/plus/debugging.html
// MCU       CPU        RAM   Flash   EEPROM      Clock
// ATtiny85            512b      8k     512b      20MHz
// Uno                   2k     32k       1k      16MHz
// Mega                  8k    256k       4k      16MHz
// STM32F103C8T6        20k     64k               72MHz
// "Bluepill" Cortex-M3
// STM32F411CEU6       128k    512k              100MHz
// "Blackpill" Cortex-M4F
// Esp8266              80k 512k-4M           80-160MHz
// ESP32               520k   2M-4M          160-240MHz

#define USTD_FEATURE_MEM_512B 1  // ATtiny85
#define USTD_FEATURE_MEM_2K 2    // Arduino UNO, ATtiny1614, AT328P
#define USTD_FEATURE_MEM_8K 3    // Arduino MEGA
#define USTD_FEATURE_MEM_32k 4   // ESP8266, Bluepill
#define USTD_FEATURE_MEM_128 5   // Blackpill
#define USTD_FEATURE_MEM_512k 6  // ESP32
#define USTD_FEATURE_MEM_1M 7

/*
A Platform sets USTD_FEATURE_MEMORY to one of the above _MEM_ defines.
// #if USTD_FEATURE_MEMORY >= USTD_FEATURE_MEM_2K
// we have at least 2k RAM
// #endif

// Platforms use the following defines to show feature-availability:
#define USTD_FEATURE_FILESYSTEM
#define USTD_FEATURE_FS_SPIFFS
#define USTD_FEATURE_FS_LITTLEFS
#define USTD_FEATURE_FS_SD

#define USTD_FEATURE_EEPROM

#define USTD_FEATURE_SYSTEMCLOCK
#define USTD_FEATURE_CLK_READ
#define USTD_FEATURE_CLK_SET

#define USTD_FEATURE_NET
*/
#ifdef __ATTINY__
#define KNOWN_PLATFORM 1
#define USTD_FEATURE_MEMORY USTD_FEATURE_MEM_512B
#define USTD_FEATURE_EEPROM
#include <Arduino.h>
#include <SoftwareSerial.h>
#elif defined(__UNO__)
#define KNOWN_PLATFORM 1
#define USTD_FEATURE_MEMORY USTD_FEATURE_MEM_2K
#define USTD_FEATURE_EEPROM
#define __ARDUINO__ 1
#include <Arduino.h>
#include <new.h>  // New Arduino core new operator
#elif defined(__ATMEGA__)
#define KNOWN_PLATFORM 1
#define USTD_FEATURE_MEMORY USTD_FEATURE_MEM_8K
#define USTD_FEATURE_EEPROM
#define __ARDUINO__ 1
#include <Arduino.h>
#include <new.h>  // New Arduino core new operator
#elif defined(__ESP__)
#define KNOWN_PLATFORM 1
#define USTD_FEATURE_FILESYSTEM
#if defined(__USE_OLD_FS__) || (defined(__ESP32__) && !defined(__USE_LITTLE_FS__))
#define FS_NO_GLOBALS  // see: https://github.com/esp8266/Arduino/issues/3819
#if defined(__ESP32__)
#define USTD_FEATURE_MEMORY USTD_FEATURE_MEM_512K
#include <SPIFFS.h>
#define USTD_FEATURE_FS_SPIFFS
#endif
#include <FS.h>
#define __USE_SPIFFS_FS__
#else
#define USTD_FEATURE_FS_LITTLEFS
#include <LittleFS.h>
#define __USE_LITTLE_FS__
#endif  // __USE_OLD_FS__ || ESP32 && !LITTLE_FS
#define HAS_SERIAL (1)
#if defined(__ESP32__) || defined(__ESP32DEV__)
#include <WiFi.h>
#define USTD_FEATURE_NETWORK
#include <time.h>  // time() ctime()
#define USTD_FEATURE_SYSTEMCLOCK
#define USTD_FEATURE_CLK_READ
#define USTD_FEATURE_CLK_SET
#include <sys/time.h>  // struct timeval
#else                  // ESP8266
#if defined(__USE_OLD_FS__) || defined(__USE_SPIFFS_FS__)
//#include <SD.h>  //otherwise bear.ssl doesn't compile...
#endif  // __USE_OLD_FS__
#include <ESP8266WiFi.h>
#define USTD_FEATURE_MEMORY USTD_FEATURE_MEM_32K
#define USTD_FEATURE_NETWORK
#include <time.h>       // time() ctime()
#include <sys/time.h>   // struct timeval
#include <coredecls.h>  // settimeofday_cb()
#define USTD_FEATURE_SYSTEMCLOCK
#define USTD_FEATURE_CLK_READ
#define USTD_FEATURE_CLK_SET
#endif  // ESP8266
#endif  // ESP

#if defined(__linux__) || defined(__APPLE__)
#define KNOWN_PLATFORM 1
#define USTD_FEATURE_MEMORY USTD_FEATURE_MEM_1M
#define __UNIXOID__ 1
#include <climits>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <sys/time.h>
#define USTD_FEATURE_NETWORK
#define USTD_FEATURE_FILESYSTEM
#define USTD_FEATURE_SYSTEMCLOCK
#define USTD_FEATURE_CLK_READ
#define USTD_FEATURE_CLK_SET

#define USTD_ASSERT 1

#ifdef USTD_ASSERT
#include <cassert>
#define ASSERT(f) assert(f)
#else  // else USTD_ASSERT
#define ASSERT(f)
#endif  // end USTD_ASSERT

typedef std::string String;

unsigned long micros() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    unsigned long tu;
    tu = (tv.tv_sec % 1000L) * 1000000L + tv.tv_usec;
    return tu;
}
unsigned long millis() {
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

#else  // else linux, apple

#ifdef USTD_ASSERT
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

#endif  // end linux, apple

#ifdef __ARDUINO__
// from: https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char *sbrk(int incr);
#else   // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
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
int freeMemory() {
    return (int)ESP.getFreeHeap();
}
#elif defined(__UNIXOID__)
// To keep the API compatible, this function gives back max INT_MAX as free memory.
int freeMemory() {
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
#error Unkown platform. Please use one of the platform defines __ATTINY__, __ATMEGA__, __ESP__, __linux__, __APPLE__
#endif
