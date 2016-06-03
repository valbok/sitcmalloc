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


TEST(BlockTest, testSplitSmall) {
    const size_t size = 16;
    char a[size] = {1};
    const size_t splitBy = 8;
    Block* b = reinterpret_cast<Block*>(a);
    Block* end;
    size_t num = b->split(a + size, splitBy, &end);
    const size_t expected = size / splitBy;
    EXPECT_EQ(expected, num);
    EXPECT_FALSE(b->empty());
    EXPECT_TRUE(b->next() != nullptr);
    EXPECT_TRUE(b->next()->next() == nullptr);
    num = 0;
    while (b) {
        b = b->next();
        ++num;
    }
    EXPECT_EQ(expected, num);
}

TEST(BlockTest, testSplitSmallOdd) {
    const size_t size = 20;
    char a[size] = {1};
    const size_t splitBy = 8;
    Block* b = reinterpret_cast<Block*>(a);
    Block* end;
    size_t num = b->split(a + size, splitBy, &end);
    const size_t expected = size / splitBy;
    EXPECT_EQ(expected, num);
    EXPECT_FALSE(b->empty());
    EXPECT_TRUE(b->next() != nullptr);
    EXPECT_TRUE(b->next()->next() == nullptr);
    num = 0;
    while (b) {
        b = b->next();
        ++num;
    }
    EXPECT_EQ(expected, num);
}

TEST(BlockTest, testSplitSmallOdd2) {
    const size_t size = 23;
    char a[size] = {1};
    const size_t splitBy = 8;
    Block* b = reinterpret_cast<Block*>(a);
    Block* end;
    size_t num = b->split(a + size, splitBy, &end);
    const size_t expected = size / splitBy;
    EXPECT_EQ(expected, num);
    EXPECT_FALSE(b->empty());
    EXPECT_TRUE(b->next() != nullptr);
    EXPECT_TRUE(b->next()->next() == nullptr);
    num = 0;
    while (b) {
        b = b->next();
        ++num;
    }
    EXPECT_EQ(expected, num);
}

TEST(BlockTest, testSplitSmall2) {
    const size_t size = 24;
    char a[size] = {1};
    const size_t splitBy = 8;
    Block* b = reinterpret_cast<Block*>(a);
    Block* end;
    size_t num = b->split(a + size, splitBy, &end);
    const size_t expected = size / splitBy;
    EXPECT_EQ(expected, num);
    EXPECT_FALSE(b->empty());
    EXPECT_TRUE(b->next() != nullptr);
    EXPECT_TRUE(b->next()->next() != nullptr);
    EXPECT_TRUE(b->next()->next()->next() == nullptr);
    num = 0;
    while (b) {
        b = b->next();
        ++num;
    }
    EXPECT_EQ(expected, num);
}

TEST(BlockTest, testSplit) {
    const int pages = 1;
    char a[pagesToBytes(pages)] = {1};
    const size_t splitBy = 8;
    Block* b = reinterpret_cast<Block*>(a);
    Block* end;
    size_t num = b->split(a + pagesToBytes(pages), splitBy, &end);
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

TEST(BlockTest, testSplitBig) {
    const int pages = sizeToMinPages(MAX_CLASS_SIZE);
    char a[pagesToBytes(pages)] = {1};
    const size_t splitBy = MAX_CLASS_SIZE;
    Block* b = reinterpret_cast<Block*>(a);
    Block* end;
    size_t num = b->split(a + pagesToBytes(pages), splitBy, &end);
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
