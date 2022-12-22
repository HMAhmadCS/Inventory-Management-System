#include "functions.h"

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


void setStr( char str1[], const char str2[], int length ) {
	for (int i = 0; i < length; i++) {
		str1[i] = str2[i];
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