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
    GameBoard* generateSudoku(int targetDifficultyRating);
    GameBoard* generateSudoku(GameBoard* solvedBoard, int targetDifficultyRating);
    GameBoard* generateSudokuWithoutGuessing();
    GameBoard* generateSudokuWithoutGuessing(int targetDifficultyRating);
    GameBoard* generateSudokuWithoutGuessing(GameBoard* solvedBoard, int targetDifficultyRating);
};



#endif
