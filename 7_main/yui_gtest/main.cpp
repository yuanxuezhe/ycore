#include <limits.h>
#include "tools.h"
#include "gtest/gtest.h"
namespace {
    TEST(ConcatenateTest, IsOk) {
        CTools tools;
        EXPECT_EQ("123", tools.Concatenate('1','2','3'));
        EXPECT_EQ("123456.7", tools.Concatenate(1,'2', "3", 45, 6.7));
    }

    TEST(GetVirtualStkAcctTest, IsOk) {
        CTools tools;
        EXPECT_EQ("12345600", tools.GetVirtualStkAcct("123456","00"));
        EXPECT_EQ("12345600", tools.GetVirtualStkAcct("123456","00", 8));
        EXPECT_EQ("1234345600", tools.GetVirtualStkAcct("123456","00", 10));
        EXPECT_EQ("1230012300", tools.GetVirtualStkAcct("123","00", 10));
        EXPECT_EQ("1234789100", tools.GetVirtualStkAcct("1234567891","00", 10));
    }
}

int main(int argc, char **argv) {
    //初始化gtest
    ::testing::InitGoogleTest(&argc, argv);
    //运行所有测试用例
    return RUN_ALL_TESTS();
}