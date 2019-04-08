#include <iostream>
#include <random>
#include "SFML/Graphics.hpp"
#include "collision.h"
#include "program_info.h"

std::vector<lc::Line> generate_random_lines(size_t num_lines = 20);
std::vector<lc::Point> get_collision_points(const std::vector<lc::Line>& lines);
void draw_lines(sf::RenderWindow& window, const std::vector<lc::Line>& lines);
void draw_collision_circles(sf::RenderWindow& window, const std::vector<lc::Point>& points);

int main()
{
    sf::RenderWindow window(sf::VideoMode(pinf::window_height, pinf::window_width), pinf::program_name);

    std::vector<lc::Line> random_lines{ generate_random_lines(100) };
    std::vector<lc::Point> collision_points{ get_collision_points(random_lines) };

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::White);
        draw_lines(window, random_lines);
        draw_collision_circles(window, collision_points);
        window.display();
    }

    return 0;
}

std::vector<lc::Line> generate_random_lines(size_t num_lines)
{
    std::vector<lc::Line> result;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> y_dist(0, pinf::window_width);
    std::uniform_real_distribution<float> x_dist(0, pinf::window_height);

    for (size_t i{}; i < num_lines; ++i) {
        result.push_back(lc::Line{ lc::Point{x_dist(gen), y_dist(gen)}, lc::Point{x_dist(gen), y_dist(gen)} });
    }

    return result;
}

std::vector<lc::Point> get_collision_points(const std::vector<lc::Line>& lines)
{
    std::vector<lc::Point> result;

    for (size_t i{}; i < lines.size(); ++i) {
        for (size_t j{ i }; j < lines.size(); ++j) {
            std::optional<lc::Point> collision{ lc::get_collision_point(lines[i], lines[j]) };

            if (collision) {
                result.push_back(*collision);
            }
        }
    }

    return result;
}

void draw_lines(sf::RenderWindow& window, const std::vector<lc::Line>& lines)
{
    for (const lc::Line& line : lines) {
        sf::Vertex l[]{
            sf::Vertex(sf::Vector2f(line.start.x, line.start.y), sf::Color::Black),
            sf::Vertex(sf::Vector2f(line.end.x, line.end.y), sf::Color::Black)
        };

        window.draw(l, std::size(l), sf::Lines);
    }
}

void draw_collision_circles(sf::RenderWindow& window, const std::vector<lc::Point>& points)
{
    for (const lc::Point& point : points) {
        sf::CircleShape p;

        p.setRadius(2.0f);
        p.setOutlineColor(sf::Color::Red);
        p.setOutlineThickness(1.0f);
        p.setPosition(sf::Vector2f(point.x - 2.0f, point.y - 2.0f));
        window.draw(p);
    }
}