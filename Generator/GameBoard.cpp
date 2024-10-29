#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include "GameBoard.hpp"
#include "Possibility.hpp"
#include <random>

GameBoard::GameBoard()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            grid[i][j] = 0;
            for (int k = 0; k < 9; k++)
            {
                possibilities[i][j][k] = false;
            }
        }
    }
    unknowns = 0;
}

GameBoard::~GameBoard()
{
}

GameBoard::GameBoard(GameBoard *board)
{
    this->unknowns = board->unknowns;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            this->grid[i][j] = board->grid[i][j];
            for (int k = 0; k < 9; k++)
            {
                this->possibilities[i][j][k] = board->possibilities[i][j][k];
            }
        }
    }
}

void GameBoard::loadGrid()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            int content;
            std::cin >> content;
            grid[i][j] = content;
            if (content == 0)
                unknowns++;
        }
    }
}

void GameBoard::printGrid()
{
    for (int i = 0; i < 37; i++)
    {
        for (int j = 0; j < 37; j++)
        {
            // pogrubione linie
            if (i % 12 == 0 || j % 12 == 0)
                std::cout << "█";
            // skrzyżowania
            else if (i % 4 == 0 && j % 4 == 0) 
                std::cout << "┼";
            // pionowe linie
            else if (j % 4 == 0)    
                std::cout << "│";
            // poziome linie
            else if (i % 4 == 0)    
                std::cout << "─";
            // numery
            else if (i % 2 == 0 && j % 2 == 0 && getNumberAt(i / 4, j / 4) != 0)    
                std::cout << getNumberAt(i / 4, j / 4);
            // możliwe numery
            else if (getNumberAt(i / 4, j / 4) == 0 && isPossibleAt(i / 4, j / 4, ((i % 4) - 1) * 3 + (j % 4)))     
                std::cout << possibilitiesChar[((i % 4) - 1) * 3 + (j % 4) - 1];
            else 
                std::cout << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void GameBoard::exportGrid()
{
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            std::cout << grid[i][j];
    std::cout << std::endl;
}

int GameBoard::getNumberAt(int x, int y)
{
    return grid[x][y];
}

void GameBoard::setNumberAt(int x, int y, int n)
{
    grid[x][y] = n;
    for (int i = 0; i < 9; i++)
    {
        possibilities[x][y][i] = false;
    }
}

bool GameBoard::isPossibleAt(int x, int y, int n)
{
    return possibilities[x][y][n - 1];
}

void GameBoard::fillAllPossibilities()
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (getNumberAt(i, j) == 0)
                redoPossibilitiesAt(i, j);
}

void GameBoard::redoPossibilitiesAt(int x, int y)
{
    bool isAvailable[9];
    for (int i = 0; i < 9; i++)
        isAvailable[i] = true;

    int squareX = x / 3 * 3;
    int squareY = y / 3 * 3;
    for (int i = 0; i < 9; i++)
    {
        if (getNumberAt(x, i) != 0)
            isAvailable[getNumberAt(x, i) - 1] = false;
        if (getNumberAt(i, y) != 0)
            isAvailable[getNumberAt(i, y) - 1] = false;
        if (getNumberAt(squareX + (i / 3), squareY + (i % 3)) != 0)
            isAvailable[getNumberAt(squareX + (i / 3), squareY + (i % 3)) - 1] = false;    
    }
    
    for (int i = 0; i < 9; i++)
        possibilities[x][y][i] = isAvailable[i];
}

void GameBoard::solveObvious()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            int numberOfPossibilities = 0;
            int lastPossibility;
            for (int k = 0; k < 9 && numberOfPossibilities < 2; k++)
                if (possibilities[i][j][k])
                {
                    numberOfPossibilities++;
                    lastPossibility = k + 1;
                }
            if (numberOfPossibilities == 1)
                fillAndFix(i, j, lastPossibility);
        }
    }
}

bool GameBoard::fillAndFix(int x, int y, int n)
{
    if (getNumberAt(x, y) == n)
        return true;
    else if (getNumberAt(x, y) != 0 || possibilities[x][y][n - 1] == false)
        return false;
    setNumberAt(x, y, n);
    unknowns--;

    int squareX = x / 3 * 3;
    int squareY = y / 3 * 3;
    std::list<Possibility> onePossibilityList;
    for (int i = 0; i < 9; i++)
    {
        if (getNumberAt(x, i) == 0)
        {
            possibilities[x][i][n - 1] = false;
            int numberOfPossibilities = 0;
            int lastPossibility;
            for (int k = 0; k < 9 && numberOfPossibilities < 2; k++)
                if (possibilities[x][i][k])
                {
                    numberOfPossibilities++;
                    lastPossibility = k + 1;
                }
            if (numberOfPossibilities == 1)
            {
                Possibility newPossibility;
                newPossibility.i = x;
                newPossibility.j = i;
                newPossibility.p = lastPossibility;
                onePossibilityList.push_back(newPossibility);
            }
        }
        
        if (getNumberAt(i, y) == 0)
        {
            possibilities[i][y][n - 1] = false;
            int numberOfPossibilities = 0;
            int lastPossibility;
            for (int k = 0; k < 9 && numberOfPossibilities < 2; k++)
                if (possibilities[i][y][k])
                {
                    numberOfPossibilities++;
                    lastPossibility = k + 1;
                }
            if (numberOfPossibilities == 1)
            {
                Possibility newPossibility;
                newPossibility.i = i;
                newPossibility.j = y;
                newPossibility.p = lastPossibility;
                onePossibilityList.push_back(newPossibility);
            }
        }
        
        if (getNumberAt(squareX + (i / 3), squareY + (i % 3)) == 0)
        {
            possibilities[squareX + (i / 3)][squareY + (i % 3)][n - 1] = false;
            int numberOfPossibilities = 0;
            int lastPossibility;
            for (int k = 0; k < 9 && numberOfPossibilities < 2; k++)
                if (possibilities[squareX + (i / 3)][squareY + (i % 3)][k])
                {
                    numberOfPossibilities++;
                    lastPossibility = k + 1;
                }
            if (numberOfPossibilities == 1)
            {
                Possibility newPossibility;
                newPossibility.i = squareX + (i / 3);
                newPossibility.j = squareY + (i % 3);
                newPossibility.p = lastPossibility;
                onePossibilityList.push_back(newPossibility);
            }
        }
    }
    while (!onePossibilityList.empty())
    {
        Possibility nextPossibility = onePossibilityList.front();
        if (fillAndFix(nextPossibility.i, nextPossibility.j, nextPossibility.p) == false)
            return false;
        onePossibilityList.pop_front();
    }
    return true;
}

int GameBoard::solve()
{
    while (unknowns > 0)
    {
        int bestI = -1, bestJ = -1, bestBet = 10;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if (grid[i][j] == 0)
                {
                    int numberOfPossibilities = 0;
                    for (int k = 0; k < 9; k++)
                        if (possibilities[i][j][k])
                            numberOfPossibilities++;
                    if (numberOfPossibilities == 0)
                        return 0;
                    
                    if (numberOfPossibilities < bestBet)
                    {
                        bestBet = numberOfPossibilities;
                        bestI = i;
                        bestJ = j;
                    }
                }
            }
        }
        if (bestI == -1)
            return 1;
        
        GameBoard* solution = nullptr;
        int numberOfSolutions = 0;
        for (int k = 0; k < 9; k++)
        {
            if (possibilities[bestI][bestJ][k])
            {
                GameBoard* p = new GameBoard(this);
                bool isValid = p->fillAndFix(bestI, bestJ, k + 1);
                if (isValid == false)
                    continue;
                int possibleSolves = p->solve();
                if (possibleSolves == 2)
                {
                    delete p;
                    return 2;
                }
                else if (possibleSolves == 1)
                {
                    if (numberOfSolutions == 1)
                    {
                        delete solution;
                        delete p;
                        return 2;
                    }
                    solution = p;
                    numberOfSolutions++;
                }
                else
                    delete p;
            }
        }
        if (solution == nullptr)
            return 0;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                this->grid[i][j] = solution->grid[i][j];
                for (int k = 0; k < 9; k++)
                {
                    this->possibilities[i][j][k] = false;
                } 
            }
        }
        delete solution;
        return 1;
    }
    return 1;
}

void GameBoard::generateNew()
{
    for (int i = 0; i < SIZE; i++)
    {
        grid[0][i] = i + 1;
        for (int j = 1; j < SIZE; j++)
        {
            grid[j][i] = 0;
        }
    }
    unknowns = SIZE * (SIZE - 1);

    std::random_device rd;
    std::mt19937 mt(rd());
    std::shuffle(&grid[0][0], &grid[0][SIZE], mt);

    // Fill all possibilities
    for (int i = 0; i < SIZE; i++)
        for (int j = 1; j < SIZE; j++)
            redoPossibilitiesAt(j, i);
    completeGrid();
}

bool GameBoard::completeGrid()
{
    // While there are still empty fields
    while (unknowns > 0)
    {
        // Find the field with the least possibilities
        int numberOfPossibilities, bestI, bestJ, bestBet = SIZE;
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                if (grid[i][j] == 0)
                {
                    numberOfPossibilities = 0;
                    for (int k = 0; k < 9; k++)
                        if (possibilities[i][j][k])
                            numberOfPossibilities++;
                    if (numberOfPossibilities == 0)
                        return false;
                    
                    if (numberOfPossibilities < bestBet)
                    {
                        bestBet = numberOfPossibilities;
                        bestI = i;
                        bestJ = j;
                    }
                }

        // Get all possibilities for this field and shuffle them
        int fieldPossibilities[bestBet];
        int counter = 0;
        for (int i = 0; counter < bestBet; i++)
            if (possibilities[bestI][bestJ][i])
            {
                fieldPossibilities[counter] = i + 1;
                counter++;
            }
        std::random_device rd;
        std::mt19937 mt(rd());
        std::shuffle(&fieldPossibilities[0], &fieldPossibilities[bestBet], mt);

        // Try out this possibilities until finding a valid solution
        GameBoard* candidate = nullptr;
        for (int i = 0; i < bestBet && candidate == nullptr; i++)
        {
            GameBoard* p = new GameBoard(this);
            if (p->fillAndFix(bestI, bestJ, fieldPossibilities[i]) == false)
                delete p;
            else if (p->completeGrid())
                candidate = p;
            else 
                delete p;
        }

        // If no candidate was found then this grid is unsolveable
        if (candidate == nullptr)
            return false;

        // Else copy solved grid to original grid
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
            {
                this->grid[i][j] = candidate->grid[i][j];
                for (int k = 0; k < 9; k++)
                    this->possibilities[i][j][k] = false; 
            }
        delete candidate;
        unknowns = 0;
        return true;
    }
    return true;
}

void GameBoard::removeRandom()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    while (unknowns <= 50)
    {
        int toRemoveI = mt() % SIZE;
        int toRemoveJ = mt() % SIZE;
        if (grid[toRemoveI][toRemoveJ] == 0)
            continue;

        if (unknowns <= 20)
        {
            int deleted1 = grid[toRemoveI][toRemoveJ];
            int deleted2 = grid[SIZE - 1 - toRemoveI][toRemoveJ];
            int deleted3 = grid[toRemoveI][SIZE - 1 - toRemoveJ];
            int deleted4 = grid[SIZE - 1 - toRemoveI][SIZE - 1 - toRemoveJ];
            grid[toRemoveI][toRemoveJ] = 0;
            grid[SIZE - 1 - toRemoveI][toRemoveJ] = 0;
            grid[toRemoveI][SIZE - 1 - toRemoveJ] = 0;
            grid[SIZE - 1 - toRemoveI][SIZE - 1 - toRemoveJ] = 0;

            GameBoard* oneSolutionTest = new GameBoard(this);
            oneSolutionTest->fillAllPossibilities();
            oneSolutionTest->solveObvious();
            int removed = 4;
            if (toRemoveI == SIZE - 1 - toRemoveI)
            {
                removed /= 2;
            }
            if (toRemoveJ == SIZE - 1 - toRemoveJ)
            {
                removed /= 2;
            }
            oneSolutionTest->unknowns = unknowns + removed;
            
            if (oneSolutionTest->solve() == 1)
            {
                unknowns += removed;
            }
            else 
            {
                grid[toRemoveI][toRemoveJ] = deleted1;
                grid[SIZE - 1 - toRemoveI][toRemoveJ] = deleted2;
                grid[toRemoveI][SIZE - 1 - toRemoveJ] = deleted3;
                grid[SIZE - 1 - toRemoveI][SIZE - 1 - toRemoveJ] = deleted4;
            }
            delete oneSolutionTest;
            printGrid();
        }
        else if (unknowns <= 40)
        {
            int deleted1 = grid[toRemoveI][toRemoveJ];
            int deleted2 = grid[SIZE - 1 - toRemoveI][SIZE - 1 - toRemoveJ];
            grid[toRemoveI][toRemoveJ] = 0;
            grid[SIZE - 1 - toRemoveI][SIZE - 1 - toRemoveJ] = 0;

            GameBoard* oneSolutionTest = new GameBoard(this);
            oneSolutionTest->fillAllPossibilities();
            oneSolutionTest->solveObvious();
            int removed = 2;
            if (toRemoveI == SIZE - 1 - toRemoveI && toRemoveJ == SIZE - 1 - toRemoveJ)
            {
                removed /= 2;
            }
            oneSolutionTest->unknowns = unknowns + removed;
            
            if (oneSolutionTest->solve() == 1)
            {
                unknowns += removed;
            }
            else 
            {
                grid[toRemoveI][toRemoveJ] = deleted1;
                grid[SIZE - 1 - toRemoveI][SIZE - 1 - toRemoveJ] = deleted2;
            }
            delete oneSolutionTest;
        }
        else
        {
            int deleted = grid[toRemoveI][toRemoveJ];
            grid[toRemoveI][toRemoveJ] = 0;

            GameBoard* oneSolutionTest = new GameBoard(this);
            oneSolutionTest->fillAllPossibilities();
            oneSolutionTest->solveObvious();
            oneSolutionTest->unknowns = unknowns + 1;
            if (oneSolutionTest->solve() == 1)
            {
                unknowns += 1;
            }
            else 
            {
                grid[toRemoveI][toRemoveJ] = deleted;
            }
            delete oneSolutionTest;
        }
    }
    
}
