#ifndef SITCMALLOC_FREE_LIST_H
#define SITCMALLOC_FREE_LIST_H

#include <stddef.h> // for size_t, nullptr, ptrdiff_t
#include "common.h"
#include <iostream>
using namespace std;
namespace sitcmalloc {

class FreeList {
public:
    FreeList() : m_len(0), m_maxLen(0), m_start(nullptr), m_end(nullptr) {}
    inline void prepend(size_t num, Block* block, Block* end) {
        m_len += num;
        if (!m_start) {
            m_maxLen = num;
            m_start = block;
            m_end = end;
        } else {
            m_end->next(block);
            m_end = end ? end : block;
        }

    }
    inline void* pop() {
        void* result = m_start;
        if (m_len > 0 ) {
            --m_len;
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

    size_t maxLength() const {
        return m_maxLen;
    }
private:
    size_t m_len;
    size_t m_maxLen;
    Block* m_start;
    Block* m_end;
};

}  // namespace sitcmalloc

#endif  // SITCMALLOC_FREE_LIST_H
