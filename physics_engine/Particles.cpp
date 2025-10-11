#include "particles.h"

Particles::Particles(float x, float y, float m):Position(x, y), Velocity(0.f, 0.f), mass(m)
{
}

Particles::~Particles()
{
}

void Particles::Initialize()
{
    shape.setRadius(25.f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(sf::Vector2f({ 25.f ,25.f }));
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

    // Simple boundary collision
    if (Position.y > 600) {
        Position.y = 600;
        Velocity.y *= -0.8f; // bounce with damping
    }
}

void Particles::Draw(sf::RenderWindow& window)
{
	window.draw(shape);
}

sf::Vector2f Particles::calculateNetForce()
{
    sf::Vector2f netForce(0.f, 0.f);

    const float g = 9.81f * 10;
    netForce.y += mass * g;

    return netForce;
}