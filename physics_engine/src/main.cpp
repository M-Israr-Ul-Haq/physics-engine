#include <SFML/Graphics.hpp>
#include <vector>
#include "particles.h"

int main()
{
    const sf::Vector2f maxSize(800.f, 600.f);
    const sf::Vector2f minSize(0.f, 0.f);
    sf::RenderWindow window(sf::VideoMode({ static_cast<unsigned int>( maxSize.x), static_cast<unsigned int>( maxSize.y) }), "Physics Engine");
    window.setFramerateLimit(60);

    std::vector<Particles> particles;
    for (int i = 0; i < 10; i++) {
        float x = 100.f + (i % 5) * 100.f;    //  columns
        float y = 100.f + (i / 5) * 100.f;    //  rows
        float m = 1.0f;
        Particles p(x, y, m);
        p.Initialize(); 
        particles.push_back(p);
    }

    sf::Clock clock;
    while (window.isOpen())
    {
        // Event handling
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
                // Add new particle at mouse position
                Particles newParticle(mousePress->position.x,
                    mousePress->position.y,
                    1.0f);
                newParticle.Initialize();
                particles.push_back(newParticle);
            }
        }

        // Get delta time
        float dt = clock.restart().asSeconds();

        // Fixed timestep
        const float fixedDt = 1.0f / 60.0f;
        if (dt > 0.25f) dt = 0.25f;  // Prevent spiral of death

        // Update all particles
        for (auto& p : particles) {
            p.Update(fixedDt, maxSize, minSize);  // Call member function
        }

        // Render
        window.clear(sf::Color::Black);
        for (auto& p : particles) {
            p.Draw(window);  // Call member function
        }
        window.display();
    }
    return 0;
}