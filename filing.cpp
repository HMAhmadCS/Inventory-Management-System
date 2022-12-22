#include "functions.h"


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


void generateID( char id[], const string & fileAddress ) {
	char x[ITEM_ID_LENGTH] = "FCIT-INV-000", y[ITEM_ID_LENGTH] = "FCIT-INV-000";
	fstream file;
	if (fileInReadMode( file, fileAddress )) {
		
		int n = numOfItemsInFile(fileAddress, sizeof(Inventory));
		if (n == 0) {
			x[9] = '0'; x[10] = '0'; x[11] = '0';
		}
		else for (int i = 0; i < n; i++) {
			file.seekg( sizeof( Inventory )*i, ios::beg );
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

