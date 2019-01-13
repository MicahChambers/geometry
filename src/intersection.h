#pragma once
#include <iterator>

namespace intersection_impl {
/// Return a new vector that is perpendicular, left to vec
template <typename Vector>
Vector ZCross(const Vector& vec) {
    return Vector{-vec.y, vec.x};
}

/// Which side of segment (pt0 -- pt1) is q on?
///
/// return 1 for left, 0 for on, -1 for right
template <typename PointT>
int LineSide(const PointT& pt0, const PointT& pt1, const PointT& q) {
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
}  // namespace intersection_impl

/// Computes intersection of two line segments. If the two lines don't
/// intersect, the output parameter is untouched.
///
/// return: 0 for non-intersecting
///         1 for intersecting
///         2 for coincident
template <typename PointT>
int ComputeIntersectionPoint(const PointT& a0, const PointT& a1,
                             const PointT& b0, const PointT& b1,
                             PointT* out = nullptr) {
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
    if((std::isnan(uA) || std::isnan(uB)) {
        return 2;
    }

    // If the denominators only are 0 then the lines are parallel
    if(std::isinf(uA) || std::isinf(uB)) {
        return 0;
    }

    if(uA < 0 || uA > 1 || uB < 0 || uB > 1) {
        return 0;
    }

    if(out != nullptr) {
        out->x = x1 + uA * (x2 - x1);
        out->y = y1 + uA * (y2 - y1);
    }
    return out;
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
template <typename It, typename OutputIt>
bool ComputeConvexPolygonIntersection(It beginA, It endA, It beginB, It endB,
                                      OutputIt out) {
    using PointT = decltype(*It);
    thread_local std::vector<PointT> p1, p2, tmp;
    p1.clear();
    p2.clear();
    tmp.clear();

    // Find intersection of A into B
    It itA, itB, itAn, itBn;
    It interA = endA;
    It interB = endB;
    for (itA = beginA, itAn = std::next(beginA); interA == endA && itA != endA;
         ++itA, ++itAn) {
        itAn = (itAn == endA ? beginA : itAn);

        for (itB = beginB, itBn = std::next(beginB);
             interA == endA && itB != endB; ++itB, ++itBn) {
            itBn = (itBn == endB ? beginB : itBn);

            if (ComputeIntersectionPoint(*itA, *itAn, *itB, *itBn) > 0) {
                interA = itA;
                interB = itB;
            }
        }
    }

    if (interA == endA) {
        // No intersection found
        if (PointInPolygon(beginA, endA, *beginB)) {
            // b completely inside A
            std::copy(beginB, endB, out);
            return true
        } else if (PointInPolygon(beginB, endB, *beginA)) {
            // a completely inside b
            std::copy(beginA, endA, out);
            return true
        } else {
            return false;
        }
    };

    bool followA = false;
    bool followB = false;
    itA = interA;
    itB = interB;
    itAn = std::next(itA);
    itBn = std::next(itB);
    PointT tmp;
    do {
        int ret = ComputeIntersectionPoint(*itA, *itAn, *itB, *itBn, &tmp);
        if (ret == 0) {
            // no intersection
        } else if (ret == 1) {
            // Add intersection point, recompute insideA
            *out = tmp;
            int side = LineSide(*itA, *itAn, *itBn);
            if (side > 0) {
                // B-next is to the left of a's line, therefore the line is
                // entering A
                followA = true;
                ++itA;
                ++itAn;
                itAn = (itAn == endA ? beginA : itAn);
            } else if (side < 0) {
                // B-next is to the right of a's line, therefore the line is
                // entering B
                followB = true;
                ++itB;
                ++itBn;
                itBn = (itBn == endB ? beginB : itBn);
            } else {
                // bn is on a--aN,
                side = LineSide(*itB, *itBn, *itAn);
                if (side < 0) {
                } else if (side > 0) {
                } else {
                }
            }
        } else if (ret == 2) {
            // coincident
            // TODO handle this by adding last of (itA, itB), first of itAn,
            // itBn)
            assert(false);
        }
        // inside A, walk around B until we find a line that directed to
        // the left of our current line on A.
        ... ++itA;
    }
    else {
        // inside B, walk around A until we find a line that directed to
        // the left of our current line on B.
        ++itB;
    }
}
else {
}
}
while (itA != interA && itB != interB) }
