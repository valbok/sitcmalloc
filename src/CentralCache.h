#ifndef SITCMALLOC_CENTRAL_CACHE_H
#define SITCMALLOC_CENTRAL_CACHE_H

#include "Span.h"
#include <stddef.h> // for size_t, nullptr, ptrdiff_t

namespace sitcmalloc {

class Block;

/**
 *
 */
class CentralCache {
public:
    static CentralCache& instance(size_t sizeClass);
    void init(size_t sizeClass);
    Block* alloc();
private:
	CentralCache() : m_sizeClass(0), m_pages(0), m_size(0) {}
	CentralCache(const CentralCache&);
	CentralCache& operator=(const CentralCache&);

	Span* fetch();

	Span m_span;
	size_t m_sizeClass;
	size_t m_pages;
	size_t m_size;
};


}  // namespace sitcmalloc

#endif  // SITCMALLOC_CENTRAL_CACHE_H
