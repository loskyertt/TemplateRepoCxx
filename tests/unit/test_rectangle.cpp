#include "rectangle.h"
#include <gtest/gtest.h>

TEST(RectangleTest, AreaIsWidthTimesHeight) {
  Rectangle r(4.0, 6.0);
  EXPECT_DOUBLE_EQ(r.area(), 24.0);
}

TEST(RectangleTest, PerimeterIsTwiceSumOfSides) {
  Rectangle r(3.0, 5.0);
  EXPECT_DOUBLE_EQ(r.perimeter(), 16.0);
}

TEST(RectangleTest, SquareIsSpecialCase) {
  Rectangle square(4.0, 4.0);
  EXPECT_DOUBLE_EQ(square.area(), 16.0);
  EXPECT_DOUBLE_EQ(square.perimeter(), 16.0);
}
