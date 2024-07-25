#include <iostream>
#include "charProcess.h"


//returns the character value of the given character
Characters getCharVal(char lookup){
    if(isalpha(lookup))
        return letter;
    if(lookup == '^')
        return caret;
    if(isdigit(lookup))
        return digit;
    if(isspace(lookup))
        return WS;
    if(lookup == ':')
        return colon;
    if(lookup == '>')
        return greater;
    if(lookup == '=')
        return equal;
    if(lookup == '<')
        return lesser;
    if(lookup == '+')
        return plus;
    if(lookup == '-')
        return minus;
    if(lookup == '*')
        return star;
    if(lookup == '/')
        return slash;
    if(lookup == '!')
        return exclamation;
    if(lookup == '.')
        return dot;
    if(lookup == '(')
        return LParen;
    if(lookup == ')')
        return RParen;
    if(lookup == ',')
        return comma;
    if(lookup == '{')
        return LCurly;
    if(lookup == '}')
        return RCurly;
    if(lookup == ';')
        return semi;
    if(lookup == '[')
        return LBracket;
    if(lookup == ']')
        return RBracket;
    if(lookup == '&')
        return ampersand;
    if(lookup == '|')
        return verticalBar;
    if(lookup == '_')
        return underscore;
    return NoChar;
}

//Returns the character found and the given index int the given string
char getChar(std::string s, int index){
    return s[index];
}