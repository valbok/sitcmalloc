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
    EXPECT_EQ(nullptr, s->prev());
    EXPECT_EQ(nullptr, s->next());
}

TEST(SpanTest, testEmptyDLL) {
    Span root(0);
    Span s1(1);
    Span s2(2);

    EXPECT_TRUE(root.empty());
    EXPECT_TRUE(s1.empty());
    EXPECT_TRUE(s2.empty());

    root.prepend(&s1);

    EXPECT_FALSE(root.empty());
    EXPECT_FALSE(s1.empty());
    EXPECT_TRUE(s2.empty());

    root.prepend(&s2);

    EXPECT_FALSE(root.empty());
    EXPECT_FALSE(s1.empty());
    EXPECT_FALSE(s2.empty());
}

TEST(SpanTest, testDLL) {
    Span root(0);
    Span s1(1);
    Span s2(2);

    root.prepend(&s1);
    root.prepend(&s2);

    EXPECT_EQ(&s2, s1.prev());
    EXPECT_EQ(nullptr, s1.next());

    EXPECT_EQ(&root, s2.prev());
    EXPECT_EQ(&s1, s2.next());

    EXPECT_EQ(nullptr, root.prev());
    EXPECT_EQ(&s2, root.next());
}

TEST(SpanTest, testRemoveLastDLL) {
    Span root(0);
    Span s1(1);
    Span s2(2);

    root.prepend(&s1);
    root.prepend(&s2);

    EXPECT_EQ(&s2, s1.prev());
    EXPECT_EQ(nullptr, s1.next());

    s1.remove();

    EXPECT_EQ(nullptr, s1.prev());
    EXPECT_EQ(nullptr, s1.next());

    EXPECT_EQ(&root, s2.prev());
    EXPECT_EQ(nullptr, s2.next());

    EXPECT_EQ(nullptr, root.prev());
    EXPECT_EQ(&s2, root.next());
}

TEST(SpanTest, testRemoveMiddleDLL) {
    Span root(0);
    Span s1(1);
    Span s2(2);

    root.prepend(&s1);
    root.prepend(&s2);

    EXPECT_EQ(&root, s2.prev());
    EXPECT_EQ(&s1, s2.next());

    s2.remove();

    EXPECT_EQ(&root, s1.prev());
    EXPECT_EQ(nullptr, s1.next());

    EXPECT_EQ(nullptr, s2.prev());
    EXPECT_EQ(nullptr, s2.next());

    EXPECT_EQ(nullptr, root.prev());
    EXPECT_EQ(&s1, root.next());
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
    Block* start,* end;
    size_t num = root->split(splitBy, 0, &start, &end);
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
    Block* start,* end;
    size_t num = root->split(splitBy, 0, &start, &end);
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
    Block* start,* end;
    size_t num = root->split(splitBy, 0, &start, &end);
    EXPECT_EQ(0, root->sizeClass());
    num = root->split(splitBy, 1, &start, &end);
    EXPECT_EQ(1, root->sizeClass());
}

TEST(SpanTest, testSplit128) {
    const int pages = 128;
    char a[pagesToBytes(pages)] = {0};
    Span* root = Span::create(a, pages);
    root->use();
    const size_t splitBy = 131072;
    Block* start,* end;
    size_t num = root->split(splitBy, 0, &start, &end);
    const size_t spanSize = sizeof(Span) - 2 * sizeof(Span*);
    const size_t expected = (pagesToBytes(pages) - spanSize) / splitBy;
    EXPECT_EQ(expected, num);
    int c = 0;
    while (start) {
        start = start->next();
        ++c;
    }
    EXPECT_EQ(num, c);
}
