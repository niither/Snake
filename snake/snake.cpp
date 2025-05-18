#include "snake.h"
using namespace std;

int main()
{
	SetupConsole();
    int bestScore = LoadScore();

    while (true) {
        int menuChoice = ShowMenu(bestScore);
        if (menuChoice == 4) break;

        int difficulty = menuChoice;

        bestScore = LoadScore();
        system("cls");
        cout << "Game Over! Final Score: " << score << endl;
        cout << "Best Score: " << bestScore << endl;
        cout << "Press any key to return to menu...";
        _getch();
    }
    return 0;
}