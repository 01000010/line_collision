#ifndef LINE_H
#define LINE_H

#include "point.h"

namespace lc
{
    class Line
    {
    public:
        Point start{};
        Point end{};

        Line(const Point& start_point, const Point& end_point)
            : start{ start_point }, end{ end_point } {}
        Line(Point&& start_point, Point&& end_point)
            : start{ std::move(start_point) }, end{ std::move(end_point) } {}
        Line() = default;

        bool operator==(const Line& line) const noexcept
        {
            return start == line.start && end == line.end;
        }

        bool operator!=(const Line& line) const noexcept
        {
            return !operator==(line);
        }
    };

    template<typename T>
    inline std::ostream& operator<<(std::ostream& stream, const Line& line)
    {
        stream << "[sp:" << line.start << " ep:" << line.end << ']';

        return stream;
    }
}

#endif