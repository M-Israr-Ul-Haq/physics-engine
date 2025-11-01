#pragma once
#include <SFML/Graphics.hpp>

class GravitySource {
private:
    float mass;
    sf::Vector2f position;
    sf::CircleShape sun;

public:
    GravitySource(sf::Vector2f pos, float m);  

    // Getters
    sf::Vector2f get_position() const { return position; }
    float get_mass() const { return mass; }

    void render(sf::RenderWindow& window);
};

class Planets {
private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float mass;
    sf::CircleShape planet;

public:
    Planets(sf::Vector2f pos, sf::Vector2f vel);

    // Getters
    sf::Vector2f get_position() const { return position; }
    sf::Vector2f get_velocity() const { return velocity; }
    float get_mass() const { return mass; }
    sf::Color get_color() const { return planet.getFillColor(); } 

    // Setters
    void set_position(sf::Vector2f p) {
        position = p;
        planet.setPosition(position);
    }
    void set_velocity(sf::Vector2f v) {
        velocity = v;
    }
    void set_mass(float m) {
        mass = m;
    }
    void set_color(sf::Color c) {  
        planet.setFillColor(c);
    }
    void set_radius(float r) {  
        planet.setRadius(r);
        planet.setOrigin(sf::Vector2f{ r, r });  // Keeps centered
    }

    void render(sf::RenderWindow& window);
};