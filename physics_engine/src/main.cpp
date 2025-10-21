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
    const int numParticles = 10;

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
                    std::cout << "Before collision:\n";
                    std::cout << "  p1 angleV = " << particles[i].getAngleV() << "\n";
                    std::cout << "  p2 angleV = " << particles[j].getAngleV() << "\n";

                    // Compute total linear momentum and kinetic energy BEFORE collision
                    sf::Vector2f pBefore(0.f, 0.f);
                    float keBefore = 0.f;
                    for (auto& p : particles)
                    {
                        float m = p.getMass();
                        float r = p.shape.getRadius();
                        float v2 = p.Velocity.x * p.Velocity.x + p.Velocity.y * p.Velocity.y;
                        float I = 0.5f * m * r * r;
                        float w2 = p.getAngleV() * p.getAngleV();
                        pBefore += p.Velocity * m;
                        keBefore += 0.5f * m * v2 + 0.5f * I * w2;
                    }

                    // Resolve collision
                    Collision::resolveParticleCollision(particles[i], particles[j]);

                    // Compute total linear momentum and kinetic energy AFTER collision
                    sf::Vector2f pAfter(0.f, 0.f);
                    float keAfter = 0.f;
                    for (auto& p : particles)
                    {
                        float m = p.getMass();
                        float r = p.shape.getRadius();
                        float v2 = p.Velocity.x * p.Velocity.x + p.Velocity.y * p.Velocity.y;
                        float I = 0.5f * m * r * r;
                        float w2 = p.getAngleV() * p.getAngleV();
                        pAfter += p.Velocity * m;
                        keAfter += 0.5f * m * v2 + 0.5f * I * w2;
                    }

                    // Output
                    std::cout << "After collision:\n";
                    std::cout << "  p1 angleV = " << particles[i].getAngleV() << "\n";
                    std::cout << "  p2 angleV = " << particles[j].getAngleV() << "\n";

                    std::cout << "Total momentum change: ("
                        << pAfter.x - pBefore.x << ", " << pAfter.y - pBefore.y << ")\n";
                    std::cout << "Total KE change: "
                        << keAfter - keBefore << "\n";
                    std::cout << "Energy is not preserved -> intentional due to friction clamping and numerical integration." << "\n\n";
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