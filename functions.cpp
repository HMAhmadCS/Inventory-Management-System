#include "functions.h"

void welcome() {
	cout << endl;
	cout << "\t***************************************************************************************************" << endl;
	cout << "\t*                                                                                                 *" << endl;
	cout << "\t*                                                                                                 *" << endl;
	cout << "\t*                              Final Term Project                                                 *" << endl;
	cout << "\t*                                                                                                 *" << endl;
	cout << "\t*                          Inventory Management System                                            *" << endl;
	cout << "\t*                                                                                                 *" << endl;
	cout << "\t*                       BCSF21M502 - Hafiz Muhammad Ahmad                                         *" << endl;
	cout << "\t*                                                                                                 *" << endl;
	cout << "\t*                                                                                                 *" << endl;
	cout << "\t***************************************************************************************************" << endl;
}

void menu() {
	cout << "What do you want to do?\n";
	cout << "\t1 --> Add Inventory Item\n";
	cout << "\t2 --> View All Inventory Items\n";
	cout << "\t3 --> Search Inventory Item\n";
	cout << "\t4 --> Edit Inventory Item\n";
	cout << "\t5 --> Delete Inventory Item\n";
	cout << "\t6 --> Assign Inventory Item\n";
	cout << "\t7 --> Retrieve Inventory Item\n";
	cout << "\t8 --> View assigned members for an item\n";
	cout << "\t9 --> Exit the program\n";

	cout << "\nEnter number of your required command: ";
}


void selectCommand( int & choice ) {
	switch (choice) {
	case 1:
		addInventoryItem();
		break;

	case 2:
		viewAllInventoryItems();
		break;

	case 3:
		searchInventoryItem();
		break;

	case 4:
		editInventoryItem();
		break;

	case 5:
		deleteInventoryItem();
		break;

	case 6:
		assignItem();
		break;

	case 7:
		retrieveItem();
		break;

	case 8:
		showAllPersonsAllocated();
		break;

	case 9:
		cout << "Do ypu really want to exit?.....(y for yes and another key for no.....) \n\n";
		if (_getch() != 'y') choice = 0;

	}

}


/***************************************** Functions For Requirements *******************************************************/

void addInventoryItem() {
	Inventory newInv = inputInventory();
	generateID( newInv.item_ID );
	if (
		writeInFile(
			INVENTORY_DATA_FILE_ADDRESS, &newInv, sizeof( Inventory ),
			indexOfDeletedItemInFile( INVENTORY_DATA_FILE_ADDRESS ) * sizeof( Inventory )
		)) {
		cout << "Inventory Added Successfully...\n\n";

	}
	else {
		cout << "Failed....\t Inventory couldn't be added....\n\n";
	}
}


void viewAllInventoryItems() {
	size_t n = numOfItemsInFile( INVENTORY_DATA_FILE_ADDRESS, sizeof( Inventory ) );
	Inventory invShown;
	for (int i = 0; i < n; i++) {
		readFromFile( INVENTORY_DATA_FILE_ADDRESS, &invShown, sizeof( Inventory ), i * sizeof( Inventory ) );
		//if (invShown.deleted) continue;
		showInventory( invShown );
		cout << "\n\t\t*******************************\n\n";
	}
}


void searchInventoryItem() {
	int searchBy;
	cout << "Search by: \t1 --> Name, \t2 --> Category";
	inputValidateInt( searchBy, 1, 2 );

	switch (searchBy) {
	case 1: searchByName(); break;
	case 2: searchByCategory(); break;
	}

}


void editInventoryItem() {
	unsigned int invIndexToChange;
	Inventory invToChange;
	cout << "Which item do you want to change (Enter the number of item): \n";
	showAllInventoryNames();

	inputValidateInt( invIndexToChange, numOfUndeletedItemsInFile( INVENTORY_DATA_FILE_ADDRESS, sizeof( Inventory ) ) );
	invIndexToChange = indexOfUndeletedItemInFile( invIndexToChange, INVENTORY_DATA_FILE_ADDRESS );

	readFromFile( INVENTORY_DATA_FILE_ADDRESS, &invToChange, sizeof( Inventory ), invIndexToChange * sizeof( Inventory ) );
	cout << "\nPrevious Details:\n";
	showInventory( invToChange );
	cout << endl;
	char retake;

	do {
		cout << "What do you want to change:\n";
		cout << "1 --> Name\n";
		cout << "2 --> Category\n";
		cout << "3 --> Count\n";
		int choice;
		inputValidateInt( choice, 1, 3 );
		clearBuffer();
		char toEdit[MAX_NAME_LENGTH];

		switch (choice) {
		case 1:
			cout << "Enter Name: ";
			inputStr( toEdit );
			setStr( invToChange.name, toEdit );
			break;
		case 2:
			cout << "Enter Category: ";
			inputStr( toEdit );
			setStr( invToChange.category, toEdit );
			break;
		case 3:
			cout << "Enter count: ";
			inputValidateInt( invToChange.item_count, 1 );
			clearBuffer();
			break;
		default:
			cout << "Wrong Input.... aborted...\n";
		}
		cout << "Press y to change another thing and any other key to go back.";
		retake = _getch();
	} while (retake == 'y');

	writeInFile( INVENTORY_DATA_FILE_ADDRESS, &invToChange, sizeof( Inventory ), invIndexToChange * sizeof( Inventory ) );

	cout << "\nUpdated Details:\n";
	readFromFile( INVENTORY_DATA_FILE_ADDRESS, &invToChange, sizeof( Inventory ), invIndexToChange * sizeof( Inventory ) );
	showInventory( invToChange );

}


void deleteInventoryItem() {
	cout << "Which Inventory you want to delete: \n";
	showAllInventoryNames();
	unsigned int invToDelIndex;
	Inventory invToDel;

	inputValidateInt( invToDelIndex, numOfUndeletedItemsInFile( INVENTORY_DATA_FILE_ADDRESS, sizeof( Inventory ) ) );

	invToDelIndex = indexOfUndeletedItemInFile( invToDelIndex, INVENTORY_DATA_FILE_ADDRESS );

	cout << "Do you really want to delete this item ? (y / n)";

	if (_getche() == 'y') {
		fstream file;
		readFromFile( INVENTORY_DATA_FILE_ADDRESS, &invToDel, sizeof( Inventory ), invToDelIndex * sizeof( Inventory ) );
		invToDel.deleted = true;
		writeInFile( INVENTORY_DATA_FILE_ADDRESS, &invToDel, sizeof( Inventory ), invToDelIndex * sizeof( Inventory ) );
		cout << "Deleted....\n\n";
	}
	else {
		return;
	}

}

void assignItem() {
	Inventory toAssign;
	unsigned int n;

	showAllInventoryNames();
	cout << "\nWhich Item to assign: ";

	inputValidateInt( n, numOfUndeletedItemsInFile( INVENTORY_DATA_FILE_ADDRESS, sizeof( Inventory ) ) );
	n = indexOfUndeletedItemInFile( n, INVENTORY_DATA_FILE_ADDRESS );

	readFromFile( INVENTORY_DATA_FILE_ADDRESS, &toAssign, sizeof( Inventory ), n * sizeof( Inventory ) );

	if (toAssign.item_count < 1) {
		cout << toAssign.name << " is not available yet. Visit later....\n\n";
		return;
	}
	cout << "Enter name of the person, whom to assign: ";

	toAssign.allocated_to[toAssign.numberOfAllocations] = inputFacMember();
	toAssign.numberOfAllocations++;
	toAssign.item_count--;

	if (
		writeInFile(
			INVENTORY_DATA_FILE_ADDRESS, &toAssign,
			sizeof( Inventory ), n * sizeof( Inventory ) )
		) {
		cout << "The item is assigned to Mr. " 
			<< toAssign.allocated_to[toAssign.numberOfAllocations - 1].name;
	};
}

void retrieveItem() {
	showAllInventoryNames();
	unsigned int n, x;
	Inventory toRetrieve;
	cout << "Which Item to retrieve: ";
	inputValidateInt( n, numOfUndeletedItemsInFile( INVENTORY_DATA_FILE_ADDRESS, sizeof( Inventory ) ) );
	n = indexOfUndeletedItemInFile( n, INVENTORY_DATA_FILE_ADDRESS );

	readFromFile( INVENTORY_DATA_FILE_ADDRESS, &toRetrieve, sizeof( Inventory ), n * sizeof( Inventory ) );

	if (toRetrieve.numberOfAllocations < 1) {
		cout << toRetrieve.name << " is not assigned to anyone.....\n\n";
		return;
	}
	
	cout << "Which Person to retrieve form: ";
	displayAllocatedPersons( toRetrieve );
	inputValidateInt( x, toRetrieve.numberOfAllocations );

	toRetrieve.allocated_to[x - 1] = toRetrieve.allocated_to[toRetrieve.numberOfAllocations];  // Set last allocated person at the place of the retrieving person.
	toRetrieve.numberOfAllocations--;
	toRetrieve.item_count++;
	if(
		writeInFile(
		INVENTORY_DATA_FILE_ADDRESS, &toRetrieve, 
		sizeof( Inventory ), n * sizeof( Inventory ) )
		) {
			cout << "The item is retrieved successfully\n\n";
	};
}

void showAllPersonsAllocated() {
	unsigned int n;
	Inventory toShowAllocations;
	cout << "Which item's allocated people you want to view: ";
	showAllInventoryNames();

	inputValidateInt( n, numOfUndeletedItemsInFile( INVENTORY_DATA_FILE_ADDRESS, sizeof( Inventory ) ) );
	n = indexOfUndeletedItemInFile( n, INVENTORY_DATA_FILE_ADDRESS );

	readFromFile( INVENTORY_DATA_FILE_ADDRESS, &toShowAllocations, sizeof( Inventory ), n * sizeof( Inventory ) );

	if (toShowAllocations.numberOfAllocations < 1) {
		cout << toShowAllocations.name << " is allocated to No one of the faculty members.\n\n";
	}

	else {
		cout << toShowAllocations.name << " is allocated to: \n";
		displayAllocatedPersons( toShowAllocations );
		cout << endl;
	}

}






/***************************************** Input Tasks *******************************************************************/

Inventory inputInventory() {
	Inventory inv;
	string str;

	cout << "Enter name of the inventory: ";
	inputStr( inv.name );

	cout << "Category of the item : ";
	inputStr( inv.category );

	cout << "Count: ";
	inputValidateInt( inv.item_count, 1 );

	return inv;
}

FacMember inputFacMember() {
	FacMember facMember{};

	inputStr( facMember.name );

	cout << "\tDepartment:\n\t\t1-> CS\t2->SE\t3->IT\t4->DS";

	unsigned int dept;
	inputValidateInt( dept, 4 );

	facMember.dept = static_cast<Department>(dept);

	return facMember;
}



/***************************************** Helping Functions *************************************************************/

bool areEqualStr( const char str1[], const char str2[], int maxLength ) {
	for (int i = 0; i < maxLength; i++) {
		if (str1[i] != str2[i]) return false;
		if (str1[i] == '\0' && str2[i] == '\0') break;
	}
	return true;
}

int strLength( const char str[] ) {
	int i = 0;
	while (str[i] != '\0')
		i++;

	return i;
}

bool isInStr( const char target[], const char part[] ) {
	int targetLength = strLength( target ), partLenght = strLength( part );

	for (int i = 0; i < (targetLength - partLenght); i++) {
		bool isPart = true;
		for (int j = 0; j < partLenght; j++) {
			if (part[j] != target[i + j]) {
				isPart = false;
				break;
			}
		}
		if (isPart) return true;
	}
	return false;
}

void inputStr( char x[] ) {
	for (int i = 0; i < MAX_NAME_LENGTH; i++) {
		cin.get( x[i] );

		if (x[i] == '\n') {
			x[i] = '\0';
			return;
		}
	}

	clearBuffer();
}

void inputValidateInt( unsigned int & n, int upperLimit ) {
	do {
		cin >> n;
		clearBuffer();

		if (n == 0 || n > upperLimit) {
			cout << "Enter correct Number..... ";
		}
	} while (n == 0 || n > upperLimit);
}

void inputValidateInt( int & n, int lowerLimit, int upperLimit ) {
	do {
		cin >> n;
		clearBuffer();

		if (n < lowerLimit || n > upperLimit) {
			cout << "Enter correct Number..... ";
		}
	} while (n < lowerLimit || n > upperLimit);
}

void setStr( char str1[], const char str2[], int length ) {
	for (int i = 0; i < length; i++) {
		str1[i] = str2[i];
	}
}

void clearBuffer() {
	cin.clear();
	cin.ignore( numeric_limits<streamsize>::max(), '\n' ); // Clear the buffer after taking the input.
}

void showAllInventoryNames() {
	size_t n = numOfItemsInFile( INVENTORY_DATA_FILE_ADDRESS, sizeof( Inventory ) );
	Inventory inv;
	for (int i = 0, j = 1; i < n; i++) {
		readFromFile( INVENTORY_DATA_FILE_ADDRESS, reinterpret_cast<char *>(&inv), sizeof( Inventory ), i * sizeof( Inventory ) );
		if (inv.deleted) continue;
		cout << j << "-> " << inv.name << endl;
		j++;
	}
}


void displayAllocatedPersons( Inventory inv ) {
	for (int i = 0; i < inv.numberOfAllocations; i++) {
		cout << "\t\t" << i + 1 << ". " << inv.allocated_to[i].name << "\t" << tellDept( inv.allocated_to[i].dept ) << endl;
	}
}

void searchByName() {
	char nameSearch[MAX_NAME_LENGTH];
	cout << "Enter name (or part of name) to search: ";
	inputStr( nameSearch );

	size_t n = numOfItemsInFile( INVENTORY_DATA_FILE_ADDRESS, sizeof( Inventory ) );
	Inventory invChecked;
	for (int i = 0; i < n; i++) {
		readFromFile( INVENTORY_DATA_FILE_ADDRESS, &invChecked, sizeof( Inventory ), i * sizeof( Inventory ) );
		if (!(invChecked.deleted) && isInStr( invChecked.name, nameSearch )) {
			showInventory( invChecked );
		}
	}

	cout << "Not Found .....";
}

void searchByCategory() {
	char category[MAX_NAME_LENGTH];

	cout << "Enter Category to search inventories: ";
	inputStr( category );

	size_t n = numOfItemsInFile( INVENTORY_DATA_FILE_ADDRESS, sizeof( Inventory ) );
	Inventory invChecked;
	bool found = false;

	for (int i = 0; i < n; i++) {
		readFromFile( INVENTORY_DATA_FILE_ADDRESS, &invChecked, sizeof( Inventory ), i * sizeof( Inventory ) );
		if (!invChecked.deleted && areEqualStr( invChecked.name, category )) {
			showInventory( invChecked );
			found = true;
		}
	}
	if (!found) {
		cout << "No Inventory belongs to entered Category...\n\n";
	}
}


string tellDept( Department dept ) {
	switch (dept) {
	case 1: return "Computer Science";
	case 2: return "Software Engineering";
	case 3: return "Information Technology";
	case 4: return "Data Science";
	}
}


/***************************************** File Tasks *******************************************************************/

bool writeInFile( const string & fileAddress, void * memoryAddress, size_t size, size_t startPoint ) {
	fstream file;

	if (fileInWriteMode( file, fileAddress )) {
		file.seekp( startPoint, ios::beg );
		file.write( reinterpret_cast<char *>(memoryAddress), size );
		file.close();
		return true;
	}
	else return false;
}

bool readFromFile( const string & fileAddress, void * memoryAddress, size_t size, size_t startPoint ) {
	fstream file;
	if (fileInReadMode( file, fileAddress )) {
		file.seekg( startPoint, ios::beg );
		file.read( reinterpret_cast<char *>(memoryAddress), size );
		file.close();
		return true;
	}
	else return false;
}

bool fileInReadMode( fstream & file, const string & name ) {
	file.open( name, ios::binary | ios::in );
	if (file.fail()) {
		cout << "Data File not found...";
		return false;
	}
	else {
		return true;
	}
}

bool fileInWriteMode( fstream & file, const string & name ) {
	file.open( name, ios::binary | ios::out | ios::in );
	if (file.fail()) {
		file.open( name, ios::binary | ios::out | ios::app );
		if (file.fail()) {
			cout << "Operation Failed...";
			return false;
		}
		else return true;
	}
	else {
		return true;
	}
}


size_t numOfItemsInFile( const string & fileAddress, int sizeOfItem ) {
	return lastIndexOfFile( fileAddress ) / sizeOfItem;
}

size_t numOfUndeletedItemsInFile( const string & fileAddress, int sizeOfItem ) {
	size_t n = numOfItemsInFile( fileAddress, sizeof( Inventory ) ), num = 0;
	Inventory invChecked;
	for (int i = 0; i < n; i++) {
		readFromFile( fileAddress, &invChecked, sizeof( Inventory ), i * sizeof( Inventory ) );
		if (!(invChecked.deleted)) num++;
	}
	return num;
}

size_t lastIndexOfFile( const string & fileAddress ) {
	fstream file;
	if (fileInReadMode( file, fileAddress )) {
		file.seekg( 0, ios::end );
		streamoff n = file.tellg();
		file.close();
		return n;
	}
	else return 0;
}



unsigned int indexOfDeletedItemInFile( const string & fileAddress ) {  //returns index to write if any file is deleted otherwise last index.
	fstream file;

	size_t n = numOfItemsInFile( fileAddress, sizeof( Inventory ) );
	Inventory invChecked;
	for (int i = 0; i < n; i++) {
		readFromFile( fileAddress, &invChecked, sizeof( Inventory ), i * sizeof( Inventory ) );
		if (invChecked.deleted) return i;
	}
	return numOfItemsInFile( fileAddress, sizeof( Inventory ) );
}

unsigned int indexOfUndeletedItemInFile( unsigned int index, const string & fileAddress ) {
	fstream file;
	index--; // Because the entered index was added one.
	Inventory invChecked;
	for (int i = 0; i <= index; i++) {
		readFromFile( fileAddress, &invChecked, sizeof( Inventory ), i * sizeof( Inventory ) );
		if (invChecked.deleted) index++;
	}
	return index;

}


void generateID( char id[] ) {
	char x[ITEM_ID_LENGTH] = "FCITINVvnl";
	setStr( id, x, ITEM_ID_LENGTH );
}


/***************************************** Output Tasks *****************************************************************/



void showInventory( Inventory inv ) {

	cout << "\tInventory ID: " << inv.item_ID << endl;
	cout << "\tName: " << inv.name << endl;
	cout << "\tCount: " << inv.item_count << endl;
	cout << "\tCategory: " << inv.category << endl;
	if (inv.numberOfAllocations > 0) {
		cout << "\tAllocated to: " << endl;
		displayAllocatedPersons( inv );
	}
	cout << "del: " << inv.deleted;
}


