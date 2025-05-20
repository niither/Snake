#include "snake.h"
using namespace std;

int main()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetupConsole(h);
    int bestScore = LoadScore();

    while (true) {
        int menuChoice = ShowMenu(h, bestScore);
        if (menuChoice == 3) {
            break;
        }
        int difficulty = menuChoice;

        system("cls");
        int score = InitGame(h, difficulty);

        if (score > bestScore) {
            SaveScore(score);
            bestScore = score;
        }

        system("cls");
        cout << "Game Over! Final Score: " << score << "\nBest Score: " << bestScore << "\nPress any key to return to menu...";
        _getch();
    }
    return 0;
}