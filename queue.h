// queue.h - ustd queue class

#pragma once

namespace ustd {

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
        quePtr0 = 0;
        quePtr1 = 0;
        size = 0;
        peakSize = 0;
        que = (T **)malloc(sizeof(T *) * maxSize);
        if (que == nullptr)
            maxSize = 0;
    }

    ~queue() {
        if (que != nullptr) {
            // If size > 0 then there's a potential memory leak.
            // This must be taken care of by the queue owner.
            free(que);
            que = nullptr;
        }
    }

    bool push(T *ent) {
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
        if (size == 0)
            return nullptr;
        T *pEnt = que[quePtr0];
        quePtr0 = (quePtr0 + 1) % maxSize;
        --size;
        return pEnt;
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
