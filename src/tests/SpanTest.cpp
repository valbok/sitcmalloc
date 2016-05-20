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

TEST(SpanTest, testPersistentLeftDLL) {
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

TEST(SpanTest, testPersistentDLL) {
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

TEST(SpanTest, testPersistentRemoveLastDLL) {
    Span root(0);
    Span s1(1);
    Span s2(2);

    root.pPrepend(&s1);
    root.pPrepend(&s2);

    EXPECT_EQ(&s2,s1.pPrev());
    EXPECT_EQ(nullptr,s1.pNext());

    s1.pRemove();

    EXPECT_EQ(nullptr,s1.pPrev());
    EXPECT_EQ(nullptr,s1.pNext());

    EXPECT_EQ(&root,s2.pPrev());
    EXPECT_EQ(nullptr,s2.pNext());

    EXPECT_EQ(nullptr,root.pPrev());
    EXPECT_EQ(&s2,root.pNext());
}

TEST(SpanTest, testPersistentRemoveMiddleDLL) {
    Span root(0);
    Span s1(1);
    Span s2(2);

    root.pPrepend(&s1);
    root.pPrepend(&s2);

    EXPECT_EQ(&root, s2.pPrev());
    EXPECT_EQ(&s1, s2.pNext());

    s2.pRemove();

    EXPECT_EQ(&root, s1.pPrev());
    EXPECT_EQ(nullptr,s1.pNext());

    EXPECT_EQ(nullptr,s2.pPrev());
    EXPECT_EQ(nullptr,s2.pNext());

    EXPECT_EQ(nullptr,root.pPrev());
    EXPECT_EQ(&s1,root.pNext());
}

TEST(SpanTest, testVariableEmptyDLL) {
    Span root(0);
    Span s1(1);
    Span s2(2);

    EXPECT_TRUE(root.vEmpty());
    EXPECT_TRUE(s1.vEmpty());
    EXPECT_TRUE(s2.vEmpty());

    root.vPrepend(&s1);

    EXPECT_FALSE(root.vEmpty());
    EXPECT_FALSE(s1.vEmpty());
    EXPECT_TRUE(s2.vEmpty());

    root.vPrepend(&s2);

    EXPECT_FALSE(root.vEmpty());
    EXPECT_FALSE(s1.vEmpty());
    EXPECT_FALSE(s2.vEmpty());
}

TEST(SpanTest, testVariableDLL) {
    Span root(0);
    Span s1(1);
    Span s2(2);

    root.vPrepend(&s1);
    root.vPrepend(&s2);

    EXPECT_EQ(&s2, s1.vPrev());
    EXPECT_EQ(nullptr, s1.vNext());

    EXPECT_EQ(&root, s2.vPrev());
    EXPECT_EQ(&s1, s2.vNext());

    EXPECT_EQ(nullptr, root.vPrev());
    EXPECT_EQ(&s2, root.vNext());
}

TEST(SpanTest, testVariableRemoveLastDLL) {
    Span root(0);
    Span s1(1);
    Span s2(2);

    root.vPrepend(&s1);
    root.vPrepend(&s2);

    EXPECT_EQ(&s2, s1.vPrev());
    EXPECT_EQ(nullptr, s1.vNext());

    s1.vRemove();

    EXPECT_EQ(nullptr, s1.vPrev());
    EXPECT_EQ(nullptr, s1.vNext());

    EXPECT_EQ(&root, s2.vPrev());
    EXPECT_EQ(nullptr, s2.vNext());

    EXPECT_EQ(nullptr, root.vPrev());
    EXPECT_EQ(&s2, root.vNext());
}

TEST(SpanTest, testVariableRemoveMiddleDLL) {
    Span root(0);
    Span s1(1);
    Span s2(2);

    root.vPrepend(&s1);
    root.vPrepend(&s2);

    EXPECT_EQ(&root, s2.vPrev());
    EXPECT_EQ(&s1, s2.vNext());

    s2.vRemove();

    EXPECT_EQ(&root, s1.vPrev());
    EXPECT_EQ(nullptr, s1.vNext());

    EXPECT_EQ(nullptr, s2.pPrev());
    EXPECT_EQ(nullptr, s2.vNext());

    EXPECT_EQ(nullptr, root.vPrev());
    EXPECT_EQ(&s1, root.vNext());
}
