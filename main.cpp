#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <conio.h>
#include <windows.h>
#include <cstddef>
using namespace std;

//F2 to show the compilation
class Players{
    public:
        void setName(string playerName){
            name = playerName;
        }
        void setSymbol(char playerSymbol){
            symbol = playerSymbol;
        }
        void setWinner(string win){
            signalWinner = win;
        }
        void setPosition(int pos){
            gamePos = pos;
        }
        string getName(){
            return name;
        }
        string getWinner(){
            return signalWinner;
        }
        char getSymbol(){
            return symbol;
        }
        int getPos(){
            return gamePos;
        }

    private:
        string name;
        char symbol;
        int gamePos;
        string signalWinner;
};

//the purpose of these variable are to allow the main program to know whether or not to call a function to play
//against another player, or one to play against the computer
//******************************
bool vsPlayer;
bool vsCPUnormal;
bool vsCPUhard;
//********************************
void createBoard(Players *players, char position[]);
bool placeMark(Players *player, char position[], int indicator, int numPlayers);
int determineWinner(char position[]);
void resetGame(Players *player, char position[]);
void playerChoice(Players *player, int indicator, int pos);
void play(Players *player, char position[], int numPlayers);
void computerNormalMove(Players *player, char position[], int indicator);
void playerChoice(Players *player, int indicator);
void checkPosition(Players *player, char position[], int indicator);
void mainMenu(Players *player, int numPlayers);
void gotoxy(int x, int y);
void displayMenu(int yvalue);
void startGame(Players *players, int numPlayers);
void displayOptions(int yposition);
void selectOptions(Players *players, int numPlayers);
void displayRules(Players *players, int numPlayers);
void displayDeveloperInformation(Players *players, int numPlayers);
void displayGameOptions(int yposition);
void twoplayerInformation (Players *players, int numPlayers);
void playervscomputerInformation(Players *players, int numPlayers);
bool blockPlayer(Players *player, char position[], int indicator);

int main()
{
    Players *players = new Players[2];
    int numPlayers = 2;
    char position[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

    mainMenu(players, numPlayers);
    createBoard(players, position);
    play(players, position, numPlayers);

    delete [] players;

    players = NULL;

    return 0;
}

void twoplayerInformation(Players *player, int numPlayers)
{
    srand(time(0));
    char mark, mark2;
    int rn = rand () % 2;//randomly generate either a 0 or 1, which will be used to assign either an X or O
    string name;

    mark = (rn == 0) ? 'X' :  'O';//assign an X or O to mark depending on what random number generator
    mark2 = (mark == 'X') ? 'O' : 'X';//depending on what symbol is assigned to mark, assign the opposite symbol to mark2

    for(int i = 0; i < numPlayers; i++){
        cout << "Player " << i+1 << ", enter your first name: ";
        getline(cin, name);
        player[i].setName(name);
        player[i].setPosition(0);//initialize each player position to zero.
        (i == 0) ? player[i].setSymbol(mark) : player[i].setSymbol(mark2);//player one gets mark, player two gets mark2
        cout << player[i].getName() << ", you will be using " << player[i].getSymbol() << "'s " << endl << endl;
    }
    system("pause");
    system("cls");
}

void playervscomputerInformation(Players *player, int numPlayers)
{
    srand(time(0));
    char mark, mark2;
    int rn = rand () % 2;
    string name;

    mark = (rn == 0) ? 'X' :  'O';
    mark2 = (mark == 'X') ? 'O' : 'X';

    cout << "Enter your first name: ";
    getline(cin, name);
    player[rn].setName(name);
    player[rn].setSymbol(mark);

    if(rn == 0){
        player[1].setName("CPU");
        player[1].setSymbol(mark2);
        player[1].setPosition(0);
    }else{
        player[0].setName("CPU");
        player[0].setSymbol(mark2);
        player[0].setPosition(0);
    }
    cout << player[rn].getName() << ", you are player " << rn + 1 << "!" << endl;
}

void play(Players *player, char position[], int numPlayers)
{
	int pos, turnNumber = 1;
	bool continueGame = true;

    do{
        while(vsPlayer && continueGame){
            for(int indicator = 0; indicator < numPlayers && continueGame == true; indicator++)
                continueGame = placeMark(player, position, indicator, numPlayers);//player one wins the game or ties
        }
        while(vsCPUnormal && continueGame){
             for(int indicator = 0; indicator < numPlayers && continueGame == true; indicator++)
                continueGame = placeMark(player, position, indicator, numPlayers);
        }
        //while(vsCPUhard && continueGame){}
        if(!continueGame){
            resetGame(player, position);
            continueGame = true;
        }
    }while(continueGame);


    cout << "Thank you for playing!" << endl;
}

//similar to check winner, but modified to allow the computer to process every possible near win scenario the player is in
//if the player has two of a kind, the computer will check to see where the player has two of a kind, and will block them!
bool blockPlayer(Players *player, char position[], int indicator)
{
    char mark = player[indicator].getSymbol(), playerMark;
    playerMark = (mark == 'X') ? 'O' : 'X';//the purpose of the "playerMark" variable is to signal to the Computer to ONLY perform
    //the blocking action if the symbol of the winning row is belongs to the player

    if((position[0] == position[1]) || (position[1] == position[2]) || (position[0] == position[2]))
        for(int i = 0; i < 3; i++)
            if((position[i] == '1' || position[i] == '2' || position[i] == '3')){
                  cout << "position changed! " << mark << " placed in position [" << i << "]" << endl;
                 system("pause");
                 position[i] = player[indicator].getSymbol();
                 return true;
            }
    else if ((position[3] == position[4]) || (position[4] == position[5]) || (position[3] == position[5]))
         for(int i = 3; i <= 5; i++)
            if((position[i] == '4' || position[i] == '5' || position[i] == '6')  && position[i] == playerMark){
                  cout << "position changed! " << mark << " placed in position [" << i << "]" << endl;
                system("pause");
                position[i] = player[indicator].getSymbol();
                return true;
            }
    else if ((position[6] == position[7]) || (position[7] == position[8]) || (position[6] == position[8]))
        for(int i = 6; i <= 8; i++)
            if((position[i] == '7' || position[i] == '8' || position[i] == '9')  && position[i] == playerMark){
                  cout << "position changed! " << mark << " placed in position [" << i << "]" << endl;
                system("pause");
                position[i] = player[indicator].getSymbol();
                return true;
            }
    else if ((position[0] == position[3]) || (position[3] == position[6]) || (position[0] == position[6]))
         for(int i = 0; i <= 6; i+=3)
            if((position[i] == '1' || position[i] == '4' || position[i] == '7') && position[i] == playerMark){
                  cout << "position changed! " << mark << " placed in position [" << i << "]" << endl;
                system("pause");
                 position[i] = player[indicator].getSymbol();
                return true;
            }
    else if ((position[1] == position[4]) || (position[4] == position[7]) || (position[1] == position[7]))
        for(int i = 1; i <= 7; i+=3)
            if((position[i] == '2' || position[i] == '5' || position[i] == '8') && position[i] == playerMark){
                  cout << "position changed! " << mark << " placed in position [" << i << "]" << endl;
                system("pause");
                position[i] = player[indicator].getSymbol();
                return true;
            }
    else if ((position[2] == position[5]) || (position[5] == position[8]) || (position[2] == position[8]))
        for(int i = 2; i <= 8; i+=3)
            if((position[i] == '3' || position[i] == '6' || position[i] == '9') && position[i] == playerMark){
                cout << "position changed! " << mark << " placed in position [" << i << "]" << endl;
                system("pause");
                position[i] = player[indicator].getSymbol();
                return true;
            }
    else if ((position[0] == position[4]) || (position[4] == position[8]) || (position[0] == position[8]))
        for(int i = 0; i <= 8; i+=4)
            if((position[i] == '1' || position[i] == '5' || position[i] == '9') && position[i] == playerMark){
                cout << "position changed! " << mark << " placed in position [" << i << "]" << endl;
                system("pause");
                position[i] = player[indicator].getSymbol();
                return true;
            }
    else if ((position[2] == position[4]) || (position[4] == position[6]) || (position[2] == position[6]))
        for(int i = 2; i <= 6; i+=2)
            if((position[i] == '3' || position[i] == '5' || position[i] == '7') && position[i] == playerMark){
                  cout << "position changed! " << mark << " placed in position [" << i << "]" << endl;
                system("pause");
                position[i] = player[indicator].getSymbol();
                return true;
            }
        cout << "position NOT changed! " << endl;
                system("pause");
    return false;
}

void computerNormalMove(Players *player, char position[], int indicator)
{
    srand(time(0));
    int cPos = rand() % 9 + 1;//cPos = computer position. The position the computer chooses is randomly generated

    while(position[cPos-1] == 'X' || position[cPos-1] == 'O')//check to see if the position the cpu chose is occupied. If true, roll again.
        cPos = rand() % 9 + 1;
    player[indicator].setPosition(cPos);
}

void playerChoice(Players *player, int indicator)
{//if the user chooses a number other than the ones displayed on the board, force them to enter one in the range 1-9
    int pos;

    cout << player[indicator].getName() << " uses " << player[indicator].getSymbol() << "'s. choose your position: ";
	cin >> pos;
	while (pos != 1 && pos != 2 && pos != 3 && pos!= 4 && pos != 5 & pos != 6 && pos != 7 && pos != 8 && pos != 9){
        cout << "Invalid value, try again: ";
        cin >> pos;
	}
	player[indicator].setPosition(pos);
}

//function will check to see if a place on the board is occupied by another player's X or O
void checkPosition(Players *player, char position[], int indicator)
{
    //loop checks to see if there is a letter in the position the user entered. If there is, it will prompt them to
    //enter a different position. This is to ensure the players don't overwrite each other's choices!
    playerChoice(player, indicator);
    while(position[player[indicator].getPos()-1] == 'X' || position[player[indicator].getPos()-1] == 'O'){
        cout << "Position occupied. Select a different one: ";
        playerChoice(player, indicator);
    }
}

bool placeMark(Players *player, char position[], int indicator, int numPlayers)
{
    if (player[indicator].getName() == "CPU")
        computerNormalMove(player, position, indicator);
    else
        checkPosition(player, position, indicator);

	int pos = player[indicator].getPos();
	char mark = player[indicator].getSymbol();

    if(){
        if(pos == 1)
            position[0] = mark;
        else if (pos == 2)
            position[1] = mark;
        else if (pos == 3)
            position[2] = mark;
        else if (pos == 4)
            position[3] = mark;
        else if (pos == 5)
            position[4] = mark;
        else if (pos == 6)
            position[5] = mark;
        else if (pos ==7)
             position[6] = mark;
        else if (pos == 8)
            position[7] = mark;
        else
            position[8] = mark;
    }else{
        cout << "block function returned true" << endl;
        system("pause");
    }

    system("cls");
    createBoard(player, position);
    if (determineWinner(position) == 2){
        player[indicator].setWinner("WINNER");
    	for(int i = 0; i < numPlayers; i++)
            if(player[i].getWinner() == "WINNER")
                cout <<  player[i].getName() << " won the game!" << endl;
        return false;
    }else if (determineWinner(position) == 1){
        cout << "Game is a tie." << endl;
        return false;
    }else
         return true;
}

int determineWinner(char position[])
{
    if(position[0] == position[1] && position[1] == position[2])//first row win
        return 2;
    else if (position[3] == position[4] && position[4] == position[5])//second row win
        return 2;
    else if (position[6] == position[7] && position[7] == position[8])//third row win
        return 2;
    else if (position[0] == position[3] && position[3] == position[6])//first column win
        return 2;
    else if (position[1] == position[4] && position[4] == position[7])//second column win
        return 2;
    else if (position[2] == position[5] && position[5] == position[8])//third column win
        return 2;
    else if (position[0] == position[4] && position[4] == position[8])//first diagonal(top left to bottom right) win
        return 2;
    else if (position[2] == position[4] && position[4] == position[6])//second diagonal(top right to bottom left) win
        return 2;
    else if(position[0] != '1' && position[1] != '2' && position[2] != '3' && position[3] != '4' != position[4] != '5' &&
				position[5] != '6' && position[6] != '7' && position[7] != '8' && position[8] != '9')
		return 1; // game is tie

    return 0; // if the game neither won or tied, return 0 and continue;
}

void createBoard(Players *players, char position[])
{
    cout << "\t\tTic Tac Toe Game" << endl << endl;
    cout << "\t\t" << "      " << "|" << "      " << "|" << "      " << endl;
    cout << "\t\t" << "   " << position[0] <<  "  " << "|" << "   "  << position[1]  << "  " << "|"  << "  " << position[2] << "      " << endl;
    cout << "\t\t" << "______" << "|" << "______" << "|" << "______" << endl;
    cout << "\t\t" << "      " << "|" << "      " << "|" << "      " << endl;
    cout << "\t\t" << "   " << position[3] <<  "  " << "|" << "   "  << position[4]  << "  " << "|"  << "  " << position[5] << "      " << endl;
    cout << "\t\t" << "______" << "|" << "______" << "|" << "______" << endl;
    cout << "\t\t" << "      " << "|" << "      " << "|" << "      " << endl;
    cout << "\t\t" << "   " << position[6] <<  "  " << "|" << "   "  << position[7]  << "  " << "|"  << "  " << position[8] << "      " << endl;
    cout << "\t\t" << "      " << "|" << "      " << "|" << "      " << endl;
}

void resetGame(Players *player, char position[])
{
	char choice, newboard[9]{'1', '2', '3', '4', '5', '6', '7', '8', '9'};

	cout << "Play again? y = yes/ n = no: ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y'){
        system("cls");
        for(int i = 0; i < 9; i++)
        	position[i] = newboard[i];
        createBoard(player, position);
    }
    else{
    	cout << "Goodbye! Thanks for playing!" << endl;
    	exit(0);
	}
}

/*********************************************************************************************
Here are the functions that help create the menu in the beginning !
My goal here is to simulate an actual menu with a cursor, rather than having something
simple (oooor stupid) like "s = start game". It's time to step it up and have a
cursor "scroll" over different options in a menu
//**************************************************************************************/

void displayGameOptions(int yposition)
{
    gotoxy(35, 0);
    cout << "SELECT GAME MODE:";
    gotoxy(30, yposition);
    cout << "--->";
    gotoxy(40, 7);
    cout << "2 - PLAYERS";
    gotoxy(40, 8);
    cout << "VS COMPUTER - NORMAL MODE";
    gotoxy(40, 9);
    cout << "VS COMPUTER - HARD MODE";
    gotoxy(40, 10);
    cout << "BACK";
}

void startGame(Players *players, int numPlayers)
{
    int asciiCode, yposition = 7;
    char input;

    system("cls");
    while (asciiCode != 13)//13 is ascii code for the "enter" key
    {
        displayGameOptions(yposition);
        gotoxy(0, 0);
        input = getch();
        asciiCode = input;

        if(input == 'w' && yposition == 7)//if the user enters w and selection arrow is at the topmost option (start), do nothing
            continue;
        else if (input == 'w' && (yposition == 8 || yposition == 9 || yposition == 10))//allow the user to move up when on line 8 or 9
            yposition--;
        else if(input == 's' && (yposition == 7 || yposition == 8 || yposition == 9))//on line 7 or 8, allow user to move down
            yposition++;
        else if (input == 's' && yposition == 10)//if the user enters s and selection arrow is at the bottommost option (quit), do nothing
            continue;
        system("cls");
        displayGameOptions(yposition);
    }
    system("cls");
    if(yposition == 7){
        vsPlayer = true;
        twoplayerInformation(players, numPlayers);
    }else if (yposition == 8){
        vsCPUnormal = true;
        playervscomputerInformation(players, numPlayers);
    }else if (yposition == 9){
        vsCPUhard = true;
        playervscomputerInformation(players, numPlayers);
    }else
        mainMenu(players, numPlayers);
}

void displayOptions(int yposition)
{
    gotoxy(30, yposition);
    cout << "--->";
    gotoxy(40, 7);
    cout << "RULES";
    gotoxy(40, 8);
    cout << "DEVELOPER INFORMATION";
    gotoxy(40, 9);
    cout << "BACK";
}

void selectOptions(Players *players, int numPlayers)
{
    char input;
    int asciiCode, yposition = 7;

    system("cls");
     while (asciiCode != 13)//13 is ascii code for the "enter" key
    {
        displayOptions(yposition);
        gotoxy(0, 0);
        input = getch();
        asciiCode = input;

        if(input == 'w' && yposition == 7)//if the user enters w and selection arrow is at the topmost option (start), do nothing
            continue;
        else if (input == 'w' && (yposition == 8 || yposition == 9))//allow the user to move up when on line 8 or 9
            yposition--;
        else if(input == 's' && (yposition == 7 || yposition == 8))//on line 7 or 8, allow user to move down
            yposition++;
        else if (input == 's' && yposition == 9)//if the user enters s and selection arrow is at the bottommost option (quit), do nothing
            continue;
        system("cls");
        displayOptions(yposition);
    }

    system("cls");
    if(yposition == 7)
        displayRules(players, numPlayers);
    else if (yposition == 8)
        displayDeveloperInformation(players, numPlayers);
    else
        mainMenu(players, numPlayers);
}

void displayDeveloperInformation(Players *players, int numPlayers)
{
    cout << "Hey guys! my name is Darien Miller, and I am the programmer behind this project!" << endl;
    cout << "Inspired by playing another console game, and by learning about classes, I decided" << endl;
    cout << "to re - visit my old tic tac toe program, and revamp it proper menus and actual ai!" << endl;
    cout << "To simulate AI, I chose to abuse the random number generator built into C++ by having " << endl;
    cout << "the \"computer\" use them to guide its hand. This is only on normal mode however. YES, I " << endl;
    cout << "have difficulty settings! If the user chooses normal mode, the computer will place a X or O" << endl;
    cout << "in the position on the TTT board depending on what number is generated by the RNG" << endl;
    cout << "On hard mode however, the computer will try to move into the corners of the board, getting the user" << endl;
    cout << "in a two way trap. If the computer is the second player however, he will try to tie you to the " << endl;
    cout << "best of his ability! Have fun, and enjoy :)" << endl << endl;
    system("pause");
    system("cls");
    selectOptions(players, numPlayers);
}

void displayRules(Players *players, int numPlayers)
{
    gotoxy(30, 4);
    cout << "Good luck scrub LOL";
    gotoxy(30, 6);
    cout << "But seriously, here's what to expect from my game: ";
    gotoxy(0, 8);
    cout << "1. You can choose whether or not to play against another player, or the computer";
    gotoxy(0, 9);
    cout << "2. When playing against the computer, depending on the difficulty setting, it'll behave differently";
    gotoxy(0, 10);
    cout << "3. Do to the nature of a text based game, when the board is displayed, you choose you desired position via numbers";
    gotoxy(0, 11);
    system("pause");
    system("cls");
    selectOptions(players, numPlayers);
}

void mainMenu(Players *players, int numPlayers)
{
    char input;
    int asciiCode, yposition = 7;

    while (asciiCode != 13)//13 is ascii code for the "enter" key. Exit loop when user clicks the enter key!
    {
        displayMenu(yposition);
        gotoxy(0, 0);
        input = getch();//read in a character from input'
        asciiCode = input;

        if(input == 'w' && yposition == 7)//if the user enters w and selection arrow is at the topmost option (start), do nothing
            continue;
        else if (input == 'w' && (yposition == 8 || yposition == 9))//allow the user to move up when on line 8 or 9
            yposition--;
        else if(input == 's' && (yposition == 7 || yposition == 8))//on line 7 or 8, allow user to move down
            yposition++;
        else if (input == 's' && yposition == 9)//if the user enters s and selection arrow is at the bottommost option (quit), do nothing
            continue;
        system("cls");
        displayMenu(yposition);
    }

    if (yposition == 7)
        startGame(players, numPlayers);
    else if(yposition == 8)
        selectOptions(players, numPlayers);
    else{
        system("cls");
        cout << "Goodbye! Play again soon! :)" << endl;
        exit(0);
    }
}

void displayMenu(int yposition)
{
    gotoxy(10, 0);
    cout << "Welcome to Darien's Tic Tac Toe Game! Please make your selection, and GOOD LUCK";
    gotoxy(40, 1);
    cout << "W - up, S - down, ENTER to select";
    gotoxy(30, yposition);
    cout << "--->";
    gotoxy(40, 7);//"start" is on line 7 in output
    cout << "START";
    gotoxy(50, yposition);
    cout << "<---";
    gotoxy(40, 8);
    cout << "OPTIONS";//"options" is on line 8 in output
    gotoxy(40, 9);
    cout << "QUIT";//"quit" is on line 9 in output
}

void gotoxy(int x, int y)
{
    COORD coord;

    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
