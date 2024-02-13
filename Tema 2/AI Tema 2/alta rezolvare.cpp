//#include <iostream>
//#include <vector>
//#include <set>
//
//const int N = 9;
//
//struct Variable {
//    int row;
//    int col;
//};
//
//std::set<int> domain = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//
//std::vector<std::vector<std::vector<int>>> sudokuBoards = {
//    {
//        {5, 3, 0, 0, 7, 0, 0, 0, 0},
//        {6, 0, 0, 1, 9, 5, 0, 0, 0},
//        {0, 9, 8, 0, 0, 0, 0, 6, 0},
//        {8, 0, 0, 0, 6, 0, 0, 0, 3},
//        {4, 0, 0, 8, 0, 3, 0, 0, 1},
//        {7, 0, 0, 0, 2, 0, 0, 0, 6},
//        {0, 6, 0, 0, 0, 0, 2, 8, 0},
//        {0, 0, 0, 4, 1, 9, 0, 0, 5},
//        {0, 0, 0, 0, 8, 0, 0, 7, 9}
//    },
//    {
//        {0, 0, 0, 0, 5, 0, 0, 7, 0},
//        {0, 0, 6, 0, 0, 0, 2, 0, 0},
//        {9, 0, 0, 4, 0, 0, 0, 1, 0},
//        {0, 0, 0, 7, 0, 4, 0, 0, 0},
//        {0, 0, 1, 0, 0, 0, 4, 0, 0},
//        {0, 3, 0, 0, 0, 6, 0, 0, 9},
//        {0, 5, 0, 0, 0, 9, 0, 0, 2},
//        {0, 0, 3, 0, 0, 0, 1, 0, 0},
//        {0, 8, 0, 0, 7, 0, 0, 0, 0}
//    }
//};
//
//int currentIndex = 0; // Indexul tablăi curente.
//
//void printSudokuBoard(const std::vector<std::vector<int>>& board) {
//    for (int i = 0; i < N; i++) {
//        for (int j = 0; j < N; j++) {
//            std::cout << board[i][j] << " ";
//        }
//        std::cout << std::endl;
//    }
//}
//
//bool isValid(int num, int row, int col, const std::vector<std::vector<int>>& board) {
//    for (int i = 0; i < N; i++) {
//        if (board[row][i] == num || board[i][col] == num) {
//            return false;
//        }
//    }
//
//    int startRow = (row / 3) * 3;
//    int startCol = (col / 3) * 3;
//    for (int i = startRow; i < startRow + 3; i++) {
//        for (int j = startCol; j < startCol + 3; j++) {
//            if (board[i][j] == num) {
//                return false;
//            }
//        }
//    }
//
//    return true;
//}
//
//Variable getNextVariableMRV(const std::vector<std::vector<int>>& board) {
//    Variable var;
//    var.row = -1;
//    var.col = -1;
//    int minDomainSize = N + 1;
//
//    for (int i = 0; i < N; i++) {
//        for (int j = 0; j < N; j++) {
//            if (board[i][j] == 0) {
//                int domainSize = 0;
//                for (int num : domain) {
//                    if (isValid(num, i, j, board)) {
//                        domainSize++;
//                    }
//                }
//                if (domainSize < minDomainSize) {
//                    minDomainSize = domainSize;
//                    var.row = i;
//                    var.col = j;
//                }
//            }
//        }
//    }
//
//    return var;
//}
//
//bool solveSudoku(std::vector<std::vector<int>>& board) {
//    Variable var = getNextVariableMRV(board);
//
//    if (var.row == -1) {
//        return true;
//    }
//
//    for (int num : domain) {
//        if (isValid(num, var.row, var.col, board)) {
//            board[var.row][var.col] = num;
//
//            if (solveSudoku(board)) {
//                return true;
//            }
//
//            board[var.row][var.col] = 0;
//        }
//    }
//
//    return false;
//}
//
//int main() {
//    while (currentIndex < sudokuBoards.size()) {
//        std::cout << "Sudoku initial:" << std::endl;
//        printSudokuBoard(sudokuBoards[currentIndex]);
//
//        if (solveSudoku(sudokuBoards[currentIndex])) {
//            std::cout << "Sudoku rezolvat:" << std::endl;
//            printSudokuBoard(sudokuBoards[currentIndex]);
//        }
//        else {
//            std::cout << "Nu există o soluție pentru Sudoku-ul dat." << std::endl;
//        }
//
//        currentIndex++; // Trecem la următoarea tablă
//    }
//
//    return 0;
//}
//
