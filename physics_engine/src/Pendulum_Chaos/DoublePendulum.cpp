#include "DoublePendulum.h"

void DoublePendulum::initialize() {
    // Initialize lengths, masses, angles
    length1 = 150.f;
    length2 = 150.f;
    mass1 = 20.0;
    mass2 = 20.0;
    theta1 = sf::degrees(180.0f);
    theta2 = sf::degrees(90.0f);
    angularVel1 = 0.0;
    angularVel2 = 0.0;

    // Pin the top point
    float pinpoint = mass1 / 3;
    pin.setRadius(pinpoint);
    pin.setPosition(sf::Vector2f{ 450,200 });
    pin.setFillColor(sf::Color::Magenta);
    pin.setOrigin(sf::Vector2f{ pinpoint, pinpoint });

    // Setup bob1
    bob1.setRadius(mass1);
    bob1.setFillColor(sf::Color::Red);
    bob1.setOrigin(sf::Vector2f{ mass1, mass1 });
    bob1.setPosition(sf::Vector2f{ 450, 350 });

    // Setup rod1
    rod1.setSize(sf::Vector2f(2.0f, length1));
    rod1.setPosition(sf::Vector2f{ 450, 200 });
    rod1.setFillColor(sf::Color::White);
    rod1.setOutlineThickness(2.0f);
    rod1.setOrigin(sf::Vector2f{ 1.f, 0.f });

    // Setup bob2  
    bob2.setRadius(mass2);
    bob2.setFillColor(sf::Color::Blue);
    bob2.setOrigin(sf::Vector2f{ mass2, mass2 });
    bob2.setPosition(sf::Vector2f{ 450, 500 });

    // Setup rod2
    rod2.setSize(sf::Vector2f(2.0f, length1));
    rod2.setPosition(sf::Vector2f{ 450, 350 });
    rod2.setFillColor(sf::Color::White);
    rod2.setOutlineThickness(2.0f);
    rod2.setOrigin(sf::Vector2f{ 1.f, 0.f });

    // Initialize trail
    trail.setPrimitiveType(sf::PrimitiveType::LineStrip);
    trailPoints.clear();
}

void DoublePendulum::update(float dt) {
    // Acceleration due to gravity
    const double g = 9.81;

    // --------------------- Angular Acceleration 1 Sub_Calculations ---------------------
    double AA1_num1 = -g * (2 * mass1 + mass2) * std::sin(theta1.asRadians());
    double AA1_num2 = -mass2 * g * std::sin(theta1.asRadians() - 2 * theta2.asRadians());
    double AA1_num3 = -2 * std::sin(theta1.asRadians() - theta2.asRadians()) * mass2;
    double AA1_num4 = angularVel2 * angularVel2 * length2 + angularVel1 * angularVel1 * length1 * std::cos(theta1.asRadians() - theta2.asRadians());
    double AA1_den1 = length1 * (2 * mass1 + mass2 - mass2 * std::cos(2 * theta1.asRadians() - 2 * theta2.asRadians()));

    // --------------------- Angular Acceleration 2 Sub_Calculations ---------------------
    double AA2_num1 = 2 * std::sin(theta1.asRadians() - theta2.asRadians());
    double AA2_num2 = angularVel1 * angularVel1 * length1 * (mass1 + mass2);
    double AA2_num3 = g * (mass1 + mass2) * std::cos(theta1.asRadians());
    double AA2_num4 = angularVel2 * angularVel2 * length2 * mass2 * std::cos(theta1.asRadians() - theta2.asRadians());
    double AA2_den2 = length2 * (2 * mass1 + mass2 - mass2 * std::cos(2 * theta1.asRadians() - 2 * theta2.asRadians()));

    // Angular Acceleration Final Form 
    double angularAcc1 = (AA1_num1 + AA1_num2 + AA1_num3 * AA1_num4) / AA1_den1;
    double angularAcc2 = (AA2_num1 * (AA2_num2 + AA2_num3 + AA2_num4)) / AA2_den2;

    // Update velocity and angle
    angularVel1 += angularAcc1 * dt;
    theta1 += sf::radians(angularVel1 * dt);
    angularVel2 += angularAcc2 * dt;
    theta2 += sf::radians(angularVel2 * dt);

    // Update positions
    sf::Vector2f pivot(450, 200);
    sf::Vector2f bob1Pos = pivot + sf::Vector2f(
        length1 * std::sin(theta1.asRadians()),
        length1 * std::cos(theta1.asRadians())
    );
    sf::Vector2f bob2Pos = sf::Vector2f(
        bob1Pos.x + length2 * std::sin(theta2.asRadians()),
        bob1Pos.y + length2 * std::cos(theta2.asRadians())
    );

    // Set positions
    bob1.setPosition(bob1Pos);
    rod1.setPosition(pivot);
    rod1.setRotation(sf::degrees(-theta1.asDegrees()));

    bob2.setPosition(bob2Pos);
    rod2.setPosition(bob1Pos);
    rod2.setRotation(sf::degrees(-theta2.asDegrees()));

    // Update trail
    trails();
}

void DoublePendulum::trails() {
    // Add current bob2 position to trail
    trailPoints.push_back(bob2.getPosition());

    // Limit trail length
    /*if (trailPoints.size() > maxTrailPoints) {
        trailPoints.erase(trailPoints.begin());
    }*/

    // Update vertex array
    trail.clear();
    for (size_t i = 0; i < trailPoints.size(); ++i) {
        sf::Color trailColor(100, 150, 255, 255);

        sf::Vertex vertex;
        vertex.position = trailPoints[i];
        vertex.color = trailColor;
        trail.append(vertex);
    }
}

void DoublePendulum::draw(sf::RenderWindow& window) {
    window.draw(trail);  
    window.draw(rod1);
    window.draw(rod2);
    window.draw(pin);
    window.draw(bob1);
    window.draw(bob2);
}