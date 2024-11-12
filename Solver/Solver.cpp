#include <algorithm>
#include <list>
#include <random>
#include "Solver.hpp"
#include "Possibility.hpp"

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

bool Solver::findOnePossibilityInArea()
{
    bool somethingChanged = false;
    int counter[GameBoard::N];
    int lastPosition[GameBoard::N][2];
    
    // Rows
    for (int i = 0; i < GameBoard::N; i++)
    {
        // Reset counter
        for (int k = 0; k < GameBoard::N; k++)
            counter[k] = 0;
        // Check each cell
        for (int j = 0; j < GameBoard::N; j++)
            // Check for each number
            for (int k = 1; k <= GameBoard::N; k++)
                if (possibilitiesBoard->isPossible(i, j, k))
                {
                    counter[k - 1]++;
                    lastPosition[k - 1][0] = i;
                    lastPosition[k - 1][1] = j;
                }
        // Check which number appears only once in this row
        for (int k = 0; k < GameBoard::N; k++)
            if (counter[k] == 1)
            {
                // solutionBoard->printGrid();
                // possibilitiesBoard->printPossibilities();
                if (!this->fillAndFix(lastPosition[k][0], lastPosition[k][1], k + 1))
                    throw 11;
                somethingChanged = true;
            }
    }
    // Columns
    for (int i = 0; i < GameBoard::N; i++)
    {
        // Reset counter
        for (int k = 0; k < GameBoard::N; k++)
            counter[k] = 0;
        // Check each cell
        for (int j = 0; j < GameBoard::N; j++)
            // Check for each number
            for (int k = 1; k <= GameBoard::N; k++)
                if (possibilitiesBoard->isPossible(j, i, k))
                {
                    counter[k - 1]++;
                    lastPosition[k - 1][0] = j;
                    lastPosition[k - 1][1] = i;
                }
        // Check which number appears only once in this row
        for (int k = 0; k < GameBoard::N; k++)
            if (counter[k] == 1)
            {
                if (!this->fillAndFix(lastPosition[k][0], lastPosition[k][1], k + 1))
                    throw 11;
                somethingChanged = true;
            }
    }
    // Squares
    for (int i = 0; i < GameBoard::N; i++)
    {
        int squareI = (i / GameBoard::SIZE) * GameBoard::SIZE;
        int squareJ = (i % GameBoard::SIZE) * GameBoard::SIZE;
        int offsetI = 0;
        int offsetJ = 0;
        // Reset counter
        for (int k = 0; k < GameBoard::N; k++)
            counter[k] = 0;
        // Check each cell
        for (int j = 0; j < GameBoard::N; j++)
        {
            int currI = squareI + offsetI;
            int currJ = squareJ + offsetJ;
            // Check for each number
            for (int k = 1; k <= GameBoard::N; k++)
                if (possibilitiesBoard->isPossible(currJ, currI, k))
                {
                    counter[k - 1]++;
                    lastPosition[k - 1][0] = currJ;
                    lastPosition[k - 1][1] = currI;
                }
            offsetJ = (offsetJ + 1) % GameBoard::SIZE;
            if (offsetJ == 0)
                offsetI++;
            
        }
        // Check which number appears only once in this row
        for (int k = 0; k < GameBoard::N; k++)
            if (counter[k] == 1)
            {
                if (!this->fillAndFix(lastPosition[k][0], lastPosition[k][1], k + 1))
                    throw 11;
                somethingChanged = true;
            }
    }

    return somethingChanged;
}

Solver::Solver(GameBoard *solutionBoard)
{
    this->solutionBoard = new GameBoard(solutionBoard);
    this->possibilitiesBoard = new PossibilitiesBoard();
    unknowns = 0;
    this->checkForMultipleSolutions = false;
}

Solver::Solver(GameBoard *solutionBoard, bool checkForMultipleSolutions) : Solver(solutionBoard)
{
    this->checkForMultipleSolutions = checkForMultipleSolutions;
}

Solver::Solver(GameBoard *solutionBoard, PossibilitiesBoard *possibilitiesBoard, bool checkForMultpileSolutions) : Solver(solutionBoard, checkForMultpileSolutions)
{
    this->possibilitiesBoard = new PossibilitiesBoard(possibilitiesBoard);
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
    while (unknowns > 0)
    {
        // Use clever algorythms to eliminate possibilities
        bool somethingChanged = true;
        while (somethingChanged)
        {
            somethingChanged = false;
            try
            {
                somethingChanged = somethingChanged || this->findOnePossibilityInArea();
            }
            catch(const int e)
            {
                // printf("Error\n");
                return 0;
            }
        }
        // printf("Requires guessing\n");
        // solutionBoard->printGrid();
        // possibilitiesBoard->printPossibilities();

        // Find the field with the least possibilities
        int bestI = -1, bestJ = -1, leastPossibilities = GameBoard::N + 1;
        for (int i = 0; i < GameBoard::N; i++)
            for (int j = 0; j < GameBoard::N; j++)
                if (solutionBoard->getNumberAt(i, j) == 0)
                {
                    int numberOfPossibilities = possibilitiesBoard->getNumberOfPossibilitiesAt(i, j);
                    if (numberOfPossibilities == 0)
                        return 0;
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
            Solver* s = new Solver(solutionBoard, possibilitiesBoard, checkForMultipleSolutions);
            s->unknowns = this->unknowns;
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
            if (result == 1 && candidate == nullptr)
            {
                candidate = s->solutionBoard;
                s->solutionBoard = nullptr;
                candidatePossibilities = s->possibilitiesBoard;
                s->possibilitiesBoard = nullptr;
                delete s;

                if (!checkForMultipleSolutions)
                    break;
            }
            // If the solver returns without a solution
            else if (result == 0)
            {
                possibilitiesBoard->setImpossible(bestI, bestJ, fieldPossibilities[i]);
                delete s;
            }
            // If the solver gives one solution but we already had one or the solver gives multiple solutions
            else
            {
                delete s;
                return 2;
            } 
        }

        // If no candidate was found then this grid is unsolveable
        if (candidate == nullptr)
            return 0;

        // Set candidate as current solution
        GameBoard* temp = this->solutionBoard;
        this->solutionBoard = candidate;
        delete temp;
        PossibilitiesBoard* tempP = this->possibilitiesBoard;
        this->possibilitiesBoard = candidatePossibilities;
        delete tempP;
        unknowns = 0;
    }
    return 1;
}

void Solver::printPossibilities()
{
    possibilitiesBoard->printPossibilities();
}
