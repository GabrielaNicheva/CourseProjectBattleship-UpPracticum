#include <iostream>
#include <cstdlib>
#include <ctime>
#include<windows.h>
using std::cout;
using std::cin;
using std::endl;

const int SHIPS_TYPES = 4;
const int SHIPS_COUNT = 10;
const int SHIP_LENGTHS[SHIPS_TYPES] = { 4,3,2,1 };
const int SHIP_COUNTS[SHIPS_TYPES] = { 1,2,3,4 };
const int GRID_TYPES = 3;
const int GRID_SIZES[GRID_TYPES] = { 6,12,15 };
enum boardElements {
	water,
	hit,
	miss,
	sunk,
	ship
};

#pragma region ShipsPositioning

bool coordinateValidation(int firstCoordinate, int secondCoordinate, int gridSize, int shipLength, char direction) {
	if (direction == 'H' || direction == 'h') {
		if (secondCoordinate + shipLength - 1 >= gridSize) {
			return false;
		}
	}

	else if (direction == 'V' || direction == 'v') {
		if (firstCoordinate + shipLength - 1 >= gridSize) {
			return false;
		}
	}

	return true;
}

bool isThePositionFree(char direction, int gridSize, int firstCoordinate, int secondCoordinate, int shipLength, int** board) {
	if (direction == 'V' || direction == 'v') {
		for (int i = firstCoordinate; i < shipLength + firstCoordinate; i++) {
			if (board[i][secondCoordinate] >= ship) {
				return false;
			}
		}
	}

	else if (direction == 'H' || direction == 'h') {
		for (int i = secondCoordinate; i < shipLength + secondCoordinate; i++) {
			if (board[firstCoordinate][i] >= ship) {
				return false;
			}
		}
	}
	return true;
}

void shipModification(char direction, int gridSize, int firstCoordinate, int secondCoordinate, int shipLength, int** playerBoard, int command, int shipId) {
	if (direction == 'V' || direction == 'v') {
		for (int i = firstCoordinate; i < shipLength + firstCoordinate; i++) {
			if (command == ship) {
				playerBoard[i][secondCoordinate] = ship + shipId;
			}
			else if (command == sunk) {
				playerBoard[i][secondCoordinate] = sunk;
			}
		}
	}

	else if (direction == 'H' || direction == 'h') {
		for (int i = secondCoordinate; i < shipLength + secondCoordinate; i++) {
			if (command == ship) {
				playerBoard[firstCoordinate][i] = ship + shipId;
			}
			else if (command == sunk) {
				playerBoard[firstCoordinate][i] = sunk;
			}
		}
	}

}

void coordinatesInput(int& firstCoordinate, int& secondCoordinate, const int gridSize) {
	while (true) {
		cin >> firstCoordinate;
		firstCoordinate--;
		cin >> secondCoordinate;
		secondCoordinate--;

		if ((firstCoordinate < 0 || firstCoordinate + 1 > gridSize) || (secondCoordinate < 0 || secondCoordinate + 1 > gridSize)) {
			cout << "Incorrect coordinates! They are out of bounds." << endl;
			cout << "Please enter the coordinates again" << endl;
		}
		else break;
	}
}

void directionInput(char& direction) {
	cout << "Please enter the direction of your ship: H for horizontal or V for vertical" << endl;
	while (true)
	{
		cin >> direction;
		if (direction != 'H' && direction != 'h' && direction != 'V' && direction != 'v') {
			break;
		}
		cout << "Invalid direction!" << endl;
		cout << "Please enter H (horizontal) or V (vertical)" << endl;
	}
}

void manualShipPositioning(int** playerBoard, int gridSize) {
	int count = 0;
	for (int i = 0; i < SHIPS_TYPES; i++) {
		for (int j = 0; j < SHIP_COUNTS[i]; j++) {
			cout << "How do you want to position your " << j + 1 << "th ship with length " << SHIP_LENGTHS[i] << endl;
			cout << "Please enter coordinates for the start of your ship (ex: 3 4)" << endl;
			cout << "Your choice: ";
			int firstCoordinate = 0;
			int secondCoordinate = 0;
			char direction;
			while (true) {
				coordinatesInput(firstCoordinate, secondCoordinate, gridSize);
				if (SHIP_LENGTHS[i] != 1) {
					directionInput(direction);
				}							

				if (coordinateValidation(firstCoordinate, secondCoordinate, gridSize, SHIP_LENGTHS[i], direction)) {
					if (isThePositionFree(direction, gridSize, firstCoordinate, secondCoordinate, SHIP_LENGTHS[i], playerBoard)) {
						shipModification(direction, gridSize, firstCoordinate, secondCoordinate, SHIP_LENGTHS[i], playerBoard, ship, count);
						cout << "You have successfully placed your ship!" << endl;
						// cout player's board with current ships
						break;
					}
					else {
						cout << "There is already a ship on this place!" << endl;
						cout << "Please enter again the coordinates and the direction" << endl;
					}
				}
				else {
					cout << "Your ship it's out of bounds." << endl;
					cout << "Please enter the coordinates again" << endl;
				}

			}
			count++;
		}
	}
}

void automaticShipPositioning(int** playerBoard, int gridSize) {
	int count = 0;
	for (int i = 0; i < SHIPS_TYPES; i++) {
		for (int j = 0; j < SHIP_COUNTS[i]; j++) {
		
			int firstCoordinate = 0;
			int secondCoordinate = 0;
			char direction;
			while (true) {
				firstCoordinate = rand() % gridSize;
				secondCoordinate = rand() % gridSize;
				int randomDirection = rand() % 2;
				(randomDirection) ? direction = 'H' : direction = 'V';

				if (coordinateValidation(firstCoordinate, secondCoordinate, gridSize, SHIP_LENGTHS[i], direction)) {
					if (isThePositionFree(direction, gridSize, firstCoordinate, secondCoordinate, SHIP_LENGTHS[i], playerBoard)) {
						shipModification(direction, gridSize, firstCoordinate, secondCoordinate, SHIP_LENGTHS[i], playerBoard, ship, count);
						break;
					}
				}

			}
			count++;
		}
	}
}
#pragma endregion

#pragma region DynamicMemory
int** allocateBoard(int gridSize) {
	int** grid = new int* [gridSize];
	for (int i = 0; i < gridSize; i++) {
		grid[i] = new int[gridSize]();
	}

	return grid;
}


void deallocateBoard(int** shotGrid, int gridSize) {
	for (int i = 0; i < gridSize; i++) {
		delete[] shotGrid[i];
	}
	delete[] shotGrid;
}

#pragma endregion

#pragma region GameConfiguration

void setBattlefield(int& difficultyLevel) {
	cout << "Choose your battlefield:" << endl;
	cout << "[1] Calm Waters     (10x10 Grid) - Standard" << endl;
	cout << "[2] Rough Seas      (12x12 Grid) - Intermediate" << endl;
	cout << "[3] Storm of Steel  (15x15 Grid) - Expert" << endl;
	cout << "Select difficulty (1-3): ";
	while (true) {
		cin >> difficultyLevel;
		if (difficultyLevel == 1 || difficultyLevel == 2 || difficultyLevel == 3) {
			break;
		}
		else {
			cout << "Invalid input!" << endl;
			cout << "Please enter a correct number (1, 2 or 3)" << endl;
			cout << "Your choice ";
		}
	}
}

void setShipPositioning(int& choiceShipsPositioning) {
	cout << "How would you like to position your 10 ships?" << endl;
	cout << "[1] Automatic (Randomly generated)" << endl;
	cout << "[2] Manual    (Enter coordinates manually)" << endl;

	cout << "Your choice: ";
	while (true) {
		cin >> choiceShipsPositioning;
		if (choiceShipsPositioning == 1 || choiceShipsPositioning == 2) {
			break;
		}
		cout << "Invalid input data!" << endl;
		cout << "Please enter 1 (automatic) or 2 (manual)" << endl;
		cout << "Your choice: ";
	}
}

void selectGameMode(int& choice) {
	cout << "Please select an option to proceed:" << endl;
	cout << "[1] New Game" << endl;
	cout << "[2] Load Saved Game" << endl;
	while (true) {
		cin >> choice;
		if (choice == 1 || choice == 2) {
			break;
		}
		else {
			cout << "Invalid input!" << endl;
			cout << "Please enter 1 (for a new game) or 2 (to load saved game)" << endl;
		}
	}
}
#pragma endregion


enum class Color
{
	Aqua = 3,
	White = 7
};

void setColor(Color color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)color);
}

void printSymbolInBoard(int element) {
	if (element == water) {
		setColor(Color::Aqua);
		cout << "\xE3\x80\xB0";
	}
	else if (element == sunk) {
		cout << "\xF0\x9F\x8E\xAF";
	}
	else if (element == miss) {
		setColor(Color::Aqua);
		cout << " \xE2\x97\xAF";
	}
	else if (element > 0 && element <= ship + SHIPS_COUNT) {
		cout << "\xF0\x9F\x9A\xA2";
	}
	else if (element < 0 && element >= -ship - SHIPS_COUNT) {
		cout << "\xF0\x9F\x94\xA5";
	}
	setColor(Color::White);

}

void printBoard(int** computerBoard, int** playerBoard, int size) {
	for (size_t i = 0; i <= size; i++) {
		if (i == 0) {
			cout << i << "   ";
		}
		else if (i < 10) {

			cout << i << "  ";
		}
		else {
			cout << i << " ";
		}
	}
	cout << endl;
	for (size_t i = 0; i < size; i++) {
		cout << i + 1 << " ";
		for (size_t j = 0; j < size; j++)
		{
			if (j == 0 && i < 9) {
				cout << "  ";
			}
			else {
				cout << " ";
			}
			if (computerBoard[i][j] >= ship) {
				printSymbolInBoard(water);
			}
			else {
				printSymbolInBoard(computerBoard[i][j]);
			}
		}
		cout << " | ";
		for (size_t j = 0; j < size; j++){
			cout << " ";
			printSymbolInBoard(playerBoard[i][j]);
		}
		cout << endl;
	}
}


bool isGameFinished(int** board, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] >= ship) return false;
		}
	}
	return true;
}

bool isSunk(int firstCoordinate, int secondCoordinate, int size, int** board) {
	int currentID = board[firstCoordinate][secondCoordinate];
	board[firstCoordinate][secondCoordinate] = -currentID;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] == currentID) {
				return false;
			}
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] == -currentID) {
				board[i][j] = sunk;
			}
		}
	}
	return true;
}

bool playerMove(int size, int** computerBoard) {
	cout << "Enter coordinates (ex: 3 4)" << endl;
	cout << "Your choice: ";
	int firstCoordinate = 0, secondCoordinate = 0;
	coordinatesInput(firstCoordinate, secondCoordinate, size);
	if (computerBoard[firstCoordinate][secondCoordinate] >= ship &&
		computerBoard[firstCoordinate][secondCoordinate] <= ship + SHIPS_COUNT) {
		if(isSunk(firstCoordinate, secondCoordinate, size, computerBoard)) {
			cout << "You have successfully sunk the ship!" << endl;
		}
		else {
			cout << "Congrats! You hit!" << endl;
		}
		return true;
	}
	else if (computerBoard[firstCoordinate][secondCoordinate] == water) {
		computerBoard[firstCoordinate][secondCoordinate] = miss;
		cout << "Unfortunately you miss :((" << endl;
		return false;
	}
	else {
		cout << "Invalid coordinates! Please enter new ones." << endl;
		return true;
	}
	return false;
}

void addNeighbors(int r, int c, int** board, int size, int* neighbors, int& currentElement) {
	int rowDir[] = { -1, 1, 0, 0 };
	int colDir[] = { 0, 0, -1, 1 };

	for (int i = 0; i < 4; i++) {
		int nextR = r + rowDir[i];
		int nextC = c + colDir[i];

		if (nextR >= 0 && nextR < size && nextC >= 0 && nextC < size) {

			if (board[nextR][nextC] == water || board[nextR][nextC] >= ship) {

				bool alreadyInStack = false;
				for (int k = 0; k < currentElement; k++) {
					if (neighbors[k] == nextR * size + nextC) {
						alreadyInStack = true;
						break;
					}
				}

				if (!alreadyInStack) {
					neighbors[currentElement] = nextR * size + nextC;
					currentElement++;
				}
			}
		}
	}
}

int* generateMoves(int gridSize) {
	int total = gridSize * gridSize;
	int* moveSequence = new int[total];

	for (int i = 0; i < total; i++) {
		moveSequence[i] = i;
	}

	for (int i = total - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		int temp = moveSequence[i];
		moveSequence[i] = moveSequence[j];
		moveSequence[j] = temp;
	}
	return moveSequence;
}

bool computerMove(int size, int** playerBoard, int& currentMoveIndex, int* totalMoves, int* neighbors, int& currentElement) {
	int firstCoordinate, secondCoordinate;
	bool validTargetFound = false;

	while (currentElement > 0) {
		currentElement--;
		int targetIndex = neighbors[currentElement];
		firstCoordinate = targetIndex / size;
		secondCoordinate = targetIndex % size;

		if (playerBoard[firstCoordinate][secondCoordinate] == water || playerBoard[firstCoordinate][secondCoordinate] >= ship) {
			validTargetFound = true;
			break;
		}
	}

	if (!validTargetFound) {
		if (currentMoveIndex >= size * size) {
			cout << "ERROR: Computer has no valid moves left!" << endl;
			return false;
		}

		firstCoordinate = totalMoves[currentMoveIndex] / size;
		secondCoordinate = totalMoves[currentMoveIndex] % size;
		currentMoveIndex++;

		while ((playerBoard[firstCoordinate][secondCoordinate] == miss ||
			playerBoard[firstCoordinate][secondCoordinate] == sunk ||
			playerBoard[firstCoordinate][secondCoordinate] < 0) &&
			currentMoveIndex < size * size) {

			firstCoordinate = totalMoves[currentMoveIndex] / size;
			secondCoordinate = totalMoves[currentMoveIndex] % size;
			currentMoveIndex++;
		}
	}
	cout << "Computer shoots at: " << firstCoordinate + 1 << " " << secondCoordinate + 1 << endl;

	if (playerBoard[firstCoordinate][secondCoordinate] >= ship) {
		if (isSunk(firstCoordinate, secondCoordinate, size, playerBoard)) {
			cout << "Computer sunk your ship!" << endl;
			return true;
		}
		else {
			cout << "Computer hit your ship!" << endl;
			addNeighbors(firstCoordinate, secondCoordinate, playerBoard, size, neighbors, currentElement);
			return true;
		}
	}
	else if (playerBoard[firstCoordinate][secondCoordinate] == water) {
		playerBoard[firstCoordinate][secondCoordinate] = miss;
		cout << "Computer missed!" << endl;
	}
	else {
		cout << "Computer targeted an old spot."<<endl;
	}
	return false;
}

void GameLogic(int** playerBoard, int** computerBoard, int size, int* totalMoves, int* neighbors, int currentMove = 0, int currentElement = 0, int step = 1) {
	while (!(isGameFinished(playerBoard, size) || isGameFinished(computerBoard, size))) {
		if (step % 2) {
			if (playerMove(size, computerBoard)) {
				step--;
			}
			printBoard(computerBoard, playerBoard, size);
		}
		else {
			Sleep(1000);
			if (computerMove(size, playerBoard, currentMove, totalMoves, neighbors, currentElement)) {
				step--;
			}
			printBoard(computerBoard, playerBoard, size);
		}
		step++;
	}
	if (isGameFinished(playerBoard, size)) {
		cout << "Computer won.";
	}
	else {
		cout << "Congrat! You won!";
	}
}


void initializeNewGame() {

	int difficultyLevel = 0;
	setBattlefield(difficultyLevel);

	int gridSize = GRID_SIZES[difficultyLevel - 1];
	int** playerBoard = allocateBoard(gridSize);
	int** computerBoard = allocateBoard(gridSize);

	int choiceShipsPositioning = 0;
	setShipPositioning(choiceShipsPositioning);

	(choiceShipsPositioning == 1) ? automaticShipPositioning(playerBoard, gridSize) :
		manualShipPositioning(playerBoard, gridSize);
	automaticShipPositioning(computerBoard, gridSize);
	printBoard(computerBoard, playerBoard, gridSize);


	int* totalMoves = generateMoves(gridSize);
	int* neighbors = new int[gridSize * gridSize];
	GameLogic(playerBoard, computerBoard, gridSize, totalMoves, neighbors);

	deallocateBoard(playerBoard, gridSize);
	deallocateBoard(computerBoard, gridSize);

	delete[] totalMoves;
	delete[] neighbors;
}

int main()
{
	srand(static_cast<unsigned int>(time(0)));
	SetConsoleOutputCP(CP_UTF8);
	int gameChoice = 0;
	selectGameMode(gameChoice);

	if (gameChoice == 1) {

		initializeNewGame();
	}
	else {
		//GameLogic();
	}
}