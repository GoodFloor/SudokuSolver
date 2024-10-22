#include <iostream>
#include "GameBoard.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    GameBoard* board = new GameBoard;
    board->loadGrid();
    board->fillAllPossibilities();
    board->printGrid();
    board->solveObvious();
    cout << board->solve() << endl;
    delete board;
    return 0;
}
