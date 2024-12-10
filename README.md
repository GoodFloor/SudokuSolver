# Generator i solver sudoku

Niniejszy program pozwala na wygenerowanie lub rozwiązanie podanej liczby plansz sudoku o dowolnym rozmiarze.

Autor: Łukasz Machnik

## Środowisko testowe

Program był uruchamiany i testowany wyłącznie w systemie Ubuntu 24.04.1. Program jest napisany w języku C++, do kompilacji wymaga kompilatora g++.

## Sposób użycia

Program można skompilować za pomocą polecenia [make](https://www.gnu.org/software/make/manual/make.html) (poniżej). Proces kompilacji zakłada istnienie katalogu src z kodem źródłowym oraz istnienie katalogu bin do którego trafią pliki pośrednie.

```bash
make
```

Aby zmienić rozmiar generowanych lub rozwiązywanych plansz należy edytować plik GameBoard.hpp znajdujący się w folderze z kodem źródłowym (./src). Poniższa linijka oznacza że program będzie obsługiwał plansze 9x9. Jeżeli chcemy używać na przykład plansz 4x4 to w miejsce cyfry 3 wpisujemy cyfrę 2. 

```C++
static const int SIZE = 3;
```

Po każdorazowej zmianie powyższej linijki należy zrekompilować cały porogram za pomocą kombinacji poniższych poleceń:

```bash
make clean
make
```

Kompilacja programu utworzy plik wykonalny o nazwie "demo". Aby uruchomić program wpisujemy nazwę tego pliku razem z dwoma argumentami: tryb w jakim ma się uruchomić program (do wyboru "solver" lub "generator") oraz liczbę generowanych (lub rozwiązywanych) plansz. 

Program uruchomiony w trybie generowania wypisze w kolejnych liniach w formie kompaktowej podaną liczbę plansz. Forma kompaktowa planszy sudoku jest ciągiem cyfr (dla większych plansz również liter) z których każda oznacza kolejne pole planszy czytając od lewej do prawej, od góry do dołu. Każde pole jest przedstawione za pomocą znaku symbolizującego wartość znajdującą się w tym polu lub '0' jeśli pole jest puste. Przykładowe uruchomienie programu w trybie generowania:

```bash
./demo generator 5
000000500030008200820750300000103000000070006054020000500000000200095008008400190
000830070040102000009000600002010064000009800401000020000080000090007006020004730
450060002000001000200008300000080160300006007008009030005040000000103900000000000
900007000006020090700035000030006015000000702080000000001000040800000500040052006
091302000080700004000900027000000600000031800210578000007000000140000200500000049
```

Program uruchomiony w trybie solvera rozwiązuje plansze wczytane ze standardowego wejścia. Program przyjmuje plansze w formie kompaktowej. Dla każdej planszy wypisuje ją przed i po rozwiązaniu. Przykład użycia solvera dla planszy 4x4:

```bash
./demo solver 1
0000030020040010
█████████████████
█   │   █   │   █
█   │   █   │   █
█   │   █   │   █
█───┼───█───┼───█
█   │   █   │   █
█   │ 3 █   │   █
█   │   █   │   █
█████████████████
█   │   █   │   █
█ 2 │   █   │ 4 █
█   │   █   │   █
█───┼───█───┼───█
█   │   █   │   █
█   │   █ 1 │   █
█   │   █   │   █
█████████████████

█████████████████
█   │   █   │   █
█ 1 │ 2 █ 4 │ 3 █
█   │   █   │   █
█───┼───█───┼───█
█   │   █   │   █
█ 4 │ 3 █ 2 │ 1 █
█   │   █   │   █
█████████████████
█   │   █   │   █
█ 2 │ 1 █ 3 │ 4 █
█   │   █   │   █
█───┼───█───┼───█
█   │   █   │   █
█ 3 │ 4 █ 1 │ 2 █
█   │   █   │   █
█████████████████
```

Można połączyć oba tryby działania za pomocą funkcji pipe:
```bash
./demo generator 1 | ./demo solver 1
█████████████████
█   │   █   │   █
█   │ 2 █   │   █
█   │   █   │   █
█───┼───█───┼───█
█   │   █   │   █
█   │   █ 4 │   █
█   │   █   │   █
█████████████████
█   │   █   │   █
█ 3 │   █   │   █
█   │   █   │   █
█───┼───█───┼───█
█   │   █   │   █
█   │   █   │ 3 █
█   │   █   │   █
█████████████████

█████████████████
█   │   █   │   █
█ 4 │ 2 █ 3 │ 1 █
█   │   █   │   █
█───┼───█───┼───█
█   │   █   │   █
█ 1 │ 3 █ 4 │ 2 █
█   │   █   │   █
█████████████████
█   │   █   │   █
█ 3 │ 1 █ 2 │ 4 █
█   │   █   │   █
█───┼───█───┼───█
█   │   █   │   █
█ 2 │ 4 █ 1 │ 3 █
█   │   █   │   █
█████████████████
```
