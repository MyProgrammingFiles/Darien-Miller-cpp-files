#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <time.h>
using namespace std;

class bankdata{
	public:
		string first;
		string last;
		int acctnum;
		double check;
		double save;
		int pin;
};

void readdata(bankdata[], int &);
int findacct (bankdata[], int, int);
void deposit(bankdata[], int);
void withdrawal (bankdata[], int);
void printaccounts (bankdata[], int);
void sortaccounts(bankdata[], int);
void viewbalance(bankdata[], int);
void deleteaccount(bankdata[], int &);
void menu();
void viewpin(bankdata[], int);
void newaccount(bankdata[], int &);

int main()
{
	int numaccts = 0;
	char select;
	bankdata accounts[50];
	
	readdata(accounts, numaccts);
	printaccounts(accounts, numaccts);
	
	while(select != 'q' && select !='Q')
	{
		menu();
		cout << "Make your selection: ";
		cin >> select;
		
		switch(select){
			case 'd':
			case 'D':
				deposit(accounts, numaccts);
				break;
			case 'w':
			case 'W':
				withdrawal(accounts, numaccts);
				break;
			case 'n':
			case 'N':
				newaccount(accounts, numaccts);
				break;
			case 'p':
			case 'P':
				printaccounts(accounts, numaccts);
				break;
			case 'V':
			case 'v':
				viewpin(accounts, numaccts);
				break;
			case 'X':
			case 'x':
				deleteaccount(accounts, numaccts);
				break;
			case 'B':
			case 'b':
				viewbalance(accounts, numaccts);
				break;
			case 'q':
			case 'Q':
				break;
			case 's':
			case 'S':
				sortaccounts(accounts, numaccts);
				break;
			default:
				cout << endl << "'"<< select <<"'" << " is not a  valid option. Try again." << endl << endl;
			
		}
	}
	
	cout << "Goodbye! Have a nice day!";
	return 0;
}


void readdata(bankdata accounts[], int &n)
{
	ifstream infile ("bankinfo.txt");
	
	infile >> accounts[n].first >> accounts[n].last >> accounts[n].acctnum >> accounts[n].check >> accounts[n].save >> accounts[n].pin;
	while(infile){
		n++;
		infile >> accounts[n].first >> accounts[n].last >> accounts[n].acctnum >> accounts[n].check >> accounts[n].save >> accounts[n].pin;
	}
	
	return;
}

void printaccounts (bankdata accounts[], int n)
{
	cout << "\t\t\t\t\tACCOUNT HOLDER LIST" << endl << endl;
	cout << "=============================================================================================" << endl;
	cout << "Name\t\t\taccount#\t  checkings acct\tsavings acct\t\tPIN" << endl;
	cout << "=============================================================================================" << endl;
	for(int i = 0; i < n; i++)
	{
		cout << accounts[i].first + " " + accounts[i].last;
		cout << "      \t" << accounts[i].acctnum;
		cout << "   \t          $"<<accounts[i].check;
		cout << "            \t$" << accounts[i].save;
		cout << "       \t\t" << accounts[i].pin << endl << endl;
	}
	
	return;
}

void viewpin(bankdata accounts[], int numaccts)
{
	int acct, pos, choice;
	
	if (numaccts != 0)
	{	
		cout << "Enter account number: ";
		cin >> acct;
		
		pos = findacct(accounts, numaccts, acct);
		
		while(pos < 0 || choice != 1)
		{
			cout << "Account " << acct << " was not found. Try again." << endl;
			cout << "Enter account number, or 1 to quit: ";
			cin >> acct;
		}
	
		cout << "Hello " << accounts[pos].first + " " + accounts[pos].last << "! Your PIN is: " << accounts[pos].pin << endl << endl;
	}
	else
		cout << endl << "There are no accounts left to view." << endl << endl;

	return;
}

void menu()
{
	cout << "D - Deposit" << endl;
	cout << "W - Withdrawal" << endl;
	cout << "P - Print Accounts" << endl;
	cout << "N - New account" << endl;
	cout << "B - Display balance" << endl;
	cout << "V - View pin" << endl;
	cout << "X - Delete account" << endl;
	cout << "S - Sort accounts in alphabetical order" << endl;
	cout << "Q - Quit" << endl;
}

void viewbalance(bankdata accounts[], int numaccts)
{
	int acct, pos, pin;
	char choice;
	
	if (numaccts != 0)
	{
		cout << "Enter account number: ";
		cin >> acct;
		
		pos = findacct(accounts, numaccts, acct);
		
		while(pos < 0)
		{
			cout << "Account " << acct << " was not found. Try again." << endl;
			cout << "Enter account number: ";
			cin >> acct;
		}
		
		cout << "Enter PIN: ";
		cin >> pin;
		
		while(pin != accounts[pos].pin)
		{
			cout << pin << " is incorrect. Try again: ";
			cin >> pin;
		}
		
		cout << "Good day " << accounts[pos].first + " " + accounts[pos].last << "!"<< endl;
		cout << "Will you be viewing your checkings or savings balance?" << endl;
		cout << "c - checkings,  s - savings: ";
		cin >> choice;
		
		while(choice != 'c'&& choice !='C' && choice != 's'&& choice !='S')
		{
			cout << "'"<< choice << "'" << " is not a valid option. Try again." << endl;
			cout << "c - checkings,  s - savings: ";
			cin >> choice;
		}
		
		if (choice == 'c'|| choice =='C')
			cout << "Checkings balance is $" << accounts[pos].check << endl << endl;
		else if (choice == 's'|| choice =='S')
			cout << "Savings balance is $" << accounts[pos].save << endl << endl;
	}
	else
		cout << "No accounts left to view" << endl;
	
	return;
}

void deleteaccount (bankdata accounts[], int &numaccts)
{
	int acct, pos, pin;
	
	if (numaccts != 0)
	{
		cout << "Enter the account number: ";
		cin >> acct;
		
		pos = findacct(accounts, numaccts, acct);
		
		while (pos < 0)
		{
			cout << "Account " << acct << " does not exist. Try again: ";
			cin >> acct;
			pos = findacct(accounts, numaccts, acct);
		}	
		
		numaccts--;
		for(int i = pos; i < numaccts; i++)
		{
			accounts[i].first = accounts[i+1].first;
			accounts[i].last = accounts[i+1].last;
			accounts[i].acctnum = accounts[i+1].acctnum;
			accounts[i].check = accounts[i+1].check;
			accounts[i].save = accounts[i+1].save;
			accounts[i].pin = accounts[i+1].pin;
		}
		
		cout << endl << "Account deleted!" << endl << endl;
	}
	else
		cout << "No accounts left to delete" << endl;
	
	return;
}

void newaccount(bankdata accounts[], int &numaccts)
{
	int acct, pos, pin;
	
	srand (time(NULL));
	pin = rand() % 9999 + 1000;
	
	cout << "Enter the new account number: ";
	cin >> acct;
	
	pos = findacct(accounts, numaccts, acct);
	
	while (pos >= 0)
	{
		cout << "Error! Account " << acct << " already exist! Try again: ";
		cin >> acct;
		pos = findacct(accounts, numaccts, acct);
	}
		
	
	while (acct < 1000 || acct > 9999)
	{
		cout << acct << " is not valid. Please enter a four digit number: ";
		cin >> acct;
		pos = findacct(accounts, numaccts, acct);
		while(pos >= 0)
		{
			cout << endl << "Account " << acct << " already exists! Try again. " << endl;
			cout << "Enter the new account number: ";
			cin >> acct;
			pos = findacct(accounts, numaccts, acct);
		}	
	}
	
	numaccts++;
	accounts[numaccts-1].check = 0;
	accounts[numaccts-1].save = 0;
	cout << "What is your first and last name?" << endl;
	cout << "First name: ";
	cin >> accounts[numaccts-1].first;
	cout << "Last name: ";
	cin >> accounts[numaccts-1].last;
	accounts[numaccts-1].acctnum = acct;
	cout << endl << "Your pin is " << pin << endl;
	accounts[numaccts-1].pin = pin;
	cout << "There are now " << numaccts << " accounts" << endl;
	
	return;
}

void deposit (bankdata accounts[], int numaccts)
{
	int acct, pos, pin;
	double amt;
	char choice;
	
	if (numaccts != 0)
	{
		cout << "Enter your account number: " << endl;
		cin >> acct;
		
		pos = findacct(accounts, numaccts, acct);
		
		while (pos < 0)
		{
			cout << "Account " << acct << " does not exist! Try again: ";
			cin >> acct;
			pos = findacct(accounts, numaccts, acct);
		}
		
		cout << "Enter PIN: ";
		cin >> pin;
		
		while(pin != accounts[pos].pin)
		{
			cout << pin << " is incorrect. Try again: ";
			cin >> pin;
		}
		
		cout << "Will you be depositing into your checkings, or savings account?" << endl;
		cout << "c - Checkings, s - Savings: ";
		cin >> choice;
		
		while (choice != 'c' && choice != 'C' && choice != 's' && choice != 'S')
		{
			cout << "'"<< choice << "' is not a valid choice. Try again." << endl;
			cout << "c - Checkings, s - Savings: ";
			cin >> choice;
		}
		
		if (choice == 'c' || choice == 'C')
		{
			cout << "Enter deposit amount: $";
			cin >> amt;
			while (amt < 0)
			{
				cout << "Please enter a non negative amount. $";
				cin >> amt;
			}
			
			accounts[pos].check += amt;
			cout << "You deposited $" << amt << ". Your new checkings balance is $" << accounts[pos].check << endl << endl;
		}
		
		else if (choice == 's' || choice == 'S')
		{
			cout << "Enter deposit amount: $";
			cin >> amt;
			
			while (amt < 0)
			{
				cout << "Please enter a non negative amount. $";
				cin >> amt;
			}
			
			accounts[pos].save += amt;
			cout << "You deposited $" << amt << ". Your new savings balance is $" << accounts[pos].save << endl << endl;
		}
	}
	else
		cout << endl << "There are no accounts to deposit money into." << endl << endl;	
	 	
	
	return;
}

void withdrawal (bankdata accounts[], int numaccts)
{
	int acct, pos, pin;
	double amt, diff;
	char choice;
	
	if (numaccts != 0)
	{	
		cout << "Enter account number: ";
		cin >> acct;
		
		pos = findacct(accounts, numaccts, acct);
		
		while (pos < 0)
		{
			cout << "Account " << acct << " does not exist. Try again: ";
			cin >> acct;
			pos = findacct(accounts, numaccts, acct);
		}
	
		cout << "Enter PIN: ";
		cin >> pin;
		
		while(pin != accounts[pos].pin)
		{
			cout << pin << " is incorrect. Try again: ";
			cin >> pin;
		}
		
		cout << "Will you be withdrawing from your Checkings, or savings account?" << endl;
		cout << "c - Checkings, s - Savings: ";
		cin >> choice;
		
		while(choice != 'c' && choice != 'C' && choice != 's' && choice != 'S')
		{
			cout << "'"<< choice << "' is not a valid option. Try again"  << endl;
			cout << "c - Checkings, s - Savings: ";
			cin >> choice;			
		}
		
		if (choice == 'c' || choice == 'C')
		{
			cout << "Enter withdrawal amount: $";
			cin >> amt;
		
			while (amt > accounts[pos].check)
			{
				diff = amt - accounts[pos].check;
				cout << "Withdrawal amount will cause an overdraft of $" << diff << ". Enter a different amount! $";
				cin >> amt;
			}
		
			while (amt < 0)
			{
				cout << "Please enter a non negative amount: $";
				cin >> amt;
			}
				
			accounts[pos].check -= amt;
			cout << "Withdrawal successful. Your new checking account balance is $" << accounts[pos].check << endl << endl;
		}
		
		else if (choice == 's' || choice == 'S')
		{
			cout << "Enter withdrawal amount: $";
			cin >> amt;
			
			while (amt > accounts[pos].save)
			{
				diff = amt - accounts[pos].save;
				cout << "Withdrawal amount will cause an overdraft of $" << diff << ". Enter a different amount!: $";
				cin >> amt;
			}
			while (amt < 0)
			{
				cout << "Please enter a non negative amount: $";
				cin >> amt;
			}
			
			accounts[pos].save -= amt;
			cout << "Withdrawal successful. Your new savings account balance is $" << accounts[pos].save << endl;
		}
	}
	else
		cout << endl << "There are no accounts to withdraw money from." << endl << endl;
		
	return;
}


void sortaccounts(bankdata accounts[], int numaccts)
{
	bankdata temp;
	bool swap;
	
	if (numaccts != 0)
	{	
		do{
			swap = false;
			for(int i =0; i < numaccts - 1; i++)
				if (accounts[i].first > accounts[i+1].first)
				{
					temp = accounts[i];
					accounts[i] = accounts[i+1];
					accounts[i+1] = temp;
					swap = true;
				}
		}while(swap);
		cout << "Accounts are sorted!" << endl << endl;
	}
	else
		cout << endl << "There are no accounts left to sort." << endl << endl;
		
	return;
}

int findacct(bankdata accounts[], int numaccts, int val)
{
	for(int i = 0; i < numaccts; i++)
		if (val == accounts[i].acctnum)
			return i;
	return -1;
}
