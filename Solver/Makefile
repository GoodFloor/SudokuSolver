executables = solver
objectFiles = main.o GameBoard.o

all: ${executables}

solver: ${objectFiles}
	g++ -o solver ${objectFiles}

%.o: %.cpp
	g++ -c $^

# Czyszczenie
exe: ${executables}
	rm -f *.o *.~

clean:
	rm -f *.o *.~ ${executables}
