#include <iostream>
#include <vector>
#include <iomanip>

// --- Constants ---
const char PLAYER_X = 'X';
const char PLAYER_O = 'O';
const char EMPTY = ' ';
const char DRAW = 'D';

// --- Utility Functions ---
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printDivider() {
    std::cout << "  +---+---+---+   +---+---+---+   +---+---+---+\n";
}

// --- Game Logic Functions ---
bool checkWin(const std::vector<std::vector<char>>& board, char player) {
    for (int i = 0; i < 3; ++i) {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player))
            return true;
    }
    return (board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
           (board[0][2] == player && board[1][1] == player && board[2][0] == player);
}

bool isBoardFull(const std::vector<std::vector<char>>& board) {
    for (auto& row : board)
        for (char cell : row)
            if (cell == EMPTY) return false;
    return true;
}

// --- Display Function ---
void printBoard(const std::vector<std::vector<std::vector<std::vector<char>>>>& boards) {
    std::cout << "\n    0   1   2       0   1   2       0   1   2\n";
    printDivider();

    for (int bigRow = 0; bigRow < 3; ++bigRow) {
        for (int smallRow = 0; smallRow < 3; ++smallRow) {
            std::cout << "  ";
            for (int bigCol = 0; bigCol < 3; ++bigCol) {
                for (int smallCol = 0; smallCol < 3; ++smallCol) {
                    std::cout << "| " << boards[bigRow][bigCol][smallRow][smallCol] << " ";
                }
                std::cout << "|   ";
            }
            std::cout << "\n";
            printDivider();
        }
        std::cout << "\n";
    }
}

// --- Main Game ---
int main() {
    // 4D board: [big_row][big_col][small_row][small_col]
    std::vector gameBoards(3,
        std::vector(3, std::vector(3, std::vector<char>(3, EMPTY))));

    // Track small board winners
    std::vector bigBoardState(3, std::vector<char>(3, EMPTY));

    char currentPlayer = PLAYER_X;
    int targetBigRow = -1, targetBigCol = -1;
    bool gameOver = false;
    char winner = EMPTY;

    std::cout << "🎮 Welcome to Ultimate Tic-Tac-Toe!\n";
    std::cout << "Player X starts. Enter coordinates as:\n";
    std::cout << "bigRow bigCol smallRow smallCol\n\n";

    while (!gameOver) {
        clearScreen();
        printBoard(gameBoards);

        std::cout << "\nPlayer " << currentPlayer << "'s turn.\n";

        if (targetBigRow != -1 && bigBoardState[targetBigRow][targetBigCol] == EMPTY) {
            std::cout << "You must play in small board (" << targetBigRow << ", " << targetBigCol << ")\n";
        } else {
            std::cout << "You can play in any unfinished small board.\n";
            targetBigRow = -1;
            targetBigCol = -1;
        }

        int bRow, bCol, sRow, sCol;
        bool validMove = false;

        while (!validMove) {
            std::cout << "Enter your move: ";
            std::cin >> bRow >> bCol >> sRow >> sCol;

            if (std::cin.fail() ||
                bRow < 0 || bRow > 2 || bCol < 0 || bCol > 2 ||
                sRow < 0 || sRow > 2 || sCol < 0 || sCol > 2) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                std::cout << "❌ Invalid input. Try again.\n";
                continue;
            }

            if (targetBigRow != -1 && (bRow != targetBigRow || bCol != targetBigCol)) {
                std::cout << "❌ You must play in board (" << targetBigRow << ", " << targetBigCol << ").\n";
                continue;
            }

            if (bigBoardState[bRow][bCol] != EMPTY) {
                std::cout << "⚠️ This board is already won or drawn.\n";
                continue;
            }

            if (gameBoards[bRow][bCol][sRow][sCol] != EMPTY) {
                std::cout << "⚠️ That cell is already taken. Try again.\n";
                continue;
            }

            validMove = true;
        }

        gameBoards[bRow][bCol][sRow][sCol] = currentPlayer;

        // Check for win/draw in small board
        if (checkWin(gameBoards[bRow][bCol], currentPlayer))
            bigBoardState[bRow][bCol] = currentPlayer;
        else if (isBoardFull(gameBoards[bRow][bCol]))
            bigBoardState[bRow][bCol] = DRAW;

        // Check overall win
        if (checkWin(bigBoardState, currentPlayer)) {
            winner = currentPlayer;
            gameOver = true;
        } else if (isBoardFull(bigBoardState)) {
            gameOver = true;
        }

        // Set next target board
        targetBigRow = sRow;
        targetBigCol = sCol;

        if (!gameOver)
            currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
    }

    clearScreen();
    printBoard(gameBoards);

    if (winner != EMPTY)
        std::cout << "\n🏆 Player " << winner << " wins the game!\n";
    else
        std::cout << "\n🤝 The game is a draw!\n";

    return 0;
}
