// queue.h - ustd queue class

#pragma once

namespace ustd {

/*! \brief Lightweight c++11 ring buffer queue implementation.

queue.h is a minimal, yet highly portable ring buffer queu implementation
that runs well on architectures with very limited resources such as attiny 8kb
avr.

## An example:

~~~{.cpp}
#include <queue.h>

queue<int> que = queue<int>(128);

int *wi=new int(1);
que.push(wi);
int *spi=que.pop();
printf("%d\n",*pi);
delete(pi);
~~~

Note: the queue interface is somewhat inconsistent in usage of T vs. T* and
might be changed in the future.
*/

// TODO: change interface so that <T> in class definition and <T>* in push/pop
// are same type. (This requires test with avr compiler flavour for attiny!)

template <class T> class queue {
  private:
    T **que;
    unsigned int peakSize;
    unsigned int maxSize;
    unsigned int size;
    unsigned int quePtr0;
    unsigned int quePtr1;

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
        que = (T **)malloc(sizeof(T *) * maxSize);
        if (que == nullptr)
            maxSize = 0;
    }

    ~queue() {
        /*!
        Deallocate the queue structure. This doesn't free any queue entry
        pointers.
        */
        if (que != nullptr) {
            // If size > 0 then there's a potential memory leak.
            // This must be taken care of by the queue owner.
            free(que);
            que = nullptr;
        }
    }

    bool push(T *ent) {
        /*! Push a new entry into the queue.
        @param ent Pointer to a T element
        @return true on success, false if queue is full.
        */
        if (size >= maxSize) {
            return false;
        }
        if (ent != nullptr) {
            que[quePtr1] = ent;
            quePtr1 = (quePtr1 + 1) % maxSize;
            ++size;
        }
        if (size > peakSize) {
            peakSize = size;
        }
        return true;
    }

    T *pop() {
        /*! Pop the oldest entry from the queue.
        Freeing memory that might have been alloctated for the entry before
        push() is up to the user.
        @return nullptr if queue is empty, or T* otherwise.
        */
        if (size == 0)
            return nullptr;
        T *pEnt = que[quePtr0];
        quePtr0 = (quePtr0 + 1) % maxSize;
        --size;
        return pEnt;
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
