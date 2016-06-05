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
    size_t num;
    num = start->split(a + bytes, splitBy, &end);
    EXPECT_EQ(1024, num);
    EXPECT_TRUE(list.empty());
    list.prepend(num, start, end);
    EXPECT_EQ(num, list.m_len);
    EXPECT_FALSE(list.empty());
    int actual = 0;
    while (list.pop()) {
        ++actual;
    }
    EXPECT_EQ(num, actual);
    EXPECT_EQ(0, list.m_len);
    EXPECT_TRUE(list.empty());

    bytes = 256;
    char b[bytes] = {1};
    start = reinterpret_cast<Block*>(b);
    num = start->split(b + bytes, splitBy, &end);
    EXPECT_EQ(bytes/splitBy, num);
    list.prepend(num, start, end);
    EXPECT_EQ(bytes/splitBy, list.m_len);
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(start, list.pop());
    int lastlen = bytes/splitBy - 1;
    EXPECT_EQ(lastlen, list.m_len);
    EXPECT_FALSE(list.empty());

    bytes = 64;
    char c[bytes] = {1};
    start = reinterpret_cast<Block*>(c);
    num = start->split(c + bytes, splitBy, &end);
    EXPECT_EQ(bytes/splitBy, num);
    list.prepend(num, start, end);
    EXPECT_EQ(lastlen + bytes/splitBy, list.m_len);
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(end, list.m_end);
    EXPECT_EQ(nullptr, end->next());

    actual = 0;
    while (list.pop()) {
        ++actual;
    }
    EXPECT_EQ(lastlen + bytes/8, actual);
    EXPECT_EQ(0, list.m_len);
    EXPECT_TRUE(list.empty());

    bytes = 8;
    char d[bytes] = {0};
    start = reinterpret_cast<Block*>(d);
    list.prepend(1, start, nullptr);
    EXPECT_EQ(1, list.m_len);
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(start, list.m_end);
    EXPECT_TRUE(nullptr != list.m_end);
    EXPECT_EQ(nullptr, list.m_end->next());

    bytes = 16;
    char e[bytes] = {1};
    start = reinterpret_cast<Block*>(e);
    num = start->split(e + bytes, splitBy, &end);
    EXPECT_EQ(bytes/8, num);

    EXPECT_TRUE(start->next() != nullptr);
    EXPECT_EQ(nullptr, end->next());
    list.prepend(num, start, start->next());

    EXPECT_EQ(3, list.m_len);
    EXPECT_EQ(nullptr, end->next());

    bytes = 8;
    char f[bytes] = {0};
    start = reinterpret_cast<Block*>(f);
    list.prepend(1, start, nullptr);
    EXPECT_EQ(4, list.m_len);
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(start, list.m_end);
    EXPECT_TRUE(nullptr != list.m_end);
    EXPECT_EQ(nullptr, list.m_end->next());
    EXPECT_TRUE(start != list.pop());
}

TEST(FreeListTest, testPrependAgain) {
    FreeList list;
    const int pages = 1;
    int bytes = pagesToBytes(pages);
    char a[bytes] = {1};
    const size_t splitBy = 64;
    Block* start = reinterpret_cast<Block*>(a);
    Block* end;
    size_t num = start->split(a + bytes, splitBy, &end);

    list.prepend(num, start, end);
    void* b = list.pop();
    EXPECT_EQ(num - 1, list.m_len);
    EXPECT_FALSE(list.empty());

    list.prepend(1, reinterpret_cast<Block*>(b), nullptr);
    EXPECT_EQ(num, list.m_len);
    EXPECT_FALSE(list.empty());

    int actual = 0;
    while (list.pop()) {
        ++actual;
    }
    EXPECT_EQ(num, actual);
    EXPECT_EQ(0, list.m_len);
    EXPECT_TRUE(list.empty());

}