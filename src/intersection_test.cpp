#include "convex_hull.h"
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
    auto out = ComputeConvexHullInPlace(poly.begin(), poly.end());
    EXPECT_EQ(out, poly.begin());
    EXPECT_EQ(out, poly.end());
}
}  // namespace
