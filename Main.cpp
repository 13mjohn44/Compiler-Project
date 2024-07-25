/*
Mateo John
5/2/24
Course: CS 4280
Program purpose:
The program is to parse the input, generate a parse tree, perform static semantics, and then generate a target file.
Any error should display detailed message, including line number if available (depending on scanner).

*/


#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "cleanString.h"
#include "testScanner.h"
#include "parser.h"
#include "node.h"
#include "preprocessor.h"


using namespace std;

int main ( int argc, char *argv[] )
{
    string currentWord;
    string allWords = "";
    string fileName;
    ifstream inputFile;
    bool status = false;

    //if we have a commmand line arguement of a file, then we will build the tree using the filename
    // Otherwise we will take in standard input, send it it to output.txt, then build the tree using that file
    if ( argc < 2 ){
        // We loop through and take in all input from keyboard or file redirect
        while(cin >> currentWord){
            allWords += currentWord + " ";
        }
        fileName = SendRedirectToFile(allWords);

        inputFile.open(fileName);

        nodeTK * parseTreeTop;
        //the parser is called here and the root is stored in parseTreeTop
        parseTreeTop = parser(inputFile);
        // parseTreeTop is then sent to printParseTree to then print the tree
        // printParseTree(parseTreeTop);
        // Check the the semantics of vars
        CheckScope(parseTreeTop, 0);

        inputFile.close();

        inputFile.close();

        ofstream targetOutput;

        string targetFilename = GetBaseFileName(fileName) + ".asm";

        targetOutput.open(targetFilename);

        treeDecider(parseTreeTop, targetOutput, status, 0);

        targetOutput.close();

        if(status == true){
            cout << fileName << endl;
        }else{
            cout << "P4 ERROR: there was some error with the parser." << endl;
        }
        
    } else if(argc > 2){
        cout << "Too many command line arguments given. Program may be called as follows: " << endl;
        cout << "scanner // read from the keyboard until simulated EOF" << endl;
        cout << "scanner < somefile // same as above except redirecting from somefile instead of keyboard, this tests keyboard input" << endl;
        cout << "scanner somefile  // read from somefile" << endl;
    } else {
        fileName = argv[1];
        inputFile.open(fileName);
        

        nodeTK * parseTreeTop;
        //the parser is called here and the root is stored in parseTreeTop
        parseTreeTop = parser(inputFile);
        
        // parseTreeTop is then sent to printParseTree to then print the tree
        // printParseTree(parseTreeTop);

        // Check the the semantics of vars
        CheckScope(parseTreeTop, 0);

        inputFile.close();

        ofstream targetOutput;

        string targetFilename = GetBaseFileName(fileName) + ".asm";

        targetOutput.open(targetFilename);

        treeDecider(parseTreeTop, targetOutput, status, 0);

        targetOutput.close();
        if(status == true){
            cout << fileName << endl;
        }else{
            cout << "P4 ERROR: there was some error with the parser." << endl;
        }
        
        
    }

  return 0;
}