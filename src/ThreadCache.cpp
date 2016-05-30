
#include "ThreadCache.h"
#include "CentralCache.h"
#include "PageHeap.h"
#include <iostream>

namespace sitcmalloc {

ThreadCache& ThreadCache::instance() {
	thread_local ThreadCache result;
    return result;
}

void* ThreadCache::alloc(size_t size) {
    const size_t sizeClass = sizeToClass(size);
    Root& root = m_list[sizeClass];

    if (root.empty()) {
        size_t num;
        Block* b = CentralCache::instance(size).alloc(num);
        if (b) {
            root.prepend(b, num);
        }
    }

    return root.pop();
}

void ThreadCache::free(void* ptr) {
    Span* span = PageHeap::span(ptr);
    if (span == nullptr) {
        return;
    }

    size_t sizeClass = span->sizeClass();
    Root& root = m_list[sizeClass];
    root.prepend(reinterpret_cast<Block*>(ptr), 1);
    if (root.m_len > root.m_maxLen) {

    }

}

}  // namespace sitcmalloc

