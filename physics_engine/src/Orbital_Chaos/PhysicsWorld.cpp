#include "PhysicsWorld.h"
#include <cmath>

void PhysicsWorld::update_planet_physics(Planets& planet, const GravitySource& source, float dt)
{
    sf::Vector2f r = planet.get_position();
    sf::Vector2f v = planet.get_velocity();
    float M = source.get_mass();
    sf::Vector2f sun_pos = source.get_position();

    // vector from sun to planet
    sf::Vector2f r_vec = sun_pos - r;
    float r_mag = std::sqrt(r_vec.x * r_vec.x + r_vec.y * r_vec.y);

    // Safety check
    if (r_mag < 1e-3f) return;

    // gravitational acceleration: a = -GM/r^3 * r_vec
    float acceleration_magnitude = PhysicsConstants::G * M / (r_mag * r_mag * r_mag);
    sf::Vector2f a_current = r_vec * acceleration_magnitude;

    // position using current velocity and acceleration
    sf::Vector2f r_new = r + v * dt + a_current * (0.5f * dt * dt);

    // new acceleration a(t + dt) at new position
    sf::Vector2f r_vec_new = r_new - sun_pos;
    float r_mag_new = std::sqrt(r_vec_new.x * r_vec_new.x + r_vec_new.y * r_vec_new.y);

    if (r_mag_new < 1e-3f) return;

    float accel_magnitude_new = -PhysicsConstants::G * M / (r_mag_new * r_mag_new * r_mag_new);
    sf::Vector2f a_new = r_vec_new * accel_magnitude_new;

    // Update velocity using average of old and new acceleration
    // v(t + dt) = v(t) + 0.5*(a(t) + a(t + dt))*dt
    sf::Vector2f v_new = v + (a_current + a_new) * (0.5f * dt);

    planet.set_position(r_new);
    planet.set_velocity(v_new);
}