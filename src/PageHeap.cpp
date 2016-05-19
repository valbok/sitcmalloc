#include "PageHeap.h"
#include "sys_alloc.h"
#include <unistd.h> 
#include <iostream>
using namespace std;
namespace sitcmalloc {


Span* PageHeap::allocFromSystem(size_t pages) {
	if (pages < MIN_ALLOCATED_PAGES) {
    	pages = MIN_ALLOCATED_PAGES;
	}
    void* ptr = sys_alloc(pagesToBytes(pages));

    return Span::create(ptr, pages);
}

Span* PageHeap::search(size_t pages) {
    Span* result = nullptr;
    cout << "SEACHING " << pages << endl;
    for (unsigned i = pages - 1; i < MAX_PAGES; ++i) {
        Span* span = &m_pageSpans[i];
        if (!span->empty()) {
            Span* data = span->dataSpan();
            cout << "   FOUND " << i+1 << " pages " << "data->pages="<<data->pages() << endl;
            data->removeData();
            cout << "   removedData empty=" << span->empty() <<"=" << span->dataSpan() <<endl;
            result = data->carve(pages);

            merge(data);
            merge(result);

            cout << "search result pages data=" << data->pages() << ":result="<<result->pages() << endl;
            break;
        }
    }

    return result;
}

void PageHeap::merge(Span* span) {
    if (!span) {
        return;
    }
    if (span->pages() <= MAX_PAGES) {
        m_pageSpans[span->pages() - 1].prependToData(span);
        
        /*Span * s = m_pageSpans[span->pages() - 1].nextSpan();
        while (s) {
            cout << s << endl;
            s = s->nextSpan();
        }*/
    } else {
        m_span.prependToData(span);
    }
}

Span* PageHeap::alloc(size_t pages) {
    cout << "ALLOC\n";
    Span* result = search(pages);
    if (!result) {
        cout << "NOT FOUND allocating from system\n";
        result = allocFromSystem(pages);
        cout << " ALLOCATED " << result << endl;;
        if (result) {
            m_tail.prependToLeft(result);
        
            merge(result);
            cout << "    trying to search again\n";
            result = search(pages);
            cout << "alloc result pages " << result <<":"<<result->pages() << endl;
        }

    }
    if (result) {
        result->use();
        result->removeData();
    }

    return result;
}

void PageHeap::free(Span* span) {

}

}  // namespace sitcmalloc
