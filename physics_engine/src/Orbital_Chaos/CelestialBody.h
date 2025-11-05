#pragma once
#include <SFML/Graphics.hpp>

class CelestialBody {
private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float mass;
    sf::CircleShape body;

public:
    CelestialBody(sf::Vector2f pos, sf::Vector2f vel);

    // Getters
    sf::Vector2f get_position() const { return position; }
    sf::Vector2f get_velocity() const { return velocity; }
    float get_mass() const { return mass; }
    sf::Color get_color() const { return body.getFillColor(); }

    // Setters
    void set_position(sf::Vector2f p) {
        position = p;
        body.setPosition(position);
    }
    void set_velocity(sf::Vector2f v) {
        velocity = v;
    }
    void set_mass(float m) {
        mass = m;
    }
    void set_color(sf::Color c) {  
        body.setFillColor(c);
    }
    void set_radius(float r) {  
        body.setRadius(r);
        body.setOrigin(sf::Vector2f{ r, r });  // Keeps centered
    }

    void render(sf::RenderWindow& window);
};