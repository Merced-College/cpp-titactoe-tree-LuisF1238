#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

const char EMPTY = ' ';

class GameState {
private:
    vector<char> board; // 9 cells for Tic-Tac-Toe

public:
    GameState() : board(9, EMPTY) {}
    GameState(const vector<char>& b) : board(b) {}

    void printBoard() const {
        for (int i = 0; i < 9; ++i) {
            cout << (board[i] == EMPTY ? '-' : board[i]) << " ";
            if ((i + 1) % 3 == 0) cout << endl;
        }
    }

    bool isFull() const {
        for (char c : board)
            if (c == EMPTY) return false;
        return true;
    }

    bool isGameOver() const {
        return checkWinner() != EMPTY || isFull();
    }

    char checkWinner() const {
        const int winPatterns[8][3] = {
            {0,1,2}, {3,4,5}, {6,7,8},
            {0,3,6}, {1,4,7}, {2,5,8},
            {0,4,8}, {2,4,6}
        };

        for (auto& pattern : winPatterns) {
            if (board[pattern[0]] != EMPTY &&
                board[pattern[0]] == board[pattern[1]] &&
                board[pattern[1]] == board[pattern[2]]) {
                return board[pattern[0]];
            }
        }
        return EMPTY;
    }

    vector<int> getAvailableMoves() const {
        vector<int> moves;
        for (int i = 0; i < 9; ++i)
            if (board[i] == EMPTY) moves.push_back(i);
        return moves;
    }

    GameState makeMove(int index, char player) const {
        vector<char> newBoard = board;
        newBoard[index] = player;
        return GameState(newBoard);
    }

    const vector<char>& getBoard() const {
        return board;
    }
};

class TicTacToeTree {
private:
    char humanPlayer, computerPlayer;

public:
    TicTacToeTree(char human, char computer)
        : humanPlayer(human), computerPlayer(computer) {}

    int minimax(const GameState& state, bool isMaximizing) {
        char winner = state.checkWinner();
        if (winner == computerPlayer) return 1;
        if (winner == humanPlayer)    return -1;
        if (state.isFull())           return 0;

        if (isMaximizing) {
            int bestScore = numeric_limits<int>::min();
            for (int move : state.getAvailableMoves()) {
                GameState newState = state.makeMove(move, computerPlayer);
                int score = minimax(newState, false);
                bestScore = max(bestScore, score);
            }
            return bestScore;
        } else {
            int bestScore = numeric_limits<int>::max();
            for (int move : state.getAvailableMoves()) {
                GameState newState = state.makeMove(move, humanPlayer);
                int score = minimax(newState, true);
                bestScore = min(bestScore, score);
            }
            return bestScore;
        }
    }

    int findBestMove(const GameState& state) {
        int bestScore = numeric_limits<int>::min();
        vector<int> bestMoves;
        for (int move : state.getAvailableMoves()) {
            GameState newState = state.makeMove(move, computerPlayer);
            int score = minimax(newState, false);
            if (score > bestScore) {
                bestScore = score;
                bestMoves.clear();
                bestMoves.push_back(move);
            } else if (score == bestScore) {
                bestMoves.push_back(move);
            }
        }
        // Randomly choose among equally good moves
        return bestMoves[rand() % bestMoves.size()];
    }
};

void playGame() {
    GameState state;
    char human, computer;

    cout << "Choose your symbol (X/O): ";
    while (true) {
        cin >> human;
        human = toupper(human);
        if (human == 'X' || human == 'O') break;
        cout << "Invalid choice. Please enter X or O: ";
    }
    computer = (human == 'X') ? 'O' : 'X';
    cout << "You are " << human << " and the computer is " << computer << ". Let's start!\n";

    srand(static_cast<unsigned>(time(nullptr)));
    TicTacToeTree ai(human, computer);
    char currentPlayer = 'X';

    while (!state.isGameOver()) {
        state.printBoard();
        if (currentPlayer == human) {
            int move;
            cout << "Your turn (" << human << "). Enter your move (0-8): ";
            cin >> move;
            if (move >= 0 && move < 9 && state.getBoard()[move] == EMPTY) {
                state = state.makeMove(move, human);
                currentPlayer = computer;
            } else {
                cout << "Invalid move. Try again.\n";
            }
        } else {
            int move = ai.findBestMove(state);
            state = state.makeMove(move, computer);
            cout << "Computer (" << computer << ") plays at position " << move << endl;
            currentPlayer = human;
        }
    }

    state.printBoard();
    char winner = state.checkWinner();
    if      (winner == computer) cout << "Computer wins!\n";
    else if (winner == human)    cout << "You win!\n";
    else                           cout << "It's a draw!\n";
}

int main() {
    playGame();
    return 0;
}
