#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <fstream>

using std::cout;
using std::cin;
using std::endl;

const int SHIPS_TYPES = 4;
const int SHIPS_COUNT = 10;
const int SHIP_LENGTHS[SHIPS_TYPES] = { 4,3,2,1 };
const int SHIP_COUNTS[SHIPS_TYPES] = { 1,2,3,4 };
const int GRID_TYPES = 3;
const int GRID_SIZES[GRID_TYPES] = { 10,12,15 };
const char* SAVE_FILE = "battleship_game.txt";

enum boardElements {
	water,
	hit,
	miss,
	sunk,
	ship
};

#pragma region HelperFunction

bool isDigit(char symbol) {
	return (symbol >= '0' && symbol <= '9');
}

int readIntFromLine() {
	char buffer[100];

	if (cin.peek() == '\n') {
		cin.ignore();
	}

	cin.getline(buffer, 100);

	if (buffer[0] == '\0') return 0;

	if ((buffer[0] == 's' || buffer[0] == 'S') && buffer[1] == '\0') {
		return -1;
	}

	int num = 0;
	for (int i = 0; buffer[i] != '\0'; i++) {
		if (i == 0 && buffer[i] == ' ') continue;

		if (isDigit(buffer[i])) {
			num = num * 10 + (buffer[i] - '0');
		}
		else if (buffer[i] == ' ' || buffer[i] == '\r') {
			break;
		}
		else {
			return 0;
		}
	}

	return (num == 0 && buffer[0] != '0') ? 0 : num;
}

enum class Color
{
	Aqua = 3,
	White = 7,
	Red = 12,
};

void setColor(Color color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)color);
}

void printHeader(const char* title) {
	setColor(Color::Aqua);
	cout << "=================================================" << endl;
	cout << "| ";
	setColor(Color::White);
	cout << title;
	int len = 0; while (title[len]) len++;
	for (int i = 0; i < 45 - len; i++) cout << " ";
	setColor(Color::Aqua);
	cout << " |" << endl;
	cout << "=================================================" << endl;
	setColor(Color::White);
}

void printVictory() {
	system("cls");
	setColor(Color::Aqua);
	cout << "********************************************" << endl;
	cout << "*                                          *" << endl;
	cout << "*            CONGRATULATIONS!              *" << endl;
	cout << "*             YOU HAVE WON!                *" << endl;
	cout << "*                                          *" << endl;
	cout << "********************************************" << endl;
	setColor(Color::White);
}

void printGameOver() {
	system("cls");
	setColor(Color::Red);
	cout << "############################################" << endl;
	cout << "#                                          #" << endl;
	cout << "#                 GAME OVER                #" << endl;
	cout << "#         THE COMPUTER DEFEATED YOU        #" << endl;
	cout << "#                                          #" << endl;
	cout << "############################################" << endl;
	setColor(Color::White);
}
#pragma endregion

#pragma region ShipsPositioning

bool coordinateValidationAfterDirection(int firstCoordinate, int secondCoordinate,
	int gridSize, int shipLength, char direction = '-') {

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

bool isThePositionFree(char direction, int gridSize, int firstCoordinate,
	int secondCoordinate, int shipLength, int** board) {

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

void shipModification(char direction, int gridSize, int firstCoordinate,
	int secondCoordinate, int shipLength, int** playerBoard, int command, int shipId) {

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
	int middle = size * 3;
	for (int k = 0; k < middle / 2; k++) cout << " ";
	cout << "COMPUTER";
	for (int k = 0; k < middle - 4; k++) cout << " ";
	cout << "PLAYER" << endl;

	cout << "    ";
	for (int i = 1; i <= size; i++) {
		cout << i << (i < 10 ? "  " : " ");
	}

	cout << "|  ";

	for (int i = 1; i <= size; i++) {
		cout << i << (i < 10 ? "  " : " ");
	}
	cout << endl;
}


void printCurrentShipsPositioning(int size, int** const playerBoard) {
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


void printBoard(int** const computerBoard, int** const playerBoard, int size) {
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

bool readShipStartCoordinates(int& row, int& col, int gridSize, int** board) {
	cout << "Your choice: ";
	int result = coordinatesInputValidation(row, col, gridSize, board);
	if (result == 1 && board[row][col] == water) {
		return true;
	}
	cout << "Invalid input! Please enter the coordinates again." << endl;
	return false;
}

bool tryPlaceShip(int row, int col, char direction, int shipLength,
	int gridSize, int** board, int shipId) {

	if (!coordinateValidationAfterDirection(row, col, gridSize, shipLength, direction)) {
		cout << "Ship does not fit in this direction!" << endl;
		cout << "Please enter the coordinates again!" << endl;
		return false;
	}

	if (!isThePositionFree(direction, gridSize, row, col, shipLength, board)) {
		cout << "There is already a ship in this position!" << endl;
		cout << "Please enter the coordinates again!" << endl;
		return false;
	}

	shipModification(direction, gridSize, row, col, shipLength, board, ship, shipId);
	return true;
}

void placeSingleShip(int gridSize, int** const playerBoard, int type, int shipId) {
	printCurrentShipsPositioning(gridSize, playerBoard);

	cout << "Placing ship with length " << SHIP_LENGTHS[type] << endl;
	cout << "Enter starting coordinates (row col):" << endl;

	int row = 0, col = 0;
	char direction = '-';

	while (true) {
		if (!readShipStartCoordinates(row, col, gridSize, playerBoard)) {
			continue;
		}

		if (SHIP_LENGTHS[type] == 1) {
			if (playerBoard[row][col] == water) {
				playerBoard[row][col] = ship + shipId;
				cout << "Ship placed successfully!" << endl;
				break;
			}
			else {
				cout << "This place is already taken!" << endl;
				cout << "Please enter the coordinates again!" << endl;
			}
		}

		else {
			directionInput(direction);

			if (tryPlaceShip(row, col, direction, SHIP_LENGTHS[type],
				gridSize, playerBoard, shipId)) {
				cout << "Ship placed successfully!" << endl;
				break;
			}
		}
	}
}


void manualShipPositioning(int** playerBoard, int gridSize) {
	int shipId = 0;
	for (int type = 0; type < SHIPS_TYPES; type++) {
		for (int count = 0; count < SHIP_COUNTS[type]; count++) {
			placeSingleShip(gridSize, playerBoard, type, shipId);
		}
		shipId++;
		Sleep(1500);
		system("cls");
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

				if (coordinateValidationAfterDirection(firstCoordinate, secondCoordinate,
					gridSize, SHIP_LENGTHS[i], direction)) {

					if (isThePositionFree(direction, gridSize, firstCoordinate,
						secondCoordinate, SHIP_LENGTHS[i], playerBoard)) {

						shipModification(direction, gridSize, firstCoordinate,
							secondCoordinate, SHIP_LENGTHS[i], playerBoard, ship, count);
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
	printHeader("      CHOOSE YOUR BATTLEFIELD");
	cout << " [1] Calm Waters     (10x10 Grid)" << endl;
	cout << " [2] Rough Seas      (12x12 Grid)" << endl;
	cout << " [3] Storm of Steel  (15x15 Grid)" << endl;
	cout << "--------------------------------------------" << endl;
	cout << "Select difficulty (1-3): ";
	while (true) {
		difficultyLevel = readIntFromLine();
		if (difficultyLevel == 1 || difficultyLevel == 2 || difficultyLevel == 3) {
			break;
		}
		cout << "Invalid input!" << endl;
		cout << "Please enter a correct number (1, 2 or 3)" << endl;
		cout << "Your choice ";
	}
}

void setShipPositioning(int& choiceShipsPositioning) {
	printHeader("How would you like to position your 10 ships?");
	cout << "[1] Automatic (Randomly generated)" << endl;
	cout << "[2] Manual    (Enter coordinates manually)" << endl;

	cout << "Your choice: ";
	while (true) {
		choiceShipsPositioning = readIntFromLine();
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
	cout << "Your choice: ";

	while (true) {
		int result = readIntFromLine();

		if (result == 1 || result == 2) {
			choice = result;
			break;
		}

		cout << "Invalid input! Please enter 1 or 2: ";
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


void saveGame(int** playerBoard, int** computerBoard, int size, int step,
	int currentMoveIndex, int* totalMoves, int* neighbors, int currentElement) {
	std::ofstream out(SAVE_FILE);

	if (!out.is_open()) {
		cout << "Error saving game!" << endl;
		return;
	}

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

#pragma region PlayerMove

int playerMove(int size, int** computerBoard) {
	cout << "Enter coordinates (ex: 3 4) or S for saving the game" << endl;
	cout << "Your choice: ";
	int firstCoordinate = 0, secondCoordinate = 0;

	int result = coordinatesInputValidation(firstCoordinate, secondCoordinate, size, computerBoard);
	if (result == -1) {
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

	else if (computerBoard[firstCoordinate][secondCoordinate] == miss || computerBoard[firstCoordinate][secondCoordinate] == sunk ||
		computerBoard[firstCoordinate][secondCoordinate] <= hit && computerBoard[firstCoordinate][secondCoordinate] != water) {

		cout << "Invalid coordinates! You have already entered them! Please enter new ones." << endl;
		return 1;
	}
	return 0;
}

#pragma endregion

#pragma region ComputerMove

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

bool isValidComputerTarget(int row, int col, int** board) {
	return board[row][col] == water || board[row][col] >= ship;
}

bool getNextComputerTarget(int size, int** board, int& row, int& col,
	int& currentMoveIndex, int* totalMoves, int* neighbors, int& currentElement) {

	while (currentElement > 0) {
		currentElement--;
		int index = neighbors[currentElement];
		row = index / size;
		col = index % size;

		if (isValidComputerTarget(row, col, board)) {
			return true;
		}
	}

	while (currentMoveIndex < size * size) {
		int index = totalMoves[currentMoveIndex++];
		row = index / size;
		col = index % size;

		if (isValidComputerTarget(row, col, board)) {
			return true;
		}
	}

	return false;
}

bool computerMove(int size, int** playerBoard, int& currentMoveIndex, int* totalMoves, int* neighbors, int& currentElement) {
	int row = 0, col = 0;

	if (!getNextComputerTarget(size, playerBoard, row, col, currentMoveIndex, totalMoves, neighbors, currentElement)) {
		cout << "ERROR: Computer has no valid moves left!" << endl;
		return false;
	}

	cout << "Computer shoots at: " << row + 1 << " " << col + 1 << endl;

	if (playerBoard[row][col] >= ship) {
		if (isSunk(row, col, size, playerBoard)) {
			cout << "Computer sunk your ship!" << endl;
		}
		else {
			cout << "Computer hit your ship!" << endl;
			addNeighbors(row, col, playerBoard, size, neighbors, currentElement);
		}
		return true;
	}

	if (playerBoard[row][col] == water) {
		playerBoard[row][col] = miss;
		cout << "Computer missed!" << endl;
	}

	return false;
}

#pragma endregion

void GameLogic(int** playerBoard, int** computerBoard, int size, int* totalMoves, int* neighbors,
	int currentMove = 0, int currentElement = 0, int step = 1) {

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
	}
	if (isGameFinished(playerBoard, size)) {
		printGameOver();
		remove(SAVE_FILE);
	}
	else {
		printVictory();
		remove(SAVE_FILE);
	}

}

#pragma endregion

bool loadGame() {
	std::ifstream in(SAVE_FILE);

	if (!in.is_open()) {
		cout << "No saved game found!" << endl;
		return false;
	}

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

	GameLogic(playerBoard, computerBoard, size, totalMoves, neighbors,
		currentMoveIndex, currentElement, step);

	deallocateBoard(playerBoard, size);
	deallocateBoard(computerBoard, size);
	delete[] totalMoves;
	delete[] neighbors;
	return true;
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
	printHeader("               Battleship");
	setColor(Color::Aqua);
	cout << "     ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << endl;

	setColor(Color::White);
	int gameChoice = 0;
	selectGameMode(gameChoice);

	if (gameChoice == 1) {

		initializeNewGame();
	}
	else {
		if (!loadGame()) {
			initializeNewGame();
		}
	}
}