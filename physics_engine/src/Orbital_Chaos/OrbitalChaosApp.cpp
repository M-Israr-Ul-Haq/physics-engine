#include "OrbitalChaosApp.h"
#include "PhysicsWorld.h"

#include <random>
#include <iostream>
#include <cmath>

OrbitalChaosApp::OrbitalChaosApp()
    : maxSize(1200.f, 900.f),
    minSize(0.f, 0.f)
{
    setup_bodies();
}

OrbitalChaosApp::~OrbitalChaosApp() {}

void OrbitalChaosApp::setup_bodies()
{
    // ---- Sun ----
    CelestialBody sun({ 400.f, 450.f }, { 0.f, 0.f });
    sun.set_mass(10000.f);
    sun.set_color(sf::Color::Yellow);
    sun.set_radius(15.f);

    bodies.push_back(sun);
    orbital_trails.push_back({});

    // ---- Planet data ----
    struct PlanetData {
        std::string name;
        float orbital_radius;
        float planet_size;
        sf::Color color;
    };

    std::vector<PlanetData> planet_data = {
    {"Mercury",  90.f,  3.f * 2.5f,  sf::Color(169, 169, 169)},  // 7.5px
    {"Venus",   140.f,  5.f * 2.5f,  sf::Color(255, 198, 73)},   // 12.5px
    {"Earth",   200.f,  6.f * 2.5f,  sf::Color(30, 144, 255)},   // 15px
    {"Mars",    270.f,  4.5f * 2.5f, sf::Color(205, 92, 92)},    // 11.25px
    {"Jupiter", 380.f, 14.f * 2.5f,  sf::Color(216, 202, 157)},  // 35px
    {"Saturn",  500.f, 12.f * 2.5f,  sf::Color(238, 215, 137)},  // 30px
    {"Uranus",  640.f,  9.f * 2.5f,  sf::Color(79, 208, 231)},   // 22.5px
    {"Neptune", 780.f,  9.f * 2.5f,  sf::Color(72, 101, 255)}    // 22.5px
    };

    sf::Vector2f sun_pos = bodies[0].get_position();
    float sun_mass = bodies[0].get_mass();

    for (const auto& data : planet_data)
    {
        // Position from sun
        sf::Vector2f planet_pos(sun_pos.x + data.orbital_radius, sun_pos.y);

        // Circular orbit velocity
        float v_circ = std::sqrt(PhysicsConstants::G * sun_mass / data.orbital_radius);

        // Perpendicular velocity
        float eccentricity = 0.3f;
        sf::Vector2f vel(0.f, v_circ* (1.0f - eccentricity));

        CelestialBody p(planet_pos, vel);
        p.set_mass(1.f);
        p.set_color(data.color);
        p.set_radius(data.planet_size);

        bodies.push_back(p);
        orbital_trails.push_back({});
    }
}

void OrbitalChaosApp::initialize()
{
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    window = sf::RenderWindow(sf::VideoMode( { static_cast<unsigned>(maxSize.x), static_cast<unsigned>(maxSize.y) }), "N-Body Orbital Simulation"
    );

    window.setFramerateLimit(60);
}

void OrbitalChaosApp::update_trails()
{
    trail_update_counter++;

    if (trail_update_counter >= TRAIL_UPDATE_INTERVAL)
    {
        trail_update_counter = 0;

        for (size_t i = 0; i < bodies.size(); ++i)
        {
            orbital_trails[i].push_back(bodies[i].get_position());

            if (orbital_trails[i].size() > MAX_TRAIL_LENGTH)
                orbital_trails[i].pop_front();
        }
    }
}

void OrbitalChaosApp::render_trails()
{
    for (size_t i = 0; i < orbital_trails.size(); ++i)
    {
        const auto& trail = orbital_trails[i];

        if (trail.size() < 2) continue;

        sf::Color base = bodies[i].get_color();

        for (size_t j = 0; j < trail.size(); ++j)
        {
            float t = (float)j / trail.size();

            sf::Color c = base;
            c.a = (uint8_t)(80 * t);

            sf::CircleShape dot(1.5f);
            dot.setOrigin({ 1.5f, 1.5f });
            dot.setFillColor(c);
            dot.setPosition(trail[j]);

            window.draw(dot);
        }
    }
}

void OrbitalChaosApp::run()
{
    sf::Clock clock;

    const int   PHYSICS_SUBSTEPS = 30;
    const float TIME_SCALE = 2.0f;

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

        float physics_dt = (frame_dt * TIME_SCALE) / (float)PHYSICS_SUBSTEPS;

        for (int n = 0; n < PHYSICS_SUBSTEPS; ++n)
        {
            physics_world.update_physics(bodies, physics_dt);
        }

        update_trails();

        window.clear(sf::Color::Black);

        render_trails();

        for (auto& b : bodies)
            b.render(window);

        window.display();
    }
}

void OrbitalChaosApp::cleanup()
{
    if (window.isOpen())
        window.close();

    std::cout << "Simulation exited cleanly.\n";
}
