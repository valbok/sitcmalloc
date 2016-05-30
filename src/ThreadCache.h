#ifndef SITCMALLOC_THREAD_CACHE_H
#define SITCMALLOC_THREAD_CACHE_H

#include <stddef.h> // for size_t, nullptr, ptrdiff_t
#include "common.h"
#include "Block.h"

namespace sitcmalloc {

/**
 *
 */
class ThreadCache {
public:
    static ThreadCache& instance();
    void* alloc(size_t size);
    void free(void* ptr);
private:
	ThreadCache() {}
	ThreadCache(const ThreadCache&) = delete;
	ThreadCache& operator=(const ThreadCache&) = delete;

    struct Root : Block {
        Root() : m_len(0), m_maxLen(0) {}
        inline void prepend(Block* block, size_t num) {
            if (empty()) {
                m_maxLen = num;
            }
            Block::prepend(block);
            m_len += num;

        }
        inline void* pop() {
            --m_len;
            return Block::pop();
        }
        size_t m_len;
        size_t m_maxLen;
    };
	Root m_list[CLASSES];
};


}  // namespace sitcmalloc

#endif  // SITCMALLOC_THREAD_CACHE_H
