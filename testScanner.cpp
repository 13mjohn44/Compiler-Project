#include <iostream>
#include <fstream>
#include <iomanip>
#include "Scanner.h"

using namespace std;

// function that calls the scanner function and prints out the returned functions
void testScanner(ifstream &file){
    Token token = Token();
    while(token.getID() != EOFTK){
        token = scanner(file);
        // prints Token ID
        cout << "token ID: " << std::setw(14) << token.getIDString() << "\tinstance: " << std::setw(9) << token.getInstance()
             << "\tline number: " << std::setw(5) << token.getLineNum() << "\tChararacter Number: " << std::setw(5) << token.GetCharNum() << std::endl;
    }
}
