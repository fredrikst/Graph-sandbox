module;

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>

export module World:Tile;

export class Tile final {
public:
    enum class State {
        Empty,
        Wall,
        Door,
        Action,
        Start,
        Goal
    };

    Tile() {}

    Tile(unsigned int row, unsigned int col, float posX, float posY, float sizeX, float sizeY, State state)
    : matrixPos_(col, row)
    , currentState_(state) {

        rectangle_.setOutlineColor(sf::Color::Black);
        rectangle_.setOutlineThickness(-2.f);
        rectangle_.setOrigin({sizeX/2, sizeY/2});
        rectangle_.setSize({sizeX, sizeY});
        rectangle_.setPosition({posX, posY});

        if (state == State::Empty || state == State::Start || state == State::Goal) {
            rectangle_.setFillColor(sf::Color::White);
        }
        else if (state == State::Wall) {
            rectangle_.setFillColor(sf::Color::Green);
        }
        else if (state == State::Door) {
            rectangle_.setFillColor(sf::Color::Yellow);
        }
        else {
            rectangle_.setFillColor(sf::Color::Red);
        }
    }

    bool operator==(const Tile& other) const {
        return this->matrixPos_ == other.matrixPos_;
    }

    void draw(sf::RenderWindow& window) {

        window.draw(rectangle_);
    }

    void setState(State state) {
        currentState_ = state;

        if (state == State::Empty) {
            rectangle_.setFillColor(sf::Color::White);
        }
        else if (state == State::Wall) {
            rectangle_.setFillColor(sf::Color::Green);
        }
        else if (state == State::Door) {
            rectangle_.setFillColor(sf::Color::Yellow);
        }
        else {
            rectangle_.setFillColor(sf::Color::Red);
        }
    }

    Tile::State getState() const {
        return currentState_;
    }

    sf::Vector2i getPosition() const {
        return sf::Vector2i(rectangle_.getPosition());
    }

    sf::Vector2i getMatrixPosition() const {
        return matrixPos_;
    }

private:
    sf::RectangleShape rectangle_;

    sf::Vector2i matrixPos_;

    State currentState_;
};

export struct TileHash
{
    std::size_t operator()(const Tile& tile) const
    {
        return std::rotl(std::hash<int>{}(tile.getMatrixPosition().x), 1) ^ std::hash<int>{}(tile.getMatrixPosition().y);
    }
};
