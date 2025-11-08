#include "PendulumChaosApp.h"
#include <random>
#include <iostream>
#include <cmath>

PendulumChaosApp::PendulumChaosApp():maxSize(900.f, 600.f), minSize(0.f, 0.f)
{
}

PendulumChaosApp::~PendulumChaosApp()
{
}

void PendulumChaosApp::initialize() {

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    window = sf::RenderWindow(sf::VideoMode({ static_cast<unsigned>(maxSize.x), static_cast<unsigned>(maxSize.y) }), "Double Pendulum Simulation");
    window.setFramerateLimit(60);

    doublependulum.initialize();
}

void PendulumChaosApp::run() {
    sf::Clock clock;

    while (window.isOpen())
    {
        // Handle events
        while (auto eventOpt = window.pollEvent())
        {
            if (eventOpt->is<sf::Event::Closed>())
                window.close();
        }

        float frame_dt = clock.restart().asSeconds();
        if (frame_dt > 0.25f) frame_dt = 0.25f;

        doublependulum.update(frame_dt * 5.f);

        window.clear(sf::Color::Black);
        doublependulum.draw(window);
        window.display();
    }
}

void PendulumChaosApp::cleanup() {
    if (window.isOpen()) {
        window.close();
    }
    std::cout << "Simulation exited cleanly!" << std::endl;
}