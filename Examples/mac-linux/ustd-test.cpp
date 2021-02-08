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

void checkCopy(array<int> ar) {
    printf("COPY ar len: %d, alloc=%d\n", ar.length(), ar.alloclen());
}

void checkRef(array<int> &ar) {
    printf("REF ar len: %d, alloc=%d\n", ar.length(), ar.alloclen());
}

int main() {
    cout << "Testing ustd..." << endl;
    cout << "Memory free is more than: " << freeMemory() << endl;

    array<int> ar = array<int>(1, 100, 1);
    queue<int> qu = queue<int>(128);
    map<int, int> mp = map<int, int>(7, 100, 1);

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

    checkCopy(ar);

    checkRef(ar);

    for (int i = 0; i < 100; i++)
        qu.pop();

    bool merr = false;
    for (int i = 0; i < mp.length(); i++) {
        if (mp.keys[i] != i || i != mp.values[i]) {
            printf("Maps err at %d: %d<->%d\n", i, mp.keys[i], mp.values[i]);
            merr = true;
        }
    }
    if (merr) {
        printf("Map selftest failed!\n");
        exit(-1);
    } else
        printf("Map selftest ok over %d!\n", mp.length());
    bool aerr = false;
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
