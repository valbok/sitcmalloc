#ifndef SITCMALLOC_FREE_LIST_H
#define SITCMALLOC_FREE_LIST_H

#include <stddef.h> // for size_t, nullptr, ptrdiff_t
#include "common.h"
#include <iostream>
using namespace std;

namespace sitcmalloc {

class FreeList {
public:
    FreeList() : m_len(0), m_initLen(0), m_start(nullptr), m_end(nullptr), m_popped(false) {}
    inline void prepend(size_t num, Block* start, Block* end) {
        m_len += num;
        m_end = end ? end : start;
        if (m_start) {
            m_end->next(start);
        } else {
            m_initLen = num;
            m_start = start;
        }
    }

    inline void* pop() {
        void* result = m_start;
        if (m_len > 0 ) {
            --m_len;
        } else {
            m_popped = true;
        }

        if (m_start) {
            m_start = m_start->next();
        }
        return result;
    }

    bool empty() const {
        return m_start == nullptr;
    }

    size_t length() const {
        return m_len;
    }

    Block* end() const {
        return m_end;
    }

    size_t initLength() const {
        return m_initLen;
    }

    bool popped() const {
        return m_popped;
    }

private:
    size_t m_len;
    size_t m_initLen;
    Block* m_start;
    Block* m_end;
    bool m_popped;
};

}  // namespace sitcmalloc

#endif  // SITCMALLOC_FREE_LIST_H
