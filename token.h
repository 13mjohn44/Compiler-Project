#include <iostream>
#include "FSA.h"
#ifndef TOKEN_H
#define TOKEN_H

using namespace std;

// struct for Tokens
struct Token{
    States tokenID; // represents the enumerated token type of the token (ie. IDTK, INTK ....)
    string instance;// holds the actual token
    string tempInst;
    int lineNum;    // holds the line its on
    int CharNum;    // holds the index of the starting character for this token

    // array of token types
    string tokens[28] = {
        "Identifier",
        "Integer",
        "Keyword",
        "Colon",
        "Assignment 2",
        "Assignment 1",
        "Greater",
        "Equivilent",
        "Lesser",
        "Plus",
        "Minus",
        "Multiplication",
        "Division",
        "Exclamation",
        "dot operator",
        "L Parentheses",
        "R Parentheses",
        "Comma",
        "L Curly Brace",
        "R Curly Brace",
        "Semicolon",
        "L Bracket",
        "R Bracket",
        "Caret",
        "Not Equivilent",
        "AND operator",
        "OR operator",
        "EOF"
    };

    public:
    // default constructor
    Token();
    // Token constructor
    Token(States, string, int, int); 
    // return enumerated token type
    States getID();
    // return string token type
    string getIDString();
    //returns char num
    int GetCharNum();
    // returns token itself
    string getInstance();
    // returns the token's line
    int getLineNum();
    // reassigns the tokens enumerated type
    void setTkID(States);

    void copyTk(Token tk);
    
    std::string getTempInst();
    
    void setTempInst(std::string);
};

#endif