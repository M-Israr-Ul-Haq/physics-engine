#include "CelestialBody.h"
#include <iostream>

GravitySource::GravitySource(sf::Vector2f pos, float m) 
    : position(pos), mass(m)  
{
    sun.setPosition(pos);
    sun.setOrigin(sf::Vector2f{ 15.f, 15.f });  
    sun.setFillColor(sf::Color::Yellow);  
    sun.setRadius(15.f);
}

void GravitySource::render(sf::RenderWindow& window)
{
    window.draw(sun);
}

Planets::Planets(sf::Vector2f pos, sf::Vector2f vel)
    : position(pos), velocity(vel), mass(1.0f)  
{
    planet.setFillColor(sf::Color::Blue);
    planet.setRadius(8.f);
    planet.setOrigin(sf::Vector2f{ 8.f, 8.f });  // CRITICAL: center the origin!
    planet.setPosition(position);
}

void Planets::render(sf::RenderWindow& window)
{
    planet.setPosition(position);
    window.draw(planet);
}