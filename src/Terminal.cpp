#include "Terminal.h"
#include <iostream>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <sys/select.h>
#endif

using namespace std;

// Clear the terminal screen
void ClearScreen() {
#ifdef _WIN32
    // Windows specific
    system("cls");
#else
    // Unix/Linux/macOS
    cout << "\033[2J\033[1;1H";
    cout.flush();
#endif
}

// Move cursor to specific position
void MoveCursor(int x, int y) {
#ifdef _WIN32
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    cout << "\033[" << y << ";" << x << "H";
    cout.flush();
#endif
}

// Set text color
void SetColor(int colorCode) {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
#else
    cout << "\033[" << colorCode << "m";
    cout.flush();
#endif
}

// Get immediate input without waiting for Enter
char GetImmediateInput() {
#ifdef _WIN32
    return _getch();
#else
    struct termios oldSettings, newSettings;
    char ch;
    
    // Get current terminal settings
    tcgetattr(STDIN_FILENO, &oldSettings);
    newSettings = oldSettings;
    
    // Disable canonical mode and echo
    newSettings.c_lflag &= ~(ICANON | ECHO);
    
    // Apply new settings
    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);
    
    // Read single character
    ch = getchar();
    
    // Restore old settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
    
    return ch;
#endif
}
