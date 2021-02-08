// ustd_queue.h - ustd queue class

#pragma once

namespace ustd {

/*! \brief Lightweight c++11 ring buffer queue implementation.

ustd_queue.h is a minimal, yet highly portable ring buffer queu implementation
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
        @param maxQueueSize The maximum number of <T> pointer entries, the queue
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
        /*! Set the value for <T>entry that's given back, if read from an empty
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
