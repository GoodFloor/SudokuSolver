#include <iostream>
#include "Solver.hpp"
#include "Statistics.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    cout << "L.p.\tlevel" << endl;
    for (int i = 0; i < 1000; i++)
    {
        GameBoard* board = new GameBoard();
        // GameBoard* solvedBoard = nullptr;
        board->loadGrid();
        
        
        // Statistics::resetStatistics();
        // cout << "Grid loaded" << endl;
        // board->printGrid();
        Solver* solver = new Solver(board, false);
        // solver->printPossibilities();
        int r = solver->solve();
        r = solver->normalizeDifficulty(board, r);
        // solver->test();
        
        // solvedBoard = solver->getBoard();
        cout << i << "\t" << r << endl;
        // solvedBoard->printGrid();
        // cout << r << endl;
        // cout << 1. * (r + u) / u << endl;
        if (r < 0)
            board->printGrid();
        

        delete board;
        delete solver;
        // cout << "Max depth: " << Statistics::getMaxDepth() << endl;
        // cout << "r = " << r << endl;
        // cout << (i + 1) * 20 << "%" << endl;
    }
    
    
    return 0;
}
