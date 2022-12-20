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
    auto * invShown = new Inventory;
    for (int i = 0; i < n; i++) {
        readFromFile( INVENTORY_DATA_FILE_ADDRESS, reinterpret_cast<char *>(invShown), sizeof( Inventory ), i * sizeof( Inventory ) );
        showInventory( *invShown );
        cout << "\n\t\t*******************************\n\n";
    }
}

void searchInventoryItem() {
    char name[21]; inputStr( name );

    size_t n = numOfItemsInFile( INVENTORY_DATA_FILE_ADDRESS, sizeof( Inventory ) );
    auto * invChecked = new Inventory;
    for (int i = 0; i < n; i++) {
        readFromFile( INVENTORY_DATA_FILE_ADDRESS, reinterpret_cast<char *>(invChecked), sizeof( Inventory ), i * sizeof( Inventory ) );
        if (areEqualStr( invChecked->name, name )) {
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
    char toEdit[MAX_NAME_LENGTH], retake;

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
        retake = _getch();
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
    cin >> n;

    cout << "Do you really want to delete this item ? (y / n)";
    if (_getch() == 'y') {
        fstream file( INVENTORY_DATA_FILE_ADDRESS, ios::out | ios::in );
        file.seekp( (sizeof( Inventory ) * n - sizeof( bool )) );
        file.put( true );
    }
    else return;
}

void assignItem() {
    showAllInventoryNames();
    int n;
    Inventory toAssign;
    cout << "Which Item to assign: ";
    cin >> n;
    n--;
    clearBuffer();
    readFromFile( INVENTORY_DATA_FILE_ADDRESS, reinterpret_cast<char *>(&toAssign), sizeof( Inventory ), n * sizeof( Inventory ) );
    cout << "Enter name of the person, whom to assign: ";
    cin >> toAssign.allocated_to[toAssign.numberOfAllocations];
    toAssign.numberOfAllocations++;
    toAssign.item_count--;
    writeInFile( INVENTORY_DATA_FILE_ADDRESS, reinterpret_cast<char *>(&toAssign), sizeof( Inventory ), n * sizeof( Inventory ) );


}

void retrieveItem() {
    showAllInventoryNames();
    int n, x;
    Inventory toRetrieve;
    cout << "Which Item to retrieve: ";
    cin >> n;
    n--;
    clearBuffer();
    readFromFile( INVENTORY_DATA_FILE_ADDRESS, reinterpret_cast<char *>(&toRetrieve), sizeof( Inventory ), n * sizeof( Inventory ) );
    cout << "Which Person to form: ";
    displayAllocatedPersons( toRetrieve );
    cin >> x;
    setStr( toRetrieve.allocated_to[x], toRetrieve.allocated_to[toRetrieve.numberOfAllocations] );  // Set last allocated person at the place of the retrieving person.
    toRetrieve.numberOfAllocations--;
    toRetrieve.item_count++;
    writeInFile( INVENTORY_DATA_FILE_ADDRESS, reinterpret_cast<char *>(&toRetrieve), sizeof( Inventory ), n * sizeof( Inventory ) );
}

void showAllPersonsAllocated() {
    int n;
    Inventory toShowAllocations;
    cout << "Which item's allocated people you want to view: ";
    showAllInventoryNames();
    cin >> n;
    n--;
    readFromFile( INVENTORY_DATA_FILE_ADDRESS, reinterpret_cast<char *>(&toShowAllocations), sizeof( Inventory ), n * sizeof( Inventory ) );
    cout << toShowAllocations.name << " is allocated to: \n";
    displayAllocatedPersons( toShowAllocations );
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
    cin >> inv.numberOfAllocations;
    clearBuffer();

    for (int i = 0; i < inv.numberOfAllocations; i++) {
        cout << "Person " << i + 1 << ": ";
        inputStr( inv.allocated_to[i] );
    }

    return inv;
}



/***************************************** Helping Functions *************************************************************/

bool areEqualStr( const char str1[], const char str2[], int maxLength ) {
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
        //if (inv.deleted) continue;
        cout << j << "-> " << inv.name << endl;
        j++;
    }
}


void displayAllocatedPersons( Inventory inv ) {
    for (int i = 0; i < inv.numberOfAllocations; i++) {
        cout << "\t\t" << i + 1 << ". " << inv.allocated_to[i] << endl;
    }
}



/***************************************** File Tasks *******************************************************************/

bool writeInFile( const string & fileAddress, char * memoryAddress, size_t size, size_t startPoint ) {
    fstream file;

    if (fileInWriteMode( file, fileAddress )) {
        file.seekp( startPoint, ios::beg );
        file.write( memoryAddress, size );
        file.close();
        return true;
    }
    else return false;
}

bool readFromFile( const string & fileAddress, char * memoryAddress, size_t size, size_t startPoint ) {
    fstream file;
    if (fileInReadMode( file, fileAddress )) {
        file.seekg( startPoint, ios::beg );
        file.read( memoryAddress, size );
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
        cout << "Operation Failed...";
        return false;
    }
    else {
        return true;
    }
}


size_t numOfItemsInFile( const string & fileAddress, int sizeOfItem ) {
    return lastIndexOfFile( fileAddress ) / sizeOfItem;
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



unsigned int indexOfDeletedItemInFile( const string & fileAddress ) {//returns index to write if any file is deleted other wise last index.
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
    displayAllocatedPersons( inv );

    cout << "del: " << inv.deleted;
}


