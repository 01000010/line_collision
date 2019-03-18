#ifndef POINT_H
#define POINT_H

namespace lc {
    template<typename T = float>
    class Point
    {
    private:
        T x{};
        T y{};

    public:
        // Constructor and destructor
        Point(const T& x, const T& y) : x{ x }, y{ y } {}
        Point(const Point& p) : Point(p.x, p.y) {}
        Point() = default;
        ~Point() = default;

        // Assignment overload
        Point& operator=(const Point& p)
        {
            x = p.x;
            y = p.y;
            return *this;
        }

        // Comparison operators
        bool operator==(const Point& p) const
        {
            return x == p.x && y == p.y;
        }

        bool operator!=(const Point& p) const
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