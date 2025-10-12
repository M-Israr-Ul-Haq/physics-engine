#include "particles.h"
#include <math.h>

// -------------Constructor / Destructor----------------
Particles::Particles(float x, float y, float m)
    : Position(x, y), Velocity(0.f, 0.f), mass(m)
{
}

Particles::~Particles()
{
}

// -------------Initialization----------------
void Particles::Initialize()
{
    shape.setRadius(5.f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(sf::Vector2f(5.f, 5.f));
    shape.setPosition(Position);
}

void Particles::Load()
{
    // (empty for now)
}

// -------------Continuous Collision Detection Update----------------
void Particles::Update(float dt, const sf::Vector2f& maxSize, const sf::Vector2f& minSize)
{
    // 1. Compute forces and acceleration
    sf::Vector2f F = calculateNetForce();
    sf::Vector2f a = F / mass;

    // 2. Update velocity (semi-implicit Euler)
    Velocity += a * dt;

    // 3. Compute continuous collision time
    float radius = shape.getRadius();
    float tc = computeTOI(Position, Velocity, radius, dt, maxSize, minSize);

    const float restitution = 0.7f; // bounce factor

    if (tc >= 0.0f && tc <= 1.0f)
    {
        // ---- Collision occurs within this frame ----

        // 4. Move up to the collision point
        Position += Velocity * (tc * dt);

        // 5. Reflect velocity depending on which boundary was hit
        if (Position.x - radius <= minSize.x || Position.x + radius >= maxSize.x)
            Velocity.x = -Velocity.x * restitution;

        if (Position.y - radius <= minSize.y || Position.y + radius >= maxSize.y)
            Velocity.y = -Velocity.y * restitution;

        // 6. Move remaining time after the bounce
        float remaining = 1.0f - tc;
        Position += Velocity * (remaining * dt);
    }
    else
    {
        // ---- No collision this frame ----
        Position += Velocity * dt;
    }

    // 7. Update shape position
    shape.setPosition(Position);
}

// -----------------Drawing-------------------
void Particles::Draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

// ---------------Force calculation---------------------
sf::Vector2f Particles::calculateNetForce()
{
    sf::Vector2f netForce(0.f, 0.f);

    const float Fy = 9.81f * 10;
    const float Fx = 7.55f * 10;

    // Force direction based on velocity
    float dirX = (Velocity.x >= 0.f) ? 1.f : -1.f;
    float dirY = (Velocity.y >= 0.f) ? 1.f : -1.f;

    netForce.x += mass * Fx * dirX;
    netForce.y += mass * Fy * dirY;

    return netForce;
}

// --------------Compute Time of Impact (Continuous Collision Detection)------------------
float Particles::computeTOI(sf::Vector2f& Position,
    sf::Vector2f& Velocity,
    float radius,
    float dt,
    const sf::Vector2f& maxSize,
    const sf::Vector2f& minSize)
{
    float tc = -1.0f; // no collision by default

    auto considerCandidate = [&](float tCandidate)
        {
            // Valid candidate must be within [0, 1]
            if (tCandidate >= 0.0f && tCandidate <= 1.0f)
            {
                if (tc < 0.0f || tCandidate < tc)
                    tc = tCandidate;
            }
        };

    // --- X boundaries ---
    if (Velocity.x < 0.0f) // moving left -> possible left wall hit
    {
        float tLeft = (minSize.x + radius - Position.x) / (Velocity.x * dt);
        considerCandidate(tLeft);
    }
    else if (Velocity.x > 0.0f) // moving right -> possible right wall hit
    {
        float tRight = (maxSize.x - radius - Position.x) / (Velocity.x * dt);
        considerCandidate(tRight);
    }

    // --- Y boundaries ---
    if (Velocity.y < 0.0f) // moving down -> possible bottom wall hit
    {
        float tBottom = (minSize.y + radius - Position.y) / (Velocity.y * dt);
        considerCandidate(tBottom);
    }
    else if (Velocity.y > 0.0f) // moving up -> possible top wall hit
    {
        float tTop = (maxSize.y - radius - Position.y) / (Velocity.y * dt);
        considerCandidate(tTop);
    }

    return tc; // -1 if no collision this frame, otherwise 0 ≤ tc ≤ 1
}