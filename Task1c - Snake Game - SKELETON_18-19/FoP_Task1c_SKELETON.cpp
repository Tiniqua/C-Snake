//---------------------------------------------------------------------------
//Program: Skeleton for Task 1c – group assignment
//Author: Pascale Vacher
//Last updated: 23 February 2018
//---------------------------------------------------------------------------

//Go to 'View > Task List' menu to open the 'Task List' pane listing the initial amendments needed to this program

//---------------------------------------------------------------------------
//----- include libraries
//---------------------------------------------------------------------------

//include standard libraries
#include <iostream>	
#include <iomanip> 
#include <conio.h> 
#include <cassert> 
#include <string>
#include <sstream>
#include <ctime>
#include <fstream>
#include <vector>
#include <stdio.h>
using namespace std;
//include our own libraries
#include "RandomUtils.h"    //for seed, random
#include "ConsoleUtils.h"	//for clrscr, gotoxy, etc.
#include "TimeUtils.h"		//for getSystemTime, timeToString, etc.
//#include "timercpp.h"
//---------------------------------------------------------------------------
//----- define constants
//---------------------------------------------------------------------------
//defining the size of the grid
const int SIZEX(12);    	//horizontal dimension
const int SIZEY(10);		//vertical dimension

							//defining symbols used for display of the grid and content
const char SPOT('0');   	//spot
const char TUNNEL(' ');    	//tunnel
const char WALL('#');    	//border
const char MOUSE('@');      //mouse
const char BODY('o');       //body
const char PILL('+');		//pill
const char MONGOOSE('M');

//defining the command letters to move the spot on the maze
const int  UP(72);			//up arrow
const int  DOWN(80); 		//down arrow
const int  RIGHT(77);		//right arrow
const int  LEFT(75);		//left arrow
							//defining the other command letters
const char QUIT('Q');		//to end the game
const char Cheat('C');
const char Unlock('U');
const char BestScoreShow('B');

struct Item {
	int x, y;
	char symbol;
};
struct GameData {
	string message;
	int score;
	int number;
};
//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------
int main() {
	//function declarations (prototypes)
	string name;
	int score(0);
	int pressed(0);
	int miceCounter(0);
	int currentMice(0);
	int cooldown(0);
	int highscore(500);
	int miceNeeded;

	int level;

	bool unlock1;
	bool unlock2;

	bool AllUnlocked = false;

	bool hasCheated = false;
	bool dead = false;
	bool pillCanSpawn = false;
	bool mongooseCanSpawn = false;
	bool isCheating = false;
	bool respawnMouse = false;
	bool hasWon = false;
	bool isInvicible = false;
	bool playerWins(string name, int score, bool hasCheated, int miceCounter, bool& hasWon, string& message, const int miceNeeded);
	void mongooseCanPlace(int& miceCounter, bool& mongooseCanSpawn);
	void checkHighScore(string& name, int& highscore, bool& unlock1, bool& unlock2, vector<string>& Scores);
	void pillCooldown(bool& pillCanSpawn, int& cooldown);
	void pillCanPlace(int& currentMice, bool& pillCanSpawn, int& cooldown, const char g[][SIZEX], Item& pill);
	void updateScore(int& score);
	void getPlayerName(string& name);
	void setLevel(int& level, const bool unlock1, const bool unlock2);
	void initialiseGame(char g[][SIZEX], char m[][SIZEX], vector<Item>& snake, bool& respawnMouse, Item& mouse, bool& isCheating, Item& pill, bool& pillCanSpawn, int& currentMice, bool& mongooseCanSpawn, Item& MONGOOSE, int& miceCounter, const int level, int& miceNeeded);
	void renderGame(const char g[][SIZEX], const string& mess, string& name, int& highscore, int& score, int& miceCounter, const int miceNeeded);
	void updateGame(char g[][SIZEX], const char m[][SIZEX], vector<Item>& s, const int kc, string& mess, int& pressed, bool& dead, bool&respawnMouse, Item& mouse, bool& isCheating, Item& pill, bool& pillCanSpawn, int& miceCounter, int& currentMice, string name, int score, bool hasCheated, bool hasWon, bool& mongooseCanSpawn, Item& mongoose, const int miceNeeded);
	bool wantsToQuit(const int key);
	bool wantsToCheat(const int key);
	bool isArrowKey(const int k);
	bool Unlocked(const int key);
	bool wantsBestScores(const int key);
	bool playerDeath(bool, int score, string name);
	int  getKeyPress();
	void endProgram(string name, int score, bool hasCheated, bool hasWon, const int level, const bool AllUnlocked, vector<string> Scores);
	void CheatFunction(char grid[][SIZEX], const char maze[][SIZEX], vector<Item>& snake, bool& respawnMouse, const Item& mouse, bool& isCheating, Item& pill, bool& pillCanSpawn, bool& hasCheated);
	void showBest(vector<string> BestScores);

	void clearScreen();
	void clearMenu();

	//local variable declarations 
	char grid[SIZEY][SIZEX];			//grid for display
	char maze[SIZEY][SIZEX];			//structure of the maze
	Item mongoose = { 0, 0, MONGOOSE };
	Item mouse = { 0, 0, MOUSE };
	Item pill = { 0, 0, PILL };
	vector<Item> snake = { { 0,0,SPOT },{ 0,0,BODY },{ 0,0,BODY },{ 0,0,BODY } };

	vector<string> BestScores = { "","","","","","" };

	string message("LET'S START...");	//current message to player
	seed();								//seed the random number generator
	SetConsoleTitle("FoP 2018-19 - Task 1c - Game Skeleton");
	getPlayerName(name);
	checkHighScore(name, highscore, unlock1, unlock2, BestScores);

	setLevel(level, unlock1, unlock2);

	clearScreen();

	initialiseGame(grid, maze, snake, respawnMouse, mouse, isCheating, pill, pillCanSpawn, currentMice, mongooseCanSpawn, mongoose, miceCounter, level, miceNeeded);	//initialise grid (incl. walls and spot)
	char key;							//current key selected by player
	do {
		renderGame(grid, message, name, highscore, score, miceCounter, miceNeeded);			//display game info, modified grid and messages							
		key = toupper(getKeyPress()); 	//read in  selected key: arrow or letter command
		pillCanPlace(currentMice, pillCanSpawn, cooldown, grid, pill);
		mongooseCanPlace(miceCounter, mongooseCanSpawn);
		playerWins(name, score, hasCheated, miceCounter, hasWon, message, miceNeeded);

		if (isArrowKey(key)) {
			pillCooldown(pillCanSpawn, cooldown);
			updateScore(score);

			updateGame(grid, maze, snake, key, message, pressed, dead, respawnMouse, mouse, isCheating, pill, pillCanSpawn, miceCounter, currentMice, name, score, hasCheated, hasWon, mongooseCanSpawn, mongoose, miceNeeded);
		}
		else if (wantsToCheat(key)) {
			CheatFunction(grid, maze, snake, respawnMouse, mouse, isCheating, pill, pillCanSpawn, hasCheated);
			message = "CHEAT MODE";
		}
		else if (Unlocked(key)) {
			AllUnlocked = true;
			message = "Levels Unlocked";
		}
		else if (wantsBestScores(key)) {
			clearMenu();
			showBest(BestScores);
		}
		else if (!wantsBestScores(key) && !Unlocked(key) && !wantsToCheat(key) && !isArrowKey(key)) {
			message = "INVALID KEY!";  //set 'Invalid key' message
		}
	} while (!wantsToQuit(key) && !playerDeath(dead, score, name) && !hasWon);		//while user does not want to quit & player is not dead & player has not won
	renderGame(grid, message, name, highscore, score, miceCounter, miceNeeded);			//display game info, modified grid and messages
	endProgram(name, score, hasCheated, hasWon, level, AllUnlocked, BestScores);						//display final message
	return 0;
}
//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------


void getPlayerName(string& name)
{
	//void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
	int characterLimit = 20;
	selectTextColour(clWhite);
	do
	{

		//showMessage(clBlack, clWhite, 40, 6, "ENTER PLAYER NAME (Maximum 20 Characters): ");
		cout << "\n\n\n\n\n\n\n        ENTER PLAYER NAME (Maximum 20 Characters): ";
		cin >> name;

		system("CLS");
	} while (name.length() > characterLimit);
}

void setLevel(int& level, const bool unlock1, const bool unlock2) {
	//void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
	int chosenLevel;
	//showMessage(clBlack, clWhite, 40, 6, "Enter Level Number (1 (easy) - 3 (hard)): ");
	cout << "\n\n\n\n\n\n\n        Enter Level Number (1 (easy) - 3 (hard)): ";
	cin >> chosenLevel;

	while (chosenLevel != 1 && chosenLevel != 2 && chosenLevel != 3) {

		//showMessage(clBlack, clWhite, 40, 6, "Please, Enter a VALID Level Number (1 (easy) - 3 (hard)): ");
		cout << "\n\n\n\n\n\n\n    Please, Enter a VALID Level Number (1 (easy) - 3 (hard)): ";
		cin >> chosenLevel;
	}

	if (chosenLevel == 1) {
		level = 1;
		//miceNeeded = 10;
	}

	if (chosenLevel == 2 && unlock1 == true) {
		level = 2;
		//miceNeeded = 12;
	}
	else if (chosenLevel == 2 && unlock1 == false) {
		//showMessage(clBlack, clWhite,40, 6, "Sorry, only level 1 is available for you.");
		//showMessage(clWhite, clRed, 40, 14, "Game Over! Thanks for Playing");

		cout << "\n\n\n\n\n\n\n    Sorry, only level 1 is available for you. \n";
		system("Pause");
		level = 1;
	}

	if (chosenLevel == 3 && unlock2 == true) {
		level = 3;
		//miceNeeded = 15;
	}
	else if (chosenLevel == 3 && unlock2 == false) {
		//showMessage(clBlack, clWhite, 40, 6, "Sorry, this level is unavailable to you.");
		//showMessage(clBlack, clWhite, 40, 7, "Level 2 it is. ");
		cout << "\n\n\n\n\n\n\n    Sorry, this level is unavailable to you. \n    Level 2 it is. \n";
		system("Pause");
		level = 2;
	}
}


void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], vector<Item>& snake, bool& respawnMouse, Item& mouse, bool& isCheating, Item& pill, bool& pillCanSpawn, int& currentMice, bool& mongooseCanSpawn, Item& mongoose, int& miceCounter, const int level, int& miceNeeded)
{											 //initialise grid and place spot in middle
	void setMouseInitialCoordinates(char maze[][SIZEX], Item& mouse);
	void setInitialMazeStructure(char maze[][SIZEX], const int level, int& miceNeeded);
	void setSnakeInitialCoordinates(vector<Item>& snake, char grid[][SIZEX]);
	void setPillInitialCoordinates(char maze[][SIZEX], Item& pill);
	void setMongooseInitialCoordinates(char maze[][SIZEX], Item& mongoose);
	void updateGrid(char g[][SIZEX], const char m[][SIZEX], const vector<Item>& i, bool& respawnMouse, const Item& mouse, bool& isCheating, Item& pill, bool& pillCanSpawn, bool& mongooseCanSpawn, Item& mongoose);

	setInitialMazeStructure(maze, level, miceNeeded);
	setSnakeInitialCoordinates(snake, maze);
	setMouseInitialCoordinates(maze, mouse);
	setPillInitialCoordinates(maze, pill);
	setMongooseInitialCoordinates(maze, mongoose);
	updateGrid(grid, maze, snake, respawnMouse, mouse, isCheating, pill, pillCanSpawn, mongooseCanSpawn, mongoose);
}
void setSnakeInitialCoordinates(vector<Item>& snake, char maze[][SIZEX])
{
	while (maze[snake.at(snake.size() - 1).x][snake.at(snake.size() - 1).y] != TUNNEL) // STILL CANT FIX THE TAIL SPAWNING OVER THE HEAD GLITCH
	{
		snake.at(snake.size() - 1).y = random(SIZEY - 2);
		snake.at(snake.size() - 1).x = random(SIZEY - 2);
	}

	//int count = snake.size()-1; 

	for (size_t i(snake.size() - 2); i != -1; --i)
	{
		snake.at(i).y = snake.at(snake.size() - 1).y;
		snake.at(i).x = snake.at(snake.size() - 1).x;
		//count--;
	}

}

void setMouseInitialCoordinates(char maze[][SIZEX], Item& mouse)
{
	while (maze[mouse.y][mouse.x] != TUNNEL)
	{
		mouse.y = random(SIZEY - 2);
		mouse.x = random(SIZEX - 2);
	}
}


void setPillInitialCoordinates(char maze[][SIZEX], Item& pill)
{
	while (maze[pill.y][pill.x] != TUNNEL)
	{
		pill.y = random(SIZEY - 2);
		pill.x = random(SIZEX - 2);
	}
}

void setMongooseInitialCoordinates(char maze[][SIZEX], Item& mongoose)
{
	while (maze[mongoose.y][mongoose.x] != TUNNEL)
	{
		mongoose.y = random(SIZEY - 2);
		mongoose.x = random(SIZEX - 2);
	}
}

void setInitialMazeStructure(char maze[][SIZEX], const int level, int& miceNeeded)
{													 //set the position of the walls in the maze
	if (level == 1) {

		fstream level1;

		level1.open("Level1.maz.txt", ios::in);

		if (level1.fail())
		{
			"ERROR!! CLOSE PROGRAM!!";
		}

		else
		{
			level1 >> miceNeeded;

			while (level1) {
				string input;
				//level1 >> input;
				char initialMaze[SIZEY][SIZEX];

				//strcpy(initialMaze, input.c_str());

				for (int i = 0; i < SIZEY; ++i) {
					for (int c = 0; c < SIZEX; ++c) {
						initialMaze[i][c] = level1.get();
					}
				}

				for (int row(0); row < SIZEY; ++row)
					for (int col(0); col < SIZEX; ++col)
						switch (initialMaze[row][col]) {
						case '#': maze[row][col] = WALL; break;
						case ' ': maze[row][col] = TUNNEL; break;
						case '@': maze[row][col] = MOUSE; break;
						}
			}

		}
	}

	if (level == 2) {
		fstream level2;

		level2.open("Level2.maz.txt", ios::in);

		if (level2.fail())
		{
			"ERROR!! CLOSE PROGRAM!!";
		}

		else
		{
			level2 >> miceNeeded;

			while (level2) {
				string input;
				//level1 >> input;
				char initialMaze[SIZEY][SIZEX];

				//strcpy(initialMaze, input.c_str());

				for (int i = 0; i < SIZEY; ++i) {
					for (int c = 0; c < SIZEX; ++c) {
						initialMaze[i][c] = level2.get();
					}
				}

				for (int row(0); row < SIZEY; ++row)
					for (int col(0); col < SIZEX; ++col)
						switch (initialMaze[row][col]) {
						case '#': maze[row][col] = WALL; break;
						case ' ': maze[row][col] = TUNNEL; break;
						case '@': maze[row][col] = MOUSE; break;
						}
			}

		}
	}

	if (level == 3) {

		fstream level3;

		level3.open("Level3.maz.txt", ios::in);

		if (level3.fail())
		{
			"ERROR!! CLOSE PROGRAM!!";
		}

		else
		{
			level3 >> miceNeeded;

			while (level3) {
				string input;
				//level1 >> input;
				char initialMaze[SIZEY][SIZEX];

				//strcpy(initialMaze, input.c_str());

				for (int i = 0; i < SIZEY; ++i) {
					for (int c = 0; c < SIZEX; ++c) {
						initialMaze[i][c] = level3.get();
					}
				}

				for (int row(0); row < SIZEY; ++row)
					for (int col(0); col < SIZEX; ++col)
						switch (initialMaze[row][col]) {
						case '#': maze[row][col] = WALL; break;
						case ' ': maze[row][col] = TUNNEL; break;
						case '@': maze[row][col] = MOUSE; break;
						}
			}

		}
	}
}
//---------------------------------------------------------------------------
//----- Update Game
//---------------------------------------------------------------------------
void updateGame(char grid[][SIZEX], const char maze[][SIZEX], vector<Item>& snake, const int keyCode, string& mess, int& pressed, bool& dead, bool& respawnMouse, Item& mouse, bool& isCheating, Item& pill, bool& pillCanSpawn, int& miceCounter, int& currentMice, string name, int score, bool hasCheated, bool hasWon, bool& mongooseCanSpawn, Item& mongoose, const int miceNeeded)
{
	void updateGameData(const char g[][SIZEX], vector<Item>& snake, const int key, string& mess, int& pressed, bool& dead, bool& respawnMouse, Item& mouse, bool& isCheating, Item& pill, bool& pillCanSpawn, int& miceCounter, int& currentMice, string name, int score, bool hasCheated, bool hasWon, bool& mongooseCanSpawn, const int miceNeeded);
	void updateGrid(char g[][SIZEX], const char maze[][SIZEX], const vector<Item>& s, bool& respawnMouse, const Item& mouse, bool& isCheating, Item& pill, bool& pillCanSpawn, bool& mongooseCanSpawn, Item& mongoose);
	updateGameData(grid, snake, keyCode, mess, pressed, dead, respawnMouse, mouse, isCheating, pill, pillCanSpawn, miceCounter, currentMice, name, score, hasCheated, hasWon, mongooseCanSpawn, miceNeeded);		//move spot in required direction
	updateGrid(grid, maze, snake, respawnMouse, mouse, isCheating, pill, pillCanSpawn, mongooseCanSpawn, mongoose);					//update grid information
}

void updateGameData(const char g[][SIZEX], vector<Item>& snake, const int key, string& mess, int& pressed, bool& dead, bool& respawnMouse, Item& mouse, bool& isCheating, Item& pill, bool& pillCanSpawn, int& miceCounter, int& currentMice, string name, int score, bool hasCheated, bool hasWon, bool& mongooseCanSpawn, const int miceNeeded)
{
	void setKeyDirection(int k, int& dx, int& dy, int& pressed);
	void snakeMove(vector<Item>& snake, int dx, int dy);
	void snakeLong(vector<Item>& snake);
	void newMouse(const char g[][SIZEX], Item& mouse);

	bool playerWins(string name, int score, bool hasCheated, int miceCounter, bool& hasWon, string& mess, const int miceNeeded);
	bool isArrowKey(const int k);
	bool playerDeath(bool, int score, string name);

	//reset message to blank
	mess = "";

	//calculate direction of movement for given key
	int dx(0), dy(0);

	setKeyDirection(key, dx, dy, pressed);

	//check new target position in grid and update game data (incl. spot coordinates) if move is possible
	switch (g[snake.at(0).y + dy][snake.at(0).x + dx]) {
	case TUNNEL://can move
		for (size_t i(snake.size() - 1); i > 0; --i) {
			snake.at(i).y = snake.at(i - 1).y;
			snake.at(i).x = snake.at(i - 1).x;
		}
		snake.at(0).y += dy;	//go in that Y direction
		snake.at(0).x += dx;	//go in that X direction

		break;
	case WALL:  		//hit a wall and stay there
						//cout << '\a';	//beep the alarm
		mess = "GameOver!";
		dead = true;
		break;
	case BODY:

		mess = "GameOver!";
		dead = true;
		break;

	case MOUSE:

		snakeMove(snake, dx, dy);

		if (!isCheating)
		{
			snakeLong(snake);
		}
		respawnMouse = true;

		newMouse(g, mouse);
		miceCounter++;
		currentMice++;
		playerWins(name, score, hasCheated, miceCounter, hasWon, mess, miceNeeded);

		break;

	case PILL:

		snakeMove(snake, dx, dy);
		snake.resize(4);
		pillCanSpawn = false;
		break;

	case MONGOOSE:
		snakeMove(snake, dx, dy);
		mess = "KILLED BY MONGOOSE!";
		dead = true;
		mongooseCanSpawn = false;
		break;
	}
}

void snakeMove(vector<Item>& snake, int dx, int dy)
{
	for (size_t i(snake.size() - 1); i > 0; --i)
	{
		snake.at(i).y = snake.at(i - 1).y;
		snake.at(i).x = snake.at(i - 1).x;
	}
	snake.at(0).y += dy;	//go in that Y direction
	snake.at(0).x += dx;	//go in that X direction
}

void snakeLong(vector<Item>& snake)
{
	snake.push_back({ snake.at(snake.size() - 1).x,snake.at(snake.size() - 1).y,BODY });
	snake.push_back({ snake.at(snake.size() - 1).x,snake.at(snake.size() - 1).y,BODY });
}


void newMouse(const char g[][SIZEX], Item& mouse)
{
	while (g[mouse.y][mouse.x] != TUNNEL)  // uses maze to prevent spawning on inner wall OR on power pill
	{
		mouse.y = random(SIZEY - 2);
		mouse.x = random(SIZEX - 2);
	}
}

void newPlace(const char g[][SIZEX], Item& item) {

	//item.y = random(SIZEY - 2);
	//item.x = random(SIZEX - 2);

	while (g[item.y][item.x] != TUNNEL)  // uses maze to prevent spawning on inner wall OR on power pill
	{
		item.y = random(SIZEY - 2);
		item.x = random(SIZEX - 2);
	}
}

void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], const vector<Item>& snake, bool& respawnMouse, const Item& mouse, bool& isCheating, Item& pill, bool& pillCanSpawn, bool& mongooseCanSpawn, Item& mongoose)
{																 //update grid configuration after each move
	void placeMaze(char g[][SIZEX], const char b[][SIZEX]);
	void placeItem(char g[][SIZEX], const Item& mouse);
	void newplace(char g[][SIZEX], const Item&);

	placeMaze(grid, maze);	//reset the empty maze configuration into grid
	for (size_t i(0); i < snake.size(); ++i) {
		placeItem(grid, snake.at(i));	//set snake grid
	}
	placeItem(grid, mouse);	//set snake grid

	if (pillCanSpawn)
	{
		newPlace(grid, pill);
		placeItem(grid, pill);
	}

	if (mongooseCanSpawn)
	{
		newPlace(grid, mongoose);
		placeItem(grid, mongoose);
	}

}

void placeMaze(char grid[][SIZEX], const char maze[][SIZEX])
{																 //reset the empty/fixed maze configuration into grid
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			grid[row][col] = maze[row][col];
}

void placeItem(char g[][SIZEX], const Item& item)
{																 //place item at its new position in grid
	g[item.y][item.x] = item.symbol;
}

void setKeyDirection(const int key, int& dx, int& dy, int& pressed)
{																 //calculate direction indicated by key
	bool isArrowKey(const int k);
	assert(isArrowKey(key));
	switch (key) {	//...depending on the selected key... 
	case LEFT:  	//when LEFT arrow pressed...
		dx = -1;	//decrease the X coordinate
		dy = 0;
		pressed = pressed + 1;
		break;
	case RIGHT: 	//when RIGHT arrow pressed...
		dx = +1;	//increase the X coordinate
		dy = 0;
		pressed = pressed + 1;
		break;
	case UP:
		dx = 0;
		dy = -1;
		pressed = pressed + 1;
		break;
	case DOWN:
		dx = 0;
		dy = +1;
		pressed = pressed + 1;
		break;
	}
}

int getKeyPress()
{												 //get key or command selected by user
												 //KEEP THIS FUNCTION AS GIVEN
	int keyPressed;
	keyPressed = _getch();			//read in the selected arrow key or command letter
	while (keyPressed == 224) 		//ignore symbol following cursor key
		keyPressed = _getch();

	return keyPressed;
}

bool isArrowKey(const int key)
{										//check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')

	return (key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN);
}
bool wantsToQuit(const int key)
{													//check if the user wants to quit (when key is 'Q' or 'q')
	return key == QUIT;
}

bool wantsToCheat(const int key)
{
	return key == Cheat;
}

bool wantsBestScores(const int key) {
	return key == BestScoreShow;
}

void CheatFunction(char grid[][SIZEX], const char maze[][SIZEX], vector<Item>& snake, bool& respawnMouse, const Item& mouse, bool& isCheating, Item& pill, bool& pillCanSpawn, bool& hasCheated)
{
	hasCheated = true;
	isCheating = !isCheating;
	snake.resize(4);

}

void pillCanPlace(int& currentMice, bool& pillCanSpawn, int& cooldown, const char g[][SIZEX], Item& pill)
{
	if (currentMice == 2)
	{
		currentMice = 0;
		pillCanSpawn = true;
	}

	if (cooldown == 10)
	{
		pillCanSpawn = false;
		cooldown = 0;

	}
}

void pillCooldown(bool& pillCanSpawn, int& cooldown)
{
	if (pillCanSpawn)
	{
		cooldown++;
	}
}

void mongooseCanPlace(int& miceCounter, bool& mongooseCanSpawn)
{
	if (miceCounter == 3)
	{
		mongooseCanSpawn = true;
	}

}

bool Unlocked(const int key) {
	return key == Unlock;
}

//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------

string tostring(int x) {	//convert an integer to a string
	std::ostringstream os;
	os << x;
	return os.str();
}
string tostring(char x) {	//convert a char to a string
	std::ostringstream os;
	os << x;
	return os.str();
}
void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message) {	//display a string using specified colour at a given position 
	gotoxy(x, y);
	selectBackColour(backColour);
	selectTextColour(textColour);
	cout << message + string(40 - message.length(), ' ');
}
void renderGame(const char g[][SIZEX], const string& mess, string& name, int& highscore, int& score, int& miceCounter, const int miceNeeded) { //display game title, messages, maze, spot and other items on screen
	string tostring(char x);
	string tostring(int x);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
	void paintGrid(const char g[][SIZEX]);

	//display game title
	showMessage(clBlack, clYellow, 0, 0, "___GAME___");
	showMessage(clDarkBlue, clRed, 40, 0, getDate() + " " + getTime() + "        ");
	showMessage(clDarkBlue, clRed, 40, 1, "FoP Task 1c - February 2019           ");
	showMessage(clDarkBlue, clRed, 40, 2, "CS4G - Games 2 - CS4G2 b              ");
	showMessage(clDarkBlue, clRed, 40, 3, "Joe Cunningham - b8014629             ");
	showMessage(clDarkBlue, clRed, 40, 4, "Sam Spawton    - b8300914             ");
	showMessage(clDarkBlue, clRed, 40, 5, "Jake Faulkner  - b8022455             ");
	showMessage(clRed, clYellow, 40, 6, "TO MOVE  - USE KEYBOARD ARROWS        ");
	showMessage(clRed, clYellow, 40, 7, "TO QUIT  - ENTER 'Q' or 'q'           ");
	showMessage(clRed, clYellow, 40, 8, "TO CHEAT - ENTER 'C' or 'c'           ");
	showMessage(clRed, clYellow, 40, 9, "TO UNLOCK LEVELS - ENTER 'U' or 'u'   ");
	showMessage(clRed, clYellow, 40, 10, "TO SHOW BEST SCORES - ENTER 'B' or 'b'");
	showMessage(clRed, clYellow, 40, 11, "PLAYERS NAME     : " + name);
	showMessage(clRed, clYellow, 40, 12, "BEST SCORE SO FAR: " + tostring(highscore));
	showMessage(clRed, clYellow, 40, 13, "CURRENT SCORE    : " + tostring(score));
	showMessage(clRed, clYellow, 40, 14, "MICE COLLECTED   : " + tostring(miceCounter) + "/" + tostring(miceNeeded));

	//print auxiliary messages if any
	showMessage(clWhite, clRed, 40, 15, mess);	//display current message

	paintGrid(g);
}

void paintGrid(const char g[][SIZEX]) { //display grid content on screen
	selectBackColour(clBlack);
	selectTextColour(clWhite);
	gotoxy(0, 2);

	for (int row(0); row < SIZEY; ++row)
	{
		for (int col(0); col < SIZEX; ++col)
		{
			if (g[row][col] == '0')
				selectTextColour(clRed);

			if (g[row][col] == 'o')
				selectTextColour(clGreen);

			if (g[row][col] == '@')
				selectTextColour(clBlue);

			if (g[row][col] == '+')
				selectTextColour(clYellow);

			if (g[row][col] == 'M')
				selectTextColour(clRed);

			cout << g[row][col];	//output cell content
			selectTextColour(clWhite);
		}
		cout << endl;
	}
}

bool playerWins(string name, int score, bool hasCheated, int miceCounter, bool& hasWon, string& message, const int miceNeeded)
{
	if (miceCounter == miceNeeded)
	{
		message = "You Win!";
		hasWon = true;
	}
	return hasWon;

}

void updateScore(int& score)
{
	score++;
}

bool playerDeath(bool dead, int score, string name)
{
	return dead;
}

void checkHighScore(string& name, int& highscore, bool& unlock1, bool& unlock2, vector<string>& Scores)
{
	fstream checkFile;

	checkFile.open(name + ".txt", ios::in);

	if (checkFile.fail())
	{
		highscore = 500;
		unlock1 = false;
		unlock2 = false;
	}

	else
	{
		checkFile >> highscore;
		char unlocked1;
		char unlocked2;
		unlocked1 = checkFile.get();
		unlocked2 = checkFile.get();

		if (unlocked1 == 'y') {
			unlock1 = true;
		}
		else {
			unlock1 = false;
		}

		if (unlocked2 == 'y') {
			unlock2 = true;
		}
		else {
			unlock2 = false;
		}
	}

	fstream BestScores;

	BestScores.open("bestScores.txt", ios::in);

	if (BestScores.fail()) {
		BestScores.close();
		BestScores.sync();
		BestScores.open("bestScores.txt", ios::out);
		BestScores.setf(ios::fixed);
		BestScores << "anonymous\n-1\nanonymous\n-1\nanonymous\n-1\nanonymous\n-1\nanonymous\n-1\nanonymous\n-1\n";
	}
	else {
		string namescore1;
		std::string stringscore1;

		string namescore2;
		std::string stringscore2;

		string namescore3;
		std::string stringscore3;

		string namescore4;
		std::string stringscore4;

		string namescore5;
		std::string stringscore5;

		string namescore6;
		std::string stringscore6;


		getline(BestScores, namescore1);
		getline(BestScores, stringscore1);
		getline(BestScores, namescore2);
		getline(BestScores, stringscore2);
		getline(BestScores, namescore3);
		getline(BestScores, stringscore3);
		getline(BestScores, namescore4);
		getline(BestScores, stringscore4);
		getline(BestScores, namescore5);
		getline(BestScores, stringscore5);
		getline(BestScores, namescore6);
		getline(BestScores, stringscore6);

		string best1 = namescore1 + " " + stringscore1;
		string best2 = namescore2 + " " + stringscore2;
		string best3 = namescore3 + " " + stringscore3;
		string best4 = namescore4 + " " + stringscore4;
		string best5 = namescore5 + " " + stringscore5;
		string best6 = namescore6 + " " + stringscore6;

		Scores.at(0) = best1;
		Scores.at(1) = best2;
		Scores.at(2) = best3;
		Scores.at(3) = best4;
		Scores.at(4) = best5;
		Scores.at(5) = best6;
	}
}

void saveHighScore(string& name, int& score, bool& hasCheated, bool& hasWon, const int level, const bool AllUnlocked, vector<string> Scores)
{
	fstream fileManip;
	int lastScore;
	string lastentry;
	fileManip.open(name + ".txt", ios::in);
	//fileManip.open(name + ".txt", ios::out);
	//fileManip.setf(ios::fixed);
	if (fileManip.fail()) {

		fileManip.open(name + ".txt", ios::out);
		fileManip.setf(ios::fixed);

		if ((!hasCheated) && (hasWon)) {

			//fileManip >> lastScore;
			if (score < 500)
			{
				//fstream fileManip(name + ".txt", ios::out);
				//fileManip.setf(ios::fixed);
				fileManip << score;
				if (AllUnlocked == true) {
					//fstream fileManip(name + ".txt", ios::out);
					fileManip << "yy";
				}
				else if (level == 1) {
					//fstream fileManip(name + ".txt", ios::out);
					fileManip << "yn";
				}
				else if (level == 2) {
					//fstream fileManip(name + ".txt", ios::out);
					fileManip << "yy";
				}
				else {
					fileManip << "nn";
				}
			}
		}
		else {
			//fstream fileManip(name + ".txt", ios::out);
			fileManip << 500;

			if (AllUnlocked == true) {


				//fileManip << lastScore;
				fileManip << "yy";
			}
			else {
				fileManip << "nn";
			}
		}
		fileManip.close();

	}
	else {
		fileManip >> lastScore;
		fileManip >> lastentry;
		fileManip.close();
		fileManip.sync();
		fileManip.open(name + ".txt", ios::out);
		fileManip.setf(ios::fixed);

		if ((!hasCheated) && (hasWon)) {
			//fileManip.open(name + ".txt", ios::out);
			//fileManip.setf(ios::fixed);

			if (score < lastScore)
			{
				//fileManip.setf(ios::fixed);
				fileManip << score;
				if (AllUnlocked == true) {
					fileManip << "yy";
				}
				else if (level == 1) {
					fileManip << "yn";
				}
				else if (level == 2) {
					fileManip << "yy";
				}
			}
		}
		else if (AllUnlocked == true) {
			//fstream fileManip(name + ".txt", ios::out);
			//fileManip.setf(ios::fixed);

			fileManip << lastScore;
			fileManip << "yy";
		}
		else {
			fileManip << lastScore;
			fileManip << lastentry;
		}
		fileManip.close();
	}

	fstream BestScores;

	BestScores.open("bestScores.txt", ios::in);

	if (BestScores.fail()) {
		cout << "CATASTROPHIC ERROR!! CLOSE THE PROGRAM!!";
	}
	else {
		if ((!hasCheated) && (hasWon)) {
			int scores[6];

			string namescore1;
			int score1;
			std::string stringscore1;

			string namescore2;
			int score2;
			std::string stringscore2;

			string namescore3;
			int score3;
			std::string stringscore3;

			string namescore4;
			int score4;
			std::string stringscore4;

			string namescore5;
			int score5;
			std::string stringscore5;

			string namescore6;
			int score6;
			std::string stringscore6;

			const int checkscore = -1;

			getline(BestScores, namescore1);
			getline(BestScores, stringscore1);
			getline(BestScores, namescore2);
			getline(BestScores, stringscore2);
			getline(BestScores, namescore3);
			getline(BestScores, stringscore3);
			getline(BestScores, namescore4);
			getline(BestScores, stringscore4);
			getline(BestScores, namescore5);
			getline(BestScores, stringscore5);
			getline(BestScores, namescore6);
			getline(BestScores, stringscore6);

			std::istringstream ss1(stringscore1);
			ss1 >> scores[0];
			score1 = scores[0];
			//stringscore1 = ss1.str();

			std::istringstream ss2(stringscore2);
			ss2 >> scores[1];
			score2 = scores[1];
			//stringscore2 = ss2.str();

			std::istringstream ss3(stringscore3);
			ss3 >> scores[2];
			score3 = scores[2];
			//stringscore3 = ss3.str();

			std::istringstream ss4(stringscore4);
			ss4 >> scores[3];
			score4 = scores[3];
			//stringscore4 = ss4.str();

			std::istringstream ss5(stringscore5);
			ss5 >> scores[4];
			score5 = scores[4];
			//stringscore5 = ss5.str();

			std::istringstream ss6(stringscore6);
			ss6 >> scores[5];
			score6 = scores[5];
			//stringscore6 = ss6.str();

			BestScores.close();
			BestScores.sync();

			BestScores.open("bestScores.txt", ios::out);

			for (int i = 0; i < 6; ++i) {
				if (scores[i] == checkscore) {
					scores[i] = 500;
				}
			}

			if (score <= scores[0]) {

				BestScores << name;
				BestScores << "\n";
				BestScores << score;
				BestScores << "\n";
				BestScores << namescore1;
				BestScores << "\n";
				BestScores << score1;
				BestScores << "\n";
				BestScores << namescore2;
				BestScores << "\n";
				BestScores << score2;
				BestScores << "\n";
				BestScores << namescore3;
				BestScores << "\n";
				BestScores << score3;
				BestScores << "\n";
				BestScores << namescore4;
				BestScores << "\n";
				BestScores << score4;
				BestScores << "\n";
				BestScores << namescore5;
				BestScores << "\n";
				BestScores << score5;
				BestScores << "\n";

			}
			else if (score <= scores[1] && score > scores[0]) {

				BestScores << namescore1;
				BestScores << "\n";
				BestScores << score1;
				BestScores << "\n";
				BestScores << name;
				BestScores << "\n";
				BestScores << score;
				BestScores << "\n";
				BestScores << namescore2;
				BestScores << "\n";
				BestScores << score2;
				BestScores << "\n";
				BestScores << namescore3;
				BestScores << "\n";
				BestScores << score3;
				BestScores << "\n";
				BestScores << namescore4;
				BestScores << "\n";
				BestScores << score4;
				BestScores << "\n";
				BestScores << namescore5;
				BestScores << "\n";
				BestScores << score5;
				BestScores << "\n";

			}
			else if (score <= scores[2] && score > scores[1]) {

				BestScores << namescore1;
				BestScores << "\n";
				BestScores << score1;
				BestScores << "\n";
				BestScores << namescore2;
				BestScores << "\n";
				BestScores << score2;
				BestScores << "\n";
				BestScores << name;
				BestScores << "\n";
				BestScores << score;
				BestScores << "\n";
				BestScores << namescore3;
				BestScores << "\n";
				BestScores << score3;
				BestScores << "\n";
				BestScores << namescore4;
				BestScores << "\n";
				BestScores << score4;
				BestScores << "\n";
				BestScores << namescore5;
				BestScores << "\n";
				BestScores << score5;
				BestScores << "\n";

			}
			else if (score <= scores[3] && score > scores[2]) {

				BestScores << namescore1;
				BestScores << "\n";
				BestScores << score1;
				BestScores << "\n";
				BestScores << namescore2;
				BestScores << "\n";
				BestScores << score2;
				BestScores << "\n";
				BestScores << namescore3;
				BestScores << "\n";
				BestScores << score3;
				BestScores << "\n";
				BestScores << name;
				BestScores << "\n";
				BestScores << score;
				BestScores << "\n";
				BestScores << namescore4;
				BestScores << "\n";
				BestScores << score4;
				BestScores << "\n";
				BestScores << namescore5;
				BestScores << "\n";
				BestScores << score5;
				BestScores << "\n";

			}
			else if (score <= scores[4] && score > scores[3]) {

				BestScores << namescore1;
				BestScores << "\n";
				BestScores << score1;
				BestScores << "\n";
				BestScores << namescore2;
				BestScores << "\n";
				BestScores << score2;
				BestScores << "\n";
				BestScores << namescore3;
				BestScores << "\n";
				BestScores << score3;
				BestScores << "\n";
				BestScores << namescore4;
				BestScores << "\n";
				BestScores << score4;
				BestScores << "\n";
				BestScores << name;
				BestScores << "\n";
				BestScores << score;
				BestScores << "\n";
				BestScores << namescore5;
				BestScores << "\n";
				BestScores << score5;
				BestScores << "\n";

			}
			else if (score <= scores[5] && score > scores[4]) {

				BestScores << namescore1;
				BestScores << "\n";
				BestScores << score1;
				BestScores << "\n";
				BestScores << namescore2;
				BestScores << "\n";
				BestScores << score2;
				BestScores << "\n";
				BestScores << namescore3;
				BestScores << "\n";
				BestScores << score3;
				BestScores << "\n";
				BestScores << namescore4;
				BestScores << "\n";
				BestScores << score4;
				BestScores << "\n";
				BestScores << namescore5;
				BestScores << "\n";
				BestScores << score5;
				BestScores << "\n";
				BestScores << name;
				BestScores << "\n";
				BestScores << score;
				BestScores << "\n";

			}
			else {

				BestScores << namescore1;
				BestScores << "\n";
				BestScores << score1;
				BestScores << "\n";
				BestScores << namescore2;
				BestScores << "\n";
				BestScores << score2;
				BestScores << "\n";
				BestScores << namescore3;
				BestScores << "\n";
				BestScores << score3;
				BestScores << "\n";
				BestScores << namescore4;
				BestScores << "\n";
				BestScores << score4;
				BestScores << "\n";
				BestScores << namescore5;
				BestScores << "\n";
				BestScores << score5;
				BestScores << "\n";
				BestScores << namescore6;
				BestScores << "\n";
				BestScores << score6;
				BestScores << "\n";
			}
		}
	}
	BestScores.close();
}

void showBest(const vector<string> BestScores) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
	showMessage(clWhite, clRed, 40, 16, "  Best Scores Are:  ");
	showMessage(clWhite, clRed, 40, 17, BestScores.at(0) + " ");
	showMessage(clWhite, clRed, 40, 18, BestScores.at(1) + " ");
	showMessage(clWhite, clRed, 40, 19, BestScores.at(2) + " ");
	showMessage(clWhite, clRed, 40, 20, BestScores.at(3) + " ");
	showMessage(clWhite, clRed, 40, 21, BestScores.at(4) + " ");
	showMessage(clWhite, clRed, 40, 22, BestScores.at(5) + " ");
}

void clearMenu() {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
	showMessage(clBlack, clWhite, 40, 0, "                           ");
	showMessage(clBlack, clWhite, 40, 1, "                           ");
	showMessage(clBlack, clWhite, 40, 2, "                           ");
	showMessage(clBlack, clWhite, 40, 3, "                           ");
	showMessage(clBlack, clWhite, 40, 4, "                           ");
	showMessage(clBlack, clWhite, 40, 5, "                           ");
	showMessage(clBlack, clWhite, 40, 6, "                           ");
	showMessage(clBlack, clWhite, 40, 7, "                           ");
	showMessage(clBlack, clWhite, 40, 8, "                           ");
	showMessage(clBlack, clWhite, 40, 9, "                           ");
	showMessage(clBlack, clWhite, 40, 10, "                           ");
	showMessage(clBlack, clWhite, 40, 11, "                           ");
	showMessage(clBlack, clWhite, 40, 12, "                           ");
	showMessage(clBlack, clWhite, 40, 13, "                           ");
	showMessage(clBlack, clWhite, 40, 14, "                           ");
}

void clearScreen() {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
	showMessage(clBlack, clWhite, 0, 0, "                           ");
	showMessage(clBlack, clWhite, 0, 1, "                           ");
	showMessage(clBlack, clWhite, 0, 2, "                           ");
	showMessage(clBlack, clWhite, 0, 3, "                           ");
	showMessage(clBlack, clWhite, 0, 4, "                           ");
	showMessage(clBlack, clWhite, 0, 5, "                           ");
	showMessage(clBlack, clWhite, 0, 6, "                           ");
	showMessage(clBlack, clWhite, 0, 7, "                           ");
	showMessage(clBlack, clWhite, 0, 8, "                           ");
	showMessage(clBlack, clWhite, 0, 9, "                           ");
	showMessage(clBlack, clWhite, 0, 10, "                          ");
	showMessage(clBlack, clWhite, 0, 11, "                          ");
	showMessage(clBlack, clWhite, 0, 12, "                          ");
	showMessage(clBlack, clWhite, 0, 13, "                          ");

}

void endProgram(string name, int score, bool hasCheated, bool hasWon, const int level, const bool AllUnlocked, vector<string> Scores)
{
	void saveHighScore(string& name, int& score, bool& hasCheated, bool& hasWon, const int level, const bool AllUnlocked, vector<string> Scores);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);

	saveHighScore(name, score, hasCheated, hasWon, level, AllUnlocked, Scores);

	showMessage(clWhite, clRed, 40, 15, "Game Over! Thanks for Playing");

	cout << "\n\n\n\n";
	system("pause");
}

