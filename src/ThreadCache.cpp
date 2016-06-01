
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
    FreeList& root = m_list[sizeClass];

    if (root.empty()) {
        Block *start = nullptr, *end = nullptr;
        size_t num = CentralCache::instance(size).alloc(&start, &end);
        if (start) {
            root.prepend(num, start, end);
        }
    }

    return root.pop();
}

bool ThreadCache::free(void* ptr) {
    Span* span = PageHeap::span(ptr);
    if (span == nullptr) {
        return false;
    }

    size_t sizeClass = span->sizeClass();
    FreeList& root = m_list[sizeClass];
    root.prepend(1, reinterpret_cast<Block*>(ptr), nullptr);
    if (root.returned()) {
        CentralCache::instance(classToSize(sizeClass)).free(span);
    }

    return true;

}

}  // namespace sitcmalloc

