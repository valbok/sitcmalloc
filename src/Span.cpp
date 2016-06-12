/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#include "Span.h"
#include "common.h"
#include "Block.h"

namespace sitcmalloc {

const char INVALID_SIZE_CLASS = 127;

Span::Span(size_t pages)
    : m_pages(pages)
    , m_blocks(0)
    , m_sizeClass(INVALID_SIZE_CLASS)
    , m_inUse(0)
    , m_prev(nullptr)
    , m_next(nullptr)
    {
}

Span* Span::create(void* p, size_t pages) {
    Span* result = reinterpret_cast<Span*>(p);
    result->m_pages = pages;
    result->m_blocks = 0;
    result->m_sizeClass = INVALID_SIZE_CLASS;
    result->m_inUse = 0;
    result->m_prev = nullptr;
    result->m_next = nullptr;

    return result;
}

void Span::prepend(Span* span) {
    ASSERT(span);

    span->m_next = m_next;
    span->m_prev = this;
    if (m_next) {
        m_next->m_prev = span;
    }
    m_next = span;
}

void Span::remove() {
    if (m_next) {
        m_next->m_prev = m_prev;
    }
    if (m_prev) {
        m_prev->m_next = m_next;
    }

    m_next = nullptr;
    m_prev = nullptr;
}

Span* Span::carve(size_t pages) {
    ASSERT(!inUse());
    Span* result = this;
    size_t delta = m_pages - pages;
    if (delta) {
        char* ptr = reinterpret_cast<char*>(this);
        result = Span::create(ptr + pagesToBytes(delta), pages);
        ASSERT(result);
        result->free();

        m_pages = delta;
    }

    return result;
}

size_t Span::split(size_t size, size_t sizeClass, Block** start, Block** end) {
    ASSERT(inUse());
    Block* result = block();
    m_blocks = result->split(reinterpret_cast<char*>(this) + pagesToBytes(m_pages), size, end);
    m_sizeClass = sizeClass & 0b1111111;
    *start = result;

    return m_blocks;
}

}  // namespace sitcmalloc

