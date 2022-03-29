#ifndef DOING_UNIT_TESTS
#define DOING_UNIT_TESTS
#include "gtest/gtest.h"
#include "Service_Helper.cpp"


TEST(fileExtensionTestSuite, trueFileExtensionTest) {
    EXPECT_TRUE (Service_Helper::isCorrectExtension("abc.lz4","lz4"));
    EXPECT_TRUE (Service_Helper::isCorrectExtension("12234asd.lz4","lz4"));
    EXPECT_TRUE (Service_Helper::isCorrectExtension("12234asd.txt","txt"));
}

TEST(fileExtensionTestSuite, falseFileExtensionTest) {
    EXPECT_FALSE (Service_Helper::isCorrectExtension("12234asd.txt","lz4"));
    EXPECT_FALSE (Service_Helper::isCorrectExtension("12234asd.lz4","txt"));
}

TEST(fileHeaderTestSuite, trueFileHeaderTest) {
    EXPECT_TRUE (Service_Helper::isCorrectStartName("abc.lz4","abc"));
    EXPECT_TRUE (Service_Helper::isCorrectStartName("core.123.txt","core"));
}

TEST(fileHeaderTestSuite, falseFileHeaderTest) {
    EXPECT_FALSE (Service_Helper::isCorrectStartName("abc.lz4","core"));
    EXPECT_FALSE (Service_Helper::isCorrectStartName("core_123.txt","core"));
}

TEST(hexIdTestSuite, trueHasHexIdTest) {
    EXPECT_TRUE (Service_Helper::hasHexId("core.1234abcd563526ef.txt"));
    EXPECT_TRUE (Service_Helper::hasHexId("examp.34321678.lz4"));
    EXPECT_TRUE (Service_Helper::hasHexId("core.1234abcd563526ef.1234.45ab.lz4"));
    EXPECT_TRUE (Service_Helper::hasHexId("core.klpo5642f.1234.45ab.lz4"));
}

TEST(hexIdTestSuite, falseHasHexIdTest) {
    EXPECT_FALSE (Service_Helper::hasHexId("core.1234bukbcd563526ef.txt"));
    EXPECT_FALSE (Service_Helper::hasHexId("examp.rtu34321678.lz4"));
    EXPECT_FALSE (Service_Helper::hasHexId("core.klpo5642f.12j34.12l45ab.lz4"));
}

TEST(triggerFileTestSuite, trueTriggerFileTest) {
    EXPECT_TRUE (Service_Helper::isTriggerName("core.1234abcd563526ef.lz4"));
    EXPECT_TRUE (Service_Helper::isTriggerName("core.ServiceName.3057.57dd721409bc4ab4b38a3c33a36a608a.3717.1647975805000000.lz4"));
    EXPECT_TRUE (Service_Helper::isTriggerName("core.service_1234abcd563526ef.23abcd6626a.lz4"));
}

TEST(triggerFileTestSuite, falseTriggerFileTest) {
    EXPECT_FALSE (Service_Helper::isTriggerName("core_1234abcd563526ef.lz4"));
    EXPECT_FALSE (Service_Helper::isTriggerName("core.1234abcd563526ef.txt"));
    EXPECT_FALSE (Service_Helper::isTriggerName("core_1234abcdk563526ef.lz4"));
    EXPECT_FALSE (Service_Helper::isTriggerName("service.1234abcd563526ef.lz4"));
    EXPECT_FALSE (Service_Helper::isTriggerName("core.ServiceName.3057.57dd721409bc4ab4b38a3c33a36a608a.3717.1647975805000000.zip"));
    EXPECT_FALSE (Service_Helper::isTriggerName("score.service_1234abcd563526ef.23abcd6626a.lz4"));
    EXPECT_FALSE (Service_Helper::isTriggerName("core"));
}
#endif
