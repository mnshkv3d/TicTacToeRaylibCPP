#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include "include/raylib-cpp.hpp"

// global variables
const int COLS = 3;
const int ROWS = 3;
const int cellWidth = 200;
const int cellHeight = 200;
const int screenWidth = 1280;
const int screenHeight = 800;
int MoveNumber = 0;

static const char* MainMenuButtons[] = {"X moves first", "O moves first"};

enum CellValue
{
    EMPTY,
    X,
    O
};
enum GameMode
{
    HOTSEAT,
    VSPC
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
struct Cell
{
    int indexI;
    int indexJ;
    CellValue value;
    Color cellColor;

    Cell(int indexI = 0, int indexJ = 0, CellValue value = EMPTY, Color cellColor = GREEN)
        : indexI(indexI), indexJ(indexJ), value(value), cellColor(cellColor) {}
};

class Grid
{
public:
    Grid();
    void GridInit();
    void DrawGrid();
    void ChangeCellState(Vector2 MousePosition);
    bool CheckWinner();

private:
    std::vector<std::vector<Cell>> grid;
};

class Player
{
public:
};
GameState currentGameState;

int main()
{
    currentGameState = MAINMENU;
    raylib::Window window(screenWidth, screenHeight, "PingTacPong");
    window.SetTargetFPS(60);

    Rectangle MainMenuRecs[2] = {0};
    int mainMenuButtonSelected = -1;
    int mouseHoverRec = -1;

    for (int i = 0; i < 2; i++)
    {
        MainMenuRecs[i] = (Rectangle){40.0f, (float)(50 + 32 * i), 150.0f, 30.0f};
    }

    Grid grid;

    while (!WindowShouldClose())
    {

        // Game loop update section

        // Mouse toggle group logic
        if (currentGameState == MAINMENU)
        {
            for (int i = 0; i < 2; i++)
            {
                if (CheckCollisionPointRec(GetMousePosition(), MainMenuRecs[i]))
                {
                    mouseHoverRec = i;

                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        mainMenuButtonSelected = i;
                        switch (mainMenuButtonSelected)
                        {
                            case 0:
                                currentGameState = PLAYER_X_MOVE;
                                mainMenuButtonSelected = -1;
                                break;
                            case 1:
                                currentGameState = PLAYER_O_MOVE;
                                mainMenuButtonSelected = -1;
                                break;
                        }
                    }
                }
                else
                {
                    mouseHoverRec = -1;
                    mainMenuButtonSelected = -1;
                }
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && (currentGameState != MAINMENU))
        {
            if (currentGameState == PLAYER_X_MOVE)
            {
                grid.ChangeCellState(GetMousePosition());
                currentGameState = PLAYER_O_MOVE;
                if (grid.CheckWinner())
                {
                    currentGameState = PLAYER_X_WIN;
                    printf("Player X Win");
                }
            }
            else if (currentGameState == PLAYER_O_MOVE)
            {
                grid.ChangeCellState(GetMousePosition());
                currentGameState = PLAYER_X_MOVE;
                if (grid.CheckWinner())
                {
                    currentGameState = PLAYER_O_WIN;
                    printf("Player O Win");
                }
            }
        }
        if (MoveNumber == 9 && currentGameState != PLAYER_X_WIN && currentGameState != PLAYER_O_WIN)
        {
            currentGameState = TIE;
        }
        if (IsKeyPressed(KEY_SPACE) && currentGameState != PLAYER_X_MOVE && currentGameState != PLAYER_O_MOVE)
        {
            grid.GridInit();
            currentGameState = MAINMENU;
            MoveNumber = 0;
        }

        // Drawing section
        BeginDrawing();
        window.ClearBackground(RAYWHITE);
        if (currentGameState == MAINMENU)
        {
            // Draw rectangles
            for (int i = 0; i < 2; i++)
            {
                DrawRectangleRec(MainMenuRecs[i], ((i == mainMenuButtonSelected) || (i == mouseHoverRec)) ? SKYBLUE : LIGHTGRAY);
                DrawRectangleLines((int)MainMenuRecs[i].x, (int)MainMenuRecs[i].y, (int)MainMenuRecs[i].width, (int)MainMenuRecs[i].height, ((i == mainMenuButtonSelected) || (i == mouseHoverRec)) ? BLUE : GRAY);
                DrawText(MainMenuButtons[i], (int)(MainMenuRecs[i].x + MainMenuRecs[i].width / 2 - MeasureText(MainMenuButtons[i], 10) / 2), (int)MainMenuRecs[i].y + 11, 10, ((i == mainMenuButtonSelected) || (i == mouseHoverRec)) ? DARKBLUE : DARKGRAY);
            }
        }
        if (currentGameState != MAINMENU)
        {
            grid.DrawGrid();
        }
        if (currentGameState == PLAYER_X_WIN)
        {
            DrawText(TextFormat("PLAYER X WIN!"), (screenWidth / 2) - (screenWidth / 10), screenHeight / 2, 40, BLUE);
        }
        if (currentGameState == PLAYER_O_WIN)
        {
            DrawText(TextFormat("PLAYER O WIN!"), screenWidth / 2, screenHeight / 2, 40, BLUE);
        }
        if (currentGameState == TIE)
        {
            DrawText(TextFormat("IT'S A TIE!"), (screenWidth / 2) - (screenWidth / 10), screenHeight / 2, 40, BLUE);
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
    for (int i = 0; i < COLS; i++)
    {
        for (int j = 0; j < ROWS; j++)
        {
            grid[i][j] = Cell(i, j, EMPTY, GREEN);
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

void Grid::ChangeCellState(Vector2 MousePosition)
{
    // check if mouse in Cells area
    if (MousePosition.x >= (screenWidth / 2) - 300 && MousePosition.x <= (screenWidth / 2) + 300 &&
        MousePosition.y >= (screenHeight / 2) - 300 && MousePosition.y <= (screenHeight / 2) + 300)
    {
        int i = (MousePosition.x - ((screenWidth / 2) - 300)) / cellWidth;
        int j = (MousePosition.y - ((screenHeight / 2) - 300)) / cellHeight;
        // check index for valid
        if (i >= 0 && i < COLS && j >= 0 && j < ROWS)
        {
            // change cell state

            if (grid[i][j].value == EMPTY && currentGameState == PLAYER_X_MOVE)
            {
                grid[i][j].value = X;
                MoveNumber++;
            }
            else if (grid[i][j].value == EMPTY && currentGameState == PLAYER_O_MOVE)
            {
                grid[i][j].value = O;
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
                printf("LINE\n");
                for (int j = 0; j < COLS; j++)
                {
                    grid[j][i].cellColor = GRAY;
                }
                return true;
            }
            else if (grid[i][0].value != EMPTY && grid[i][0].value == grid[i][1].value && grid[i][1].value == grid[i][2].value)
            {
                printf("ROW\n");
                for (int j = 0; j < COLS; j++)
                {
                    grid[i][j].cellColor = GRAY;
                }
                return true;
            }
        }
        // diagonal 1
        if (grid[0][0].value != EMPTY && grid[0][0].value == grid[1][1].value && grid[1][1].value == grid[2][2].value)
        {
            printf("DIAGONAL 1!\n");
            for (int i = 0; i < COLS; i++)
            {
                grid[i][i].cellColor = GRAY;
            }
            return true;
        }
        // diagonal 2
        if (grid[2][0].value != EMPTY && grid[2][0].value == grid[1][1].value && grid[1][1].value == grid[0][2].value)
        {
            printf("DIAGONAL 2!\n");
            grid[2][0].cellColor = GRAY;
            grid[1][1].cellColor = GRAY;
            grid[0][2].cellColor = GRAY;
            return true;
        }
    }
    return false;
}