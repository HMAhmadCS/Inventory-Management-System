#include "functions.h"


bool writeInFile( const char fileAddress[], void * memoryAddress, size_t size, size_t startPoint) {
	fstream file;

	if (fileInWriteMode( file, fileAddress )) {
		file.seekp( startPoint, ios::beg );
		file.write( reinterpret_cast<char *>(memoryAddress), size );
		file.close();
		return true;
	}
	else return false;
}

bool addItemInFile( const char fileAddress[], void * memoryAddress, size_t size) {
	fstream file;

	if (fileInAppendMode( file, fileAddress )) {
		file.write( reinterpret_cast<char *>(memoryAddress), size );
		file.close();
		return true;
	}
	else return false;
}

void writeInOpenedFile( fstream & file, void * memoryAddress, size_t size ) {
		file.write( reinterpret_cast<char *>(memoryAddress), size );
}

bool readFromFile( const char fileAddress[], void * memoryAddress, size_t size, size_t startPoint) {
	fstream file;
	if (fileInReadMode( file, fileAddress )) {
		file.seekg( startPoint, ios::beg );
		file.read( reinterpret_cast<char *>(memoryAddress), size );
		file.close();
		return true;
	}
	else return false;
}

void readFromOpenedFile( fstream & file, void * memoryAddress, size_t size ) {
	file.read( reinterpret_cast<char *>(memoryAddress), size );
}

bool fileInReadMode( fstream & file, const char name[]) {
	file.open( name, ios::binary | ios::in );
	if (file.fail()) {
		return false;
	}
	else {
		return true;
	}
}

bool fileInWriteMode( fstream & file, const char name[]) {
	file.open( name, ios::binary | ios::out | ios::in );
	if (file.fail()) {
		file.open( name, ios::binary | ios::out | ios::app );
		if (file.fail()) {
			return false;
		}
		else return true;
	}
	else {
		return true;
	}
}

bool fileInAppendMode( fstream & file, const char name[]) {
	file.open( name, ios::binary | ios::out | ios::app );
	if (file.fail()) {
		return false;
	}
	else return true;
}


size_t numOfItemsInFile( const char fileAddress[], int sizeOfItem) {
	return lastIndexOfFile( fileAddress ) / sizeOfItem;
}

size_t numOfUndeletedItemsInFile( const char fileAddress[], size_t size) {
	size_t n = numOfItemsInFile( fileAddress, size ), num = 0;
	Inventory invChecked;
	fstream file;
	fileInReadMode( file, INVENTORY_DATA_FILE_ADDRESS );

	for (int i = 0; i < n; i++) {
		readFromOpenedFile( file, &invChecked, sizeof( Inventory ) );
		if (!(invChecked.deleted)) num++;
	}
	file.close();
	return num;
}

size_t lastIndexOfFile( const char fileAddress[]) {
	fstream file;
	if (fileInReadMode( file, fileAddress )) {
		file.seekg( 0, ios::end );
		streamoff n = file.tellg();
		file.close();
		return n;
	}
	else return 0;
}



//unsigned int indexOfDeletedItemInFile( const char fileAddress[]) {  //returns index to write if any file is deleted otherwise last index.
//	fstream file;
//
//	size_t n = numOfItemsInFile( fileAddress, sizeof( Inventory ) );
//	Inventory invChecked;
//	for (int i = 0; i < n; i++) {
//		readFromFile( fileAddress, &invChecked, sizeof( Inventory ), i * sizeof( Inventory ) );
//		if (invChecked.deleted) return i;
//	}
//	return numOfItemsInFile( fileAddress, sizeof( Inventory ) );
//}

unsigned int indexOfUndeletedItemInFile( unsigned int index, const char fileAddress[]) {
	fstream file;
	index--; // Because the entered index was added one.
	Inventory invChecked;
	for (int i = 0; i <= index; i++) {
		readFromFile( fileAddress, &invChecked, sizeof( Inventory ), i * sizeof( Inventory ) );
		if (invChecked.deleted) index++;
	}
	return index;

}


void generateID( char id[], const char fileAddress[]) {
	char x[ITEM_ID_LENGTH] = "FCIT-INV-000", y[ITEM_ID_LENGTH] = "FCIT-INV-000";
	fstream file;
	if (fileInReadMode( file, fileAddress )) {

		int n = numOfItemsInFile( fileAddress, sizeof( Inventory ) );
		if (n == 0) {
			x[9] = '0'; x[10] = '0'; x[11] = '0';
		}
		else for (int i = 0; i < n; i++) {
			file.seekg( sizeof( Inventory ) * i, ios::beg );
			file >> x;
			if (x[9] < y[9] || x[9] == y[9] && x[10] < y[10] || x[9] == y[9] && x[10] == y[10] && x[11] < y[11]) {
				setStr( x, y, ITEM_ID_LENGTH );
			}
			else setStr( y, x, ITEM_ID_LENGTH );
		}
	}
	else {
		x[9] = '0'; x[10] = '0'; x[11] = '0';
	}
	x[11] += 1;
	x[12] = '\0';
	setStr( id, x, ITEM_ID_LENGTH );
}



void removeDeletedItems( const char fileAddress[], unsigned int size ) {

	cout << "Exiting....";
	fstream file;
	fstream temp;
	Inventory inv;
	size_t n = numOfItemsInFile( fileAddress, size );
	fileInReadMode( file, fileAddress );
	fileInAppendMode( temp, "temp.dat" );

	for (int i = 0; i < n; i++) {
		readFromOpenedFile( file, &inv, size );
		if (inv.deleted) continue;
		writeInOpenedFile( temp, &inv, size );
	}

	file.close();
	temp.close();

	char filename[] = "inventory_item_data.txt";

	remove( fileAddress );

	if (rename( "temp.dat", fileAddress )) {
		cout << "\n....Exited";
	}
}
