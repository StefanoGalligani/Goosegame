#ifndef IOCOLOR_H_INCLUDED
#define IOCOLOR_H_INCLUDED
void gotoXY(short x, short y) {
    COORD CursorPos = {x, y};
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, CursorPos);
}

void setColor(int bg, int fg) {
    int val;
    if (bg<0) bg=0;
    if (fg>15) fg=15;
    val=bg*16+fg;
    SetConsoleTextAttribute(
        GetStdHandle(STD_OUTPUT_HANDLE),
        val);
}
#endif // IOCOLOR_H_INCLUDED
