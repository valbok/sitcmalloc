/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#ifndef SITCMALLOC_THREAD_CACHE_H
#define SITCMALLOC_THREAD_CACHE_H

#include <stddef.h> // for size_t, nullptr, ptrdiff_t
#include "common.h"
#include "Block.h"
#include "FreeList.h"

namespace sitcmalloc {

/**
 * Cache based on threads.
 * Each thread has its own a cache of blocks
 * that could be allocated/returned to user.
 */
class ThreadCache {
public:

    /**
     * Returns an instance of object.
     */
    static ThreadCache& instance();

    /**
     * Allocates number of bytes.
     */
    void* alloc(size_t size);

    /**
     * Frees previously allocated bytes.
     */
    bool free(void* ptr);

    /**
     * Returns length of cached blocks.
     * Used for testing purpose.
     */
    size_t len(size_t size) const;

    /**
     * Used for testing purpose.
     */
    bool empty(size_t size) const;

private:
    ThreadCache() = default;
    ThreadCache(const ThreadCache&) = delete;
    ThreadCache& operator=(const ThreadCache&) = delete;

    /**
     * Lists of blocks by size classes.
     */
    FreeList m_list[CLASSES];
};

}  // namespace sitcmalloc

#endif  // SITCMALLOC_THREAD_CACHE_H
