#include "DoublePendulum.h"

void DoublePendulum::initialize() {
    // Initialize lengths, masses, angles
    length1 = 150.f;
    length2 = 150.f;
    mass1 = 20.0;
    mass2 = 20.0;
    theta1 = sf::degrees(90.0f);
    theta2 = sf::degrees(45.0f);
    angularVel1 = 0.0;
    angularVel2 = 0.0;

    // Pin the top point
    float pinpoint = mass1 / 3;
    pin.setRadius(pinpoint);
    pin.setPosition(sf::Vector2f{ 450,100 });
    pin.setFillColor(sf::Color::Magenta);
    pin.setOrigin(sf::Vector2f{ pinpoint, pinpoint });

    // Setup bob1
    bob1.setRadius(mass1);
    bob1.setFillColor(sf::Color::Red);
    bob1.setOrigin(sf::Vector2f{ mass1, mass1 }); // Center the origin
    bob1.setPosition(sf::Vector2f{ 450, 250 });

    // Setup rod1
    rod1.setSize(sf::Vector2f(2.0f, length1));
    rod1.setPosition(sf::Vector2f{ 450, 100 });
    rod1.setFillColor(sf::Color::White);
    rod1.setOutlineThickness(2.0f);
    rod1.setOrigin(sf::Vector2f{ 1.f, 0.f });

    // Setup bob2  
    bob2.setRadius(mass2);
    bob2.setFillColor(sf::Color::Blue);
    bob2.setOrigin(sf::Vector2f{ mass2, mass2 }); // Center the origin
    bob2.setPosition(sf::Vector2f{ 450, 400 });

    // Setup rod2
    rod2.setSize(sf::Vector2f(2.0f, length1));
    rod2.setPosition(sf::Vector2f{ 450, 250 });
    rod2.setFillColor(sf::Color::White);
    rod2.setOutlineThickness(2.0f);
    rod2.setOrigin(sf::Vector2f{ 1.f, 0.f });
}

void DoublePendulum::update(float dt) {
    // Acceleration due to gravity
    const double g = 9.81;

    // Basic pendulum motion for bob1
    double angularAcc1 = -g / length1 * std::sin(theta1.asRadians());

    // Update velocity and angle
    angularVel1 += angularAcc1 * dt;
    theta1 += sf::radians(angularVel1 * dt);

    // Update positions
    sf::Vector2f pivot(450, 100);
    sf::Vector2f bob1Pos = pivot + sf::Vector2f(
        length1 * std::sin(theta1.asRadians()),
        length1 * std::cos(theta1.asRadians())
    );

    bob1.setPosition(bob1Pos);
    rod1.setPosition(pivot);
    rod1.setRotation(sf::degrees(-theta1.asDegrees()));

    // For now, just make bob2 follow bob1 with fixed offset
    bob2.setPosition(bob1Pos + sf::Vector2f(0, length2));
    rod2.setPosition(bob1Pos);
    rod2.setRotation(sf::degrees(0)); // Keep it vertical for now
}

void DoublePendulum::draw(sf::RenderWindow& window) {
    window.draw(rod1);
    window.draw(rod2);
    window.draw(pin);
    window.draw(bob1);
    window.draw(bob2);
}