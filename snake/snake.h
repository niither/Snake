#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

enum Color : short {
	BLACK, BLUE, GREEN, AQUA, RED, PURPLE, YELLOW, WHITE, GRAY, LIGHTBLUE, 
	LIGHTGREEN, LIGHTAQUA, LIGHTRED, LIGHTPURPLE, LIGHTYELLOW, BRIGHTWHITE
};
enum Key : short {
	UP = 72, DOWN = 80, LEFT = 75, RIGHT = 77, ENTER = 13, ESCAPE = 27, 
	SPACE = 32, BACKSPACE = 8
};

void SetupConsole();
void gotoXY(int x, int y);
void printTitle();
int ShowMenu(int bestScore);
void SaveScore(int score);
int LoadScore();

void SetupConsole() {
    CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(h, &cursorInfo);
	system("mode con cols=100 lines=50");
	system("title Snake");
}
void gotoXY(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(h, c);
}
void printTitle() {
    const char* title[] = {
        "    ########                                ###                  ",
        "  ###########                               ###                  ",
        " ###       #                                ##     ##       ###  ",
        " ##           ##   ##           #####       ##   ####      ##### ",
        "  #####      ### #####        ###  ##       ##  ###       ##  ## ",
        "    #######  ## ### ###      ##    ##     ### ###        ######  ",
        "         ### ####    ##   ####    ###  #########        #####    ",
        "         ### ###     #########  #########  ### ###    ### ##  ###",
        "###########  ##       ####  ######   ##    ##   #######   ###### ",
        " ########                     ##            #      ###     ####  "
    };
    int lines = sizeof(title) / sizeof(title[0]);
    for (int i = 0; i < lines; ++i) {
        gotoXY(28, 1 + i);
        cout << title[i] << "\n";
    }
    cout << "\n\n";
}
int ShowMenu(int bestScore) {
    int choice;
    do {
        system("cls");
        printTitle();
        cout << "\t\t\t\t\t\t======== MENU ========\n\n";
        cout << "\t\t\t\t\t\t1. Easy\n";
        cout << "\t\t\t\t\t\t2. Medium\n";
        cout << "\t\t\t\t\t\t3. Hard\n";
        cout << "\t\t\t\t\t\t4. Exit\n\n";
        cout << "\t\t\t\t\t\tBest score: " << bestScore << "\n";
        cout << "\t\t\t\t\t\tYour choice (1-4): ";
        cin >> choice;
    } while (choice < 1 || choice > 4);
    return choice;
}
void SaveScore(int score) {
    int currentScore = LoadScore();
    FILE* file;
    int error = fopen_s(&file, "C:/Users/user/Desktop/STEP/cpp/snake/bestScore.txt", "w");
    if (score > currentScore && error == 0 && file != nullptr) {
        string strScore = to_string(score);
        const char* cScore = strScore.c_str();
        fputs(cScore, file);
        fclose(file);
    }
}
int LoadScore() {
    int bestScore = 0;
    FILE* file;
    int error = fopen_s(&file, "C:/Users/user/Desktop/STEP/cpp/snake/bestScore.txt", "r");
    if (error == 0 && file != nullptr) {
        char line[200];
        bestScore = (int)fgets(line, 199, file);
        fclose(file);
    }
    return bestScore;
}