#include <random>
#include <math.h>
#include "particle.h"
#include "Collision.h"

// -------------Constructor / Destructor----------------
Particle::Particle(float x, float y, float m)
    : Position(x, y), Velocity(0.f, 0.f), mass(m), angleA( 0.0f ), angleV(0.0f)
{
}

Particle::~Particle()
{
}

// -------------Initialization----------------
void Particle::Initialize()
{
    shape.setRadius(15.f);
    shape.setOrigin(sf::Vector2f(15.f, 15.f));
    shape.setPosition(Position);

    // Set random color
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> colorDist(0, 5);

    sf::Color colors[] = { sf::Color::Red, sf::Color::Green, sf::Color::Blue,
                          sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan };
    shape.setFillColor(colors[colorDist(gen)]);

    // Setting the axis of the particles
    const float width = shape.getRadius();
    const float height = 2.0f;

    axis.setSize(sf::Vector2f(width, height));
    axis.setPosition(shape.getPosition());
    axis.setFillColor(sf::Color::Black);

    // Set random velocity on initialization
    setRandomVelocity();
}

void Particle::Load()
{
    // (empty for now)
}

// -------------Continuous Collision Detection Update----------------
void Particle::Update(float dt, const sf::Vector2f& maxSize, const sf::Vector2f& minSize)
{
    // Resolve the collision with walls
    Collision::resolveWallCollision(*this, dt, maxSize, minSize);

    // Spin the particles
    Collision::spinningParticles(*this);

    // Update shape & axis position 
    shape.setPosition(Position);
    axis.setPosition(Position);
}

// ---------------Random Velocity Initialization---------------------
void Particle::setRandomVelocity(float minSpeed, float maxSpeed)
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

// -----------------Drawing-------------------
void Particle::Draw(sf::RenderWindow& window)
{
    window.draw(shape);
    window.draw(axis);
}