#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <random>
#include "physicsWorld/Collision.h"
#include "physicsWorld/Particle.h"

int main()
{
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    const sf::Vector2f maxSize(800.f, 600.f);
    const sf::Vector2f minSize(0.f, 0.f);

    sf::RenderWindow window(sf::VideoMode({ static_cast<unsigned int>(maxSize.x),static_cast<unsigned int>(maxSize.y) }), "Physics Engine");
    window.setFramerateLimit(60);

    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(50.0f, maxSize.x - 50.0f);
    std::uniform_real_distribution<float> distY(50.0f, maxSize.y - 50.0f);

    // Create multiple particles
    std::vector<Particle> particles;
    const int numParticles = 100;

    for (size_t i = 0; i < numParticles; i++)
    {
        float x = distX(gen);
        float y = distY(gen);
        float m = 1.0f;
        particles.emplace_back(x, y, m);
        particles.back().Initialize();
    }

    // -------------- GAME LOOP --------------
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

        // Update particles (also checks for CCD with walls)
        for (auto& p : particles) {
            p.Update(fixedDt, maxSize, minSize);
        }

        // Particle Collision Detection 
        for (size_t i = 0; i < particles.size(); ++i)
        {
            for (size_t j = i + 1; j < particles.size(); ++j)
            {
                if (Collision::checkParticleCollision(particles[i], particles[j]))
                {
                    Collision::resolveParticleCollision(particles[i], particles[j]);
                }
            }
        }

        // ----------------------- Render -----------------------
        window.clear(sf::Color::Black);
        for (auto& p : particles) {
            p.Draw(window);
        }
        window.display();
    }
    return 0;
}