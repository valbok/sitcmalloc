#ifndef SITCMALLOC_COMMON_H
#define SITCMALLOC_COMMON_H

#include <stddef.h> // for size_t, NULL, ptrdiff_t
#include <stdint.h>
#include <cstdio>
#include <iostream>
#include <assert.h>
namespace sitcmalloc {

#define ASSERT(x) assert((x))

static const size_t PAGE_SHIFT = 13;
static const size_t MAX_PAGES = 1 << (20 - PAGE_SHIFT);
static const size_t MIN_ALLOCATED_PAGES = MAX_PAGES;
static const int MAX_CLASS_SIZE = 256 * 1024;

static const size_t classSizes[] = {
    0,      8,      16,     32,     48,     64,     80,     96,     
    112,    128,    144,    160,    176,    192,    208,    224,    
    240,    256,    288,    320,    352,    384,    416,    448,    
    480,    512,    576,    640,    704,    768,    832,    896,    
    960,    1024,   1152,   1280,   1408,   1536,   1664,   1920,   
    2304,   2560,   2816,   3072,   3328,   3584,   4608,   5120,   
    5632,   6656,   7168,   9216,   10240,  11264,  13312,  14336,  
    18432,  22528,  26624,  28672,  36864,  45056,  53248,  61440,  
    73728,  81920,  90112,  98304,  106496, 114688, 122880, 131072, 
    139264, 147456, 155648, 163840, 172032, 180224, 188416, 196608, 
    204800, 212992, 221184, 229376, 237568, 245760, 253952, 262144};

static const size_t CLASSES = sizeof(classSizes) / sizeof(classSizes[0]) - 1;

static inline size_t pagesToBytes(size_t pages) {
    return pages << PAGE_SHIFT;
}

static inline size_t sizeToClass(size_t size) {
    int i = CLASSES - 1;
    // todo bin search
    for (; i >= 0 && classSizes[i] >= size; --i) {
    }

    return i;
}

static inline size_t classToSize(size_t sizeClass) {
    return classSizes[sizeClass + 1];
}

static inline size_t sizeToMinPages(size_t size) {
    size_t pages = 1;
    for (; pagesToBytes(pages) / size < 2; ++pages) {
    }
    return pages;
}

}  // namespace sitcmalloc

#endif  // SITCMALLOC_COMMON_H
