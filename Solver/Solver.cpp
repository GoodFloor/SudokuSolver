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

bool Solver::findHiddenSingles()
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

bool Solver::findNakedPairs()
{
    bool somethingChanged = false;
    int currentPair[2];

    // For each cell
    for (int i = 0; i < GameBoard::N; i++)
        for (int j = 0; j < GameBoard::N; j++)
            // If it has 2 possibilities
            if (possibilitiesBoard->getNumberOfPossibilitiesAt(i, j) == 2)
            {
                // Save them for later
                int t = 0;
                for (int k = 1; k <= GameBoard::N && t < 2; k++)
                    if (possibilitiesBoard->isPossible(i, j, k))
                    {
                        currentPair[t] = k;
                        t++;
                    }
                
                // Find same pair in row
                for (int k = j + 1; k < GameBoard::N && !usedAsNakedPair[i][j][0]; k++)
                    if (possibilitiesBoard->getNumberOfPossibilitiesAt(i, k) == 2 && possibilitiesBoard->isPossible(i, k, currentPair[0]) && possibilitiesBoard->isPossible(i, k, currentPair[1]))
                    {
                        usedAsNakedPair[i][j][0] = true;
                        usedAsNakedPair[i][k][0] = true;
                        // Remove this two numbers from other cells in this area;
                        for (int l = 0; l < GameBoard::N; l++)
                            if (l != j && l != k && (possibilitiesBoard->isPossible(i, l, currentPair[0]) || possibilitiesBoard->isPossible(i, l, currentPair[1])))
                            {
                                somethingChanged = true;
                                possibilitiesBoard->setImpossible(i, l, currentPair[0]);
                                possibilitiesBoard->setImpossible(i, l, currentPair[1]);
                                if (possibilitiesBoard->getNumberOfPossibilitiesAt(i, l) == 0)
                                    throw 11;
                                else if (possibilitiesBoard->getNumberOfPossibilitiesAt(i, l) == 1)
                                    if (!this->fillAndFix(i, l, possibilitiesBoard->getFirstPossibility(i, l)))
                                        throw 11;
                            }
                        break;
                    }
                
                // Find same pair in column
                for (int k = i + 1; k < GameBoard::N && !usedAsNakedPair[i][j][1]; k++)
                    if (possibilitiesBoard->getNumberOfPossibilitiesAt(k, j) == 2 && possibilitiesBoard->isPossible(k, j, currentPair[0]) && possibilitiesBoard->isPossible(k, j, currentPair[1]))
                    {
                        usedAsNakedPair[i][j][1] = true;
                        usedAsNakedPair[k][j][1] = true;
                        // Remove this two numbers from other cells in this area;
                        for (int l = 0; l < GameBoard::N; l++)
                            if (l != i && l != k && (possibilitiesBoard->isPossible(l, j, currentPair[0]) || possibilitiesBoard->isPossible(l, j, currentPair[1])))
                            {
                                somethingChanged = true;
                                possibilitiesBoard->setImpossible(l, j, currentPair[0]);
                                possibilitiesBoard->setImpossible(l, j, currentPair[1]);
                                if (possibilitiesBoard->getNumberOfPossibilitiesAt(l, j) == 0)
                                    throw 11;
                                else if (possibilitiesBoard->getNumberOfPossibilitiesAt(l, j) == 1)
                                    if (!this->fillAndFix(l, j, possibilitiesBoard->getFirstPossibility(l, j)))
                                        throw 11;
                            }
                        break;
                    }
                
                // Find same pair in region
                int regionRootI = i / GameBoard::SIZE * GameBoard::SIZE;
                int regionRootJ = j / GameBoard::SIZE * GameBoard::SIZE;
                int positionInRegion = (i - regionRootI) * GameBoard::SIZE + (j - regionRootJ);
                for (int k = positionInRegion + 1; k < GameBoard::N && !usedAsNakedPair[i][j][2]; k++)
                    if (possibilitiesBoard->getNumberOfPossibilitiesAt(getSquareI(regionRootI, k), getSquareJ(regionRootJ, k)) == 2 && possibilitiesBoard->isPossible(getSquareI(regionRootI, k), getSquareJ(regionRootJ, k), currentPair[0]) && possibilitiesBoard->isPossible(getSquareI(regionRootI, k), getSquareJ(regionRootJ, k), currentPair[1]))
                    {
                        usedAsNakedPair[i][j][2] = true;
                        usedAsNakedPair[getSquareI(regionRootI, k)][getSquareJ(regionRootJ, k)][1] = true;
                        // Remove this two numbers from other cells in this area;
                        for (int l = 0; l < GameBoard::N; l++)
                            if (!(getSquareI(regionRootI, l) == i && getSquareJ(regionRootJ, l) == j) && l != k && (possibilitiesBoard->isPossible(getSquareI(regionRootI, l), getSquareJ(regionRootJ, l), currentPair[0]) || possibilitiesBoard->isPossible(getSquareI(regionRootI, l), getSquareJ(regionRootJ, l), currentPair[1])))
                            {
                                somethingChanged = true;
                                possibilitiesBoard->setImpossible(getSquareI(regionRootI, l), getSquareJ(regionRootJ, l), currentPair[0]);
                                possibilitiesBoard->setImpossible(getSquareI(regionRootI, l), getSquareJ(regionRootJ, l), currentPair[1]);
                                if (possibilitiesBoard->getNumberOfPossibilitiesAt(getSquareI(regionRootI, l), getSquareJ(regionRootJ, l)) == 0)
                                    throw 11;
                                else if (possibilitiesBoard->getNumberOfPossibilitiesAt(getSquareI(regionRootI, l), getSquareJ(regionRootJ, l)) == 1)
                                    if (!this->fillAndFix(getSquareI(regionRootI, l), getSquareJ(regionRootJ, l), possibilitiesBoard->getFirstPossibility(getSquareI(regionRootI, l), getSquareJ(regionRootJ, l))))
                                        throw 11;
                            }
                        break;
                    }
            }
    return somethingChanged;
}

bool Solver::findPointingNumbers()
{
    bool somethingChanged = false;
    // For each row
    for (int i = 0; i < GameBoard::N; i++)
    {
        // Remembers if given number didn't appear yet (-1), appear in multiple or none of regions (-2) or appeared in one region only (number of this region)
        int region[GameBoard::N];
        for (int k = 0; k < GameBoard::N; k++)
            region[k] = -1;
        // How many numbers are assigned sth > -2
        int possiblePointingPairs = GameBoard::N;
        // For each cell in a row
        for (int j = 0; j < GameBoard::N && possiblePointingPairs > 0; j++)
        {
            int num = solutionBoard->getNumberAt(i, j);
            // Num
            if (num > 0)
            {
                region[num - 1] = -2;
                possiblePointingPairs--;
                continue;
            }
            int regionI = i / GameBoard::SIZE * GameBoard::SIZE;
            int regionJ = j / GameBoard::SIZE * GameBoard::SIZE;
            int regionId = regionI + regionJ / GameBoard::SIZE;
            // For each possibility
            for (int p = 1; p <= GameBoard::N; p++)
            {
                if (region[p - 1] == -2 || !possibilitiesBoard->isPossible(i, j, p))
                    continue;
                
                // Possibility didn't appear before
                if (region[p - 1] == -1)
                {
                    region[p - 1] = regionId;
                }
                // Possibility appeared in different region
                else if (region[p - 1] != regionId)
                {
                    region[p - 1] = -2;
                    possiblePointingPairs--;
                }
            }
        }
        // For each possibility
        for (int p = 1; p <= GameBoard::N && possiblePointingPairs > 0; p++)
        {
            // It appeared in multiple or none regions
            if (region[p - 1] < 0)
                continue;
            
            // In that region remove it from all other cells
            int regionRootI = region[p - 1] / GameBoard::SIZE * GameBoard::SIZE;
            int regionRootJ = region[p - 1] % GameBoard::SIZE * GameBoard::SIZE;
            for (int k = 0; k < GameBoard::N; k++)
            {
                int ii = getSquareI(regionRootI, k);
                int jj = getSquareJ(regionRootJ, k);
                if (ii == i || solutionBoard->getNumberAt(ii, jj) > 0)
                    continue;
                if (possibilitiesBoard->isPossible(ii, jj, p))
                {
                    somethingChanged = true;
                    possibilitiesBoard->setImpossible(ii, jj, p);
                }
            }  
        }
    }
    
    return somethingChanged;
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

GameBoard *Solver::exportBoard()
{
    GameBoard* myBoard = this->solutionBoard;
    this->solutionBoard = nullptr;
    return myBoard;
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
    // printf("%i\n", Statistics::getDepth());
    int difficultyRating = 0;
    while (unknowns > 0)
    {
        // Use clever algorythms to eliminate possibilities
        bool somethingChanged = true;
        while (somethingChanged)
        {
            somethingChanged = false;
            try
            {
                if (this->findHiddenSingles())
                {
                    // printf("Found hidden singles\n");
                    somethingChanged = true;
                    difficultyRating += 1;
                    hiddenSinglesFound++;
                    continue;
                }
                if (this->findNakedPairs())
                {
                    // printf("Found naked pairs\n");
                    somethingChanged = true;
                    difficultyRating += 2;
                    nakedPairsFound++;
                    continue;
                }
                // if (this->findPointingNumbers())
                // {
                //     somethingChanged = true;
                //     difficultyRating += 2;
                //     continue;
                // }
                
            }
            catch(const int e)
            {
                Statistics::exitSolve();
                return -1;
            }
            // if (somethingChanged)
            //     possibilitiesBoard->printPossibilities();
        }
        if (unknowns == 0)
        {
            Statistics::successfullSolve(hiddenSinglesFound, nakedPairsFound);
            Statistics::exitSolve();
            return difficultyRating;
        }

        // printf("Requires guessing\n");
        difficultyRating += 10;
        
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
                    // printf("Found hidden singles\n");
                    somethingChanged = true;
                    difficultyRating += 1;
                    continue;
                }
                if (this->findNakedPairs())
                {
                    // printf("Found naked pairs\n");
                    somethingChanged = true;
                    difficultyRating += 2;
                    continue;
                }
                // if (this->findPointingNumbers())
                // {
                //     somethingChanged = true;
                //     difficultyRating += 2;
                //     continue;
                // }
                
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

void Solver::printPossibilities()
{
    possibilitiesBoard->printPossibilities();
}

void Solver::test()
{
    possibilitiesBoard->printPossibilities();
    findPointingNumbers();
    possibilitiesBoard->printPossibilities();
}
