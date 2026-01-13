#include <iostream>
#include <cstdlib>
#include <ctime>
using std::cout;
using std::cin;
using std::endl;

const int SHIPS_TYPES = 4;
const int SHIP_LENGTHS[SHIPS_TYPES] = { 4,3,2,1 };
const int SHIP_COUNTS[SHIPS_TYPES] = { 1,2,3,4 };
const int GRID_TYPES = 3;
const int GRID_SIZES[GRID_TYPES] = { 10,12,15 };


#pragma region ShipsPositioning

bool coordinateValidation(int coordinate, int gridSize) {
	if (coordinate < 0 || coordinate + 1 > gridSize) {
		return false;
	}

	return true;
}

bool coordinationValidationAfterDirection(int firstCoordinate, int secondCoordinate, int gridSize, int shipLength, char direction) {
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
			if (board[i][secondCoordinate] == 1) {
				return false;
			}
		}
	}

	else if (direction == 'H' || direction == 'h') {
		for (int i = secondCoordinate; i < shipLength + secondCoordinate; i++) {
			if (board[firstCoordinate][i] == 1) {
				return false;
			}
		}
	}
	return true;
}

void setShipOnPosition(char direction, int gridSize, int firstCoordinate, int secondCoordinate, int shipLength, int** playerBoard) {
	if (direction == 'V' || direction == 'v') {
		for (int i = firstCoordinate; i < shipLength + firstCoordinate; i++) {
			playerBoard[i][secondCoordinate] = 1;
		}
	}

	else if (direction == 'H' || direction == 'h') {
		for (int i = secondCoordinate; i < shipLength + secondCoordinate; i++) {
			playerBoard[firstCoordinate][i] = 1;
		}
	}

}

void coordinatesInput(int& firstCoordinate, int& secondCoordinate, const int gridSize) {
	while (true) {
		cin >> firstCoordinate;
		firstCoordinate--;
		cin >> secondCoordinate;
		secondCoordinate--;

		if ((firstCoordinate < 0 || firstCoordinate + 1 > gridSize) && (secondCoordinate < 0 || secondCoordinate + 1 > gridSize)) {
			break;
		}
		cout << "Your ship it's out of bounds." << endl;
		cout << "Please enter the coordinates again" << endl;
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

				if (coordinationValidationAfterDirection(firstCoordinate, secondCoordinate, gridSize, SHIP_LENGTHS[i], direction)) {
					if (isThePositionFree(direction, gridSize, firstCoordinate, secondCoordinate, SHIP_LENGTHS[i], playerBoard)) {
						setShipOnPosition(direction, gridSize, firstCoordinate, secondCoordinate, SHIP_LENGTHS[i], playerBoard);
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
		}
	}
}

void automaticShipPositioning(int** playerBoard, int gridSize) {
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

				if (coordinationValidationAfterDirection(firstCoordinate, secondCoordinate, gridSize, SHIP_LENGTHS[i], direction)) {
					if (isThePositionFree(direction, gridSize, firstCoordinate, secondCoordinate, SHIP_LENGTHS[i], playerBoard)) {
						setShipOnPosition(direction, gridSize, firstCoordinate, secondCoordinate, SHIP_LENGTHS[i], playerBoard);
						break;
					}
				}

			}
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



int main()
{
	srand(static_cast<unsigned int>(time(0)));
	cout << "Welcome to Battleship" << endl;
	cout << "Please select an option to proceed:" << endl;
	cout << "[1] New Game" << endl;
	cout << "[2] Load Saved Game" << endl;
	int choice = 0;
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

	if (choice == 1) {
		cout << "Choose your battlefield:" << endl;
		cout << "[1] Calm Waters     (10x10 Grid) - Standard" << endl;
		cout << "[2] Rough Seas      (12x12 Grid) - Intermediate" << endl;
		cout << "[3] Storm of Steel  (15x15 Grid) - Expert" << endl;
		cout << "Select difficulty (1-3): ";
		int difficultyLevel = 0;
		while (true) {
			cin >> difficultyLevel;
			if (difficultyLevel == 1 || difficultyLevel == 2 || difficultyLevel == 3) {
				break;
			}
			else {
				cout << "Invalid input!" << endl;
				cout << "Please enter a number between 1,2, and 3" << endl;
				cout << "1 - for a standard level" << endl;
				cout << "2 - for an intermediate level" << endl;
				cout << "3 - for an expert level" << endl;
			}
		}
		int gridSize = GRID_SIZES[difficultyLevel - 1];
		int** playerBoard = allocateBoard(gridSize);
		int** computerBoard = allocateBoard(gridSize);

		cout << "How would you like to position your 10 ships?" << endl;
		cout << "[1] Automatic (Randomly generated)" << endl;
		cout << "[2] Manual    (Enter coordinates manually)" << endl;

		cout << "Your choice: ";

		int choiceShipsPositioning = 0;
		while (true) {
			cin >> choiceShipsPositioning;
			if (choiceShipsPositioning == 1 || choiceShipsPositioning == 2) {
				break;
			}
			cout << "Invalid input data!" << endl;
			cout << "Please enter 1 - for automatic and 2 for manual" << endl;
			cout << "Your choice: ";
		}

		(choiceShipsPositioning == 1) ? automaticShipPositioning(playerBoard, gridSize) : manualShipPositioning(playerBoard, gridSize);

		for (int i = 0; i < gridSize; i++) {
			for (size_t j = 0; j < gridSize; j++)
			{
				cout << playerBoard[i][j] << " ";
			}
			cout << endl;
		}

		deallocateBoard(playerBoard, gridSize);
		deallocateBoard(computerBoard, gridSize);
	}
}