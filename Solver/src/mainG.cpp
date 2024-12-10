#include <iostream>
#include <iomanip>
#include <csignal>
#include <string.h>
#include "Solver.hpp"
#include "Generator.hpp"
#include "Statistics.hpp"

using namespace std;

bool breakFlag = true;

void signalHandler(int n)
{
    breakFlag = false;
    return;
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, signalHandler);

    int maximumUnknowns = 0;
    int maximumDifficulty = 0;
    int maximumDepth = 0;
    double last = 0.0;

    // Statistics
    int unknownsData[GameBoard::N * GameBoard::N];
    int difficultyData[GameBoard::N * GameBoard::N];
    unsigned long long averageDepth = 0;
    unsigned long long averageHiddenSingles = 0;
    unsigned long long averageNakedPairs = 0;
    unsigned long long averageDifficulty = 0;
    unsigned long long averageUnknnowns = 0;
    for (int i = 0; i < GameBoard::N * GameBoard::N; i++)
    {
        unknownsData[i] = 0;
        difficultyData[i] = 0;
    }
    // Generating and testing boards
    int targetPermutations = 10'000;
    for (int i = 1; i <= targetPermutations && breakFlag; i++)
    {
        Generator* generator = new Generator();
        GameBoard* board;
        if (argc == 2 && strcmp(argv[1], "L") == 0)
            board = generator->generateSudoku();
        else
            board = generator->generateSudokuWithoutGuessing();
        
        delete generator;
        // board->printGrid();

        Statistics::resetStatistics();
        Solver* solver = new Solver(board, false);
        int r = solver->solve();
        r = solver->normalizeDifficulty(board, r);
        if (r > maximumDifficulty)
            maximumDifficulty = r;
        if (Statistics::getMaxDepth() > maximumDepth)
            maximumDepth = Statistics::getMaxDepth();
        int numberOfUnknowns = 0;
        for (int i = 0; i < GameBoard::N; i++)
            for (int j = 0; j < GameBoard::N; j++)
                if (board->getNumberAt(i, j) == 0)
                    numberOfUnknowns++;
        if (numberOfUnknowns > maximumUnknowns)
            maximumUnknowns = numberOfUnknowns;
        
        // Statistics
        unknownsData[numberOfUnknowns]++;
        // cout << numberOfUnknowns << endl;
        difficultyData[r]++;
        averageDifficulty += r;
        averageDepth += Statistics::getMaxDepth();
        averageHiddenSingles += Statistics::getFoundHiddenSingles();
        averageNakedPairs += Statistics::getfoundNakedPairs();
        averageUnknnowns += numberOfUnknowns;

        delete solver;
        delete board;

        // Progress indicator
        cerr << fixed << setprecision(1);
        if ((i * 100.0 / targetPermutations) - last >= .1)
        {
            cerr << i * 100.0 / targetPermutations << endl;
            last = i * 100.0 / targetPermutations;
        }

        if (i == targetPermutations || !breakFlag)
        {
            double avgUnk = 1. * averageUnknnowns / i;
            cout << "Generated boards: " << i << endl;
            cout << "Maximum number of unknowns: " << maximumUnknowns << endl;
            cout << "Maximum difficulty: " << maximumDifficulty << endl;
            cout << "Average difficulty: " << 1. * averageDifficulty / i << endl;
            cout << "Maximum depth: " << maximumDepth << endl;
            cout << "Average depth: " << 1. * averageDepth / i << endl;
            cout << "Average number of hiddenSingles() used: " << 1. * averageHiddenSingles / i << endl;
            cout << "Average number of nakedPairs() used: " << 1. * averageNakedPairs / i << endl;

            cout << "Unknowns summary: " << endl;
            for (int i = 0; i < GameBoard::N * GameBoard::N; i++)
                if (unknownsData[i] != 0)
                    cout << "\t" << i << ";" << unknownsData[i] << endl;

            cout << "Difficulty summary: " << endl;
            for (int i = 0; i < GameBoard::N * GameBoard::N; i++)
                if (difficultyData[i] != 0)
                    cout << "\t" << i << ";" << difficultyData[i] << endl;
        }
    }
    return 0;
}

 
