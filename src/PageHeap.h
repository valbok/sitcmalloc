#ifndef SITCMALLOC_PAGEHEAP_H
#define SITCMALLOC_PAGEHEAP_H

#include <stddef.h> // for size_t, NULL, ptrdiff_t
#include "Span.h"
#include "common.h"

namespace sitcmalloc {


/**
 *
 */
class PageHeap {
public:


    /**
     *
     */
    Span* alloc(size_t);

    /**
     *
     */
    void free(Span*);


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
    void merge(Span*);

    Span m_pageSpans[MAX_PAGES];
    Span m_span;
    Span m_tail;

};



}  // namespace sitcmalloc

#endif  // SITCMALLOC_PAGEHEAP_H
