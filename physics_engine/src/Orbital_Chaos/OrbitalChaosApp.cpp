#include "OrbitalChaosApp.h"
#include "PhysicsWorld.h"

#include <random>
#include <iostream>
#include <cmath>

// Diagnostics for Confirming the planetary motion...
/* static void printDiagnostics(const std::vector<CelestialBody>& bodies)
{
    if (bodies.size() < 2) return;

    // Sun is index 0
    const CelestialBody& sun = bodies[0];

    for (size_t i = 1; i < bodies.size(); i++)
    {
        const CelestialBody& b = bodies[i];

        sf::Vector2f r = b.get_position() - sun.get_position();
        sf::Vector2f v = b.get_velocity();

        float dist = std::sqrt(r.x * r.x + r.y * r.y);
        float speed = std::sqrt(v.x * v.x + v.y * v.y);

        // Specific orbital energy  E = v^2/2 − μ/r
        float mu = PhysicsConstants::G * sun.get_mass();
        float energy = 0.5f * speed * speed - mu / dist;

        // Specific angular momentum h = |r × v|
        float h = std::abs(r.x * v.y - r.y * v.x);

        // Estimated semi-major axis  a = −μ / (2E)
        float a = -mu / (2.0f * energy);

        // Estimated eccentricity e = sqrt(1 − h^2/(aμ))
        float e = std::sqrt(1.0f - (h * h) / (a * mu));

        std::cout
            << "Planet " << i
            << " | r = " << dist
            << " | v = " << speed
            << " | a = " << a
            << " | e = " << e
            << "\n";
    }

    std::cout << "------------------------------------\n";
}
*/
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
    CelestialBody sun({ 600.f, 450.f }, { 0.f, 0.f });
    sun.set_mass(5000.f);
    sun.set_color(sf::Color::Yellow);
    sun.set_radius(20.f);

    bodies.push_back(sun);
    orbital_trails.push_back({});

    // ---- Planet data ----
    struct PlanetData {
        std::string name;
        float perihelion;     // closest distance to sun (pixels)
        float semi_major;     // semi-major axis (pixels)
        float planet_size;
        sf::Color color;
    };

    // Highly elliptical orbits for visual effect
    std::vector<PlanetData> planet_data = {
        {"Mercury",  60.f,  110.f,   4.f,   sf::Color(169,169,169)},
        {"Venus",   100.f,  150.f,   6.f,   sf::Color(255,198,73)},
        {"Earth",   130.f,  190.f,   6.5f,  sf::Color(30,144,255)},
        {"Mars",    170.f,  240.f,   5.f,   sf::Color(205,92,92)},
        {"Jupiter", 230.f,  320.f,  15.f,   sf::Color(216,202,157)},
        {"Saturn",  280.f,  370.f,  13.f,   sf::Color(238,215,137)},
        {"Uranus",  320.f,  410.f,  10.f,   sf::Color(79,208,231)},
        {"Neptune", 360.f,  450.f,  10.f,   sf::Color(72,101,255)}
    };

    sf::Vector2f sun_pos = bodies[0].get_position();
    float sun_mass = bodies[0].get_mass();

    for (const auto& data : planet_data)
    {
        // Start at perihelion (closest point)
        sf::Vector2f planet_pos(sun_pos.x + data.perihelion, sun_pos.y);

        // Velocity at perihelion using vis-viva equation
        // v = sqrt(GM * (2/r - 1/a))
        float v_peri = std::sqrt(PhysicsConstants::G * sun_mass *
            (2.0f / data.perihelion - 1.0f / data.semi_major));

        // Perpendicular velocity (upwards for counterclockwise orbit)
        sf::Vector2f vel(0.f, -v_peri);

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

    window = sf::RenderWindow(sf::VideoMode({ static_cast<unsigned>(maxSize.x), static_cast<unsigned>(maxSize.y) }), "N-Body Orbital Simulation"
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

    const int   PHYSICS_SUBSTEPS = 40;
    const float TIME_SCALE = 10.2f;  // Speed up simulation

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

        // For Diagnostics...
        /*static int frameCounter = 0;
        frameCounter++;

        if (frameCounter % 300 == 0)   // print every ~5 seconds
        {
            printDiagnostics(bodies);
        }
        */

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