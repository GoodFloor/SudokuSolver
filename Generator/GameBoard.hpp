class GameBoard
{
private:
    int grid[9][9];
    std::string possibilitiesChar[9] = {"¹","²","³","⁴","⁵","⁶","⁷","⁸","⁹"};
    bool possibilities[9][9][9];
    int unknowns;
    const int SIZE = 9;
public:
    GameBoard();
    ~GameBoard();
    GameBoard(GameBoard* board);
    void loadGrid();
    void printGrid();
    void exportGrid();
    int getNumberAt(int x, int y);
    void setNumberAt(int x, int y, int n);
    bool isPossibleAt(int x, int y, int n);
    
    void fillAllPossibilities();
    void redoPossibilitiesAt(int x, int y);
    void solveObvious();
    bool fillAndFix(int x, int y, int n);
    int solve();
    void generateNew();
    bool completeGrid();
    void removeRandom();
};
