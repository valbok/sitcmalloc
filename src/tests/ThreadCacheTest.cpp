/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <gtest/gtest.h>

#include <ThreadCache.h>

#include <iostream>
#include <cstring>
#include <thread>

using namespace std;
using namespace sitcmalloc;

TEST(ThreadCacheTest, testInstance) {
    ThreadCache* tc1 = nullptr;
    ThreadCache* tc2 = nullptr;

    std::thread t1([&tc1] {tc1 = &ThreadCache::instance();});
    std::thread t2([&tc2] {tc2 = &ThreadCache::instance();});
    t1.join();
    t2.join();
    EXPECT_TRUE(tc1 != tc2);
}

TEST(ThreadCacheTest, testAlloc) {
    const char* str = "SiTCMalloc";
    size_t size = strlen(str) + 1;
    void* a = nullptr, *b = nullptr, *c = nullptr;
    {
        a = ThreadCache::instance().alloc(size);
        EXPECT_TRUE(a != nullptr);
        char* s = new (a) char[size];
        strcpy(s, str);
        EXPECT_TRUE(strcmp(s, str) == 0);
    }
    {
        void* b = ThreadCache::instance().alloc(size);
        EXPECT_TRUE(b != nullptr);
        EXPECT_TRUE(a != b);
        char* s = new (a) char[size];
        strcpy(s, str);
        EXPECT_TRUE(strcmp(s, str) == 0);
    }
    {
        c = ThreadCache::instance().alloc(MAX_CLASS_SIZE);
        EXPECT_TRUE(c != nullptr);
        EXPECT_TRUE(a != c);
        EXPECT_TRUE(b != c);
    }
}

TEST(ThreadCacheTest, testAllocMax) {
    char str[MAX_CLASS_SIZE] = {0};
    for (size_t i = 0; i < MAX_CLASS_SIZE - 1; ++i) {
        str[i] = i % 255;
    }
    char* s = (char*) ThreadCache::instance().alloc(MAX_CLASS_SIZE);
    strcpy(s, str);
    EXPECT_TRUE(strcmp(s, str) == 0);
}

TEST(ThreadCacheTest, testAllocMoreMax) {
    const size_t size = MAX_CLASS_SIZE * 2;
    char* s = (char*) ThreadCache::instance().alloc(size);
    for (size_t i = 0; i < size; ++i) {
        s[i] = i % 255;
    }
    for (size_t i = 0; i < size; ++i) {
        const char v = i % 255;
        EXPECT_EQ(v, s[i]);
    }
}

TEST(ThreadCacheTest, testFree) {
    const size_t size = 2;
    EXPECT_EQ(0, ThreadCache::instance().len(size));
    char* s1 = (char*) ThreadCache::instance().alloc(size);
    EXPECT_EQ(1021, ThreadCache::instance().len(size));
    EXPECT_TRUE(ThreadCache::instance().free(s1));
    EXPECT_EQ(1022, ThreadCache::instance().len(size));
    char* s2 = (char*) ThreadCache::instance().alloc(size);
    EXPECT_EQ(1021, ThreadCache::instance().len(size));
    char* s3 = (char*) ThreadCache::instance().alloc(size);
    char* s4 = (char*) ThreadCache::instance().alloc(size);
    EXPECT_EQ(1019, ThreadCache::instance().len(size));
    EXPECT_TRUE(ThreadCache::instance().free(s2));
    EXPECT_TRUE(ThreadCache::instance().free(s3));
    EXPECT_TRUE(ThreadCache::instance().free(s4));
}

TEST(ThreadCacheTest, testFreeReturn) {
    const size_t size = MAX_CLASS_SIZE / 2;
    EXPECT_EQ(0, ThreadCache::instance().len(size));
    char* s1 = (char*) ThreadCache::instance().alloc(size);
    EXPECT_EQ(6, ThreadCache::instance().len(size));
    EXPECT_TRUE(ThreadCache::instance().free(s1));
    EXPECT_EQ(7, ThreadCache::instance().len(size));
    char* s2 = (char*) ThreadCache::instance().alloc(size);
    EXPECT_EQ(6, ThreadCache::instance().len(size));
    char* s3 = (char*) ThreadCache::instance().alloc(size);
    EXPECT_EQ(5, ThreadCache::instance().len(size));
    char* s4 = (char*) ThreadCache::instance().alloc(size);
    EXPECT_EQ(4, ThreadCache::instance().len(size));
    char* s5 = (char*) ThreadCache::instance().alloc(size);
    EXPECT_EQ(3, ThreadCache::instance().len(size));
    char* s6 = (char*) ThreadCache::instance().alloc(size);
    EXPECT_EQ(2, ThreadCache::instance().len(size));
    char* s7 = (char*) ThreadCache::instance().alloc(size);
    EXPECT_EQ(1, ThreadCache::instance().len(size));
    char* s8 = (char*) ThreadCache::instance().alloc(size);
    EXPECT_EQ(0, ThreadCache::instance().len(size));
    EXPECT_TRUE(ThreadCache::instance().empty(size));

    char* s9 = (char*) ThreadCache::instance().alloc(size);
    EXPECT_TRUE(s9 != nullptr);
    EXPECT_EQ(6, ThreadCache::instance().len(size));
    EXPECT_FALSE(ThreadCache::instance().empty(size));

    EXPECT_TRUE(ThreadCache::instance().free(s9));
    EXPECT_EQ(7, ThreadCache::instance().len(size));
    EXPECT_FALSE(ThreadCache::instance().empty(size));

    EXPECT_TRUE(ThreadCache::instance().free(s8));
    EXPECT_EQ(7, ThreadCache::instance().len(size));
    EXPECT_FALSE(ThreadCache::instance().empty(size));

    EXPECT_TRUE(ThreadCache::instance().free(s7));
    EXPECT_EQ(7, ThreadCache::instance().len(size));
    EXPECT_FALSE(ThreadCache::instance().empty(size));

    EXPECT_TRUE(ThreadCache::instance().free(s6));
    EXPECT_EQ(7, ThreadCache::instance().len(size));
    EXPECT_FALSE(ThreadCache::instance().empty(size));

    EXPECT_TRUE(ThreadCache::instance().free(s5));
    EXPECT_EQ(7, ThreadCache::instance().len(size));
    EXPECT_FALSE(ThreadCache::instance().empty(size));

    EXPECT_TRUE(ThreadCache::instance().free(s4));
    EXPECT_EQ(7, ThreadCache::instance().len(size));
    EXPECT_FALSE(ThreadCache::instance().empty(size));

    EXPECT_TRUE(ThreadCache::instance().free(s3));
    EXPECT_EQ(7, ThreadCache::instance().len(size));
    EXPECT_FALSE(ThreadCache::instance().empty(size));

    EXPECT_TRUE(ThreadCache::instance().free(s2));
    // All blocks are freed
    EXPECT_EQ(7, ThreadCache::instance().len(size));
    EXPECT_FALSE(ThreadCache::instance().empty(size));
}