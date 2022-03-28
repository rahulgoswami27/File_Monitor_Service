#ifndef DOING_UNIT_TESTS
#define DOING_UNIT_TESTS
#include "gtest/gtest.h"
#include "Service_Helper.cpp"


TEST(fileExtensionTest, test1) {
    EXPECT_TRUE (Service_Helper::isCorrectExtension("abc.lz4","lz4"));
    EXPECT_TRUE (Service_Helper::isCorrectExtension("12234asd.lz4","lz4"));
    EXPECT_FALSE (Service_Helper::isCorrectExtension("12234asd.txt","lz4"));
}

TEST(fileHeaderTest, test2) {
    EXPECT_FALSE (Service_Helper::isCorrectStartName("abc.lz4","core"));
    EXPECT_TRUE (Service_Helper::isCorrectStartName("core.123.txt","core"));
}
#endif
