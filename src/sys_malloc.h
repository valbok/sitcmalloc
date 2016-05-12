#ifndef SITCMALLOC_SYS_MALLOC_H
#define SITCMALLOC_SYS_MALLOC_H

#include <stddef.h> // for size_t, NULL, ptrdiff_t

namespace sitcmalloc {

/**
 *
 */
void* sys_alloc(size_t size);

/**
 *
 */
bool sys_free(void* start, size_t length);

}  // namespace sitcmalloc

#endif  // SITCMALLOC_SYS_MALLOC_H
