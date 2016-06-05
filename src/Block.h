/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#ifndef SITCMALLOC_BLOCK_H
#define SITCMALLOC_BLOCK_H

#include "common.h"
#include <stddef.h> // for size_t, nullptr, ptrdiff_t

namespace sitcmalloc {

/**
 * Block or chunk of memory with originally requested size.
 * Organized with other blocks as singly linked list.
 */
class Block {
public:

    /**
     * Default constructor.
     */
    Block() : m_next(nullptr) {}

    /**
     * Returns next block in sll.
     */
    inline Block* next() const {
        return m_next;
    }

    /**
     * Sets next block in sll.
     */
    inline void setNext(Block* block) {
        m_next = block;
    }

    /**
     * Splits memory to chunks/blocks and creates linked list.
     *
     * @param Last available address to stop splitting.
     * @param Size to split by.
     * @param[out] end Pointer to last block element
     *                 which does not have any succeeding objects.
     * @return Number of created blocks.
     */
    size_t split(void* limit, size_t size, Block** end) {
        ASSERT(size >= sizeof(Block*));
        Block** tail = reinterpret_cast<Block**>(&m_next);
        char* start = reinterpret_cast<char*>(&m_next);

        size_t num = 0;
        while (start + size <= limit) {
            *tail = (Block*)start;
            tail = reinterpret_cast<Block**>(start);
            start += size;
            ++num;
        }
        *tail = nullptr;
        *end = reinterpret_cast<Block*>(tail);

        ASSERT(start <= limit);
        ASSERT(*tail == nullptr);
        ASSERT((*end)->m_next == nullptr);

        return num;
    }

    /**
     * Checks if current block does not have next elements.
     */
    inline bool empty() const {
        return m_next == nullptr;
    }

private:

    Block(const Block&) = delete;
    Block& operator=(const Block&) = delete;

    /**
     * Next block in SLL;
     */
    Block* m_next;
};

}  // namespace sitcmalloc

#endif  // SITCMALLOC_BLOCK_H
