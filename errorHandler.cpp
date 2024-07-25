#include <iostream>
#include "errorHandler.h"
#include "FSA.h"

using namespace std;

// this just holds an array of possible error messages
// index would corrospond to the enumarted errors in :Stats;
string errorMessage[8]{
    "SCANNER ERROR: Identifier exceeded character limit: ",
    "SCANNER ERROR: Integer exceeded length limit: ",
    "SCANNER ERROR: Token begins with unacceptable character: ",
    "SCANNER ERROR: Unacceptable character found: ",
    "SCANNER ERROR: Interupted while parsing &&, expected \"&\" after \"&\", recived: ",
    "SCANNER ERROR: Interupted while parsing =!=, expected \"=\" after \"=!\", recived: ",
    "SCANNER ERROR: Interupted while parsing ||, expected \"|\" after \"|\", recived: "
};

// returns the error message with a given errorState (code)
string getError(States code){
    return errorMessage[((int)code * -1) - 1];
}

//returns true is the current state is an error
bool isError(States state){
    if(state < (States)0)
        return true;
    
    return false;
}
