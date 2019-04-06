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
        // Constructors and destructor - following rule of zero
        Line(const Point<T>& start_point, const Point<T>& end_point)
            : start{ start_point }, end{ end_point } {}
        Line() = default;

        // Comparison operators
        bool operator==(const Line<T>& line) const noexcept
        {
            return start == line.start && end == line.end;
        }

        bool operator!=(const Line<T>& line) const noexcept
        {
            return !operator==(line);
        }
    };

    template<typename T>
    inline std::ostream& operator<<(std::ostream& stream, const Line<T>& line)
    {
        stream << "[sp:" << line.start << " ep:" << line.end << ']';

        return stream;
    }
}
#endif