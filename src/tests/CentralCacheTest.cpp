/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <gtest/gtest.h>

#include <CentralCache.h>

#include <iostream>
#include <cstring>

using namespace std;
using namespace sitcmalloc;

TEST(CentralCacheTest, testInstance) {
    EXPECT_TRUE(&CentralCache::instance(1) == &CentralCache::instance(1));
    EXPECT_TRUE(&CentralCache::instance(1) == &CentralCache::instance(2));
    EXPECT_TRUE(&CentralCache::instance(1) == &CentralCache::instance(8));
    EXPECT_TRUE(&CentralCache::instance(1) != &CentralCache::instance(9));
}

TEST(CentralCacheTest, testFetch) {
    CentralCache& c = CentralCache::instance(1);
    Block* b = nullptr, *end = nullptr;
    size_t num = c.alloc(&b, &end);
    EXPECT_TRUE(b != nullptr);
    EXPECT_TRUE(end != nullptr);
    EXPECT_FALSE(b->empty());
    EXPECT_EQ(1022, num);
}

TEST(CentralCacheTest, testAllocLarge) {
    for (int i = 1; i < 1000; ++i) {
        CentralCache& c = CentralCache::instance(MAX_CLASS_SIZE + i);
        Block* b = nullptr, *end = nullptr;
        size_t num = c.alloc(&b, &end);
        EXPECT_TRUE(b != nullptr);
        EXPECT_TRUE(end != nullptr);
        EXPECT_TRUE(b->empty());
        EXPECT_EQ(1, num);
    }
}

