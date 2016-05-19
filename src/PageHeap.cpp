#include "PageHeap.h"
#include "sys_alloc.h"
#include <unistd.h>
#include <iostream>
using namespace std;
namespace sitcmalloc {


Span* PageHeap::allocFromSystem(size_t pages) {
	if (pages < MIN_ALLOCATED_PAGES) {
    	pages = MIN_ALLOCATED_PAGES;
	}
    void* ptr = sys_alloc(pagesToBytes(pages));

    return Span::create(ptr, pages);
}

Span* PageHeap::search(size_t pages) {
    Span* result = nullptr;
    for (unsigned i = pages - 1; i < MAX_PAGES; ++i) {
        Span* root = &m_pageSpans[i];
        if (!root->vEmpty()) {
            Span* span = root->vNext();

            span->vRemove();
            result = span->carve(pages);

            merge(span);
            merge(result);

            return result;
        }
    }

    for (Span* span = m_span.vNext(); span != nullptr; span = span->vNext()) {
        if (span->pages() >= pages) {
            if (!result
                || span->pages() < result->pages()
                || (span->pages() == result->pages() && span->data() < result->data())
                ) {
                result = span;
                ASSERT(!result->inUse());
            }
        }
    }

    return result ? result->carve(pages) : nullptr;
}

void PageHeap::merge(Span* span) {
    if (!span) {
        return;
    }
    if (span->pages() <= MAX_PAGES) {
        m_pageSpans[span->pages() - 1].vPrepend(span);
    } else {
        m_span.vPrepend(span);
    }
}

Span* PageHeap::alloc(size_t pages) {
    Span* result = search(pages);
    if (!result) {
        result = allocFromSystem(pages);
        if (result) {
            m_tail.pPrependToLeft(result);

            merge(result);
            result = search(pages);
        }
    }

    if (result) {
        result->use();
        result->vRemove();
    }

    return result;
}

void PageHeap::free(Span* span) {

}

}  // namespace sitcmalloc
