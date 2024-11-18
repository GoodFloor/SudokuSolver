#include <algorithm>
#include <random>
#include "Generator.hpp"
#include "Solver.hpp"
#include "Possibility.hpp"

Generator::Generator()
{
}

Generator::~Generator()
{
}

GameBoard *Generator::generateSolvedSudoku()
{
    int firstRow[GameBoard::N];
    for (int i = 0; i < GameBoard::N; i++)
        firstRow[i] = i + 1;
    std::random_device rd;
    std::mt19937 mt(rd());
    std::shuffle(&firstRow[0], &firstRow[GameBoard::N], mt);
    GameBoard* newGrid = new GameBoard();
    for (int i = 0; i < GameBoard::N; i++)
        newGrid->setNumberAt(0, i, firstRow[i]);
    Solver* gridFiller = new Solver(newGrid, false);
    delete newGrid;
    gridFiller->preprocess();
    gridFiller->solve();
    newGrid = gridFiller->exportBoard();
    delete gridFiller;
    return newGrid;
}

GameBoard *Generator::generateSudoku()
{
    return generateSudoku(this->generateSolvedSudoku());
}

GameBoard *Generator::generateSudoku(GameBoard *solvedBoard)
{
    return generateSudoku(solvedBoard, INT32_MAX);
}

GameBoard *Generator::generateSudoku(int targetDifficultyRating)
{
    return generateSudoku(this->generateSolvedSudoku(), targetDifficultyRating);
}

GameBoard *Generator::generateSudoku(GameBoard *solvedBoard, int targetDifficultyRating)
{
    Possibility fieldsList[GameBoard::N * GameBoard::N];
    for (int i = 0; i < GameBoard::N; i++)
        for (int j = 0; j < GameBoard::N; j++)
        {
            Possibility curr;
            curr.i = i;
            curr.j = j;
            curr.p = solvedBoard->getNumberAt(i, j);
            fieldsList[i * GameBoard::N + j] = curr;
        }
    std::random_device rd;
    std::mt19937 mt(rd());
    std::shuffle(&fieldsList[0], &fieldsList[GameBoard::N * GameBoard::N], mt);
    Solver* solver;
    int currentDifficultyRating = 0;
    for (int i = 0; i < GameBoard::N * GameBoard::N && currentDifficultyRating < targetDifficultyRating; i++)
    {
        solver = new Solver(solvedBoard, true);
        solver->getBoard()->setNumberAt(fieldsList[i].i, fieldsList[i].j, 0);
        solver->preprocess();
        int result = solver->solve();
        
        if (result >= 0 && result <= targetDifficultyRating)
        {
            currentDifficultyRating = result;
            solvedBoard->setNumberAt(0, 0, 0);
            solvedBoard->setNumberAt(fieldsList[i].i, fieldsList[i].j, 0);
        }
        delete solver;
    }
    return solvedBoard;
}

