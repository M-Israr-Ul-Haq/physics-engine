#include "particles.h"
#include <random>
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
    shape.setRadius(25.f);
    shape.setOrigin(sf::Vector2f(25.f, 25.f));
    shape.setPosition(Position);

    // Set random color
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> colorDist(0, 5);

    sf::Color colors[] = { sf::Color::Red, sf::Color::Green, sf::Color::Blue,
                          sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan };
    shape.setFillColor(colors[colorDist(gen)]);

    // Set random velocity on initialization
    setRandomVelocity();
}

void Particles::Load()
{
    // (empty for now)
}

// -------------Continuous Collision Detection Update----------------
void Particles::Update(float dt, const sf::Vector2f& maxSize, const sf::Vector2f& minSize)
{
    // 1. Compute continuous collision time
    float radius = shape.getRadius();
    float tc = computeTOI(Position, Velocity, radius, dt, maxSize, minSize);

    const float restitution = 1.0f; // perfectly elastic collision (no energy loss)

    if (tc >= 0.0f && tc <= 1.0f)
    {
        // ---- Collision occurs within this frame ----

        // 2. Move up to the collision point
        Position += Velocity * (tc * dt);

        // 3. Reflect velocity depending on which boundary was hit
        if (Position.x - radius <= minSize.x || Position.x + radius >= maxSize.x)
            Velocity.x = -Velocity.x * restitution;

        if (Position.y - radius <= minSize.y || Position.y + radius >= maxSize.y)
            Velocity.y = -Velocity.y * restitution;

        // 4. Move remaining time after the bounce
        float remaining = 1.0f - tc;
        Position += Velocity * (remaining * dt);
    }
    else
    {
        // ---- No collision this frame ----
        Position += Velocity * dt;
    }

    // 5. Update shape position
    shape.setPosition(Position);
}

// -----------------Drawing-------------------
void Particles::Draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

// ---------------Random Velocity Initialization---------------------
void Particles::setRandomVelocity(float minSpeed, float maxSpeed)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> speedDist(minSpeed, maxSpeed);
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159f);

    float speed = speedDist(gen);
    float angle = angleDist(gen);

    Velocity.x = speed * cos(angle);
    Velocity.y = speed * sin(angle);
}

// --------------- Particle-Particle Collision ------------------
void Particles::p2pCollision(Particles& other)
{
    sf::Vector2f d = this->shape.getPosition() - other.shape.getPosition();
    float distance = std::sqrt(d.x * d.x + d.y * d.y);
    float radiusSum = this->shape.getRadius() + other.shape.getRadius();

    if (distance == 0.0f || distance >= radiusSum) return;

    // Normalized direction vector
    sf::Vector2f normal = d / distance;

    // Relative velocity
    sf::Vector2f v = this->Velocity - other.Velocity;
    float vDotN = v.x * normal.x + v.y * normal.y;

    // Ignore if already separating
    if (vDotN > 0) return;

    // ---------- Debug: before collision ----------
    float KE_before = 0.5f * this->mass * (this->Velocity.x * this->Velocity.x + this->Velocity.y * this->Velocity.y)
        + 0.5f * other.mass * (other.Velocity.x * other.Velocity.x + other.Velocity.y * other.Velocity.y);

    sf::Vector2f P_before = this->Velocity * this->mass + other.Velocity * other.mass;

    // Elastic collision response
    float mSum = this->mass + other.mass;
    float impulse = (2.0f * vDotN) / mSum;

    this->Velocity -= impulse * other.mass * normal;
    other.Velocity += impulse * this->mass * normal;

    // ---------- Debug: after collision ----------
    float KE_after = 0.5f * this->mass * (this->Velocity.x * this->Velocity.x + this->Velocity.y * this->Velocity.y)
        + 0.5f * other.mass * (other.Velocity.x * other.Velocity.x + other.Velocity.y * other.Velocity.y);

    sf::Vector2f P_after = this->Velocity * this->mass + other.Velocity * other.mass;

    std::cout << std::fixed;
    std::cout << "\n---- Collision Diagnostics ----\n";
    std::cout << "Momentum before       : (" << P_before.x << ", " << P_before.y << ")\n";
    std::cout << "Momentum after        : (" << P_after.x << ", " << P_after.y << ")\n";
    std::cout << "Change in Momentum    : (" << (P_after.x - P_before.x) << ", " << (P_after.y - P_before.y) << ")\n";
    std::cout << "KE before             : " << KE_before << "\n";
    std::cout << "KE after              : " << KE_after << "\n";
    std::cout << "Change in KE          : " << (KE_after - KE_before) << "\n";
    std::cout << "--------------------------------\n";

    // Separate overlapping particles
    float overlap = radiusSum - distance;
    sf::Vector2f correction = normal * (overlap / 2.0f);
    this->shape.move(correction);
    other.shape.move(-correction);
}

// --------------Compute Time of Impact (Continuous Collision Detection)------------------
float Particles::computeTOI(const sf::Vector2f& Position,
    const sf::Vector2f& Velocity,
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
    if (Velocity.y < 0.0f) // moving up -> possible top wall hit
    {
        float tTop = (minSize.y + radius - Position.y) / (Velocity.y * dt);
        considerCandidate(tTop);
    }
    else if (Velocity.y > 0.0f) // moving down -> possible bottom wall hit
    {
        float tBottom = (maxSize.y - radius - Position.y) / (Velocity.y * dt);
        considerCandidate(tBottom);
    }

    return tc; // -1 if no collision this frame, otherwise 0 ≤ tc ≤ 1
}