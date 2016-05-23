
#include "ThreadCache.h"

#include <iostream>

namespace sitcmalloc {

ThreadCache& ThreadCache::instance() {
	thread_local ThreadCache result;
    return result;
}

void* ThreadCache::alloc(size_t sizeClass) {
    Block* block = &m_list[sizeClass];
    if (block && !block->empty()) {
        return block->pop();
    }

    return nullptr;
}

}  // namespace sitcmalloc

