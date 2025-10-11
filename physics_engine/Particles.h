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
	void Update(float dt);
	void Draw(sf::RenderWindow& window);
	sf::Vector2f calculateNetForce();
};
