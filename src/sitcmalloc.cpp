/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#include "sitcmalloc.h"
#include "overrides.h"
#include "ThreadCache.h"

namespace sitcmalloc {

void* _malloc(size_t size) {
    return ThreadCache::instance().alloc(size);
}

void _free(void* p) {
    if (ThreadCache::instance().free(p) != true) {

    }
}

} // namespace sitcmalloc