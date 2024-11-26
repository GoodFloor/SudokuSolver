#include <iostream>
#include <list>
#include "GameBoard.hpp"
#include "Possibility.hpp"

GameBoard::GameBoard()
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            grid[i][j] = 0; 
}

GameBoard::~GameBoard()
{
}

GameBoard::GameBoard(GameBoard *board)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            this->grid[i][j] = board->grid[i][j];
}

void GameBoard::loadGrid()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            char content;
            std::cin >> content;
            if (content >= '0' && content <= '9')
                grid[i][j] = content - '0';
            else if (content >= 'A' && content <= 'Z')
                grid[i][j] = content - 'A' + 10;
            else 
                j--;
        }
    }
}

void GameBoard::printGrid()
{
    for (int i = 0; i < GameBoard::N; i++)
        for (int j = 0; j < GameBoard::N; j++)
            printf("%i", this->getNumberAt(i, j));
    printf("\n");
    
    // for (int i = 0; i <= GameBoard::N * 4; i++)
    // {
    //     for (int j = 0; j <= GameBoard::N * 4; j++)
    //     {
    //         // Pogrubione linie
    //         if (i % (GameBoard::SIZE * 4) == 0 || j % (GameBoard::SIZE * 4) == 0)
    //             std::cout << "█";
    //         // Skrzyżowania
    //         else if (i % 4 == 0 && j % 4 == 0)
    //             std::cout << "┼";
    //         // Pionowe linie
    //         else if (j % 4 == 0)    
    //             std::cout << "│";
    //         // Poziome linie
    //         else if (i % 4 == 0)    
    //             std::cout << "─";
    //         // Rozwiązane pola (9-...)   
    //         else if (i % 4 == 2 && j % 4 == 2 && this->getNumberAt(i / 4, j / 4) > 9)
    //             std::cout << (char)((this->getNumberAt(i / 4, j / 4) - 10) + 'A');
    //         // Rozwiązane pola (1-9)
    //         else if (i % 4 == 2 && j % 4 == 2 && this->getNumberAt(i / 4, j / 4) != 0)
    //             std::cout << this->getNumberAt(i / 4, j / 4);
    //         else
    //             std::cout << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;
}

int GameBoard::getNumberAt(int x, int y)
{
    return grid[x][y];
}

void GameBoard::setNumberAt(int x, int y, int n)
{
    grid[x][y] = n;
}
