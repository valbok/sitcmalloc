
#include "CentralCache.h"
#include "PageHeap.h"
#include "common.h"
#include "Block.h"


#include <iostream>
namespace sitcmalloc {

CentralCache& CentralCache::instance(size_t sizeClass) {
	static CentralCache list[CLASSES];
	CentralCache& result = list[sizeClass];
	result.m_sizeClass = sizeClass;
    result.m_pages = classToPages(sizeClass);
    result.m_size = classToSize(sizeClass);
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
    Span* span = fetch();
    Block* result = span ? span->block() : nullptr;
    if (!result) {
        std::lock_guard<std::mutex> lock(m_mutex);
        Span* s = PageHeap::instance().alloc(m_pages);        
        if (s) {
            result = s->split(m_size);
        }
    }
    return result;
}

}  // namespace sitcmalloc

