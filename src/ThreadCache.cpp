
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
cout <<"ThreadCache alloc size " << size << endl;
    FreeList& root = m_list[sizeToClass(size)];
    if (root.empty()) {
        cout <<"   empty " << size << endl;
        Block *start = nullptr, *end = nullptr;
        size_t num = CentralCache::instance(size).alloc(&start, &end);
        cout <<"  ALLOCed num=" <<num <<endl;
        if (start) {
            root.prepend(num, start, end);
        }
    }
    void* r = root.pop();
cout <<"pop len=" << root.len() <<": empty=" <<root.empty() << endl;
    return r;
}

bool ThreadCache::free(void* ptr) {    
    Span* span = PageHeap::span(ptr);
    if (!span) {
        return false;
    }

    bool result = true;
    FreeList& root = m_list[span->sizeClass()];
    const size_t numToPrepend = 1;
    Block* start = reinterpret_cast<Block*>(ptr);
    Block* end = nullptr;

    root.prepend(numToPrepend, start, end);
    if (root.returned()) {
        result = CentralCache::instance(span->size()).free(span);
        root.clear();
    }

    return result;
}

size_t ThreadCache::len(size_t size) const {
    return m_list[sizeToClass(size)].len();
}

bool ThreadCache::empty(size_t size) const {
    return m_list[sizeToClass(size)].empty();
}

}  // namespace sitcmalloc

