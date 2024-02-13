#include <iostream>
#include <vector>
#include <set>
#include <algorithm>  // Adăugat pentru a folosi std::find

const int N = 9;

struct Variable {
    int row;
    int col;
};

std::set<int> domain = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

std::vector<std::vector<std::vector<int>>> sudokuBoards = {
    {
        {8, 4, 0, 0, 5, 0, -1, 0, 0},
        {3, 0, 0, 6, 0, 8, 0, 4, 0},
        {0, 0, -1, 4, 0, 9, 0, 0, -1},
        {0, 2, 3, 0, -1, 0, 9, 8, 0},
        {1, 0, 0, -1, 0, -1, 0, 0, 4},
        {0, 9, 8, 0, -1, 0, 1, 6, 0},
        {-1, 0, 0, 5, 0, 3, -1, 0, 0},
        {0, 3, 0, 1, 0, 6, 0, 0, 7},
        {0, 0, -1, 0, 2, 0, 0, 1, 3}
    }
};

int currentIndex = 0; // Indexul tablăi curente.

void printSudokuBoard(const std::vector<std::vector<int>>& board) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

bool isValid(int num, int row, int col, const std::vector<std::vector<int>>& board) {
    // Verificăm dacă numărul este în domeniu și nu există conflict cu alte celule pe aceeași linie și coloană
    if (domain.count(num) == 0) {
        return false;
    }

    for (int i = 0; i < N; i++) {
        if (board[row][i] == num || board[i][col] == num) {
            return false;
        }
    }

    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            if (board[i][j] == num) {
                return false;
            }
        }
    }

    if (num == -1) {
        // Verificăm dacă trebuie să fie un număr par
        if (board[row][col] % 2 != 0) {
            return false;
        }
    }

    return true;
}

Variable getNextVariableMRV(const std::vector<std::vector<int>>& board) {
    Variable var;
    var.row = -1;
    var.col = -1;
    int minDomainSize = N + 1;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 0) {
                int domainSize = 0;
                for (int num : domain) {
                    if (isValid(num, i, j, board)) {
                        domainSize++;
                    }
                }
                if (domainSize < minDomainSize) {
                    minDomainSize = domainSize;
                    var.row = i;
                    var.col = j;
                }
            }
        }
    }

    return var;
}

bool solveSudoku(std::vector<std::vector<int>>& board) {
    Variable var = getNextVariableMRV(board);

    if (var.row == -1) {
        return true;
    }

    for (int num : domain) {
        if (isValid(num, var.row, var.col, board)) {
            board[var.row][var.col] = num;

            // Utilizăm Forward Checking pentru a verifica domeniile viitoarelor celule
            std::set<int> originalDomain = domain;
            domain.erase(num);
            bool consistent = true;

            for (int i = 0; i < N; i++) {
                if (board[var.row][i] == 0) {
                    Variable futureVar;
                    futureVar.row = var.row;
                    futureVar.col = i;

                    if (std::find(domain.begin(), domain.end(), -1) != domain.end()) {
                        // Dacă există celule pentru numere pare în domeniu, verificăm restricția
                        domain.erase(-1);
                        if (num % 2 != 0) {
                            consistent = false;
                            break;
                        }
                    }

                    for (int x : domain) {
                        if (!isValid(x, futureVar.row, futureVar.col, board)) {
                            domain.erase(x);
                        }
                    }
                    if (domain.empty()) {
                        consistent = false;
                        break;
                    }
                }
            }

            if (consistent && solveSudoku(board)) {
                return true;
            }

            board[var.row][var.col] = 0;
            domain = originalDomain;
        }
    }

    return false;
}

int main() {
    //while (currentIndex < sudokuBoards.size()) {
        std::cout << "Sudoku initial:" << std::endl;
        printSudokuBoard(sudokuBoards[currentIndex]);

        if (solveSudoku(sudokuBoards[currentIndex])) {
            std::cout << "Sudoku rezolvat:" << std::endl;
            printSudokuBoard(sudokuBoards[currentIndex]);
        }
        else {
            std::cout << "Nu există o soluție pentru Sudoku-ul dat." << std::endl;
        }

        currentIndex++; // Trecem la următoarea tablă
    //}

    return 0;
}