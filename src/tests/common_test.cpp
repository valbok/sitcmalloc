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
    //EXPECT_EQ(2, sizeToClass(32));
    EXPECT_EQ(0, sizeToClass(3));
    EXPECT_EQ(0, sizeToClass(1));
    EXPECT_EQ(0, sizeToClass(7));
    EXPECT_EQ(1, sizeToClass(8));
    EXPECT_EQ(CLASSES - 1, sizeToClass(MAX_SIZE));
}
