// platform.h - adapt platform specific stuff
#pragma once

#ifdef __ATTINY__
#define KNOWN_PLATFORM 1
#include <Arduino.h>
#include <SoftwareSerial.h>
#elif defined(__ATMEGA__)
#define KNOWN_PLATFORM 1
#include <Arduino.h>
#elif defined(__ESP__)
#define KNOWN_PLATFORM 1
#ifdef __USE_OLD_FS__
#define FS_NO_GLOBALS  // see: https://github.com/esp8266/Arduino/issues/3819
#include <FS.h>
#else
#include <LittleFS.h>
#endif  // __USE_OLD_FS__
#define HAS_SERIAL (1)
#if defined(__ESP32__) || defined(__ESP32DEV__)
#ifdef __USE_OLD_FS__
#include <SPIFFS.h>
#else
#include <LittleFS.h>
#endif  // __USE_OLD_FS__
#include <WiFi.h>
#include <time.h>      // time() ctime()
#include <sys/time.h>  // struct timeval
#else                  // ESP8266
#ifdef __USE_OLD_FS__
#include <SD.h>  //otherwise bear.ssl doesn't compile...
#endif           // __USE_OLD_FS__
#include <ESP8266WiFi.h>
#include <time.h>       // time() ctime()
#include <sys/time.h>   // struct timeval
#include <coredecls.h>  // settimeofday_cb()
#endif                  // ESP8266
#endif                  // ESP

#if defined(__linux__) || defined(__APPLE__)
#define KNOWN_PLATFORM 1
#define __UNIXOID__ 1
#include <cassert>
#include <climits>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sys/time.h>

#define USTD_ASSERTS 1
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

#endif  // end linux, apple

#ifndef UINT_MAX
#define UINT_MAX (65535)  // or 4294967295 (mostly)
#endif

#ifndef KNOWN_PLATFORM
#error Unkown platform. Please use one of the platform defines __ATTINY__, __ATMEGA__, __ESP__, __linux__, __APPLE__
#endif
