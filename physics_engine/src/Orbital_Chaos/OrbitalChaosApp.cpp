#include "OrbitalChaosApp.h"
#include "PhysicsWorld.h"
#include <random>
#include <iostream>
#include <cmath>

OrbitalChaosApp::OrbitalChaosApp()
    : maxSize(1200.f, 900.f),  
    minSize(0.f, 0.f),
    sun(sf::Vector2f(600.f, 450.f), 10000.f)  
{
    setup_planets();
}

OrbitalChaosApp::~OrbitalChaosApp()
{
}

void OrbitalChaosApp::setup_planets()
{
    // Planetary data: radius from sun, relative size, color
    // Scaled to fit in 1200x900 window (position of sun 600, 450)
    struct PlanetData {
        std::string name;
        float orbital_radius;
        float planet_size;
        sf::Color color;
    };

    std::vector<PlanetData> planet_data = {
        {"Mercury", 50.f,  3.f,  sf::Color(169, 169, 169)},      
        {"Venus",   70.f,  5.f,  sf::Color(255, 198, 73)},     
        {"Earth",   95.f,  6.f,  sf::Color(30, 144, 255)},     
        {"Mars",    125.f, 4.5f, sf::Color(205, 92, 92)},      
        {"Jupiter", 180.f, 14.f, sf::Color(216, 202, 157)},    
        {"Saturn",  240.f, 12.f, sf::Color(238, 215, 137)},    
        {"Uranus",  300.f, 9.f,  sf::Color(79, 208, 231)},     
        {"Neptune", 360.f, 9.f,  sf::Color(72, 101, 255)}      
    };

    sf::Vector2f sun_pos = sun.get_position();
    float sun_mass = sun.get_mass();

    // Create each planet
    for (const auto& data : planet_data) {
        // Positioned planets to the right of sun
        sf::Vector2f planet_pos(sun_pos.x + data.orbital_radius, sun_pos.y);

        // Circular orbit velocity
        float v_circular = std::sqrt(PhysicsConstants::G * sun_mass / data.orbital_radius);

        // Set velocity perpendicular to radius 
        sf::Vector2f planet_vel(0.f, v_circular);

        // Create planet
        Planets planet(planet_pos, planet_vel);
        planet.set_mass(1.0f);  // Mass doesn't affect orbit in this simulation
        planet.set_color(data.color);
        planet.set_radius(data.planet_size);

        planets.push_back(planet);

        // Initialize empty trail for this planet
        orbital_trails.push_back(std::deque<sf::Vector2f>());
    }
}

void OrbitalChaosApp::initialize()
{
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    window.create(sf::VideoMode({ static_cast<unsigned int>(maxSize.x),
                                  static_cast<unsigned int>(maxSize.y) }),
        "Solar System Simulation - All Planets");
    window.setFramerateLimit(60);
}

void OrbitalChaosApp::update_trails()
{
    trail_update_counter++;

    // Update trails every N frames to avoid overcrowding
    if (trail_update_counter >= TRAIL_UPDATE_INTERVAL) {
        trail_update_counter = 0;

        for (size_t i = 0; i < planets.size(); ++i) {
            orbital_trails[i].push_back(planets[i].get_position());

            // Remove old trail points if too long
            if (orbital_trails[i].size() > MAX_TRAIL_LENGTH) {
                orbital_trails[i].pop_front();
            }
        }
    }
}

void OrbitalChaosApp::render_trails()
{
    for (size_t i = 0; i < orbital_trails.size(); ++i) {
        const auto& trail = orbital_trails[i];

        if (trail.size() < 2) continue;

        // Get planet color for trail
        sf::Color planet_color = planets[i].get_color();

        // Draw trail points as small circles
        for (size_t j = 0; j < trail.size(); ++j) {
            // Fade older trail points
            float alpha_factor = static_cast<float>(j) / static_cast<float>(trail.size());
            sf::Color trail_color = planet_color;
            trail_color.a = static_cast<std::uint8_t>(80 * alpha_factor);

            sf::CircleShape trail_point(1.5f);  // Small circle for trail
            trail_point.setPosition(trail[j]);
            trail_point.setOrigin(sf::Vector2f{ 1.5f, 1.5f });
            trail_point.setFillColor(trail_color);

            window.draw(trail_point);
        }
    }
}

void OrbitalChaosApp::run()
{
    sf::Clock clock;

    // Physics sub step configuration
    const int PHYSICS_SUBSTEPS = 10;
    const float TIME_SCALE = 5.0f;  // Adjust orbit speed

    while (window.isOpen())
    {
        // Event handling
        while (auto eventOpt = window.pollEvent())
        {
            if (eventOpt->is<sf::Event::Closed>())
                window.close();

            // Press SPACE to clear trails
            if (eventOpt->is<sf::Event::KeyPressed>()) {
                if (eventOpt->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Space) {
                    for (auto& trail : orbital_trails) {
                        trail.clear();
                    }
                }
            }
        }

        // Get frame time
        float frame_dt = clock.restart().asSeconds();
        if (frame_dt > 0.25f) frame_dt = 0.25f;

        // Physics update with substeps
        float physics_dt = (frame_dt * TIME_SCALE) / static_cast<float>(PHYSICS_SUBSTEPS);

        for (int i = 0; i < PHYSICS_SUBSTEPS; ++i) {
            for (auto& planet : planets) {
                physics_world.update_planet_physics(planet, sun, physics_dt);
            }
        }

        update_trails();
        window.clear(sf::Color::Black);

        // Draw trails first (behind planets)
        render_trails();

        sun.render(window);
        // Draw all planets
        for (auto& planet : planets) {
            planet.render(window);
        }

        window.display();
    }
}

void OrbitalChaosApp::cleanup()
{
    if (window.isOpen())
        window.close();
    std::cout << "Resources released successfully!" << std::endl;
}