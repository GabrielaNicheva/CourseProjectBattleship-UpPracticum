#include <iostream>
#include <cstdlib>
#include <ctime>
#include<windows.h>
#include <fstream>

using std::cout;
using std::cin;
using std::endl;

const int SHIPS_TYPES = 4;
const int SHIPS_COUNT = 10;
const int SHIP_LENGTHS[SHIPS_TYPES] = { 4,3,2,1 };
const int SHIP_COUNTS[SHIPS_TYPES] = { 1,2,3,4 };
const int GRID_TYPES = 3;
const int GRID_SIZES[GRID_TYPES] = { 6,12,15 };
const char* SAVE_FILE = "battleship_game.txt";


enum boardElements {
	water,
	hit,
	miss,
	sunk,
	ship
};

bool isDigit(char symbol) {
	return (symbol >= '0' && symbol <= '9');
}

#pragma region ShipsPositioning

bool coordinateValidationAfterDirection(int firstCoordinate, int secondCoordinate, int gridSize, int shipLength, char direction = '-') {

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

int isCoordinateValidNumber() {
	char buffer[10];
	cin >> buffer;

	if (buffer[0] == 's' || buffer[0] == 'S') {
		return -1;
	}

	int num = 0;
	for (int i = 0; buffer[i] != '\0'; i++) {
		if (buffer[i] >= '0' && buffer[i] <= '9') {
			num = num * 10 + (buffer[i] - '0');
		}
		else {
			return 0;
		}
	}
	cin.clear();
	cin.ignore();
	return num;

}

int coordinatesInputValidation(int& firstCoordinate, int& secondCoordinate,
	const int gridSize, int** board) {

	firstCoordinate = isCoordinateValidNumber();
	if (firstCoordinate == -1) return -1;
	if (firstCoordinate <= 0) return 0;

	secondCoordinate = isCoordinateValidNumber();
	if (secondCoordinate <= 0) return 0;

	firstCoordinate--;
	secondCoordinate--;

	if (firstCoordinate < 0 || firstCoordinate >= gridSize ||
		secondCoordinate < 0 || secondCoordinate >= gridSize) {
		return 0;
	}

	if (board[firstCoordinate][secondCoordinate] == miss ||
		board[firstCoordinate][secondCoordinate] <= hit  && board[firstCoordinate][secondCoordinate] != water ||
		board[firstCoordinate][secondCoordinate] == sunk) {
		return 0;
	}

	return 1;
}



void directionInput(char& direction) {
	cout << "Please enter the direction of your ship: H for horizontal or V for vertical" << endl;
	while (true)
	{
		cin >> direction;
		if (direction == 'H' || direction == 'h' || direction == 'V' || direction == 'v') {
			break;
		}
		cout << "Invalid direction!" << endl;
		cout << "Please enter H (horizontal) or V (vertical)" << endl;
	}
}

#pragma region PrintBoard

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



void printFirstRow(int size) {
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
}


void printCurrentShipsPositioning(int size, int** playerBoard) {
	printFirstRow(size);
	for (size_t i = 0; i < size; i++) {
		cout << i + 1 << " ";

		for (size_t j = 0; j < size; j++) {
			if (j == 0 && i < 9) {
				cout << "  ";
			}
			else {
				cout << " ";
			}
			printSymbolInBoard(playerBoard[i][j]);
		}
		cout << endl;
	}
}


void printBoard(int** computerBoard, int** playerBoard, int size) {
	printFirstRow(size);
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
		for (size_t j = 0; j < size; j++) {
			cout << " ";
			printSymbolInBoard(playerBoard[i][j]);
		}
		cout << endl;
	}
}
#pragma endregion



void manualShipPositioning(int** playerBoard, int gridSize) {
	int count = 0;
	for (int i = 0; i < SHIPS_TYPES; i++) {
		for (int j = 0; j < SHIP_COUNTS[i]; j++) {
			printCurrentShipsPositioning(gridSize, playerBoard);
			cout << "How do you want to position your " << j + 1 << "th ship with length " << SHIP_LENGTHS[i] << endl;
			cout << "Please enter coordinates for the start of your ship (ex: 3 4)" << endl;
			cout << "Your choice: ";
			int firstCoordinate = 0;
			int secondCoordinate = 0;
			char direction = '-';
			while (true) {
				if (coordinatesInputValidation(firstCoordinate, secondCoordinate, gridSize, playerBoard) == 1) {
					if (SHIP_LENGTHS[i] != 1) {
						directionInput(direction);
						if (coordinateValidationAfterDirection(firstCoordinate, secondCoordinate, gridSize, SHIP_LENGTHS[i], direction)) {
							if (isThePositionFree(direction, gridSize, firstCoordinate, secondCoordinate, SHIP_LENGTHS[i], playerBoard)) {
								shipModification(direction, gridSize, firstCoordinate, secondCoordinate, SHIP_LENGTHS[i], playerBoard, ship, count);
								cout << "You have successfully placed your ship!" << endl;
								break;
							}
							else {
								cout << "There is already a ship on this place!" << endl;
								cout << "Please enter again the coordinates and the direction" << endl;
							}
						}
						else {
							cout << "Incorrect input!" << endl;
							cout << "Please enter the coordinates again" << endl;
						}
					}							
					else {
						playerBoard[firstCoordinate][secondCoordinate] = ship + count;
						cout << "You have successfully placed your ship!" << endl;
						break;
					}
				}
				else {
					cout << "Invalid input! Please enter the coordinates!" << endl;
					cout << "Your choice: ";
				}
				
			}
			Sleep(2000);
			system("cls");
			count++;
		}
	}
	printCurrentShipsPositioning(gridSize, playerBoard);
	Sleep(2000);
	system("cls");

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

				if (coordinateValidationAfterDirection(firstCoordinate, secondCoordinate, gridSize, SHIP_LENGTHS[i], direction)) {
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
		char symbol;
		cin >> symbol;
		if (isDigit(symbol)) {
			difficultyLevel = symbol - '0';
			if (difficultyLevel == 1 || difficultyLevel == 2 || difficultyLevel == 3) {
				break;
			}
		}
		cout << "Invalid input!" << endl;
		cout << "Please enter a correct number (1, 2 or 3)" << endl;
		cout << "Your choice ";
	}
}

void setShipPositioning(int& choiceShipsPositioning) {
	cout << "How would you like to position your 10 ships?" << endl;
	cout << "[1] Automatic (Randomly generated)" << endl;
	cout << "[2] Manual    (Enter coordinates manually)" << endl;

	cout << "Your choice: ";
	while (true) {
		char symbol;
		cin >> symbol;
		if (isDigit(symbol)) {
			choiceShipsPositioning = symbol - '0';
			if (choiceShipsPositioning == 1 || choiceShipsPositioning == 2) {
				break;
			}
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
		char symbol;
		cin >> symbol;
		if (isDigit(symbol)) {
			choice = symbol - '0';
			if (choice == 1 || choice == 2) {
				break;
			}
		}
		cout << "Invalid input!" << endl;
		cout << "Please enter 1 (for a new game) or 2 (to load saved game)" << endl;
	}
}
#pragma endregion



#pragma region SaveLoadGame

void loadBoard(std::ifstream& in, int** board, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			in >> board[i][j];
		}
	}
}
void saveBoard(std::ofstream& out, int** board, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			out << board[i][j] << " ";
		}
		out << endl;
	}
}


void saveGame(int** playerBoard, int** computerBoard, int size, int step, int currentMoveIndex, int* totalMoves, int* neighbors, int currentElement) {
	std::ofstream out(SAVE_FILE);

	out << size << endl;
	out << step << endl;
	out << currentMoveIndex << endl;
	out << currentElement << endl;

	saveBoard(out, playerBoard, size);
	saveBoard(out, computerBoard, size);

	for (int i = 0; i < size * size; i++) {
		out << totalMoves[i] << " ";
	}
	out << endl;

	for (int i = 0; i < currentElement; i++) {
		out << neighbors[i] << " ";
	}

	out.close();
}



#pragma endregion


#pragma region MainLogic

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

int playerMove(int size, int** computerBoard) {
	cout << "Enter coordinates (ex: 3 4) or S for saving the game" << endl;
	cout << "Your choice: ";
	int firstCoordinate = 0, secondCoordinate = 0;

	int result = coordinatesInputValidation(firstCoordinate, secondCoordinate, size, computerBoard);
	if ( result == -1) {
		return -1;
	}
	else if (result == 0) {
		cout << "Invalid coordinates! Please enter new ones." << endl;
		return 1;
	}
	else if (computerBoard[firstCoordinate][secondCoordinate] >= ship &&
		computerBoard[firstCoordinate][secondCoordinate] <= ship + SHIPS_COUNT) {
		if (isSunk(firstCoordinate, secondCoordinate, size, computerBoard)) {
			cout << "You have successfully sunk the ship!" << endl;
		}
		else {
			cout << "Congrats! You hit!" << endl;
		}
		return 1;
	}
	else if (computerBoard[firstCoordinate][secondCoordinate] == water) {
		computerBoard[firstCoordinate][secondCoordinate] = miss;
		cout << "Unfortunately you miss :((" << endl;
		return 0;
	}
	return 0;
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
		cout << "Computer targeted an old spot." << endl;
	}
	return false;
}

void GameLogic(int** playerBoard, int** computerBoard, int size, int* totalMoves, int* neighbors, int currentMove = 0, int currentElement = 0, int step = 1) {
	while (!(isGameFinished(playerBoard, size) || isGameFinished(computerBoard, size))) {
		if (step % 2) {
			printBoard(computerBoard, playerBoard, size);

			int result = playerMove(size, computerBoard);
			if (result == 1) {
				step--;
			}
			else if (result == -1) {
				saveGame(playerBoard, computerBoard, size, step, currentMove, totalMoves, neighbors, currentElement);
				cout << "Game saved!" << endl;
				return;
			}

			//printBoard(computerBoard, playerBoard, size);
			Sleep(2000);
			system("cls");

		}
		else {

			if (computerMove(size, playerBoard, currentMove, totalMoves, neighbors, currentElement)) {
				step--;
			}
			Sleep(1500);
			printBoard(computerBoard, playerBoard, size);
			Sleep(4000);
			system("cls");
		}
		step++;
		//Sleep(2000);

	}
	if (isGameFinished(playerBoard, size)) {
		cout << "Computer won.";
	}
	else {
		cout << "Congrats! You won!";
	}
}

#pragma endregion

void loadGame() {
	std::ifstream in(SAVE_FILE);

	int size, step, currentMoveIndex, currentElement;
	in >> size >> step >> currentMoveIndex >> currentElement;

	int** playerBoard = allocateBoard(size);
	int** computerBoard = allocateBoard(size);

	loadBoard(in, playerBoard, size);
	loadBoard(in, computerBoard, size);

	int* totalMoves = new int[size * size];
	for (int i = 0; i < size * size; i++) {
		in >> totalMoves[i];
	}

	int* neighbors = new int[size * size];
	for (int i = 0; i < currentElement; i++) {
		in >> neighbors[i];
	}

	in.close();

	GameLogic(playerBoard, computerBoard, size, totalMoves, neighbors, currentMoveIndex, currentElement, step);
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
	//printBoard(computerBoard, playerBoard, gridSize);


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
		loadGame();
	}
	
}