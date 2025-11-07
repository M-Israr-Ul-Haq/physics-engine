#pragma once
#include <SFML/Graphics.hpp>
#include "DoublePendulum.h"

class PendulumChaosApp
{
private:
    sf::RenderWindow window;
    DoublePendulum doublependulum;
    sf::Vector2f maxSize;
    sf::Vector2f minSize;

public:
    PendulumChaosApp();
    ~PendulumChaosApp();

    void initialize();
    void run();
    void cleanup();
};