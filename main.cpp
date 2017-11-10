#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
using namespace std;

class BankUsers{
    public:
        BankUsers(){PIN = rand()% 9999 + 1000;}
        void setName(string name){this -> name = name;}
        void setAcctNum(int id){acctnum = id;}
        void setCheck(double bal){checkingsBalance = bal;}
        void setSave(double bal){savingsBalance = bal;}
        string getName(){return name;}
        int getAcctNum(){return acctnum;}
        double getcheckingsBalance(){return checkingsBalance;}
        double getsavingsBalance(){return savingsBalance;}
        int getpin(){return PIN;}
    private:
        string name;
        int acctnum;
        double checkingsBalance;
        double savingsBalance;
        int PIN;
};

BankUsers *readAccts(BankUsers *users, int &max_accts);
void printAccounts (BankUsers *users, int max_accts);
BankUsers *resizeArrays(BankUsers *users, int max_accts);
void withdrawal(BankUsers *users, int max_accts);
void deposit(BankUsers *users, int max_accts);
void viewBalance(BankUsers *users, int max_accts);
void menu();
void sortAccounts(BankUsers *users, int max_accts);
void makeNewAccount(BankUsers *users, int max_accts);
void deleteAccount(BankUsers *users, int max_accts);
int findAccount(BankUsers *users, int max_accts);
void mainMenu(BankUsers *users, int max_accts);
void gotoxy(int x, int y);
void displayMenu(int yvalue);
void displayRules(BankUsers *users, int max_accts);
void mainMenu(BankUsers *users, int max_accts);

int main()
{
    int max_accts = 0;
    srand(time(0));//set random number generator for constructor function, which will assign a random 4 digit PIN to each object
    BankUsers *users = new BankUsers[max_accts+1];//initialize dynamic array to 1, which will be expanded when accounts are read in

    users = readAccts(users, max_accts);
    mainMenu(users, max_accts);

    //delete [] users; doing this crashes the program! How come?
    return 0;
}

BankUsers *readAccts(BankUsers *users, int &maxaccts)
{
    ifstream infile("classbank.txt");
    string name;
    int id, PIN;
    double cbal, sbal;

    infile >> name >> id >> cbal >> sbal;
    while(infile){
        users[maxaccts].setName(name), users[maxaccts].setAcctNum(id), users[maxaccts].setCheck(cbal), users[maxaccts].setSave(sbal);
        maxaccts++;
        users = resizeArrays(users, maxaccts);
        infile >> name >> id >> cbal >> sbal;
    }

     return users;
}

BankUsers *resizeArrays(BankUsers *users, int max_accts)
{
    BankUsers *temp = new BankUsers[max_accts+1];

    for(int i = 0; i < max_accts; i++)
        temp[i] = users[i];

    delete [] users;
    users = temp;

    return users;
}

void printAccounts(BankUsers *users, int max_accts)
{
    system("cls");
    cout << "\t\t\tACCOUNT HOLDER LIST" << endl;
    cout << "****************************************************************************" << endl;
    cout << "*NAME\t\t\tACCT NO.\t\tCHECKINGS $\t\tSAVINGS $"<< endl;
    cout << "****************************************************************************" << endl;
    for(int i = 0; i < max_accts; i++){
        cout << users[i].getName();
        cout << "\t\t\t" << users[i].getAcctNum();
        cout << "\t\t\t" << users[i].getsavingsBalance();
        cout << "\t\t\t" << users[i].getcheckingsBalance() << endl;
    }

}

void withdrawal(BankUsers *users, int max_accts)
{

}

void deposit(BankUsers *users, int max_accts)
{

}

void makeNewAccount(BankUsers *users, int max_accts)
{

}

void deleteAccount(BankUsers *users, int max_accts)
{

}

void viewBalance(BankUsers *users, int max_accts)
{

}

void mainMenu(BankUsers *users, int max_accts)
{
    char input;
    int yposition = 4;

    while (input != char(13))//13 is ascii code for the "enter" key. Exit loop when user clicks the enter key!
    {
        displayMenu(yposition);
        gotoxy(35, yposition);
        cout << "->";
        gotoxy(55, yposition);
        cout << "<-";
        gotoxy(0, 0);
        input = getch();//read in a character from input

        if(input == 'w' && yposition == 4)//if the user enters w and selection arrow is at the topmost option (withdrawal), do nothing
            continue;
        //allow the user to move up when on any line other than the top most one
        else if (input == 'w' && (yposition == 5 || yposition == 6 || yposition == 7 || yposition == 8 || yposition == 9 || yposition == 10))
            yposition--;
        //allow user to move down when on any line other than the bottom most one
        else if(input == 's' && (yposition == 4 || yposition == 5 || yposition == 6 || yposition == 7 || yposition == 8 || yposition == 9))
            yposition++;
        else if (input == 's' && yposition == 10)//if the user enters s and selection arrow is at the bottommost option (quit), do nothing
            continue;
        system("cls");
    }

    if (yposition == 4)
        withdrawal(users, max_accts);
    else if(yposition == 5)
        deposit(users, max_accts);
    else if(yposition == 6)
        viewBalance(users, max_accts);
    else if(yposition == 7)
        printAccounts(users, max_accts);
    else if(yposition == 8)
        makeNewAccount(users, max_accts);
    else if(yposition == 9)
        deleteAccount(users, max_accts);
    else{
        cout << "Goodbye! have a good day! :)" << endl;
        exit(0);
    }
}

void displayMenu(int yposition)
{
    gotoxy(15, 0);
    cout << "Welcome to the World Virtual Bank! Please make your selection: ";
    gotoxy(40, 1);
    cout << "W - up, S - down, ENTER to select";
    gotoxy(40, 4);//"withdrawal" is on line 4 in output
    cout << "WITHDRAWAL";
    gotoxy(40, 5);
    cout << "DEPOSIT";//"deposit" is on line 5 in output
    gotoxy(40, 6);
    cout << "VIEW BALANCE";//"quit" is on line 9 in output
    gotoxy(40, 7);
    cout << "VIEW ACCOUNTS";
    gotoxy(40, 8);
    cout << "NEW ACCOUNT";
    gotoxy(40, 9);
    cout << "DELETE ACCOUNT";
    gotoxy(40, 10);
    cout << "QUIT";
}

void gotoxy(int x, int y)
{
    COORD coord;

    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
