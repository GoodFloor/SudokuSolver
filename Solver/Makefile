BIN = ./bin/
SRC = ./src/

executables = solver generator
commonObjectFiles = GameBoard.o PossibilitiesBoard.o Solver.o Statistics.o SolverPolynomial.o
solverObjectFiles = ${commonObjectFiles} mainS.o
generatorObjectFiles = ${commonObjectFiles} Generator.o mainG.o

solverBin = $(addprefix $(BIN), $(solverObjectFiles))
generatorBin = $(addprefix $(BIN), $(generatorObjectFiles))


all: ${executables}

solver: ${solverBin}
	g++ -o solver ${solverBin}

generator: ${generatorBin}
	g++ -o generator ${generatorBin}

$(BIN)%.o: $(SRC)%.cpp
	g++ -o $@ -c $^

# Czyszczenie
exe: ${executables}
	rm -f $(BIN)* $(SRC)*.gch

clean:
	rm -f $(BIN)* $(SRC)*.gch ${executables}
