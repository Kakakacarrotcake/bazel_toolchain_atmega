
#include "hdr/math.hpp"
#include "gtest/gtest.h"

TEST(GENERAL_TEST, add_works_correctly) {

    int x = 5;
    int y = 4;
    EXPECT_EQ(x + y, add(x, y));
}