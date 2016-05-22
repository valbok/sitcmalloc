#ifndef SITCMALLOC_BLOCK_H
#define SITCMALLOC_BLOCK_H

#include <stddef.h> // for size_t, nullptr, ptrdiff_t
#include "common.h"
namespace sitcmalloc {

/**
 *
 */
class Block {
public:

    inline Block* next() {
       return reinterpret_cast<Block*>(m_next);
    }

    size_t split(void* limit, size_t size) {
        void** tail = &m_next;
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
        if (!prev) {
            return;
        }
        prev->m_next = m_next;
        m_next = nullptr;
    }

private:
    Block() {}
    Block(const Block&);
    Block& operator=(const Block&);

    void* m_next;
};


}  // namespace sitcmalloc

#endif  // SITCMALLOC_BLOCK_H
