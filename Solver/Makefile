executables = solver generator
commonObjectFiles = GameBoard.o PossibilitiesBoard.o Solver.o Statistics.o
solverObjectFiles = ${commonObjectFiles} mainS.o
generatorObjectFiles = ${commonObjectFiles} Generator.o mainG.o

all: ${executables}

solver: ${solverObjectFiles}
	g++ -o solver ${solverObjectFiles}

generator: ${generatorObjectFiles}
	g++ -o generator ${generatorObjectFiles}

%.o: %.cpp GameBoard.hpp
	g++ -c $^

# Czyszczenie
exe: ${executables}
	rm -f *.o *.~

clean:
	rm -f *.o *.~ *.gch ${executables}
