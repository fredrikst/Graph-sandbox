module;

#include <SFML/Graphics.hpp>

export module World:Square;

export class Square final {
public:
    enum class State {
        Empty,
        Wall,
        Door,
        Action,
        Start,
        Goal
    };

    Square() {}

    Square(unsigned int row, unsigned int col, float posX, float posY, float sizeX, float sizeY, State state)
    : idRow_(row)
    , idCol_(col)
    , currentState_(state) {

        rectangle_.setOutlineColor(sf::Color::Black);
        rectangle_.setOutlineThickness(-5.f);
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

        if (state == State::Start || state == State::Goal) {
            auto varray = sf::VertexArray(sf::PrimitiveType::Triangles, 3);
            varray[0].position = {posX-10, posY+10};
            varray[0].color = sf::Color::Black;
            varray[1].position = {posX, posY-10};
            varray[1].color = sf::Color::Black;
            varray[2].position = {posX+10, posY+10};
            varray[2].color = sf::Color::Black;
            startOrGoal_ = varray;
        }
    }

    bool operator==(Square& other) {
        return this->idRow_ == other.idRow_ && this->idCol_ == other.idCol_;
    }

    void draw(sf::RenderWindow& window) {

        window.draw(rectangle_);

        if (startOrGoal_.has_value()) {
            window.draw(startOrGoal_.value());
        }
    }

    void setID(int row, int col) {
        idRow_ = row;
        idCol_ = col;
    }

    void setSize(float x, float y) {
        rectangle_.setSize({x, y});
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

private:
    sf::RectangleShape rectangle_;
    std::optional<sf::VertexArray> startOrGoal_;

    unsigned int idRow_, idCol_;

    State currentState_;
};