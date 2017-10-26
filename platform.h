// platform.h - adapt platform specific stuff
#pragma once

#if defined(__linux__) || defined(__APPLE__)
#define __UNIXOID__ 1
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
unsigned long timeDiff(unsigned long first, unsigned long second) {
    if (second >= first)
        return second - first;
    return (unsigned long)-1 - first + second + 1;
}

class sensorprocessor {
  public:
    unsigned int noVals = 0;
    unsigned int smoothIntervall;
    unsigned int pollTimeSec;
    double sum = 0.0;
    double eps;
    bool first = true;
    double meanVal = 0;
    double lastVal = -99999.0;
    unsigned long last;

    // average of smoothIntervall measurements
    // update sensor value, if newvalue differs by at least eps, or if
    // pollTimeSec has elapsed.
    sensorprocessor(unsigned int smoothIntervall = 5,
                    int unsigned pollTimeSec = 60, double eps = 0.1)
        : smoothIntervall{smoothIntervall}, pollTimeSec{pollTimeSec}, eps{eps} {
        reset();
    }

    // changes the value into a smoothed version
    // returns true, if sensor-value is a valid update
    // an update is valid, if the new value differs by at least eps from last
    // last value, or, if pollTimeSec secs have elapsed.
    bool filter(double *pvalue) {
        meanVal = (meanVal * noVals + (*pvalue)) / (noVals + 1);
        if (noVals < smoothIntervall) {
            ++noVals;
        }
        double delta = lastVal - meanVal;
        if (delta < 0.0) {
            delta = (-1.0) * delta;
        }
        if (delta > eps || first) {
            first = false;
            lastVal = meanVal;
            *pvalue = meanVal;
            last = millis();
            return true;
        } else {
            if (pollTimeSec != 0) {
                if (timeDiff(last, millis()) > pollTimeSec * 1000L) {
                    *pvalue = meanVal;
                    last = millis();
                    lastVal = meanVal;
                    return true;
                }
            }
        }
        return false;
    }

    bool filter(long *plvalue) {
        double tval = (double)*plvalue;
        bool ret = filter(&tval);
        if (ret) {
            *plvalue = (long)tval;
        }
        return ret;
    }

    void reset() {
        noVals = 0;
        sum = 0.0;
        first = true;
        meanVal = 0;
        lastVal = -99999.0;
        last = millis();
    }
};

}  // namespace ustd
