/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <gtest/gtest.h>
#include <Span.h>
#include <Block.h>
#include <common.h>
#include <iostream>

using namespace std;
using namespace sitcmalloc;

TEST(SpanTest, testCreate) {
    char a[50] = {0};
    Span* s = Span::create(a, 1);
    EXPECT_EQ(1, s->pages());
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

TEST(SpanTest, testUse) {
    Span root(0);
    EXPECT_FALSE(root.inUse());
    root.use();
    EXPECT_TRUE(root.inUse());
    root.free();
    EXPECT_FALSE(root.inUse());
}

TEST(SpanTest, testCarve) {
    char a[pagesToBytes(10)] = {0};
    Span* root = Span::create(a, 10);
    Span* c = root->carve(2);
    EXPECT_TRUE(c != nullptr);
    EXPECT_EQ(2, c->pages());
    EXPECT_EQ(8, root->pages());
}

TEST(SpanTest, testSplit) {
    const int pages = 1;
    char a[pagesToBytes(pages)] = {0};
    Span* root = Span::create(a, pages);
    root->use();
    const size_t splitBy = 8;
    size_t num;
    Block* start = root->split(splitBy, 0, num);
    const size_t spanSize = sizeof(Span) - 2 * sizeof(Span*);
    const size_t expected = (pagesToBytes(pages) - spanSize) / splitBy;
    EXPECT_EQ(root->block(), start);
    int anum = 0;
    while (start) {
        ++anum;
        start = start->next();
    }
    EXPECT_EQ(expected, anum);
    EXPECT_EQ(num, anum);
}

TEST(SpanTest, testSplitBig) {
    const int pages = sizeToMinPages(MAX_CLASS_SIZE);
    char a[pagesToBytes(pages)] = {0};
    Span* root = Span::create(a, pages);
    root->use();
    const size_t splitBy = MAX_CLASS_SIZE;
    size_t num;
    Block* start = root->split(splitBy, 0, num);
    const size_t spanSize = sizeof(Span) - 2 * sizeof(Span*);
    const size_t expected = (pagesToBytes(pages) - spanSize) / splitBy;
    EXPECT_EQ(root->block(), start);
    int anum = 0;
    while (start) {
        ++anum;
        start = start->next();
    }
    EXPECT_EQ(expected, anum);
    EXPECT_EQ(num, anum);
}

TEST(SpanTest, testSplitSizeClass) {
    const int pages = 1;
    char a[pagesToBytes(pages)] = {0};
    Span* root = Span::create(a, pages);
    root->use();
    const size_t splitBy = 8;
    size_t num;
    root->split(splitBy, 0, num);
    EXPECT_EQ(0, root->sizeClass());
    root->split(splitBy, 1, num);
    EXPECT_EQ(1, root->sizeClass());
}
