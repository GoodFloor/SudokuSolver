#include <list>
#include "Solver.hpp"

int Solver::findHiddenSingles()
{
    int foundCounter = 0;
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
                foundCounter++;
                if (!this->fillAndFix(lastPosition[k][0], lastPosition[k][1], k + 1))
                    throw 11;
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
                foundCounter++;
                if (!this->fillAndFix(lastPosition[k][0], lastPosition[k][1], k + 1))
                    throw 11;
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
                foundCounter++;
                if (!this->fillAndFix(lastPosition[k][0], lastPosition[k][1], k + 1))
                    throw 11;
            }
    }

    return foundCounter;
}

int Solver::findNakedPairs()
{
    int foundCounter = 0;
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
                bool flag = true;
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
                                if (flag)
                                {
                                    flag = false;
                                    foundCounter++;
                                }
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
                
                flag = true;
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
                                if (flag)
                                {
                                    flag = false;
                                    foundCounter++;
                                }
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

                flag = true;
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
                                if (flag)
                                {
                                    flag = false;
                                    foundCounter++;
                                }
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
    return foundCounter;
}
