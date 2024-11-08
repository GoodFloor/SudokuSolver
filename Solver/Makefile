executables = solver
objectFiles = main.o GameBoard.o PossibilitiesBoard.o Solver.o

all: ${executables}

solver: ${objectFiles}
	g++ -o solver ${objectFiles}

%.o: %.cpp %.hpp
	g++ -c $^

# Czyszczenie
exe: ${executables}
	rm -f *.o *.~

clean:
	rm -f *.o *.~ ${executables}
