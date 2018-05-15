// array.h - array class

#pragma once

#include "platform.h"

namespace ustd {

#define ARRAY_INC_SIZE 16
#define ARRAY_MAX_SIZE UINT_MAX  // 65535 or 4294967295 (mostly)
#define ARRAY_INIT_SIZE 16

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
    array(unsigned int startSize = ARRAY_INIT_SIZE,
          unsigned int maxSize = ARRAY_MAX_SIZE,
          unsigned int incSize = ARRAY_INC_SIZE, bool shrink = true)
        : startSize(startSize), maxSize(maxSize), incSize(incSize),
          shrink(shrink) {
        size = 0;
        if (maxSize < startSize)
            maxSize = startSize;
        allocSize = startSize;
        arr = ualloc(allocSize);  // new T[allocSize];
    }

    ~array() {
        if (arr != nullptr) {
            ufree(arr);
            arr = nullptr;
        }
    }

    bool resize(unsigned int newSize) {
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
        if (size == 0)
            return true;
        else
            return false;
    }

    unsigned int length() const {
        return (size);
    }
    unsigned int alloclen() const {
        return (allocSize);
    }
};
}  // namespace ustd
