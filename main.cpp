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

		inputValidateInt( choice, 0, 8 );

		selectCommand( choice );

		if (choice != 0) {
			system( "pause" );
			system( "cls" );
		}

	} while (choice != 0);


	return 0;
}