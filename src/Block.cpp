#include "Block.h"
#include "common.h"
#include <iostream>
#include <cstdio>
using namespace std;

namespace sitcmalloc {

size_t Block::split(void* limit, size_t size) {
    void** tail = &m_next;
    char* start = reinterpret_cast<char*>(&m_next);
    size_t num = 0;
    while (start + size <= limit) {
        *tail = start;
        tail = reinterpret_cast<void**>(start);
        start += size;
        ++num;
    }
    ASSERT(start <= limit);

    return num;
}

void* Block::next() {
    return m_next;
}

}  // namespace sitcmalloc

