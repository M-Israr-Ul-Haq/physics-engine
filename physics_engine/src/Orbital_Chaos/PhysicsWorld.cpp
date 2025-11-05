#include "PhysicsWorld.h"
#include <cmath>
#include <vector>

void PhysicsWorld::update_physics(std::vector<CelestialBody>& bodies, float dt)
{
    // Vectors for pairwise distances, accelerations, and temporary positions
    sf::Vector2f r_ij;
    sf::Vector2f r_ij_new;
    std::vector<sf::Vector2f> accel(bodies.size(), { 0.f, 0.f });      // old accelerations
    std::vector<sf::Vector2f> accel_new(bodies.size(), { 0.f, 0.f });  // new accelerations
    std::vector<sf::Vector2f> pos_new(bodies.size(), { 0.f, 0.f });    // new positions

    const float EPSILON = 1e-5f; // Small value to avoid singularity

    // 1) Compute current accelerations
    for (size_t i = 0; i < bodies.size(); i++) {
        for (size_t j = 0; j < bodies.size(); j++) {
            if (i == j) continue;  // skip self-interaction

            r_ij = bodies[j].get_position() - bodies[i].get_position();
            float dist = std::sqrt(r_ij.x * r_ij.x + r_ij.y * r_ij.y);

            if (dist < EPSILON) continue; // avoid singularity / divide by zero

            float invDist3 = 1.0f / (dist * dist * dist);
            float accel_mag = PhysicsConstants::G * bodies[j].get_mass() * invDist3;

            accel[i] += r_ij * accel_mag;  // accumulate acceleration
        }
    }

    // 2) Compute new positions
    for (size_t i = 0; i < bodies.size(); i++) {
        pos_new[i] = bodies[i].get_position() +
            bodies[i].get_velocity() * dt +
            0.5f * accel[i] * dt * dt;
    }

    // 3) Compute new accelerations at new positions
    for (size_t i = 0; i < bodies.size(); i++) {
        for (size_t j = 0; j < bodies.size(); j++) {
            if (i == j) continue;

            r_ij_new = pos_new[j] - pos_new[i];
            float dist = std::sqrt(r_ij_new.x * r_ij_new.x + r_ij_new.y * r_ij_new.y);

            if (dist < EPSILON) continue;

            float invDist3 = 1.0f / (dist * dist * dist);
            float accel_mag_new = PhysicsConstants::G * bodies[j].get_mass() * invDist3;

            accel_new[i] += r_ij_new * accel_mag_new;
        }
    }

    // 4) Update velocities and positions
    for (size_t i = 0; i < bodies.size(); i++) {
        // Velocity Verlet update
        sf::Vector2f v_new = bodies[i].get_velocity() + 0.5f * (accel[i] + accel_new[i]) * dt;

        bodies[i].set_position(pos_new[i]);
        bodies[i].set_velocity(v_new);
    }
}