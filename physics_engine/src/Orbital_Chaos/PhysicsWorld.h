#pragma once
#include <SFML/Graphics.hpp>
#include "CelestialBody.h"

namespace PhysicsConstants {
    constexpr float G = 1.0f;
}

class PhysicsWorld
{
public:
    void update_planet_physics(Planets& planet, const GravitySource& source, float dt);
};