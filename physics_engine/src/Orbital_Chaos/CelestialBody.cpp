#include "CelestialBody.h"

void CelestialBody::createPlanet(float m, float r, sf::Vector2f pos, sf::Vector2f vel)
{
    Object planet;
    planet.mass = m;
    planet.radius = r;
    planet.position = pos;
    planet.velocity = vel;
    planet.acceleration = { 0.f, 0.f };

    auto circle = std::make_shared<sf::CircleShape>(r);
    circle->setOrigin({ r, r });
    circle->setPosition(pos);
    circle->setFillColor(sf::Color::Red);
    planet.shape = circle;

    planets.emplace_back(planet);
}

void CelestialBody::createMeteor(float m, float r, sf::Vector2f pos, sf::Vector2f vel)
{
    Object meteor;
    meteor.mass = m;
    meteor.radius = r;
    meteor.position = pos;
    meteor.velocity = vel;
    meteor.acceleration = { 0.f, 0.f };

    auto rect = std::make_shared<sf::RectangleShape>(sf::Vector2f{ r * 2, r * 2 });
    rect->setOrigin({ r, r });
    rect->setPosition(pos);
    rect->setFillColor(sf::Color::White);
    meteor.shape = rect;

    meteors.emplace_back(meteor);
}

void CelestialBody::createStar(float r, sf::Vector2f pos)
{
    Object star;
    star.mass = 0.f;  // Stars don't participate in physics
    star.radius = r;
    star.position = pos;
    star.velocity = { 0.f, 0.f };
    star.acceleration = { 0.f, 0.f };

    auto circle = std::make_shared<sf::CircleShape>(r);
    circle->setOrigin({ r, r});
    circle->setPosition(pos);
    circle->setFillColor(sf::Color::Yellow);
    star.shape = circle;

    stars.emplace_back(star);
}

void CelestialBody::update(float dt)
{
    for (auto& p : planets) {
        p.velocity += p.acceleration * dt;
        p.position += p.velocity * dt;
        p.shape->setPosition(p.position);
    }

    for (auto& m : meteors) {
        m.velocity += m.acceleration * dt;
        m.position += m.velocity * dt;
        m.shape->setPosition(m.position);
    }
}

void CelestialBody::draw(sf::RenderWindow& window)
{
    for (auto& p : planets)
        window.draw(*p.shape);

    for (auto& m : meteors)
        window.draw(*m.shape);
    for (auto& s : stars)
        window.draw(*s.shape);
}