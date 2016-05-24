
#include "ThreadCache.h"
#include "CentralCache.h"
#include <iostream>

namespace sitcmalloc {

ThreadCache& ThreadCache::instance() {
	thread_local ThreadCache result;
    return result;
}

Block* ThreadCache::alloc(size_t size) {
    Block* result = nullptr;
    if (size <= MAX_CLASS_SIZE) {
        const size_t sizeClass = sizeToClass(size);
        Block* block = &m_list[sizeClass];
        if (block->empty()) {
            Block* b = CentralCache::instance(sizeClass).alloc();
            if (b) {
                block->prepend(b);
            }
        }

        result = block->pop();
    } else {

    }

    return result;
}

}  // namespace sitcmalloc

