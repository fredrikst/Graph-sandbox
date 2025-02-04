module;

#include <vector>

#include <SFML/Graphics.hpp>

export module World;
export import :Square;

export class World final {
public:
    World(unsigned int nRows, unsigned int nCols) 
    : nRows_(nRows)
    , nCols_(nCols) {

        int sizeX = 800 / nCols;
        int sizeY = 600 / nRows;

        for (int iRow=0; iRow < nRows_; ++iRow) {

            std::vector<Square> col;
            for (int iCol=0; iCol < nCols_; ++iCol) {

                Square::State state;
                if (iRow == 6 && iCol != 4 && iCol != 12) {
                    state = Square::State::Wall;
                }
                else if (iCol == 6 && iRow < 6) {
                    state = Square::State::Wall;
                }
                else if (iRow == 0 && iCol == 0) {
                    state = Square::State::Start;
                }
                else if (iRow == 1 && iCol == 8) {
                    state = Square::State::Goal;
                }
                else {
                    state = Square::State::Empty;
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

private:
    std::vector<std::vector<Square>> world_;
    unsigned int nRows_, nCols_;
};