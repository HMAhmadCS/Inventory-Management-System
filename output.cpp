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




void showInventory( Inventory inv ) {

	cout << "\tInventory ID: " << inv.item_ID << endl;
	cout << "\tName: " << inv.name << endl;
	cout << "\tCount: " << inv.item_count << endl;
	cout << "\tCategory: " << inv.category << endl;
	if (inv.numberOfAllocations > 0) {
		cout << "\tAllocated to: " << endl;
		displayAllocatedPersons( inv );
	}
}


void displayAllocatedPersons( Inventory inv ) {
	for (int i = 0; i < inv.numberOfAllocations; i++) {
		cout << "\t\t" << i + 1 << ". " << inv.allocated_to[i].name << "\t" << tellDept( inv.allocated_to[i].dept ) << endl;
	}
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