#ifndef SITCMALLOC_SPAN_H
#define SITCMALLOC_SPAN_H

#include <stddef.h> // for size_t, nullptr, ptrdiff_t

namespace sitcmalloc {

class Block;

/**
 *
 */
class Span {
public:
    Span(size_t pages = 0);

    static Span* create(void* p, size_t pages);
    inline size_t pages() const {
        return m_pages;
    }

    inline Block* block() {
        return reinterpret_cast<Block*>(data());
    }

    inline Span* next() const {
        return m_next;
    }

    inline Span* prev() const {
        return m_prev;
    }

    void prepend(Span* span);
    void remove();
    inline bool empty() const {
        return m_prev == nullptr && m_next == nullptr;
    }

    // | this   | result with requested pages
    // |--------|++++|
    Span* carve(size_t pages);

    inline void use() {
        m_inUse = 1;
    }

    inline void free() {
        m_inUse = 0;
    }

    inline bool inUse() const {
        return m_inUse;
    }

    size_t sizeClass() const {
        return m_sizeClass;
    }

    size_t split(size_t size, size_t sizeClass, Block** start, Block** end);
private:
    inline void* data() {
        return &m_prev;
    }

    size_t m_pages;
    unsigned char m_sizeClass : 7;
    unsigned char m_inUse : 1;
    Span* m_prev;
    Span* m_next;
};


}  // namespace sitcmalloc

#endif  // SITCMALLOC_SPAN_H
