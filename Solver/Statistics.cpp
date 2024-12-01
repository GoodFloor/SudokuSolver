#include "Statistics.hpp"

int Statistics::depth = 0;
int Statistics::maxDepth = 0;
int Statistics::foundHiddenSingles = 0;
int Statistics::foundNakedPairs = 0;

int Statistics::getDepth()
{
    return Statistics::depth;
}

int Statistics::getMaxDepth()
{
    return Statistics::maxDepth;
}

int Statistics::getFoundHiddenSingles()
{
    return Statistics::foundHiddenSingles;
}

int Statistics::getfoundNakedPairs()
{
    return Statistics::foundNakedPairs;
}

void Statistics::resetStatistics()
{
    Statistics::depth = 0;
    Statistics::maxDepth = 0;
    Statistics::foundHiddenSingles = 0;
    Statistics::foundNakedPairs = 0;
}

void Statistics::enterSolve()
{
    Statistics::depth++;
    if (Statistics::depth > Statistics::maxDepth)
        Statistics::maxDepth = Statistics::depth;
}

void Statistics::exitSolve()
{
    Statistics::depth--;
}

void Statistics::successfullSolve(int foundHiddenSingles, int foundNakedPairs)
{
    Statistics::foundHiddenSingles += foundHiddenSingles;
    Statistics::foundNakedPairs += foundNakedPairs;
}
