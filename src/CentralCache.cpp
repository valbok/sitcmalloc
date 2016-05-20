
#include "CentralCache.h"
#include "PageHeap.h"
#include "common.h"

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

void* CentralCache::alloc() {
    Span* span = fetch();
    void* result = span ? span->data() : nullptr;
    if (!result) {
        Span* s = PageHeap::instance().alloc(m_pages);
        if (!s) {
            return nullptr;
        }
        m_span.vPrepend(s);

        size_t num = s->split(m_size, result);
        if (!num) {
            return nullptr;
        }
    } 
    return result;
}

}  // namespace sitcmalloc

