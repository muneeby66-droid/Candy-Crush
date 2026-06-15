#include <iostream>
#include <cstdlib>   // For rand() and system()
#include <ctime>     // For time()    
#include <windows.h>
// For Sleep() - needed for the visual delay

using namespace std;
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

}

void myLine(int x1, int y1, int x2, int y2, int color) //use three 3 integers if you want colored lines.
{
    HWND console_handle = GetConsoleWindow();
    HDC device_context = GetDC(console_handle);

    //change the color by changing the values in RGB (from 0-255) to get shades of gray. For other colors use 3 integers for colors.
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(color, color, color)); //2 is the width of the pen
    SelectObject(device_context, pen);
    MoveToEx(device_context, x1, y1, NULL);
    LineTo(device_context, x2, y2);
    ReleaseDC(console_handle, device_context);
    DeleteObject(pen);

}

void myTriangle(int x1, int y1, int x2, int y2, int R, int G, int B)
{
    HWND console_handle = GetConsoleWindow();
    HDC device_context = GetDC(console_handle);

    //change the color by changing the values in RGB (from 0-255) to get shades of gray. For other colors use 3 integers for colors.
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B)); //2 is the width of the pen
    SelectObject(device_context, pen);

    int mid = (x1 + x2) / 2;
    MoveToEx(device_context, mid, y1, NULL);
    LineTo(device_context, x2, y2);
    LineTo(device_context, x1, y2);
    LineTo(device_context, mid + 1, y1);

    HBRUSH brush = ::CreateSolidBrush(RGB(0, 0, 0));  //Fill color is black
    SelectObject(device_context, brush);

    FloodFill(device_context, mid, (y1 + y2) / 2, RGB(R, G, B));

    ReleaseDC(console_handle, device_context);
    DeleteObject(pen);
    DeleteObject(brush);
}

// This function checks if any of the 4 cursor keys are pressed.
// If any cursor key is pressed, then the function returns true, and whichKey identifies which of the 4 cursor keys is pressed.
// whichkey is assigned following values if any cursor key is pressed. 1 for left, 2 for up, 3 for right and 4 for left.
bool isCursorKeyPressed(int& whickKey)   //whichKey passed as reference....
{
    char key;
    key = (char)GetAsyncKeyState(37);
    if (key == 1)
    {
        whickKey = 1; // 1 if left key is pressed
        return true;
    }
    key = (char)GetAsyncKeyState(38);
    if (key == 1)
    {

        whickKey = 2; // 2 if up key is pressed
        return true;
    }

    key = (char)GetAsyncKeyState(39);
    if (key == 1)
    {

        whickKey = 3; // 3 if right key is pressed
        return true;
    }
    key = (char)GetAsyncKeyState(40);
    if (key == 1)
    {

        whickKey = 4;   // 4 if down key is pressed
        return true;
    }
    key = (char)GetAsyncKeyState(13);
    if (key == 1)
    {

        whickKey = 5;   // 4 if down key is pressed
        return true;
    }
    return false;
}

void myRect(int x1, int y1, int x2, int y2, int R, int G, int B)
{
    HWND console_handle = GetConsoleWindow();
    HDC device_context = GetDC(console_handle);

    //change the color by changing the values in RGB (from 0-255)
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
    SelectObject(device_context, pen);
    HBRUSH brush = ::CreateSolidBrush(RGB(0, 0, 0)); //Fill color is passed as parameter to the function!!!

    SelectObject(device_context, brush);

    Rectangle(device_context, x1, y1, x2, y2);
    ReleaseDC(console_handle, device_context);
    DeleteObject(pen);
    DeleteObject(brush);
}
void myEllipse(int x1, int y1, int x2, int y2, int R, int G, int B)
{
    HWND console_handle = GetConsoleWindow();
    HDC device_context = GetDC(console_handle);

    //change the color by changing the values in RGB (from 0-255)
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
    SelectObject(device_context, pen);
    HBRUSH brush = ::CreateSolidBrush(RGB(0, 0, 0));  //Fill color is black
    SelectObject(device_context, brush);
    Ellipse(device_context, x1, y1, x2, y2);
    ReleaseDC(console_handle, device_context);
    DeleteObject(pen);
    DeleteObject(brush);


}

// 0, 1, 2, 3 : Normal Candies
// 4 : Color Bomb
// 10-13 : Horizontal Stripes (Clear Row)
// 20-23 : Vertical Stripes (Clear Column)
// Functions Declarations
void clearCell(int board[8][8], int r, int c, int& score);
void PrintBoardWithCursor(int board[][8], int cursorRow, int cursorCol, bool selected, int r, int c, int& score, int timeLeft);

void Grid(int board[8][8])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board[i][j] = rand() % 4;
        }
    }
}
//explosion
void clearCell(int board[8][8], int r, int c, int& score)
{
    if (r < 0 || r >= 8 || c < 0 || c >= 8)
        return;

    int val = board[r][c];
    if (val == -1) return;

    board[r][c] = -1; // Destroy
    score += 10;

    // Horizontal Stripe
    if (val >= 10 && val <= 13)
    {
        score += 60;
        for (int j = 0; j < 8; j++)
            clearCell(board, r, j, score);
    }
    // Vertical Stripe
    else if (val >= 20 && val <= 23)
    {
        score += 60;
        for (int i = 0; i < 8; i++)
            clearCell(board, i, c, score);
    }
}

//MATCH 3
bool match(int board[8][8], int& score)
{
    bool found = false;
    // Horizontal
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            int c = board[i][j];
            if (c != -1 && c != 4 && (c % 10 == board[i][j + 1] % 10) && (c % 10 == board[i][j + 2] % 10))
            {
                clearCell(board, i, j, score);
                clearCell(board, i, j + 1, score);
                clearCell(board, i, j + 2, score);
                found = true;
            }
        }
    }
    // Vertical
    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 6; i++)
        {
            int c = board[i][j];
            if (c != -1 && c != 4 && (c % 10 == board[i + 1][j] % 10) && (c % 10 == board[i + 2][j] % 10))
            {
                clearCell(board, i, j, score);
                clearCell(board, i + 1, j, score);
                clearCell(board, i + 2, j, score);
                found = true;
            }
        }
    }
    return found;
}

bool match4by4(int board[8][8], int& score)
{
    bool found = false;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            int c = board[i][j];
            if (c != -1 && c != 4 && (c % 10 == board[i][j + 1] % 10) && (c % 10 == board[i][j + 2] % 10) && (c % 10 == board[i][j + 3] % 10))
            {
                int color = c % 10;
                clearCell(board, i, j, score);
                clearCell(board, i, j + 1, score);
                clearCell(board, i, j + 2, score);
                clearCell(board, i, j + 3, score);
                board[i][j + 1] = 20 + color;
                found = true;
                score += 50;
            }
        }
    }
    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 5; i++)
        {
            int c = board[i][j];
            if (c != -1 && c != 4 && (c % 10 == board[i + 1][j] % 10) && (c % 10 == board[i + 2][j] % 10) && (c % 10 == board[i + 3][j] % 10))
            {
                int color = c % 10;
                clearCell(board, i, j, score);
                clearCell(board, i + 1, j, score);
                clearCell(board, i + 2, j, score);
                clearCell(board, i + 3, j, score);
                board[i + 3][j] = 10 + color;
                found = true; score += 50;
            }
        }
    }
    return found;
}

bool match5by5(int board[8][8], int& score)
{
    bool found = false;
    // Horizontal
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int c = board[i][j];
            if (c != -1 && c != 4 && (c % 10 == board[i][j + 1] % 10) && (c % 10 == board[i][j + 2] % 10) && (c % 10 == board[i][j + 3] % 10) && (c % 10 == board[i][j + 4] % 10))
            {
                clearCell(board, i, j, score);
                clearCell(board, i, j + 1, score);
                clearCell(board, i, j + 2, score);
                clearCell(board, i, j + 3, score);
                clearCell(board, i, j + 4, score);
                board[i][j + 2] = 4; // Bomb
                found = true; score += 200;
            }
        }
    }
    // Vertical
    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            int c = board[i][j];
            if (c != -1 && c != 4 && (c % 10 == board[i + 1][j] % 10) && (c % 10 == board[i + 2][j] % 10) && (c % 10 == board[i + 3][j] % 10) && (c % 10 == board[i + 4][j] % 10))
            {
                clearCell(board, i, j, score);
                clearCell(board, i + 1, j, score);
                clearCell(board, i + 2, j, score);
                clearCell(board, i + 3, j, score);
                clearCell(board, i + 4, j, score);
                board[i + 2][j] = 4; // Bomb
                found = true; score += 200;
            }
        }
    }
    return found;
}

bool isTShape(int board[][8], int& score)
{
    bool found = false;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            int c = board[i][j];

            // Skip empty cells or Color Bombs (4)
            if (c == -1 || c == 4)
                continue;

            //Check 1: Horizontal Center (Left & Right match center)
            bool hMatch = (j > 0 && j < 7) && (board[i][j - 1] % 10 == c % 10) && (board[i][j + 1] % 10 == c % 10);

            //Check 2: Vertical Center (Up & Down match center)
            bool vMatch = (i > 0 && i < 7) && (board[i - 1][j] % 10 == c % 10) && (board[i + 1][j] % 10 == c % 10);

            //Case A: Horizontal Bar with Vertical Tail
            if (hMatch)
            {
                // T Pointing UP (Tail goes Up)
                if (i >= 2 && (board[i - 1][j] % 10 == c % 10) && (board[i - 2][j] % 10 == c % 10))
                {
                    clearCell(board, i, j - 1, score);
                    clearCell(board, i, j + 1, score);
                    clearCell(board, i - 1, j, score); // Tail 1
                    clearCell(board, i - 2, j, score); // Tail 2
                    clearCell(board, i, j, score);     // Center
                    found = true;
                    score += 100;
                }
                // T Pointing down (Tail goes Down)
                else if (i <= 5 && (board[i + 1][j] % 10 == c % 10) && (board[i + 2][j] % 10 == c % 10))
                {
                    clearCell(board, i, j - 1, score);
                    clearCell(board, i, j + 1, score);
                    clearCell(board, i + 1, j, score); // Tail 1
                    clearCell(board, i + 2, j, score); // Tail 2
                    clearCell(board, i, j, score);     // Center
                    found = true;
                    score += 100;
                }
            }

            //Case B: Vertical Bar with Horizontal Tail
            if (vMatch)
            {
                // T Pointing left
                if (j >= 2 && (board[i][j - 1] % 10 == c % 10) && (board[i][j - 2] % 10 == c % 10))
                {
                    clearCell(board, i - 1, j, score);
                    clearCell(board, i + 1, j, score);
                    clearCell(board, i, j - 1, score); // Tail 1
                    clearCell(board, i, j - 2, score); // Tail 2
                    clearCell(board, i, j, score);     // Center
                    found = true;
                    score += 100;
                }
                // T Pointing right
                else if (j <= 5 && (board[i][j + 1] % 10 == c % 10) && (board[i][j + 2] % 10 == c % 10))
                {
                    clearCell(board, i - 1, j, score);
                    clearCell(board, i + 1, j, score);
                    clearCell(board, i, j + 1, score); // Tail 1
                    clearCell(board, i, j + 2, score); // Tail 2
                    clearCell(board, i, j, score);     // Center
                    found = true;
                    score += 100;
                }
            }
        }
    }
    return found;
}

bool isLShape(int board[][8], int& score)
{
    bool found = false;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            int c = board[i][j];
            if (c == -1 || c == 4)
                continue;

            // We treat board[i][j] as the CORNER of the L-Shape
            // An L-Shape is 5 Candies: The Corner + 2 in one direction + 2 in perpendicular direction.

            // 1. Bottom-Right Shape ( |__ )
            // Requires: 2 candies Down, 2 candies Right
            if (i <= 5 && j <= 5)
            {
                if ((board[i + 1][j] % 10 == c % 10) && (board[i + 2][j] % 10 == c % 10) && (board[i][j + 1] % 10 == c % 10) && (board[i][j + 2] % 10 == c % 10))
                {
                    clearCell(board, i, j, score);       // Corner
                    clearCell(board, i + 1, j, score);   // Down 1
                    clearCell(board, i + 2, j, score);   // Down 2
                    clearCell(board, i, j + 1, score);   // Right 1
                    clearCell(board, i, j + 2, score);   // Right 2
                    found = true; score += 100;
                }
            }

            // 2. Bottom-Left Shape ( __| )
            // Requires: 2 candies Down, 2 candies Left
            if (i <= 5 && j >= 2)
            {
                if ((board[i + 1][j] % 10 == c % 10) && (board[i + 2][j] % 10 == c % 10) && (board[i][j - 1] % 10 == c % 10) && (board[i][j - 2] % 10 == c % 10))
                {
                    clearCell(board, i, j, score);       // Corner
                    clearCell(board, i + 1, j, score);   // Down 1
                    clearCell(board, i + 2, j, score);   // Down 2
                    clearCell(board, i, j - 1, score);   // Left 1
                    clearCell(board, i, j - 2, score);   // Left 2
                    found = true; score += 100;
                }
            }

            // 3. Top-Right Shape ( |¯¯ )
            // Requires: 2 candies Up, 2 candies Right
            if (i >= 2 && j <= 5)
            {
                if ((board[i - 1][j] % 10 == c % 10) && (board[i - 2][j] % 10 == c % 10) && (board[i][j + 1] % 10 == c % 10) && (board[i][j + 2] % 10 == c % 10))
                {
                    clearCell(board, i, j, score);       // Corner
                    clearCell(board, i - 1, j, score);   // Up 1
                    clearCell(board, i - 2, j, score);   // Up 2
                    clearCell(board, i, j + 1, score);   // Right 1
                    clearCell(board, i, j + 2, score);   // Right 2
                    found = true; score += 100;
                }
            }

            // 4. Top-Left Shape ( ¯¯| )
            // Requires: 2 candies Up, 2 candies Left
            if (i >= 2 && j >= 2)
            {
                if ((board[i - 1][j] % 10 == c % 10) && (board[i - 2][j] % 10 == c % 10) && (board[i][j - 1] % 10 == c % 10) && (board[i][j - 2] % 10 == c % 10))
                {
                    clearCell(board, i, j, score);       // Corner
                    clearCell(board, i - 1, j, score);   // Up 1
                    clearCell(board, i - 2, j, score);   // Up 2
                    clearCell(board, i, j - 1, score);   // Left 1
                    clearCell(board, i, j - 2, score);   // Left 2
                    found = true; score += 100;
                }
            }
        }
    }
    return found;
}
void applygravity(int board[8][8])
{
    for (int j = 0; j < 8; j++)
    {
        int empty = 7;
        for (int i = 7; i >= 0; i--)
        {
            if (board[i][j] != -1)
            {
                board[empty][j] = board[i][j];
                if (empty != i) board[i][j] = -1;
                empty--;
            }
        }
        for (; empty >= 0; empty--)
            board[empty][j] = rand() % 4;
    }
}

bool isadjacent(int r1, int c1, int r2, int c2)
{
    return (r1 == r2 && abs(c1 - c2) == 1) || (c1 == c2 && abs(r1 - r2) == 1);
}

//BOMB SWAP
bool resolveSpecialSwap(int board[8][8], int r1, int c1, int r2, int c2, int& score)
{
    int bombR, bombC, targetR, targetC;
    if (board[r1][c1] == 4)
    {
        bombR = r1;
        bombC = c1;
        targetR = r2;
        targetC = c2;
    }
    else if (board[r2][c2] == 4)
    {
        bombR = r2;
        bombC = c2;
        targetR = r1;
        targetC = c1;
    }
    else return false;

    int targetVal = board[targetR][targetC];
    if (targetVal == -1 || targetVal == 4)
        return false;

    int colorToKill = targetVal % 10;
    int count = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int c = board[i][j];
            if (c != -1 && c != 4 && (c % 10 == colorToKill))
            {
                clearCell(board, i, j, score);
                count++;
            }
        }
    }
    board[bombR][bombC] = -1;
    score += count * 20;
    return true;
}

bool swap_candies(int board[8][8], int r1, int c1, int r2, int c2, int& score)
{
    if (!isadjacent(r1, c1, r2, c2)) return false;

    if (board[r1][c1] == 4 || board[r2][c2] == 4)
    {
        if (resolveSpecialSwap(board, r1, c1, r2, c2, score))
            return true;
    }

    swap(board[r1][c1], board[r2][c2]);

    bool matched = false;
    if (match5by5(board, score))
        matched = true;
    else if (match4by4(board, score))
        matched = true;
    else if (isTShape(board, score))
        matched = true;
    else if (isLShape(board, score))
        matched = true;
    else if (match(board, score))
        matched = true;

    if (matched)
        return true;

    swap(board[r1][c1], board[r2][c2]);
    return false;
}

void DrawBoard(int board[8][8], int cursorRow, int cursorCol,
    bool firstSelected, int firstRow, int firstCol, int& score)
{
    const int startX = 180;
    const int startY = 170;
    const int cell = 55;
    const int gridW = 8, gridH = 8;

    // Draw thin grid lines
    for (int r = 0; r <= gridH; ++r)
    {
        int y = startY + r * cell;
        myLine(startX - 5, y, startX + gridW * cell + 5, y, 230);
    }
    for (int c = 0; c <= gridW; ++c)
    {
        int x = startX + c * cell;
        myLine(x, startY - 5, x, startY + gridH * cell + 5, 230);
    }














    // Draw candies
    for (int r = 0; r < gridH; ++r)
    {
        for (int c = 0; c < gridW; ++c)
        {
            int val = board[r][c];
            int x1 = startX + c * cell + 4;
            int y1 = startY + r * cell + 4;
            int x2 = x1 + (cell - 8);
            int y2 = y1 + (cell - 8);

            if (val == -1)
                continue;
            // NORMAL CANDIES
            if (val >= 0 && val <= 3)
            {
                if (val == 0)
                    myEllipse(x1, y1, x2, y2, 255, 165, 0); // yellow/orange
                else if (val == 1)
                    myRect(x1, y1, x2, y2, 0, 200, 0);      // green
                else if (val == 2)
                    myTriangle(x1, y1, x2, y2, 0, 120, 255);// blue
                else
                    myEllipse(x1, y1, x2, y2, 220, 40, 40); // red
            }
            //for bomb candy val = 4
            else if (val == 4)
            {
                // Draw red circle base
                myEllipse(x1, y1, x2, y2, 220, 40, 40);

                // Draw white star lines
                myLine(x1 + 10, y1 + 10, x2 - 10, y2 - 10, 255);
                myLine(x1 + 10, y2 - 10, x2 - 10, y1 + 10, 255);
            }

            //Horizontal Special (10–13)
            else if (val >= 10 && val <= 13)
            {
                int base = val % 10;

                // Base candy
                if (base == 0) myEllipse(x1, y1, x2, y2, 255, 165, 0);
                else if (base == 1) myRect(x1, y1, x2, y2, 0, 200, 0);
                else if (base == 2) myTriangle(x1, y1, x2, y2, 0, 120, 255);
                else myEllipse(x1, y1, x2, y2, 220, 40, 40);

                // Horizontal strip
                myLine(x1, (y1 + y2) / 2, x2, (y1 + y2) / 2, 255);
            }

            //Vertical Special (20–23)
            else if (val >= 20 && val <= 23)
            {
                int base = val % 10;

                // Base candy
                if (base == 0) myEllipse(x1, y1, x2, y2, 255, 165, 0);
                else if (base == 1) myRect(x1, y1, x2, y2, 0, 200, 0);
                else if (base == 2) myTriangle(x1, y1, x2, y2, 0, 120, 255);
                else myEllipse(x1, y1, x2, y2, 220, 40, 40);

                // Vertical stripe
                myLine((x1 + x2) / 2, y1, (x1 + x2) / 2, y2, 255);
            }

        }
    }

    // Draw Bright Cursor Border (outline only, using myLine)
    {
        int cx1 = startX + cursorCol * cell;
        int cy1 = startY + cursorRow * cell;
        int cx2 = cx1 + cell;
        int cy2 = cy1 + cell;

        myLine(cx1, cy1, cx2, cy1, 300); // top
        myLine(cx1, cy2, cx2, cy2, 300); // bottom
        myLine(cx1, cy1, cx1, cy2, 300); // left
        myLine(cx2, cy1, cx2, cy2, 300); // right
    }

    // First-selected highlight (if any)


}

void PrintBoardWithCursor(int board[][8], int cursorRow, int cursorCol, bool firstSelected, int firstRow, int firstCol, int& score, int timeLeft)
{
    system("cls");
    cout << " TIME: " << timeLeft << "s \t SCORE: " << score << endl;

    // Only draw using graphics function
    DrawBoard(board, cursorRow, cursorCol, firstSelected, firstRow, firstCol, score);

    cout << endl;
    cout << " Use keys: u=Up  d=Down  l=Left  r=Right  s=Select  q=Quit" << endl;

    if (!firstSelected)
        cout << " Press 's' to select a candy." << endl;
    else
        cout << " Selected cell: (" << firstRow << "," << firstCol << "). Now pick adjacent cell to swap." << endl;
}


void gameplaywithcursor(int board[8][8])
{
    int cursorRow = 0, cursorCol = 0;
    bool firstSelected = false;
    int firstRow = 0, firstCol = 0;
    char key = ' ';
    int score = 0;

    int timeLimit = 180;
    time_t startTime = time(0);

    while (true)
    {
        int timeLeft = timeLimit - (time(0) - startTime);
        if (timeLeft <= 0)
        {
            system("cls");
            cout << "TIME UP! Final Score: " << score << endl;
            break;
        }
        PrintBoardWithCursor(board, cursorRow, cursorCol, firstSelected, firstRow, firstCol, score, timeLeft);

        cout << "Enter command (u/d/l/r/s/q): ";
        cin >> key;

        if (key == 'q') break;

        // To move cursor
        if (key == 'u' && cursorRow > 0)
            cursorRow--;
        else if (key == 'd' && cursorRow < 7)
            cursorRow++;
        else if (key == 'l' && cursorCol > 0)
            cursorCol--;
        else if (key == 'r' && cursorCol < 7)
            cursorCol++;

        // Selection + Swap
        else if (key == 's')
        {
            // FIRST SELECTION
            if (!firstSelected)
            {
                firstSelected = true;
                firstRow = cursorRow;
                firstCol = cursorCol;
            }
            else
            {
                // SECOND SELECTION: try swap
                bool validSwap = swap_candies(board, firstRow, firstCol, cursorRow, cursorCol, score);

                if (validSwap)
                {
                    // Show explosion before gravity
                    PrintBoardWithCursor(board, cursorRow, cursorCol, false, -1, -1, score, timeLeft);
                    Sleep(500); //used so that we can see where gravity is being applied and wher is the destruction happening

                    applygravity(board);

                    // Chain reactions
                    bool moreCrush = true;
                    while (moreCrush)
                    {
                        moreCrush = false;

                        if (match5by5(board, score))
                            moreCrush = true;
                        else if (match4by4(board, score))
                            moreCrush = true;
                        else if (isTShape(board, score))
                            moreCrush = true;
                        else if (isLShape(board, score))
                            moreCrush = true;
                        else if (match(board, score))
                            moreCrush = true;

                        if (moreCrush)
                        {
                            PrintBoardWithCursor(board, cursorRow, cursorCol, false, -1, -1, score, timeLeft);
                            Sleep(500);
                            applygravity(board);
                        }
                    }
                }

                // deselect after attempting swap
                firstSelected = false;
            }
        }
    }
}


int main()
{
    srand((unsigned)time(0));
    int board[8][8];
    Grid(board);
    int score = 0;

    // for clean board initially
    bool cleaning = true;
    while (cleaning)
    {
        cleaning = false;
        if (match5by5(board, score) || match4by4(board, score) || isTShape(board, score) || isLShape(board, score) || match(board, score))
        {
            applygravity(board);
            score = 0;
            cleaning = true;
        }
    }
    // to Remove lucky bombs
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j] == 4)
                board[i][j] = rand() % 4;
        }
    }

    gameplaywithcursor(board);
    return 0;
}

