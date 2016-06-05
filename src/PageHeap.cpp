#include "PageHeap.h"
#include "PageMap.h"
#include "sys_alloc.h"
#include <unistd.h>
#include <iostream>
using namespace std;
namespace sitcmalloc {

Span* PageHeap::allocFromSystem(size_t pages) {
	if (pages < MIN_ALLOCATED_PAGES) {
    	pages = MIN_ALLOCATED_PAGES;
	}

    size_t size = pagesToBytes(pages);
    size_t alignment = pagesToBytes(1);
    size = ((size + alignment - 1) / alignment) * alignment;
    void* ptr = sys_alloc(size);
    uintptr_t n = reinterpret_cast<uintptr_t>(ptr);

    if ((n & (alignment - 1)) != 0) {
        size_t extra = alignment - (n & (alignment - 1));
        void* ptr2 = sys_alloc(extra);
        if (reinterpret_cast<uintptr_t>(ptr2) != (n + size)) {
            ptr = sys_alloc(size + alignment - 1);
            n = reinterpret_cast<uintptr_t>(ptr);
            if ((n & (alignment-1)) != 0) {
                n += alignment - (n & (alignment - 1));
            }
            ptr = reinterpret_cast<void*>(n);
        } else {
            ptr = reinterpret_cast<void*>(n + extra);
        }
    }
    ASSERT((reinterpret_cast<uintptr_t>(ptr) & (alignment - 1)) == 0);

    return Span::create(ptr, pages);
}

Span* PageHeap::fetch(size_t pages) {
    Span* result = nullptr;
    for (unsigned i = pages - 1; i < MAX_PAGES; ++i) {
        Span* root = &m_pageSpans[i];
        if (!root->empty()) {
            result = root->next();
            break;
        }
    }

    // Search in large spans.
    if (!result) {
        for (Span* span = m_largeSpan.next(); span != nullptr; span = span->next()) {
            if (span->pages() >= pages) {
                if (!result
                    || span->pages() < result->pages()
                    || (span->pages() == result->pages() && span < result)
                    ) {
                    result = span;
                }
            }
        }
    }

    if (result) {
        ASSERT(!result->inUse());
        ASSERT(result->pages() >= pages);
        result->remove();
        if (result->pages() != pages) {
            Span* s = result->carve(pages);
            ASSERT(s->pages() == pages);
            PageMap::store(s);
            if (s != result) {
                PageMap::store(result);
                merge(result);
            }

            result = s;
        }

        result->use();
    }

    return result;
}

void PageHeap::merge(Span* span) {
    if (!span) {
        return;
    }
    if (span->pages() <= MAX_PAGES) {
        m_pageSpans[span->pages() - 1].prepend(span);
    } else {
        m_largeSpan.prepend(span);
    }
}

Span* PageHeap::alloc(size_t pages) {
    std::lock_guard<std::mutex> lock(m_mutex);
    Span* result = fetch(pages);
    if (!result) {
        result = allocFromSystem(pages);
        if (result) {
            PageMap::store(result);
            merge(result);
            result = fetch(pages);
        }
    }

    return result;
}

bool PageHeap::free(Span* span) {
    std::lock_guard<std::mutex> lock(m_mutex);

    // span should not exist in our lists
    // thus span->next()/prev() must not be called.

    const uintptr_t pageID = PageMap::key(span);
    const size_t pages = span->pages();

    Span* prev = PageHeap::span(pageID - 1);
    if (prev && prev->inUse() == false) {
        ASSERT(PageMap::key(prev) + prev->pages() == pageID);
        prev->remove();
        prev->setPages(prev->pages() + pages);
        span = prev;
    }

    Span* next = PageHeap::span(pageID + pages);
    if (next && next->inUse() == false) {
        ASSERT(PageMap::key(next) == pageID + pages);
        next->remove();
        span->setPages(span->pages() + next->pages());
    }

    PageMap::remove(span);
    return sys_free(span, pagesToBytes(span->pages()));
}


}  // namespace sitcmalloc
