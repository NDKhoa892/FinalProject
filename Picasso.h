#ifndef _PICASSO_H_
#define _PICASSO_H_

#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

/// WINDOWS
const int WINDOW_HEIGHT = 32;
const int WINDOW_WIDTH = 128;

void initWindowSize() { 
    COORD coord; 
    coord.X = WINDOW_WIDTH + 100; 
    coord.Y = WINDOW_HEIGHT + 100; 

    SMALL_RECT Rect; 
    Rect.Top = 0; 
    Rect.Left = 0; 
    Rect.Bottom = WINDOW_HEIGHT - 1; 
    Rect.Right = WINDOW_WIDTH - 1; 

    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle 
    SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size 
    SetConsoleWindowInfo(Handle, TRUE, &Rect);            // Set Window Size 
} 

// void initWindowSize() {
//     HWND console = GetConsoleWindow();
//     RECT r;
//     GetWindowRect(console, &r);
//     MoveWindow(console, r.left, r.top, 953, 550, TRUE); 
// }

void gotoXY(short int x, short int y) {
    static HANDLE h = NULL;  
    if(!h) h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { y, x };  
    SetConsoleCursorPosition(h,c);
}

/// DESIGN
const int KEY_ARROW = 224;
const int KEY_UP = 72;
const int KEY_LEFT = 75;
const int KEY_DOWN = 80;
const int KEY_RIGHT = 77;

const int KEY_ENTER = 13;
const int KEY_BACKSPACE = 8;
const int KEY_CONTROL_V = 22;

const char CHAR_VERTICAL = char(179);               // |
const char CHAR_HORIZONTAL = char(196);             // -
const char CHAR_VERTICAL_HORIZONTAL = char(197);    // +
const char CHAR_VERTICAL_LEFT = char(180);          // -|
const char CHAR_VERTICAL_RIGHT = char(195);         // |-
const char CHAR_HORIZONTAL_DOWN = char(194);
const char CHAR_HORIZONTAL_UP = char(193);
const char CHAR_UPPER_LEFT = char(218);             
const char CHAR_UPPER_RIGHT = char(191);
const char CHAR_LOWER_LEFT = char(192);
const char CHAR_LOWER_RIGHT = char(217);

void drawTitle(int &x, int titleLines, const string* title) {
    for (int i = 0; i < titleLines; ++i) {
        gotoXY(x, (WINDOW_WIDTH - title[i].size()) / 2 - 1);
        cout << title[i];
        ++x;
    }
}

void drawMenu(int titleLines, const string* title, int optionsCount, const string* options, int &choice) {
    system("cls");

    int startX = (WINDOW_HEIGHT - titleLines - optionsCount * 3) / 2 - 1;

    if (titleLines == 0) startX -= 3;
    if (startX < 0) startX = 1;

    int x = startX;

    drawTitle(x, titleLines, title);

    for (int i = 0; i < optionsCount; ++i) {
        x += 3;

        gotoXY(x, (WINDOW_WIDTH - options[i].size()) / 2 - 1);
        cout << options[i];
    }

    while (1) {
        gotoXY(startX + titleLines + 3 + choice * 3, (WINDOW_WIDTH - options[choice].size()) / 2 - 4);
        cout << ">> " << options[choice] << " <<";

        gotoXY(WINDOW_HEIGHT - 1, 0);
        cout << "Use arrow keys to move, enter to choose and backspace to return.";

        int c = getch();

        gotoXY(startX + titleLines + 3 + choice * 3, (WINDOW_WIDTH - options[choice].size()) / 2 - 4);
        cout << "   " << options[choice] << "   ";

        switch (c) {
            case KEY_ARROW: {
                c = getch();

                switch (c) {
                    case KEY_DOWN:
                        choice = (choice + 1) % optionsCount;
                        break;
                    
                    case KEY_UP:
                        choice = (choice + optionsCount - 1) % optionsCount;
                        break;
                }

                break;
            }

            case KEY_ENTER:
                return;

            case KEY_BACKSPACE:
                choice = -1;
                return;
        }
    }
}

void drawTable(int startX, string tableTitle, int columnsCount, int* columnsWidth, string* columnsName, int rowsCount, string** table) {
    for (int i = 0; i < columnsCount; ++i)  {
        if (columnsWidth[i] < columnsName[i].size() + 2)
            columnsWidth[i] = columnsName[i].size() + 2;

        for (int j = 0; j < rowsCount; ++j)
            if (columnsWidth[i] < table[j][i].size() + 2)
                columnsWidth[i] = table[j][i].size() + 2;
    }

    gotoXY(startX, (WINDOW_WIDTH - tableTitle.size()) / 2);
    cout << tableTitle;

    int columnsWidthSum = 0;
    for (int i = 0; i < columnsCount; ++i) 
        columnsWidthSum += columnsWidth[i];

    int x = startX + 2;
    int startY = (WINDOW_WIDTH - columnsWidthSum - columnsCount + 1) / 2 - 1;
    int y = startY;

    if (startY < 0) startY = y = 1;

    gotoXY(x + 1, startY);
    for (int i = 0; i < columnsWidthSum + columnsCount - 1; ++i) 
        cout << CHAR_HORIZONTAL;

    gotoXY(x, y + (columnsWidth[0] - columnsName[0].size()) / 2);
    cout << columnsName[0];

    for (int i = 1; i < columnsCount; ++i) {
        y += columnsWidth[i - 1];

        gotoXY(x, y);
        cout << CHAR_VERTICAL;

        gotoXY(x + 1, y);
        cout << CHAR_VERTICAL_HORIZONTAL;

        ++y;

        gotoXY(x, y + (columnsWidth[i] - columnsName[i].size()) / 2);
        cout << columnsName[i];
    }

    ++x;
    for (int i = 0; i < rowsCount; ++i) {
        ++x;
        y = startY;
        
        gotoXY(x, y + (columnsWidth[0] - table[0][0].size()) / 2);
        cout << table[i][0];

        for (int j = 1; j < columnsCount; ++j) {
            y += columnsWidth[j - 1];

            gotoXY(x, y);
            cout << CHAR_VERTICAL;
            ++y;

            gotoXY(x, y + (columnsWidth[j] - table[i][j].size()) / 2);
            cout << table[i][j];
        }

        cout << '\n';
    }
}

void drawBox(string title, int containsCount, string* contains, int optionsCount, string* options, int &choice) {
    int width = title.size() + 4;

    for (int i = 0; i < containsCount; ++i)
        if (width < contains[i].size() + 4)
            width = contains[i].size() + 4;

    int optionsWidth = 0;
    for (int i = 0; i < optionsCount; ++i) 
        if (optionsWidth < options[i].size() + 6)
            optionsWidth = options[i].size() + 6;

    if (optionsWidth < (width - optionsCount - 1) / optionsCount + 1)
        optionsWidth = (width - optionsCount - 1) / optionsCount + 1; 

    if (width < optionsWidth * optionsCount + optionsCount + 1)
        width = optionsWidth * optionsCount + optionsCount + 1;

    int height = containsCount + 6;

    int x = (WINDOW_HEIGHT - height) / 2 - 1;

    int startY = (WINDOW_WIDTH - width) / 2 - 1;

    for (int i = x; i <= x + height - 1; ++i) {
        gotoXY(i, startY);
        for (int j = 0; j < width; ++j) cout << ' ';
    }
    gotoXY(WINDOW_HEIGHT - 1, 0);
    cout << "                                                                    ";

    gotoXY(x, startY);
    cout << CHAR_UPPER_LEFT;
    for (int i = 2; i < width; ++i)
        cout << CHAR_HORIZONTAL;
    cout << CHAR_UPPER_RIGHT;

    ++x;
    gotoXY(x, startY);
    cout << CHAR_VERTICAL << ' ' << title;
    gotoXY(x, startY + width - 1);
    cout << CHAR_VERTICAL;

    ++x;
    gotoXY(x, startY);
    cout << CHAR_VERTICAL_RIGHT;
    for (int i = 2; i < width; ++i)
        cout << CHAR_HORIZONTAL;
    cout << CHAR_VERTICAL_LEFT;

    for (int i = 0; i < containsCount; ++i) {
        ++x;
        gotoXY(x, startY);
        cout << CHAR_VERTICAL << ' ' << contains[i];
        gotoXY(x, startY + width - 1);
        cout << CHAR_VERTICAL;
    }

    ++x;
    gotoXY(x, startY);
    cout << CHAR_VERTICAL_RIGHT;
    for (int i = 2; i < width; ++i)
        cout << CHAR_HORIZONTAL;
    cout << CHAR_VERTICAL_LEFT;

    gotoXY(x + 2, startY);
    cout << CHAR_LOWER_LEFT;
    for (int i = 2; i < width; ++i)
        cout << CHAR_HORIZONTAL;
    cout << CHAR_LOWER_RIGHT;

    ++x;
    int y = startY;
    gotoXY(x, y);
    cout << CHAR_VERTICAL;
    for (int i = 0; i < optionsCount; ++i) {
        ++y;
        gotoXY(x, y + (optionsWidth - options[i].size()) / 2);
        cout << options[i];

        y += optionsWidth;

        gotoXY(x - 1, y);
        if (i < optionsCount - 1)
            cout << CHAR_HORIZONTAL_DOWN;

        gotoXY(x, y);
        cout << CHAR_VERTICAL;

        gotoXY(x + 1, y);
        if (i < optionsCount - 1)
            cout << CHAR_HORIZONTAL_UP;
    }

    while (1) {
        gotoXY(x, startY + choice * optionsWidth + choice + 1 + (optionsWidth - options[choice].size() - 4) / 2);
        cout << "> " << options[choice] << " <";

        gotoXY(WINDOW_HEIGHT - 1, 0);
        cout << "Use arrow keys to move, enter to choose.";

        int c = getch();

        gotoXY(x, startY + choice * optionsWidth + choice + 1 + (optionsWidth - options[choice].size() - 4) / 2);
        cout << "  " << options[choice] << "  ";

        switch (c) {
            case KEY_ARROW: {
                c = getch();

                switch (c) {
                    case KEY_RIGHT:
                        choice = (choice + 1) % optionsCount;
                        break;
                    
                    case KEY_LEFT:
                        choice = (choice + optionsCount - 1) % optionsCount;
                        break;
                }

                break;
            }

            case KEY_ENTER:
                return;
        }
    }
}

bool drawYesNoBox(string title, string message) {
    const int containsCount = 3;
    string contains[containsCount] = { 
        "",
        message,
        "" 
    };

    const int optionsCount = 2;
    string options[optionsCount] = { "No", "Yes" };

    int choice = 1;

    drawBox(title, containsCount, contains, optionsCount, options, choice);

    return choice == 1;
}

void drawOkayBox(string title, string message) {
    const int containsCount = 3;
    string contains[containsCount] = { 
        "",
        message,
        "" 
    };

    const int optionsCount = 1;
    string options[optionsCount] = { "Okay" };

    int choice = 0;

    drawBox(title, containsCount, contains, optionsCount, options, choice);
}

string getClipboardData() {
    // Try opening the clipboard
    if (! OpenClipboard(nullptr)) return "";

    // Get handle of clipboard object for ANSI text
    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == nullptr) return "";

    // Lock the handle to get the actual text pointer
    char * pszText = static_cast<char*>(GlobalLock(hData));
    if (pszText == nullptr) return "";

    // Save text in a string class instance
    string text(pszText);

    // Release the lock
    GlobalUnlock(hData);

    // Release the clipboard
    CloseClipboard();

    return text;
}

bool drawInputBox(string title, int fieldLength, int inputsCount, string* instructions, string* inputsData) {
    int width = title.size() + 4;

    if (width < fieldLength + 4)
        width = fieldLength + 4;

    int optionsWidth = 12;

    if (optionsWidth < (width - 3) / 2 + 1)
        optionsWidth = (width - 3) / 2 + 1; 

    if (width < optionsWidth * 2 + 3)
        width = optionsWidth * 2 + 3;

    int height = inputsCount + 6;

    int startX = (WINDOW_HEIGHT - height) / 2 - 1;
    int x = startX;

    int startY = (WINDOW_WIDTH - width) / 2 - 1;

    for (int i = x; i <= x + height - 1; ++i) {
        gotoXY(i, startY);
        for (int j = 0; j < width; ++j) cout << ' ';
    }
    gotoXY(WINDOW_HEIGHT - 1, 0);
    cout << "                                                                    ";

    gotoXY(x, startY);
    cout << CHAR_UPPER_LEFT;
    for (int i = 2; i < width; ++i)
        cout << CHAR_HORIZONTAL;
    cout << CHAR_UPPER_RIGHT;

    ++x;
    gotoXY(x, startY);
    cout << CHAR_VERTICAL << ' ' << title;
    gotoXY(x, startY + width - 1);
    cout << CHAR_VERTICAL;

    ++x;
    gotoXY(x, startY);
    cout << CHAR_VERTICAL_RIGHT;
    for (int i = 2; i < width; ++i)
        cout << CHAR_HORIZONTAL;
    cout << CHAR_VERTICAL_LEFT;

    for (int i = 0; i < inputsCount; ++i) {
        ++x;
        gotoXY(x, startY);
        cout << CHAR_VERTICAL << ' ' << instructions[i] + inputsData[i];
        gotoXY(x, startY + width - 1);
        cout << CHAR_VERTICAL;
    }

    ++x;
    gotoXY(x, startY);
    cout << CHAR_VERTICAL_RIGHT;
    for (int i = 2; i < width; ++i)
        cout << CHAR_HORIZONTAL;
    cout << CHAR_VERTICAL_LEFT;

    gotoXY(x + 2, startY);
    cout << CHAR_LOWER_LEFT;
    for (int i = 2; i < width; ++i)
        cout << CHAR_HORIZONTAL;
    cout << CHAR_LOWER_RIGHT;

    ++x;
    int y = startY;

    gotoXY(x, y);
    cout << CHAR_VERTICAL;

    ++y;
    gotoXY(x, y + (optionsWidth - 6) / 2);
    cout << "Cancel";

    y += optionsWidth;
    gotoXY(x - 1, y);
    cout << CHAR_HORIZONTAL_DOWN;

    gotoXY(x, y);
    cout << CHAR_VERTICAL;

    gotoXY(x + 1, y);
    cout << CHAR_HORIZONTAL_UP;

    ++y;
    gotoXY(x, y + (optionsWidth - 2) / 2);
    cout << "Okay";

    y += optionsWidth;
    gotoXY(x, y);
    cout << CHAR_VERTICAL;

    int choice = 1;
    int curInput = 0;

    while (1) {
        gotoXY(x, startY + choice * optionsWidth + choice + 1 + (optionsWidth - 10) / 2);
        cout << (choice == 0 ? "> Cancel <" : " > Okay < ");

        y = startY + instructions[curInput].size() + inputsData[curInput].size() + 2;
        gotoXY(startX + curInput + 3, y);

        int c = getch();

        switch (c) {
            case KEY_ARROW: {
                c = getch();

                switch (c) {
                    case KEY_RIGHT:
                    case KEY_LEFT: 
                        gotoXY(x, startY + choice * optionsWidth + choice + 1 + (optionsWidth - 10) / 2);
                        cout << (choice == 0 ? "  Cancel  " : "   Okay   ");

                        choice = (choice + 1) % 2;

                        break;
                    

                    case KEY_DOWN: 
                        curInput = (curInput + 1) % inputsCount;
                        break;
                    

                    case KEY_UP: 
                        curInput = (curInput + inputsCount - 1) % inputsCount;
                        break;
                }

                break;
            }
            
            case KEY_BACKSPACE:
                if (inputsData[curInput].size() > 0) {
                    gotoXY(startX + curInput + 3, y - 1);
                    cout << ' ';

                    inputsData[curInput].pop_back();
                }

                break;

            case KEY_CONTROL_V: {
                string clipboard = getClipboardData();

                cout << clipboard;
                inputsData[curInput] += clipboard;

                break;
            }

            case KEY_ENTER:
                return (choice == 1);

            default: 
                cout << char(c);
                inputsData[curInput] += char(c);
        }
    }
}

bool drawInputBox(string title, int fieldLength, string instruction, string &inputData) {
    string instructions[1] = { instruction };

    string inputsData[1] = { inputData };

    bool res = drawInputBox(title, fieldLength, 1, instructions, inputsData);

    inputData = inputsData[0];
    
    return res;
}

#endif
