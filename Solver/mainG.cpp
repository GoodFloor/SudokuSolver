#include <iostream>
#include "Solver.hpp"
#include "Generator.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    Generator* generator = new Generator();
    generator->generateSudoku()->printGrid();
    delete generator;
    return 0;
}
