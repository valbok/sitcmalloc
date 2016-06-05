/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <PageMap.h>
#include <gtest/gtest.h>
#include <iostream>

using namespace std;
using namespace sitcmalloc;

const int magic = 16334848;
int data[magic];

TEST(PageMapTest, testInstance) {
    EXPECT_TRUE(&PageMap::instance() == &PageMap::instance());
}

TEST(PageMapTest, testSet) {
    for (int i = 0; i < magic; ++i) {
        data[i] = i + 1;
        EXPECT_TRUE(PageMap::set(i, &data[i]));
        EXPECT_EQ(&data[i], PageMap::get(i));
        EXPECT_EQ(data[i], *static_cast<int*>(PageMap::get(i)));
    }
}

TEST(PageMapTest, testSetAgain) {
    for (int i = 0; i < magic; ++i) {
        EXPECT_EQ(data[i], *static_cast<int*>(PageMap::get(i)));
    }
}
