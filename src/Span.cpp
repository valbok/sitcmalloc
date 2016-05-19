#include "Span.h"
#include <common.h>
#include <iostream>

using namespace std;
namespace sitcmalloc {

Span::Span():
    m_prev(nullptr),
    m_next(nullptr),
    m_pages(0),
    m_inUse(false),
    m_data(nullptr) {
}

Span* Span::create(void* p, size_t pages) {
    if (!p) {
        return nullptr;
    }
    Span* result = reinterpret_cast<Span*>(p);
    result->m_prev = result->m_next = nullptr;
    result->m_data = nullptr;
    result->m_pages = pages;

    return result;
}

size_t Span::pages() const {
    return m_pages;
}

void* Span::data() {
    return &m_data;
}

Span* Span::pNext() const {
    return m_next;
}

Span* Span::pPrev() const {
    return m_prev;
}

void Span::use() {
    m_inUse = true;
}

void Span::free() {
    m_inUse = false;
}

bool Span::inUse() const {
    return m_inUse;
}

void Span::pPrependToLeft(Span* span) {
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

void Span::pPrepend(Span* span) {
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

void Span::vPrepend(Span* span) {
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

void Span::vRemove() {
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

Span* Span::vNext() {
    return *nextData();
}

Span* Span::vPrev() {
    return *prevData();
}

bool Span::vEmpty() {
    return m_data == nullptr;
}

Span* Span::carve(size_t pages) {
    Span* result = this;
    size_t delta = m_pages - pages;
    if (delta) {
        cout << "delta " << delta << endl;
        char* ptr = reinterpret_cast<char*>(this);
        result = Span::create(ptr + pagesToBytes(delta), pages);
        ASSERT(result);

        pPrepend(result);

        m_pages = delta;
    }

    return result;
}

void Span::pRemove() {
    if (m_prev) {
        m_prev->m_next = m_next;
    }
    if (m_next) {
        m_next->m_prev = m_prev;
    }
    m_prev = nullptr;
    m_next = nullptr;
}

Span** Span::nextData() {
    return reinterpret_cast<Span**>(data());
}

Span** Span::prevData() {
    return reinterpret_cast<Span**>((Span*)data() + 1);
}


}  // namespace sitcmalloc

