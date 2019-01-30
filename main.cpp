// The only libraries used are the C++ STL and SFML. SFML is used to display the results
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <limits>

// The constants for the program, these could be easily saved in a properties file but for now having them as constant
// expressions is good enough. Easy to change the parameters and they get optimized at compile time.
constexpr int AMOUNT_LINES = 15;
constexpr int WIN_WIDTH = 800;
constexpr int WIN_HEIGHT = 600;
constexpr float X_MIN = 0.0f;
constexpr float X_MAX = static_cast<float>(WIN_WIDTH);
constexpr float Y_MIN = 0.0f;
constexpr float Y_MAX = static_cast<float>(WIN_HEIGHT);

const std::string WIN_NAME = "Line collision";

namespace lc
{

// The xy plane of the program is as follows:
//
//         (x = 0, y = 0) | (x = 1, y = 0) | (x = 2, y = 0) | ...
//        ----------------+----------------+----------------+-----
//         (x = 0, y = 1) | (x = 1, y = 1) | (x = 2, y = 1) | ...
//        ----------------+----------------+----------------+-----
//         (x = 0, y = 2) | (x = 1, y = 2) | (x = 2, y = 2) | ...
//        ----------------+----------------+----------------+-----
//

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Classes ////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Point class. Each instance of the class Point has 2 properties x and y. Marks the place for a point.
template <class T = float>
class Point
{
  public:
    // Datarulers
    T x;
    T y;

    // Constructors
    Point(const T &x, const T &y) : x(x), y(y){};
    Point() : x(T()), y(T()){};

    // Copy-constructors
    Point(const Point &p) : x(p.x), y(p.y){};
    Point &operator=(const Point &p)
    {
        x = p.x;
        y = p.y;
        return *this;
    }

    // In-place operators (Point and Point)
    Point &operator+=(const Point &p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }

    Point &operator-=(const Point &p)
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    Point &operator*=(const Point &p)
    {
        x *= p.x;
        y *= p.y;
        return *this;
    }

    Point &operator/=(const Point &p)
    {
        x /= p.x;
        y /= p.y;
        return *this;
    }

    // In-place operators (Point and T (for scaling porpuses, maybe))
    Point &operator+=(const T &t)
    {
        x += t;
        y += t;
        return *this;
    }

    Point &operator-=(const T &t)
    {
        x -= t;
        y -= t;
        return *this;
    }

    Point &operator*=(const T &t)
    {
        x *= t;
        y *= t;
        return *this;
    }

    Point &operator/=(const T &t)
    {
        x /= t;
        y /= t;
        return *this;
    }

    // Binary operators (Point and Point)
    Point operator+(const Point &p) const { return Point(*this) += p; }
    Point operator-(const Point &p) const { return Point(*this) -= p; }
    Point operator*(const Point &p) const { return Point(*this) *= p; }
    Point operator/(const Point &p) const { return Point(*this) /= p; }

    // Binary operators (Point and T (for scaling porpuses, maybe))
    Point operator+(const T &t) const { return Point(*this) += t; }
    Point operator-(const T &t) const { return Point(*this) -= t; }
    Point operator*(const T &t) const { return Point(*this) *= t; }
    Point operator/(const T &t) const { return Point(*this) /= t; }

    // Comparison operators
    bool operator==(const Point &p) const { return x == p.x && y == p.y; }
    bool operator!=(const Point &p) const { return !operator==(p); }

    // Friend functions for the operators (declared inside the class defined outside of it).
    // These may seem confusing at first or conterintuitive, they basically return a new point and are based on the
    // already defined in-place operators. Added for the sake of completion.
    template <class TT>
    friend Point<TT> operator+(const TT &t, const Point<TT> &p);
    template <class TT>
    friend Point<TT> operator-(const TT &t, const Point<TT> &p);
    template <class TT>
    friend Point<TT> operator*(const TT &t, const Point<TT> &p);
    template <class TT>
    friend Point<TT> operator/(const TT &t, const Point<TT> &p);

    // Printing operator (<<)
    template <class TT>
    friend std::ostream &operator<<(std::ostream &io, const Point<TT> &p);
};

// Friend functions definitions need to be outside of the class
template <class T>
Point<T> operator+(const T &t, const Point<T> &p)
{
    return p + t;
}

template <class T>
Point<T> operator-(const T &t, const Point<T> &p)
{
    return p - t;
}

template <class T>
Point<T> operator*(const T &t, const Point<T> &p)
{
    return p * t;
}

template <class T>
Point<T> operator/(const T &t, const Point<T> &p)
{
    return p / t;
}

template <class T>
std::ostream &operator<<(std::ostream &io, const Point<T> &p)
{
    io << "Point(x: " << p.x << "\ty: " << p.y << ")";
    return io;
}

// Line class. The Line class stores 2 Point class objects, they mark the starting point and the end point of the line.
template <class T>
class Line
{
  public:
    // Data
    Point<T> start_point;
    Point<T> end_point;

    // Constructors
    Line() : start_point(Point<T>()), end_point(Point<T>()) {}
    Line(const Point<T> start, const Point<T> end) : start_point(start), end_point(end) {}

    // Copy constructors
    Line(const Line &l) : start_point(l.start_point), end_point(l.end_point) {}
    Line &operator=(const Line &l)
    {
        start_point = l.start_point;
        start_point = l.end_point;
        return *this;
    }

    // Comparison operands
    bool operator==(const Line &l) const { return start_point == l.start_point && end_point == l.end_point; }
    bool operator!=(const Line &l) const { return !operator==(l); }

    // Printing operator (<<)
    template <class TT>
    friend std::ostream &operator<<(std::ostream &io, const Line<TT> &l);
};

// Printing operator definition. (Needs to be outside of the class)
template <class T>
std::ostream &operator<<(std::ostream &io, const Line<T> &l)
{
    io << "Line[s: " << l.start_point << "\te: " << l.end_point << "]";
    return io;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Static functions ///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// rand_line() generates random x and y coordinates for the points that define the start and end of a line, that way
// we obtain a random line. The random values are generated using uniform distribution thanks to the mt19937 method
template <class T>
Line<T> rand_line()
{
    std::random_device rand_device;
    std::mt19937 generator(rand_device());
    std::uniform_real_distribution<> distribution_x(X_MIN, X_MAX);
    std::uniform_real_distribution<> distribution_y(Y_MIN, Y_MAX);
    return Line<T>(Point<T>(distribution_x(generator), distribution_y(generator)),
                   Point<T>(distribution_x(generator), distribution_y(generator)));
}

// are_lines_colliding() takes 2 lines as arguments and returns a boolean, true if the lines passed are coliding
// otherwise false. This is used to check if the lines are indeed intersecting and not just in the pathway of one
// another
template <class T>
bool are_lines_colliding(const Line<T> &l1, const Line<T> l2)
{
    const Point<T> a{l1.start_point};
    const Point<T> b{l1.end_point};
    const Point<T> c{l2.start_point};
    const Point<T> d{l2.end_point};

    const T denominator{(b.x - a.x) * (d.y - c.y) - (b.y - a.y) * (d.x - c.x)};
    const T numerator1{(a.y - c.y) * (d.x - c.x) - (a.x - c.x) * (d.y - c.y)};
    const T numerator2{(a.y - c.y) * (b.x - a.x) - (a.x - c.x) * (b.y - a.y)};

    if (denominator == 0)
    {
        return numerator1 == 0 && numerator2 == 0;
    }

    const T r{numerator1 / denominator};
    const T s{numerator2 / denominator};

    return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
}

// get_collision_point() takes 2 lines as arguments and returns a Point of the x and y position where the collision
// between the given lines is occuring. This method should be used after are_lines_colliding() due to the way in
// which the collision gets check. It check wether the lines share a pathway between one another and then gets the
// point in which they meet.
template <class T>
Point<T> get_collision_point(const Line<T> &l1, const Line<T> &l2)
{
    const Point<T> a{l1.start_point};
    const Point<T> b{l1.end_point};
    const Point<T> c{l2.start_point};
    const Point<T> d{l2.end_point};

    const T a1{b.y - a.y};
    const T b1{a.x - b.x};
    const T c1{a1 * a.x + b1 * a.y};

    const T a2{d.y - c.y};
    const T b2{c.x - d.x};
    const T c2{a2 * c.x + b2 * c.y};

    const T determinant{a1 * b2 - a2 * b1};

    if (determinant == 0)
    {
        return Point<T>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    }
    else
    {
        const T x{(b2 * c1 - b1 * c2) / determinant};
        const T y{(a1 * c2 - a2 * c1) / determinant};
        return Point<T>(x, y);
    }
}
} // namespace lc

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions (Helpers) ////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// print_vector() it... ugh... prints vectors. On the console.
template <class T>
void print_vector(const std::vector<T> &v)
{
    for (const T &i : v)
    {
        std::cout << i << std::endl;
    }
}

// save_vector() saves the information stored in the vectors into a txt file.
template <class T>
void save_vector(const std::vector<T> &v, const std::string &filename)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main program ///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    // Generate the random lines
    std::vector<lc::Line<float>> lines;
    for (int i{0}; i < AMOUNT_LINES; i++)
    {
        lines.push_back(lc::rand_line<float>());
    }

    // Print the lines in the console
    std::cout << "Random lines generated: " << std::endl
              << std::endl;
    print_vector(lines);

    // Store the lines generated in file

    // Check for collision between the lines
    std::vector<lc::Point<float>> collision_points;
    for (int i{0}; i < lines.size(); i++)
    {
        for (int j{i}; j < lines.size(); j++)
        {
            if (lc::are_lines_colliding(lines[i], lines[j])) // Are the lines touching?
            {
                // If so check where exactly is where they touch.
                lc::Point<float> p{lc::get_collision_point(lines[i], lines[j])};
                collision_points.push_back(p);
            }
        }
    }

    // Print the points of collision
    std::cout << "\nPoints where there is collision: " << std::endl
              << std::endl;
    print_vector(collision_points);

    // Store the points of collision

    // Create the window and draw the graphics
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), WIN_NAME);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear(sf::Color::White);

        // Draw the generated lines
        for (const lc::Line<float> &l : lines)
        {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(l.start_point.x, l.start_point.y), sf::Color::Black),
                sf::Vertex(sf::Vector2f(l.end_point.x, l.end_point.y), sf::Color::Black)};
            window.draw(line, 2, sf::Lines);
        }

        // Draw the points of collsion
        for (const lc::Point<float> p : collision_points)
        {
            sf::CircleShape point;
            point.setRadius(2.0f);
            point.setOutlineColor(sf::Color::Red);
            point.setOutlineThickness(1.0f);
            point.setPosition(sf::Vector2f(p.x - 1, p.y - 1));
            window.draw(point);
        }

        // Show they window
        window.display();
    }

    return 0;
}