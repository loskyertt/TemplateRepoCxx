#include "circle.h"
#include <gtest/gtest.h>

class CircleTest : public ::testing::Test {
 protected:
  Circle unit_circle{1.0};
  Circle circle_5{5.0};
};

TEST_F(CircleTest, AreaIsCorrect) {
  // π * r²
  EXPECT_NEAR(unit_circle.area(), 3.14159, 1e-4);
  EXPECT_NEAR(circle_5.area(), 78.5398, 1e-3);
}

TEST_F(CircleTest, PerimeterIsCorrect) {
  // 2 * π * r
  EXPECT_NEAR(unit_circle.perimeter(), 6.28318, 1e-4);
  EXPECT_NEAR(circle_5.perimeter(), 31.4159, 1e-3);
}

TEST_F(CircleTest, RadiusAccessor) {
  EXPECT_DOUBLE_EQ(circle_5.radius(), 5.0);
}
