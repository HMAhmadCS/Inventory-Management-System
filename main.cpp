/*
	 *	Final Term Project
	 *	Inventory Management System
	 *  BCSF21M502 - Hafiz Muhammad Ahmad
*/

#include "functions.h"



int main() {

	int choice;

	welcome();

	do {
		menu();

		inputValidateInt( choice, 1, 9 );

		selectCommand( choice );

		if (choice != 9) {
			system( "pause" );
			system( "cls" );
		}

	} while (choice != 9);


	return 0;
}