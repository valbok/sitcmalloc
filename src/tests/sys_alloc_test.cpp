/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <src/sys_alloc.h>
#include <gtest/gtest.h>
#include <iostream>
#include <cstring>

using namespace std;
using namespace sitcmalloc;

TEST(sys_alloc_test, testSmallAlloc) {
    const char* hello = "hello world!";
    const size_t count = strlen(hello);
    char *p = (char *) sys_alloc(count);
    memcpy(p, hello, strlen(hello) + 1);
    EXPECT_TRUE(strcmp(p, hello) == 0);
    EXPECT_FALSE(sys_free((void*)p, count));
}

TEST(sys_alloc_test, testPageAlloc) {
    const size_t count = getpagesize()*2;
    char *p = (char *) sys_alloc(count);
    for (size_t i = 0; i < count; ++i) {
        p[i] = i % 255;
    }
    for (size_t i = 0; i < count; ++i) {
        const char e = i % 255;
        EXPECT_EQ(e, p[i]);
    }

    // Frees because length more than page size.
    EXPECT_TRUE(sys_free((void*) p, count));
}
