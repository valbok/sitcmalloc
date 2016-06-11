/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#ifndef SITCMALLOC_CENTRAL_CACHE_H
#define SITCMALLOC_CENTRAL_CACHE_H

#include "Span.h"
#include "Block.h"
#include <stddef.h> // for size_t, nullptr, ptrdiff_t
#include <mutex>

namespace sitcmalloc {

/**
 * Base class of cached memory allocated from PageHeap
 * that shared between all threads.
 */
class CentralCache {
public:

    /**
     * Default ctor
     */
    CentralCache() : m_sizeClass(0), m_pages(0), m_size(0) {}

    /**
     * Returns proper instance by size.
     * For example if it needs to allocate only 14 bytes.
     * It returns handler that keeps list of 16 bytes chunks of memory.
     */
    static CentralCache& instance(size_t size);

    /**
     * Allocates proper number of bytes
     * that determined by requested size.
     * For example if cache contains 8 bytes.
     * It returns quite few blocks. 
     * Each block would be 8 bytes.
     *
     * @param[out] Pointer to start chunk of memory
     * @param[out] Pointer to last chunk of memory
     * @return Number of allocated chunks/blocks of memory.
     *         Always more than 1.
     * @note Returns all memory of one span object.
     */
    virtual size_t alloc(Block** start, Block** end) = 0;

    /**
     * Frees previously allocated span object.
     */
    virtual bool free(Span*) = 0;

    /**
     * Inits current object by size.
     */ 
    virtual void init(size_t size) = 0;
    virtual ~CentralCache() = default;

protected:

    CentralCache(const CentralCache&) = delete;
    CentralCache& operator=(const CentralCache&) = delete;

    /**
     * Root object of singly-linked list of spans.
     */
    Span m_span;

    /**
     * Size class of current cache.
     */
    size_t m_sizeClass;

    /**
     * Number of pages allocated from PageHeap based on size.
     */
    size_t m_pages;

    /**
     * Number of bytes allocated from PageHeap based on size.
     */
    size_t m_size;

    /**
     * Synchronizes the access by few threads to current cache object.
     */    
    std::mutex m_mutex;
};

/**
 * To handle "small" chunks of memory.
 * Belongs to all size classes that is not LARGE_CLASS.
 */
class CentralSmallCache : public CentralCache {
public:

    /**
     * @copydoc CentralCache::CentralCache()
     */
    CentralSmallCache() : CentralCache(), m_freeSpans(0), m_maxFreeSpans(2) {}

    /**
     * @copydoc CentralCache::alloc(Block**,Block**)
     */
    virtual size_t alloc(Block** start, Block** end) override;

    /**
     * @copydoc CentralCache::free(Span*)
     */
    virtual bool free(Span*) override;

    /**
     * @copydoc CentralCache::init(size_t)
     */
    virtual void init(size_t size) override;

protected:

    /**
     * Finds and removes proper satisfied span object from the list.
     */
    Span* fetch();

    /**
     * Current number of spans in the list.
     */
    size_t m_freeSpans;

    /**
     * Number of spans that allowed to be kept in the list.
     * If length of the list exceeds this number, 
     * needs to return objects back to PageHeap.
     */
    size_t m_maxFreeSpans;
};

/**
 * Handles objects with size class LARGE_CLASS.
 */
class CentralLargeCache : public CentralCache {
public:

    /**
     * @copydoc CentralCache::alloc()
     * @return Large class always returns only requested amount, thus it is 1.
     */
    virtual size_t alloc(Block** start, Block** end) override;

    /**
     * @copydoc CentralCache::free(Span*)
     */
    virtual bool free(Span*) override;

    /**
     * @copydoc CentralCache::init(size_t)
     */
    virtual void init(size_t size) override;
};

}  // namespace sitcmalloc

#endif  // SITCMALLOC_CENTRAL_CACHE_H
