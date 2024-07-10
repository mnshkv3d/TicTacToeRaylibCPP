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
enum GameState { PLAYER1, PLAYER2, END };

struct Cell {
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
    void ChangeCellState(Vector2 ballPosition);

    Cell& getCell(int i, int j);

private:
    std::vector<std::vector<Cell>> grid;
};

int main()
{

    raylib::Window window(screenWidth, screenHeight, "PingTacPong");
    window.SetTargetFPS(60);
    Grid grid;

    while (!WindowShouldClose())
    {
        // Game loop update section
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            grid.ChangeCellState(GetMousePosition());
        }

        // Drawing section
        BeginDrawing();
        window.ClearBackground(BLACK);
        grid.DrawGrid();
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
            DrawRectangleLines(x, y, cellWidth, cellHeight, WHITE);

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
            if (grid[i][j].value == EMPTY)
                grid[i][j].value = X;
            else if (grid[i][j].value == X)
                grid[i][j].value = O;
            else if (grid[i][j].value == O)
                grid[i][j].value = EMPTY;
        }
    }
}

Cell& Grid::getCell(int i, int j)
{
    return grid[i][j];
}
