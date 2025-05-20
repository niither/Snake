#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
using namespace std;

enum Color : short { // Color codes for console text
	BLACK, BLUE, GREEN, AQUA, RED, PURPLE, YELLOW, WHITE, GRAY, LIGHTBLUE, 
	LIGHTGREEN, LIGHTAQUA, LIGHTRED, LIGHTPURPLE, LIGHTYELLOW, BRIGHTWHITE
};
enum Key : short { //Key codes for direction and escape
	UP = 72, DOWN = 80, LEFT = 75, RIGHT = 77, ESCAPE = 27
};
enum GameObj : short { // Game objects codes
    HALL, WALL, FOOD 
};

// Function prototypes
void SetupConsole(HANDLE h);
void gotoXY(HANDLE h, int x, int y);
void printTitle(HANDLE h);
int ShowMenu(HANDLE h, int bestScore);
int InitGame(HANDLE h, int difficulty);
void SaveScore(int score);
int LoadScore();

void SetupConsole(HANDLE h) {
	// Hide the cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(h, &cursorInfo);
	// Set the console size and title
    system("mode con cols=100 lines=50");
    system("title Snake");
}
void gotoXY(HANDLE h, int x, int y) { // Move the cursor to (x, y) position
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(h, c);
}
void printTitle(HANDLE h) { // Print the pseudographic title
    SetConsoleTextAttribute(h, GREEN);
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
	int lines = sizeof(title) / sizeof(title[0]); // Flexible line count
    for (int i = 0; i < lines; ++i) {
        gotoXY(h, 28, 1 + i);
        cout << title[i] << "\n";
    }
    SetConsoleTextAttribute(h, WHITE);
    cout << "\n\n";
}
int ShowMenu(HANDLE h, int bestScore) {
    int choice;
    do {
        system("cls");
        printTitle(h);
        SetConsoleTextAttribute(h, LIGHTPURPLE);
        cout << "\t\t\t\t\t\t======== MENU ========\n\n";
        SetConsoleTextAttribute(h, WHITE);
        cout << "\t\t\t\t\t\t1. Easy\n";
        cout << "\t\t\t\t\t\t2. Hard\n";
        cout << "\t\t\t\t\t\t3. Exit\n\n";
        SetConsoleTextAttribute(h, LIGHTPURPLE);
        cout << "\t\t\t\t\t\tBest score: " << bestScore << "\n";
        cout << "\t\t\t\t\t\tYour choice (1-3): ";
        SetConsoleTextAttribute(h, WHITE);
        cin >> choice;
    } while (choice < 1 || choice > 3);
    return choice;
}
int InitGame(HANDLE h, int difficulty) {
    srand(time(0));
    const int height = 20;
    const int width = 40;
    const int maxSize = height * width;

	// Initialize the map - 2D array memory allocation
    int** map = new int* [height];
    for (int i = 0; i < height; i++) {
        map[i] = new int[width];
        for (int j = 0; j < width; j++) {
            if (i == 0 || j == 0 || i == height - 1 || j == width - 1)
                map[i][j] = WALL;
            else
                map[i][j] = HALL;
        }
    }

	// Initialize the snake coordinates
    int snakeX[maxSize], snakeY[maxSize];
	int length = 3; // Starting length of the snake
    int dir = RIGHT; // Starting direction
    snakeX[0] = width / 2;
    snakeY[0] = height / 2;
    snakeX[1] = snakeX[0] - 1;
    snakeY[1] = snakeY[0];
    snakeX[2] = snakeX[1] - 1;
    snakeY[2] = snakeY[1];

    int foodX = rand() % (width - 2) + 1;
    int foodY = rand() % (height - 2) + 1;
    map[foodY][foodX] = FOOD;

    int score = 0;
    bool gameOver = false;

	while (!gameOver) { // Main game loop
        if (_kbhit()) {
            int key = _getch();
            if ((key == UP && dir != DOWN) ||
                (key == DOWN && dir != UP) ||
                (key == LEFT && dir != RIGHT) ||
                (key == RIGHT && dir != LEFT)) {
                dir = key;
            }
            if (key == ESCAPE) {
                break;
            }
        }
        for (int i = length - 1; i > 0; i--) {
            snakeX[i] = snakeX[i - 1];
            snakeY[i] = snakeY[i - 1];
        }
        switch (dir) {
        case UP:
            snakeY[0]--;
            break;
        case DOWN:
            snakeY[0]++;
            break;
        case LEFT:
            snakeX[0]--;
            break;
        case RIGHT:
            snakeX[0]++;
            break;
        }
		// Check for collisions
        if (map[snakeY[0]][snakeX[0]] == WALL) {
            gameOver = true;
        }
        for (int i = 1; i < length; i++) {
            if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
                gameOver = true;
        }
		// Check for food
        if (snakeX[0] == foodX && snakeY[0] == foodY) {
            length++;
            score += 10;
			// Generate new food
            do {
                foodX = rand() % (width - 2) + 1;
                foodY = rand() % (height - 2) + 1;
            } while (map[foodY][foodX] != HALL);
            map[foodY][foodX] = FOOD;
			// Generate walls (for hard difficulty)
            if (difficulty == 2) {
                int wallX, wallY;
                do {
                    wallX = rand() % (width - 2) + 1;
                    wallY = rand() % (height - 2) + 1;
                } while (map[wallY][wallX] != HALL);
                map[wallY][wallX] = WALL;
            }
        }

        gotoXY(h, 0, 0);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                bool printed = false;
                if (x == foodX && y == foodY) {
                    SetConsoleTextAttribute(h, LIGHTRED);
                    cout << '*';
                    printed = true;
                }
                else {
                    for (int i = 0; i < length; i++) {
                        if (snakeX[i] == x && snakeY[i] == y) {
                            SetConsoleTextAttribute(h, LIGHTGREEN);
                            cout << (i == 0 ? 'O' : 'o');
                            printed = true;
                            break;
                        }
                    }
                }
                if (!printed) {
                    if (map[y][x] == WALL) {
                        SetConsoleTextAttribute(h, BLUE);
                        cout << '#';
                    }
                    else {
                        cout << ' ';
                    }
                }
            }
            SetConsoleTextAttribute(h, WHITE);
            cout << '\n';
        }
        cout << "Score: " << score << '\n';
		Sleep(100); // Game speed
    }
    SaveScore(score);

	// Free allocated memory
    for (int i = 0; i < height; ++i)
        delete[] map[i];
    delete[] map;
    return score;
}
void SaveScore(int score) { // Save the score to .txt file
    int currentScore = LoadScore();
    FILE* file;
    int error = fopen_s(&file,
        "C:/Users/user/Desktop/STEP/cpp/snake/bestScore.txt", "w");
    if (score > currentScore && error == 0 && file != nullptr) {
        string strScore = to_string(score);
        const char* cScore = strScore.c_str();
        fputs(cScore, file);
        fclose(file);
    }
}
int LoadScore() { // Load the best score from .txt file
    int bestScore = 0;
    FILE* file;
    int error = fopen_s(&file,
        "C:/Users/user/Desktop/STEP/cpp/snake/bestScore.txt", "r");
    if (error == 0 && file != nullptr) {
        char line[4];
        if (fgets(line, sizeof(line), file)) {
            bestScore = atoi(line); // Convert string to int
        }
        fclose(file);
    }
    return bestScore;
}