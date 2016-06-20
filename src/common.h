/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#ifndef SITCMALLOC_COMMON_H
#define SITCMALLOC_COMMON_H

#include <stddef.h> // for size_t, NULL, ptrdiff_t
#include <stdint.h>
#include <cstdlib>
#include <cstdio>

namespace sitcmalloc {

/**
 * Custom assert
 */
inline void _assert(const char* expression, const char* file, int line) {
    fprintf(stderr, "Assertion '%s' failed: %s:%d\n", expression, file, line);
    abort();
}

#if defined NDEBUG && NDEBUG
#define ASSERT(EXPRESSION) ((void)0)
#else
#define ASSERT(x) (x) ? (void)0 : _assert(#x, __FILE__, __LINE__)
#endif

/**
 * Number of bits in one page: 1 << PAGE_SHIFT
 */
static const size_t PAGE_SHIFT = 13;

/**
 * Defines size of one page.
 */
static const size_t PAGE_SIZE = 1 << PAGE_SHIFT;

/**
 * Number of maximum pages operated.
 */
static const size_t MAX_PAGES = 1 << (20 - PAGE_SHIFT);

/**
 * Number of minimum allocated pages by one probe.
 */
static const size_t MIN_ALLOCATED_PAGES = MAX_PAGES;

/**
 * Lookup table of size classes.
 * For example size 15 belongs to class 2 with size 16.
 */
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

/**
 * Total number of classes.
 */
static const size_t CLASSES = sizeof(classSizes) / sizeof(classSizes[0]);

/**
 * Size of maximum class
 */
static const int MAX_CLASS_SIZE = classSizes[CLASSES - 1];

/**
 * Any sizes that more the maximum class size belong to "large" class.
 */
static const size_t LARGE_CLASS = CLASSES - 1;

/**
 * Returns bytes of pages.
 */
static inline size_t pagesToBytes(size_t pages) {
    return pages << PAGE_SHIFT;
}

/**
 * Returns pages by size.
 */
static inline size_t sizeToPages(size_t size) {
    return (size >> PAGE_SHIFT) + ((size & (PAGE_SIZE - 1)) > 0 ? 1 : 0);
}

/**
 * Returns class of requested size.
 */
static inline size_t sizeToClass(size_t size) {
    int i = LARGE_CLASS;
    // todo bin search
    for (; i >= 0 && classSizes[i] >= size; --i) {
    }

    return i;
}

/**
 * Returns size of requested class.
 */
static inline size_t classToSize(size_t sizeClass) {
    return classSizes[sizeClass + 1];
}

/**
 * Returns number of pages that will be allocated at once.
 */
static inline size_t sizeToMinPages(size_t size, size_t min = 2) {
    size_t pages = 1;
    for (; pagesToBytes(pages) / size < min; ++pages) {
    }
    return pages;
}

/**
 * Returns aligned size by page.
 */
static inline size_t pageAligned(size_t size) {
    return ((size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;
}

}  // namespace sitcmalloc

#endif  // SITCMALLOC_COMMON_H
