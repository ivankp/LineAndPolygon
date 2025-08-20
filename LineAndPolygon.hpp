#pragma once

#include <array>
#include <vector>

// https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection#Algebraic_form
// https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line_segment
// https://en.wikipedia.org/wiki/Intersection_(geometry)#Two_line_segments
// https://en.wikipedia.org/wiki/Point_in_polygon#Ray_casting_algorithm

namespace {

std::array<double, 2> operator-(
    const std::array<double, 2>& a,
    const std::array<double, 2>& b
) noexcept {
    return { a[0] - b[0], a[1] - b[1] };
}

double operator^(
    const std::array<double, 2>& a,
    const std::array<double, 2>& b
) noexcept {
    return a[0] * b[1] - a[1] * b[0];
}

}

bool LineAndPolygonOverlap(
    const std::array<std::array<double, 2>, 2>& line,
    const std::vector<std::array<double, 2>>& polygon
) noexcept {
    bool before = false, after = false;

    // Vector in the line direction
    const auto lv = line[1] - line[0];
    // TODO: zero line length

    // Previous iteration had a far edge intersection
    bool throughVertex = false;

    double t;
    auto ev = polygon.back();
    for (const auto& e2 : polygon) {
        // https://stackoverflow.com/a/565282/2640636
        const auto dv = ev - line[0];
        ev = e2 - ev; // vector in the edge direction

        const double d = lv ^ ev;
        // Line and edge are parallel
        if (d == 0) {
            goto next;
        }

        t = ev ^ dv;
        // Intersection is outside the edge: t/d not in [0, 1]
        if (d < 0 ? (t < d || 0 < t) : (t < 0 || d < t)) {
            goto next;
        }
        // https://stackoverflow.com/a/52732707/2640636
        // TODO: approximate comparison
        if (throughVertex || t == 0) {
            // TODO: Handle on this iteration
            throughVertex = false;
        } else if (t == d) {
            if (&e2 == &polygon.back()) {
                // This is the last vertex
                // TODO: Handle on this iteration
            } else {
                // Handle on the next iteration
                throughVertex = true;
            }
        }

        t = dv ^ lv;
        // Intersection is before the line: t/d < 0
        if (d < 0 ? 0 < t : t < 0) {
            before = !before;
            goto next;
        }
        // Intersection is after the line: t/d > 1
        if (d < 0 ? t < d : d < t) {
            after = !after;
            goto next;
        }

        // Line and edge segments intersect
        return true;
next:
        ev = e2;
    }

    return before && after;
}
