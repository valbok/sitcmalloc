#ifndef SITCMALLOC_BLOCK_H
#define SITCMALLOC_BLOCK_H

#include <stddef.h> // for size_t, nullptr, ptrdiff_t

namespace sitcmalloc {

/**
 *
 */
class Block {
public:

    void* next();
    size_t split(void*, size_t);
  void* m_next;
private:
    Block() {}
    Block(const Block&);
    Block& operator=(const Block&);


};


}  // namespace sitcmalloc

#endif  // SITCMALLOC_BLOCK_H
