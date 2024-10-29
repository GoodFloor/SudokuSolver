executable = generator
objectFiles = main.o GameBoard.o

all: ${executable}

${executable}: ${objectFiles}
	g++ -o ${executable} ${objectFiles}

%.o: %.cpp
	g++ -c $^

# Czyszczenie
exe: ${executable}
	rm -f *.o *.~

clean:
	rm -f *.o *.~ ${executable}
