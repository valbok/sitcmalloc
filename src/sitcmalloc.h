/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#ifndef SITCMALLOC_H
#define SITCMALLOC_H

#include <stddef.h> // for size_t, nullptr, ptrdiff_t

namespace sitcmalloc {

extern "C" {
    void* _malloc(size_t);
    void _free(void*);
} // extern "C"

} // namespace sitcmalloc

#endif // SITCMALLOC_H