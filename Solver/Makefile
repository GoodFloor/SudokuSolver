executables = solver generator
commonObjectFiles = GameBoard.o PossibilitiesBoard.o Solver.o GameBoard.hpp
solverObjectFiles = ${commonObjectFiles} mainS.o
generatorObjectFiles = ${commonObjectFiles} Generator.o mainG.o

all: ${executables}

solver: ${solverObjectFiles}
	g++ -o solver ${solverObjectFiles}

generator: ${generatorObjectFiles}
	g++ -o generator ${generatorObjectFiles}

%.o: %.cpp
	g++ -c $^

# Czyszczenie
exe: ${executables}
	rm -f *.o *.~

clean:
	rm -f *.o *.~ *.gch ${executables}
