#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class DoublePendulum
{
private:
    float length1;
    float length2;
    
    float mass1;
    float mass2;

    sf::Angle theta1;
    sf::Angle theta2;
    double angularVel1;
    double angularVel2;

    sf::CircleShape bob1;
    sf::CircleShape bob2;
    sf::RectangleShape rod1;
    sf::RectangleShape rod2;

    sf::CircleShape pin;
public:
    void initialize();
    void update(float dt);
    void draw(sf::RenderWindow& window);
};