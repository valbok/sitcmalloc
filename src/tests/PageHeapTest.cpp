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
/*
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
*/

TEST(PageHeapTest, testSeqAlloc) {
    const int pages = 2;
    Span* s1 = PageHeap::instance().alloc(2);
    Span* s2 = PageHeap::instance().alloc(3);
    Span* s3 = PageHeap::instance().alloc(4);
    Span* s4 = PageHeap::instance().alloc(5);

    EXPECT_TRUE(s1 != NULL);
    EXPECT_TRUE(s2 != NULL);
    EXPECT_TRUE(s3 != NULL);
    EXPECT_TRUE(s4 != NULL);
    // Prev tests allocated spans
    //EXPECT_FALSE(nullptr == s1->prev());
    EXPECT_EQ(s2, s1->next());
    EXPECT_EQ(s1, s2->prev());
    EXPECT_EQ(s3, s2->next());
    EXPECT_EQ(s2, s3->prev());
    EXPECT_EQ(s4, s3->next());
    EXPECT_EQ(s3, s4->prev());
    EXPECT_TRUE(nullptr != s4->next());
}

