/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#include "ThreadCache.h"
#include "CentralCache.h"
#include "PageHeap.h"

namespace sitcmalloc {

ThreadCache& ThreadCache::instance() {
    thread_local ThreadCache result;
    return result;
}

void* ThreadCache::alloc(size_t size) {
    FreeList& root = m_list[sizeToClass(size)];
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
    Span* span = ptr ? PageHeap::span(ptr) : nullptr;
    if (!span) {

        return false;
    }

    bool result = true;
    const size_t sizeClass = span->sizeClass();
    if (sizeClass == LARGE_CLASS) {
        CentralCache::instance(span->blockSize()).free(span);
        return result;
    }

    FreeList& root = m_list[sizeClass];
    if (root.m_len == root.m_initLen) {
        if (span->freeBlock() == 0) {
            result = CentralCache::instance(span->blockSize()).free(span);
        }
    } else {
        const size_t numToPrepend = 1;
        Block* start = reinterpret_cast<Block*>(ptr);
        Block* end = nullptr;

        root.prepend(numToPrepend, start, end);
    }

    return result;
}

size_t ThreadCache::len(size_t size) const {
    return m_list[sizeToClass(size)].m_len;
}

bool ThreadCache::empty(size_t size) const {
    return m_list[sizeToClass(size)].empty();
}

}  // namespace sitcmalloc

