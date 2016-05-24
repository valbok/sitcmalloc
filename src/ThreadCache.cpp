
#include "ThreadCache.h"
#include "CentralCache.h"
#include <iostream>

namespace sitcmalloc {

ThreadCache& ThreadCache::instance() {
	thread_local ThreadCache result;
    return result;
}

void* ThreadCache::alloc(size_t size) {
    const size_t sizeClass = sizeToClass(size);
    Block& root = m_list[sizeClass];

    if (root.empty()) {
        Block* b = CentralCache::instance(size).alloc();
        if (b) {
            root.prepend(b);
        }
    }

    return root.pop();
}

}  // namespace sitcmalloc

