
#include "CentralCache.h"
#include "PageHeap.h"
#include "common.h"
#include "Block.h"


#include <iostream>
namespace sitcmalloc {

CentralCache& CentralCache::instance(size_t size) {
	static CentralCache list[CLASSES];

    const size_t sizeClass = sizeToClass(size);
	CentralCache& result = list[sizeClass];
	result.m_sizeClass = sizeClass;
    // @todo Cache it
    result.m_size = sizeClass == LARGE_CLASS ? alignment(size) : classToSize(sizeClass);
    result.m_pages = sizeToMinPages(result.m_size);

	return result;
}

Span* CentralCache::fetch() {
    Span* span = m_span.vNext();
    if (span) {
        span->vRemove();
    }
    return span;
}

Block* CentralCache::alloc() {
    std::lock_guard<std::mutex> lock(m_mutex);
    Span* span = fetch();
    Block* result = span ? span->block() : nullptr;
    if (!result) {        
        Span* s = PageHeap::instance().alloc(m_pages);        
        if (s) {
            result = s->split(m_size);
        }
    }

    return result;
}

}  // namespace sitcmalloc

