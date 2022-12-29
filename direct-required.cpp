#include "functions.h"

void addInventoryItem() {
	Inventory newInv = inputInventory();
	generateID( newInv.item_ID, INVENTORY_DATA_FILE_ADDRESS );
	if (
		addItemInFile( INVENTORY_DATA_FILE_ADDRESS, &newInv, sizeof( Inventory ) )
		) {
		cout << "Inventory Added Successfully...\n\n";
	}
	else {
		cout << "Failed....\t Inventory couldn't be added....\n\n";
	}
}


void viewAllInventoryItems() {
	size_t n = numOfItemsInFile( INVENTORY_DATA_FILE_ADDRESS, sizeof( Inventory ) );
	if (n < 1) {
		cout << "No items to show...\n\n";
		return;
	}
	
	Inventory invShown;
	fstream file;
	fileInReadMode( file, INVENTORY_DATA_FILE_ADDRESS );
	
	for (int i = 0; i < n; i++) {
		readFromOpenedFile( file, &invShown, sizeof( Inventory ) );
		if (invShown.deleted) {
			continue;
		}
		showInventory( invShown );
		cout << "\n\t\t*******************************\n\n";
	}
	file.close();

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

	int numberOfUndelInvs = numOfUndeletedItemsInFile( INVENTORY_DATA_FILE_ADDRESS, sizeof( Inventory ) );

	if (numberOfUndelInvs < 1) {
		cout << "No items to edit in the saved data....\n\n";
		return;
	}

	unsigned int invIndexToChange;
	Inventory invToChange;
	cout << "Which item do you want to change (Enter the number of item): \n";
	showAllInventoryNames();

	inputValidateInt( invIndexToChange, numberOfUndelInvs );
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

	int numberOfUndelInvs = numOfUndeletedItemsInFile( INVENTORY_DATA_FILE_ADDRESS, sizeof( Inventory ) );

	if (numberOfUndelInvs < 1) {
		cout << "No items to delete in the saved data....\n\n";
		return;
	}

	cout << "Which Inventory you want to delete: \n";
	showAllInventoryNames();
	unsigned int invToDelIndex;
	Inventory invToDel;

	inputValidateInt( invToDelIndex, numberOfUndelInvs );

	invToDelIndex = indexOfUndeletedItemInFile( invToDelIndex, INVENTORY_DATA_FILE_ADDRESS );

	cout << "Do you really want to delete this item ? (y / n)";

	if (_getche() == 'y') {

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

	int numberOfUndelInvs = numOfUndeletedItemsInFile( INVENTORY_DATA_FILE_ADDRESS, sizeof( Inventory ) );

	if (numberOfUndelInvs < 1) {
		cout << "No items to assign to anyone....\n\n";
		return;
	}

	Inventory toAssign;
	unsigned int n;

	showAllInventoryNames();
	cout << "\nWhich Item to assign: ";

	inputValidateInt( n, numberOfUndelInvs );
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
	int numberOfUndelInvs = numOfUndeletedItemsInFile( INVENTORY_DATA_FILE_ADDRESS, sizeof( Inventory ) );
	if (numberOfUndelInvs < 1) {
		cout << "No items to view in the saved data....\n\n";
		return;
	}

	showAllInventoryNames();

	unsigned int n, x;
	Inventory toRetrieve;

	cout << "Which Item to retrieve: ";
	inputValidateInt( n, numberOfUndelInvs );
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
	if (
		writeInFile(
			INVENTORY_DATA_FILE_ADDRESS, &toRetrieve,
			sizeof( Inventory ), n * sizeof( Inventory ) )
		) {
		cout << "The item is retrieved successfully\n\n";
	};
}

void showAllPersonsAllocated() {

	int numberOfUndelInvs = numOfUndeletedItemsInFile( INVENTORY_DATA_FILE_ADDRESS, sizeof( Inventory ) );
	if (numberOfUndelInvs < 1) {
		cout << "No items to view in the saved data....\n\n";
		return;
	}

	unsigned int n;
	Inventory toShowAllocations;
	cout << "Which item's allocated people you want to view: ";
	showAllInventoryNames();

	inputValidateInt( n, numberOfUndelInvs );
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
