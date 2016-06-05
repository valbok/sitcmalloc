/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#ifndef SITCMALLOC_SYS_ALLOC_H
#define SITCMALLOC_SYS_ALLOC_H

#include <stddef.h> // for size_t, NULL, ptrdiff_t

namespace sitcmalloc {

/**
 * Allocates bytes from system.
 */
void* sys_alloc(size_t size);

/**
 * Deallocates previously allocated bytes to system.
 */
bool sys_free(void* start, size_t size);

}  // namespace sitcmalloc

#endif  // SITCMALLOC_SYS_ALLOC_H
