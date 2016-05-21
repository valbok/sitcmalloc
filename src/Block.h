#ifndef SITCMALLOC_BLOCK_H
#define SITCMALLOC_BLOCK_H

#include <stddef.h> // for size_t, nullptr, ptrdiff_t

namespace sitcmalloc {

/**
 *
 */
class Block {
public:

    Block* next();
    size_t split(void*, size_t);

private:
    Block() {}
    Block(const Block&);
    Block& operator=(const Block&);

    void* m_next;
};


}  // namespace sitcmalloc

#endif  // SITCMALLOC_BLOCK_H
