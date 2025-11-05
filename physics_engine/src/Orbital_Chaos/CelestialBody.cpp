#include "CelestialBody.h"
#include <iostream>

CelestialBody::CelestialBody(sf::Vector2f pos, sf::Vector2f vel)
    : position(pos), velocity(vel), mass(1.0f)  
{
    body.setFillColor(sf::Color::White);
    body.setRadius(8.f);
    body.setOrigin(sf::Vector2f{ 8.f, 8.f });  // CRITICAL: center the origin!
}

void CelestialBody::render(sf::RenderWindow& window)
{
    body.setPosition(position);
    window.draw(body);
}