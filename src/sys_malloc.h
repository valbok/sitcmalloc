#ifndef SITCMALLOC_SYS_MALLOC_H_
#define SITCMALLOC_SYS_MALLOC_H_

#include <stddef.h>                     // for size_t, NULL, ptrdiff_t

namespace sitcmalloc {

void* sys_alloc(size_t size, size_t *actual_size,
	size_t alignment);

bool sys_free(void* start, size_t length);

}  // namespace sitcmalloc

#endif  // SITCMALLOC_SYS_MALLOC_H_
