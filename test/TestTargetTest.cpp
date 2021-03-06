#include "CppUTest/CommandLineTestRunner.h"
#include "TestTarget.h"
#include <iostream>

// テストグループを定義
TEST_GROUP(TestFuncGroup)
{
    // 各テストケースの実行直前に呼ばれる仮想メソッド
    TEST_SETUP()
    {
        std::cout << " TEST_SETUP called" << std::endl;
    }

    // 各テストケースの実行直後に呼ばれる仮想メソッド
    TEST_TEARDOWN()
    {
        std::cout << " TEST_TEARDOWN called" << std::endl;
    }
};

// テストを実行するメソッド
TEST(TestFuncGroup, TestFunc2)
{
    std::cout << "TestFunc2" << std::endl;
    int ret = TestFunc2(10);
    CHECK_EQUAL(10, ret);
}

// テストを実行するメソッド
TEST(TestFuncGroup, TestFunc1)
{
    std::cout << "TestFunc1" << std::endl;
    int ret = TestFunc1(10);
    CHECK_EQUAL(20, ret);
}