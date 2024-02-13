//#include <iostream>
//#include <vector>
//#include <unordered_set>
//#include <chrono>
//#include <string>
//#include <sstream>
//
//using namespace std;
//
//const int SIZE = 3; // Dimensiunea matricei
//
//struct State {
//    vector<vector<int>> board;
//
//    string stateToString() {
//        string stateStr = "";
//        for (int i = 0; i < SIZE; i++) {
//            for (int j = 0; j < SIZE; j++) {
//                stateStr += to_string(board[i][j]) + " ";
//            }
//        }
//        return stateStr;
//    }
//
//    // Constructor pentru a inițializa starea cu o matrice dată
//    State(const vector<int>& cells) {
//        board = vector<vector<int>>(SIZE, vector<int>(SIZE, 0));
//        int index = 0;
//        for (int i = 0; i < SIZE; i++) {
//            for (int j = 0; j < SIZE; j++) {
//                board[i][j] = cells[index++];
//            }
//        }
//    }
//
//    // Funcție pentru a verifica dacă starea este finală
//    bool isFinalState() {
//        int value = 1;
//        for (int i = 0; i < SIZE; i++) {
//            for (int j = 0; j < SIZE; j++) {
//                if (board[i][j] == 0) {
//                    // Dacă celula curentă este 0, continuă verificarea pentru celelalte valori crescătoare
//                    //value++;
//                    continue;
//                }
//                if (board[i][j] != value % (SIZE * SIZE)) {
//                    return false;
//                }
//                value++;
//            }
//        }
//        return true;
//    }
//
//    // Funcție pentru a verifica dacă o mutare este validă
//    bool isValidMove(int i, int j) {
//        if (i >= 0 && i < SIZE && j >= 0 && j < SIZE) {
//            return true;
//        }
//        return false;
//    }
//
//    // Funcție pentru a aplica o mutare
//    State makeMove(int i, int j, int emptyI, int emptyJ) {
//        State newState = *this;
//        swap(newState.board[i][j], newState.board[emptyI][emptyJ]);
//        return newState;
//    }
//};
//
//// Funcție pentru a afișa starea
//void printState(const State& state) {
//    for (int i = 0; i < SIZE; i++) {
//        for (int j = 0; j < SIZE; j++) {
//            cout << state.board[i][j] << " ";
//        }
//        cout << endl;
//    }
//}
//
//// Funcție pentru a verifica dacă o stare este într-o mulțime de stări
//bool isStateInSet(const string& stateStr, const unordered_set<string>& stateSet) {
//    return stateSet.find(stateStr) != stateSet.end();
//}
//
//// Funcție pentru a efectua o căutare DFS cu o adâncime maximă
//bool depthLimitedSearch(State state, int depth, int& currentDepth, unordered_set<string>& visited) {
//    if (depth == 0) {
//        if (state.isFinalState()) {
//            printState(state);
//            return true;
//        }
//        return false;
//    }
//
//    int emptyI, emptyJ;
//    bool found = false;
//    for (int i = 0; i < SIZE; i++) {
//        for (int j = 0; j < SIZE; j++) {
//            if (state.board[i][j] == 0) {
//                emptyI = i;
//                emptyJ = j;
//                found = true;
//                break;
//            }
//        }
//        if (found) break;
//    }
//
//    bool solutionFound = false;
//    int moves[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
//    for (int move = 0; move < 4; move++) {
//        int i = emptyI + moves[move][0];
//        int j = emptyJ + moves[move][1];
//
//        if (state.isValidMove(i, j)) {
//            State newState = state.makeMove(i, j, emptyI, emptyJ);
//
//            string stateStr = newState.stateToString();
//            if (!isStateInSet(stateStr, visited)) {
//                visited.insert(stateStr);
//                solutionFound = depthLimitedSearch(newState, depth - 1, currentDepth, visited);
//                if (solutionFound) return true;
//                visited.erase(stateStr);
//            }
//        }
//    }
//    return false;
//}
//
//// Funcție pentru a efectua o căutare IDDFS
//void iterativeDeepeningDFS(State initialState) {
//    int currentDepth = 1;
//    while (true) {
//        unordered_set<string> visited;
//        visited.insert(initialState.stateToString());
//
//        cout << "Depth: " << currentDepth << endl;
//
//        if (depthLimitedSearch(initialState, currentDepth, currentDepth, visited)) {
//            cout << "Soluția a fost găsită!" << endl;
//            return;
//        }
//
//        currentDepth++;
//    }
//}
//
//int main() {
//    
//    vector<int> initial1 = { 2, 5, 3, 1, 0, 6, 4, 7, 8 };
//    vector<int> initial2 = { 2, 7, 5, 0, 8, 4, 3, 1, 6 };
//    vector<int> initial3 = { 8, 6, 7, 2, 5, 4, 0, 3, 1 };
//
//    State initialState1(initial1);
//    State initialState2(initial2);
//    State initialState3(initial3);
//
//    auto start = chrono::high_resolution_clock::now();
//    iterativeDeepeningDFS(initialState1);
//    auto end = chrono::high_resolution_clock::now();
//    cout << "Timpul de executie pentru instanta 1: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
//
//    start = chrono::high_resolution_clock::now();
//    iterativeDeepeningDFS(initialState2);
//    end = chrono::high_resolution_clock::now();
//    cout << "Timpul de executie pentru instanta 2: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
//
//    start = chrono::high_resolution_clock::now();
//    iterativeDeepeningDFS(initialState3);
//    end = chrono::high_resolution_clock::now();
//    cout << "Timpul de executie pentru instanta 3: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
//
//    return 0;
//}
