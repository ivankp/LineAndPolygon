#pragma once

#include <array>
#include <vector>

// https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection#Algebraic_form
// https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line_segment
// https://en.wikipedia.org/wiki/Intersection_(geometry)#Two_line_segments
// https://en.wikipedia.org/wiki/Point_in_polygon#Ray_casting_algorithm

bool LineAndPolygon(
    std::array<std::array<double, 2>> line,
    const std::vector<std::array<double, 2>>& polygon
) noexcept {
    bool before = false, after = false;

    // Line: [ [ x1, y1 ], [ x2, y2 ] ]
    // Edge: [ [ x3, y3 ], [ x4, y4 ] ]

    double
        x1, x2, x3, x4,
        y1, y2, y3, y4;
    std::tie(std::tie(x1, y1), std::tie(x2, y2)) = line;

    const double
        x12 = x1 - x2,
        y12 = y1 - y2;

    std::tie(x3, y3) = polygon.back();
    for (const auto& p : polygon) {
        std::tie(x4, y4) = p;

        const double
            x13 = x1 - x3,
            y13 = y1 - y3,
            x34 = x3 - x4,
            y34 = y3 - y4;

        // TODO: https://stackoverflow.com/a/52732707/2640636

        const double d = x12 * y34 - y12 * x34;
        const double e = x13 * y12 - y13 * x12;
        if (d > 0 ? (e < 0 || d < e) : (e > 0 || d > e))
            goto next;

        const double l = x13 * y34 - y13 * x34;
        if (d > 0 ? l < 0 : l > 0) {
            before = !before;
            goto next;
        }
        if (d > 0 ? d < l : d > l) {
            after = !after;
            goto next;
        }

        return true;

next:
        x3 = x4;
        y3 = y4;
    }

    return before && after;
}

// TODO: going into polygon through a vertex: double counting intersection
// TODO: zero line length
// TODO: d = 0
