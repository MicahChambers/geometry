#include "geometry/convex_hull.h"
#include "geometry/shape.h"

#include "gtest/gtest.h"

#include <limits.h>
#include <random>

namespace {

struct Point2f {
    float x, y;
};

// Tests factorial of negative numbers.
TEST(ConvexHull, zero) {
    std::vector<Point2f> poly;
    auto out = ComputeConvexHullInPlace(poly.begin(), poly.end());
    EXPECT_EQ(out, poly.begin());
    EXPECT_EQ(out, poly.end());
}

TEST(ConvexHull, one) {
    std::vector<Point2f> poly{{0.2, 0.3}};
    auto out = ComputeConvexHullInPlace(poly.begin(), poly.end());
    EXPECT_EQ(out, poly.begin());
}

TEST(ConvexHull, two) {
    std::vector<Point2f> poly{{0.2, 0.3}, {0.4, 0.5}};
    auto out = ComputeConvexHullInPlace(poly.begin(), poly.end());
    EXPECT_EQ(out, poly.begin());
}

TEST(ConvexHull, triangle) {
    std::vector<Point2f> poly(3);
    MakeRegularPolygon(3, poly.begin());

    auto hull = poly;
    EXPECT_EQ(3, ComputeConvexHullInPlace(&hull));

    // find match
    size_t start = 0;
    for (start = 0; start < hull.size(); ++start) {
        if (std::abs(hull[start].x - poly.front().x) < 0.0001 &&
            std::abs(hull[start].y - poly.front().y) < 0.0001) {
            break;
        }
    }
    ASSERT_LT(start, hull.size());

    // Compare starting from start
    for (size_t i = start, j = 0; j < hull.size(); ++i, ++j) {
        i = (i == hull.size() ? 0 : i);
        EXPECT_FLOAT_EQ(poly[j].x, hull[i].x);
        EXPECT_FLOAT_EQ(poly[j].y, hull[i].y);
    }
}

TEST(ConvexHull, square1) {
    std::vector<Point2f> poly(4);
    MakeRegularPolygon(4, poly.begin());

    auto hull = poly;
    std::shuffle(hull.begin(), hull.end(), std::default_random_engine(17));
    EXPECT_EQ(4, ComputeConvexHullInPlace(&hull));

    // find match
    size_t start = 0;
    for (start = 0; start < hull.size(); ++start) {
        if (std::abs(hull[start].x - poly.front().x) < 0.0001 &&
            std::abs(hull[start].y - poly.front().y) < 0.0001) {
            break;
        }
    }
    ASSERT_LT(start, hull.size());

    // Compare starting from start
    for (size_t i = start, j = 0; j < hull.size(); ++i, ++j) {
        i = (i == hull.size() ? 0 : i);
        EXPECT_FLOAT_EQ(poly[j].x, hull[i].x);
        EXPECT_FLOAT_EQ(poly[j].y, hull[i].y);
    }
}

TEST(ConvexHull, square2) {
    std::vector<Point2f> poly(4);
    MakeRegularPolygon(4, M_PI / 4., poly.begin());

    auto hull = poly;
    std::shuffle(hull.begin(), hull.end(), std::default_random_engine(17));
    EXPECT_EQ(4, ComputeConvexHullInPlace(&hull));

    // find match
    size_t start = 0;
    for (start = 0; start < hull.size(); ++start) {
        if (std::abs(hull[start].x - poly.front().x) < 0.0001 &&
            std::abs(hull[start].y - poly.front().y) < 0.0001) {
            break;
        }
    }
    ASSERT_LT(start, hull.size());

    // Compare starting from start
    for (size_t i = start, j = 0; j < hull.size(); ++i, ++j) {
        i = (i == hull.size() ? 0 : i);
        EXPECT_FLOAT_EQ(poly[j].x, hull[i].x);
        EXPECT_FLOAT_EQ(poly[j].y, hull[i].y);
    }
}

TEST(ConvexHull, octogon) {
    std::vector<Point2f> poly(8);
    MakeRegularPolygon(8, poly.begin());

    auto hull = poly;
    std::shuffle(hull.begin(), hull.end(), std::default_random_engine(17));
    EXPECT_EQ(8, ComputeConvexHullInPlace(&hull));

    // find match
    size_t start = 0;
    for (start = 0; start < hull.size(); ++start) {
        if (std::abs(hull[start].x - poly.front().x) < 0.0001 &&
            std::abs(hull[start].y - poly.front().y) < 0.0001) {
            break;
        }
    }
    ASSERT_LT(start, hull.size());

    // Compare starting from start
    for (size_t i = start, j = 0; j < hull.size(); ++i, ++j) {
        i = (i == hull.size() ? 0 : i);
        EXPECT_FLOAT_EQ(poly[j].x, hull[i].x);
        EXPECT_FLOAT_EQ(poly[j].y, hull[i].y);
    }
}
}  // namespace
