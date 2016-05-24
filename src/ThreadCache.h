#ifndef SITCMALLOC_THREAD_CACHE_H
#define SITCMALLOC_THREAD_CACHE_H

#include <stddef.h> // for size_t, nullptr, ptrdiff_t
#include "common.h"
#include "Block.h"

namespace sitcmalloc {

/**
 *
 */
class ThreadCache {
public:
    static ThreadCache& instance();
    Block* alloc(size_t size);
private:
	ThreadCache() {}
	ThreadCache(const ThreadCache&);
	ThreadCache& operator=(const ThreadCache&);

	Block m_list[CLASSES];
};


}  // namespace sitcmalloc

#endif  // SITCMALLOC_THREAD_CACHE_H
