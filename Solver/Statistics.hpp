#ifndef STATISTICS_H
#define STATISTICS_H

class Statistics
{
private:
    static int depth;
    static int maxDepth;
    static int foundHiddenSingles;
    static int foundNakedPairs;
public:
    Statistics() = delete;
    static int getDepth();
    static int getMaxDepth();
    static int getFoundHiddenSingles();
    static int getfoundNakedPairs();
    static void resetStatistics();
    static void enterSolve();
    static void exitSolve();
    static void successfullSolve(int foundHiddenSingles, int foundNakedPairs);
};

#endif
