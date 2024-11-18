#ifndef GAME_BOARD_H
#define GAME_BOARD_H

class GameBoard
{
public:
    static const int SIZE = 3;
    static const int N = SIZE * SIZE;
private:
    int grid[N][N];
public:
    GameBoard();
    ~GameBoard();
    GameBoard(GameBoard* board);
    void loadGrid();
    void printGrid();
    int getNumberAt(int x, int y);
    void setNumberAt(int x, int y, int n);
};

#endif
