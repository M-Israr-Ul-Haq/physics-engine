#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>

class DoublePendulum
{
private:
    float length1;
    float length2;

    float mass1;
    float mass2;

    sf::Angle theta1;
    sf::Angle theta2;
    double angularVel1;
    double angularVel2;

    sf::CircleShape bob1;
    sf::CircleShape bob2;
    sf::RectangleShape rod1;
    sf::RectangleShape rod2;
    sf::CircleShape pin;

    // Trail system
    std::vector<sf::Vector2f> trailPoints;
    const size_t maxTrailPoints = 1000;  // Maximum number of trail points
    sf::VertexArray trail;

public:
    void initialize();
    void update(float dt);
    void trails();
    void draw(sf::RenderWindow& window);
};