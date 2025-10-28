#pragma once
#include <SFML/Graphics.hpp>
#include "CelestialBody.h"
#include <vector>

class OrbitalChaosApp {
private:
    sf::Vector2f maxSize;
    sf::Vector2f minSize;
    sf::RenderWindow window;

    CelestialBody celestialSystem;

public:
    OrbitalChaosApp();
    ~OrbitalChaosApp();

    void initialize();
    void run();
    void cleanup();
};