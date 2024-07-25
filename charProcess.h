#include <iostream>

#ifndef CHARPROCESS_H
#define CHARPROCESS_H


//Character included: digit, letter(upper and lower), =  <  > == =!= : :=  +  -  *  /   ^  . (  ) , { } ; [ ] || && _

// enumeration for charaters for or the char value lookup
enum Characters{
    letter,
    caret,
    digit,
    WS,
    colon,
    greater,
    equal,
    lesser,
    plus,
    minus,
    star,
    slash,
    exclamation,
    dot,
    LParen,
    RParen,
    comma,
    LCurly,
    RCurly,
    semi,
    LBracket,
    RBracket,
    ampersand,
    verticalBar,
    underscore,
    _EOF,
    NoChar
};

// returns the integer value of char based off above enumeration
Characters getCharVal(char);

//Returns the character found and the given index int the given string
char getChar(std::string, int);

#endif