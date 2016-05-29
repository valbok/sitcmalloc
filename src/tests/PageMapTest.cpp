/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <gtest/gtest.h>
#include <PageMap.h>
#include <common.h>
#include <iostream>

using namespace std;
using namespace sitcmalloc;

TEST(PageMapTest, testInstance) {
    EXPECT_TRUE(&PageMap::instance() == &PageMap::instance());
}

TEST(PageMapTest, testSet) {
    for (int i = 0; i < 1000; ++i) {
        int j = i + 1;
        EXPECT_TRUE(PageMap::set(i, &j));
        EXPECT_EQ(&j, PageMap::get(i));
        EXPECT_EQ(j, *static_cast<int*>(PageMap::get(i)));
    }
}
