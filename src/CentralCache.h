#ifndef SITCMALLOC_CENTRAL_CACHE_H
#define SITCMALLOC_CENTRAL_CACHE_H

#include "Span.h"
#include "Block.h"
#include <stddef.h> // for size_t, nullptr, ptrdiff_t
#include <mutex>

namespace sitcmalloc {

/**
 *
 */
class CentralCache {
public:
    CentralCache() : m_sizeClass(0), m_pages(0), m_size(0) {}

    static CentralCache& instance(size_t size);
    virtual size_t alloc(Block** start, Block** end) = 0;
    virtual bool free(Span*) = 0;
    virtual void init(size_t size) = 0;
    virtual ~CentralCache() {}

protected:

    CentralCache(const CentralCache&) = delete;
    CentralCache& operator=(const CentralCache&) = delete;

    Span m_span;
    size_t m_sizeClass;
    size_t m_pages;
    size_t m_size;
    std::mutex m_mutex;    
};

/**
 *
 */
class CentralSmallCache : public CentralCache {
public:
    CentralSmallCache() : CentralCache(), m_freeSpans(0), m_maxFreeSpans(2) {}

    virtual size_t alloc(Block** start, Block** end) override;
    virtual bool free(Span*) override;
    virtual void init(size_t size) override;

protected:
    Span* fetch();

    size_t m_freeSpans;
    size_t m_maxFreeSpans;
};

class CentralLargeCache : public CentralCache {
public:
    
    /**
     * @return Large class always returns only requested amount, thus it is 1.
     */
    virtual size_t alloc(Block** start, Block** end) override;
    virtual bool free(Span*) override;
    virtual void init(size_t size) override;
};

}  // namespace sitcmalloc

#endif  // SITCMALLOC_CENTRAL_CACHE_H
