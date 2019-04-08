#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <type_traits>

namespace lc {
    class Point final
    {
    public:
        float x{};
        float y{};

        Point(float x, float y) : x{ x }, y{ y } {}
        Point() = default;

        bool operator==(const Point& p) const noexcept
        {
            return x == p.x && y == p.y;
        }

        bool operator!=(const Point& p) const noexcept
        {
            return !operator==(p);
        }
    };

    inline std::ostream& operator<<(std::ostream& stream, const Point& point)
    {
        stream << "(x:" << point.x << " y:" << point.y << ')';

        return stream;
    }
}

#endif