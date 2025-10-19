#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Particle
{
private:
    float mass;
	float angleV;
	float angleA;
public:
    sf::Vector2f Position;
    sf::Vector2f Velocity;
    sf::CircleShape shape;
	sf::RectangleShape axis;

	Particle(float x, float y, float m);
	~Particle();

	// Getters
	float getMass() const { return mass; }
	float getAngleA() const { return angleA; }
	float& getAngleV() { return angleV; }

	// Setters
	void setAngleV(float v) { angleV = v; }
	void setAngleA(float a) { angleA = a; }

	void Initialize();
	void Load();
	void Update(float dt, const sf::Vector2f& maxSize, const sf::Vector2f& minSize);
	void Draw(sf::RenderWindow& window);

	void setRandomVelocity(float minSpeed = 100.f, float maxSpeed = 300.f);
};