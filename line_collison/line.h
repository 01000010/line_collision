#ifndef LINE_H
#define LINE_H

#include "point.h"

namespace lc
{
    template<typename T = float>
    class Line
    {
    private:
        Point<T> start{};
        Point<T> end{};

    public:
        // Constructors and destructor
        Line(const Point<T>& start_point, const Point<T>& end_point)
            : start{ start_point }, end{ end_point } {}
        Line(const Line<T>& line) : Line(line.start, line.end) {}
        Line() = default;
        ~Line() = default;

        // Assignment overload
        Line& operator=(const Line<T>& line)
        {
            start = line.start;
            end = line.end;
            return *this;
        }

        // Comparison operators
        bool operator==(const Line<T>& line)
        {
            return start == line.start && end == line.end;
        }

        bool operator!=(const Line<T>& line)
        {
            return !operator==(line);
        }
    };
}
#endif