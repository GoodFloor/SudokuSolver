#include <iostream>
#include "Solver.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    GameBoard* board = new GameBoard();
    GameBoard* solvedBoard = nullptr;
    board->loadGrid();
    board->printGrid();
    Solver* solver = new Solver(board);
    solver->preprocess();
    solver->printPossibilities();
    int r = solver->solve();
    
    solvedBoard = solver->getBoard();
    solvedBoard->printGrid();

    delete board;
    delete solver;
    return 0;
}
