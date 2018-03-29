#include <iostream>
#include <conio.h>
#include <vector>
#include <windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;

struct SnakeHead{
	int length;
	int x;
	int y;
	char head;
	char movingDirection;
	int score;
};

struct Food {
	int rowPosition;
	int columnPosition;
	char symbol;
};
//controls the flow of the game, and when it ends
bool GameIsRunning = true; 
//these variables will control how wide and how long the board is. All you have to do change these variables, and the board size 
//in the output will adjust accordingly
const int NUM_COLUMNS = 50, NUM_ROWS = 20;
const int SPEED = 80; //lower is faster, higher is slower!
int prevSnakeSegX, prevSnakeSegY;//these variables will hold the previous x and y position of each Snake segment
void printBoard(SnakeHead &Snake, Food &FoodItem);
void placeFood(Food &FoodItem, vector<int> &tailx, vector<int> &taily);
void playGame(SnakeHead &Snake, Food &FoodItem, vector<int> &tailx, vector<int> &taily);
void drawLegend(const SnakeHead &Snake, const Food &FoodItem);
void movement(SnakeHead &Snake);
void readMovement(SnakeHead &Snake, Food &FoodItem, vector<int> &tailx, vector<int> &taily);
void XYCoordinatesBox();
void gotoxy(int x, int y);
void flickerHead(SnakeHead &Snake, const int &headPreviousX, const int &headPreviousY);
void setData(SnakeHead &Snake, Food &FoodItem, vector<int> &tailx, vector<int> &taily);
bool selfCollision(SnakeHead &snake, vector<int> &tailx, vector<int> &taily);
void createSnakeBody(SnakeHead &Snake, vector<int> &tailx, vector<int> &taily);

int main()
{
	Food FoodItem;
	SnakeHead Snake;
	vector<int> tailx(20), taily(20);//these arrays will hold the x and y position of each snake segement, and will grow
	//accordingly each time a Food object is eaten
	srand(time(0));
	playGame(Snake, FoodItem, tailx, taily);

	cout << "\n\nEnter to continue...";
	cin.get();
	return 0;
}

void playGame(SnakeHead &Snake, Food &FoodItem, vector<int> &tailx, vector<int> &taily) {
	setData(Snake, FoodItem, tailx, taily);
	printBoard(Snake, FoodItem);
	placeFood(FoodItem, tailx, taily);
	while (GameIsRunning) {
		movement(Snake);//read the keyboard input the user entered at the console
		readMovement(Snake, FoodItem, tailx, taily);//analyze which key was pressed, and act accordingly
		Sleep(SPEED);//wait fifty milliseconds before going to the next iteration of the loop
	}
}

void setData(SnakeHead &Snake, Food &FoodItem, vector<int> &tailx, vector<int> &taily) {
	Snake.score = 0;
	Snake.length = 1;
	Snake.x = NUM_COLUMNS/2, Snake.y = NUM_ROWS/2;
	tailx[0] = Snake.x, taily[0] = Snake.y;
	Snake.head = '0';
	FoodItem.symbol = 'f';
}

//print the borders of the game that will cause a game over if the snake strikes it
void printBoard(SnakeHead &Snake, Food &FoodItem)
{
	for (int i = 0; i < NUM_ROWS; i++) {
		if (i > 0 and i < NUM_ROWS - 1) {//creates the borders on the left and right
			cout << "*";
			for (int k = 1; k < NUM_COLUMNS - 1; k++)
				cout << " ";
			cout << "*\n";
			continue;
		}
		for (int j = 0; j < NUM_COLUMNS; j++)//creates top most and bottom most borders
			cout << "*";
		cout << endl;
	}
	drawLegend(Snake, FoodItem);
	XYCoordinatesBox();
}

//draw the legend over on the side to briefly guide the user on how the game is played
void drawLegend(const SnakeHead &Snake, const Food &FoodItem)
{
	const int BOX_WIDTH = 29, BOX_LENGTH = 19; //these variable determine how big the "legend" will be in the output

	gotoxy(NUM_COLUMNS + 15, 2); cout << "LEGEND:";

	gotoxy(NUM_COLUMNS, 1);
	for (int i = 0; i < BOX_WIDTH; i++)
		cout << "-";
	
	gotoxy(NUM_COLUMNS, BOX_LENGTH);
	for (int k = 0; k < BOX_WIDTH; k++)
		cout << "-";

	gotoxy(NUM_COLUMNS + 5, 4); cout << Snake.head << " -> SnakeHead\n";
	gotoxy(NUM_COLUMNS + 5, 6); cout << FoodItem.symbol << " -> Food\n";
	gotoxy(NUM_COLUMNS + 5, 8); cout << "#  -> Border\n";
	gotoxy(NUM_COLUMNS + 5, 10); cout << "w  -> Move Up\n";
	gotoxy(NUM_COLUMNS + 5, 12); cout << "s  -> Move Down\n";
	gotoxy(NUM_COLUMNS + 5, 14); cout << "a  -> Move Left\n";
	gotoxy(NUM_COLUMNS + 5, 16); cout << "d  -> Move Right";
}

//create the box that will print out the x and y coordinates of the Snake head
void XYCoordinatesBox()
{
	gotoxy(NUM_COLUMNS, 20);
	for (int i = 1; i < 10; i++) {
		cout << "|\n";
		gotoxy(NUM_COLUMNS, 20 + i);
	}		
}

//function to check to see if the user has inputted one of the four valid keys (w s a d), and will assign the appropiate value
//to the snakes current direction
void movement(SnakeHead &SnakeHead)
{
	if (_kbhit()) {//check to see if a key is pressed. IF true, execute the block of code below
		char input = _getch();//save the key the user pressed

		//if w is pressed, and the snake is not moving down, set the direction to up!
		if (input == 'w' and SnakeHead.movingDirection != 's') SnakeHead.movingDirection  = 'w';
		//if s is pressed, and the snake is not moving up, set the direction to down!
		else if (input == 's' and SnakeHead.movingDirection != 'w') SnakeHead.movingDirection = 's';
		//if a is pressed, and the snake is not moving right, set the direction to left!
		else if (input == 'a' and SnakeHead.movingDirection != 'd') SnakeHead.movingDirection = 'a';
		//finally, if d is pressed, and the snake is not moving right, set the direction to left!
		else if (input == 'd' and SnakeHead.movingDirection != 'a') SnakeHead.movingDirection = 'd';
	}
}

/*
this function is where all the magic happens. It will accomplish the following tasks: 
1) print out the snake head, and the snake body
2)fill the tailx and taily arrays with the x and y positions of each snake segment of the snake body
3)Increment the x or y position of the snake head based on what key the user pressed
4)Determine whether or not the snake head hit a wall, its own body, or a Food object*/
void readMovement(SnakeHead &Snake, Food &FoodItem, vector<int> &tailx, vector<int> &taily)
{
	//if the snake head has not hit a Food object yet, go to position of the previous instance of the snake head. Otherwise go
	//to the previous instance of the very last snake segment of the snake body
	(Snake.length == 1) ? gotoxy(tailx[0], taily[0]) : gotoxy(prevSnakeSegX, prevSnakeSegY);
	cout << " ";//print out a blank to prevent the snake from leaving copies of itself from previous iteration of this function
	for (int i = 1; i < Snake.length; i++) { //print out the snake body
		gotoxy(tailx[i], taily[i]); cout << Snake.head;
	}
	gotoxy(Snake.x, Snake.y); cout << Snake.head;//print out the snake head
	
	createSnakeBody(Snake, tailx, taily);

	int headPreviousX = Snake.x, headPreviousY = Snake.y;

	if (Snake.movingDirection == 'w') Snake.y--;
	else if (Snake.movingDirection == 's') Snake.y++;
	else if (Snake.movingDirection == 'a') Snake.x--;
	else if (Snake.movingDirection == 'd') Snake.x++;

	if ((Snake.x == 0 or Snake.x == NUM_COLUMNS -1) or (Snake.y == 0 or Snake.y == NUM_ROWS - 1) or selfCollision(Snake, tailx, taily)){
		flickerHead(Snake, headPreviousX, headPreviousY);
		gotoxy(30, 22); cout << "GAME OVER BITCH!!";
		GameIsRunning = false;
	}else if(Snake.length == NUM_COLUMNS * NUM_ROWS){
		gotoxy(30, 22); cout << "You win!";
		GameIsRunning = false;
	}else if (Snake.y == FoodItem.rowPosition and Snake.x == FoodItem.columnPosition) {
		placeFood(FoodItem, tailx, taily);
		Snake.score += 10;
		Snake.length += 5;
	}

	gotoxy(NUM_COLUMNS + 5, 20); 
	cout << "food X: " << FoodItem.columnPosition << "\tfood Y: " << FoodItem.rowPosition;
	cout << "\nscore: " << Snake.score << endl;
	cout << "Snake Length: " << Snake.length << endl;
}

void flickerHead(SnakeHead &Snake, const int &headPreviousX, const int &headPreviousY){
	for(int i = 0; i < 3; i++){
		gotoxy(headPreviousX, headPreviousY);
		cout << " ";
		Sleep(200);
		gotoxy(headPreviousX, headPreviousY);
		cout << Snake.head;
		Sleep(200);
	}
}

void placeFood(Food &FoodItem, vector<int> &tailx, vector<int> &taily){
	FoodItem.rowPosition = rand() % 18 + 1, FoodItem.columnPosition = rand() % (NUM_COLUMNS - 2) + 1;
	int len = tailx.size();

	for (int i = 0; i < len; i++) 
		if (FoodItem.rowPosition == taily[i] and FoodItem.columnPosition == tailx[i]) {
			FoodItem.rowPosition = rand() % 18 + 1, FoodItem.columnPosition = rand() % (NUM_COLUMNS - 2) + 1;
			i = 0;
		}
	gotoxy(FoodItem.columnPosition, FoodItem.rowPosition); cout << FoodItem.symbol;
}

//assigns the x and y positions of each snake segment to the tailx and taily array
void createSnakeBody(SnakeHead &Snake, vector<int> &tailx, vector<int> &taily) {
	int prevHeadX = tailx[0];//save the previous x position of the Snake head
	int prevHeadY = taily[0];//save the previous y position of the Snake head
	tailx[0] = Snake.x;//update the current x position of the Snake head for the next iteration of this function
	taily[0] = Snake.y;//do the same for the y position
	for (int i = 1; i < Snake.length; i++) {
		prevSnakeSegX = tailx[i];//save the "ith" position of the tailx array
		prevSnakeSegY = taily[i];//save the "ith position of the taily array
		tailx[i] = prevHeadX;//give tailx a new value from the previous position of the snake head (will be shifted down to last position)
		taily[i] = prevHeadY;//do the same thing for the taily array
		prevHeadX = prevSnakeSegX;//assign a new value from the previous x position of a segment to the x pos of the snake head
		prevHeadY = prevSnakeSegY;//do the same for the previous y position of the y head
		tailx.push_back(int());//increase the size of the tailx array
		taily.push_back(int());//do the same from the taily array
	}
}

bool selfCollision(SnakeHead &snake, vector<int> &tailx, vector<int> &taily) {
	int len = snake.length;

	for (int i = 0; i < len; i++) 
		if (snake.x == tailx[i] and snake.y == taily[i] and snake.length > 1)
			return true;
	return false;
}

void gotoxy(int x, int y){
	COORD coord;

	coord.X = x;
	coord.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
