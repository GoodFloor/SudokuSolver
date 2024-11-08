#include "PossibilitiesBoard.hpp"
#include <string>
#include <iostream>

PossibilitiesBoard::PossibilitiesBoard()
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                grid[i][j][k] = true;
}

PossibilitiesBoard::PossibilitiesBoard(PossibilitiesBoard *p)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                this->grid[i][j][k] = p->grid[i][j][k];
}

PossibilitiesBoard::~PossibilitiesBoard()
{
}

bool PossibilitiesBoard::isPossible(int x, int y, int z)
{
    if (x < 0 || y < 0 || z <= 0 || x >= N || y >= N || z > N)
        throw std::out_of_range("Out of possibilities range");
    return grid[x][y][z - 1];
}

void PossibilitiesBoard::setPossible(int x, int y, int z)
{
    grid[x][y][z - 1] = true;
}

void PossibilitiesBoard::setImpossible(int x, int y, int z)
{
    grid[x][y][z - 1] = false;
}

int PossibilitiesBoard::getFirstPossibility(int x, int y)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 1; k <= N; k++)
                if (isPossible(x, y, k))
                    return k;
    return 0;
}

int PossibilitiesBoard::getOnlyPossibilityAt(int x, int y)
{
    int onlyPossibility = 0;
    for (int k = 1; k <= N; k++)
        if (isPossible(x, y, k) && onlyPossibility != 0)
            return -1;
        else if (isPossible(x, y, k))
            onlyPossibility = k;    
    return onlyPossibility;
}

void PossibilitiesBoard::printPossibilities()
{
    for (int i = 0; i <= N * 4; i++)
    {
        for (int j = 0; j <= N * 4; j++)
        {
            int n = ((i % 4) - 1) * 3 + (j % 4);
            // Pogrubione linie
            if (i % (SIZE * 4) == 0 || j % (SIZE * 4) == 0)
                std::cout << "█";
            // Skrzyżowania
            else if (i % 4 == 0 && j % 4 == 0)
                std::cout << "┼";
            // Pionowe linie
            else if (j % 4 == 0)    
                std::cout << "│";
            // Poziome linie
            else if (i % 4 == 0)    
                std::cout << "─";
            // Rozwiązane pola
            else if (n > 0 && n <= N && this->isPossible(i / 4, j / 4, n))
                std::cout << n;
            else
                std::cout << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
}

void PossibilitiesBoard::clearPossibilitiesAt(int x, int y)
{
    for (int k = 0; k < N; k++)
    {
        grid[x][y][k] = false;
    }
    
}

int PossibilitiesBoard::getNumberOfPossibilitiesAt(int x, int y)
{
    int n = 0;
    for (int i = 1; i <= N; i++)
        if (this->isPossible(x, y, i))
            n++;
    return n;
}
