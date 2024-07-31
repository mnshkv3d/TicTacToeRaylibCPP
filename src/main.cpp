#include <iostream>
#include <memory>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>
#include <raylib-cpp.hpp>

// global variables
const int COLS = 3;
const int ROWS = 3;
const int NumSquares = 9;
const int cellWidth = 200;
const int cellHeight = 200;
const int screenWidth = 1280;
const int screenHeight = 800;
int MoveNumber = 0;

// UI text
static const char* MainMenuMessage[] = {
    "Please select a game mode and choose who moves first.",
    "Then press the Start button.",
    "Please first select a game mode and who moves first"};
static const char* MainMenuButtons[] = {"Hotseat", "Versus AI", "X moves first", "O moves first", "Start Game"};
static const char* WinText[] = {"PLAYER X WIN!", "PLAYER O WIN!", "IT'S A TIE!", "THE GAME IS FINISHED. PLAY AGAIN?"};
static const char* YesNoText[] = {"YES", "NO"};
// UI text

enum CellValue
{
    EMPTY,
    X,
    O
};
enum GameMode
{
    HOTSEAT,
    VERSUS_AI
};
enum GameState
{
    MAINMENU,
    PLAYER_X_MOVE,
    PLAYER_O_MOVE,
    PLAYER_X_WIN,
    PLAYER_O_WIN,
    TIE,
    GAME_FINISHED
};

struct Cell
{
    int cellNumber;
    int indexI;
    int indexJ;
    CellValue value;
    Color cellColor;

    Cell(int cellNumber = 0, int indexI = 0, int indexJ = 0, CellValue value = EMPTY, Color cellColor = GRAY)
        : cellNumber(cellNumber), indexI(indexI), indexJ(indexJ), value(value), cellColor(cellColor) {}
};

class Grid
{
public:
    Grid();
    void GridInit();
    void DrawGrid();
    void ChangeCellState(Vector2 MousePosition, GameState& currentGameState, std::vector<CellValue>& board);
    bool CheckWinner();

private:
    std::vector<std::vector<Cell>> grid;
};

class Player
{
    CellValue piece;

public:
    virtual ~Player() {}

    void setPiece(CellValue p) { piece = p; };
    CellValue getPiece() const { return piece; };
};

class HumanPlayer : public Player
{
public:
    void HumanMove();
};

class AIPlayer : public Player
{
public:
    void AIMove();
};

bool IsMouseOnGrid(Vector2 MousePosition);

int main()
{
    // Window init
    raylib::Window window(screenWidth, screenHeight, "TicTacToe");
    window.SetTargetFPS(60);
    // Window init

    // creating game objects
    GameState currentGameState = MAINMENU;
    GameMode currentGameMode;
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;
    Grid grid;
    std::vector<CellValue> board(NumSquares, EMPTY);
    // creating game objects

    // Main menu UI
    int mainMenuButtonSelected = -1;
    int mouseHoverRec = -1;
    bool buttonClicked = false;
    bool isGameModeSelected = false;
    bool isFirsMoveSelected = false;
    bool isGameFinished = false;
    bool drawErrorMessage = false;
    bool exitGame = false;

    int MessageCounter = 0;

    Rectangle MainMenuRecs[5] = {0};
    for (int i = 0; i < 5; i++)
    {
        MainMenuRecs[i] = (Rectangle){(screenWidth / 2) - 75.0f, (float)(250 + 32 * i), 150.0f, 30.0f};
    }
    Rectangle YesNoRecs[2] = {0};
    for (int i = 0; i < 2; i++)
    {
        YesNoRecs[i] = (Rectangle){(screenWidth / 2) - 150.0f + 152.0f * i, 250.0f, 150.0f, 30.0f};
    }
    // Main menu UI

    // main game loop
    while (!exitGame)
    {

        if (window.ShouldClose() || IsKeyPressed(KEY_ESCAPE))
        {
            exitGame = true;
        }
        // Menu UI update
        if (currentGameState == MAINMENU)
        {
            for (int i = 0; i < 5; i++)
            {
                if (CheckCollisionPointRec(GetMousePosition(), MainMenuRecs[i]))
                {
                    mouseHoverRec = i;
                    break;
                }
                else
                    mouseHoverRec = -1;
            }
            if ((mouseHoverRec >= 0) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                mainMenuButtonSelected = mouseHoverRec;
                buttonClicked = true;
            }
            if (buttonClicked)
            {
                switch (mainMenuButtonSelected)
                {
                    case 0:  // Hotseat
                    {
                        player1 = std::make_unique<HumanPlayer>();
                        player2 = std::make_unique<HumanPlayer>();
                        currentGameMode = HOTSEAT;
                        isGameModeSelected = true;
                        break;
                    }
                    case 1:  // Versus AI
                    {
                        player1 = std::make_unique<HumanPlayer>();
                        player2 = std::make_unique<AIPlayer>();
                        currentGameMode = VERSUS_AI;
                        isGameModeSelected = true;
                        break;
                    }
                    case 2:  // Player X moves first
                    {
                        if (player1 != nullptr && player2 != nullptr)
                        {
                            player1->setPiece(X);
                            player2->setPiece(O);
                            isFirsMoveSelected = true;
                        }
                        else
                            drawErrorMessage = true;
                        break;
                    }
                    case 3:  // Player O moves first
                    {
                        if (player1 != nullptr && player2 != nullptr)
                        {
                            player1->setPiece(O);
                            player2->setPiece(X);
                            isFirsMoveSelected = true;
                        }
                        else
                            drawErrorMessage = true;
                        break;
                    }
                    case 4:  // Start game
                    {
                        if ((isGameModeSelected && isFirsMoveSelected))
                        {
                            if (player1->getPiece() == X)
                            {
                                currentGameState = PLAYER_X_MOVE;
                            }
                            else
                            {
                                currentGameState = PLAYER_O_MOVE;
                            }
                        }
                        else
                            drawErrorMessage = true;
                        break;
                    }
                }
                buttonClicked = false;
            }
            if (drawErrorMessage)
            {
                MessageCounter++;
                if (MessageCounter > 60)
                {
                    drawErrorMessage = false;
                    MessageCounter = 0;
                }
            }
        }
        // Menu UI update

        // Restart menu
        if (currentGameState == GAME_FINISHED)
        {
            for (int i = 0; i < 2; i++)
            {
                if (CheckCollisionPointRec(GetMousePosition(), YesNoRecs[i]))
                {
                    mouseHoverRec = i;
                    break;
                }
                else
                    mouseHoverRec = -1;
            }
            if ((mouseHoverRec >= 0) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                mainMenuButtonSelected = mouseHoverRec;
                buttonClicked = true;
            }
            if (buttonClicked)
            {
                switch (mainMenuButtonSelected)
                {
                    case 0: {
                        if (player1 != nullptr && player2 != nullptr)
                        {
                            player1 = nullptr;
                            player2 = nullptr;
                            printf("Players destoyed");
                        }
                        grid.GridInit();
                        isGameModeSelected = false;
                        isFirsMoveSelected = false;
                        drawErrorMessage = false;
                        currentGameState = MAINMENU;
                        MoveNumber = 0;
                        break;
                    }

                    case 1:
                        exitGame = true;
                        break;
                }
            }
        }
        // Restart menu

        //  Player interaction section
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && (currentGameState != MAINMENU) && IsMouseOnGrid(GetMousePosition()))
        {
            if (currentGameState == PLAYER_X_MOVE)
            {
                grid.ChangeCellState(GetMousePosition(), currentGameState, board);
                currentGameState = PLAYER_O_MOVE;
            }
            else if (currentGameState == PLAYER_O_MOVE)
            {
                grid.ChangeCellState(GetMousePosition(), currentGameState, board);
                currentGameState = PLAYER_X_MOVE;
            }
        }
        if (MoveNumber >= 5 && (currentGameState == PLAYER_X_MOVE || currentGameState == PLAYER_O_MOVE))
        {
            if (grid.CheckWinner())
            {
                currentGameState = PLAYER_X_WIN;
                isGameFinished = true;
                printf("WIN X");
            }
            if (grid.CheckWinner())
            {
                currentGameState = PLAYER_O_WIN;
                isGameFinished = true;
                printf("WIN O");
            }
        }
        if (MoveNumber == 9 && currentGameState != PLAYER_X_WIN && currentGameState != PLAYER_O_WIN)
        {
            currentGameState = TIE;
            isGameFinished = true;
        }
        if (isGameFinished)
        {
            MessageCounter++;
            if (MessageCounter > 120)
            {
                currentGameState = GAME_FINISHED;
                MessageCounter = 0;
                isGameFinished = false;
            }
        }
        // Player interaction section

        //  Drawing section
        window.BeginDrawing();
        window.ClearBackground(RAYWHITE);
        switch (currentGameState)
        {
            case 0: {
                DrawText(MainMenuMessage[0], (screenWidth - MeasureText(MainMenuMessage[0], 30)) / 2, screenHeight - 750, 30, GRAY);
                DrawText(MainMenuMessage[1], (screenWidth - MeasureText(MainMenuMessage[1], 30)) / 2, screenHeight - 720, 30, GRAY);

                if (drawErrorMessage)
                {
                    DrawText(MainMenuMessage[2], (screenWidth - MeasureText(MainMenuMessage[2], 40)) / 2, (screenHeight / 2) - 20, 40, RED);
                }
                else
                {  // Draw rectangles
                    if (isGameModeSelected && currentGameMode == HOTSEAT)
                    {
                        DrawRectangleRec(MainMenuRecs[0], SKYBLUE);
                    }
                    else
                    {
                        DrawRectangleRec(MainMenuRecs[0], ((0 == mouseHoverRec)) ? SKYBLUE : LIGHTGRAY);
                    }
                    if (isGameModeSelected && currentGameMode == VERSUS_AI)
                    {
                        DrawRectangleRec(MainMenuRecs[1], SKYBLUE);
                    }
                    else
                    {
                        DrawRectangleRec(MainMenuRecs[1], ((1 == mouseHoverRec)) ? SKYBLUE : LIGHTGRAY);
                    }
                    if (player1 != nullptr && (isFirsMoveSelected && player1->getPiece() == X))
                    {
                        DrawRectangleRec(MainMenuRecs[2], SKYBLUE);
                    }
                    else
                    {
                        DrawRectangleRec(MainMenuRecs[2], ((2 == mouseHoverRec)) ? SKYBLUE : LIGHTGRAY);
                    }
                    if (player1 != nullptr && (isFirsMoveSelected && player1->getPiece() == O))
                    {
                        DrawRectangleRec(MainMenuRecs[3], SKYBLUE);
                    }
                    else
                    {
                        DrawRectangleRec(MainMenuRecs[3], ((3 == mouseHoverRec)) ? SKYBLUE : LIGHTGRAY);
                    }
                    if (isGameModeSelected && isFirsMoveSelected)
                    {
                        DrawRectangleRec(MainMenuRecs[4], GREEN);
                    }
                    else
                    {
                        DrawRectangleRec(MainMenuRecs[4], ((4 == mouseHoverRec)) ? SKYBLUE : LIGHTGRAY);
                    }
                    for (int i = 0; i < 5; i++)
                    {
                        DrawRectangleLines((int)MainMenuRecs[i].x, (int)MainMenuRecs[i].y, (int)MainMenuRecs[i].width, (int)MainMenuRecs[i].height, ((i == mouseHoverRec)) ? BLUE : GRAY);
                        DrawText(MainMenuButtons[i], (int)(MainMenuRecs[i].x + MainMenuRecs[i].width / 2 - MeasureText(MainMenuButtons[i], 20) / 2), (int)MainMenuRecs[i].y + 5, 20, ((i == mouseHoverRec)) ? DARKBLUE : DARKGRAY);
                    }
                }
                break;
            }
            case 3:
                DrawText(TextFormat(WinText[0]), (screenWidth - MeasureText(WinText[0], 40)) / 2, screenHeight - 750, 40, BLUE);
                grid.DrawGrid();
                break;
            case 4:
                DrawText(TextFormat(WinText[1]), (screenWidth - MeasureText(WinText[1], 40)) / 2, screenHeight - 750, 40, BLUE);
                grid.DrawGrid();
                break;
            case 5:
                DrawText(TextFormat(WinText[2]), (screenWidth - MeasureText(WinText[2], 40)) / 2, screenHeight - 750, 40, BLUE);
                grid.DrawGrid();
                break;
            case 6:
                DrawText(TextFormat(WinText[3]), (screenWidth - MeasureText(WinText[3], 40)) / 2, screenHeight - 750, 40, BLUE);
                for (int i = 0; i < 2; i++)
                {
                    DrawRectangleRec(YesNoRecs[i], ((i == mouseHoverRec)) ? SKYBLUE : LIGHTGRAY);
                    DrawRectangleLines((int)YesNoRecs[i].x, (int)YesNoRecs[i].y, (int)YesNoRecs[i].width, (int)YesNoRecs[i].height, ((i == mouseHoverRec)) ? BLUE : GRAY);
                    DrawText(YesNoText[i], (int)(YesNoRecs[i].x + YesNoRecs[i].width / 2 - MeasureText(YesNoText[i], 20) / 2), (int)YesNoRecs[i].y + 5, 20, ((i == mouseHoverRec)) ? DARKBLUE : DARKGRAY);
                }
                break;
            default:
                grid.DrawGrid();
                break;
        }
        window.EndDrawing();
    }
    return 0;
}

Grid::Grid()
{
    grid.resize(COLS, std::vector<Cell>(ROWS));
    GridInit();
}

void Grid::GridInit()
{
    int cellNumber = 0;
    for (int i = 0; i < COLS; i++)
    {
        for (int j = 0; j < ROWS; j++)
        {
            grid[i][j] = Cell(cellNumber, i, j, EMPTY, LIGHTGRAY);
            cellNumber++;
            printf("Cell number: %d\n", grid[i][j].cellNumber);
        }
    }
}

void Grid::DrawGrid()
{
    for (int i = 0; i < COLS; ++i)
    {
        for (int j = 0; j < ROWS; ++j)
        {
            int x = ((screenWidth / 2) - 300) + i * cellWidth;
            int y = ((screenHeight / 2) - 300) + j * cellHeight;

            DrawRectangle(x, y, cellWidth, cellHeight, grid[i][j].cellColor);
            DrawRectangleLines(x, y, cellWidth, cellHeight, RAYWHITE);

            // Drawing cell contents based on their value
            if (grid[i][j].value == X)
            {
                DrawLine(x, y, x + cellWidth, y + cellHeight, RED);
                DrawLine(x, y + cellHeight, x + cellWidth, y, RED);
            }
            else if (grid[i][j].value == O)
            {
                DrawCircleLines(x + cellWidth / 2, y + cellHeight / 2, cellWidth / 2 - 10, BLUE);
            }
        }
    }
}

void Grid::ChangeCellState(Vector2 MousePosition, GameState& currentGameState, std::vector<CellValue>& board)
{
    {
        int i = (MousePosition.x - ((screenWidth / 2) - 300)) / cellWidth;
        int j = (MousePosition.y - ((screenHeight / 2) - 300)) / cellHeight;
        int cellNum = grid[i][j].cellNumber;
        // check index for valid
        if (i >= 0 && i < COLS && j >= 0 && j < ROWS)
        {
            // change cell state

            if (grid[i][j].value == EMPTY && currentGameState == PLAYER_X_MOVE)
            {
                grid[i][j].value = X;
                board[cellNum] = X;
                MoveNumber++;
            }
            else if (grid[i][j].value == EMPTY && currentGameState == PLAYER_O_MOVE)
            {
                grid[i][j].value = O;
                board[cellNum] = O;
                MoveNumber++;
            }
        }
    }
}

bool Grid::CheckWinner()
{
    if (MoveNumber >= 5)
    {
        for (int i = 0; i < COLS; i++)
        {
            if (grid[0][i].value != EMPTY && grid[0][i].value == grid[1][i].value && grid[1][i].value == grid[2][i].value)
            {
                for (int j = 0; j < COLS; j++)
                {
                    grid[j][i].cellColor = GREEN;
                }
                return true;
            }
            else if (grid[i][0].value != EMPTY && grid[i][0].value == grid[i][1].value && grid[i][1].value == grid[i][2].value)
            {
                for (int j = 0; j < COLS; j++)
                {
                    grid[i][j].cellColor = GREEN;
                }
                return true;
            }
        }
        // diagonal 1
        if (grid[0][0].value != EMPTY && grid[0][0].value == grid[1][1].value && grid[1][1].value == grid[2][2].value)
        {
            for (int i = 0; i < COLS; i++)
            {
                grid[i][i].cellColor = GREEN;
            }
            return true;
        }
        // diagonal 2
        if (grid[2][0].value != EMPTY && grid[2][0].value == grid[1][1].value && grid[1][1].value == grid[0][2].value)
        {
            grid[2][0].cellColor = GREEN;
            grid[1][1].cellColor = GREEN;
            grid[0][2].cellColor = GREEN;
            return true;
        }
    }
    return false;
}

bool IsMouseOnGrid(Vector2 MousePosition)
{
    // check if mouse in Cells area
    if (MousePosition.x >= (screenWidth / 2) - 300 && MousePosition.x <= (screenWidth / 2) + 300 &&
        MousePosition.y >= (screenHeight / 2) - 300 && MousePosition.y <= (screenHeight / 2) + 300)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void HumanPlayer::HumanMove()
{
}

void AIPlayer::AIMove()
{
}
