#include "PageHeap.h"
//#include "common.h"
#include "sys_alloc.h"

namespace sitcmalloc {

bool PageHeap::search(size_t pages, Span& span) {

}

bool PageHeap::allocFromSystem(size_t pages, Span&) {
	if (pages < MIN_ALLOCATED_PAGES) {
    	pages = MIN_ALLOCATED_PAGES;
	}
    void* ptr = sys_alloc(pagesToBytes(pages));
    if (!ptr) {
        return false;
    }

    Span.init(ptr, pages);
    return true;
}

bool PageHeap::alloc(size_t pages, Span& span) {
    if (search(pages, span)) {
    	return true;
    }

    
}

bool PageHeap::free(Span& span) {

}

}  // namespace sitcmalloc
