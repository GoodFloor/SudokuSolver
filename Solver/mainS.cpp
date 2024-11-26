#include <iostream>
#include "Solver.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    for (int i = 0; i < 1000; i++)
    {
        GameBoard* board = new GameBoard();
        GameBoard* solvedBoard = nullptr;
        board->loadGrid();
        // board->printGrid();
        Solver* solver = new Solver(board, false);
        // solver->printPossibilities();
        int r = solver->solve();
        
        // solvedBoard = solver->getBoard();
        // cout << i << " = " << r << endl;
        // solvedBoard->printGrid();

        delete board;
        delete solver;
    }
    
    
    return 0;
}
