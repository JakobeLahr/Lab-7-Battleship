/*
    Jakobe Lahr
    Computer Science FA_24
    Due: 12/17/24
    Lab 7: This lab is a game of Battleship between the 
    user and the computer where you pick your own 
    locations for your ships and the computer is 
    randomized for all. You as the player also get 
    to pick your target each round and it tracks the 
    amount of ships left to end when either party destroyes
    5 of them. 
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

const int GRID_SIZE = 10;
const char WATER = '~';
const char SHIP = '#';
const char HIT = 'H';
const char MISS = 'M';

void startGrid(char grid[GRID_SIZE][GRID_SIZE]);
void displayGrid(const char grid[GRID_SIZE][GRID_SIZE], const string& label);
bool placeShip(char grid[GRID_SIZE][GRID_SIZE], int size, int x, int y, char direction);
bool isValidPlacement(char grid[GRID_SIZE][GRID_SIZE], int size, int x, int y, char direction);
void placeComputerShips(char grid[GRID_SIZE][GRID_SIZE]);
void userPlaceShips(char grid[GRID_SIZE][GRID_SIZE]);
bool isValidCoordinate(int x, int y);
void userTurn(char computerShips[GRID_SIZE][GRID_SIZE], char userGuesses[GRID_SIZE][GRID_SIZE]);
void computerTurn(char userShips[GRID_SIZE][GRID_SIZE], char computerGuesses[GRID_SIZE][GRID_SIZE]);
bool isGameOver(char grid[GRID_SIZE][GRID_SIZE]);
bool askReplay();

int countRemainingShips(char grid[GRID_SIZE][GRID_SIZE]);

int main() {
    srand(time(0)); //random number generator

    bool playAgain = true;

    while (playAgain) {
        char userShips[GRID_SIZE][GRID_SIZE], userGuesses[GRID_SIZE][GRID_SIZE];
        char computerShips[GRID_SIZE][GRID_SIZE], computerGuesses[GRID_SIZE][GRID_SIZE];

        startGrid(userShips);     //grids
        startGrid(userGuesses);
        startGrid(computerShips);
        startGrid(computerGuesses);

        userPlaceShips(userShips);            //place ships
        placeComputerShips(computerShips);

        bool gameOver = false;
        while (!gameOver) {
            cout << "\nYour Ships:" << endl;
            displayGrid(userShips, "Your Ships");
            cout << "\nYour Guesses (Enemy Grid):" << endl;
            displayGrid(userGuesses, "Your Guesses");
            cout << "\nComputer Guesses (Your Grid):" << endl;
            displayGrid(computerGuesses, "Computer Guesses");

            userTurn(computerShips, userGuesses);
            if (isGameOver(computerShips)) {
                cout << "You win! All enemy ships are destroyed." << endl;
                gameOver = true;
                break;
            }

            computerTurn(userShips, computerGuesses);
            if (isGameOver(userShips)) {
                cout << "You lose! All your ships are destroyed." << endl;
                gameOver = true;
                break;
            }
        }

        playAgain = askReplay();
    }

    cout << "Thanks for playing Battleship!" << endl;
    return 0;
}

void startGrid(char grid[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = WATER;
        }
    }
}

void displayGrid(const char grid[GRID_SIZE][GRID_SIZE], const string& label) {           //i hate arrays after this
    cout << label << endl;
    cout << "  ";
    for (int i = 0; i < GRID_SIZE; i++) {
        cout << i << " ";
    }
    cout << endl;

    for (int i = 0; i < GRID_SIZE; i++) {
        cout << i << " ";
        for (int j = 0; j < GRID_SIZE; j++) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}

bool isValidCoordinate(int x, int y) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE;
}

bool isValidPlacement(char grid[GRID_SIZE][GRID_SIZE], int size, int x, int y, char direction) {
    if (direction == 'H') {
        if (y + size > GRID_SIZE) return false;
        for (int i = 0; i < size; i++) {
            if (grid[x][y + i] != WATER) return false;
        }
    }
    else if (direction == 'V') {
        if (x + size > GRID_SIZE) return false;
        for (int i = 0; i < size; i++) {
            if (grid[x + i][y] != WATER) return false;
        }
    }
    return true;
}

bool placeShip(char grid[GRID_SIZE][GRID_SIZE], int size, int x, int y, char direction) {
    if (!isValidPlacement(grid, size, x, y, direction)) return false;

    if (direction == 'H') {
        for (int i = 0; i < size; i++) {
            grid[x][y + i] = SHIP;
        }
    }
    else if (direction == 'V') {
        for (int i = 0; i < size; i++) {
            grid[x + i][y] = SHIP;
        }
    }
    return true;
}

void placeComputerShips(char grid[GRID_SIZE][GRID_SIZE]) {
    int shipSizes[] = { 5, 4, 3, 3, 2 };

    for (int size : shipSizes) {
        bool placed = false;
        while (!placed) {
            int x = rand() % GRID_SIZE;
            int y = rand() % GRID_SIZE;
            char direction = (rand() % 2 == 0) ? 'H' : 'V';
            placed = placeShip(grid, size, x, y, direction);
        }
    }
}

void userPlaceShips(char grid[GRID_SIZE][GRID_SIZE]) {
    int shipSizes[] = { 5, 4, 3, 3, 2 };
    string directions = "HV";

    for (int size : shipSizes) {
        bool placed = false;
        while (!placed) {
            displayGrid(grid, "Your Ship Placement");
            cout << "Place a ship of size " << size << "." << endl;
            int x, y;
            char direction;
            cout << "Enter starting coordinates (x y): ";
            cin >> x >> y;
            cout << "Enter direction (H for horizontal, V for vertical): ";
            cin >> direction;

            if (isValidCoordinate(x, y) && directions.find(direction) != string::npos) {
                placed = placeShip(grid, size, x, y, direction);
                if (!placed) cout << "Invalid placement. Try again." << endl;
            }
            else {
                cout << "Invalid input. Try again." << endl;
            }
        }
    }
}

void userTurn(char computerShips[GRID_SIZE][GRID_SIZE], char userGuesses[GRID_SIZE][GRID_SIZE]) {
    int x, y;
    bool validShot = false;

    while (!validShot) {
        displayGrid(userGuesses, "Your Guesses (Enemy Grid)");
        cout << "Enter coordinates to fire (x y): ";
        cin >> x >> y;

        if (isValidCoordinate(x, y) && userGuesses[x][y] == WATER) {
            validShot = true;
            if (computerShips[x][y] == SHIP) {
                cout << "HIT!" << endl;
                userGuesses[x][y] = HIT;
                computerShips[x][y] = HIT;
            }
            else {
                cout << "MISS!" << endl;
                userGuesses[x][y] = MISS;
            }
        }
        else {
            cout << "Invalid or repeated shot. Try again." << endl;
        }
    }
}

void computerTurn(char userShips[GRID_SIZE][GRID_SIZE], char computerGuesses[GRID_SIZE][GRID_SIZE]) {
    int x, y;
    bool validShot = false;

    while (!validShot) {
        x = rand() % GRID_SIZE;
        y = rand() % GRID_SIZE;

        if (computerGuesses[x][y] == WATER) {
            validShot = true;
            if (userShips[x][y] == SHIP) {
                cout << "Computer fires at (" << x << ", " << y << "): HIT!" << endl;
                computerGuesses[x][y] = HIT;
                userShips[x][y] = HIT;
            }
            else {
                cout << "Computer fires at (" << x << ", " << y << "): MISS!" << endl;
                computerGuesses[x][y] = MISS;
            }
        }
    }
}

bool isGameOver(char grid[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] == SHIP) return false;
        }
    }
    return true;
}

bool askReplay() {
    char choice;
    cout << "Do you want to play again? (y/n): ";
    cin >> choice;
    return choice == 'y' || choice == 'Y';
}
