module;

#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <iostream>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

export module NavGraph;
import World;

export class Node {
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
    : position_(position), state_(state) {}

    bool operator==(const Node& other) const {
        return this->position_ == other.position_;
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
};

export struct NodeHash
{
    std::size_t operator()(const Node& node) const
    {
        return std::rotl(std::hash<int>{}(node.getPosition().x), 1) ^ std::hash<int>{}(node.getPosition().y);
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

    const std::unordered_set<Node, NodeHash>& getNodes() const {
        return nodes_;
    }

    sf::Vector2i getStartPosition() const {
        return startPosition_;
    }

    Node* getStartNode() const {
        return getNode(startPosition_);
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

    Node* getNode(const sf::Vector2i& position) const {
        Node tempNode(position, Node::State::Empty);

        auto itFound = nodes_.find(tempNode);

        return itFound != nodes_.end() ? const_cast<Node*>(&(*itFound)) : nullptr;
    }

    sf::Vector2i startPosition_;
    std::unordered_set<Node, NodeHash> nodes_;
};
