#include "intersection.h"
#include "shape.h"

#include "gtest/gtest.h"

#include <limits.h>
#include <random>

namespace {

struct Point2f {
    float x, y;
};

// Tests factorial of negative numbers.
TEST(Intersection, TwoTriangle) {
    std::vector<Point2f> poly1;
    std::vector<Point2f> poly2;
    MakeRegularPolygon(3, poly1.begin());
    MakeRegularPolygon(4, poly1.begin());
    std::vector<Point2f> out(poly1.size() + poly2.size());
    EXPECT_TRUE(ComputeConvexPolygonIntersection<std::vector<Point2f>>(
        poly1, poly2, &out));
}
}  // namespace
