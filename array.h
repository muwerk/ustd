// array.h - array class

#pragma once

#include "platform.h"

namespace ustd {

#define ARRAY_INC_SIZE 16
#define ARRAY_MAX_SIZE UINT_MAX  // 65535 or 4294967295 (mostly)
#define ARRAY_INIT_SIZE 16

/*! \brief Lightweight c++ array implementation.

array.h is a minimal, yet highly portable array data type implementation
that runs well on architectures with very limited resources.

The array class either:

* * allocates memory dynamically on array-writes or array-reads, or
* * work in fully static mode without any dynamic allocation once the array
object has been created.

The library header-only.

## An example for dynamic mode:

~~~{.cpp}
#include <array.h>

ustd::array<int> intArray;

intArray[0]=13; // Memory for array[0] is allocated
intArray.add(3);  // the array is extended, if necessary
int p=intArray[0];

printf("[0]:%d [1]:%d length=%d\n",intArray[0],intArray[1].intArray.length())
~~~

## An example for static mode

~~~{.cpp}
#include <array.h>

ustd::array<int> intArray=ustd::array<int>(5,5,0,false);  // array length is
fixed 5

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
#ifdef __ATTINY__
        return (T *)malloc(n * sizeof(T));
#else
        return new T[n];
#endif
    }
    void ufree(T *p) {
#ifdef __ATTINY__
        free(p);
#else
        delete[] p;
#endif
    }

  public:
    array(unsigned int startSize =
              ARRAY_INIT_SIZE, /*!< Initial allocation-size for the array */
          unsigned int maxSize =
              ARRAY_MAX_SIZE, /*!< Maximum allowed size for the array */
          unsigned int incSize =
              ARRAY_INC_SIZE, /*!< Step-size for allocation-increments */
          bool shrink =
              true /*!< allow arrays to shrink (deallocate unneeded entries) */)
        : startSize(startSize), maxSize(maxSize), incSize(incSize),
          shrink(shrink) {
        /*!
         * Constructs an array object. All allocation-hints are optional, the
         * array class will allocate memory as needed during writes, if
         * startSize!=maxSize.
         */
        size = 0;
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

    int add(T &ent) {
        /*! Append an array element after the current end of the array */
        if (size >= allocSize) {
            if (incSize == 0)
                return -1;
            if (!resize(allocSize + incSize))
                return -1;
        }
        arr[size] = ent;
        ++size;
        return size - 1;
    }

    bool erase(unsigned int index) {
        /*! Delete array element at given index */
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
            memset(&bad, 0, sizeof(bad));
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
            memset(&bad, 0, sizeof(bad));
            return bad;
        }
        return arr[i];
    }

    bool isEmpty() const {
        /*! Return true, if array is empty. */
        if (size == 0)
            return true;
        else
            return false;
    }

    unsigned int length() const {
        /*! returns the number of array-members */
        return (size);
    }
    unsigned int alloclen() const {
        /*! Return the number of allocated array-entries, which can be larger
         * than the length of the array. */
        return (allocSize);
    }
};
}  // namespace ustd
