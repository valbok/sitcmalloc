#include "PageHeap.h"
//#include "common.h"
#include "sys_alloc.h"

namespace sitcmalloc {

Span* PageHeap::search(size_t pages) {

}

Span* PageHeap::allocFromSystem(size_t pages) {
    size_t ask = pages > MIN_ALLOCATED_PAGES ? pages : MIN_ALLOCATED_PAGES;
    void* ptr = sys_alloc(pagesToBytes(pages));
    if (!ptr) {
        return NULL;
    }



}

Span* PageHeap::alloc(size_t pages) {
    Span* result = search(pages);
}

void PageHeap::free(Span* span) {

}

}  // namespace sitcmalloc
