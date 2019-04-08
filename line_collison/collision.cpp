#include "collision.h"

namespace // helper functions
{
    bool are_lines_colliding(const lc::Line& l1, const lc::Line& l2)
    {
        const float denominator{
            (l1.end.x - l1.start.x) * (l2.end.y - l2.start.y) - (l1.end.y - l1.start.y) * (l2.end.x - l2.start.x)
        };
        const float numerator1{
            (l1.start.y - l2.start.y) * (l2.end.x - l2.start.x) - (l1.start.x - l2.start.x) * (l2.end.y - l2.start.y)
        };
        const float numerator2{
            (l1.start.y - l2.start.y) * (l1.end.x - l1.start.x) - (l1.start.x - l2.start.x) * (l1.end.y - l1.start.y)
        };

        if (!denominator) {
            return !numerator1 && !numerator2;
        }

        const float r{ numerator1 / denominator };
        const float s{ numerator2 / denominator };

        return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
    }
}

std::optional<lc::Point> lc::get_collision_point(const Line & l1, const Line & l2)
{
    if (!are_lines_colliding(l1, l2)) {
        return std::nullopt;
    }

    // First line characteristics
    const float a1{ l1.end.y - l1.start.y };
    const float b1{ l1.start.x - l1.end.x };
    const float c1{ a1 * l1.start.x + b1 * l1.start.y };
    // Second line characteristics
    const float a2{ l2.end.y - l2.start.y };
    const float b2{ l2.start.x - l2.end.x };
    const float c2{ a2 * l2.start.x + b2 * l2.start.y };
    // Determine if lines are colliding
    const float determinant{ a1 * b2 - a2 * b1 };

    // If determinant is zero then there is no collision
    if (!determinant) {
        return std::nullopt;
    }
    else { // The lines are colliding so we retrive the collision point
        const float x{ (b2 * c1 - b1 * c2) / determinant };
        const float y{ (a1 * c2 - a2 * c1) / determinant };

        return Point{ x, y };
    }
}