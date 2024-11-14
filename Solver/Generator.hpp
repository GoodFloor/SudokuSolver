#ifndef GENERATOR_H
#define GENERATOR_H

#include "GameBoard.hpp"

class Generator
{
private:
public:
    Generator();
    ~Generator();
    GameBoard* generateSolvedSudoku();
    GameBoard* generateSudoku(GameBoard* solvedBoard);
    GameBoard* generateSudoku();
};



#endif
