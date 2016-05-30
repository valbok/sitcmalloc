#include "PageHeap.h"
#include "PageMap.h"
#include "sys_alloc.h"
#include <unistd.h>
#include <iostream>
using namespace std;
namespace sitcmalloc {

namespace {

static inline uintptr_t key(void* ptr) {
    return reinterpret_cast<uintptr_t>(ptr) >> PAGE_SHIFT;
}
inline static void store(Span* s) {
    auto map = PageMap::instance();
    //cout << s <<"-"<< s + pagesToBytes(s->pages()) <<endl;
    for (size_t i = 0; i < pagesToBytes(s->pages()); i += pagesToBytes(1)) {
        uintptr_t start = reinterpret_cast<uintptr_t>(s);
        void* offset = reinterpret_cast<void*>(start + i);
        //cout <<"__key(s + i)="<<s+i<<":"<<key(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(s) + i))<<endl;
        map.set(key(offset), s);
    }
}
}

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

Span* PageHeap::search(size_t pages) {
    Span* result = nullptr;
    for (unsigned i = pages - 1; i < MAX_PAGES; ++i) {
        Span* root = &m_pageSpans[i];
        if (!root->vEmpty()) {
            Span* span = root->vNext();

            span->vRemove();
            result = span->carve(pages);
            merge(span);
            if (span != result) {
                merge(result);
            }
            break;
        }
    }

    // Search in large spans.
    if (!result) {
        for (Span* span = m_span.vNext(); span != nullptr; span = span->vNext()) {
            if (span->pages() >= pages) {
                if (!result
                    || span->pages() < result->pages()
                    || (span->pages() == result->pages() && span < result)
                    ) {
                    result = span;
                    ASSERT(!result->inUse());
                }
            }
        }
    }
    if (result) {
        Span* s = result->carve(pages);
        store(s);
        if (s != result) {
            store(result);
        }

        result = s;
    }

    return result;
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
    std::lock_guard<std::mutex> lock(m_mutex);
    Span* result = search(pages);
    if (!result) {
        result = allocFromSystem(pages);
        if (result) {
            m_tail.pPrependToLeft(result);

            store(result);
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


Span* PageHeap::span(void* ptr) {
    return reinterpret_cast<Span*>(PageMap::get(key(ptr)));
}

}  // namespace sitcmalloc
