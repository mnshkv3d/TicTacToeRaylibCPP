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

static const char* MainMenuButtons[] = {"Hotseat", "Versus AI", "X moves first", "O moves first", "Start Game"};

// static const char* WinText[] = {"PLAYER X WIN!", "PLAYER O WIN!", "IT'S A TIE!"};

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
    TIE
};

struct GameSettings
{
    GameMode gameMode;
    CellValue player1;
    CellValue player2;
    bool gameReadyToStart;

    GameSettings(GameMode gm = HOTSEAT, CellValue p1 = X, CellValue p2 = O, bool start = false)
        : gameMode(gm), player1(p1), player2(p2), gameReadyToStart(start) {}
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
    raylib::Window window(screenWidth, screenHeight, "PingTacPong");
    window.SetTargetFPS(60);
    // Window init

    // creating game objects
    GameState currentGameState;
    GameSettings currentGameSettings;
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;
    Grid grid;
    std::vector<CellValue> board(NumSquares, EMPTY);

    // Main menu UI
    Rectangle MainMenuRecs[5] = {0};
    int mainMenuButtonSelected = -1;
    int mouseHoverRec = -1;
    bool buttonClicked = false;

    for (int i = 0; i < 5; i++)
    {
        MainMenuRecs[i] = (Rectangle){40.0f, (float)(50 + 32 * i), 150.0f, 30.0f};
    }
    // Main menu UI

    // initial game state
    currentGameState = MAINMENU;

    // main game loop
    while (!WindowShouldClose())
    {
        // Game loop update section

        // Menu UI update
        if (currentGameState == MAINMENU)
        {
            for (int i = 0; i < 5; i++)
            {
                if (CheckCollisionPointRec(GetMousePosition(), MainMenuRecs[i]))
                {
                    mouseHoverRec = i;
                    printf("Mouse hover %d\n", mouseHoverRec);

                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        mainMenuButtonSelected = i;
                        buttonClicked = true;
                    }
                }
                else
                {
                    // mouseHoverRec = -1;
                    printf("Mouse hover %d\n", mouseHoverRec);
                }
            }
            if (buttonClicked)
            {
                switch (mainMenuButtonSelected)
                {
                    case 0:  // Hotseat
                    {
                        player1 = std::make_unique<HumanPlayer>();
                        player2 = std::make_unique<HumanPlayer>();
                        currentGameSettings.gameMode = HOTSEAT;
                        mainMenuButtonSelected = 1;
                        printf("Button HotSeat pressed");
                        break;
                    }

                    case 1:  // Versus AI
                    {
                        player1 = std::make_unique<HumanPlayer>();
                        player2 = std::make_unique<AIPlayer>();
                        currentGameSettings.gameMode = VERSUS_AI;
                        mainMenuButtonSelected = 2;
                        printf("Button AI");
                        break;
                    }
                    case 2:  // Player X moves first
                    {

                        if (player1 != nullptr && player2 != nullptr)
                        {
                            player1->setPiece(X);
                            player2->setPiece(O);
                            currentGameSettings.gameReadyToStart = true;
                        }
                        mainMenuButtonSelected = 3;
                        printf("Button X first");
                        break;
                    }
                    case 3:  // Player O moves first
                    {
                        if (player1 != nullptr && player2 != nullptr)
                        {
                            player1->setPiece(O);
                            player2->setPiece(X);
                            currentGameSettings.gameReadyToStart = true;
                        }
                        mainMenuButtonSelected = 4;
                        printf("Button O first");
                        break;
                    }
                    case 4:  // Start game
                    {
                        if (currentGameSettings.gameReadyToStart && (player1 != nullptr && player2 != nullptr))
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
                        printf("Button Start");
                        break;
                    }
                }
                buttonClicked = false;
            }
        }
        // Menu UI update
        // Player interaction section
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
                printf("WIN X");
            }
            if (grid.CheckWinner())
            {
                currentGameState = PLAYER_O_WIN;
                printf("WIN O");
            }
        }
        if (MoveNumber == 9 && currentGameState != PLAYER_X_WIN && currentGameState != PLAYER_O_WIN)
        {
            currentGameState = TIE;
        }
        if (IsKeyPressed(KEY_SPACE) && currentGameState != PLAYER_X_MOVE && currentGameState != PLAYER_O_MOVE)
        {
            if (player1 != nullptr && player2 != nullptr)
            {
                player1 = nullptr;
                player2 = nullptr;
                printf("Players destoyed");
            }
            grid.GridInit();
            currentGameState = MAINMENU;
            MoveNumber = 0;
        }
        // Player interaction section

        //  Drawing section
        BeginDrawing();
        window.ClearBackground(RAYWHITE);
        if (currentGameState == MAINMENU)
        {
            // Draw rectangles
            for (int i = 0; i < 5; i++)
            {
                DrawRectangleRec(MainMenuRecs[i], ((i == mouseHoverRec)) ? SKYBLUE : LIGHTGRAY);
                DrawRectangleLines((int)MainMenuRecs[i].x, (int)MainMenuRecs[i].y, (int)MainMenuRecs[i].width, (int)MainMenuRecs[i].height, ((i == mouseHoverRec)) ? BLUE : GRAY);
                DrawText(MainMenuButtons[i], (int)(MainMenuRecs[i].x + MainMenuRecs[i].width / 2 - MeasureText(MainMenuButtons[i], 10) / 2), (int)MainMenuRecs[i].y + 11, 10, ((i == mouseHoverRec)) ? DARKBLUE : DARKGRAY);
            }
        }
        if (currentGameState != MAINMENU)
        {
            grid.DrawGrid();
        }
        if (currentGameState == PLAYER_X_WIN)
        {
            DrawText(TextFormat("PLAYER X WIN!"), (screenWidth - MeasureText("PLAYER X WIN!", 40)) / 2, screenHeight - 750, 40, BLUE);
        }
        if (currentGameState == PLAYER_O_WIN)
        {
            DrawText(TextFormat("PLAYER O WIN!"), (screenWidth - MeasureText("PLAYER O WIN!", 40)) / 2, screenHeight - 750, 40, BLUE);
        }
        if (currentGameState == TIE)
        {
            DrawText(TextFormat("IT'S A TIE!"), (screenWidth - MeasureText("IT'S A TIE!", 40)) / 2, screenHeight - 750, 40, BLUE);
        }
        EndDrawing();
    }

    CloseWindow();
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
                // board[cellNum] = X;
                MoveNumber++;
            }
            else if (grid[i][j].value == EMPTY && currentGameState == PLAYER_O_MOVE)
            {
                grid[i][j].value = O;
                // board[cellNum] = O;
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
