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
TEST(Intersection, TriangleSquare1) {
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

TEST(Intersection, TriangleSquare2) {
    auto poly1 = MakeRegularPolygon<std::vector<Point2f>>(3);
    auto poly2 = MakeRegularPolygon<std::vector<Point2f>>(4, M_PI / 4.0);
    std::vector<Point2f> out(poly1.size() + poly2.size());
    EXPECT_TRUE(ComputeConvexPolygonIntersection<std::vector<Point2f>>(
        poly1, poly2, &out));

    std::vector<Point2f> expected{
        {-0.22474492, 0.707107}, {-0.5, 0.707107},
        {-0.5, -0.707107},       {-0.22474492, -0.707107},
        {0.70710671, -0.169102}, {0.70710671, 0.169102},
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

TEST(Intersection, TriangleSquare3) {
    auto poly1 = MakeRegularPolygon<std::vector<Point2f>>(3);
    auto poly2 = MakeRegularPolygon<std::vector<Point2f>>(4, M_PI / 4);
    for (auto& pt : poly2) {
        pt.x += 0.22;
    }
    std::vector<Point2f> out(poly1.size() + poly2.size());
    EXPECT_TRUE(ComputeConvexPolygonIntersection<std::vector<Point2f>>(
        poly1, poly2, &out));

    std::vector<Point2f> expected{
        {-0.2247449, 0.707107},   {-0.48710677, 0.707107},
        {-0.48710677, -0.707107}, {-0.2247449, -0.707107},
        {0.927107, -0.042084951}, {0.927107, 0.042084951},
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

TEST(Intersection, SquareSquare) {
    auto poly1 = MakeRegularPolygon<std::vector<Point2f>>(4);
    auto poly2 = MakeRegularPolygon<std::vector<Point2f>>(4);
    std::vector<Point2f> out(poly1.size() + poly2.size());
    EXPECT_TRUE(ComputeConvexPolygonIntersection<std::vector<Point2f>>(
        poly1, poly2, &out));
    for (size_t i = 0; i < out.size(); ++i) {
        std::cerr << out[i].x << ", " << out[i].y << std::endl;
    }

    std::vector<Point2f> expected{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

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
