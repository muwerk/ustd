// ustd_map.h - ustd queue class

#pragma once
#include "ustd_array.h"

namespace ustd {

#define MAX_MAP_SIZE UINT_MAX

/*! \brief Lightweight c++11 dictionary map implementation.

ustd_map.h is a minimal, yet highly portable dictionary map type implementation
that runs well on architectures with very limited resources such as attiny 8kb
avr.

The map class either:

* * allocates memory dynamically on map defintions, or
* * work in fully static mode without any dynamic allocation once the map
object has been created.

The library header-only.

Make sure to provide the <a
href="https://github.com/muwerk/ustd/blob/master/README.md">required platform
define</a> before including ustd headers.

## An example for dynamic mode:

~~~{.cpp}
#define __ATTINY__ 1  // Appropriate platform define required
#include <ustd_map.h>

ustd::map<int,double> myMap;

myMap[0] = 13.3; // Memory for map is allocated, if necessary
double p = myMap[0];
~~~

## An example for static mode

~~~{.cpp}
#define __ATTINY__ 1  // Appropriate platform define required
#include <ustd_map.h>

// map size is fixed 5 (startSize==maxSize), no dynamic extensions:
ustd::map<int, float> mayMap = ustd::map<int,float>(5, 5, 0, false);
~~~

## Iteration over keys

~~~{.cpp}
    ustd::map<int,double> myMap;
    myMap[0]=1.1;
    myMap[1]=1.2;
    for (auto i : myMap.keysArray()) {
        printf("%d->%f\n",i,myMap[i]);
    }
~~~
 */

template <class K, class V> class map {
  private:
    unsigned int size;
    unsigned int peakSize;
    unsigned int allocSize;
    unsigned int startSize;
    unsigned int maxSize;
    unsigned int incSize;
    bool shrink;
    V bad = {};

  public:
    ustd::array<K> keys;   /*! Array of keys */
    ustd::array<V> values; /*! Array of values */

  public:
    map(unsigned int startSize = ARRAY_INIT_SIZE, unsigned int maxSize = ARRAY_MAX_SIZE,
        unsigned int incSize = ARRAY_INC_SIZE, bool shrink = true)
        : startSize(startSize), maxSize(maxSize), incSize(incSize), shrink(shrink),
          keys(array<K>(startSize, maxSize, incSize, shrink)),
          values(array<V>(startSize, maxSize, incSize, shrink)) {
        /*!
         * Constructs a map object. All allocation-hints are optional, the
         * array class used by map will allocate memory as needed during writes,
         * if startSize!=maxSize.
         * @param startSize The number of map entries that are allocated
         * during object creation
         * @param maxSize The maximal limit of records that will be allocated.
         * If startSize < maxSize, the map size will grow automatically as
         * needed.
         * @param incSize The number of map entries that are allocated as a
         * chunk if the map needs to grow
         * @param shrink Boolean indicating, if the map should deallocate
         * memory, if the map size shrinks (due to erase()).
         */

        size = 0;
        allocSize = startSize;
    }

    /* Implicit, no need:
    map(const map &mp) {
        size = mp.size;
        peakSize = mp.peakSize;
        allocSize = mp.allocSize;
        startSize = mp.startSize;
        maxSize = mp.maxSize;
        incSize = mp.incSize;
        shrink = mp.shrink;
        bad = mp.bad;
        keys = mp.keys;
        values = mp.values;
    } */

    ~map() {
        /*! Free resources */
    }

    V operator[](K key) const {
        /*! Read value of map for given key, a=myMap[3].
        @param key map-key
        @return Corresponding value. The value set be setInvalidValue() is given
        back for invalid reads (or by default a value set to zero) */
        for (unsigned int i = 0; i < keys.length(); i++) {
            if (keys[i] == key)
                return values[i];
        }
        return bad;
    }

    V &operator[](K key) {
        /*! Write a map value for a given key
        @param key map-key
        @return value on success, or setInvalidValue() on error (e.g. map full)
      */
        for (unsigned int i = 0; i < keys.length(); i++) {
            if (keys[i] == key) {
                return values[i];
            }
        }
        int i = keys.add(key);
        if (i == -1) {
            return bad;
        }
        if (i >= 0) {
            size++;
            return values[i];
        }
        return bad;
    }

    int find(K key) {
        /*! Get the index of the key and value arrays of the map
        @param key Map-key.
        @return index, if found, -1 on error */
        for (unsigned int i = 0; i < keys.length(); i++) {
            if (keys[i] == key)
                return i;
        }
        return -1;
    }

    int erase(K key) {
        /*! Delete the entry corresponding to map-key. This might lead to
        memory-deallocation, if shrink=True during map creation
        @param key Map-key of entry to be deleted
        @return index of entry been deleted or -1 on error */
        for (unsigned int i = 0; i < keys.length(); i++) {
            if (keys[i] == key) {
                values.erase(i);
                keys.erase(i);
                return i;
            }
        }
        return -1;
    }

    void setInvalidValue(V &entryInvalidValue) {
        /*! Set the value that's given back, if read of an invalid
        key is requested. By default, an entry all set to zero is given back.
        Using this function, the value of an invalid read can be configured.
        * @param entryInvalidValue The value that is given back in case an
        invalid operation (e.g. read out of invalid key) is tried.
        */
        bad = entryInvalidValue;
    }

    bool isEmpty() {
        /*! Check, if map is empty.
        @return boolean true on empty map */
        if (size == 0)
            return true;
        else
            return false;
    }

    const ustd::array<K> &keysArray() {
        /*! Reference to array of keys

        @return const reference to array of keys, e.g. for iteration.
        */
        return keys;
    }

    unsigned int length() {
        /*! Check number of map-members.
        @return number of map entries */
        return (size);
    }

    unsigned int peak() {
        /*! Check peak number of map-members.
        @return maximum number members the map had since creation */
        return (peakSize);
    }
};
}  // namespace ustd
