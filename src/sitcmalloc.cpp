/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <string.h> // memset
#include <unistd.h> // getpagesize
#include "sitcmalloc.h"
#include "overrides.h"
#include "ThreadCache.h"
#include "PageHeap.h"

namespace sitcmalloc {

static size_t s_pagesize = 0;

void* _malloc(size_t size) noexcept {
    return ThreadCache::instance().alloc(size);
}

void _free(void* p) noexcept {
    ThreadCache::instance().free(p);
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

        try {
            (*h)();
        } catch (const std::bad_alloc&) {
            if (!nothrow) {
                throw;
            }
            return nullptr;
        }

        // Retry allocation.
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

extern "C" {

void* _new(size_t size) {
    return _new(size, false);
}

void* _new_nothrow(size_t size, const std::nothrow_t&) noexcept {
    return _new(size, true);
}

void* _new_array(size_t size) __attribute__((alias("_new")));
void* _new_array_nothrow(size_t size, const std::nothrow_t&) noexcept __attribute__((alias("_new_nothrow")));
void _delete(void* p) noexcept __attribute__((alias("_free")));
void _delete_array(void*) noexcept __attribute__((alias("_free")));
void _delete_nothrow(void* p, const std::nothrow_t&) noexcept __attribute__((alias("_free")));
void _delete_array_nothrow(void* p, const std::nothrow_t&) noexcept __attribute__((alias("_free")));

void* _realloc(void* ptr, size_t new_size) noexcept {
    if (!ptr) {
        return _malloc(new_size);
    }

    if (new_size == 0) {
        _free(ptr);
        return nullptr;
    }

    Span* s = PageHeap::span(ptr);
    if (!s) {
        return nullptr;
    }

    void* result = ptr;
    size_t old_size = s->size();
    if (new_size > old_size) {
        result = _malloc(new_size);
    }

    return result;
}

void* _calloc(size_t n, size_t item_size) noexcept {
    const size_t size = n * item_size;
    if (item_size != 0 && size / item_size != n) {
        return nullptr;
    }

    void* result = _malloc(size);
    if (result) {
        memset(result, 0, size);
    }

    return result;
}

size_t _malloc_size(void* ptr) noexcept {
    Span* s = PageHeap::span(ptr);
    return s ? s->size() : 0;
}

int _mallopt(int cmd, int value) noexcept {
    // Error.
    return 1;
}

// todo
void* _valloc(size_t size) noexcept {
    if (s_pagesize == 0) {
        s_pagesize = getpagesize();
    }

    size = (size + s_pagesize - 1) & ~(s_pagesize - 1);
    void* result = _malloc(size);
    ASSERT((reinterpret_cast<uintptr_t>(result) % s_pagesize) == 0);

    return result;
}

void* _pvalloc(size_t size) noexcept {
    if (s_pagesize == 0) {
        s_pagesize = getpagesize();
    }
    // pvalloc(0) should allocate one page
    if (size == 0) {
        size = s_pagesize;
    }
    size = (size + s_pagesize - 1) & ~(s_pagesize - 1);
    void* result = _malloc(size);
    ASSERT((reinterpret_cast<uintptr_t>(result) % s_pagesize) == 0);

    return result;
}

void* _memalign(size_t align, size_t size) noexcept {
    ASSERT(0); // todo
    return nullptr;
}


} // extern "C"

} // namespace sitcmalloc