#include <iostream>
#include "Solver.hpp"
#include "Statistics.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    // for (int i = 0; i < 3360; i++)
    // {
        GameBoard* board = new GameBoard();
        // GameBoard* solvedBoard = nullptr;
        board->loadGrid();
        // board->printGrid();
        Solver* solver = new Solver(board, true);
        // solver->printPossibilities();
        // int r = solver->solve();
        solver->test();
        
        // solvedBoard = solver->getBoard();
        // cout << 1 << " = " << r << endl;
        // solvedBoard->printGrid();
        // if (r >= 0)
            // board->printGrid();
        

        delete board;
        delete solver;
        // cout << "Max depth: " << Statistics::getMaxDepth() << endl;
        // cout << "r = " << r << endl;
    // }
    
    
    return 0;
}
