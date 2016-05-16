#ifndef SITCMALLOC_PAGEHEAP_H
#define SITCMALLOC_PAGEHEAP_H

#include <stddef.h> // for size_t, NULL, ptrdiff_t
#include "Span.h"

namespace sitcmalloc {


/**
 *
 */
class PageHeap {
public:

    static const size_t PAGE_SHIFT = 13;
    static const size_t MAX_PAGES = 1 << (20 - PAGE_SHIFT);
    static const size_t MIN_ALLOCATED_PAGES = MAX_PAGES;

    /**
     *
     */
    Span* alloc(size_t);

    /**
     *
     */
    void free(Span*);

    static inline size_t pagesToBytes(size_t pages) {
        return pages << PAGE_SHIFT;
    }

    static PageHeap& instance() {
        static PageHeap result;
        return result;
    }


private:
    PageHeap() {}
    PageHeap(PageHeap&);
    PageHeap& operator=(const PageHeap&);

    Span* search(size_t);
    Span* allocFromSystem(size_t);

};


}  // namespace sitcmalloc

#endif  // SITCMALLOC_PAGEHEAP_H
