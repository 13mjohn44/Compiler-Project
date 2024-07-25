#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "Scanner.h"
#include "FSA.h"
#include "token.h"
#include "charProcess.h"
#include "errorHandler.h"
#include "preprocessor.h"
#include "stringMult.h"

using namespace std;

bool printToken = false;

//array of keywords
const int keyLength = 17;
string keyWords [keyLength] = {"start", "stop", "while", "repeat", "until", "label", "return", "cin", "cout", "tape", "jump", "if", "then", "pick", "create", "set", "func"};

//This function reads a line one at a time then breaks them apart into individual tokens
Token ScanLIne(string rawLine, string line, int &index, int lineNumber, bool eof){
    Token tempToken = Token();  //creating a null token
    if(eof){
        tempToken = Token(EOFTK, "EOF", lineNumber, index);
        return tempToken;
    }
    
    line += '\n'; //reappends that \n taken away by getline()
    States state = s1;  //setting initial state to 0
    States nextState;   
    char lookAhead = line[index]; //setting char to starting index
    string S = "";

    //checks that we dont have a final state
    while(state < 1000){
        nextState = (States)(Table[state][getCharVal(lookAhead)]);

        //produce error message and exit program if nextstate will result in error
        if(isError(nextState)){
            cout << getError(nextState) << "'" << lookAhead << "'" << endl;
            cout << "line " << lineNumber << ":" << (index + 1) << ":\n" << "'" << rawLine << "'" << endl;
            string space = " ";
            cout << space * (index + 1) << "^";
            exit(-1);
        }
        // returning the appropriate token depending on the state
        if(nextState >= 1000) {
            Token tempToken = Token(nextState, S, lineNumber, index);
            if(tempToken.tokenID == ID_TK){ // checking to see if the id is a keyword
                if(isKeyWord(S)){
                    tempToken.setTkID(KW_TK); // changing token type if it is
                }
                return tempToken;
            }else {
                return tempToken;
            }
        }else { // if no token has been found update the state and get the next lookahead
            state = nextState;
            if(!isspace(lookAhead))
                S += lookAhead;
            lookAhead = line[++index];
        }
    }

    return tempToken;
}


void printTK(Token tk){
    if(printToken){
        cout << tk.getIDString() << " : " << tk.getInstance() << " " << tk.getLineNum() << ":" << tk.GetCharNum() << endl;
    }
}

//returns true if the current identifier is a keyword
bool isKeyWord(string id){
    for(int i = 0; i < keyLength; i++){
        if(id.compare(keyWords[i]) == 0){
            return true; 
        }
    }
    return false;
}

// so this part of the scanner feed lines to the line scanner and returns what ever token it gets
Token scanner(ifstream &fileTcompile){
    static int lineNumber = 1;
    static string rawLine;             // all variables are used to record the current
    static string line;                // position of the scanner
    static bool endOfFile = false;
    static bool fileComplete = false;
    static int index = 0;
    static bool lineDone = true;
    bool gotToken = false;

    if(!endOfFile){
        while(!gotToken){
            if(lineDone){
                getline(fileTcompile, rawLine);
                if(!fileComplete && !fileTcompile)  // this constatly checks if the end of the
                {                                   // has been reached yet
                    if(fileTcompile.eof()){
                        endOfFile = true;           // if it has change bool
                    }
                    else{
                        cout << "Error: Problem reading the file in scanner function.";
                        cout << "Make sure you enter a file pointer at function call";
                        exit(0);
                    }
                }
                if(!fileComplete){          // if the file is read fine just reset values for new line
                    lineDone = false;
                    line = RemoveComment(rawLine);
                    index = 0;
                }
            }

            if(!lineDone){
                if(!isOnlySpace(line, index) && !fileComplete){ // wont search line if tail is all whitespace
                    if(endOfFile){
                        fileComplete = true;
                        lineNumber--;
                    }
                    Token currentTk = ScanLIne(rawLine, line, index, lineNumber, endOfFile); // creates a token
                    printTK(currentTk);
                    return currentTk;
                    
                } else if (endOfFile && !fileComplete){ // this catches the case of trailing new lines
                    fileComplete = true;
                    if(isOnlySpace(line,index)){ //this will set the index if there is any extra WS
                        lineNumber--;
                    }
                    Token currentTk = ScanLIne("EOF", "EOF", index, lineNumber, endOfFile);
                    printTK(currentTk);
                    return currentTk;
                    
                } else {
                    lineDone = true; // the line is complete will move on to next line
                }
                unsigned int tempIndex = index;
                if(tempIndex >= line.length()){
                    lineDone = true; // line is done when index reaches the end of string
                }
            }
            lineNumber++; // incrementing the line number
        }
    }
    Token currentTk = ScanLIne("EOF", "EOF", index, lineNumber, endOfFile); // this shouldn't run but incase it does it returns and eof token 
    printTK(currentTk);                                                        
    return currentTk;
}   