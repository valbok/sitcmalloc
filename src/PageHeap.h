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


    /**
     *
     */
    bool alloc(size_t, Span&);

    /**
     *
     */
    bool free(Span&);


    static PageHeap& instance() {
        static PageHeap result;
        return result;
    }


private:
    PageHeap() {}
    PageHeap(PageHeap&);
    PageHeap& operator=(const PageHeap&);

    bool search(size_t, Span&);
    bool allocFromSystem(size_t, Span&);

};


}  // namespace sitcmalloc

#endif  // SITCMALLOC_PAGEHEAP_H
