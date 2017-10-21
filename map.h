// map.h - ustd queue class

#pragma once

namespace ustd {

#define MAX_MAP_SIZE UINT_MAX

template <class K, class V> class map {
  private:
    unsigned int size;
    unsigned int peakSize;
    unsigned int maxSize;

  public:
    map(unsigned int maxMapSize = MAX_MAP_SIZE) : maxSize(maxMapSize) {
        size = 0;
        peakSize = 0;
    }

    ~map() {
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
}
