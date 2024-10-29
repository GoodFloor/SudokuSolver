#include <iostream>
#include "GameBoard.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    GameBoard* board = new GameBoard;
    board->generateNew();
    board->exportGrid();
    board->removeRandom();
    board->exportGrid();
    board->printGrid();
    delete board;
    return 0;
}
