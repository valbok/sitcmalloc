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

void* oom(size_t size, bool nothrow) {
	while (true) {
		std::new_handler h = std::get_new_handler();
        if (!h) {
            if (nothrow) {
                return nullptr;
            }
	       throw std::bad_alloc();
	    }
        // Otherwise, try the new_handler.  If it returns, retry the
        // allocation.  If it throws std::bad_alloc, fail the allocation.
        // if it throws something else, don't interfere.
        try {
            (*h)();
        } catch (const std::bad_alloc&) {
            if (!nothrow) {
                throw;
            }
            return nullptr;
        }
    
        // we get here if new_handler returns successfully. So we retry
        // allocation.
        void* ptr = _malloc(size);
        if (ptr) {
          return ptr;
        }
	}
}

void* _new(size_t size, bool nothrow) {
	void* result = _malloc(size);
	if (result) {
		return result;
	}

	return oom(size, nothrow);
}

extern "C" void* _new(size_t size) {
	return _new(size, false);
}



void _free(void* p) {
    if (ThreadCache::instance().free(p) != true) {

    }
}

} // namespace sitcmalloc