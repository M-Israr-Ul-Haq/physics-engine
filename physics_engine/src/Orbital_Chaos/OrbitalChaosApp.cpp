#include "OrbitalChaosApp.h"
#include <random>
#include <iostream>

OrbitalChaosApp::OrbitalChaosApp() : maxSize(800.f, 600.f), minSize(0.f, 0.f) {}
OrbitalChaosApp::~OrbitalChaosApp() {}

void OrbitalChaosApp::initialize()
{
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    window.create(sf::VideoMode({ static_cast<unsigned int>(maxSize.x), static_cast<unsigned int>(maxSize.y) }), "Planetary Motion Simulation");
    window.setFramerateLimit(60);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(50.f, maxSize.x - 50.f);
    std::uniform_real_distribution<float> distY(50.f, maxSize.y - 50.f);
    std::uniform_real_distribution<float> distVel(-100.f, 100.f);
    std::uniform_real_distribution<float> distRadius(5.f, 20.f);

    // Create some planets
    for (int i = 0; i < 100; ++i) {
        float m = 1.f;
        float r = distRadius(gen);
        sf::Vector2f pos(distX(gen), distY(gen));
        sf::Vector2f vel(distVel(gen), distVel(gen));
        celestialSystem.createPlanet(m, r, pos, vel);
    }
    // Create some meteors
    for (int i = 0; i < 500; ++i) {
        float m = 1.f;
        float r = distRadius(gen) * 0.97f;
        sf::Vector2f pos(distX(gen) * 10, distY(gen) * 10);
        sf::Vector2f vel(distVel(gen) * 10, distVel(gen) * 10);
        celestialSystem.createMeteor(m, r, pos, vel);
    }
    // Create some Stars
    for (int i = 0; i < 30; ++i) {
        float m = 1.f;
        float r = distRadius(gen) * 0.97f;
        sf::Vector2f pos(distX(gen), distY(gen));
        celestialSystem.createStar(r, pos);
    }
}

void OrbitalChaosApp::run()
{
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event();
        while (auto eventOpt = window.pollEvent())
        {
            if (eventOpt->is<sf::Event::Closed>())
                window.close();
        }

        float dt = clock.restart().asSeconds();
        if (dt > 0.25f) dt = 0.25f; // clamp large delta

        celestialSystem.update(dt);
         
        window.clear(sf::Color::Black);
        celestialSystem.draw(window);
        window.display();
    }
}

void OrbitalChaosApp::cleanup()
{
    if (window.isOpen())
        window.close();

    std::cout << "Resources released successfully!" << std::endl;
}