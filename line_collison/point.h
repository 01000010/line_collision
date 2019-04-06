#ifndef POINT_H
#define POINT_H

#include <iostream>

namespace lc {
    template<typename T = float>
    class Point final
    {
    private:
        T x{};
        T y{};

    public:
        // Constructors and destructor - following rule of zero
        Point(const T& x, const T& y) : x{ x }, y{ y } {}
        Point() = default;

        // Comparison operators
        bool operator==(const Point& p) const noexcept
        {
            return x == p.x && y == p.y;
        }

        bool operator!=(const Point& p) const noexcept
        {
            return !operator==(p);
        }
    };

    template<typename T>
    inline std::ostream& operator<<(std::ostream& stream, const Point<T>& point)
    {
        stream << "(x:" << point.x << " y:" << point.y << ')';

        return stream;
    }
}
#endif