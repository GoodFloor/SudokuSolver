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
public:
    Solver(GameBoard* solutionBoard);
    Solver(GameBoard* solutionBoard, bool checkForMultpileSolutions);
    Solver(GameBoard* solutionBoard, PossibilitiesBoard* possibilitiesBoard, bool checkForMultpileSolutions);
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
