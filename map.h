// map.h - ustd queue class

#pragma once
#include "array.h"

namespace ustd {

/*! \brief Lightweight c++11 dictionary map implementation.

map.h is a minimal, yet highly portable dictionary map type implementation
that runs well on architectures with very limited resources such as attiny 8kb
avr.

The map class either:

* * allocates memory dynamically on map defintions, or
* * work in fully static mode without any dynamic allocation once the map
object has been created.

The library header-only.

## An example for dynamic mode:

~~~{.cpp}
#include <array.h>

ustd::array<int> intArray;

intArray[0] = 13; // Memory for array[0] is allocated
intArray.add(3);  // the array is extended, if necessary
int p = intArray[0];

printf("[0]:%d [1]:%d length=%d\n", intArray[0], intArray[1], intArray.length())
~~~

## An example for static mode

~~~{.cpp}
#include <array.h>

// array length is fixed 5 (startSize==maxSize), no dynamic extensions:
ustd::array<int> intArray = ustd::array<int>(5, 5, 0, false);
~~~
 */

#define MAX_MAP_SIZE UINT_MAX

template <class K, class V> class map {
  private:
    unsigned int size;
    unsigned int peakSize;
    unsigned int allocSize;
    unsigned int startSize;
    unsigned int maxSize;
    unsigned int incSize;
    bool shrink;
    V bad;

  public:
    ustd::array<K> keys;
    ustd::array<V> values;

  public:
    map(unsigned int startSize = ARRAY_INIT_SIZE,
        unsigned int maxSize = ARRAY_MAX_SIZE,
        unsigned int incSize = ARRAY_INC_SIZE, bool shrink = true)
        : startSize(startSize), maxSize(maxSize), incSize(incSize),
          shrink(shrink), keys(array<K>(startSize, maxSize, incSize, shrink)),
          values(array<V>(startSize, maxSize, incSize, shrink)) {
        size = 0;
        allocSize = startSize;
    }

    ~map() {
    }

    V operator[](K key) const {
        for (unsigned int i = 0; i < keys.length(); i++) {
            if (keys[i] == key)
                return values[i];
        }
#ifdef USTD_ASSERT
        assert(false);  // key not found
#endif
        memset(&bad, 0, sizeof(bad));
        return bad;
    }

    V &operator[](K key) {
        for (unsigned int i = 0; i < keys.length(); i++) {
            if (keys[i] == key) {
                return values[i];
            }
        }
        int i = keys.add(key);
        if (i == -1) {
#ifdef USTD_ASSERT
            assert(false);  // key not found
#endif
            memset(&bad, 0, sizeof(bad));
            return bad;
        }
        if (i >= 0) {
            size++;
            return values[i];
        }
#ifdef USTD_ASSERT
        assert(false);  // key not found
#endif
        memset(&bad, 0, sizeof(bad));
        return bad;
    }

    int find(K key) {
        for (unsigned int i = 0; i < keys.length(); i++) {
            if (keys[i] == key)
                return i;
        }
        return -1;
    }

    int erase(K key) {
        for (unsigned int i = 0; i < keys.length(); i++) {
            if (keys[i] == key) {
                values.erase(i);
                keys.erase(i);
                return i;
            }
        }
        return -1;
    }

    bool isEmpty() {
        if (size == 0)
            return true;
        else
            return false;
    }

    unsigned int length() {
        return (size);
    }

    unsigned int peak() {
        return (peakSize);
    }
};
}  // namespace ustd
