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


/***************************************** Functions For Requirements *******************************************************/

/*
Delete inventory item
Assign inventory item
Retrieve inventory item
View the list of faculty members who have borrowed a specific item.
*/

bool addInventoryItem() {
	Inventory newInv = inputInventory();
	generateID( newInv.item_ID );
	if (
		writeInFile(
			INVENTORY_DATA_FILE_ADDRESS,
			reinterpret_cast<char *>(&newInv),
			sizeof( Inventory ),
			indexOfDeletedItemInFile( INVENTORY_DATA_FILE_ADDRESS )
		)) {
		return true;
	}
	else return false;
}


void viewAllInventoryItems() {
	size_t n = numOfItemsInFile( INVENTORY_DATA_FILE_ADDRESS, sizeof( Inventory ) );
	Inventory * invShown = new Inventory;
	for (int i = 0; i < n; i++) {
		readFromFile( INVENTORY_DATA_FILE_ADDRESS, reinterpret_cast<char *>(invShown), sizeof( Inventory ), i * sizeof( Inventory ) );
		showInventory( *invShown );
		cout << "\n\t\t*******************************\n\n";
	}
}

void searchInventoryItem() {
	char name[21]; inputStr( name );

	int n = numOfItemsInFile( INVENTORY_DATA_FILE_ADDRESS, sizeof( Inventory ) );
	Inventory * invChecked = new Inventory;
	for (int i = 0; i < n; i++) {
		readFromFile( INVENTORY_DATA_FILE_ADDRESS, reinterpret_cast<char *>(invChecked), sizeof( Inventory ), i * sizeof( Inventory ) );
		if (equalCstr( invChecked->name, name, MAX_NAME_LENGTH )) {
			showInventory( *invChecked );
			return;
		}
	}

	cout << "Not Found .....";
	delete invChecked;

}


void editInventoryItem() {
	int invIndexToChange;
	Inventory invToChange;
	cout << "Which item do you want to change (Enter the number of item): \n";
	showAllInventoryNames();
	cin >> invIndexToChange;
	invIndexToChange--;
	readFromFile( INVENTORY_DATA_FILE_ADDRESS, reinterpret_cast<char *>(&invToChange), sizeof( Inventory ), invIndexToChange * sizeof( Inventory ) );
	cout << "\nPrevious Details:\n";
	showInventory( invToChange );
	cout << endl;
	cout << "What do you want to change:\n";
	cout << "1 --> Name\n";
	cout << "2 --> Category\n";
	cout << "3 --> Count\n";
	int choice;
	cin >> choice;
	clearBuffer();
	char toEdit[MAX_NAME_LENGTH], retake = '.';

	do {
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
			cin >> invToChange.item_count;
			clearBuffer();
			break;
		default:
			cout << "Wrong Input.... aborted...\n";
		}
		cout << "Press y to change another thing and any other key to go back.";
	} while (retake == 'y');

	writeInFile( INVENTORY_DATA_FILE_ADDRESS, reinterpret_cast<char *>(&invToChange), sizeof( Inventory ), invIndexToChange * sizeof( Inventory ) );
	cout << "\nUpdated Details:\n";
	Inventory changedInventory;
	readFromFile( INVENTORY_DATA_FILE_ADDRESS, reinterpret_cast<char *>(&changedInventory), sizeof( Inventory ), invIndexToChange * sizeof( Inventory ) );
	showInventory( changedInventory );

}


void deleteInventoryItem() {
	cout << "Which Inventory you want to delete: \n";
	showAllInventoryNames();
	int n;
	Inventory invToDelete;
	cin >> n;

	cout << "Do you really want to delete this item ? (y / n)";
	if (_getch() == 'y') {
		fstream file( INVENTORY_DATA_FILE_ADDRESS, ios::out | ios::in );
		file.seekp( (sizeof( Inventory ) * n - sizeof( bool )) );
		file.put( true );
	}
	else return;
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
	cin >> inv.item_count;
	clearBuffer();

	cout << "To how much people " << inv.name << " is allocated: ";
	cin >> inv.numberOfAlocations;
	clearBuffer();

	for (int i = 0; i < inv.numberOfAlocations; i++) {
		cout << "Person " << i + 1 << ": ";
		inputStr( inv.allocated_to[i] );
	}

	return inv;
}



/***************************************** Helping Functions *************************************************************/

bool equalCstr( char str1[], char str2[], int maxLength ) {
	for (int i = 0; i < maxLength; i++) {
		if (str1[i] != str2[i]) return false;
		if (str1[i] == '\0' && str2[i] == '\0') break;
	}
	return true;
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

void setStr( char str1[], char str2[], int length ) {
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
		//if (inv.deleted) continue;
		cout << j << "-> " << inv.name << endl;
		j++;
	}
}



/***************************************** File Tasks *******************************************************************/

bool writeInFile( string fileAddress, char * memoryAddress, size_t size, size_t startPoint ) {
	fstream file;

	if (fileInWriteMode( file, fileAddress )) {
		file.seekp( startPoint, ios::beg );
		file.write( memoryAddress, size );
		file.close();
		return true;
	}
	else return false;
}

bool readFromFile( string fileAddress, char * memoryAddress, size_t size, size_t startPoint ) {
	fstream file;
	if (fileInReadMode( file, fileAddress )) {
		file.seekg( startPoint, ios::beg );
		file.read( memoryAddress, size );
		file.close();
		return true;
	}
	else return false;
}

bool fileInReadMode( fstream & file, string name ) {
	file.open( name, ios::binary | ios::in );
	if (file.fail()) {
		cout << "Data File not found...";
		return false;
	}
	else {
		return true;
	}
}

bool fileInWriteMode( fstream & file, string name ) {
	file.open( name, ios::binary | ios::out | ios::in );
	if (file.fail()) {
		cout << "Operation Failed...";
		return false;
	}
	else {
		return true;
	}
}


size_t numOfItemsInFile( string fileAddress, int sizeOfItem ) {
	return lastIndexOfFile( fileAddress ) / sizeOfItem;
}

size_t lastIndexOfFile( string fileAddress ) {
	fstream file;
	if (fileInReadMode( file, fileAddress )) {
		file.seekg( 0, ios::end );
		int n = file.tellg();
		file.close();
		return n;
	}
	else return 0;
}



unsigned int indexOfDeletedItemInFile( string fileAddress ) {//returns index to write if any file is deleted other wise last index.
	fstream file;
	if (fileInReadMode( file, fileAddress )) {

		char delBit = 'a';

		while (!file.eof()) {
			file.seekg( sizeof( Inventory ) - sizeof( bool ), ios::cur );
			file.get( delBit );
			if ((bool) delBit) {
				unsigned int cur = file.tellg();
				cout << cur / sizeof( Inventory );
				return (cur - sizeof( Inventory ));//return the index for deleted item.
			}
		}

		return lastIndexOfFile( fileAddress );

	}
	return 0;
}


void generateID( char id[] ) {
	char x[ITEM_ID_LENGTH] = "FCITINVvnl";
	setStr( id, x, ITEM_ID_LENGTH );
}


/***************************************** Output Tasks *******************************************************************/



void showInventory( Inventory inv ) {

	cout << "\tInventory ID: " << inv.item_ID << endl;
	cout << "\tName: " << inv.name << endl;
	cout << "\tCount: " << inv.item_count << endl;
	cout << "\tCategory: " << inv.category << endl;
	cout << "\tAllocated to: " << endl;
	for (int i = 0; i < inv.numberOfAlocations; i++) {
		cout << "\t\t" << i + 1 << ". " << inv.allocated_to[i] << endl;
	}

	cout << "del: " << inv.deleted;
}


