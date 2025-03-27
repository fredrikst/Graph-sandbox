module;

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <list>
#include <iostream>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

export module NavGraph;
import World;

class Node {
public:
    enum class State {
        Empty,
        Wall,
        Door,
        Action,
        Start,
        Goal
    };

    Node(const sf::Vector2i& position, State state) 
    : position_(position), state_(state) {
        if (state == State::Start || state == State::Goal) {
            auto varray = sf::VertexArray(sf::PrimitiveType::Triangles, 3);
            varray[0].position = sf::Vector2f(position.x-10, position.y+10);
            varray[0].color = sf::Color::Black;
            varray[1].position = sf::Vector2f(position.x, position.y-10);
            varray[1].color = sf::Color::Black;
            varray[2].position = sf::Vector2f(position.x+10, position.y+10);
            varray[2].color = sf::Color::Black;
            startOrGoal_ = varray;
        }
        else {
            nodeShape_.setOrigin(sf::Vector2f(4, 4));
            nodeShape_.setPosition(sf::Vector2f(position));
            nodeShape_.setFillColor(sf::Color(0, 0 , 0));
            nodeShape_.setRadius(4);
        }
    }

    bool operator==(const Node& other) const {
        return this->position_ == other.position_;
    }

    void draw(sf::RenderWindow& window) const {
        if (state_ == Node::State::Start || state_ == Node::State::Goal) {
            window.draw(startOrGoal_.value());
        }
        else {
            window.draw(nodeShape_);
        }
    }

    sf::Vector2i getPosition() const {
        return position_;
    }

    Node::State getState() const {
        return state_;
    }

    std::unordered_map<Node*, double> getNeighbors() const {
        return neigbors_;
    }

    void addNeighbor(Node* neighbor) {
        neigbors_.emplace(neighbor, 0.0);
    }

private:
    sf::Vector2i position_;

    State state_;
    std::unordered_map<Node*, double> neigbors_;

    std::optional<sf::VertexArray> startOrGoal_;
    sf::CircleShape nodeShape_;
};

struct NodeHash
{
    std::size_t operator()(const Node& node) const
    {
        return (53 + std::hash<int>{}(node.getPosition().x)) * 53 + std::hash<int>{}(node.getPosition().y);
    }
};

export class NavGraph {
public:
    void create(const World& world) {
        
        std::unordered_set<Tile, TileHash> encounteredTiles;
        std::queue<Tile> tilesToVisit;

        auto currentTile = world.getStart();
        startPosition_ = currentTile.getPosition();
        tilesToVisit.emplace(currentTile);

        encounteredTiles.insert(currentTile);
        createNodeIfNotExists(currentTile);

        while (!tilesToVisit.empty()) {
            currentTile = tilesToVisit.front();
            tilesToVisit.pop();

            Node* currentNode = getNode(currentTile.getPosition());


            auto neighborTiles = world.getNeighbors(currentTile);

            for (const auto& neighborTile : neighborTiles) {
                bool isTileEncountered = encounteredTiles.contains(neighborTile.get());

                if (!isTileEncountered) {

                    tilesToVisit.push(neighborTile.get());
                    encounteredTiles.insert(neighborTile.get());
                }

                Node* neighborNode = createNodeIfNotExists(neighborTile.get());
                currentNode->addNeighbor(neighborNode);
            }
        }
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
            node.draw(window);
        }
    }

private:
    Node* createNodeIfNotExists(const Tile& tile) {
        
        Tile::State tileState = tile.getState();
        Node::State nodeState;
        
        switch (tileState) {
            case Tile::State::Empty:
                nodeState = Node::State::Empty;
                break;
            case Tile::State::Wall:
                nodeState = Node::State::Wall;
                break;
            case Tile::State::Door:
                nodeState = Node::State::Door;
                break;
            case Tile::State::Action:
                nodeState = Node::State::Action;
                break;
            case Tile::State::Start:
                nodeState = Node::State::Start;
                break;
            case Tile::State::Goal:
                nodeState = Node::State::Goal;
                break;
            default:
        }


        Node tempNode(tile.getPosition(), nodeState);

        auto insertedNode = nodes_.insert(tempNode);

        return const_cast<Node*>(&(*insertedNode.first));
    }

    Node* getNode(const sf::Vector2i& position) {
        Node tempNode(position, Node::State::Empty);

        auto itFound = nodes_.find(tempNode);

        return itFound != nodes_.end() ? const_cast<Node*>(&(*itFound)) : nullptr;
    }


    sf::Vector2i startPosition_;
    std::unordered_set<Node, NodeHash> nodes_;
};