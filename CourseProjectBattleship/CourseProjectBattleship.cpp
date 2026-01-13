#include <iostream>
using std::cout;
using std::cin;
using std::endl;

const int SHIPS_TYPES = 4;
const int SHIP_LENGTHS[SHIPS_TYPES] = { 4,3,2,1 };
const int SHIP_COUNTS[SHIPS_TYPES] = { 1,2,3,4 };
const int GRID_TYPES = 3;
const int GRID_SIZES[GRID_TYPES] = {10,12,15};


#pragma region ShipsPositioning

const char* validationShipPositioning(int** playerBoard, int gridSize, int firstCoordinate, int secondCoordinate, char direction, int shipSize) {
	const char* message = nullptr;
	if (playerBoard == nullptr) {
		message = "Something wnet wrong!";
	}
	if (firstCoordinate > gridSize || secondCoordinate > gridSize) {
		message = "The coordinate it's out of bounds.";
	}
	else if (playerBoard[firstCoordinate - 1][secondCoordinate - 1] == 1) {
		message = "You already have a ship on this place";
	}
	else if (direction != 'H' && direction != 'h' && direction != 'V' && direction != 'v') {
		message = "Invalid direction!";
	}

	else {
		
		if (direction == 'V' || direction == 'v') {
			if (firstCoordinate + shipSize > gridSize - 1) {
				message = "Error: Ship exceeds board boundaries! Choose a different start or direction.";
			}
			else {
				for (int i = firstCoordinate; i < shipSize + firstCoordinate; i++) {
					playerBoard[firstCoordinate - 1][secondCoordinate - 1] = 1;
				}
			}
		}

		else if (direction == 'H' || direction == 'h') {
			if (secondCoordinate + shipSize > gridSize - 1) {
				message = "Error: Ship exceeds board boundaries! Choose a different start or direction.";
			}
			else {
				for (int i = secondCoordinate; i < shipSize + secondCoordinate; i++) {
					playerBoard[firstCoordinate - 1][secondCoordinate - 1] = 1;
				}
			}
		}
	}

	return message;
}

void manualShipPositioning(int** playerBoard, int gridSize) {
	for (int i = 0; i < SHIPS_TYPES; i++) {
		for (int j = 0; j < SHIP_COUNTS[i]; j++) {
			cout << "How do you want to position your " << j + 1 << "th ship with length " << SHIP_LENGTHS[i] << endl;
			cout << "Please enter coordinates for the start of your ship (ex: 3 4)"<<endl;
			cout << "Your choice: ";
			int firstCoordinate = 0;
			int secondCoordinate = 0;
			char direction;
			while (true) {
				cin >> firstCoordinate;
				cin >> secondCoordinate;
				cout << "Please enter the direction of your ship: H for horizontal or V for vertical" << endl;
				cin >> direction;		
				const char* result = validationShipPositioning(playerBoard, gridSize, firstCoordinate, secondCoordinate, direction, SHIP_LENGTHS[i]);
				if (result == nullptr) {
					cout << "You have successfully placed your ship!";
					break;
				}
				else {
					cout << result;
					cout << "Please enter again the coordinates and the direction" << endl;
				}
			}
		}
	}
}

void automaticShipPositioning(int** playerBoard, int gridSize) {

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

		cout << "How would you like to position your 10 ships?"<<endl;
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

		int i = 0;
		deallocateBoard(playerBoard, gridSize);
		deallocateBoard(computerBoard, gridSize);
	}
}