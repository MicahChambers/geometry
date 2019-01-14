#pragma once

#include <algorithm>
#include <iterator>
#include "accessors.h"

#include <iostream>

namespace convex_hull_impl {

/// Which side of segment (pt0 -- pt1) is q on?
///
/// return 1 for left, 0 for on, -1 for right
template <typename PointT>
int LineSide(const PointT& pt0, const PointT& pt1, const PointT& pt2) {
    const double tmp = -(GetY(pt1) - GetY(pt0)) * (GetX(pt2) - GetX(pt0)) +
                       (GetX(pt1) - GetX(pt0)) * (GetY(pt2) - GetY(pt0));
    if (tmp < 0) {
        return -1;
    } else if (tmp > 0) {
        return 1;
    } else {
        return 0;
    }
}

template <typename ostream, typename PointT>
ostream& operator<<(ostream& oss, const PointT& pt) {
    oss << "(" << pt.x << ", " << pt.y << ")";
    return oss;
}

///
/// Want bottom to go right, top to go left so that we get a right handed
/// polygon. This means that the polygon should continuously turn left
///
/// \return iterator pointing to the end of the top (one after top)
template <typename It>
It GrahamInPlaceScan(It begin, It end, int side) {
    using PointT = decltype(*begin);
    // Sort either by increasing (for top) or decreasing (bottom).
    // side is just a scalar to multiply x b
    std::sort(begin, end, [side](const PointT& lhs, const PointT& rhs) {
        return side * GetX(lhs) < side * GetX(rhs);
    });

    // hull0 and hull1 are the two last members of hull. Initially there is only
    // one valid member (begin) so set both of them to that. Once we have two
    // members on the hull hull1 will be the last and hull0 will be second to
    // last.
    auto hull0 = begin;
    auto hull1 = begin;

    // initially the top is just 1 element, the left, meaning end is 1 past it
    auto hullEnd = std::next(begin);
    for (auto it = std::next(begin); it != end;
         ++it, ++hull0, ++hull1, ++hullEnd) {
        if (hull0 == hull1 && hull1 != begin) {
            // hull1 is no longer begin, so hull0 (its predecessor) can go back
            // to trailing hull1 by 1. At least until the while below below
            // walks backward
            hull0--;
        }
        while (hull1 != begin && LineSide(*hull0, *hull1, *it) < 0) {
            // the two latest hull points and *it form a right turn, which means
            // hull1 doesn't begong on the hull. Move hullEnd
            // back one, thus removing hull1 from the hull set.

            // When there are no valid points in the hull, hull0 == hull1 ==
            // begin, so don't walk off the start with hull0
            if (hull0 != begin) {
                --hull0;
            }
            --hull1;
            --hullEnd;
        }
        std::swap(*it, *hullEnd);
    }

    return hullEnd;
}

}  // namespace convex_hull_impl

template <typename It>
It ComputeConvexHullInPlace(It begin, It end) {
    // make sure there are at least 3 elements, if not return begin
    if (begin == end || std::next(begin, 1) == end ||
        std::next(begin, 2) == end) {
        return begin;
    }

    // build top, newEnd points to the
    auto topEnd = convex_hull_impl::GrahamInPlaceScan(begin, end, -1);

    // Top includes leftmost and right most points, we also need those to
    // compute the bottom, so need to rotate the left most element around to the
    // end of the top
    std::rotate(begin, std::next(begin), topEnd);

    // begin to topEnd now includes the left-most and right most points (on the
    // end), need to include those in the bottom so shift left by 2
    --topEnd;
    --topEnd;
    return convex_hull_impl::GrahamInPlaceScan(topEnd, end, 1);
}

template <typename PolygonT>
size_t ComputeConvexHullInPlace(PolygonT* poly) {
    auto newEnd = ComputeConvexHullInPlace(poly->begin(), poly->end());
    if (newEnd == poly->begin()) {
        // failed, just return original size
        return poly->size();
    }
    poly->resize(std::distance(poly->begin(), newEnd));
    return poly->size();
}
