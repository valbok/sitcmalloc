/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#ifndef SITCMALLOC_FREE_LIST_H
#define SITCMALLOC_FREE_LIST_H

#include <stddef.h> // for size_t, nullptr, ptrdiff_t
#include "common.h"

namespace sitcmalloc {

/**
 * Singly linked list of blocks.
 * Is used to fetch available cached chunk of memory (block).
 */
struct FreeList {

    /**
     * Default constructor
     */
    FreeList() : m_len(0), m_initLen(0), m_start(nullptr), m_end(nullptr), m_waterline(~0) {}

    /**
     * Prepends blocks of start-end range to current list.
     */
    inline void prepend(size_t num, Block* start, Block* end) {
        m_len += num;
        if (m_start) {
            m_end->setNext(start);
        } else {
            m_initLen = num;
            m_start = start;
        }
        m_end = end ? end : start;
        m_end->setNext(nullptr);
    }

    /**
     * Pops chunk of bytes from current list.
     * @return Address of memory
     */
    inline void* pop() {
        void* result = m_start;
        if (m_len > 0 ) {
            --m_len;
            if (m_len < m_waterline) {
                m_waterline = m_len;
            }
        }

        if (m_start) {
            m_start = m_start->next();
        }

        return result;
    }

    /**
     * @return true If no blocks available in the list.
     */
    bool empty() const {
        return m_start == nullptr;
    }

    /**
     * Current number of blocks in the list.
     */
    size_t m_len;

    /**
     * Number of blocks moved from a span.
     * Currently all blocks are fetched from a span and pushed to the list.
     */
    size_t m_initLen;

    /**
     * Root block of data.
     */
    Block* m_start;

    /**
     * End block of data.
     */
    Block* m_end;

    /**
     * Minimum number of blocks that left after returning to user.
     * 0 means all blocks have been allocated and used.
     * initLen - no blocks allocated.
     */
    size_t m_waterline;
};


}  // namespace sitcmalloc

#endif  // SITCMALLOC_FREE_LIST_H
