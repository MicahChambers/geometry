#pragma once
#include "point_polygon.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <iterator>

namespace inter_impl {

template <typename PointA, typename PointB>
bool AlmostEqual(const PointA& ptA, const PointB& ptB) {
    // the machine epsilon has to be scaled to the magnitude of the values
    // used and multiplied by the desired precision in ULPs (units in the
    // last place)
    constexpr int ULP = 2;

    const double distSq =
        std::pow(GetX(ptA) - GetX(ptB), 2) + std::pow(GetY(ptA) - GetY(ptB), 2);
    const double mag = std::pow(GetX(ptA), 2) + std::pow(GetY(ptA), 2) +
                       std::pow(GetX(ptB), 2) + std::pow(GetY(ptB), 2);
    return distSq <= std::numeric_limits<double>::epsilon() * mag * ULP ||
           distSq < std::numeric_limits<double>::min();
}

/// Return a new vector that is perpendicular, left to vec
template <typename Vector>
Vector ZCross(const Vector& vec) {
    return Vector{-vec.y, vec.x};
}

template <typename PointT>
double Cross(const PointT& p0, const PointT& p1, const PointT& q0,
             const PointT& q1) {
    return (GetX(p1) - GetX(p0)) * (GetY(q1) - GetY(q0)) -
           (GetY(p1) - GetY(p0)) * (GetX(q1) - GetX(q0));
}

/// Which side of segment (pt0 -- pt1) is q on?
///
/// return 1 for left, 0 for on, -1 for right
template <typename PointT>
int8_t LineSide(const PointT& pt0, const PointT& pt1, const PointT& q) {
    const double tmp =
        -(pt1.y - pt0.y) * (q.x - pt0.x) + (pt1.x - pt0.x) * (q.y - pt0.y);
    if (tmp < 0) {
        return -1;
    } else if (tmp > 0) {
        return 1;
    } else {
        return 0;
    }
}

template <typename It, typename OutIt>
void Advance(bool isInside, const It& begin, const It& end, OutIt& out, It& it,
             It& itn) {
    // advance B
    //    if (isInside) {
    //        *out++ = *itn;
    //    }
    ++it;
    ++itn;
    if (it == end) {
        it = begin;
    }
    if (itn == end) {
        itn = begin;
    }
}

}  // namespace inter_impl

/// Computes intersection of two line segments. If the two lines don't
/// intersect, the output parameter is untouched.
///
/// return: 0 for non-intersecting
///         1 for intersecting
///         2 for coincident
template <typename PointT, typename OutPointT>
int ComputeIntersectionPoint(const PointT& a0, const PointT& a1,
                             const PointT& b0, const PointT& b1,
                             OutPointT* out) {
    const double x1 = a0.x;
    const double y1 = a0.y;

    const double x2 = a1.x;
    const double y2 = a1.y;

    const double x3 = b0.x;
    const double y3 = b0.y;

    const double x4 = b1.x;
    const double y4 = b1.y;

    const double numA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3));
    const double denomA = ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
    const double uA = numA / denomA;

    const double numB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3));
    const double denomB = ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
    const double uB = numB / denomB;

    // If the denominator and numerator for the equations for ua and ub are 0
    // then the two lines are coincident. Considering round off error, if either
    // are nan, then consider them coincident
    if ((std::isnan(uA) || std::isnan(uB))) {
        return 2;
    }

    // If the denominators only are 0 then the lines are parallel
    if (std::isinf(uA) || std::isinf(uB)) {
        return 0;
    }

    if (uA < 0 || uA > 1 || uB < 0 || uB > 1) {
        return 0;
    }

    if (out != nullptr) {
        out->x = x1 + uA * (x2 - x1);
        out->y = y1 + uA * (y2 - y1);
    }
    return 1;
}

// template <typename PolygonT>
// bool ComputeConvexPolygonIntersection(const PolygonT& subject,
//                                      const PolygonT& convexClip,
//                                      PolygonT* out) {
//    std::back_insert_iterator outIt(*out);
//    return ComputeConvexPolygonIntersection(subject.begin(), subject.end(),
//                                            convexClip.begin(),
//                                            convexClip.end(), outIt);
//}

// template <typename It, typename Vector, typename OutputIt>
// void PolyEdgeClip(It begin, It end, Vector x0, Vector x1, OutputIt res) {
//    int i, side0, side1;
//    Vector tmp;
//    Vector v0 = sub->v + sub->len - 1, v1;
//    res->len = 0;
//
//    side0 = left_of(x0, x1, v0);
//    if (side0 != -1) poly_append(res, v0);
//
//    for (i = 0; i < sub->len; i++) {
//        v1 = sub->v + i;
//        side1 = left_of(x0, x1, v1);
//        if (side0 + side1 == 0 && side0)
//            /* last point and current straddle the edge */
//            if (line_sect(x0, x1, v0, v1, &tmp)) poly_append(res, &tmp);
//        if (i == sub->len - 1) break;
//        if (side1 != -1) poly_append(res, v1);
//        v0 = v1;
//        side0 = side1;
//    }
//}
//

/// Create a new polygon that contains the intersection of the two polygons.
/// Both polygons must be convex and right handed.
///
/// repeat
/// {Check to see if p and q intersect}
/// if p and q intersect then
///    if this intersection is the same as the first intersection then halt
///    else output the point of intersection and
///    {set inside}
///    if p in E hp(Q) then inside = "P"
///    else inside = "Q "
/// {Advance either p or q.}
/// if q x p >= 0 then
///    if p in hp(Q) then
///      (advance q)
///    else
///      (advance p)
/// else {
///    if q in hp(p) then
///      (advance p)
///    else
///      (advance q);
/// until repeat has executed more than 2(| P | + | Q | times
template <typename It, typename OutputIt>
OutputIt ComputeConvexPolygonIntersection(It beginA, It endA, It beginB,
                                          It endB, OutputIt out) {
    using PointT = typename std::remove_reference<decltype(*OutputIt{})>::type;

    // Find intersection of A into B
    size_t n = std::distance(beginA, endA) + std::distance(beginB, endB);
    char inside = 'X';  // 1 => A, 2 => B
    OutputIt firstInt = out;
    It itA = beginA;
    It itB = beginB;
    It itAn = std::next(beginA);
    It itBn = std::next(beginB);
    PointT tmp;
    for (size_t i = 0; i < n; ++i) {
        // NOTE: coincident lines are intentionally not handled, in so doing
        // coincident segments will be left out of the intersection, but the
        // shape will be correct
        if (ComputeIntersectionPoint(*itA, *itAn, *itB, *itBn, &tmp) == 1) {
            // Exit condition
            if (inside != 'X' && inter_impl::AlmostEqual(tmp, *firstInt)) {
                // already saw this intersection, done
                break;
            }
            std::cerr << "inside: " << inside << ", intersection at triangle:"
                      << std::distance(beginA, itA)
                      << ", square: " << std::distance(beginB, itB) << "), "
                      << "(" << tmp.x << ", " << tmp.y << ")"
                      << "first (" << firstInt->x << ", " << firstInt->y << ")"
                      << std::endl;
            *out++ = tmp;

            auto side = inter_impl::LineSide(*itA, *itAn, *itBn);
            if (side > 0) {
                // bn to the left of a -- an, therefore it is inside a
                inside = 'A';
            } else {
                // bn to the right of a -- an, therefore it is inside b
                inside = 'B';
            }
        }

        // Advance either p or q
        if (inter_impl::Cross(*itA, *itAn, *itB, *itBn) >= 0) {
            auto side = inter_impl::LineSide(*itA, *itAn, *itBn);
            if (side > 0) {
                // advance A
                std::cerr << "advance triangle" << std::endl;
                inter_impl::Advance(inside == 2, beginA, endA, out, itA, itAn);
            } else {
                // advance B
                std::cerr << "advance square" << std::endl;
                inter_impl::Advance(inside == 1, beginB, endB, out, itB, itBn);
            }
        } else {
            auto side = inter_impl::LineSide(*itB, *itBn, *itAn);
            if (side > 0) {
                // advance B
                std::cerr << "advance square" << std::endl;
                inter_impl::Advance(inside == 2, beginB, endB, out, itB, itBn);
            } else {
                // advance A
                std::cerr << "advance triangle" << std::endl;
                inter_impl::Advance(inside == 1, beginA, endA, out, itA, itAn);
            }
        }
    }

    if (inside == 0) {
        // No intersection found
        if (PointInPolygon(*beginB, beginA, endA)) {
            // b completely inside A
            return std::copy(beginB, endB, out);
        } else if (PointInPolygon(*beginA, beginB, endB)) {
            // a completely inside b
            return std::copy(beginA, endA, out);
        } else {
            // out never changed, just return it
            return out;
        }
    };
    return out;
}

/// Create a new polygon that contains the intersection of the two polygons.
/// Both polygons must be convex and right handed.
///
template <typename PolygonT>
bool ComputeConvexPolygonIntersection(const PolygonT& poly1,
                                      const PolygonT& poly2, PolygonT* out) {
    out->resize(poly1.size() + poly2.size());
    auto end = ComputeConvexPolygonIntersection(
        poly1.begin(), poly1.end(), poly2.begin(), poly2.end(), out->begin());
    out->resize(std::distance(out->begin(), end));
    if (end == out->begin()) {
        return false;
    } else {
        return true;
    }
}
