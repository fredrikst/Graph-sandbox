#include <SFML/Graphics.hpp>

import World;

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({800, 600}), "Graph sandbox");
    window.setFramerateLimit(144);

    World world(10, 14);

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {

            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();
        world.draw(window);
        window.display();
    }
}