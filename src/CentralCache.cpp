
#include "CentralCache.h"
#include "PageHeap.h"
#include "common.h"
#include "Block.h"
#include <mutex>

#include <iostream>
namespace sitcmalloc {

CentralCache& CentralCache::instance(size_t size) {
	static CentralCache list[CLASSES];

    const size_t sizeClass = sizeToClass(size);
	CentralCache& result = list[sizeClass];
	result.m_sizeClass = sizeClass;
    // @todo Cache it
    result.m_size = sizeClass == LARGE_CLASS ? pageAligned(size) : classToSize(sizeClass);
    result.m_pages = sizeClass == LARGE_CLASS ? sizeToMinPages(result.m_size + sizeof(Span), 1) : sizeToMinPages(result.m_size, 8);
    result.m_maxFreeSpans = 2; // tmp

    return result;
}

Span* CentralCache::fetch() {
    Span* span = m_span.next();
    if (span) {
        std::lock_guard<std::mutex> lock(m_mutex);
        span->remove();
        --m_freeSpans;        
    }

    return span;
}

size_t CentralCache::alloc(Block** start, Block** end) {
    Span* span = fetch();
    if (!span) {
        span = PageHeap::instance().alloc(m_pages);
    }
    size_t num = span ? span->split(m_size, m_sizeClass, start, end) : 0;

    return num;
}

void CentralCache::free(Span* span) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_span.prepend(span);
    ++m_freeSpans;
    if (m_freeSpans > m_maxFreeSpans) {
        const size_t delta = m_freeSpans - m_maxFreeSpans;
        for (size_t i = 0; i < delta; ++i) {
            Span* s = m_span.next();
            if (s) {
                --m_freeSpans;
                s->remove();
                // @todo Remove only extra pages, not whole spans
                PageHeap::instance().free(s);
            }
        }
    }
}

}  // namespace sitcmalloc

