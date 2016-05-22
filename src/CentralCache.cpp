
#include "CentralCache.h"
#include "PageHeap.h"
#include "common.h"
#include "Block.h"

namespace sitcmalloc {

CentralCache& CentralCache::instance(size_t sizeClass) {
	static CentralCache list[CLASSES];
	CentralCache& result = list[sizeClass];
	result.m_sizeClass = sizeClass;
    result.m_pages = classToPages(result.m_sizeClass);
    result.m_size = sizeToClass(sizeClass);
	return result;
}

Span* CentralCache::fetch() {
    Span* span = m_span.vNext();
    if (span) {
        span->vRemove();
        return span;
    }
    return nullptr;
}

Block* CentralCache::alloc() {
    Span* span = fetch();
    Block* result = span ? span->block() : nullptr;
    if (!result) {
        Span* s = PageHeap::instance().alloc(m_pages);
        if (s) {
            m_span.vPrepend(s);
            result = s->split(m_size);
        }
    }
    return result;
}

}  // namespace sitcmalloc

