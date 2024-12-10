#include <algorithm>
#include <list>
#include <random>
#include "Solver.hpp"
#include "Possibility.hpp"
#include "Statistics.hpp"

void Solver::findPossibilitiesAt(int x, int y)
{
    bool isAvailable[GameBoard::N];
    for (int i = 0; i < GameBoard::N; i++)
        isAvailable[i] = true;

    int squareX = x / GameBoard::SIZE * GameBoard::SIZE;
    int squareY = y / GameBoard::SIZE * GameBoard::SIZE;
    int offsetX = 0;
    int offsetY = 0;

    for (int i = 0; i < GameBoard::N; i++)
    {
        if (solutionBoard->getNumberAt(x, i) != 0)
            possibilitiesBoard->setImpossible(x, y, solutionBoard->getNumberAt(x, i));
        if (solutionBoard->getNumberAt(i, y) != 0)
            possibilitiesBoard->setImpossible(x, y, solutionBoard->getNumberAt(i, y));
        if (solutionBoard->getNumberAt(squareX + offsetX, squareY + offsetY) != 0)
            possibilitiesBoard->setImpossible(x, y, solutionBoard->getNumberAt(squareX + offsetX, squareY + offsetY));
        offsetX = (offsetX + 1) % GameBoard::SIZE;
        if (offsetX == 0)
            offsetY++;
    }
}

void Solver::findAllPossibilities()
{
    for (int i = 0; i < GameBoard::N; i++)
        for (int j = 0; j < GameBoard::N; j++)
            if (solutionBoard->getNumberAt(i, j) == 0)
                this->findPossibilitiesAt(i, j); 
}

bool Solver::fillAndFix(int x, int y, int z)
{
    if (solutionBoard->getNumberAt(x, y) == z)
        return true;
    if (solutionBoard->getNumberAt(x, y) != 0 || !possibilitiesBoard->isPossible(x, y, z))
        return false;

    solutionBoard->setNumberAt(x, y, z);
    possibilitiesBoard->clearPossibilitiesAt(x, y);
    unknowns--;
    
    int squareX = x / GameBoard::SIZE * GameBoard::SIZE;
    int squareY = y / GameBoard::SIZE * GameBoard::SIZE;
    int offsetX = 0;
    int offsetY = 0;
    std::list<Possibility> solvedCells;

    for (int i = 0; i < GameBoard::N; i++)
    {
        if (solutionBoard->getNumberAt(x, i) == 0)
        {
            possibilitiesBoard->setImpossible(x, i, z);
            int onlyPossibility = possibilitiesBoard->getOnlyPossibilityAt(x, i);
            if (onlyPossibility == 0)
                return false;
            if (onlyPossibility > 0)
            {
                Possibility newPossibility;
                newPossibility.i = x;
                newPossibility.j = i;
                newPossibility.p = onlyPossibility;
                solvedCells.push_back(newPossibility);
            }
        }
        if (solutionBoard->getNumberAt(i, y) == 0)
        {
            possibilitiesBoard->setImpossible(i, y, z);
            int onlyPossibility = possibilitiesBoard->getOnlyPossibilityAt(i, y);
            if (onlyPossibility == 0)
                return false;
            if (onlyPossibility > 0)
            {
                Possibility newPossibility;
                newPossibility.i = i;
                newPossibility.j = y;
                newPossibility.p = onlyPossibility;
                solvedCells.push_back(newPossibility);
            }
        }
        if (solutionBoard->getNumberAt(squareX + offsetX, squareY + offsetY) == 0)
        {
            possibilitiesBoard->setImpossible(squareX + offsetX, squareY + offsetY, z);
            int onlyPossibility = possibilitiesBoard->getOnlyPossibilityAt(squareX + offsetX, squareY + offsetY);
            if (onlyPossibility == 0)
                return false;
            if (onlyPossibility > 0)
            {
                Possibility newPossibility;
                newPossibility.i = squareX + offsetX;
                newPossibility.j = squareY + offsetY;
                newPossibility.p = onlyPossibility;
                solvedCells.push_back(newPossibility);
            }
        }
        offsetX = (offsetX + 1) % GameBoard::SIZE;
        if (offsetX == 0)
            offsetY++;
    }

    while (!solvedCells.empty())
    {
        Possibility nextPossibility = solvedCells.front();
        if (this->fillAndFix(nextPossibility.i, nextPossibility.j, nextPossibility.p) == false)
            return false;
        solvedCells.pop_front();
    }
    return true;
}

void Solver::findSolvedCells()
{
    for (int i = 0; i < GameBoard::N; i++)
        for (int j = 0; j < GameBoard::N; j++)
            if (possibilitiesBoard->getOnlyPossibilityAt(i, j) > 0)
                this->fillAndFix(i, j, possibilitiesBoard->getOnlyPossibilityAt(i, j));
}

int Solver::getSquareI(int rootI, int offset)
{
    int offsetI = offset / GameBoard::SIZE;
    return rootI + offsetI;
}

int Solver::getSquareJ(int rootJ, int offset)
{
    int offsetJ = offset % GameBoard::SIZE;
    return rootJ + offsetJ;
}

Solver::Solver(GameBoard *solutionBoard) : Solver(solutionBoard, false) {}

Solver::Solver(GameBoard *solutionBoard, bool checkForMultipleSolutions)
{
    this->checkForMultipleSolutions = checkForMultipleSolutions;
    this->solutionBoard = new GameBoard(solutionBoard);
    this->possibilitiesBoard = new PossibilitiesBoard();
    unknowns = 0;
    this->preprocess();
    for (int i = 0; i < GameBoard::N; i++)
        for (int j = 0; j < GameBoard::N; j++)
            for (int k = 0; k < 3; k++)
                usedAsNakedPair[i][j][k] = false;
}

Solver::Solver(Solver *solver)
{
    this->checkForMultipleSolutions = solver->checkForMultipleSolutions;
    this->unknowns = solver->unknowns;
    this->solutionBoard = new GameBoard(solver->solutionBoard);
    this->possibilitiesBoard = new PossibilitiesBoard(solver->possibilitiesBoard);
    for (int i = 0; i < GameBoard::N; i++)
        for (int j = 0; j < GameBoard::N; j++)
            for (int k = 0; k < 3; k++)
                usedAsNakedPair[i][j][k] = solver->usedAsNakedPair[i][j][k];
}

Solver::~Solver()
{
    if (solutionBoard != nullptr)
        delete solutionBoard;
    if (possibilitiesBoard != nullptr)
        delete possibilitiesBoard;
}

GameBoard *Solver::getBoard()
{
    return solutionBoard;
}

void Solver::unlinkBoard()
{
    this->solutionBoard = nullptr;
}

void Solver::preprocess()
{
    this->findAllPossibilities();
    for (int i = 0; i < GameBoard::N; i++)
        for (int j = 0; j < GameBoard::N; j++)
            if (solutionBoard->getNumberAt(i, j) == 0)
                unknowns++;
            else
                possibilitiesBoard->clearPossibilitiesAt(i, j);
    this->findSolvedCells();
}

int Solver::solve()
{
    Statistics::enterSolve();
    int hiddenSinglesFound = 0;
    int nakedPairsFound = 0;
    int difficultyRating = 0;
    while (unknowns > 0)
    {
        // Use clever algorythms to eliminate possibilities
        try
        {
            int r = this->findHiddenSingles();
            if (r > 0)
            {
                difficultyRating += r;
                hiddenSinglesFound += r;
                continue;
            }
            r = this->findNakedPairs();
            if (r > 0)
            {
                difficultyRating += r * 2;
                nakedPairsFound += r;
                continue;
            }
        }
        catch(const int e)
        {
            Statistics::exitSolve();
            return -1;
        }

        difficultyRating += 5;
        // Find the field with the least possibilities
        int bestI = -1, bestJ = -1, leastPossibilities = GameBoard::N + 1;
        for (int i = 0; i < GameBoard::N; i++)
            for (int j = 0; j < GameBoard::N; j++)
                if (solutionBoard->getNumberAt(i, j) == 0)
                {
                    int numberOfPossibilities = possibilitiesBoard->getNumberOfPossibilitiesAt(i, j);
                    if (numberOfPossibilities == 0)
                    {
                        Statistics::exitSolve();
                        return -1;
                    }
                    if (numberOfPossibilities < leastPossibilities)
                    {
                        leastPossibilities = numberOfPossibilities;
                        bestI = i;
                        bestJ = j;
                    } 
                }

        // Get all possibilities for this field and shuffle them
        int fieldPossibilities[leastPossibilities];
        int counter = 0;
        for (int i = 1; counter < leastPossibilities; i++)
            if (possibilitiesBoard->isPossible(bestI, bestJ, i))
            {
                fieldPossibilities[counter] = i;
                counter++;
            }
        std::random_device rd;
        std::mt19937 mt(rd());
        std::shuffle(&fieldPossibilities[0], &fieldPossibilities[leastPossibilities], mt);
        
        // Try out possibilities
        GameBoard* candidate = nullptr;
        PossibilitiesBoard* candidatePossibilities = nullptr;
        for (int i = 0; i < leastPossibilities; i++)
        {
            // Create copy of solver
            Solver* s = new Solver(this);
            // If inserting this possibility into the grid causes error
            if (!s->fillAndFix(bestI, bestJ, fieldPossibilities[i]))
            {
                possibilitiesBoard->setImpossible(bestI, bestJ, fieldPossibilities[i]);
                delete s;
                continue;
            }
            // Try solving this new grid
            int result = s->solve();
            // If there is only one solution and no previous attempts gave a solution            
            if (result >= 0 && candidate == nullptr)
            {
                difficultyRating += result;
                candidate = s->solutionBoard;
                s->solutionBoard = nullptr;
                candidatePossibilities = s->possibilitiesBoard;
                s->possibilitiesBoard = nullptr;
                delete s;

                if (!checkForMultipleSolutions)
                    break;
            }
            // If the solver returns without a solution
            else if (result == -1)
            {
                possibilitiesBoard->setImpossible(bestI, bestJ, fieldPossibilities[i]);
                delete s;
            }
            // If the solver gives one solution but we already had one or the solver gives multiple solutions
            else
            {
                delete s;
                if (candidate != nullptr)
                {
                    delete candidate;
                    delete candidatePossibilities;
                }
                
                Statistics::exitSolve();
                return -2;
            } 
        }

        // If no candidate was found then this grid is unsolveable
        if (candidate == nullptr)
        {
            Statistics::exitSolve();
            return -1;
        }

        // Set candidate as current solution
        GameBoard* temp = this->solutionBoard;
        this->solutionBoard = candidate;
        delete temp;
        PossibilitiesBoard* tempP = this->possibilitiesBoard;
        this->possibilitiesBoard = candidatePossibilities;
        delete tempP;
        unknowns = 0;
    }
    Statistics::successfullSolve(hiddenSinglesFound, nakedPairsFound);
    Statistics::exitSolve();
    return difficultyRating;
}

int Solver::solveWithoutGuessing()
{
    int difficultyRating = 0;
    bool somethingChanged = true;
    while (unknowns > 0 && somethingChanged)
    {
        somethingChanged = false;
            try
            {
                if (this->findHiddenSingles())
                {
                    somethingChanged = true;
                    difficultyRating += 1;
                    continue;
                }
                if (this->findNakedPairs())
                {
                    somethingChanged = true;
                    difficultyRating += 2;
                    continue;
                }
            }
            catch(const int e)
            {
                Statistics::exitSolve();
                return -1;
            }
    }
    if (unknowns > 0)
        return -1;
    return difficultyRating;
}

int Solver::normalizeDifficulty(int difficulty)
{
    return difficulty / GameBoard::N; 
}
