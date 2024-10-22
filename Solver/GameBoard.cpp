#include <iostream>
#include <list>
#include "GameBoard.hpp"
#include "Possibility.hpp"

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
    printGrid();
    while (!onePossibilityList.empty())
    {
        Possibility nextPossibility = onePossibilityList.front();
        if (fillAndFix(nextPossibility.i, nextPossibility.j, nextPossibility.p) == false)
            return false;
        onePossibilityList.pop_front();
    }
    return true;
}

bool GameBoard::solve()
{
    while (unknowns > 0)
    {
        int bestI, bestJ, bestBet = 10;
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
                        return false;
                    
                    if (numberOfPossibilities < bestBet)
                    {
                        bestBet = numberOfPossibilities;
                        bestI = i;
                        bestJ = j;
                    }
                }
            }
        }
        GameBoard* solution = nullptr;
        for (int k = 0; k < 9 && solution == nullptr; k++)
        {
            if (possibilities[bestI][bestJ][k])
            {
                GameBoard* p = new GameBoard(this);
                bool isValid = p->fillAndFix(bestI, bestJ, k + 1);
                if (p->solve() && isValid)
                    solution = p;
                else
                    delete p;
            }
        }
        if (solution == nullptr)
            return false;
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
        return true;
    }
    return true;
}
