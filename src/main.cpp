#include <SFML/Graphics.hpp>

import World;
import NavGraph;

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({800, 600}), "Graph sandbox");
    window.setFramerateLimit(144);

    World world({0, 0}, {1, 8}, 10, 14);
    NavGraph navGraph;
    navGraph.create(world);

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
        navGraph.draw(window);
        window.display();
    }
}