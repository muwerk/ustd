// ustd_queue.h - ustd queue class

#pragma once

namespace ustd {

// Helper class for queue iterators:
template <typename T> class queueIterator {
  private:
    T *values_ptr;
    unsigned int position;
    unsigned int maxSize;

  public:
    queueIterator(T *values_ptr, unsigned int p, unsigned int maxSize)
        : values_ptr{values_ptr}, position{p}, maxSize(maxSize) {
    }

    bool operator!=(const queueIterator<T> &other) const {
        return !(*this == other);
    }

    bool operator==(const queueIterator<T> &other) const {
        return position == other.position;
    }

    queueIterator &operator++() {
        position = (position + 1) % maxSize;
        return *this;
    }

    T &operator*() const {
        return *(values_ptr + position);
    }
};

/*! \brief Lightweight c++11 ring buffer queue implementation.

ustd_queue.h is a minimal, yet highly portable ring buffer queue implementation
that runs well on architectures with very limited resources such as attiny 8kb
avr.

Make sure to provide the <a
href="https://github.com/muwerk/ustd/blob/master/README.md">required platform
define</a> before including ustd headers.

## An example:

~~~{.cpp}
#define __ATTINY__ 1  // Appropriate platform define required
#include <ustd_queue.h>

queue<int> que = queue<int>(128);

int wi;
wi=1;
que.push(wi);
int wo;
wo=que.pop();
printf("%d\n",wo);
~~~

## Queue inspection with iterators

queue<int> que = queue<int>(16);

que.push(12);
que.push(13);

// Iterate through queue content (does not modify content)
for (auto i : que) {
    printf("%d\n", i);
}

// New pop() the values:
int w0,w1;
w0=que.pop();
w1=que.pop();
// Queue is now empty.

printf("%d %d, len=%d\n",w0,w1,que.length());
*/

template <class T> class queue {
  private:
    T *que;
    unsigned int peakSize;
    unsigned int maxSize;
    unsigned int size;
    unsigned int quePtr0;
    unsigned int quePtr1;
    T bad = {};

  public:
    queue(unsigned int maxQueueSize) : maxSize(maxQueueSize) {
        /*! Constructs a queue object
        @param maxQueueSize The maximum number of entries, the queue
        can hold.
        */
        quePtr0 = 0;
        quePtr1 = 0;
        size = 0;
        peakSize = 0;
        que = (T *)malloc(sizeof(T) * maxSize);
        if (que == nullptr)
            maxSize = 0;
    }

    queue(const queue &qu) {
        peakSize = qu.peakSize;
        maxSize = qu.maxSize;
        size = qu.size;
        quePtr0 = qu.quePtr0;
        quePtr1 = qu.quePtr1;
        bad = qu.bad;
        que = (T *)malloc(sizeof(T) * maxSize);
        if (que == nullptr) {
            maxSize = 0;
            size = 0;
        } else {
            unsigned int in = quePtr0;
            for (unsigned int i = 0; i < size; i++) {
                que[in] = qu.que[in];
                in = (in + 1) % maxSize;
            }
        }
    }

    ~queue() {
        /*!
        Deallocate the queue structure.
        */
        if (que != nullptr) {
            free(que);
            que = nullptr;
        }
    }

    // iterators
    queueIterator<T> begin() {
        /*! Iterator support: begin() */
        return queueIterator<T>(que, quePtr0, maxSize);
    }
    queueIterator<T> end() {
        /*! Iterator support: end() */
        return queueIterator<T>(que, (quePtr0 + size) % maxSize, maxSize);
    }

    queueIterator<const T> begin() const {
        /*! Iterator support: begin() */
        return queueIterator<const T>(que, quePtr0, maxSize);
    }

    queueIterator<const T> end() const {
        /*! Iterator support: end() */
        return queueIterator<const T>(que, (quePtr0 + size) % maxSize, maxSize);
    }

    void getInternalStartStopPtrs(unsigned int *p0, unsigned int *p1) {
        *p0 = quePtr0;
        *p1 = quePtr1;
    }

    bool push(T ent) {
        /*! Push a new entry into the queue.
        @param ent T element
        @return true on success, false if queue is full.
        */
        if (size >= maxSize) {
            return false;
        }
        que[quePtr1] = ent;
        quePtr1 = (quePtr1 + 1) % maxSize;
        ++size;
        if (size > peakSize) {
            peakSize = size;
        }
        return true;
    }

    T pop() {
        /*! Pop the oldest entry from the queue.
        @return badEntry if queue is empty, or T element otherwise.
        */
        if (size == 0)
            return bad;
        T ent = que[quePtr0];
        quePtr0 = (quePtr0 + 1) % maxSize;
        --size;
        return ent;
    }

    void setInvalidValue(T &entryInvalidValue) {
        /*! Set the value that's given back, if read from an empty
        queue is requested. By default, an entry all set to zero is given
        back. Using this function, the value of an invalid read can be
        configured.
        * @param entryInvalidValue The value that is given back in case an
        invalid operation (e.g. read out of bounds) is tried.
        */
        bad = entryInvalidValue;
    }

    bool isEmpty() {
        /*! Check, if queue is empty.
        @return true: queue empty, false: not empty.
        */
        if (size == 0)
            return true;
        else
            return false;
    }

    unsigned int length() {
        /*! Check number of queue entries.
        @return number of entries in the queue.
        */
        return (size);
    }

    unsigned int peak() {
        /*! Check the maxiumum number of entries that have been in the queue.
        @return max number of queue entries.
         */
        return (peakSize);
    }
};
}  // namespace ustd
