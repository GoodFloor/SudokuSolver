#ifndef POSSIBILITIES_BOARD_H
#define POSSIBILITIES_BOARD_H

#include "GameBoard.hpp"

class PossibilitiesBoard
{
private:
    bool grid[GameBoard::N][GameBoard::N][GameBoard::N];
public:
    PossibilitiesBoard();
    PossibilitiesBoard(PossibilitiesBoard* p);
    ~PossibilitiesBoard();
    bool isPossible(int x, int y, int z);
    void setPossible(int x, int y, int z);
    void setImpossible(int x, int y, int z);
    int getFirstPossibility(int x, int y);
    // @return 0 if no possibilities at (x, y) OR -1 if multiple possibilities at (x, y) OR number representing only possibility at (x, y)
    int getOnlyPossibilityAt(int x, int y);
    void printPossibilities();
    void clearPossibilitiesAt(int x, int y);
    int getNumberOfPossibilitiesAt(int x, int y);
};

#endif
