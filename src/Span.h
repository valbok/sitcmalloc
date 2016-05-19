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
    Span(): m_prev(nullptr), m_next(nullptr), m_pages(0), m_inUse(false), m_data(nullptr) {

    }
    static Span* create(void* p, size_t pages) {
        if (!p) {
            return nullptr;
        }
        Span* result = reinterpret_cast<Span*>(p);
        result->m_prev = result->m_next = nullptr;
        result->m_data = nullptr;
        result->m_pages = pages;
        return result;
    }

    inline size_t pages(size_t pages = 0) {
        if (pages) {
            m_pages = pages;
        }
        return m_pages;
    }

    inline void* data() {
        return &m_data;
    }

    Span* pNext() const {
        return m_next;
    }

    Span* pPrev() const {
        return m_prev;
    }

    void use() {
        m_inUse = true;
    }

    void free() {
        m_inUse = false;
    }

    bool inUse() const {
        return m_inUse;
    }

    void pPrependToLeft(Span* span) {
        ASSERT(span);
        ASSERT(span->m_next == nullptr);
        ASSERT(span->m_prev == nullptr);

        span->m_prev = m_prev;
        span->m_next = this;
        if (m_prev) {
            m_prev->m_next = span;
        }

        m_prev = span;
    }

    void pPrepend(Span* span) {
        ASSERT(span);
        ASSERT(span->m_next == nullptr);
        ASSERT(span->m_prev == nullptr);

        span->m_next = m_next;
        span->m_prev = this;
        if (m_next) {
            m_next->m_prev = span;
        }

        m_next = span;
    }

    void vPrepend(Span* span) {
        ASSERT(span);
        ASSERT(!span->inUse());

        Span** nextPtr = nextData();
        Span** prevPtr = prevData();
        Span* next = *nextPtr;
        Span* prev = *prevPtr;

        *span->nextData() = next;
        *span->prevData() = this;
        if (next) {
            *next->prevData() = span;
        }
        *nextPtr = span;
    }

    void vRemove() {
        Span** nextPtr = nextData();
        Span** prevPtr = prevData();
        Span* next = *nextPtr;
        Span* prev = *prevPtr;

        if (next) {
            *next->prevData() = prev;
        }
        if (prev) {
            *prev->nextData() = next;
        }

        *nextPtr = nullptr;
        *prevPtr = nullptr;
    }

    Span* vNext() {
        return *nextData();
    }

    Span* vPrev() {
        return *prevData();
    }

    bool empty() {
        return m_data == nullptr;
    }

    Span* carve(size_t pages) {
        Span* result = this;
        size_t delta = m_pages - pages;
        if (delta) {
            char* ptr = reinterpret_cast<char*>(this);
            result = Span::create(ptr + pagesToBytes(delta), pages);
            ASSERT(result);

            pPrepend(result);

            m_pages = delta;
        }

        return result;
    }

    void pRemove() {
        if (m_prev) {
            m_prev->m_next = m_next;
        }
        if (m_next) {
            m_next->m_prev = m_prev;
        }
        m_prev = nullptr;
        m_next = nullptr;
    }
private:

    Span** nextData() {
        return reinterpret_cast<Span**>(data());
    }

    Span** prevData() {
        return reinterpret_cast<Span**>((Span*)data() + 1);
    }


    Span* m_prev;
    Span* m_next;
    size_t m_pages;
    bool m_inUse;
    void* m_data;

};


}  // namespace sitcmalloc

#endif  // SITCMALLOC_SPAN_H
