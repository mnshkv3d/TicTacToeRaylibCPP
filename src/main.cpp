#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <array>
#include "include/raylib-cpp.hpp"
#include "include/raymath.hpp"

// global variables
const int COLS = 3;
const int ROWS = 3;
const int cellWidth = 200;
const int cellHeight = 200;
const int screenWidth = 1280;
const int screenHeight = 800;
int MoveNumber = 0;

enum MainMenu
{
    FIRST_MOVE_X,
    FIRST_MOVE_O,
};

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
    END
};

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

    Cell(int indexI = 0, int indexJ = 0, CellValue value = EMPTY)
        : indexI(indexI), indexJ(indexJ), value(EMPTY) {}
};

class Grid
{
public:
    Grid();
    void GridInit();
    void DrawGrid();
    void ChangeCellState(Vector2 MousePosition, GameManager& GameManager);
    bool CheckWinner(GameManager& GameManager);

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

    Rectangle MainMenuRecs[2] = {0};
    int mainMenuButtonSelected = -1;
    int mouseHoverRec = -1;

    for (int i = 0; i < 2; i++)
        MainMenuRecs[i] = (Rectangle){40.0f, (float)(50 + 32 * i), 150.0f, 30.0f};

    GameManager gm;
    Grid grid;
    // gm.CurrentGameState = PLAYER_X_MOVE;

    while (!WindowShouldClose())
    {

        // Game loop update section

        // Mouse toggle group logic
        if (gm.CurrentGameState == MAINMENU)
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
                                gm.CurrentGameState = PLAYER_X_MOVE;
                            case 1:
                                gm.CurrentGameState = PLAYER_O_MOVE;
                        }
                    }
                }
                else
                    mouseHoverRec = -1;
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && (gm.CurrentGameState != MAINMENU || gm.CurrentGameState == END))
        {
            if (gm.CurrentGameState == PLAYER_X_MOVE)
            {
                grid.ChangeCellState(GetMousePosition(), gm);
                gm.CurrentGameState = PLAYER_O_MOVE;
                grid.CheckWinner(gm);
                if (grid.CheckWinner(gm))
                {
                    printf("Player X Win");
                }
            }
            else if (gm.CurrentGameState == PLAYER_O_MOVE)
            {
                grid.ChangeCellState(GetMousePosition(), gm);
                gm.CurrentGameState = PLAYER_X_MOVE;
                grid.CheckWinner(gm);
                if (grid.CheckWinner(gm))
                {
                    printf("Player O Win");
                }
            }
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            grid.GridInit();
            gm.CurrentGameState = MAINMENU;
            MoveNumber = 0;
        }

        // Drawing section
        BeginDrawing();
        window.ClearBackground(RAYWHITE);
        if (gm.CurrentGameState == MAINMENU)
        {
            // Draw rectangles
            for (int i = 0; i < 2; i++)
            {
                DrawRectangleRec(MainMenuRecs[i], ((i == mainMenuButtonSelected) || (i == mouseHoverRec)) ? SKYBLUE : LIGHTGRAY);
                DrawRectangleLines((int)MainMenuRecs[i].x, (int)MainMenuRecs[i].y, (int)MainMenuRecs[i].width, (int)MainMenuRecs[i].height, ((i == mainMenuButtonSelected) || (i == mouseHoverRec)) ? BLUE : GRAY);
                DrawText(MainMenuButtons[i], (int)(MainMenuRecs[i].x + MainMenuRecs[i].width / 2 - MeasureText(MainMenuButtons[i], 10) / 2), (int)MainMenuRecs[i].y + 11, 10, ((i == mainMenuButtonSelected) || (i == mouseHoverRec)) ? DARKBLUE : DARKGRAY);
            }
        }
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
            {
                grid[i][j].value = X;
                MoveNumber++;
                printf("Move X\nCell i: %d, Cell j: %d \nMove number: %d\n", i, j, MoveNumber);
            }
            else if (grid[i][j].value == EMPTY && GameManager.CurrentGameState == PLAYER_O_MOVE)
            {
                grid[i][j].value = O;
                MoveNumber++;
                printf("Move O\nCell i: %d, Cell j: %d \nMove number: %d\n", i, j, MoveNumber);
            }
        }
    }
}

bool Grid::CheckWinner(GameManager& GameManager)
{
    if (MoveNumber >= 5)
    {
        // firs COL
        if (grid[0][0].value != EMPTY && grid[0][0].value == grid[0][1].value && grid[0][1].value == grid[0][2].value)
        {
            printf("COL 1!\n");
            GameManager.CurrentGameState = END;
            return true;
        }
        // second COL
        else if (grid[1][0].value != EMPTY && grid[1][0].value == grid[1][1].value && grid[1][1].value == grid[1][2].value)
        {
            printf("COL 2!\n");
            GameManager.CurrentGameState = END;
            return true;
        }
        // third COL
        else if (grid[2][0].value != EMPTY && grid[2][0].value == grid[2][1].value && grid[2][1].value == grid[2][2].value)
        {
            printf("COL 3!\n");
            GameManager.CurrentGameState = END;
            return true;
        }
        // firs row
        else if (grid[0][0].value != EMPTY && grid[0][0].value == grid[1][0].value && grid[1][0].value == grid[2][0].value)
        {
            printf("ROW 1!\n");
            GameManager.CurrentGameState = END;
            return true;
        }
        // second row
        else if (grid[0][1].value != EMPTY && grid[0][1].value == grid[1][1].value && grid[1][1].value == grid[2][1].value)
        {
            printf("ROW 2!\n");
            GameManager.CurrentGameState = END;
            return true;
        }
        // third row
        else if (grid[0][2].value != EMPTY && grid[0][2].value == grid[1][2].value && grid[1][2].value == grid[2][2].value)
        {
            printf("ROW3 3!\n");
            GameManager.CurrentGameState = END;
            return true;
        }
        // diagonal 1
        else if (grid[0][0].value != EMPTY && grid[0][0].value == grid[1][1].value && grid[1][1].value == grid[2][2].value)
        {
            printf("DIAGONAL 1!\n");
            GameManager.CurrentGameState = END;
            return true;
        }
        // diagonal 2
        else if (grid[2][0].value != EMPTY && grid[2][0].value == grid[1][1].value && grid[1][1].value == grid[0][2].value)
        {
            printf("DIAGONAL 2!\n");
            GameManager.CurrentGameState = END;
            return true;
        }
    }
    return false;
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