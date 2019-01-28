// Libraries. Usin SFML to give a visual representation of the random lines generated
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <array>

// The constants for the program, these could be easily saved in a properties file but for now having them as constant
// expressions is good enough. Easy to change the parameters and they get optimized at compile time
constexpr int MIN_AMOUNT_LINES = 10;
constexpr int MAX_AMOUNT_LINES = 15;
constexpr int WIN_WIDTH = 800;
constexpr int WIN_HEIGHT = 600;
constexpr float MIN_LINE_SIZE = 20.0f;
constexpr float X_MIN = 0.0f;
constexpr float X_MAX = static_cast<float>(WIN_WIDTH);
constexpr float Y_MIN = 0.0f;
constexpr float Y_MAX = static_cast<float>(WIN_HEIGHT);

const std::string WIN_NAME = "Line collision";

namespace lc
{
// Each instance of the class Point has 2 properties x and y (its coordinates) coordinates in this program will work
// quite differently than on an xy plane for the sake of saving time in not having to program ways to translate screen pixels
// into xy coordinates and viseversa. So as of now the
template <class T = float>
class Point
{
  public:
    // Data
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
    // These may seem confusing at first or conterintuitive, they basically return a new point and are based on the already
    // defined in-place operators. Added for the sake of completion
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

template <class T>
std::ostream &operator<<(std::ostream &io, const Line<T> &l)
{
    io << "Line[s: " << l.start_point << "\te: " << l.end_point << "]";
    return io;
}

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

template <class T>
bool line_collision(const Line<T> &l1, const Line<T> l2)
{
    /////////////////////////////////////////////// work on this tomorrow, shit ////////////////////////////////////////////
    // Here be magic
    return false;
}

} // namespace lc

int main()
{
    // Generate the lines before drawing the graphics
    std::vector<lc::Line<float>> lines;
    for (int i{0}; i < MAX_AMOUNT_LINES; i++)
    {
        lines.push_back(lc::rand_line<float>());
    }

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

        for (const lc::Line<float> &l : lines)
        {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(l.start_point.x, l.start_point.y), sf::Color::Black),
                sf::Vertex(sf::Vector2f(l.end_point.x, l.end_point.y), sf::Color::Black)};
            window.draw(line, 2, sf::Lines);
        }

        window.display();
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Here be dragons ////

// lc::Point<float> p1(2.0f, 2.0f);
// lc::Point<float> p2(3.0f, 3.0f);

// std::cout << p1 + p2 << std::endl;

// lc::Line<float> l1(p1, p2);

// std::cout << l1 << std::endl;

// std::cout << "Random lines:\n";
// for (int i{0}; i < 10; i++)
// {
//     std::cout << lc::rand_line() << std::endl;
// }

// sf::Vertex line[] = {
//     sf::Vertex(sf::Vector2f(l.start_point.x, l.start_point.y), sf::Color::Black),
//     sf::Vertex(sf::Vector2f(l.end_point.x, l.end_point.y), sf::Color::Black)};