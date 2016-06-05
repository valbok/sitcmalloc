/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#ifndef SITCMALLOC_SPAN_H
#define SITCMALLOC_SPAN_H

#include <stddef.h> // for size_t, nullptr, ptrdiff_t
#include "common.h"

namespace sitcmalloc {

class Block;

/**
 * Represents contiguous pages of memory.
 */
class Span {
public:

    /**
     * Creates span object based on prodived
     * already allocated memory.
     *
     * @param Memory where span should be placed.
     * @param Number of pages to associate with a span.
     * @return nullptr if error.
     */
    static Span* create(void* p, size_t pages);

    /**
     * Default constructor is used for root nodes of linked lists.
     */
    Span(size_t pages = 0);

    /**
     * @return Pages of the span.
     */
    inline size_t pages() const {
        return m_pages;
    }

    /**
     * Sets pages to the span.
     */
    inline void setPages(size_t pages) {
        m_pages = pages;
    }

    /**
     * @return First block memory.
     */
    inline Block* block() {
        return reinterpret_cast<Block*>(data());
    }

    /**
     * @return Next span in doubly linked list.
     */
    inline Span* next() const {
        return m_next;
    }

    /**
     * @return Prev span in doubly linked list.
     */
    inline Span* prev() const {
        return m_prev;
    }

    /**
     * Prepends span to current.
     */
    void prepend(Span* span);

    /**
     * Removes current span from doubly linked list.
     */
    void remove();

    /**
     * @return True if the span does not contain any
     *         following spans in the list.
     */
    inline bool empty() const {
        return m_prev == nullptr && m_next == nullptr;
    }

    /**
     * Removes next located span from the list and returns it.
     */
    inline Span* pop() {
        Span* next = m_next;
        if (next) {
            next->remove();
        }
        return next;
    }

    /**
     * Carves current memory by pages and returns
     * exactly requests number of pages.
     *
     * | this   | result with requested pages |
     * |--------|+++++++++++++++++++++++++++++|
     * @return span with requests pages.
     */
    Span* carve(size_t pages);

    /**
     * Makes current span is under using.
     */
    inline void use() {
        m_inUse = 1;
    }

    /**
     * Makes current span as not under using.
     */
    inline void free() {
        m_inUse = 0;
    }

    /**
     * @return true if the span is under using.
     */
    inline bool inUse() const {
        return m_inUse;
    }

    /**
     * @return Associated size class
     */
    inline size_t sizeClass() const {
        return m_sizeClass;
    }

    /**
     * @return Size of the span in bytes.
     */
    inline size_t size() const {
        return pagesToBytes(m_pages);
    }

    /**
     * Decreases number of allocated blocks.
     * If 0 means all blocks have been freed
     * and current span could also be freed.
     * Span could not be freed before all blocks returned from user.
     *
     * @return Number of allocated blocks.
     */
    inline unsigned freeBlock() {
        return m_blocks > 0 ? --m_blocks : 0;
    }

    /**
     * Splits current span by size.
     * Creats single linked list of blocks.
     * Before: |------------------|
     *          size size size
     * After   1----2----3----4---|
     * There are 4 blocks: 1 points to 2, 2 -> 3, 3 -> 4 and 4 -> nullptr
     *
     * @param Size to split by.
     * @param Requested size class.
     * @param[out] Pointer to start/first block.
     * @param[out] Pointer to end/last block. Last block never has following blocks.
     * @return Number of blocks created.
     */
    size_t split(size_t size, size_t sizeClass, Block** start, Block** end);

private:

    /**
     * @return Pointer to place where data is located.
     */
    inline void* data() {
        return &m_prev;
    }

    /**
     * Number of pages.
     */
    size_t m_pages;

    /**
     * Number of allocated blocks to user.
     */
    unsigned int m_blocks : 16;

    /**
     * Size class.
     */
    unsigned char m_sizeClass : 7;

    /**
     * Is current span in use,
     * means has been allocated.
     */
    unsigned char m_inUse : 1;

    /**
     * Previous span in doubly linked list.
     */
    Span* m_prev;

    /**
     * Next span in doubly linked list.
     */
    Span* m_next;
};

}  // namespace sitcmalloc

#endif  // SITCMALLOC_SPAN_H
