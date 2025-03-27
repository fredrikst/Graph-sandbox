module;

#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

export module World;
export import :Tile;

export class World final {
public:
    World(sf::Vector2i start, sf::Vector2i goal, unsigned int nRows, unsigned int nCols) 
    : nRows_(nRows)
    , nCols_(nCols)
    , start_(start)
    , goal_(goal) {

        int sizeX = 800 / nCols;
        int sizeY = 600 / nRows;

        for (int iRow=0; iRow < nRows_; ++iRow) {

            std::vector<Tile> col;
            for (int iCol=0; iCol < nCols_; ++iCol) {

                Tile::State state;
                if (iRow == 6 && iCol != 4 && iCol != 12) {
                    state = Tile::State::Wall;
                }
                else if (iCol == 6 && iRow < 6) {
                    state = Tile::State::Wall;
                }
                else if (start == sf::Vector2i{iRow, iCol}) {
                    state = Tile::State::Start;
                }
                else if (goal == sf::Vector2i{iRow, iCol}) {
                    state = Tile::State::Goal;
                }
                else {
                    state = Tile::State::Empty;
                }

                col.emplace_back(iRow, iCol, sizeX*iCol+(sizeX/2), sizeY*iRow+(sizeY/2), sizeX, sizeY, state);
            }
            world_.push_back(col);
        }
    }

    void draw(sf::RenderWindow& window) {
        for (int iRow=0; iRow < nRows_; ++iRow) {
            for (int iCol=0; iCol < nCols_; ++iCol) {
                world_[iRow][iCol].draw(window);
            }
        }
    }

    std::vector<std::reference_wrapper<const Tile>> getNeighbors(const Tile& tile) const {
        std::vector<std::reference_wrapper<const Tile>> neighbors;

        sf::Vector2i matrixPosition = tile.getMatrixPosition();

        if (matrixPosition.x - 1 >= 0) {
            auto state = world_[matrixPosition.y][matrixPosition.x - 1].getState();
            if (state != Tile::State::Wall) {
                neighbors.push_back(std::ref(world_[matrixPosition.y][matrixPosition.x - 1]));
            }
        }
        if (matrixPosition.x + 1 < nCols_) {
            auto state = world_[matrixPosition.y][matrixPosition.x + 1].getState();
            if (state != Tile::State::Wall) {
                neighbors.push_back(std::ref(world_[matrixPosition.y][matrixPosition.x + 1]));
            }
        }
        if (matrixPosition.y - 1 >= 0) {
            auto state = world_[matrixPosition.y - 1][matrixPosition.x].getState();
            if (state != Tile::State::Wall) {
                neighbors.push_back(std::ref(world_[matrixPosition.y - 1][matrixPosition.x]));
            }
        }
        if (matrixPosition.y + 1 < nRows_) {
            auto state = world_[matrixPosition.y + 1][matrixPosition.x].getState();
            if (state != Tile::State::Wall) {
                neighbors.push_back(std::ref(world_[matrixPosition.y + 1][matrixPosition.x]));
            }
        }

        return neighbors; 
    }

    const Tile& getStart() const {
        return world_[start_.y][start_.x];
    }

    unsigned int getNrRows() {
        return nRows_;
    }

    unsigned int getNrCols() {
        return nCols_;
    }

    const Tile& getTile(const sf::Vector2i& matrixPosition) const {
        return world_[matrixPosition.y][matrixPosition.x];
    }

private:
    std::vector<std::vector<Tile>> world_;
    unsigned int nRows_, nCols_;
    sf::Vector2i start_, goal_;
};