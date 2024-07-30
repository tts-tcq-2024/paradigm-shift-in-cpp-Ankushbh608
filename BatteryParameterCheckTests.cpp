#include "BatteryParameterCheck.h"
#include <gtest/gtest.h>

TEST(BatteryCheckTest, Temperature) {
    EXPECT_TRUE(batteryIsOk(28, 74, 0.6));
    EXPECT_FALSE(batteryIsOk(48, 83, 0.4));
    EXPECT_FALSE(batteryIsOk(-5, 52, 0.6));
    EXPECT_FALSE(batteryIsOk(30, 10, 0.5));
    EXPECT_FALSE(batteryIsOk(25, 60, 0.85));
    EXPECT_TRUE(batteryIsOk(43, 76, 0.6));
    EXPECT_TRUE(batteryIsOk(2, 22, 0.78));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
