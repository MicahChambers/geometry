#include "accessors.h"

#include <cstdint>
#include <iterator>

namespace point_polygon_impl {
/// Which side of segment (pt0 -- pt1) is q on?
///
/// return 1 for left, 0 for on, -1 for right
template <typename PointT>
int8_t LineSide(const PointT& pt0, const PointT& pt1, const PointT& q) {
    const double tmp = -(GetY(pt1) - GetY(pt0)) * (GetX(q) - GetX(pt0)) +
                       (GetX(pt1) - GetX(pt0)) * (GetY(q) - GetY(pt0));
    if (tmp < 0) {
        return -1;
    } else if (tmp > 0) {
        return 1;
    } else {
        return 0;
    }
}
}  // namespace point_polygon_impl

template <typename PointT, typename It>
bool PointInPolygon(const PointT& pt, It begin, It end) {
    int wn = 0;  // the  winding number counter

    // loop through all edges of the polygon
    for (auto it = begin, itn = std::next(begin); it != end; ++it, ++itn) {
        itn = (itn == end ? begin : itn);

        if (GetY(*it) <= GetY(pt)) {  // start y <= P.y
            if (GetY(*itn) > GetY(pt) &&
                point_polygon_impl::LineSide(*it, *itn, pt) > 0) {
                // an upward crossing and P left of  edge
                // have  a valid up intersect
                ++wn;
            }
        } else {  // start y > P.y (no test needed)
            if (GetY(*itn) <= GetY(pt)) {
                if (point_polygon_impl::LineSide(*it, *itn, pt) < 0) {
                    // a downward crossing and P right of  edge
                    // have  a valid down intersect
                    --wn;
                }
            }
        }
    }
    return wn != 0;
}

template <typename PointT, typename PolygonT>
bool PointInPolygon(const PointT& pt, const PolygonT& poly) {
    return PointInPolygon(pt, poly.begin(), poly.end());
}
