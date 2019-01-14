#include "geometry/intersection.h"
#include "geometry/shape.h"

#include "gtest/gtest.h"

#include <limits.h>
#include <random>

namespace {

struct Point2f {
    float x, y;
};

// Tests factorial of negative numbers.
TEST(Intersection, TwoTriangle) {
    std::vector<Point2f> poly1(3);
    std::vector<Point2f> poly2(4);
    MakeRegularPolygon(3, poly1.begin());
    MakeRegularPolygon(4, poly2.begin());
    std::vector<Point2f> out(poly1.size() + poly2.size());
    EXPECT_TRUE(ComputeConvexPolygonIntersection<std::vector<Point2f>>(
        poly1, poly2, &out));

    std::vector<Point2f> expected{
        {1, 0},       {-0.26794922, 0.732051},  {-0.5, 0.5},
        {-0.5, -0.5}, {-0.26794916, -0.732051},
    };

    for (size_t i = 0; i < std::min(expected.size(), out.size()); ++i) {
        EXPECT_FLOAT_EQ(expected[i].x, out[i].x);
        EXPECT_FLOAT_EQ(expected[i].y, out[i].y);
    }
    ASSERT_EQ(expected.size(), out.size());
    for (size_t i = 0; i < std::min(expected.size(), out.size()); ++i) {
        ASSERT_FLOAT_EQ(expected[i].x, out[i].x);
        ASSERT_FLOAT_EQ(expected[i].y, out[i].y);
    }
}

}  // namespace
