#include "particles.h"

Particles::Particles(float x, float y, float m):Position(x, y), Velocity(0.f, 0.f), mass(m)
{
}

Particles::~Particles()
{
}

void Particles::Initialize()
{
    shape.setRadius(5.f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(sf::Vector2f({ 5.f ,5.f }));
    shape.setPosition(Position);
}

void Particles::Load()
{ 
	
}

void Particles::Update(float dt)
{
    sf::Vector2f F = calculateNetForce();
    sf::Vector2f a = F / mass;

    Velocity += a * dt;
    Position += Velocity * dt;

    // Update shape position
    shape.setPosition(Position);

    // Boundary limits
    const float radius = shape.getRadius();
    const float restitution = 0.7f; // bounciness

    // --- X boundaries ---
    if (Position.x - radius < 0.f) {
        Position.x = radius; // push back inside
        Velocity.x = -Velocity.x * restitution;
    }
    else if (Position.x + radius > 800.f) {
        Position.x = 800.f - radius;
        Velocity.x = -Velocity.x * restitution;
    }

    // --- Y boundaries ---
    if (Position.y - radius < 0.f) {
        Position.y = radius;
        Velocity.y = -Velocity.y * restitution;
    }
    else if (Position.y + radius > 600.f) {
        Position.y = 600.f - radius;
        Velocity.y = -Velocity.y * restitution;
    }

    // Update shape position again after correction
    shape.setPosition(Position);
}

void Particles::Draw(sf::RenderWindow& window)
{
	window.draw(shape);
}

sf::Vector2f Particles::calculateNetForce()
{
    sf::Vector2f netForce(0.f, 0.f);

    const float Fy = 9.81f * 10;
    const float Fx = 7.55f * 10;

    // Update Forces direction of collision with walls
    float dirX = (Velocity.x >= 0.f) ? 1.f : -1.f;
    float dirY = (Velocity.y >= 0.f) ? 1.f : -1.f;

    netForce.x += mass * Fx * dirX;
    netForce.y += mass * Fy * dirY;

    return netForce;
}