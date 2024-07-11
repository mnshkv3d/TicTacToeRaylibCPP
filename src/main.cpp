#include <iostream>
#include <cstdio>
#include <vector>
#include "include/raylib-cpp.hpp"
#include "include/raymath.hpp"

// global variables
const int COLS = 3;
const int ROWS = 3;
const int cellWidth = 200;
const int cellHeight = 200;
const int screenWidth = 1280;
const int screenHeight = 800;

enum CellValue { EMPTY, X, O };
enum GameMode { HOTSEAT, VSPC };
enum GameState { MAINMENU, PLAYER_X_MOVE, PLAYER_O_MOVE, END };

struct GameManager
{
    GameMode CurrentGameMode;
    GameState CurrentGameState;

    GameManager(GameMode CurrentGameMode = HOTSEAT, GameState CurrentGameState = MAINMENU)
        : CurrentGameMode(CurrentGameMode), CurrentGameState(CurrentGameState)
    {}
};

struct Cell
{
    int indexI;
    int indexJ;
    CellValue value;

    Cell(int indexI = 0, int indexJ = 0, CellValue value = EMPTY) : indexI(indexI), indexJ(indexJ), value(EMPTY) {}
};

class Grid
{
public:
    Grid();
    void GridInit();
    void DrawGrid();
    void ChangeCellState(Vector2 MousePosition, GameManager& GameManager);

    Cell& getCell(int i, int j);

private:
    std::vector<std::vector<Cell>> grid;
};

class Player
{
public:
};

int main()
{

    raylib::Window window(screenWidth, screenHeight, "PingTacPong");
    window.SetTargetFPS(60);
    GameManager gm;
    Grid grid;
    gm.CurrentGameState = PLAYER_X_MOVE;
    while (!WindowShouldClose())
    {
        // Game loop update section
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && gm.CurrentGameState != MAINMENU)
        {
            if (gm.CurrentGameState == PLAYER_X_MOVE)
            {
                grid.ChangeCellState(GetMousePosition(), gm);
                printf("Player X move\n");
                gm.CurrentGameState = PLAYER_O_MOVE;
            }
            else if (gm.CurrentGameState == PLAYER_O_MOVE)
            {
                grid.ChangeCellState(GetMousePosition(), gm);
                printf("Player O move \n");
                gm.CurrentGameState = PLAYER_X_MOVE;
            }
        }

        // Drawing section
        BeginDrawing();
        window.ClearBackground(RAYWHITE);
        if (gm.CurrentGameState != MAINMENU)
        {
            grid.DrawGrid();
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
            grid[i][j] = Cell(i, j, EMPTY);
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

            DrawRectangle(x, y, cellWidth, cellHeight, GREEN);
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

void Grid::ChangeCellState(Vector2 MousePosition, GameManager& GameManager)
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
            if (grid[i][j].value == EMPTY && GameManager.CurrentGameState == PLAYER_X_MOVE)
                grid[i][j].value = X;
            else if (grid[i][j].value == EMPTY && GameManager.CurrentGameState == PLAYER_O_MOVE)
                grid[i][j].value = O;
        }
    }
}

Cell& Grid::getCell(int i, int j)
{
    return grid[i][j];
}

// // check index for valid
// if (i >= 0 && i < COLS && j >= 0 && j < ROWS)
// {
//     // change cell state
//     if (grid[i][j].value == EMPTY)
//         grid[i][j].value = X;
//     else if (grid[i][j].value == X)
//         grid[i][j].value = O;
//     else if (grid[i][j].value == O)
//         grid[i][j].value = EMPTY;
// }

// if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
// {
//     for (int i = 0; i < COLS; ++i)
//     {
//         for (int j = 0; j < ROWS; ++j)
//         {
//             Cell& cell = grid.getCell(i, j);
//             std::cout << "Cell (" << cell.indexI << ", " << cell.indexJ << "): ";
//             switch (cell.value)
//             {
//                 case EMPTY: std::cout << "EMPTY"; break;
//                 case X: std::cout << "X"; break;
//                 case O: std::cout << "O"; break;
//             }
//             std::cout << std::endl;
//         }
//     }
// }