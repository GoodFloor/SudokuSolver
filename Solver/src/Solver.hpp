#ifndef SOLVER_H
#define SOLVER_H

#include "GameBoard.hpp"
#include "PossibilitiesBoard.hpp"

class Solver
{
private:
    bool checkForMultipleSolutions;
    GameBoard* solutionBoard;
    PossibilitiesBoard* possibilitiesBoard;
    int unknowns;
    bool usedAsNakedPair[GameBoard::N][GameBoard::N][3];
    void findPossibilitiesAt(int x, int y);
    void findAllPossibilities();
    bool fillAndFix(int x, int y, int z);
    void findSolvedCells(); // Also known as Naked Singles
    int findHiddenSingles();
    int findNakedPairs();
    int getSquareI(int rootI, int offset);
    int getSquareJ(int rootJ, int offset);
public:
    Solver(GameBoard* solutionBoard);
    Solver(GameBoard* solutionBoard, bool checkForMultpileSolutions);
    Solver(Solver* solver);
    ~Solver();
    GameBoard* getBoard();
    // Unlinks board so it doesn't get deleted with Solver
    void unlinkBoard();
    void preprocess();
    // @return -1 if no solutions, -2 if multiple solution, board difficulty rating otherwise
    int solve();
    int solveWithoutGuessing();
    int normalizeDifficulty(int difficulty);
};

#endif
