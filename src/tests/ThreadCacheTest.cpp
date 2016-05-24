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
    {
        const char* str = "SiTCMalloc";
        size_t size = strlen(str) + 1;
        void* b = ThreadCache::instance().alloc(size);
        EXPECT_TRUE(b != nullptr);
        char* c = new (b) char[size];
        strcpy(c, str);
        EXPECT_TRUE(strcmp(c, str) == 0);
    }
    {
        void* b = ThreadCache::instance().alloc(MAX_CLASS_SIZE);
        EXPECT_TRUE(b != nullptr);
    }
}