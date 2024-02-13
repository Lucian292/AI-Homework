//#include <iostream>
//#include <vector>
//#include <set>
//
//using namespace std;
//
//const int N = 9;
//const int EMPTY = 0;
//
//// Definim clasa pentru o variabilă Sudoku
//class Variable {
//public:
//    int row, col, value;
//    Variable(int r, int c, int v) : row(r), col(c), value(v) {}
//};
//
//// Funcție pentru afișarea tablei Sudoku
//void printBoard(vector<vector<int>>& board) {
//    for (int i = 0; i < N; i++) {
//        if (i % 3 == 0 && i != 0) {
//			cout << "------|-------|-------" << endl;
//		}
//        for (int j = 0; j < N; j++) {
//            if (j % 3 == 0 && j != 0) {
//                cout << "| ";
//            }
//            cout << board[i][j] << " ";
//        }
//        cout << endl;
//    }
//}
//
//// Funcție pentru verificarea dacă o valoare este validă într-o căsuță
//bool isValid(vector<vector<int>>& board, int row, int col, int value) {
//    // Verificăm constrângerile pentru linii și coloane
//    for (int i = 0; i < N; i++) {
//        if (board[row][i] == value || board[i][col] == value) {
//            return false;
//        }
//    }
//
//    // Verificăm constrângerile pentru regiunea 3x3
//    int boxRow = row - row % 3;
//    int boxCol = col - col % 3;
//    for (int i = 0; i < 3; i++) {
//        for (int j = 0; j < 3; j++) {
//            if (board[boxRow + i][boxCol + j] == value) {
//                return false;
//            }
//        }
//    }
//
//    return true;
//}
//
//// Funcție pentru găsirea următoarei variabile neatribuite
//Variable nextUnassignedVariable(vector<vector<int>>& board) {
//    for (int i = 0; i < N; i++) {
//        for (int j = 0; j < N; j++) {
//            if (board[i][j] == EMPTY) {
//                return Variable(i, j, EMPTY);
//            }
//        }
//    }
//    return Variable(-1, -1, -1);
//}
//
//
////bool forwardChecking(vector<vector<int>>& board, int row, int col, int value) {
////    for (int i = 0; i < N; i++) {
////        if (i != col && board[row][i] == EMPTY) {
////            if (!isValid(board, row, i, value)) {
////                return false;
////            }
////        }
////        if (i != row && board[i][col] == EMPTY) {
////            if (!isValid(board, i, col, value)) {
////                return false;
////            }
////        }
////    }
////
////    int boxRow = row - row % 3;
////    int boxCol = col - col % 3;
////    for (int i = boxRow; i < boxRow + 3; i++) {
////        for (int j = boxCol; j < boxCol + 3; j++) {
////            if (i != row && j != col && board[i][j] == EMPTY) {
////                if (!isValid(board, i, j, value)) {
////                    return false;
////                }
////            }
////        }
////    }
////
////    return true;
////}
//
//bool solveSudoku(vector<vector<int>>& board) {
//    Variable var = nextUnassignedVariable(board);
//    if (var.row == -1) {
//        return true;  // Toate variabilele sunt atribuite
//    }
//
//    // Ordine MRV (Minimum Remaining Values) - alege variabila cu cel mai mic domeniu
//    int minDomainSize = N + 1;
//    Variable nextVar = Variable(-1, -1, -1);
//    for (int i = 0; i < N; i++) {
//        for (int j = 0; j < N; j++) {
//            if (board[i][j] == EMPTY) {
//                set<int> domain;
//                for (int value = 1; value <= N; value++) {
//                    if (isValid(board, i, j, value)) {
//                        domain.insert(value);
//                    }
//                }
//                int domainSize = domain.size();
//                if (domainSize < minDomainSize) {
//                    minDomainSize = domainSize;
//                    nextVar = Variable(i, j, EMPTY);
//                }
//            }
//        }
//    }
//
//    int row = nextVar.row;
//    int col = nextVar.col;
//
//    for (int value = 1; value <= N; value++) {
//        if (isValid(board, row, col, value)/* && forwardChecking(board, row, col, value)*/) {
//            board[row][col] = value;
//
//            if (solveSudoku(board)) {
//                return true;
//            }
//
//            board[row][col] = EMPTY;
//        }
//    }
//
//    return false;
//}
//
//// Funcție pentru verificarea arc-consistency între două variabile
//bool arcConsistency(vector<vector<int>>& board, int row1, int col1, int row2, int col2) {
//    set<int> validValues;
//    for (int value1 = 1; value1 <= N; value1++) {
//        if (isValid(board, row1, col1, value1)) {
//            bool value2Valid = false;
//            for (int value2 = 1; value2 <= N; value2++) {
//                if (isValid(board, row2, col2, value2) && value2 != value1) {
//                    value2Valid = true;
//                    break;
//                }
//            }
//            if (!value2Valid) {
//                validValues.insert(value1);
//            }
//        }
//    }
//
//    // Actualizăm domeniul variabilei 1
//    if (!validValues.empty()) {
//        board[row1][col1] = *validValues.begin();
//        return true;
//    }
//
//    return false;
//}
//
//// Funcție pentru aplicarea Arc Consistency pe toate restricțiile din Sudoku
//void applyArcConsistency(vector<vector<int>>& board) {
//    for (int i = 0; i < N; i++) {
//        for (int j = 0; j < N; j++) {
//            // Verificăm restricțiile pentru căsuțele vecine
//            for (int k = 0; k < N; k++) {
//                if (k != j) {
//                    arcConsistency(board, i, j, i, k);
//                }
//                if (k != i) {
//                    arcConsistency(board, i, j, k, j);
//                }
//            }
//            // Verificăm restricțiile pentru căsuțele din aceeași regiune 3x3
//            int boxRow = i - i % 3;
//            int boxCol = j - j % 3;
//            for (int r = boxRow; r < boxRow + 3; r++) {
//                for (int c = boxCol; c < boxCol + 3; c++) {
//                    if (r != i && c != j) {
//                        arcConsistency(board, i, j, r, c);
//                    }
//                }
//            }
//        }
//    }
//}
//
//
//int main() {
//    vector<vector<int>> board1 = {
//        {5, 3, 0, 0, 7, 0, 0, 0, 0},
//        {6, 0, 0, 1, 9, 5, 0, 0, 0},
//        {0, 9, 8, 0, 0, 0, 0, 6, 0},
//        {8, 0, 0, 0, 6, 0, 0, 0, 3},
//        {4, 0, 0, 8, 0, 3, 0, 0, 1},
//        {7, 0, 0, 0, 2, 0, 0, 0, 6},
//        {0, 6, 0, 0, 0, 0, 2, 8, 0},
//        {0, 0, 0, 4, 1, 9, 0, 0, 5},
//        {0, 0, 0, 0, 8, 0, 0, 7, 9}
//    };
//
//    vector<vector<int>> board2 = {
//        {0, 0, 0, 0, 5, 0, 0, 7, 0},
//        {0, 0, 6, 0, 0, 0, 2, 0, 0},
//        {9, 0, 0, 4, 0, 0, 0, 1, 0},
//        {0, 0, 0, 7, 0, 4, 0, 0, 0},
//        {0, 0, 1, 0, 0, 0, 4, 0, 0},
//        {0, 3, 0, 0, 0, 6, 0, 0, 9},
//        {0, 5, 0, 0, 0, 9, 0, 0, 2},
//        {0, 0, 3, 0, 0, 0, 1, 0, 0},
//        {0, 8, 0, 0, 7, 0, 0, 0, 0}
//    };
//
//
//    if (solveSudoku(board1)) {
//        cout << "Solutie gasita:" << endl;
//        printBoard(board1);
//    }
//    else {
//        cout << "Nu exista solutie." << endl;
//    }
//
//    if (solveSudoku(board2)) {
//	cout << "Solutie gasita:" << endl;
//	printBoard(board2);
//    }
//    else {
//	cout << "Nu exista solutie." << endl;
//    }
//
//    board1 = {
//        {5, 3, 0, 0, 7, 0, 0, 0, 0},
//        {6, 0, 0, 1, 9, 5, 0, 0, 0},
//        {0, 9, 8, 0, 0, 0, 0, 6, 0},
//        {8, 0, 0, 0, 6, 0, 0, 0, 3},
//        {4, 0, 0, 8, 0, 3, 0, 0, 1},
//        {7, 0, 0, 0, 2, 0, 0, 0, 6},
//        {0, 6, 0, 0, 0, 0, 2, 8, 0},
//        {0, 0, 0, 4, 1, 9, 0, 0, 5},
//        {0, 0, 0, 0, 8, 0, 0, 7, 9}
//    };
//
//    board2 = {
//        {0, 0, 0, 0, 5, 0, 0, 7, 0},
//        {0, 0, 6, 0, 0, 0, 2, 0, 0},
//        {9, 0, 0, 4, 0, 0, 0, 1, 0},
//        {0, 0, 0, 7, 0, 4, 0, 0, 0},
//        {0, 0, 1, 0, 0, 0, 4, 0, 0},
//        {0, 3, 0, 0, 0, 6, 0, 0, 9},
//        {0, 5, 0, 0, 0, 9, 0, 0, 2},
//        {0, 0, 3, 0, 0, 0, 1, 0, 0},
//        {0, 8, 0, 0, 7, 0, 0, 0, 0}
//    };
//
//    applyArcConsistency(board1);
//
//    if (solveSudoku(board1)) {
//        cout << "Solutie gasita:" << endl;
//        printBoard(board1);
//    }
//    else {
//        cout << "Nu exista solutie." << endl;
//    }
//
//    applyArcConsistency(board2);
//
//    if (solveSudoku(board2)) {
//		cout << "Solutie gasita:" << endl;
//		printBoard(board2);
//	}
//    else {
//		cout << "Nu exista solutie." << endl;
//	}
//
//    return 0;
//}
