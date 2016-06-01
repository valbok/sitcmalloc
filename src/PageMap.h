#ifndef SITCMALLOC_PAGE_MAP_H
#define SITCMALLOC_PAGE_MAP_H

#include "Span.h"
#include <stddef.h> // for size_t, nullptr, ptrdiff_t
#include <string.h> // for memset
#include "common.h"
#include "sys_alloc.h"
#include <iostream>
using namespace std;

namespace sitcmalloc {

/**
 *
 */
class PageMap {
public:

    static PageMap& instance() {
        static PageMap map;
        return map;
    }

    static inline bool set(uintptr_t key, void* value) {
        return instance()._set(key, value);
    }

    static inline bool set(void* ptr, void* value) {
        return instance()._set(key(ptr), value);
    }

    static inline void* get(uintptr_t key) {
        return instance()._get(key);
    }

    static inline void* get(void* ptr) {
        return instance()._get(key(ptr));
    }

    static inline uintptr_t key(void* ptr) {
        return reinterpret_cast<uintptr_t>(ptr) >> PAGE_SHIFT;
    }

    inline static void store(Span* s) {
        storeSpan(s);
    }

    inline static void remove(Span* s) {
        storeSpan(s, true);
    }

private:

    static const int NODE_BITS = 12;
    static const int NODE_SIZE = 1 << NODE_BITS;
    static const int LEAF_BITS = 11;
    static const int LEAF_SIZE = 1 << LEAF_BITS;

    PageMap() = default;

    inline static void storeSpan(Span* s, bool remove = false) {
        void* value = remove ? nullptr : s;
        //cout << s->pages()<<":"<<s <<"-"<< s + pagesToBytes(s->pages()) <<endl;
        for (size_t i = 0; i < pagesToBytes(s->pages()); i += pagesToBytes(1)) {
            uintptr_t start = reinterpret_cast<uintptr_t>(s);
            void* offset = reinterpret_cast<void*>(start + i);
            //cout <<key(offset)<<endl;
            set(offset, value);
        }
    }

    bool _set(uintptr_t key, void* ptr) {
        uintptr_t node, leaf, value;
        keys(key, node, leaf, value);
        if (m_nodes[node] == nullptr) {
            m_nodes[node] = create<Node>();
        }
        if (m_nodes[node] == nullptr) {
            return false;
        }
        if (m_nodes[node]->leaves[leaf] == nullptr) {
            m_nodes[node]->leaves[leaf] = create<Leaf>();
        }
        if (m_nodes[node]->leaves[leaf] == nullptr) {
            return false;
        }

        m_nodes[node]->leaves[leaf]->values[value] = ptr;
        return true;
    }

    void* _get(uintptr_t key) {
        uintptr_t node, leaf, value;
        keys(key, node, leaf, value);
        if (m_nodes[node] == nullptr) {
            return nullptr;
        }
        if (m_nodes[node]->leaves[leaf] == nullptr) {
            return nullptr;
        }

        return m_nodes[node]->leaves[leaf]->values[value];
    }

    static inline void keys(uintptr_t key, uintptr_t& node, uintptr_t& leaf, uintptr_t& value) {
        node = key >> (LEAF_BITS + NODE_BITS);
        leaf = (key >> LEAF_BITS) & (NODE_SIZE - 1);
        value = key & (LEAF_SIZE- 1);
    }
    struct Leaf {
        void* values[1 << LEAF_BITS] = {nullptr};
    };
    struct Node {
        Leaf* leaves[1 << NODE_BITS] = {nullptr};
    };

    template<class T>
    static inline T* create() {
        return reinterpret_cast<T*>(sys_alloc(sizeof(T)));
    }

    Node* m_nodes[NODE_SIZE] = {nullptr};

};

}  // namespace sitcmalloc

#endif  // SITCMALLOC_PAGE_MAP_H
