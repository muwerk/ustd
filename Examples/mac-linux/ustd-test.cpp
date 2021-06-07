#include <iostream>
#include <list>
#include <string>

#include <stdio.h>
#include <time.h>

#define USE_SERIAL_DBG 1

#include "ustd_platform.h"

#include "ustd_array.h"
#include "ustd_map.h"
#include "ustd_queue.h"

#include "ustd_functional.h"

using std::cout;
using std::endl;

using ustd::array;
using ustd::map;
using ustd::queue;

bool arrayIteratorCheck(array<int> &ar) {
    printf("Array Iterator: ");
    for (auto n : ar) {
        printf("%d ", n);
    }
    printf("\n");
    return true;
}

bool queueIteratorCheck(queue<int> &que) {
    printf("Queue Iterator: ");
    for (auto n : que) {
        printf("%d ", n);
    }
    printf("\n");
    return true;
}

bool mapKeysIteratorCheck(map<int, int> mp) {
    printf("Map Iterator: ");
    for (auto n : mp.keysArray()) {
        printf("%d -> %d | ", n, mp[n]);
    }
    printf("\n");
    return true;
}

bool checkCopyAr(array<int> ar) {
    bool aerr = false;
    printf("COPY ar len: %d, alloc=%d\n", ar.length(), ar.alloclen());
    for (int i = 0; i < ar.length(); i++) {
        if (ar[i] != i) {
            aerr = true;
            printf("COPY Array: err at: %d\n", i);
        }
    }
    arrayIteratorCheck(ar);
    return aerr;
}

bool checkRefAr(array<int> &ar) {
    bool aerr = false;
    printf("REF ar len: %d, alloc=%d\n", ar.length(), ar.alloclen());
    for (int i = 0; i < ar.length(); i++) {
        if (ar[i] != i) {
            aerr = true;
            printf("REF Array: err at: %d\n", i);
        }
    }
    arrayIteratorCheck(ar);
    return aerr;
}

bool checkInitializersAr() {
    const int in[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    array<int> ar(in, 20);
    printf("Initlen=%d\n", ar.length());
    for (unsigned int i = 0; i < 20; i++) {
        if (ar[i] != i + 1) {
            printf("init-Array: err at: %d\n", i);
            return false;
        }
    }
    return true;
}

bool constArrayInit() {
    printf("Array const init: ");
    const int ci[] = {1, 2, 3, 4, 5};
    ustd::array<int> ia(ci, 5);
    for (auto i : ia) {
        printf("%d ", i);
    }
    printf("\n");
    return true;
}

bool cpMap(map<int, int> mp, map<int, int> *pmp) {
    mapKeysIteratorCheck(mp);
    mapKeysIteratorCheck(*pmp);
    if (pmp->length() == mp.length()) {
        printf("Copy-map-length: %d\n", mp.length());
        return true;
    } else {
        printf("Copy-map-length: %d vs %d\n", mp.length(), pmp->length());
        return false;
    }
}
bool cpQue(queue<int> qu, queue<int> *pqu) {
    if (pqu->length() == qu.length()) {
        printf("Copy-que-length: %d\n", qu.length());
        return true;
    } else {
        printf("Copy-que-length: %d vs %d\n", qu.length(), pqu->length());
        return false;
    }
}

int main() {
    cout << "Testing ustd..." << endl;
    cout << "Memory free is more than: " << freeMemory() << endl;

    array<int> ar = array<int>(1, 100, 1);
    queue<int> qu = queue<int>(128);
    map<int, int> mp = map<int, int>(7, 100, 1);

    bool aerr = false;
    for (int i = 0; i < 100; i++) {
        // printf("%d ", i);
        ar[i] = i;
        // printf(" - ");
        qu.push(i);
        // printf(" - ");
        mp[i] = i;
        // printf("\n");
    }
    printf("ar len: %d, alloc=%d\n", ar.length(), ar.alloclen());
    printf("qu len: %d\n", qu.length());
    printf("mp len: %d\n", mp.length());

    if (checkCopyAr(ar))
        aerr = true;

    if (checkRefAr(ar))
        aerr = true;

    if (!checkInitializersAr())
        aerr = true;

    constArrayInit();

    bool qerr = false;
    if (!cpQue(qu, &qu))
        qerr = true;
    if (qerr)
        printf("Queue copy error!");

    for (int i = 0; i < 100; i++)
        qu.pop();
    for (int i = 0; i < 100; i++) {
        unsigned int i0, i1;
        qu.push(i + 100);
        qu.getInternalStartStopPtrs(&i0, &i1);
        printf("[%d,%d](%d) ", i0, i1, i);
    }
    queueIteratorCheck(qu);
    for (int i = 0; i < 100; i++) {
        unsigned int i0, i1;
        qu.pop();
        qu.getInternalStartStopPtrs(&i0, &i1);
        printf("[%d,%d](%d) ", i0, i1, i);
    }

    bool merr = false;
    for (int i = 0; i < mp.length(); i++) {
        if (mp.keys[i] != i || i != mp.values[i]) {
            printf("Maps err at %d: %d<->%d\n", i, mp.keys[i], mp.values[i]);
            merr = true;
        }
    }
    if (!cpMap(mp, &mp))
        merr = true;

    if (merr) {
        printf("Map selftest failed!\n");
        exit(-1);
    } else
        printf("Map selftest ok over %d!\n", mp.length());
    for (int i = 0; i < ar.length(); i++) {
        if (ar[i] != i) {
            aerr = true;
            printf("Array: err at: %d\n", i);
        }
    }
    if (aerr) {
        printf("Array selftest failed!\n");
        exit(-1);
    } else
        printf("Array selftest ok over %d!\n", ar.length());

    cout << "Done ustd." << endl;

    return 0;
}
