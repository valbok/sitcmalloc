/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <common.h>
#include <gtest/gtest.h>
#include <iostream>



using namespace std;
using namespace sitcmalloc;

TEST(common_test, testClass2Size) {
    EXPECT_EQ(1024, classToSize(32));
    EXPECT_EQ(MAX_SIZE, classToSize(CLASSES - 1));
}

TEST(common_test, testSize2Class) {
    EXPECT_EQ(0, sizeToClass(1));
    EXPECT_EQ(0, sizeToClass(2));
    EXPECT_EQ(0, sizeToClass(3));
    EXPECT_EQ(0, sizeToClass(4));
    EXPECT_EQ(0, sizeToClass(5));
    EXPECT_EQ(0, sizeToClass(6));
    EXPECT_EQ(0, sizeToClass(7));
    EXPECT_EQ(1, sizeToClass(8));
    EXPECT_EQ(1, sizeToClass(9));
    EXPECT_EQ(1, sizeToClass(10));
    EXPECT_EQ(1, sizeToClass(12));
    EXPECT_EQ(1, sizeToClass(15));
    EXPECT_EQ(2, sizeToClass(16));
    EXPECT_EQ(2, sizeToClass(17));
    EXPECT_EQ(2, sizeToClass(25));
    EXPECT_EQ(2, sizeToClass(31));
    EXPECT_EQ(3, sizeToClass(32));
    EXPECT_EQ(4, sizeToClass(48));
    EXPECT_EQ(4, sizeToClass(63));
    EXPECT_EQ(5, sizeToClass(64));
    EXPECT_EQ(32, sizeToClass(1023));
    EXPECT_EQ(33, sizeToClass(1024));
    EXPECT_EQ(CLASSES, sizeToClass(MAX_SIZE));
}
