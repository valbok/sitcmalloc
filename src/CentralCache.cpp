
#include "CentralCache.h"
#include "PageHeap.h"
#include "common.h"
#include "Block.h"
#include <mutex>

#include <iostream>
namespace sitcmalloc {

CentralCache& CentralCache::instance(size_t size) {
	static CentralCache list[CLASSES];

    const size_t sizeClass = sizeToClass(size);
	CentralCache& result = list[sizeClass];
	result.m_sizeClass = sizeClass;
    // @todo Cache it
    result.m_size = sizeClass == LARGE_CLASS ? pageAligned(size) : classToSize(sizeClass);
    result.m_pages = sizeClass == LARGE_CLASS ? sizeToMinPages(result.m_size + sizeof(Span), 1) : sizeToMinPages(result.m_size, 8);

	return result;
}

Span* CentralCache::fetch() {
    Span* span = m_span.vNext();
    if (span) {
        span->vRemove();
    }
    return span;
}

Block* CentralCache::alloc(size_t& num) {
    Span* span = fetch();
    Block* result = span ? span->block() : nullptr;
    if (!result) {
        Span* s = PageHeap::instance().alloc(m_pages);
        if (s) {
            result = s->split(m_size, m_sizeClass, num);
        }
    }

    return result;
}

}  // namespace sitcmalloc

