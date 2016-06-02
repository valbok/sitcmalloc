
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
    if (!span) {
        return false;
    }

    const size_t sizeClass = span->sizeClass();
    FreeList& root = m_list[sizeClass];
    const size_t numToPrepend = 1;
    Block* start = reinterpret_cast<Block*>(ptr);
    Block* end = nullptr;

    root.prepend(numToPrepend, start, end);
    if (root.returned()) {
        CentralCache::instance(classToSize(sizeClass)).free(span);
        root.clear();
    }

    return true;
}

}  // namespace sitcmalloc

