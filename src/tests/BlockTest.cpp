/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <gtest/gtest.h>
#include <Block.h>
#include <common.h>
#include <iostream>

using namespace std;
using namespace sitcmalloc;


TEST(BlockTest, testSplit) {
    const int pages = 1;
    char a[pagesToBytes(pages)] = {0};
    const size_t splitBy = 8;
    Block* b = reinterpret_cast<Block*>(a);
    size_t num = b->split(a + pagesToBytes(pages), splitBy);
    const size_t expected = pagesToBytes(pages) / splitBy;
    EXPECT_EQ(expected, num);
    num = 0;
    while (b) {
        b = b->next();
        ++num;
    }
    EXPECT_EQ(expected, num);
}
