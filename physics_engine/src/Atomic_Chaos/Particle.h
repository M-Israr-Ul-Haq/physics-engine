#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include <iostream>

class Particle
{
private:
    float mass;
    float angleV;
    float Rotation;

public:
    // State variables
    sf::Vector2f Position;
    sf::Vector2f Velocity;

    // Visual components
    sf::CircleShape shape;
    sf::RectangleShape axis;

    // Constructor / Destructor
    Particle(float x = 0.f, float y = 0.f, float m = 1.0f);
    ~Particle();

    // Getters
    float getMass() const { return mass; }
    float& getAngleV() { return angleV; }
    const float& getAngleV() const { return angleV; }
    float getRotation() const { return Rotation; }

    // Setters
    void setAngleV(float v) { angleV = v; }

    // Core methods
    void Initialize();
    void Load();
    void Update(float dt, const sf::Vector2f& maxSize, const sf::Vector2f& minSize);
    void Draw(sf::RenderWindow& window);

    // Initialization helpers
    void setRandomVelocity(float minSpeed = 100.f, float maxSpeed = 300.f);
    void setRandomAngularVelocity(float minSpin = -5.0f, float maxSpin = 5.0f);
};