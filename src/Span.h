#ifndef SITCMALLOC_SPAN_H
#define SITCMALLOC_SPAN_H

#include <stddef.h> // for size_t, nullptr, ptrdiff_t
#include <common.h>
#include <iostream>

using namespace std;
namespace sitcmalloc {

/**
 *
 */
class Span {
public:
    Span();

    static Span* create(void* p, size_t pages);

    size_t pages() const;
    void* data();

    Span* pNext() const;
    Span* pPrev() const;

    void use();
    void free();

    bool inUse() const;

    void pPrependToLeft(Span* span);
    void pPrepend(Span* span);
    void pRemove();

    Span* vNext();
    Span* vPrev();


    void vPrepend(Span* span);
    void vRemove();

    bool vEmpty();

    Span* carve(size_t pages);

private:

    Span** nextData();
    Span** prevData();

    Span* m_prev;
    Span* m_next;
    size_t m_pages;
    bool m_inUse;
    void* m_data;
};


}  // namespace sitcmalloc

#endif  // SITCMALLOC_SPAN_H
