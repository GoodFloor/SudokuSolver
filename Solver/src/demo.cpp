#include <iostream>
#include <string.h>
#include "Generator.hpp"
#include "Solver.hpp"

using namespace std;

void wrongSyntaxMessage()
{
    cout << "Poprawna składnia:" << endl;
    cout << "\tdemo [generator/solver] <n>" << endl;
    cout << "Gdzie:" << endl;
    cout << "\tgenerator:\tgeneruje <n> łamigłówek rozwiązywalnych bez zgadywania. Na standardowym wyjściu wypisuje je w formacie kompaktowym." << endl;
    cout << "\tsolver:   \trozwiązuje <n> łamigłówek wczytanych ze standardowego wejścia. Dla każdej z nich drukuje planszę przed i po rozwiązaniu." << endl;
    cout << "\t<n> musi być większe od 0!" << endl;
}

void generate(int n)
{
    Generator* generator = new Generator();
    for (int i = 0; i < n; i++)
    {
        GameBoard* board = generator->generateSudokuWithoutGuessing();
        board->exportGrid();
        delete board;
    }
    delete generator;    
}

void solve(int n)
{
    for (int i = 0; i < n; i++)
    {
        GameBoard* board = new GameBoard();
        board->loadGrid();
        board->printGrid();
        Solver* solver = new Solver(board, false);
        int r = solver->solve();
        if (r == -1)
            cout << "Plansza nie ma rozwiązań" << endl;
        else
            solver->getBoard()->printGrid();
        delete board;
        delete solver;
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        wrongSyntaxMessage();
        return 44;
    }
    int n = atoi(argv[2]);
    if (n <= 0)
    {
        wrongSyntaxMessage();
        return 44;
    }
    if (strcmp(argv[1], "generator") == 0)
    {
        generate(n);
    }
    else if (strcmp(argv[1], "solver") == 0)
    {
        solve(n);
    }
    else
    {
        wrongSyntaxMessage();
        return 44;
    }
    return 0;
}
