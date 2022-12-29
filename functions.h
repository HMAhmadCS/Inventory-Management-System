#include<iostream>
#include<string>
#include<limits>
#include<ios>
#include<fstream>
#include<conio.h>
#include<cstdio>

using namespace std;


const int MAX_NAME_LENGTH = 20;
const int MAX_ALLOCATIONS = 15;
const int ITEM_ID_LENGTH = 13;

const char INVENTORY_DATA_FILE_ADDRESS[] = "inventory_item_data.txt";
//const char INVENTORY_DATA_FILE_ADDRESS[] = "D:\\Ahmad\\Studies\\Semester 2\\Assignments_PF\\final_term_project\\inventory_item_data.txt";
enum Department {
	CS = 1, SE, IT, DS
};

struct FacMember {
	char name[MAX_NAME_LENGTH + 1];
	Department dept;
};

struct Inventory {
	char item_ID[ITEM_ID_LENGTH]{};
	char name[MAX_NAME_LENGTH + 1]{};  // 20 + (1 for null character).
	char category[MAX_NAME_LENGTH + 1]{};
	int item_count{};
	int numberOfAllocations = 0;
	FacMember allocated_to[MAX_ALLOCATIONS]{};
	bool deleted = false;  // a flag for deleted inventory.
};



void welcome();
void menu();
void selectCommand( int & choice );

/***************************************** Functions For Requirements *******************************************************/

void addInventoryItem();
void viewAllInventoryItems();
void searchInventoryItem();
void editInventoryItem();
void deleteInventoryItem();
void assignItem();
void retrieveItem();
void showAllPersonsAllocated();

/***************************************** Input Tasks *******************************************************************/

Inventory inputInventory();
FacMember inputFacMember();

/***************************************** Helping Functions *************************************************************/

bool areEqualStr( const char str1[], const char str2[], int maxLength = MAX_NAME_LENGTH );
int strLength( const char str[] );
bool isInStr( const char target[], const char part[] );
void inputStr( char x[] );
void inputValidateInt( unsigned int & n, int upperLimit );
void inputValidateInt( int & n, int lowerLimit, int upperLimit = numeric_limits<int>::max() );
void setStr( char str1[], const char str2[], int length = MAX_NAME_LENGTH );
void clearBuffer();
void showAllInventoryNames();
void displayAllocatedPersons( Inventory inv );
void searchByName();
void searchByCategory();

string tellDept( Department dept );

/***************************************** File Tasks *******************************************************************/

bool writeInFile( const char fileAddress[], void * memoryAddress, size_t size, size_t startPoint = 0 );
bool addItemInFile( const char fileAddress[], void * memoryAddress, size_t size );
void writeInOpenedFile( fstream & file, void * memoryAddress, size_t size );
bool readFromFile( const char fileAddress[], void * memoryAddress, size_t size, size_t startPoint );
void readFromOpenedFile( fstream & file, void * memoryAddress, size_t size );
bool fileInReadMode( fstream & file, const char name[] );
bool fileInWriteMode( fstream & file, const char name[] );
bool fileInAppendMode( fstream & file, const char name[] );
size_t numOfItemsInFile( const char fileAddress[], int sizeOfItem );
size_t numOfUndeletedItemsInFile( const char fileAddress[], size_t size );
size_t lastIndexOfFile( const char fileAddress[] );
void generateID( char id[], const char fileAddress[] );
//unsigned int indexOfDeletedItemInFile( const char fileAddress[] );
unsigned int indexOfUndeletedItemInFile( unsigned int index, const char fileAddress[] );
void removeDeletedItems( const char fileAddress[], unsigned int size );

/***************************************** Output Tasks *******************************************************************/

void showInventory( Inventory inv );
