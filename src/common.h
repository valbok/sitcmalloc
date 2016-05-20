#ifndef SITCMALLOC_COMMON_H
#define SITCMALLOC_COMMON_H

#include <stddef.h> // for size_t, NULL, ptrdiff_t
#include <stdint.h>  
#include <assert.h>
namespace sitcmalloc {

#define ASSERT(x) assert((x))

static const size_t PAGE_SHIFT = 13;
static const size_t MAX_PAGES = 1 << (20 - PAGE_SHIFT);
static const size_t MIN_ALLOCATED_PAGES = MAX_PAGES;
static const int MAX_SMALL_SIZE = 1024;
static const size_t CLASSES = 82;



static inline size_t pagesToBytes(size_t pages) {
    return pages << PAGE_SHIFT;
}

static inline size_t classToPages(size_t sizeClass) {
    return 1; // TODO
}

static inline size_t sizeToClass(size_t size) {
    if (size <= MAX_SMALL_SIZE) {
      return (static_cast<uint32_t>(size) + 7) >> 3;
  	}

	return (static_cast<uint32_t>(size) + 127 + (120 << 7)) >> 7;
}

static inline size_t classToSize(size_t sizeClass) {
    if (sizeClass <= 128) {
    	return sizeClass * 8;
    }

    return sizeClass * 128 - (120 << 7);
}


}  // namespace sitcmalloc

#endif  // SITCMALLOC_COMMON_H
