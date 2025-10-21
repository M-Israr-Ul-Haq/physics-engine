#pragma once
#include <SFML/Graphics.hpp>
#include "Particle.h"

class Collision
{
private:
    static float distance(const sf::Vector2f& p1, const sf::Vector2f& p2);
    static float dotProduct(const sf::Vector2f& v1, const sf::Vector2f& v2);
public:
   
    // Collision detection
    static bool checkParticleCollision(const Particle& p1, const Particle& p2);
    static bool checkWallCollision(const sf::Vector2f& position, float radius, const sf::Vector2f& maxSize, const sf::Vector2f& minSize);

    // Collision resolution
    static void resolveParticleCollision(Particle& p1, Particle& p2);
    static void resolveWallCollision(Particle& particle, float dt, const sf::Vector2f& maxSize, const sf::Vector2f& minSize);

    // Time of impact calculation for CCD (contineous collision detection)
    static float computeTOI(const sf::Vector2f& position, const sf::Vector2f& velocity,
        float radius, float dt, const sf::Vector2f& maxSize,
        const sf::Vector2f& minSize);
};