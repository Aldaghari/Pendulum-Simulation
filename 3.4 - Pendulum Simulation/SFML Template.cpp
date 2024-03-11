#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

const float PI = 3.14159265359;

class Pendulum
{
private:
    // Bob
    float radius;
    float mass;
    float angle;
    float angularVelocity;
    float angularAcceleration;
    sf::CircleShape bobShape;
    sf::Vector2f pos;
    sf::Vector2f velocity;

    // Arm
    sf::Vector2f armStartPos;
    sf::Vector2f armEndPos;
    sf::Vertex armLine[2];
    float armLength;

public:
    Pendulum(float r, float m, float a) : radius{ r }, mass{ m }, angle{ a }, angularVelocity{ 0.0f }, angularAcceleration { 0.0f },
        pos{ 400.0f, 300.0f }, armStartPos(400.0f, 0.0f), armLength { 300.0f }
    {
        // Bob
        bobShape.setRadius(radius);
        bobShape.setOrigin(radius, radius);
        bobShape.setFillColor(sf::Color::Transparent);
        bobShape.setOutlineThickness(1.1f);
        bobShape.setOutlineColor(sf::Color::White);


        // Arm
        armEndPos = pos;
        armLine[0] = sf::Vertex(armStartPos);
        armLine[1] = sf::Vertex(armEndPos);

        pos.x = 400.0f + armLength * sin(angle);
        pos.y = armLength * cos(angle);

}

    void update()
    {
        // Bob
        angularVelocity += angularAcceleration;
        angle += angularVelocity;

        pos.x = 400.0f + armLength * sin(angle);
        pos.y = armLength * cos(angle);


        bobShape.setPosition(pos);
        angularAcceleration = 0.0f;
        angularVelocity /= 1.001f;


        // arm
        armEndPos = pos;
        armLine[1] = sf::Vertex(armEndPos);
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(bobShape);
        window.draw(armLine, 2, sf::Lines);
    }

    void applyForce()
    {
        float gravity = -0.0005f;
        angularAcceleration = gravity * sin(angle);
    }

    void spawnAtMouse(const sf::Event& event, const sf::RenderWindow& window)
    {
        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

        if (event.mouseButton.button == sf::Mouse::Left)
        {
            pos = mousePos;
        }
    }

};


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    window.setFramerateLimit(120);

    Pendulum pendulum(25.0f, 4.0f, PI / 4.0f);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            pendulum.spawnAtMouse(event, window);
        }

        // Update
        pendulum.applyForce();
        pendulum.update();

        // Draw
        window.clear(sf::Color::Black);
        pendulum.draw(window);

        window.display();

    }

    return 0;
}