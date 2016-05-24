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
    EXPECT_EQ(8, classToSize(0));
    EXPECT_EQ(16, classToSize(1));
    EXPECT_EQ(32, classToSize(2));
    EXPECT_EQ(48, classToSize(3));
    EXPECT_EQ(112, classToSize(7));


    EXPECT_EQ(1024, classToSize(32));
    EXPECT_EQ(81920, classToSize(64));
    EXPECT_EQ(MAX_CLASS_SIZE, classToSize(86));
    EXPECT_EQ(classToSize(86), classToSize(CLASSES - 1));
}

TEST(common_test, testSize2Class) {
    EXPECT_EQ(0, sizeToClass(1));
    EXPECT_EQ(0, sizeToClass(2));
    EXPECT_EQ(0, sizeToClass(3));
    EXPECT_EQ(0, sizeToClass(4));
    EXPECT_EQ(0, sizeToClass(5));
    EXPECT_EQ(0, sizeToClass(6));
    EXPECT_EQ(0, sizeToClass(7));
    EXPECT_EQ(0, sizeToClass(8));
    EXPECT_EQ(1, sizeToClass(9));
    EXPECT_EQ(1, sizeToClass(10));
    EXPECT_EQ(1, sizeToClass(12));
    EXPECT_EQ(1, sizeToClass(15));
    EXPECT_EQ(1, sizeToClass(16));
    EXPECT_EQ(2, sizeToClass(17));
    EXPECT_EQ(2, sizeToClass(25));
    EXPECT_EQ(2, sizeToClass(31));
    EXPECT_EQ(2, sizeToClass(32));
    EXPECT_EQ(3, sizeToClass(48));
    EXPECT_EQ(4, sizeToClass(63));
    EXPECT_EQ(4, sizeToClass(64));
    EXPECT_EQ(6, sizeToClass(96));
    EXPECT_EQ(32, sizeToClass(1023));
    EXPECT_EQ(32, sizeToClass(1024));
    EXPECT_EQ(33, sizeToClass(1025));
    EXPECT_EQ(39, sizeToClass(2048));
    EXPECT_EQ(85, sizeToClass(253952));
    EXPECT_EQ(86, sizeToClass(253953));
    EXPECT_EQ(86, sizeToClass(MAX_CLASS_SIZE - 1));
    EXPECT_EQ(CLASSES - 1, sizeToClass(MAX_CLASS_SIZE));
}

TEST(common_test, testClassToPages) {
    EXPECT_EQ(1, sizeToMinPages(classToSize(0)));
    EXPECT_EQ(1, sizeToMinPages(classToSize(32)));
    EXPECT_EQ(11, sizeToMinPages(classToSize(60)));
    EXPECT_EQ(18, sizeToMinPages(classToSize(63)));
    EXPECT_EQ(20, sizeToMinPages(classToSize(64)));
    EXPECT_EQ(22, sizeToMinPages(classToSize(65)));
    EXPECT_EQ(32, sizeToMinPages(classToSize(70)));
    EXPECT_EQ(42, sizeToMinPages(classToSize(75)));
    EXPECT_EQ(46, sizeToMinPages(classToSize(77)));
    EXPECT_EQ(48, sizeToMinPages(classToSize(78)));
    EXPECT_EQ(50, sizeToMinPages(classToSize(79)));
    EXPECT_EQ(52, sizeToMinPages(classToSize(80)));
    EXPECT_EQ(54, sizeToMinPages(classToSize(81)));
    EXPECT_EQ(56, sizeToMinPages(classToSize(82)));
    EXPECT_EQ(58, sizeToMinPages(classToSize(83)));
    EXPECT_EQ(60, sizeToMinPages(classToSize(84)));
    EXPECT_EQ(62, sizeToMinPages(classToSize(85)));
    EXPECT_EQ(64, sizeToMinPages(classToSize(86)));
}