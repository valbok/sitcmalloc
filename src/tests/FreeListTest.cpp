/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <gtest/gtest.h>
#include <Block.h>
#include <FreeList.h>
#include <common.h>
#include <iostream>

using namespace std;
using namespace sitcmalloc;


TEST(FreeListTest, testPrepend) {
    FreeList list;
    const int pages = 1;
    int bytes = pagesToBytes(pages);
    char a[bytes] = {1};
    const size_t splitBy = 8;
    Block* start = reinterpret_cast<Block*>(a);
    Block* end;
    size_t num = start->split(a + bytes, splitBy, &end);
    EXPECT_EQ(1024, num);
    EXPECT_TRUE(list.empty());
    list.prepend(num, start, end);
    EXPECT_EQ(num, list.length());
    EXPECT_FALSE(list.empty());
    int actual = 0;
    while (list.pop()) {
        ++actual;
    }
    EXPECT_EQ(num, actual);
    EXPECT_EQ(0, list.length());

    bytes = 256;
    char b[bytes] = {1};
    start = reinterpret_cast<Block*>(b);
    num = start->split(b + bytes, splitBy, &end);
    EXPECT_EQ(bytes/8, num);
    list.prepend(num, start, end);
    EXPECT_EQ(bytes/8, list.length());
    EXPECT_EQ(start, list.pop());
    int lastlen = bytes/8 - 1;
    EXPECT_EQ(lastlen, list.length());

    bytes = 64;
    char c[bytes] = {1};
    start = reinterpret_cast<Block*>(c);
    num = start->split(c + bytes, splitBy, &end);
    EXPECT_EQ(bytes/8, num);
    list.prepend(num, start, end);
    EXPECT_EQ(lastlen + bytes/8, list.length());
    EXPECT_EQ(end, list.end());
    EXPECT_EQ(nullptr, end->next());

    actual = 0;
    while (list.pop()) {
        ++actual;
    }
    EXPECT_EQ(lastlen + bytes/8, actual);
    EXPECT_EQ(0, list.length());

    bytes = 8;
    char d[bytes] = {0};
    start = reinterpret_cast<Block*>(d);
    list.prepend(1, start, nullptr);
    EXPECT_EQ(1, list.length());
    EXPECT_EQ(start, list.end());
    EXPECT_TRUE(nullptr != list.end());
    EXPECT_EQ(nullptr, list.end()->next());

    bytes = 16;
    char e[bytes] = {1};
    start = reinterpret_cast<Block*>(e);
    num = start->split(e + bytes, splitBy, &end);
    EXPECT_EQ(bytes/8, num);

    EXPECT_TRUE(start->next() != nullptr);
    list.prepend(num, start, start->next());

    EXPECT_EQ(3, list.length());
    EXPECT_EQ(nullptr, end->next());

    bytes = 8;
    char f[bytes] = {0};
    start = reinterpret_cast<Block*>(f);
    list.prepend(1, start, nullptr);
    EXPECT_EQ(4, list.length());
    EXPECT_EQ(start, list.end());
    EXPECT_TRUE(nullptr != list.end());
    EXPECT_EQ(nullptr, list.end()->next());
    EXPECT_TRUE(start != list.pop());
}