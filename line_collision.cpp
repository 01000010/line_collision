#include <iostream>
#include <string_view>
#include <vector>
#include <utility>
#include <random>
#include <limits>
#include <fstream>
#include <iomanip>
#include <format>
#include <concepts>

#include <SFML/Graphics.hpp>

constexpr std::size_t AMOUNT_LINES{ 15 };
constexpr int WIN_WIDTH{ 800 };
constexpr int WIN_HEIGHT{ 600 };
constexpr float X_MIN{ 0.0f };
constexpr float X_MAX{ static_cast<float>(WIN_WIDTH) };
constexpr float Y_MIN{ 0.0f };
constexpr float Y_MAX{ static_cast<float>(WIN_HEIGHT) };
constexpr std::string_view WIN_NAME{ "Line collision" };
constexpr std::string_view SAVEFILE_LINES{ "lines.txt" };
constexpr std::string_view SAVEFILE_COLLISION_POINTS{ "collision_points.txt" };
constexpr std::string_view SAVEFILE_SCREENSHOT{ "screenshot.png" };

namespace lc
{
	template<std::floating_point T = float>
	class Point
	{
	public:
		T x;
		T y;

		Point() noexcept : x{ T{} }, y{ T{} } {}
		Point(const T& x, const T& y) noexcept : x{ x }, y{ y } {}
		Point(const Point& p) noexcept : x{ p.x }, y{ p.y } {}
		Point(Point&& p) noexcept : x{ p.x }, y{ p.y } {}

		Point& operator=(const Point& p) noexcept
		{
			x = p.x;
			y = p.y;

			return *this;
		}

		Point& operator=(Point&& p) noexcept
		{
			x = p.x;
			y = p.y;

			return *this;
		}

		Point& operator+=(const Point& p) noexcept
		{
			x += p.x;
			y += p.y;

			return *this;
		}

		Point& operator-=(const Point& p) noexcept
		{
			x -= p.x;
			y -= p.y;

			return *this;
		}

		Point& operator*=(const Point& p) noexcept
		{
			x *= p.x;
			y *= p.y;

			return *this;
		}

		Point& operator/=(const Point& p) noexcept
		{
			x /= p.x;
			y /= p.y;

			return *this;
		}

		Point& operator+=(const T& t) noexcept
		{
			x += t;
			y += t;

			return *this;
		}

		Point& operator-=(const T& t) noexcept
		{
			x -= t;
			y -= t;

			return *this;
		}

		Point& operator*=(const T& t) noexcept
		{
			x *= t;
			y *= t;

			return *this;
		}

		Point& operator/=(const T& t) noexcept
		{
			x /= t;
			y /= t;

			return *this;
		}

		Point operator+(const Point& p) const noexcept { return Point{ *this } += p; }
		Point operator-(const Point& p) const noexcept { return Point{ *this } -= p; }
		Point operator*(const Point& p) const noexcept { return Point{ *this } *= p; }
		Point operator/(const Point& p) const noexcept { return Point{ *this } /= p; }
		Point operator+(const T& t) const noexcept { return Point{ *this } += t; }
		Point operator-(const T& t) const noexcept { return Point{ *this } -= t; }
		Point operator*(const T& t) const noexcept { return Point{ *this } *= t; }
		Point operator/(const T& t) const noexcept { return Point{ *this } /= t; }
		bool operator==(const Point& p) const noexcept { return x == p.x && y == p.y; }
		bool operator!=(const Point& p) const noexcept { return !operator==(p); }

		template<std::floating_point T>
		friend Point<T> operator+(const T& t, const Point<T>& p) noexcept;

		template<std::floating_point T>
		friend Point<T> operator-(const T& t, const Point<T>& p) noexcept;

		template<std::floating_point T>
		friend Point<T> operator*(const T& t, const Point<T>& p) noexcept;

		template<std::floating_point T>
		friend Point<T> operator/(const T& t, const Point<T>& p) noexcept;

		template<std::floating_point T>
		friend std::ostream& operator<<(std::ostream& io, const Point<T>& p);
	};

	template<std::floating_point T>
	Point<T> operator+(const T& t, const Point<T>& p) noexcept
	{
		return p + t;
	}

	template<std::floating_point T>
	Point<T> operator-(const T& t, const Point<T>& p) noexcept
	{
		return p - t;
	}

	template<std::floating_point T>
	Point<T> operator*(const T& t, const Point<T>& p) noexcept
	{
		return p * t;
	}

	template<std::floating_point T>
	Point<T> operator/(const T& t, const Point<T>& p) noexcept
	{
		return p / t;
	}

	template<std::floating_point T>
	std::ostream& operator<<(std::ostream& io, const Point<T>& p)
	{
		return io << std::format("Point(x: {:8.1f} y: {:8.1f})", p.x, p.y);
	}

	template<std::floating_point T>
	class Line
	{
	public:
		Point<T> start_point;
		Point<T> end_point;

		Line() noexcept : start_point{ Point<T>{} }, end_point{ Point<T>{} } {}
		Line(const Point<T>& start, const Point<T>& end) noexcept : start_point{ start }, end_point{ end } {}
		Line(Point<T>&& start, Point<T>&& end) noexcept : start_point{ start }, end_point{ end } {}
		Line(const Line& l) noexcept : start_point{ l.start_point }, end_point{ l.end_point } {}
		Line(Line&& l) noexcept : start_point{ l.start_point }, end_point{ l.end_point } {}

		Line& operator=(const Line& l)
		{
			start_point = l.start_point;
			end_point = l.end_point;

			return *this;
		}

		Line& operator=(Line&& l)
		{
			start_point = l.start_point;
			end_point = l.end_point;

			return *this;
		}

		bool operator==(const Line& l) const { return start_point == l.start_point && end_point == l.end_point; }
		bool operator!=(const Line& l) const { return !operator==(l); }

		template<std::floating_point T>
		friend std::ostream& operator<<(std::ostream& io, const Line<T>& l);
	};

	template<std::floating_point T>
	std::ostream& operator<<(std::ostream& io, const Line<T>& l)
	{
		return io << std::format("Line[s: (x: {:8.1f} y: {:8.1f}) e: (x: {:8.1f} y: {:8.1f})]", l.start_point.x, l.start_point.y, l.end_point.x, l.end_point.y);
	}

	template<std::floating_point T>
	Line<T> rand_line()
	{
		std::mt19937 generator{ std::random_device{}() };
		std::uniform_real_distribution<> distribution_x{ X_MIN, X_MAX };
		std::uniform_real_distribution<> distribution_y{ Y_MIN, Y_MAX };

		return Line<T>{ Point<T>{ static_cast<T>(distribution_x(generator)), static_cast<T>(distribution_y(generator)) },
			Point<T>{ static_cast<T>(distribution_x(generator)), static_cast<T>(distribution_y(generator)) }
		};
	}

	template<std::floating_point T>
	bool are_lines_colliding(const Line<T>& l1, const Line<T>& l2)
	{
		const Point<T> a{ l1.start_point };
		const Point<T> b{ l1.end_point };
		const Point<T> c{ l2.start_point };
		const Point<T> d{ l2.end_point };
		const T denominator{ (b.x - a.x) * (d.y - c.y) - (b.y - a.y) * (d.x - c.x) };
		const T numerator1{ (a.y - c.y) * (d.x - c.x) - (a.x - c.x) * (d.y - c.y) };
		const T numerator2{ (a.y - c.y) * (b.x - a.x) - (a.x - c.x) * (b.y - a.y) };

		if (denominator == 0) {
			return numerator1 == 0 && numerator2 == 0;
		}

		const T r{ numerator1 / denominator };
		const T s{ numerator2 / denominator };

		return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
	}

	template<std::floating_point T>
	Point<T> get_collision_point(const Line<T>& l1, const Line<T>& l2)
	{
		const Point<T> a{ l1.start_point };
		const Point<T> b{ l1.end_point };
		const Point<T> c{ l2.start_point };
		const Point<T> d{ l2.end_point };
		const T a1{ b.y - a.y };
		const T b1{ a.x - b.x };
		const T c1{ a1 * a.x + b1 * a.y };
		const T a2{ d.y - c.y };
		const T b2{ c.x - d.x };
		const T c2{ a2 * c.x + b2 * c.y };
		const T determinant{ a1 * b2 - a2 * b1 };
		T x{};
		T y{};

		if (determinant == 0) {
			x = std::numeric_limits<T>::max();
			y = std::numeric_limits<T>::max();
		}
		else {
			x = (b2 * c1 - b1 * c2) / determinant;
			y = (a1 * c2 - a2 * c1) / determinant;

		}

		return Point<T>(x, y);
	}
} // namespace lc

template<typename T>
void print_vector(const std::vector<T>& v)
{
	for (const T& i : v) {
		std::cout << i << std::endl;
	}
}

template<typename T>
void save_vector(const std::vector<T>& v, std::string_view filename)
{
	std::ofstream save{ filename.data() };

	for (const T& i : v) {
		save << i << std::endl;
	}

	save.close();
}

template<std::floating_point T>
void generate_rng_lines(std::vector<lc::Line<T>>& lines_vector, std::size_t line_count)
{
	for (std::size_t i{}; i < line_count; ++i) {
		lines_vector.emplace_back(lc::rand_line<float>());
	}
}

template<std::floating_point T>
void get_collision_points(std::vector<lc::Point<T>>& collision_points_vector, const std::vector<lc::Line<T>>& lines)
{
	static const lc::Point<float> p_max(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

	for (std::size_t i{}; i < lines.size(); i++) {
		for (std::size_t j{ i }; j < lines.size(); j++) {
			if (lc::are_lines_colliding(lines[i], lines[j])) {
				lc::Point<T> p{ lc::get_collision_point(lines[i], lines[j]) };

				if (p != p_max) {
					collision_points_vector.emplace_back(std::move(p));
				}
			}
		}
	}
}

int main()
{
	std::vector<lc::Line<float>> lines{};

	generate_rng_lines(lines, AMOUNT_LINES);
	std::cout << "Random lines generated:\n\n";
	print_vector(lines);

	std::vector<lc::Point<float>> collision_points{};

	get_collision_points(collision_points, lines);
	std::cout << "\nPoints where there is collision:\n\n";
	print_vector(collision_points);

	sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), WIN_NAME.data());
	sf::Texture image_buffer{};
	sf::Vector2u size_data{ window.getSize() };

	image_buffer.create(size_data.x, size_data.y);

	while (window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) {
					lines.clear();
					generate_rng_lines(lines, AMOUNT_LINES);
					std::cout << "\n\nNew lines generated:\n";
					print_vector(lines);
					collision_points.clear();
					get_collision_points(collision_points, lines);
					std::cout << "\nNew collision points found:\n";
					print_vector(collision_points);
				}

				if (event.key.code == sf::Keyboard::S) {
					std::cout << "\nSaving lines and collision points information...\n";
					save_vector(lines, SAVEFILE_LINES);
					save_vector(collision_points, SAVEFILE_COLLISION_POINTS);
					image_buffer.copyToImage().saveToFile(SAVEFILE_SCREENSHOT.data());
				}
			}

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(sf::Color::White);

		for (const lc::Line<float>& l : lines) {
			sf::Vertex line[]{
				sf::Vertex(sf::Vector2f(l.start_point.x, l.start_point.y), sf::Color::Black),
				sf::Vertex(sf::Vector2f(l.end_point.x, l.end_point.y), sf::Color::Black)
			};

			window.draw(line, 2, sf::Lines);
		}

		for (const lc::Point<float>& p : collision_points) {
			sf::CircleShape point;

			point.setRadius(2.0f);
			point.setOutlineColor(sf::Color::Red);
			point.setOutlineThickness(1.0f);
			point.setPosition(sf::Vector2f(p.x - 2.0f, p.y - 2.0f));
			window.draw(point);
		}

		image_buffer.update(window);
		window.display();
	}

	return 0;
}
