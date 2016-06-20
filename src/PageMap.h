/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#ifndef SITCMALLOC_PAGE_MAP_H
#define SITCMALLOC_PAGE_MAP_H

#include "Span.h"
#include "common.h"
#include "sys_alloc.h"
#include <stddef.h> // for size_t, nullptr, ptrdiff_t

#ifndef __x86_64__
#error System not supported
#endif

namespace sitcmalloc {

/**
 * Radix tree to map address to its span object.
 */
class PageMap {
public:

    /**
     * Creates instance of the map.
     */
    static PageMap& instance() {
        static PageMap map;
        return map;
    }

    /**
     * Sets address for a key.
     */
    static inline bool set(uintptr_t key, void* value) {
        return instance()._set(key, value);
    }

    /**
     * Sets address for another address.
     */
    static inline bool set(void* ptr, void* value) {
        return instance()._set(key(ptr), value);
    }

    /**
     * Gets address by key.
     */
    static inline void* get(uintptr_t key) {
        return instance()._get(key);
    }

    /**
     * Gets address by address.
     */
    static inline void* get(void* ptr) {
        return instance()._get(key(ptr));
    }

    /**
     * Creates a key by an address.
     */
    static inline uintptr_t key(void* ptr) {
        return reinterpret_cast<uintptr_t>(ptr) >> PAGE_SHIFT;
    }

    /**
     * Associates all addresses of a span with this span.
     */
    inline static void store(Span* s) {
        setSpan(s, s);
    }

    /**
     * Removes the association of the span and its address range.
     */
    inline static void remove(Span* s) {
        setSpan(s, nullptr);
    }

private:

    /**
     * From 64 bits only 48 is used to translate virtual to physical address.
     * The top 16 are thus unused.
     *
     * 64 bits can be represented by:
     * |16 unused bits | 13 page bits| first node | second     | leaf     |
     * xxxxxxxxxxxxxxxx.-------------.000000000000.000000000000.00000000000
     *
     * 13 bits are also not used because spans allocated by pages
     * and never less than one page which consist of 13 bits.
     */

    /**
     * Number of bits of nodes.
     */
    static const int NODE_BITS = 12;

    /**
     * Bytes in nodes.
     */
    static const int NODE_SIZE = 1 << NODE_BITS;

    /**
     * Number of bits in leaves.
     */
    static const int LEAF_BITS = 11;

    /**
     * Bytes in leaves.
     */
    static const int LEAF_SIZE = 1 << LEAF_BITS;

    /**
     * Default constructor
     */
    PageMap() = default;

    /**
     * Associates span with its byte ranges.
     */
    inline static void setSpan(Span* s, void* value) {
        uintptr_t start = reinterpret_cast<uintptr_t>(s);
        for (size_t i = 0; i < pagesToBytes(s->pages()); i += PAGE_SIZE) {
            void* offset = reinterpret_cast<void*>(start + i);
            set(offset, value);
        }
    }

    /**
     * Sets value by key.
     */
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

    /**
     * Gets value by key.
     */
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

    /**
     * Creates array indices by a key.
     */
    static inline void keys(uintptr_t key, uintptr_t& node, uintptr_t& leaf, uintptr_t& value) {
        node = key >> (LEAF_BITS + NODE_BITS);
        leaf = (key >> LEAF_BITS) & (NODE_SIZE - 1);
        value = key & (LEAF_SIZE - 1);
    }

    /**
     * Leaf node.
     */
    struct Leaf {
        void* values[1 << LEAF_BITS] = {nullptr};
    };

    /**
     * Interior node.
     */
    struct Node {
        Leaf* leaves[1 << NODE_BITS] = {nullptr};
    };

    /**
     * Allocates memory for requested type.
     */
    template<class T>
    static inline T* create() {
        return reinterpret_cast<T*>(sys_alloc(sizeof(T)));
    }

    /**
     * Array of first nodes.
     */
    Node* m_nodes[NODE_SIZE] = {nullptr};

};

}  // namespace sitcmalloc

#endif  // SITCMALLOC_PAGE_MAP_H
