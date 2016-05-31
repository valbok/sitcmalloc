
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
        Block *b = nullptr, *end = nullptr;
        size_t num = CentralCache::instance(size).alloc(&b, &end);
        if (b) {
            cout <<"alloc prepending="<<sizeClass<<":"<< num<<endl;
            root.prepend(num, b, end);
            cout <<"alloc AFTER root is empty="<<sizeClass<<":"<< root.empty()<<endl;
        }
    }
    void* p = root.pop();
    cout <<"alloc_________ root is empty="<<sizeClass<<":"<< root.empty()<<endl;
    return p;
}

bool ThreadCache::free(void* ptr) {
    Span* span = PageHeap::span(ptr);
    if (span == nullptr) {
        return false;
    }

    size_t sizeClass = span->sizeClass();
    FreeList& root = m_list[sizeClass];
    cout <<"free root is empty="<<sizeClass<<":"<< root.empty()<<endl;
    root.prepend(1, reinterpret_cast<Block*>(ptr), 0);
    if (root.length() > root.maxLength()) {
        // todo
    }
    return true;

}

}  // namespace sitcmalloc

