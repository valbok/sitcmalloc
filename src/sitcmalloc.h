/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#ifndef SITCMALLOC_H
#define SITCMALLOC_H

#include <stddef.h> // for size_t, nullptr, ptrdiff_t

namespace std {
struct nothrow_t;
}

namespace sitcmalloc {

extern "C" {
    void* _new(size_t size);
    void* _new_nothrow(size_t size, const std::nothrow_t&) noexcept;
    void _delete(void* p) noexcept;
    void _delete_nothrow(void* p, const std::nothrow_t&) noexcept;
    void* _new_array(size_t size);
    void* _new_array_nothrow(size_t size, const std::nothrow_t&) noexcept;
    void _delete_array(void* p) noexcept;
    void _delete_array_nothrow(void* p, const std::nothrow_t&) noexcept;

    void* _malloc(size_t size) noexcept;
    void _free(void* ptr) noexcept;
    void* _realloc(void* ptr, size_t size) noexcept;
    void* _calloc(size_t nmemb, size_t size) noexcept;
    void _cfree(void* ptr) noexcept;

    void* _memalign(size_t __alignment, size_t __size) noexcept;
    int _posix_memalign(void** ptr, size_t align, size_t size) noexcept;
    void* _valloc(size_t size) noexcept;
    void* _pvalloc(size_t size) noexcept;

    void _malloc_stats(void) noexcept;
    int _mallopt(int cmd, int value) noexcept;
    struct mallinfo _mallinfo(void) noexcept;
    size_t _malloc_size(void* ptr) noexcept;

} // extern "C"

} // namespace sitcmalloc

#endif // SITCMALLOC_H