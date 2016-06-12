/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#ifndef SITCMALLOC_PAGEHEAP_H
#define SITCMALLOC_PAGEHEAP_H

#include "Span.h"
#include "PageMap.h"
#include <mutex>
#include <stddef.h> // for size_t, NULL, ptrdiff_t

namespace sitcmalloc {

/**
 * Memory management handler that operates with pages only.
 */
class PageHeap {
public:

    /**
     * Allocates bytes.
     *
     * @param Number of pages to allocate
     * @return Span of requested pages.
     */
    Span* alloc(size_t);

    /**
     * Frees previously allocated span.
     * @return true If no errors.
     */
    bool free(Span*);

    /**
     * Returns cached span by address.
     */
    static inline Span* span(void* ptr) {
        return reinterpret_cast<Span*>(PageMap::get(ptr));
    }

    /**
     * Returns cached span by key or page number.
     */
    static inline Span* span(uintptr_t key) {
        return reinterpret_cast<Span*>(PageMap::get(key));
    }

    /**
     * Creates the handler
     */
    static PageHeap& instance() {
        static PageHeap result;
        return result;
    }

    /**
     * Returns size of previously allocated memory by pointer.
     */
    static size_t size(void* ptr) {
        Span* s = span(ptr);
        return s ? s->blockSize() : 0;
    }

private:
    PageHeap() = default;
    PageHeap(PageHeap&) = delete;
    PageHeap& operator=(const PageHeap&) = delete;

    /**
     * Fetches span by size if exists in cache.
     */
    Span* fetch(size_t);

    /**
     * Allocates span from the system by requested size.
     */
    Span* allocFromSystem(size_t);

    /**
     * Merges span to current caches by number of pages.
     */
    void merge(Span*);

    /**
     * Cached spans by number of pages.
     */
    Span m_pageSpans[MAX_PAGES];

    /**
     * All cached large spans.
     */
    Span m_largeSpan;

    /**
     * To synchronize accessing to the heap.
     */
    std::mutex m_mutex;
};

}  // namespace sitcmalloc

#endif  // SITCMALLOC_PAGEHEAP_H
