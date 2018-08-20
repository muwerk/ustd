#include <iostream>
#include <list>
#include <string>

#include <stdio.h>
#include <time.h>

#define USE_SERIAL_DBG 1

#include "platform.h"

#include "array.h"
#include "map.h"
#include "queue.h"

using std::cout;
using std::endl;

using ustd::array;
using ustd::map;
using ustd::queue;

int main() {
    cout << "Testing ustd..." << endl;
    array<int> ar = array<int>(1, 100, 1);
    queue<int> qu = queue<int>(128);
    map<String, int> mp = map<String, int>(7, 100, 1);

    for (int i = 0; i < 100; i++) {
        printf("%d ", i);
        ar[i] = i;
        printf(" - ");
        qu.push(new int(i));
        printf(" - ");
        mp[std::to_string(i)] = i;
        printf("\n");
    }
    printf("ar len: %d, alloc=%d\n", ar.length(), ar.alloclen());
    printf("qu len: %d, alloc=%d\n", qu.length(), ar.alloclen());
    printf("mp len: %d, alloc=%d\n", mp.length(), ar.alloclen());

    for (int i = 0; i < 100; i++)
        delete qu.pop();

    bool merr = false;
    for (int i = 0; i < mp.length(); i++) {
        if (mp.keys[i] != std::to_string(i) || i != mp.values[i]) {
            printf("Maps err at %d: %s<->%d\n", i, mp.keys[i].c_str(),
                   mp.values[i]);
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
