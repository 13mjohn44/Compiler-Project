#include <iostream>
#include <string>
#include <ctype.h>

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

using namespace std;


string GetBaseFileName (string);

// returns true if the given string is all whitespace
bool isOnlySpace(string);

// If standard input was given at program call, then it is processed here and then sent to a output.txt
string SendRedirectToFile (string);

// overload to start from a specific index in the string
bool isOnlySpace(string, int);

//Checks if a line contains comments
int ContainsComment(string);

//Removes any comments from a line
string RemoveComment(string);

#endif