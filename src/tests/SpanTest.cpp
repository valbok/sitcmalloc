/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <gtest/gtest.h>
#include <Span.h>

#include <iostream>

using namespace std;
using namespace sitcmalloc;

TEST(SpanTest, testCreate) {
    char* a[50] = {0};
    const int pages = 1;
    Span* s = Span::create(a, pages);
    EXPECT_EQ(pages, s->pages());
    EXPECT_EQ(nullptr, s->pPrev());
    EXPECT_EQ(nullptr, s->pNext());

    EXPECT_EQ(nullptr, s->vPrev());
    EXPECT_EQ(nullptr, s->vNext());
}

TEST(SpanTest, testPersistentLeftSLL) {
    Span root(0);
    Span s1(1);
    Span s2(2);

    root.pPrependToLeft(&s1);
    root.pPrependToLeft(&s2);
    
    EXPECT_EQ(nullptr,s1.pPrev());
    EXPECT_EQ(&s2,s1.pNext());

    EXPECT_EQ(&s1,s2.pPrev());
    EXPECT_EQ(&root,s2.pNext());

    EXPECT_EQ(&s2,root.pPrev());
    EXPECT_EQ(nullptr,root.pNext());
}

TEST(SpanTest, testPersistentSLL) {
    Span root(0);
    Span s1(1);
    Span s2(2);

    root.pPrepend(&s1);
    root.pPrepend(&s2);

    
    EXPECT_EQ(&s2,s1.pPrev());
    EXPECT_EQ(nullptr,s1.pNext());

    EXPECT_EQ(&root,s2.pPrev());
    EXPECT_EQ(&s1,s2.pNext());

    EXPECT_EQ(nullptr,root.pPrev());
    EXPECT_EQ(&s2,root.pNext());

}
