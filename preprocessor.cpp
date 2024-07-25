#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctype.h>
#include "preprocessor.h"

using namespace std;



//this will remove the extension from a filename
string GetBaseFileName (string fileName){
    size_t lastIndex = fileName.find_last_of("."); 
    string rawName = fileName.substr(0, lastIndex);

    return rawName;
}


//This function Checks if the entered string contains "//", meaning that it contains a comment
//it will return the index of the comment on that line
int ContainsComment(string line)
{   string s1 = "//";
    int M = s1.length();
    int N = line.length();

    for (int i = 0; i <= N - M; i++) {
        int j;
 
        // For current index i, check for pattern match
        for (j = 0; j < M; j++)
            if (line[i + j] != s1[j])
                break;
 
        if (j == M)
            return i;
    }
 
    return -1;
}

string SendRedirectToFile (string strings){
    ofstream outputFile;
    string fileName = "kb.txt";

    outputFile.open(fileName);

    if(!outputFile){
        cout << "Could not open the file, terminating" << endl;
        exit(0);
    }

    outputFile << strings;

    outputFile.close();
    
    return fileName;
}


// returns true if the given string is all whitespace
bool isOnlySpace(string line){
    for(int i = 0; i < line.length(); i++){
        if(!isspace(line[i]))
            return false;
    }
    return true;
}

//This function will remove any comments by calling the CoantainsComment function, and
// receiving the index of the comment if it has one. It then cuts off the rest of the line
//starting from that index, and then returns the cleaned line
string RemoveComment(string rawLine){
    int commentsIndex = ContainsComment(rawLine);
    string cleanedLine = "";

    if(commentsIndex > -1){
        for(unsigned int i = 0; i < commentsIndex; i++){
            cleanedLine += rawLine[i];
        }
    }else{
        return rawLine;
    }
    return cleanedLine;
}

// overload to start from a specific index in the string
bool isOnlySpace(string line, int index){
    for(unsigned int i = index; i < line.length(); i++){
        if(!isspace(line[i]))
            return false;
    }
    return true;
}
