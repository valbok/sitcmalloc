/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <gtest/gtest.h>

#include <PageHeap.h>

#include <iostream>
#include <cstring>

using namespace std;
using namespace sitcmalloc;


TEST(PageHeapTest, testInstance) {
    EXPECT_TRUE(&PageHeap::instance() == &PageHeap::instance());
}

TEST(PageHeapTest, testLessMinAllocPages) {
    const int pages = 2;

    Span* s1 = PageHeap::instance().alloc(pages);
    EXPECT_EQ(pages, s1->pages());
    Span* s2 = PageHeap::instance().alloc(pages);
    EXPECT_EQ(pages, s1->pages());
    EXPECT_EQ(pages, s2->pages());
    Span* s3 = PageHeap::instance().alloc(pages);

    EXPECT_TRUE(s1 != nullptr);
    EXPECT_TRUE(s2 != nullptr);
    EXPECT_TRUE(s3 != nullptr);
    EXPECT_EQ(pages, s1->pages());
    EXPECT_EQ(pages, s2->pages());
    EXPECT_EQ(pages, s3->pages());
}

TEST(PageHeapTest, testSeqAlloc) {
    const int pages = 2;
    Span* s1 = PageHeap::instance().alloc(pages);
    Span* s2 = PageHeap::instance().alloc(pages);
    Span* s3 = PageHeap::instance().alloc(pages);
    Span* s4 = PageHeap::instance().alloc(pages);

    EXPECT_TRUE(s1 != nullptr);
    EXPECT_TRUE(s2 != nullptr);
    EXPECT_TRUE(s3 != nullptr);
    EXPECT_TRUE(s4 != nullptr);

    EXPECT_EQ(s2, s1->pPrev());
    EXPECT_EQ(s3, s2->pPrev());
    EXPECT_EQ(s1, s2->pNext());
    EXPECT_EQ(s4, s3->pPrev());
    EXPECT_EQ(s2, s3->pNext());
    EXPECT_EQ(s3, s4->pNext());
}
