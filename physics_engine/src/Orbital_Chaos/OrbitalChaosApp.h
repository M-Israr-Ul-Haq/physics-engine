#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>
#include "CelestialBody.h"
#include "PhysicsWorld.h"

class OrbitalChaosApp {
private:
    sf::Vector2f maxSize;
    sf::Vector2f minSize;
    sf::RenderWindow window;

    // Sun + planets stored together
    std::vector<CelestialBody> bodies;

    // Orbital trails (only for planets, not sun)
    std::vector<std::deque<sf::Vector2f>> orbital_trails;

    const size_t MAX_TRAIL_LENGTH = 500;
    int trail_update_counter = 0;
    const int TRAIL_UPDATE_INTERVAL = 3;

    PhysicsWorld physics_world;

public:
    OrbitalChaosApp();
    ~OrbitalChaosApp();

    void initialize();
    void run();
    void cleanup();

private:
    void setup_bodies();   // renamed from setup_planets
    void update_trails();
    void render_trails();
};