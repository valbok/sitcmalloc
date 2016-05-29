#include "Span.h"
#include <common.h>
#include <iostream>
#include "Block.h"

using namespace std;


namespace sitcmalloc {

Span::Span(size_t pages):
    m_pPrev(nullptr),
    m_pNext(nullptr),
    m_pages(pages),
    m_sizeClass(127),
    m_inUse(0),
    m_vPrev(nullptr),
    m_vNext(nullptr) {
}

Span* Span::create(void* p, size_t pages) {
    if (!p) {
        return nullptr;
    }
    Span* result = reinterpret_cast<Span*>(p);
    result->m_pPrev = result->m_pNext = nullptr;
    result->m_vPrev = result->m_vNext = nullptr;
    result->m_pages = pages;

    return result;
}

size_t Span::pages() const {
    return m_pages;
}

void* Span::data() {
    return &m_vPrev;
}

Block* Span::block() {
    return reinterpret_cast<Block*>(data());
}


Span* Span::pNext() const {
    return m_pNext;
}

Span* Span::pPrev() const {

    return m_pPrev;
}

void Span::use() {
    m_inUse = 1;
}

void Span::free() {
    m_inUse = 0;
}

bool Span::inUse() const {
    return m_inUse;
}

size_t Span::sizeClass() const {
    return m_sizeClass;
}

void Span::pPrependToLeft(Span* span) {
    ASSERT(span);
    ASSERT(span->m_pNext == nullptr);
    ASSERT(span->m_pPrev == nullptr);

    span->m_pPrev = m_pPrev;
    span->m_pNext = this;
    if (m_pPrev) {
        m_pPrev->m_pNext = span;
    }

    m_pPrev = span;
}

void Span::pPrepend(Span* span) {
    ASSERT(span);
    ASSERT(span->m_pNext == nullptr);
    ASSERT(span->m_pPrev == nullptr);

    span->m_pNext = m_pNext;
    span->m_pPrev = this;
    if (m_pNext) {
        m_pNext->m_pPrev = span;
    }

    m_pNext = span;
}

void Span::vPrepend(Span* span) {
    ASSERT(span);
    ASSERT(!span->inUse());

    span->m_vNext = m_vNext;
    span->m_vPrev = this;
    if (m_vNext) {
        m_vNext->m_vPrev = span;
    }
    m_vNext = span;
}

void Span::vRemove() {
    if (m_vNext) {
        m_vNext->m_vPrev = m_vPrev;
    }
    if (m_vPrev) {
        m_vPrev->m_vNext = m_vNext;
    }

    m_vNext = nullptr;
    m_vPrev = nullptr;
}

Span* Span::vNext() const {
    return m_vNext;
}

Span* Span::vPrev() const {
    return m_vPrev;
}

bool Span::vEmpty() const {
    return m_vPrev == nullptr && m_vNext == nullptr;
}

Span* Span::carve(size_t pages) {
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

void Span::pRemove() {
    if (m_pPrev) {
        m_pPrev->m_pNext = m_pNext;
    }
    if (m_pNext) {
        m_pNext->m_pPrev = m_pPrev;
    }
    m_pPrev = nullptr;
    m_pNext = nullptr;
}

Block* Span::split(size_t size, size_t sizeClass) {
    ASSERT(inUse());
    Block* result = reinterpret_cast<Block*>(data());
    size_t num = result->split(reinterpret_cast<char*>(this) + pagesToBytes(m_pages), size);
    m_sizeClass = sizeClass & 0b1111111;
    return num ? result : nullptr;
}

}  // namespace sitcmalloc

