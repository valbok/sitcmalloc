#ifndef SITCMALLOC_PAGE_MAP_H
#define SITCMALLOC_PAGE_MAP_H

#include <stddef.h> // for size_t, nullptr, ptrdiff_t
#include <string.h>                     // for memset
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

    static bool set(uintptr_t key, void* value) {
        return instance()._set(key, value);
    }

    static void* get(uintptr_t key) {
        return instance()._get(key);
    }

private:

    static const int NODE_BITS = 12;
    static const int NODE_SIZE = 1 << NODE_BITS;
    static const int LEAF_BITS = 11;
    static const int LEAF_SIZE = 1 << LEAF_BITS;

    PageMap() = default;
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
