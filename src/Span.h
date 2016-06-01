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
    size_t pages() const;

    Block* block();

    Span* next() const;
    Span* prev() const;
    void prepend(Span* span);
    void remove();
    bool empty() const;

    // | this   | result with requested pages
    // |--------|++++|
    Span* carve(size_t pages);

    void use();
    void free();
    bool inUse() const;
    size_t sizeClass() const;

    size_t split(size_t size, size_t sizeClass, Block** start, Block** end);
private:
    void* data();

    size_t m_pages;
    unsigned char m_sizeClass : 7;
    unsigned char m_inUse : 1;
    Span* m_prev;
    Span* m_next;
};


}  // namespace sitcmalloc

#endif  // SITCMALLOC_SPAN_H
