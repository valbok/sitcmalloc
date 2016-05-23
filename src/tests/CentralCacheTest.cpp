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
    EXPECT_FALSE(&CentralCache::instance(1) == &CentralCache::instance(2));
}

TEST(CentralCacheTest, testFetch) {
    CentralCache& c = CentralCache::instance(1);
    Block* b = c.alloc();
    EXPECT_TRUE(b != nullptr);
    EXPECT_FALSE(b->empty());

}
