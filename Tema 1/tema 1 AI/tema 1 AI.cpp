#include <iostream>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <string>
#include <sstream>
#include <queue>
#include <algorithm>
#include <cmath>

using namespace std;

const int SIZE = 3; // Dimensiunea matricei

struct State {
    vector<vector<int>> board;

    string stateToString() {
        string stateStr = "";
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                stateStr += to_string(board[i][j]) + " ";
            }
        }
        return stateStr;
    }

    // Constructor pentru a inițializa starea cu o matrice dată
    State(const vector<int>& cells) {
        board = vector<vector<int>>(SIZE, vector<int>(SIZE, 0));
        int index = 0;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = cells[index++];
            }
        }
    }

    // Funcție pentru a verifica dacă starea este finală
    bool isFinalState() {
        int value = 1;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == 0) {
                    // Dacă celula curentă este 0, continuă verificarea pentru celelalte valori crescătoare
                    //value++;
                    continue;
                }
                if (board[i][j] != value % (SIZE * SIZE)) {
                    return false;
                }
                value++;
            }
        }
        return true;
    }

    // Funcție pentru a verifica dacă o mutare este validă
    bool isValidMove(int i, int j) {
        if (i >= 0 && i < SIZE && j >= 0 && j < SIZE) {
            return true;
        }
        return false;
    }

    // Funcție pentru a aplica o mutare
    State makeMove(int i, int j, int emptyI, int emptyJ) {
        State newState = *this;
        swap(newState.board[i][j], newState.board[emptyI][emptyJ]);
        return newState;
    }
};

// Funcție pentru a afișa starea
void printState(const State& state) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << state.board[i][j] << " ";
        }
        cout << endl;
    }
}

// Funcție pentru a verifica dacă o stare este într-o mulțime de stări
bool isStateInSet(const string& stateStr, const unordered_set<string>& stateSet) {
    return stateSet.find(stateStr) != stateSet.end();
}

// Funcție pentru a efectua o căutare DFS cu o adâncime maximă
bool depthLimitedSearch(State state, int depth, int& currentDepth, int& moves, unordered_set<string>& visited) {
    if (depth == 0) {
        if (state.isFinalState()) {
            printState(state);
            cout << "Numarul de mutari: " << moves << endl;
            return true;
        }
        return false;
    }

    int emptyI, emptyJ;
    bool found = false;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (state.board[i][j] == 0) {
                emptyI = i;
                emptyJ = j;
                found = true;
                break;
            }
        }
        if (found) break;
    }

    bool solutionFound = false;
    int movesArray[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
    for (int move = 0; move < 4; move++) {
        int i = emptyI + movesArray[move][0];
        int j = emptyJ + movesArray[move][1];

        if (state.isValidMove(i, j)) {
            State newState = state.makeMove(i, j, emptyI, emptyJ);

            string stateStr = newState.stateToString();
            if (!isStateInSet(stateStr, visited)) {
                visited.insert(stateStr);
                moves++;
                solutionFound = depthLimitedSearch(newState, depth - 1, currentDepth, moves, visited);
                if (solutionFound) return true;
                visited.erase(stateStr);
                moves--;
            }
        }
    }
    return false;
}

// Funcție pentru a efectua o căutare IDDFS
void iterativeDeepeningDFS(State initialState) {
    int currentDepth = 1;
    int moves = 0;
    while (true) {
        unordered_set<string> visited;
        visited.insert(initialState.stateToString());

        if (depthLimitedSearch(initialState, currentDepth, currentDepth, moves, visited)) {
            //cout << "Număr total de mutări: " << moves << endl;
            return;
        }

        currentDepth++;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Euristica distanței Manhattan
int heuristicManhattan(const State& state) {
    int distance = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (state.board[i][j] == 0) continue;
            int targetI = (state.board[i][j] - 1) / SIZE;
            int targetJ = (state.board[i][j] - 1) % SIZE;
            distance += abs(i - targetI) + abs(j - targetJ);
        }
    }
    return distance;
}

// Euristica distanței Hamming
int heuristicHamming(const State& state) {
    int misplaced = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (state.board[i][j] == 0) continue;
            if (state.board[i][j] != (i * SIZE + j + 1)) {
                misplaced++;
            }
        }
    }
    return misplaced;
}

// O altă euristică simplă
int heuristicSimple(const State& state) {
    int value = 1;
    int misplaced = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (state.board[i][j] != 0 && state.board[i][j] != value) {
                misplaced++;
            }
            value++;
        }
    }
    return SIZE * SIZE - misplaced; // Numărul corect așezat
}

// Structură pentru prioritizarea stărilor în coadă în funcția Greedy
struct GreedyNode {
    State state;
    int cost;

    GreedyNode(const State& s, int h) : state(s), cost(h) {}

    bool operator<(const GreedyNode& other) const {
        return cost > other.cost; // Sortare în ordine descrescătoare
    }
};

// Funcție pentru a efectua căutarea Greedy
bool greedySearch(const State& initialState, int (*heuristic)(const State&)) {
    priority_queue<GreedyNode> openSet;
    unordered_set<string> visited;
    int moves = 0;

    openSet.push(GreedyNode(initialState, heuristic(initialState)));

    while (!openSet.empty()) {
        State currentState = openSet.top().state;
        openSet.pop();
        moves++;
        if (currentState.isFinalState()) {
            printState(currentState);
            cout << "Numarul de mutari: " << moves << endl;
            return true;
        }

        int emptyI, emptyJ;
        bool found = false;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (currentState.board[i][j] == 0) {
                    emptyI = i;
                    emptyJ = j;
                    found = true;
                    break;
                }
            }
            if (found) break;
        }

        int movesArray[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
        for (int move = 0; move < 4; move++) {
            int i = emptyI + movesArray[move][0];
            int j = emptyJ + movesArray[move][1];

            if (currentState.isValidMove(i, j)) {
                State newState = currentState.makeMove(i, j, emptyI, emptyJ);

                string stateStr = newState.stateToString();
                if (visited.find(stateStr) == visited.end()) {
                    visited.insert(stateStr);
                    openSet.push(GreedyNode(newState, heuristic(newState)));
                }
            }
        }
    }

    return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



int main() {

    vector<int> initial1 = { 2, 5, 3, 1, 0, 6, 4, 7, 8 };
    vector<int> initial2 = { 2, 7, 5, 0, 8, 4, 3, 1, 6 };
    vector<int> initial3 = { 8, 6, 7, 2, 5, 4, 0, 3, 1 };

    State initialState1(initial1);
    State initialState2(initial2);
    State initialState3(initial3);

    auto start = chrono::high_resolution_clock::now();
    iterativeDeepeningDFS(initialState1);
    auto end = chrono::high_resolution_clock::now();
    cout << "Timpul de executie pentru instanta 1 (IDDFS): " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

    //cout << "Greedy Search with Manhattan Distance Heuristic:" << endl;
    start = chrono::high_resolution_clock::now();
    greedySearch(initialState1, heuristicManhattan);
    end = chrono::high_resolution_clock::now();
    cout << "Timpul de executie pentru instanta 1 (Greedy - Manhattan): " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

    //cout << "Greedy Search with Hamming Distance Heuristic:" << endl;
    start = chrono::high_resolution_clock::now();
    greedySearch(initialState1, heuristicHamming);
    end = chrono::high_resolution_clock::now();
    cout << "Timpul de executie pentru instanta 1 (Greedy - Hamming): " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

    //dureaza mai mult
    //cout << "Greedy Search with Simple Heuristic:" << endl;
    start = chrono::high_resolution_clock::now();
    greedySearch(initialState1, heuristicSimple);
    end = chrono::high_resolution_clock::now();
    cout << "Timpul de executie pentru instanta 1 (Greedy - Simple): " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    start = chrono::high_resolution_clock::now();
    iterativeDeepeningDFS(initialState2);
    end = chrono::high_resolution_clock::now();
    cout << "Timpul de executie pentru instanta 2 (IDDFS): " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

    start = chrono::high_resolution_clock::now();
    greedySearch(initialState2, heuristicManhattan);
    end = chrono::high_resolution_clock::now();
    cout << "Timpul de executie pentru instanta 2 (Greedy - Manhattan): " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

    start = chrono::high_resolution_clock::now();
    greedySearch(initialState2, heuristicHamming);
    end = chrono::high_resolution_clock::now();
    cout << "Timpul de executie pentru instanta 2 (Greedy - Hamming): " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

    start = chrono::high_resolution_clock::now();
    greedySearch(initialState2, heuristicSimple);
    end = chrono::high_resolution_clock::now();
    cout << "Timpul de executie pentru instanta 2 (Greedy - Simple): " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*
    start = chrono::high_resolution_clock::now();
    iterativeDeepeningDFS(initialState3);
    end = chrono::high_resolution_clock::now();
    cout << "Timpul de executie pentru instanta 3 (IDDFS): " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
    */
    start = chrono::high_resolution_clock::now();
    greedySearch(initialState3, heuristicManhattan);
    end = chrono::high_resolution_clock::now();
    cout << "Timpul de executie pentru instanta 3 (Greedy - Manhattan): " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
    
    start = chrono::high_resolution_clock::now();
    greedySearch(initialState3, heuristicHamming);
    end = chrono::high_resolution_clock::now();
    cout << "Timpul de executie pentru instanta 3 (Greedy - Hamming): " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
   
    start = chrono::high_resolution_clock::now();
    greedySearch(initialState3, heuristicSimple);
    end = chrono::high_resolution_clock::now();
    cout << "Timpul de executie pentru instanta 3 (Greedy - Simple): " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

    return 0;
}
