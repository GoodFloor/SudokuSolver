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
    GameBoard* generateSudoku();
    GameBoard* generateSudoku(GameBoard* solvedBoard);
    GameBoard* generateSudoku(int targetDifficultyRating);
    GameBoard* generateSudoku(GameBoard* solvedBoard, int targetDifficultyRating);
};



#endif
