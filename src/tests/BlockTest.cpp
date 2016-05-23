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
    char a[pagesToBytes(pages)] = {1};
    const size_t splitBy = 8;
    Block* b = reinterpret_cast<Block*>(a);
    size_t num = b->split(a + pagesToBytes(pages), splitBy);
    const size_t expected = pagesToBytes(pages) / splitBy;
    EXPECT_EQ(expected, num);
    EXPECT_FALSE(b->empty());
    num = 0;
    while (b) {
        b = b->next();
        ++num;
    }
    EXPECT_EQ(expected, num);
}

TEST(BlockTest, testPrepend) {
    char a[10] = {0};
    char b[10] = {0};
    Block* ab = reinterpret_cast<Block*>(a);
    Block* bb = reinterpret_cast<Block*>(b);
    ab->prepend(bb);
    EXPECT_EQ(bb, ab->next());
    EXPECT_FALSE(ab->empty());
    EXPECT_TRUE(bb->empty());
}

TEST(BlockTest, testRemove) {
    char a[10] = {0};
    char b[10] = {0};
    char c[10] = {0};
    Block* ab = reinterpret_cast<Block*>(a);
    Block* bb = reinterpret_cast<Block*>(b);
    Block* cb = reinterpret_cast<Block*>(c);
    ab->prepend(cb);
    ab->prepend(bb);
    EXPECT_EQ(bb, ab->next());
    EXPECT_EQ(cb, bb->next());
    EXPECT_FALSE(ab->empty());
    EXPECT_FALSE(bb->empty());
    EXPECT_TRUE(cb->empty());

    bb->remove(ab);
    EXPECT_EQ(cb, ab->next());
    EXPECT_EQ(nullptr, bb->next());
    EXPECT_FALSE(ab->empty());
    EXPECT_TRUE(bb->empty());
    EXPECT_TRUE(cb->empty());

    ab->remove(nullptr);
    EXPECT_EQ(nullptr, bb->next());    
    EXPECT_TRUE(ab->empty());
    EXPECT_TRUE(bb->empty());
    EXPECT_TRUE(cb->empty());
}

TEST(BlockTest, testPop) {
    Block a;
    Block b;
    Block c;
    a.prepend(&c);
    a.prepend(&b);
    EXPECT_EQ(&b, a.next());
    EXPECT_EQ(&c, b.next());
    EXPECT_EQ(nullptr, c.next());

    EXPECT_EQ(&b, a.pop());
    EXPECT_EQ(nullptr, b.next());
    EXPECT_EQ(&c, a.next());

}