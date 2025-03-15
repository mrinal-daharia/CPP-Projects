#include <iostream>
#include <limits>

using namespace std;

const char HUMAN = 'X';
const char AI = 'O';
const char EMPTY = ' ';

void printBoard(char board[3][3]) {
    cout << "\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) {
                cout << " " << (i * 3 + j + 1) << " ";
            } else {
                cout << " " << board[i][j] << " ";
            }
            if (j < 2) cout << "|";
        }
        cout << "\n";
        if (i < 2) cout << "-----------\n";
    }
    cout << "\n";
}

bool isMovesLeft(char board[3][3]) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == EMPTY)
                return true;
    return false;
}

int evaluate(char board[3][3]) {
    for (int row = 0; row < 3; row++) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
            if (board[row][0] == AI) return +10;
            else if (board[row][0] == HUMAN) return -10;
        }
    }
    
    for (int col = 0; col < 3; col++) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
            if (board[0][col] == AI) return +10;
            else if (board[0][col] == HUMAN) return -10;
        }
    }
    
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == AI) return +10;
        else if (board[0][0] == HUMAN) return -10;
    }
    
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == AI) return +10;
        else if (board[0][2] == HUMAN) return -10;
    }
    
    return 0;
}

int minimax(char board[3][3], bool isMax) {
    int score = evaluate(board);
    if (score == 10 || score == -10) return score;
    if (!isMovesLeft(board)) return 0;
    
    if (isMax) {
        int best = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = AI;
                    best = max(best, minimax(board, false));
                    board[i][j] = EMPTY;
                }
            }
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = HUMAN;
                    best = min(best, minimax(board, true));
                    board[i][j] = EMPTY;
                }
            }
        }
        return best;
    }
}

pair<int, int> findBestMove(char board[3][3]) {
    int bestVal = -1000;
    pair<int, int> bestMove = {-1, -1};
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = AI;
                int moveVal = minimax(board, false);
                board[i][j] = EMPTY;
                
                if (moveVal > bestVal) {
                    bestMove = {i, j};
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}

void playGame() {
    char board[3][3] = {
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY}
    };
    
    int move;
    while (isMovesLeft(board)) {
        printBoard(board);
        cout << "Enter your move (1-9): ";
        cin >> move;
        int x = (move - 1) / 3;
        int y = (move - 1) % 3;
        if (board[x][y] != EMPTY) {
            cout << "Invalid move! Try again.\n";
            continue;
        }
        board[x][y] = HUMAN;
        
        if (evaluate(board) == -10) {
            printBoard(board);
            cout << "You win!\n";
            return;
        }
        
        if (!isMovesLeft(board)) break;
        
        cout << "AI is making a move...\n";
        pair<int, int> aiMove = findBestMove(board);
        board[aiMove.first][aiMove.second] = AI;
        
        if (evaluate(board) == 10) {
            printBoard(board);
            cout << "AI wins!\n";
            return;
        }
    }
    
    printBoard(board);
    cout << "It's a draw!\n";
}

int main() {
    cout << "Welcome to Tic Tac Toe (Human vs AI)\n";
    playGame();
    return 0;
}