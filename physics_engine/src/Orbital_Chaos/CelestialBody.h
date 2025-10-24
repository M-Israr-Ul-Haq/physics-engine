#pragma once
#include <SFML/Graphics.hpp>

struct Object {
    float mass, radius;
    sf::Vector2f position, velocity, acceleration;
    std::shared_ptr<sf::Shape> shape;
};

class CelestialBody {
private:
    std::vector<Object> planets;        // Massive & Slow-moving
    std::vector<Object> meteors;        // Fast & Chaotic
    std::vector<Object> stars;          // Static Background
public:
    void createPlanet(float m, float r, sf::Vector2f pos, sf::Vector2f vel);
    void createMeteor(float m, float r, sf::Vector2f pos, sf::Vector2f vel);
    void createStar(float r, sf::Vector2f pos);  // Stars don't move

    void update(float dt);
    void draw(sf::RenderWindow& window);
};