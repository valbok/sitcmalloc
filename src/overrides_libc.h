/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#ifndef SITCMALLOC_OVERRIDES_LIBC_H
#define SITCMALLOC_OVERRIDES_LIBC_H

#include <new>
#include <stddef.h> // for size_t, nullptr, ptrdiff_t
#include <sys/cdefs.h>    // for __THROW

#include "sitcmalloc.h"

using namespace sitcmalloc;

#ifndef __GNUC__
#error Only for GCC distributions
#endif

#define ALIAS(fn) __attribute__ ((alias (#fn), used))

// If not a glibc.
#ifndef __THROW
#define __THROW
#endif

void* operator new(size_t size) throw (std::bad_alloc)
    ALIAS(_new);
void operator delete(void* p) noexcept
    ALIAS(_delete);
void* operator new[](size_t size) throw (std::bad_alloc)
    ALIAS(_new_array);
void operator delete[](void* p) noexcept
    ALIAS(_delete_array);
void* operator new(size_t size, const std::nothrow_t& nt) noexcept
    ALIAS(_new_nothrow);
void* operator new[](size_t size, const std::nothrow_t& nt) noexcept
    ALIAS(_new_array_nothrow);
void operator delete(void* p, const std::nothrow_t& nt) noexcept
    ALIAS(_delete_nothrow);
void operator delete[](void* p, const std::nothrow_t& nt) noexcept
    ALIAS(_delete_array_nothrow);
void operator delete(void *p, size_t size) noexcept
    ALIAS(_delete);
void operator delete[](void *p, size_t size) noexcept
    ALIAS(_delete_array);

extern "C" {

void* malloc(size_t size) __THROW
    ALIAS(_malloc);
void free(void* ptr) __THROW
    ALIAS(_free);
void* realloc(void* ptr, size_t size) __THROW
    ALIAS(_realloc);
void* calloc(size_t n, size_t size) __THROW
    ALIAS(_calloc);
void cfree(void* ptr) __THROW
    ALIAS(_free);
void* memalign(size_t align, size_t s) __THROW
    ALIAS(_memalign);
void* valloc(size_t size) __THROW
    ALIAS(_valloc);
void* pvalloc(size_t size) __THROW
    ALIAS(_pvalloc);
//int posix_memalign(void** r, size_t a, size_t s) __THROW
//ALIAS(_posix_memalign);
int mallopt(int cmd, int value) __THROW
    ALIAS(_mallopt);
size_t malloc_size(void* p) __THROW
ALIAS(_malloc_size);
size_t malloc_usable_size(void* p) __THROW
ALIAS(_malloc_size);

static void* glibc_override_malloc(size_t size, const void *caller)
    ALIAS(_malloc);
static void* glibc_override_realloc(void *ptr, size_t size,
    const void *caller)
    ALIAS(_realloc);
static void glibc_override_free(void *ptr, const void *caller)
    ALIAS(_free);
static void* glibc_override_memalign(size_t align, size_t size,
    const void *caller)
    ALIAS(_memalign);

void* (* volatile __malloc_hook)(size_t, const void*)
    = &glibc_override_malloc;
void* (* volatile __realloc_hook)(void*, size_t, const void*)
    = &glibc_override_realloc;
void (* volatile __free_hook)(void*, const void*)
    = &glibc_override_free;
void* (* volatile __memalign_hook)(size_t,size_t, const void*)
    = &glibc_override_memalign;

}   // extern "C"

#undef ALIAS

#endif  // SITCMALLOC_OVERRIDES_LIBC_H

