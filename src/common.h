#ifndef SITCMALLOC_COMMON_H
#define SITCMALLOC_COMMON_H

#include <stddef.h> // for size_t, NULL, ptrdiff_t

namespace sitcmalloc {

static const size_t PAGE_SHIFT = 13;
static const size_t MAX_PAGES = 1 << (20 - PAGE_SHIFT);
static const size_t MIN_ALLOCATED_PAGES = MAX_PAGES;

static inline size_t pagesToBytes(size_t pages) {
    return pages << PAGE_SHIFT;
}


}  // namespace sitcmalloc

#endif  // SITCMALLOC_COMMON_H
