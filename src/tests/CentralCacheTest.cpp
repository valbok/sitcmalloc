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
    size_t num;
    Block* b = c.alloc(num);
    EXPECT_TRUE(b != nullptr);
    EXPECT_FALSE(b->empty());
    EXPECT_EQ(1020, num);
}

TEST(CentralCacheTest, testAllocLarge) {
    CentralCache& c = CentralCache::instance(MAX_CLASS_SIZE + 1);
    size_t num;
    Block* b = c.alloc(num);
    EXPECT_TRUE(b != nullptr);
    EXPECT_TRUE(b->empty());
    EXPECT_EQ(1, num);

}
