#include <iostream>
#include<string>
#include<limits>
#include<ios>
#include<fstream>
#include <conio.h>

using namespace std;


const int MAX_NAME_LENGTH = 20; 
const int MAX_ALLOCATIONS = 15;
const int ITEM_ID_LENGTH = 11;

const string INVENTORY_DATA_FILE_ADDRESS = "inventory_item_data.txt";
//const string INVENTORY_DATA_FILE_ADDRESS = "D:\\Ahmad\\Studies\\Semester 2\\Assignments_PF\\final_term_project\\inventory_item_data.txt";

struct Inventory {
	char item_ID[ITEM_ID_LENGTH];
	char name[MAX_NAME_LENGTH + 1];  // 20 + (1 for null character).
	char category[MAX_NAME_LENGTH + 1];
	int item_count;
	int numberOfAlocations;
	char allocated_to[MAX_NAME_LENGTH + 1][MAX_ALLOCATIONS];
	bool deleted = false;  // a flag for delted inventory.
};



void welcome();


/***************************************** Functions For Requirements *******************************************************/

/*
Edit inventory item( Including item count )
Delete inventory item
Assign inventory item
Retrieve inventory item
View the list of faculty members who have borrowed a specific item.
*/

bool addInventoryItem();
void viewAllInventoryItems();
void searchInventoryItem();
void editInventoryItem();
void deleteInventoryItem();


/***************************************** Input Tasks *******************************************************************/

Inventory inputInventory();

/***************************************** Helping Functions *************************************************************/

bool equalCstr( char str1[], char str2[], int maxLength );
void inputStr( char x[] );
void setStr( char str1[], char str2[], int length = MAX_NAME_LENGTH );
void clearBuffer();
void showAllInventoryNames();

/***************************************** File Tasks *******************************************************************/

bool writeInFile( string fileAddress, char * memoryAddress, size_t size, size_t startPoint = 0 );
bool readFromFile( string fileAddress, char * memoryAddress, size_t size, size_t startPoint );
bool fileInReadMode( fstream & file, string name );
bool fileInWriteMode( fstream & file, string name );
size_t numOfItemsInFile( string fileAddress, int sizeOfItem );
size_t lastIndexOfFile( string fileAddress );
void generateID(char id[]);
unsigned int indexOfDeletedItemInFile(string fileAddress);

/***************************************** Output Tasks *******************************************************************/



void showInventory( Inventory inv );
