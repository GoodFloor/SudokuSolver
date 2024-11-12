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
    void findSolvedCells();
public:
    Solver(GameBoard* solutionBoard);
    Solver(GameBoard* solutionBoard, bool checkForMultpileSolutions);
    Solver(GameBoard* solutionBoard, PossibilitiesBoard* possibilitiesBoard, bool checkForMultpileSolutions);
    ~Solver();
    GameBoard* getBoard();
    void preprocess();
    int solve();


    void printPossibilities();
};

#endif
