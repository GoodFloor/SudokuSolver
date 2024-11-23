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
    // return somethingChanged;
    int currentPair[2];

    // Rows
    for (int i = 0; i < GameBoard::N; i++)
    {
        // Check each cell
        for (int j = 0; j < GameBoard::N - 1; j++)
            // If 2 possibilities in a cell
            if (possibilitiesBoard->getNumberOfPossibilitiesAt(i, j) == 2)
            {
                // Save them
                int t = 0;
                for (int k = 1; k <= GameBoard::N && t < 2; k++)
                    if (possibilitiesBoard->isPossible(i, j, k))
                    {
                        currentPair[t] = k;
                        t++;
                    }
                // Check each following cell
                for (int k = j + 1; k < GameBoard::N; k++)
                    // If this pair repeats
                    if (possibilitiesBoard->getNumberOfPossibilitiesAt(i, k) == 2 && possibilitiesBoard->isPossible(i, k, currentPair[0]) && possibilitiesBoard->isPossible(i, k, currentPair[1]))
                    {
                        // // DEBUG START
                        // printf("######## BEGIN ROW ########\n");
                        // printf("(%i, %i) at (%i, %i) and (%i, %i)\n", currentPair[0], currentPair[1], i, j, i, k);
                        // possibilitiesBoard->printPossibilities();
                        // // DEBUG END
                        // Remove this two numbers from other cells in this area;
                        for (int l = 0; l < GameBoard::N; l++)
                            if (l != j && l != k && (possibilitiesBoard->isPossible(i, l, currentPair[0]) || possibilitiesBoard->isPossible(i, l, currentPair[1])))
                            {
                                // // DEBUG START
                                // printf("(%i, %i)\n", i, l);
                                // // DEBUG END
                                somethingChanged = true;
                                possibilitiesBoard->setImpossible(i, l, currentPair[0]);
                                possibilitiesBoard->setImpossible(i, l, currentPair[1]);
                                if (possibilitiesBoard->getNumberOfPossibilitiesAt(i, l) == 0)
                                    throw 11;
                                else if (possibilitiesBoard->getNumberOfPossibilitiesAt(i, l) == 1)
                                    if (!this->fillAndFix(i, l, possibilitiesBoard->getFirstPossibility(i, l)))
                                        throw 11;
                                
                            }
                        // // DEBUG START
                        // possibilitiesBoard->printPossibilities();
                        // printf("######## END ########\n");
                        // // DEBUG END
                        break;
                    }
            } 
    }

    // Columns
    for (int i = 0; i < GameBoard::N; i++)
    {
        // Check each cell
        for (int j = 0; j < GameBoard::N - 1; j++)
            // If 2 possibilities in a cell
            if (possibilitiesBoard->getNumberOfPossibilitiesAt(j, i) == 2)
            {
                // Save them
                int t = 0;
                for (int k = 1; k <= GameBoard::N && t < 2; k++)
                    if (possibilitiesBoard->isPossible(j, i, k))
                    {
                        currentPair[t] = k;
                        t++;
                    }
                // Check each following cell
                for (int k = j + 1; k < GameBoard::N; k++)
                    // If this pair repeats
                    if (possibilitiesBoard->getNumberOfPossibilitiesAt(k, i) == 2 && possibilitiesBoard->isPossible(k, i, currentPair[0]) && possibilitiesBoard->isPossible(k, i, currentPair[1]))
                    {
                        // // DEBUG START
                        // printf("######## BEGIN COLUMN ########\n");
                        // printf("(%i, %i) at (%i, %i) and (%i, %i)\n", currentPair[0], currentPair[1], j, i, k, i);
                        // possibilitiesBoard->printPossibilities();
                        // // DEBUG END
                        // Remove this two numbers from other cells in this area;
                        for (int l = 0; l < GameBoard::N; l++)
                            if (l != j && l != k && (possibilitiesBoard->isPossible(l, i, currentPair[0]) || possibilitiesBoard->isPossible(l, i, currentPair[1])))
                            {
                                // // DEBUG START
                                // printf("(%i, %i)\n", l, i);
                                // // DEBUG END
                                somethingChanged = true;
                                possibilitiesBoard->setImpossible(l, i, currentPair[0]);
                                possibilitiesBoard->setImpossible(l, i, currentPair[1]);
                                if (possibilitiesBoard->getNumberOfPossibilitiesAt(l, i) == 0)
                                    throw 11;
                                else if (possibilitiesBoard->getNumberOfPossibilitiesAt(l, i) == 1)
                                    if (!this->fillAndFix(l, i, possibilitiesBoard->getFirstPossibility(l, i)))
                                        throw 11;
                                
                            }
                        // // DEBUG START
                        // possibilitiesBoard->printPossibilities();
                        // printf("######## END ########\n");
                        // // DEBUG END
                        break;
                    }
            } 
    }

    // Squares
    int squareI = 0;
    int squareJ = 0;
    for (int i = 0; i < GameBoard::N; i++)
    {
        // Check each cell
        for (int j = 0; j < GameBoard::N - 1; j++)
            // If 2 possibilities in a cell
            if (possibilitiesBoard->getNumberOfPossibilitiesAt(getSquareI(squareI, j), getSquareJ(squareJ, j)) == 2)
            {
                // Save them
                int t = 0;
                for (int k = 1; k <= GameBoard::N && t < 2; k++)
                    if (possibilitiesBoard->isPossible(getSquareI(squareI, j), getSquareJ(squareJ, j), k))
                    {
                        currentPair[t] = k;
                        t++;
                    }
                // Check each following cell
                for (int k = j + 1; k < GameBoard::N; k++)
                    // If this pair repeats
                    if (possibilitiesBoard->getNumberOfPossibilitiesAt(getSquareI(squareI, k), getSquareJ(squareJ, k)) == 2 && possibilitiesBoard->isPossible(getSquareI(squareI, k), getSquareJ(squareJ, k), currentPair[0]) && possibilitiesBoard->isPossible(getSquareI(squareI, k), getSquareJ(squareJ, k), currentPair[1]))
                    {
                        // // DEBUG START
                        // printf("######## BEGIN SQUARE ########\n");
                        // printf("(%i, %i) at (%i, %i) and (%i, %i)\n", currentPair[0], currentPair[1], getSquareI(squareI, j), getSquareJ(squareJ, j), getSquareI(squareI, k), getSquareJ(squareJ, k));
                        // possibilitiesBoard->printPossibilities();
                        // // DEBUG END
                        // Remove this two numbers from other cells in this area;
                        for (int l = 0; l < GameBoard::N; l++)
                            if (l != j && l != k && (possibilitiesBoard->isPossible(getSquareI(squareI, l), getSquareJ(squareJ, l), currentPair[0]) || possibilitiesBoard->isPossible(getSquareI(squareI, l), getSquareJ(squareJ, l), currentPair[1])))
                            {
                                // // DEBUG START
                                // printf("(%i, %i)\n", getSquareI(squareI, l), getSquareJ(squareJ, l));
                                // // DEBUG END
                                somethingChanged = true;
                                possibilitiesBoard->setImpossible(getSquareI(squareI, l), getSquareJ(squareJ, l), currentPair[0]);
                                possibilitiesBoard->setImpossible(getSquareI(squareI, l), getSquareJ(squareJ, l), currentPair[1]);
                                if (possibilitiesBoard->getNumberOfPossibilitiesAt(getSquareI(squareI, l), getSquareJ(squareJ, l)) == 0)
                                    throw 11;
                                else if (possibilitiesBoard->getNumberOfPossibilitiesAt(getSquareI(squareI, l), getSquareJ(squareJ, l)) == 1)
                                    if (!this->fillAndFix(getSquareI(squareI, l), getSquareJ(squareJ, l), possibilitiesBoard->getFirstPossibility(getSquareI(squareI, l), getSquareJ(squareJ, l))))
                                        throw 11;
                                
                            }
                        // // DEBUG START
                        // possibilitiesBoard->printPossibilities();
                        // printf("######## END ########\n");
                        // // DEBUG END
                        break;
                    }
            } 
        squareJ = (squareJ + GameBoard::SIZE) % GameBoard::N;
        if (squareJ == 0)
            squareI += GameBoard::SIZE;
        
    }    
    // // DEBUG START
    // printf("######## RESULT: %i ########\n", somethingChanged);
    // // DEBUG END
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
}

Solver::Solver(Solver *solver)
{
    this->checkForMultipleSolutions = solver->checkForMultipleSolutions;
    this->unknowns = solver->unknowns;
    this->solutionBoard = new GameBoard(solver->solutionBoard);
    this->possibilitiesBoard = new PossibilitiesBoard(solver->possibilitiesBoard);
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
                }
                if (this->findNakedPairs())
                {
                    // printf("Found naked pairs\n");
                    somethingChanged = true;
                    difficultyRating += 1;
                }
            }
            catch(const int e)
            {
                return -1;
            }
            // if (somethingChanged)
            //     possibilitiesBoard->printPossibilities();
        }
        if (unknowns == 0)
            return difficultyRating;

        // printf("Requires guessing\n");
        difficultyRating += 5;
        
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
                        return -1;
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
                return -2;
            } 
        }

        // If no candidate was found then this grid is unsolveable
        if (candidate == nullptr)
            return -1;

        // Set candidate as current solution
        GameBoard* temp = this->solutionBoard;
        this->solutionBoard = candidate;
        delete temp;
        PossibilitiesBoard* tempP = this->possibilitiesBoard;
        this->possibilitiesBoard = candidatePossibilities;
        delete tempP;
        unknowns = 0;
    }
    return difficultyRating;
}

void Solver::printPossibilities()
{
    possibilitiesBoard->printPossibilities();
}
