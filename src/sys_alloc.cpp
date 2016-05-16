#include "sys_alloc.h"

#include <errno.h>                      // for EAGAIN, errno
#include <stdint.h>                     // for uintptr_t, intptr_t
#include <sys/mman.h>                   // for munmap, mmap, MADV_DONTNEED, etc
#include <unistd.h>                     // for sbrk, getpagesize, off_t
#include <iostream>

namespace sitcmalloc {

namespace {

void* sbrk_alloc(size_t size) {
    void* result = sbrk(size);
    if (result == reinterpret_cast<void*>(-1)) {
        return NULL;
    }

    return result;
}

void* mmap_alloc(size_t size) {
    return NULL;
}

} // namespace

void* sys_alloc(size_t size) {
    return sbrk_alloc(size);
}

bool sys_free(void* start, size_t length) {
    static size_t pagesize = getpagesize();
    const size_t pagemask = pagesize - 1;

    size_t new_start = reinterpret_cast<size_t>(start);
    size_t end = new_start + length;
    size_t new_end = end;

    // Round up the starting address and round down the ending address
    // to be page aligned:
    new_start = (new_start + pagesize - 1) & ~pagemask;
    new_end = new_end & ~pagemask;

    if (new_end > new_start) {
        int result;
        do {
          result = madvise(reinterpret_cast<char*>(new_start),
              new_end - new_start, MADV_DONTNEED);
        } while (result == -1 && errno == EAGAIN);

        return result != -1;
    }

    return false;
}

}  // namespace sitcmalloc
