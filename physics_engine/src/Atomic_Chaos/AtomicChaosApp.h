#pragma once
#include <SFML/Graphics.hpp>
#include "Particle.h"
class AtomicChaosApp
{
private:
	const sf::Vector2f maxSize;
	const sf::Vector2f minSize;

	std::vector<Particle> particles;
	const int numParticles = 500;
	sf::RenderWindow window;
public:
	AtomicChaosApp();
	~AtomicChaosApp();

	void initialize();
	void run();
	void cleanup();
};