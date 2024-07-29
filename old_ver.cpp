// tic tac toe game

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// global const
const char X = 'X';
const char O = 'O';
const char EMPTY = ' ';
const char TIE = 'T';
const char NO_ONE = 'N';

// fucntions prototype

void instructions();
char askYesNo(std::string question);
int askNumner(std::string question, int high, int low = 0);
char humanPiece();
char opponent(char piece);
void displayBoard(const std::vector<char>& board);
char winner(const std::vector<char>& board);
bool isLegal(const std::vector<char>& board, int move);
int humanMove(const std::vector<char>& board, char human);
int computerMove(std::vector<char> board, char computer);
void announceWinner(char winner, char computer, char human);

int main()
{
    int move;
    const int NUM_SQUARES = 9;
    std::vector<char> board(NUM_SQUARES, EMPTY);
    instructions();
    char human = humanPiece();
    char computer = opponent(human);
    char turn = X;
    displayBoard(board);
    while (winner(board) == NO_ONE)
    {
        if (turn == human)
        {
            move = humanMove(board, human);
            board[move] = human;
        }
        else
        {
            move = computerMove(board, computer);
            board[move] = computer;
        }
        displayBoard(board);
        turn = opponent(turn);
    }
    announceWinner(winner(board), computer, human);
    return 0;
}

void instructions()
{
    std::cout << "Welcome to the ultimate man-machine showdown: Tic-Tac-Toe.\n";
    std::cout << "--where human brain is pit against silicon processor.\n\n";
    std::cout << "Make your move known by entering a number, 0 - 8. The number\n";
    std::cout << "corresponds to the disired board position, as illustrated:\n\n";
    std::cout << "0 | 1 | 2\n";
    std::cout << "----------\n";
    std::cout << "3 | 4 | 5\n";
    std::cout << "----------\n";
    std::cout << "6 | 7 | 8\n\n";
    std::cout << "Prepare yourself, human. The battle is about to begin.\n\n";
}

char askYesNo(std::string question)
{
    char responce;
    do
    {
        std::cout << question << "(y/n): ";
        std::cin >> responce;
    } while (responce != 'y' && responce != 'n');
    return responce;
}

int askNumner(std::string question, int high, int low)
{
    int number;
    do
    {
        std::cout << question << " (" << low << " - " << high << "): ";
        std::cin >> number;
    } while (number > high || number < low);
    return number;
}

char humanPiece()
{
    char go_first = askYesNo("Do you require the first move?");
    if (go_first == 'y')
    {
        std::cout << "\nThen take the first move. You will need it.\n";
        return X;
    }
    else
    {
        std::cout << "\nYour bravery will be undoing... i will go first.\n";
        return O;
    }
}

char opponent(char piece)
{
    if (piece == X)
    {
        return O;
    }
    else
    {
        return X;
    }
}

void displayBoard(const std::vector<char>& board)
{
    std::cout << "\n\t" << board[0] << " | " << board[1] << " | " << board[2];
    std::cout << "\n\t" << "-----------";
    std::cout << "\n\t" << board[3] << " | " << board[4] << " | " << board[5];
    std::cout << "\n\t" << "-----------";
    std::cout << "\n\t" << board[6] << " | " << board[7] << " | " << board[8];
    std::cout << "\n\n";
}

char winner(const std::vector<char>& board)
{
    // all possible win rows
    const int WINNING_ROWS[8][3] = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8},
        {0, 4, 8},
        {2, 4, 6}};

    const int TOTAL_ROWS = 8;
    // if in one of the winning rows already had all 3 signs (!= EMPTY) when the winner are announce
    for (int row = 0; row < TOTAL_ROWS; ++row)
    {
        if ((board[WINNING_ROWS[row][0]] != EMPTY) &&
            (board[WINNING_ROWS[row][0]] == board[WINNING_ROWS[row][1]]) &&
            (board[WINNING_ROWS[row][1]] == board[WINNING_ROWS[row][2]]))
        {
            return board[WINNING_ROWS[row][0]];
        }
    }
    // checking empty places
    if (std::count(board.begin(), board.end(), EMPTY) == 0)
        return TIE;
    // if no one wins game continue
    return NO_ONE;
}

inline bool isLegal(int move, const std::vector<char>& board)
{
    return (board[move] == EMPTY);
}

int humanMove(const std::vector<char>& board, char human)
{
    int move = askNumner("Where will you move?", (board.size() - 1));
    while (!isLegal(move, board))
    {
        std::cout << "\nThat square is already occupied, foolish human.\n";
        move = askNumner("Where will you move?", (board.size() - 1));
    }
    std::cout << "Fine...\n";
    return move;
}

int computerMove(std::vector<char> board, char computer)
{
    unsigned int move = 0;
    bool found = false;
    // if AI can win in next move it choose it
    while (!found && move < board.size())
    {
        if (isLegal(move, board))
        {
            board[move] = computer;
            found = winner(board) == computer;
            board[move] = EMPTY;
        }
        if (!found)
        {
            ++move;
        }
    }
    // else if human can win in the next move - block it
    if (!found)
    {
        move = 0;
        char human = opponent(computer);
        while (!found && move < board.size())
        {
            if (isLegal(move, board))
            {
                board[move] = human;
                found = winner(board) == human;
                board[move] = EMPTY;
            }
            if (!found)
            {
                ++move;
            }
        }
    }
    if (!found)
    {
        move = 0;
        unsigned int i = 0;
        const int BEST_MOVES[] = {4, 0, 2, 6, 8, 1, 3, 5, 7};
        // choose the best cell
        while (!found && i < board.size())
        {
            move = BEST_MOVES[i];
            if (isLegal(move, board))
            {
                found = true;
            }
            ++i;
        }
    }
    std::cout << "I shall take square number " << move << std::endl;
    return move;
}

void announceWinner(char winner, char computer, char human)
{
    if (winner == computer)
    {
        std::cout << winner << "'s won!\n";
    }
    else if (winner == human)
    {
        std::cout << winner << "'s won!\n";
    }
    else
    {
        std::cout << "It's a tie.\n";
    }
}