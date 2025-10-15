#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Particles
{
private:
    float mass;
public:
    sf::Vector2f Position;
    sf::Vector2f Velocity;
    sf::CircleShape shape;

	Particles(float x, float y, float m);
	~Particles();

	void Initialize();
	void Load();
	void Update(float dt, const sf::Vector2f& maxSize, const sf::Vector2f& minSize);
	void Draw(sf::RenderWindow& window);
	sf::Vector2f calculateNetForce();

	float computeTOI(const sf::Vector2f& Position, const sf::Vector2f& Velocity, float radius, float dt, const sf::Vector2f& maxSize, const sf::Vector2f& minSize);
	void setRandomVelocity(float minSpeed = 100.f, float maxSpeed = 300.f);

	void p2pCollision(Particles& other);
};