#include "functions.h"


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
			cout << "Enter correct Number.....";
		}
	} while (n < lowerLimit || n > upperLimit);
}


void clearBuffer() {
	cin.clear();
	cin.ignore( numeric_limits<streamsize>::max(), '\n' ); // Clear the buffer after taking the input.
}
