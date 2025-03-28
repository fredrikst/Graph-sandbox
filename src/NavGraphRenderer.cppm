module;

#include <unordered_set>
#include <array>
#include <queue>
#include <mutex>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

export module NavGraphRenderer;
export import NavGraph;

export class NavGraphRenderer {
public:
    NavGraphRenderer(const NavGraph& navGraph)
    : nodes_(navGraph.getNodes()) {
        startPosition_ = navGraph.getStartPosition();
    }

    void draw(sf::RenderWindow& window) {
        Node tempNode(startPosition_, Node::State::Empty);
        auto itFound = nodes_.find(tempNode);

        std::queue<Node*> nodesToVisit;
        std::unordered_set<const Node*> encounteredNodes;
        auto startNode = const_cast<Node*>(&(*itFound));
        nodesToVisit.push(startNode);
        encounteredNodes.insert(startNode);

        while (!nodesToVisit.empty()) {
            auto currentNode = nodesToVisit.front();
            nodesToVisit.pop();

            auto neighbors = currentNode->getNeighbors();

            for (const auto& neighbor : neighbors) {
                auto itFound = encounteredNodes.find(neighbor.first);

                if (itFound == encounteredNodes.end()) {
                    nodesToVisit.push(neighbor.first);
                    encounteredNodes.insert(neighbor.first);
                }

                std::array line = 
                {
                    sf::Vertex{sf::Vector2f(currentNode->getPosition()), sf::Color(sf::Color::Black)},
                    sf::Vertex{sf::Vector2f(neighbor.first->getPosition()), sf::Color(sf::Color::Black)}
                };

                window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
            }
        }

        for (const Node& node : nodes_) {
            auto state = node.getState();
            auto position = node.getPosition();

            sf::Color nodeColor = sf::Color::Black;

            {
                std::scoped_lock lock(vistedNodesMutex_);

                auto itVisited = visitedNodes_.find(node);

                if (itVisited != visitedNodes_.end()) {
                    nodeColor = sf::Color::Red;
                }
            }

            if (state == Node::State::Start || state == Node::State::Goal) {

                auto varray = sf::VertexArray(sf::PrimitiveType::Triangles, 3);
                varray[0].position = sf::Vector2f(position.x-10, position.y+10);
                varray[0].color = nodeColor;
                varray[1].position = sf::Vector2f(position.x, position.y-10);
                varray[1].color = nodeColor;
                varray[2].position = sf::Vector2f(position.x+10, position.y+10);
                varray[2].color = nodeColor;
                startOrGoal_ = varray;
                window.draw(startOrGoal_);
            }
            else {
                nodeShape_.setOrigin(sf::Vector2f(4, 4));
                nodeShape_.setPosition(sf::Vector2f(position));
                nodeShape_.setFillColor(nodeColor);
                nodeShape_.setRadius(4);
                window.draw(nodeShape_);
            }
        }
    }

    void setVisitedNode(Node* node) {
        std::scoped_lock lock(vistedNodesMutex_);
        visitedNodes_.insert(*node);
    }

private:
    const std::unordered_set<Node, NodeHash>& nodes_;
    std::unordered_set<Node, NodeHash> visitedNodes_;
    sf::Vector2i startPosition_;

    sf::VertexArray startOrGoal_;
    sf::CircleShape nodeShape_;

    std::mutex vistedNodesMutex_;
};
