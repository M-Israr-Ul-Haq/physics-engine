#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "particles.h"

int main()
{
    const sf::Vector2f maxSize(800.f, 600.f);
    const sf::Vector2f minSize(0.f, 0.f);

    sf::RenderWindow window(sf::VideoMode({ static_cast<unsigned int>(maxSize.x),static_cast<unsigned int>(maxSize.y) }),"Physics Engine");
    window.setFramerateLimit(60);

    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(30.0, 500.0);

    // Create initial two particles
    float x_A = dist(gen);
    float y_A = dist(gen);
    float x_B = dist(gen);
    float y_B = dist(gen);

    Particles particleA(x_A, y_A, 1.0f);
    Particles particleB(x_B, y_B, 1.0f);

    // Initialize all particles
    particleA.Initialize();
    particleB.Initialize();

    sf::Clock clock;
    while (window.isOpen())
    {
        // Event handling
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        // Get delta time
        float dt = clock.restart().asSeconds();

        // Fixed timestep
        const float fixedDt = 1.0f / 60.0f;
        if (dt > 0.25f) dt = 0.25f;  // Prevent spiral of death

        // Update all particles
        particleA.Update(fixedDt, maxSize, minSize);
        particleB.Update(fixedDt, maxSize, minSize);
        
        // Particle-Particle collision...
        particleA.p2pCollision(particleB);

        // Render
        window.clear(sf::Color::Black);
        particleA.Draw(window);
        particleB.Draw(window);
        window.display();
    }

    return 0;
}