#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct classGrowths{
	string name;
	int stats[8];
};

struct childGrowths{
	string name;
	string parent;
	int initialGrowths[8];//child unit's default growth rates
	double finalGrowths[8];//child unit's growth rates after parental inheritance 
	double finalGrowthswithfather[8];//child unit's growth rates after parental +inheritance 
	int sumOfgrowths = 0;
	int sumOfgrowthswithfather = 0;
};

struct fatherGrowths{
	string name;
	int stats[8];
};

struct motherGrowths{
	string name;
	int stats[8];
};

motherGrowths *readMominfo(int &numMothers, ifstream &infile);
fatherGrowths *readDadinfo(int &numFathers, ifstream &input);
classGrowths *readClassinfo(int &numClasses, ifstream &fin);
childGrowths *readChildinfo(int &numChildren, ifstream &myfile);
void printMominfo(motherGrowths *mom, int numMothers);
void printDadinfo(fatherGrowths *dad, int numFathers);
void printClassinfo(classGrowths *Class, int numClasses);
void printChildinfo(childGrowths *child, int numChildren);
void setChildGrowths(childGrowths *child, fatherGrowths *dad, motherGrowths *mom, classGrowths *Class,int numFathers, int cchoice, int mchoice, int clChoice);
int findFatherPosition(childGrowths *child, fatherGrowths *dad, int numFathers);
void printChildGrowthRates(childGrowths *child, int childChoice);

int main ()
{
	int motherChoice, childChoice, classChoice, numMothers = 0, numFathers = 0, numClasses = 0, numChildren = 0;
	int boon, bane;
	string growthName[8] {"HP", "Attack", "Magic", "Skill", "Speed", "Luck", "Defense", "Resistance"};
	ifstream infile("momgrowths.txt");//read the growths of the mother into the array of mothers from this text file
	ifstream input("dadgrowths.txt");//read the growths of the father into the array of father from this text file
	ifstream fin("ClassGrowth.txt");//read the growths of the classes into the array of classes (NOT c++ classes, fire emblem classes!)
	ifstream myfile("childgrowths.txt");//read the growths of the child into the array of children

	motherGrowths *mom = readMominfo(numMothers, infile);
	fatherGrowths *dad = readDadinfo(numFathers, input);
	classGrowths *Class = readClassinfo(numClasses, fin);
	childGrowths *child = readChildinfo(numChildren, myfile);

	printMominfo(mom, numMothers);
	cout << "Choose the mother: ";
	cin >> motherChoice;
	if(motherChoice == 1){
		for(int i = 0; i < 8; i++)
			cout << i+1 << " = " << growthName[i] << " ";
		cout << "What is your avatar's boon?: ";
		cin >> boon;
		cout << "What is your avatar's bane?: ";
		cin >> bane;
	}
	printChildinfo(child, numChildren);
	cout << "Choose the child: ";
	cin >> childChoice;
	printClassinfo(Class, numClasses);
	cout << "Choose the class the child belongs to: ";
	cin >> classChoice;
	setChildGrowths(child, dad, mom, Class, numFathers, childChoice, motherChoice, classChoice);
	printChildGrowthRates(child, childChoice);
	 
	delete [] mom;
	delete [] dad;
	delete [] Class;
	delete [] child;
	
	return 0;
}

motherGrowths *readMominfo(int &numMothers, ifstream &infile)
{
	infile >> numMothers;
	motherGrowths *mom = new motherGrowths[numMothers];
	
	for(int k = 0; k < numMothers; k++){
		infile >> mom[k].name;
		for(int i = 0; i < 8; i++)
			infile >> mom[k].stats[i];	
	}
	
	return mom;//return the memory address the pointer "mom" is pointing to
}	

fatherGrowths *readDadinfo(int &numFathers, ifstream &input)
{
	input >> numFathers;
	fatherGrowths *dad = new fatherGrowths[numFathers];
	
	for(int k = 0; k < numFathers; k++){
		input >> dad[k].name;
		for(int i = 0; i < 8; i++)
			input >> dad[k].stats[i];	
	}
	
	return dad;//return the memory address the pointer "dad" is pointing to
}

classGrowths *readClassinfo(int &numClasses, ifstream &fin)
{
	fin >> numClasses;	
	classGrowths *Class = new classGrowths[numClasses];
	
	for(int k = 0; k < numClasses; k++){
		fin >> Class[k].name;
		for(int i = 0; i < 8; i++)
			fin >> Class[k].stats[i];	
	}
	
	return Class;//return the memory address the pointer "Class" is pointing to
}

childGrowths *readChildinfo(int &numChildren, ifstream &myfile)
{
	myfile >> numChildren;
	childGrowths *child = new childGrowths[numChildren];
	
	for(int k = 0; k < numChildren; k++){
		myfile >> child[k].name;
		for(int i = 0; i < 8; i++)
			myfile >> child[k].initialGrowths[i];
		myfile >> child[k].parent;	
	}
	
	return child;//return the memory address the pointer "child" is pointing to
}

void printMominfo(motherGrowths *mom, int numMothers)
{
	cout << "Mothers: " << endl;
	for(int i = 0; i < numMothers-1; i++)
		cout << i+1 << ") " << mom[i].name << "\t \t" << i+2 << ") " << mom[i+1].name << endl;
	cout << endl << endl;
}

void printClassinfo(classGrowths *Class, int numClasses)
{
	cout << "classes: " << endl;
	for(int i = 0; i < numClasses-1; i++){
		cout << i+1 << ") " << Class[i].name << "\t \t" << i+2 << ") " << Class[i+1].name << endl;
	}
	cout << endl << endl;
}

void printChildinfo(childGrowths *child, int numChildren)
{
	cout << "children: " << endl;
	for(int i = 0; i < numChildren-1; i++)
		cout << i+1 << ") " << child[i].name << "\t \t" << i+2 << ") " << child[i+1].name << endl;
		cout << endl << endl;
}

//in the childgrowths.txt file, I read in a father along with a child and their stats. This function will return the position in the
//array of father names where the name of the child's father is found
int findFatherPosition(childGrowths *child, fatherGrowths *dad, int numFathers)
{
	for(int i = 0; i < numFathers; i++)
		if(child[i].parent == dad[i].name || child[i].parent == "Azura")
			return i;
}

/*calculates the child's final growth rates after parental inheritance by using this formula: 
new child growth % = (child growth % + mother growth %)/2 + class growth %
"cchoice" is the variable holding the child of choice the user chose in main. "mchoice" is the choice of mom
, and "clChoice" is the choice of class the child belongs to */
void setChildGrowths(childGrowths *child, fatherGrowths *dad, motherGrowths *mom, classGrowths *Class, int numFathers, int cchoice, int mchoice, int clChoice)
{
	int dadpos= findFatherPosition(child, dad, numFathers);
	
	for(int i = 0; i < 8; i++){
		//for the member "finalgrowths", average out the mothers and childs growths in position i, and add that to class growth in position i
		child[cchoice-1].finalGrowths[i] = Class[clChoice-1].stats[i] + (mom[mchoice-1].stats[i] + child[cchoice-1].initialGrowths[i])/2.0;
		//do the same here, but average out the growths of the mother, father AND child, then add it to class growths
		child[cchoice-1].finalGrowthswithfather[i] = Class[clChoice-1].stats[i] +(mom[mchoice-1].stats[i] 
			+ child[cchoice-1].initialGrowths[i]+ dad[dadpos-1].stats[i])/3.0;
		child[cchoice-1].sumOfgrowths += child[cchoice-1].finalGrowths[i];
		child[cchoice-1].sumOfgrowthswithfather += child[cchoice-1].finalGrowthswithfather[i];
	}
		//child[cchoice-1].finalGrowths[i] =
}

void printChildGrowthRates(childGrowths *child, int childChoice)
{
	string growthName[8] {"HP", "Attack", "Magic", "Skill", "Speed", "Luck", "Defense", "Resistance"};
	
	cout << "Here are " << child[childChoice-1].name << "'s final growth rates without father" << endl;
	for(int i = 0; i < 8; i++)
		cout << growthName[i] << ": " << child[childChoice-1].finalGrowths[i] <<"%"<< endl;
	cout << "There growth total is: " << child[childChoice-1].sumOfgrowths << endl << endl;
	
	cout << "Here are " << child[childChoice-1].name << "'s final growth rates WITH father, " << child[childChoice-1].parent << endl;
	for(int k = 0; k < 8; k++)
		cout << growthName[k]  << ": " << child[childChoice-1].finalGrowthswithfather[k]  << "%"<< endl;
	cout << "There growth total is: " << child[childChoice-1].sumOfgrowths  << endl << endl;
	
		
}
