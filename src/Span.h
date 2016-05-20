#ifndef SITCMALLOC_SPAN_H
#define SITCMALLOC_SPAN_H

#include <stddef.h> // for size_t, nullptr, ptrdiff_t

namespace sitcmalloc {

/**
 *
 */
class Span {
public:
    Span(size_t pages = 0);

    static Span* create(void* p, size_t pages);

    size_t pages() const;
    void* data();

    Span* pNext() const;
    Span* pPrev() const;
    void pPrependToLeft(Span* span);
    void pPrepend(Span* span);
    void pRemove();

    Span* vNext() const;
    Span* vPrev() const;
    void vPrepend(Span* span);
    void vRemove();
    bool vEmpty() const;

    Span* carve(size_t pages);

    void use();
    void free();
    bool inUse() const;

private:

    Span* m_pPrev;
    Span* m_pNext;
    size_t m_pages;
    bool m_inUse;
    Span* m_vPrev;
    Span* m_vNext;
    void* m_data;
};


}  // namespace sitcmalloc

#endif  // SITCMALLOC_SPAN_H
