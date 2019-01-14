#include "geometry/intersection.h"
#include "geometry/shape.h"

#include "gtest/gtest.h"

#include <ostream>

#include <limits.h>
#include <random>

namespace {

struct Point2f {
    float x, y;
};

std::ostream& operator<<(std::ostream& os, const std::vector<Point2f>& foo) {
    os << "[";
    for (size_t i = 0; i < foo.size(); ++i) {
        os << "(" << foo[i].x << ", " << foo[i].y << "), ";
    }
    os << "]";
    return os;
}

template <typename T>
void ExpectPolygonsMatch(const T& poly1, const T& poly2) {
    // find a good match
    size_t sz1 = poly1.size();
    size_t sz2 = poly2.size();
    EXPECT_EQ(sz1, sz2);
    size_t i;
    for (i = 0; i < poly1.size(); ++i) {
        if (std::abs(poly1[i].x - poly2[0].x) < 1e-7 &&
            std::abs(poly1[i].y - poly2[0].y) < 1e-7) {
            break;
        }
    }

    for (size_t j = 0; j < poly1.size(); ++j) {
        size_t k = (i + j >= poly1.size() ? i + j - poly1.size() : i + j);
        EXPECT_NEAR(poly1[k].x, poly2[j].x, 1e-5);
        EXPECT_NEAR(poly1[k].y, poly2[j].y, 1e-5);
    }
}

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

    ExpectPolygonsMatch(expected, out);
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

    ExpectPolygonsMatch(expected, out);
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

    ExpectPolygonsMatch(expected, out);
}

TEST(Intersection, SquareSquare) {
    auto poly1 = MakeRegularPolygon<std::vector<Point2f>>(4);
    auto poly2 = MakeRegularPolygon<std::vector<Point2f>>(4);
    std::vector<Point2f> out(poly1.size() + poly2.size());
    EXPECT_TRUE(ComputeConvexPolygonIntersection<std::vector<Point2f>>(
        poly1, poly2, &out));

    std::vector<Point2f> expected{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    ExpectPolygonsMatch(expected, out);
}

}  // namespace
