
#include "CentralCache.h"
#include "PageHeap.h"
#include "common.h"
#include "Block.h"

#include <iostream>
namespace sitcmalloc {

CentralCache& CentralCache::instance(size_t size) {
	static CentralSmallCache list[CLASSES - 1];
    static CentralLargeCache largeCache;
    
    CentralCache* result = nullptr;
    const size_t sizeClass = sizeToClass(size);

    if (sizeClass == LARGE_CLASS) {
        largeCache.init(pageAligned(size));
        result = &largeCache;
    } else {
        CentralSmallCache& cache = list[sizeClass];
        cache.init(classToSize(sizeClass));
        result = &cache;
    }

    return *result;
}

void CentralSmallCache::init(size_t size) {
    m_size = size;
    m_pages = sizeToMinPages(m_size, 8);
    m_maxFreeSpans = 2; // todo
}

void CentralLargeCache::init(size_t size) {
    m_size = size;
    m_pages = sizeToMinPages(m_size + sizeof(Span), 1);
}

Span* CentralSmallCache::fetch() {
    Span* span = m_span.next();
    if (span) {
        std::lock_guard<std::mutex> lock(m_mutex);
        span->remove();
        --m_freeSpans;        
    }

    return span;
}

size_t CentralSmallCache::alloc(Block** start, Block** end) {
    Span* span = fetch();
    if (!span) {
        span = PageHeap::instance().alloc(m_pages);
    }
    size_t num = span ? span->split(m_size, m_sizeClass, start, end) : 0;

    return num;
}

bool CentralSmallCache::free(Span* span) {
    std::lock_guard<std::mutex> lock(m_mutex);
    bool result = true;
    m_span.prepend(span);
    ++m_freeSpans;
    if (m_freeSpans > m_maxFreeSpans) {
        const size_t delta = m_freeSpans - m_maxFreeSpans;
        for (size_t i = 0; i < delta; ++i) {
            Span* s = m_span.next();
            if (s) {
                --m_freeSpans;
                s->remove();
                if (PageHeap::instance().free(s) != true) {
                    result = false;
                }
            }
        }
    }

    return result;
}

size_t CentralLargeCache::alloc(Block** start, Block** end) {
    Span* span = PageHeap::instance().alloc(m_pages);
    return span ? span->split(m_size, m_sizeClass, start, end) : 0;
}

bool CentralLargeCache::free(Span* span) {
    return PageHeap::instance().free(span);
}

}  // namespace sitcmalloc

