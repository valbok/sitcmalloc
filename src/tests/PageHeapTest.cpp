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
    Span* s4 = PageHeap::instance().alloc(MIN_ALLOCATED_PAGES);

    EXPECT_TRUE(s1 != nullptr);
    EXPECT_TRUE(s2 != nullptr);
    EXPECT_TRUE(s3 != nullptr);
    EXPECT_EQ(pages, s1->pages());
    EXPECT_EQ(pages, s2->pages());
    EXPECT_EQ(pages, s3->pages());
    EXPECT_EQ(MIN_ALLOCATED_PAGES, s4->pages());
}

TEST(PageHeapTest, testLargeAllocPages) {
    const int pages = 200;

    Span* s1 = PageHeap::instance().alloc(pages);
    EXPECT_EQ(pages, s1->pages());
}

TEST(PageHeapTest, testPageMap) {
    for (size_t pages = 1000; pages > 0; pages >>= 1) {
        Span* s = PageHeap::instance().alloc(pages);
        for (size_t i = 0; i < pagesToBytes(pages); ++i) {
            Span* t = PageHeap::span(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(s) + i));
            EXPECT_EQ(s, t);
        }
    }
}

TEST(PageHeapTest, testAllocUniquePtrs) {
    Span* spans[1000] = {0};
    for (size_t i = 0; i < 1000; ++i) {
        Span* s = PageHeap::instance().alloc(1);
        for (size_t j = 0; j < 1000 && spans[j] != 0; ++j) {
            EXPECT_TRUE(spans[j] != s);
        }
        spans[i] = s;
    }
}