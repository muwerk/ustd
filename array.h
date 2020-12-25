// array.h - array class

#pragma once

/*! \mainpage Ustd micro-standard-library for low-resource AVRs
\section Introduction
ustd provides minimal and highly portable implementations for:

* * \ref ustd::array<T>
* * \ref ustd::queue<T>
* * \ref ustd::map<K,V>

classes.

In additional, for Atmel AVRs a drop-in replacement for std::function<>
is provided as ustd::function<>

* * \ref functional.h

The libraries are header-only and should work with any c++11 compiler
and support platforms starting with 8k attiny, avr, arduinos, up to esp8266,
esp32 and mac and linux.

Make sure to provide the <a
href="https://github.com/muwerk/ustd/blob/master/README.md">required platform
define</a> before including ustd headers.

\section Reference
<a href="https://github.com/muwerk/ustd">ustd github repository</a>
*/

#include "platform.h"

namespace ustd {

#define ARRAY_INC_SIZE 16
#define ARRAY_MAX_SIZE UINT_MAX  // 65535 or 4294967295 (mostly)
#define ARRAY_INIT_SIZE 16

/*! \brief Lightweight c++11 array implementation.

array.h is a minimal, yet highly portable array data type implementation
that runs well on architectures with very limited resources such as attiny 8kb
avr.

The array class either:

* * allocates memory dynamically on array-writes or array-reads, or
* * work in fully static mode without any dynamic allocation once the array
object has been created.

The library header-only.

Make sure to provide the <a
href="https://github.com/muwerk/ustd/blob/master/README.md">required platform
define</a> before including ustd headers.

## An example for dynamic mode:

~~~{.cpp}
#define __ATTINY__ 1   // Platform defines required, see doc, mainpage.
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

template <typename T> class array {

  private:
    T *arr;
    unsigned int startSize;
    unsigned int maxSize;
    unsigned int incSize = ARRAY_INC_SIZE;
    bool shrink = true;
    unsigned int allocSize;
    unsigned int size;
    T bad;

    T *ualloc(unsigned int n) {
#if defined(__ATTINY__)
        return (T *)malloc(n * sizeof(T));
#else
        return new T[n];
#endif
    }
    void ufree(T *p) {
#if defined(__ATTINY__)
        free(p);
#else
        delete[] p;
#endif
    }

  public:
    array(unsigned int startSize = ARRAY_INIT_SIZE, unsigned int maxSize = ARRAY_MAX_SIZE,
          unsigned int incSize = ARRAY_INC_SIZE, bool shrink = true)
        : startSize(startSize), maxSize(maxSize), incSize(incSize), shrink(shrink) {
        /*!
         * Constructs an array object. All allocation-hints are optional, the
         * array class will allocate memory as needed during writes, if
         * startSize!=maxSize.
         * @param startSize The number of array entries that are allocated
         * during object creation
         * @param maxSize The maximal limit of records that will be allocated.
         * If startSize < maxSize, the array size will grow automatically as
         * needed.
         * @param incSize The number of array entries that are allocated as a
         * chunk if the array needs to grow
         * @param shrink Boolean indicating, if the array should deallocate
         * memory, if the array size shrinks (due to erase()).
         */
        size = 0;
        memset(&bad, 0, sizeof(bad));
        if (maxSize < startSize)
            maxSize = startSize;
        allocSize = startSize;
        arr = ualloc(allocSize);  // new T[allocSize];
    }

    ~array() {
        /*! Free resources */
        if (arr != nullptr) {
            ufree(arr);
            arr = nullptr;
        }
    }

    bool resize(unsigned int newSize) {
        /*! Change the array allocation size.
         *
         * Note: Usage of this function is optional for optimization. By
         * default, all necessary allocations (and deallocations, if shrink=true
         * during construction was set) are handled automatically.
         * @param newSize the new number of array entries, corresponding memory
         * is allocated/freed as necessary.
         */
        unsigned int mv = newSize;
        if (newSize > maxSize) {
            if (maxSize == allocSize)
                return false;
            else
                newSize = maxSize;
        }
        if (!shrink) {
            if (newSize <= allocSize)
                return true;
        } else {
            if (newSize < allocSize)
                mv = newSize;
            else
                mv = allocSize;
        }
        T *arrn = ualloc(newSize);  // new T[newSize];
        if (arrn == nullptr)
            return false;
        for (unsigned int i = 0; i < mv; i++) {
            arrn[i] = arr[i];
        }
        ufree(arr);
        arr = arrn;
        allocSize = newSize;
        return true;
    }

    void setInvalidValue(T &entryInvalidValue) {
        /*! Set the value for <T>entry that's given back, if read of an invalid
        index is requested. By default, an entry all memset to zero is given
        back. Using this function, the value of an invalid read can be
        configured.
        * @param entryInvalidValue The value that is given back in case an
        invalid operation (e.g. read out of bounds) is tried.
        */
        bad = entryInvalidValue;
    }

    int add(T &entry) {
        /*! Append an array element after the current end of the array
         * @param entry array element that is appended after the last current
         * entry. The new array size must be smaller than maxSize as defined
         * during array creation. New array memory is automatically allocated if
         * within maxSize boundaries. */
        if (size >= allocSize) {
            if (incSize == 0)
                return -1;
            if (!resize(allocSize + incSize))
                return -1;
        }
        arr[size] = entry;
        ++size;
        return size - 1;
    }

    bool erase(unsigned int index) {
        /*! Delete array element at given index
         * @param index The array index of the element to be erased. The array
         * size is reduced by 1, and memory might be freed, if shrink=True
         * during array creation.
         */
        if (index >= size) {
            return false;
        }
        for (unsigned int i = index; i < size - 1; i++) {
            arr[i] = arr[i + 1];
        }
        --size;
        if (shrink) {
            if ((size < allocSize - incSize) && (allocSize > incSize)) {
                resize(allocSize - incSize);
            }
        }
        return true;
    }

    T operator[](unsigned int i) const {
        /*! Read content of array element at i, a=myArray[3] */
        if (i >= allocSize) {
            if (incSize == 0) {
#ifdef USTD_ASSERT
                assert(i < allocSize);
#endif
            }
            if (!resize(allocSize + incSize)) {
#ifdef USTD_ASSERT
                assert(i < allocSize);
#endif
            }
        }
        if (i >= size && i <= allocSize)
            size = i + 1;
        if (i >= allocSize) {
            return bad;
        }
        return arr[i];
    }

    T &operator[](unsigned int i) {
        /*! Assign content of array element at i, e.g. myArray[3]=3 */
        if (i >= allocSize) {
            if (incSize == 0) {
#ifdef USTD_ASSERT
                assert(i < allocSize);
#endif
            }
            if (!resize(allocSize + incSize)) {
#ifdef USTD_ASSERT
                assert(i < allocSize);
#endif
            }
        }
        if (i >= size && i <= allocSize)
            size = i + 1;
        if (i >= allocSize) {
            return bad;
        }
        return arr[i];
    }

    bool isEmpty() const {
        /*! Check, if array is empty.
        @return true if array empty, false otherwise. s*/
        if (size == 0)
            return true;
        else
            return false;
    }

    unsigned int length() const {
        /*! Check number of array-members.
        @return number of array entries */
        return (size);
    }
    unsigned int alloclen() const {
        /*! Check the number of allocated array-entries, which can be larger
         * than the length of the array.
         * @return number of allocated entries. */
        return (allocSize);
    }
};
}  // namespace ustd
