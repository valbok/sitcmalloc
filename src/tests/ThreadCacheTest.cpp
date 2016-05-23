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
