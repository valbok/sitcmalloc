#ifndef SITCMALLOC_PAGEHEAP_H
#define SITCMALLOC_PAGEHEAP_H

#include <stddef.h> // for size_t, NULL, ptrdiff_t
#include "Span.h"
#include "PageMap.h"
#include <mutex>

namespace sitcmalloc {


/**
 *
 */
class PageHeap {
public:


    /**
     * @param pages
     */
    Span* alloc(size_t);

    /**
     *
     */
    bool free(Span*);

    static inline Span* span(void* ptr) {
        return reinterpret_cast<Span*>(PageMap::get(ptr));
    }

    static inline Span* span(uintptr_t key) {
        return reinterpret_cast<Span*>(PageMap::get(key));
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
    void merge(Span*);

    Span m_pageSpans[MAX_PAGES];
    Span m_largeSpan;

    std::mutex m_mutex;
};



}  // namespace sitcmalloc

#endif  // SITCMALLOC_PAGEHEAP_H
