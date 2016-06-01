#ifndef SITCMALLOC_BLOCK_H
#define SITCMALLOC_BLOCK_H

#include <stddef.h> // for size_t, nullptr, ptrdiff_t
#include "common.h"
#include <iostream>
using namespace std;
namespace sitcmalloc {

/**
 *
 */
class Block {
public:
    Block() : m_next(nullptr) {}

    inline Block* next(Block* n = nullptr) {
        if (n) {
            m_next = n;
        }
        return m_next;
    }

    /**
     * @param[out] end Pointer to last block element
     *                 which does not have any succeeding objects.
     */
    size_t split(void* limit, size_t size, Block** end) {
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
        *end = reinterpret_cast<Block*>(tail);
        ASSERT(start <= limit);

        return num;
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
