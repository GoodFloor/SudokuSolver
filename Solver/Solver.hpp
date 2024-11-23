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
    void findPossibilitiesAt(int x, int y);
    void findAllPossibilities();
    bool fillAndFix(int x, int y, int z);
    void findSolvedCells(); // Also known as Naked Singles
    bool findHiddenSingles();
    bool findNakedPairs();
public:
    int getSquareI(int rootI, int offset);
    int getSquareJ(int rootJ, int offset);
    Solver(GameBoard* solutionBoard);
    Solver(GameBoard* solutionBoard, bool checkForMultpileSolutions);
    Solver(Solver* solver);
    ~Solver();
    GameBoard* getBoard();
    // Returns board and removes link to it
    GameBoard* exportBoard();
    void preprocess();
    // @return -1 if no solutions, -2 if multiple solution, board difficulty rating otherwise
    int solve();


    void printPossibilities();
};

#endif
