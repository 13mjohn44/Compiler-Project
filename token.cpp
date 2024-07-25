#include <iostream>
#include "token.h"


using namespace std;

// default constructor
Token::Token(){
    tokenID = noState;
    instance = "\0";
    tempInst = "\0";
    lineNum = -1;
    CharNum = -1;

}

// Token constructor
Token::Token(States ID, string inst, int line, int index){
    tokenID = ID;
    instance = inst;
    tempInst = inst;
    lineNum = line;
    CharNum = index;
}
// reassigns the tokens enumerated type
void Token::setTkID(States st){
    tokenID = st;
} 
// return enumerated token type
States Token::getID(){
    return tokenID;
}
// return string token type
string Token::getIDString(){
    return tokens[tokenID - 1000];
}
// returns token itself
string Token::getInstance(){
    return instance;
}
// returns the token's line
int Token::getLineNum(){
    return lineNum;
}

int Token::GetCharNum(){
    return CharNum;
}

void Token::copyTk(Token tk){
    tokenID = tk.getID();
    instance = tk.getInstance();
    lineNum = tk.getLineNum();
}

std::string Token::getTempInst(){
    return tempInst;
}
void Token::setTempInst(std::string inst){
    tempInst = inst;
}