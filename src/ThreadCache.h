#ifndef SITCMALLOC_THREAD_CACHE_H
#define SITCMALLOC_THREAD_CACHE_H

#include <stddef.h> // for size_t, nullptr, ptrdiff_t
#include "common.h"
#include "Block.h"
#include "FreeList.h"

namespace sitcmalloc {

/**
 *
 */
class ThreadCache {
public:
    static ThreadCache& instance();
    void* alloc(size_t size);
    bool free(void* ptr);

    size_t len(size_t size) const;
    bool empty(size_t size) const;

private:
    ThreadCache() = default;
    ThreadCache(const ThreadCache&) = delete;
    ThreadCache& operator=(const ThreadCache&) = delete;

	FreeList m_list[CLASSES];
};


}  // namespace sitcmalloc

#endif  // SITCMALLOC_THREAD_CACHE_H
