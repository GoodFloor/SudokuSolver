#include <iostream>
#include "Solver.hpp"
#include "Generator.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    for (int i = 0; i < 10000; i++)
    {
        Generator* generator = new Generator();
        GameBoard* board = generator->generateSudoku();
        delete generator;
        Solver* solver = new Solver(board, false);
        solver->preprocess();
        int r = solver->solve();
        cout << r << "\t";
        board->printGrid();
        delete solver;
        delete board;
    }
    return 0;
}

 