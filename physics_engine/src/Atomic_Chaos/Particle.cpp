#include <random>
#include <math.h>
#include "Particle.h"
#include "Collision.h"

// -------------Constructor / Destructor----------------
Particle::Particle(float x, float y, float m)
    : Position(x, y), Velocity(0.f, 0.f), mass(m), angleV(0.0f), Rotation(0.0f)
{
}

Particle::~Particle()
{
}

// -------------Initialization----------------
void Particle::Initialize()
{
    shape.setRadius(5.f);
    shape.setOrigin(sf::Vector2f(5.f, 5.f));
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
    const float height = 1.0f;
    axis.setSize(sf::Vector2f(width, height));
    axis.setPosition(shape.getPosition());
    axis.setOrigin(sf::Vector2f{ width / 2.f, height / 2.f });
    axis.setFillColor(sf::Color::Black);

    // Set random velocity & angular velocity on initialization
    setRandomVelocity();
    setRandomAngularVelocity(-5.0f, 5.0f);
}

void Particle::Load()
{
    // (empty for now)
}

// -------------Update----------------
void Particle::Update(float dt, const sf::Vector2f& maxSize, const sf::Vector2f& minSize)
{
    Rotation += angleV * dt;
    angleV *= 0.99f;

    // Resolve the collision with walls
    Collision::resolveWallCollision(*this, dt, maxSize, minSize);

    // Update shape position and rotation
    shape.setPosition(Position);
    shape.setRotation(sf::radians(Rotation));

    // Update axis position and rotation
    axis.setPosition(Position);
    axis.setRotation(sf::radians(Rotation));
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

// ---------------Random Angular Velocity Initialization---------------------
void Particle::setRandomAngularVelocity(float minSpin, float maxSpin)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> spinDist(minSpin, maxSpin);

    angleV = spinDist(gen);     // radians per second
    Rotation = 0.0f;            // Start at 0 rotation
}

// -----------------Drawing-------------------
void Particle::Draw(sf::RenderWindow& window)
{
    window.draw(shape);
    window.draw(axis);
}