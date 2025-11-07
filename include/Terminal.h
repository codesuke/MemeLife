#ifndef TERMINAL_H
#define TERMINAL_H

// Terminal control functions
void ClearScreen();
void MoveCursor(int x, int y);
void SetColor(int colorCode);
char GetImmediateInput();

#endif // TERMINAL_H
