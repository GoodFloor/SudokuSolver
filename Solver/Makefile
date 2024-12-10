BIN = ./bin/
SRC = ./src/

executables = demo
objectFiles = GameBoard.o PossibilitiesBoard.o Solver.o Statistics.o SolverPolynomial.o Generator.o demo.o

demoBin = $(addprefix $(BIN), $(objectFiles))

all: ${executables}

demo: ${demoBin}
	g++ -o demo ${demoBin}

$(BIN)%.o: $(SRC)%.cpp
	g++ -o $@ -c $^

# Czyszczenie
exe: ${executables}
	rm -f $(BIN)* $(SRC)*.gch

clean:
	rm -f $(BIN)* $(SRC)*.gch ${executables}
