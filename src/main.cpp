#include <SFML/Graphics.hpp>

#include <future>
#include <iostream>

import World;
import NavGraphRenderer;
import BreadthFirst;

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({800, 600}), "Graph sandbox");
    window.setFramerateLimit(144);

    World world({0, 0}, {1, 8}, 10, 14);
    NavGraph navGraph;
    navGraph.create(world);
    NavGraphRenderer navGraphRenderer(navGraph);

    BreadthFirst breadthFirst(navGraph, navGraphRenderer);

    auto searchThread = std::async(std::launch::async, &BreadthFirst::search, breadthFirst, 
    [](Node* node) -> bool
    {
        return node->getState() == Node::State::Goal;
    });

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
        navGraphRenderer.draw(window);
        window.display();
    }

    searchThread.wait();
}
