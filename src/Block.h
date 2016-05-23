#ifndef SITCMALLOC_BLOCK_H
#define SITCMALLOC_BLOCK_H

#include <stddef.h> // for size_t, nullptr, ptrdiff_t
#include "common.h"
#include <iostream>
namespace sitcmalloc {

/**
 *
 */
class Block {
public:
    Block() : m_next(nullptr) {}

    inline Block* next() const {
       return m_next;
    }

    size_t split(void* limit, size_t size) {
        ASSERT(size >= sizeof(Block*));
        void** tail = reinterpret_cast<void**>(&m_next);
        char* start = reinterpret_cast<char*>(&m_next);
        size_t num = 0;
        while (start + size <= limit) {
            *tail = start;
            tail = reinterpret_cast<void**>(start);
            start += size;
            ++num;
        }
        *tail = nullptr;
        ASSERT(start <= limit);

        return num;
    }

    inline void prepend(Block* block) {
        if (!block) {
            return;
        }

        block->m_next = m_next;
        m_next = block;
    }

    inline void remove(Block* prev) {
        if (prev) {
            prev->m_next = m_next;    
        }
        m_next = nullptr;
    }

    inline Block* pop() {
        Block* next = m_next;
        if (m_next) {
            m_next->remove(this);
        }
        return next;
    }

    inline bool empty() const {
        return m_next == nullptr;
    }

private:
    
    Block(const Block&);
    Block& operator=(const Block&);

    Block* m_next;
};


}  // namespace sitcmalloc

#endif  // SITCMALLOC_BLOCK_H
