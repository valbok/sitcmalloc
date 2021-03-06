
#include "CentralCache.h"
#include "PageHeap.h"
#include "common.h"
#include "Block.h"

namespace sitcmalloc {

CentralCache& CentralCache::instance(size_t size) {
    const size_t sizeClass = sizeToClass(size);
    if (sizeClass == LARGE_CLASS) {
        static CentralLargeCache largeCache;
        largeCache.init(pageAligned(size));
        return largeCache;
    }

    static CentralSmallCache list[CLASSES - 1];
    CentralSmallCache& cache = list[sizeClass];
    cache.init(classToSize(sizeClass));
    return cache;
}

void CentralSmallCache::init(size_t size) {
    m_size = size;
    m_sizeClass = sizeToClass(size);
    m_pages = sizeToMinPages(m_size, 8);
    m_maxFreeSpans = 2; // todo
}

void CentralLargeCache::init(size_t size) {
    m_size = size;
    m_sizeClass = sizeToClass(size);
    m_pages = sizeToMinPages(m_size + sizeof(Span), 1);
}

Span* CentralSmallCache::fetch() {
    Span* span = m_span.next();
    if (span) {
        span->remove();
        --m_freeSpans;
    }

    return span;
}

size_t CentralSmallCache::alloc(Block** start, Block** end) {
    std::lock_guard<std::mutex> lock(m_mutex);
    Span* span = fetch();
    if (!span) {
        span = PageHeap::instance().alloc(m_pages);
    }

    return span ? span->split(m_size, m_sizeClass, start, end) : 0;
}

bool CentralSmallCache::free(Span* span) {
    std::lock_guard<std::mutex> lock(m_mutex);
    bool result = true;
    m_span.prepend(span);
    if (++m_freeSpans > m_maxFreeSpans) {
        Span* s = fetch();
        ASSERT(s);
        result = PageHeap::instance().free(s);
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

