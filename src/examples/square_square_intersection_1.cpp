#include "geometry/intersection.h"
#include "geometry/shape.h"
#include "simple_svg.hpp"

using namespace svg;

struct Point2f {
    float x, y;
};

int main() {
    auto poly1 = MakeRegularPolygon<std::vector<Point2f>>(4);
    auto poly2 = MakeRegularPolygon<std::vector<Point2f>>(4);
    std::vector<Point2f> polyi;
    ComputeConvexPolygonIntersection(poly1, poly2, &polyi);
    for (const auto& pt : polyi) {
        std::cerr << pt.x << ", " << pt.y << std::endl;
    }

    Dimensions dimensions(100, 100);
    Document doc("my_svg.svg", Layout(dimensions, Layout::BottomLeft));

    // Red image border.
    Polygon border(Stroke(1, Color::Red));
    border << Point(0, 0) << Point(dimensions.width, 0)
           << Point(dimensions.width, dimensions.height)
           << Point(0, dimensions.height);
    doc << border;

    // Condensed notation, parenthesis isolate temporaries that are inserted
    // into parents.

    Polygon poly1d(Color::Transparent, Stroke(.5, Color::Blue));
    for (const auto& pt : poly1) {
        poly1d << Point(10 * pt.x + 50, 10 * pt.y + 50);
    }
    doc << poly1d;

    Polygon poly2d(Color::Transparent, Stroke(.5, Color::Red));
    for (const auto& pt : poly2) {
        poly2d << Point(10 * pt.x + 50, 10 * pt.y + 50);
    }
    doc << poly2d;

    Polygon polyid(Color::Transparent, Stroke(.5, Color::Green));
    for (const auto& pt : polyi) {
        polyid << Point(10 * pt.x + 50, 10 * pt.y + 50);
    }
    doc << polyid;
    doc.save();
}
