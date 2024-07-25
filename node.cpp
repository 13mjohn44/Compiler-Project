#include <iostream>
#include <vector>
#include "node.h"
#include "parser.h"

// these are the Labels of each non terminal as strings
std::string myLabels[26] = {
        "PARSER",
        "PROGRAM",
        "FUNC",
        "BLOCK",
        "VARS",
        "EXPR",
        "N_",
        "Nprime_",
        "A_",
        "M_",
        "R_",
        "STATS",
        "STAT",
        "MSTAT",
        "IN",
        "OUT",
        "IF_",
        "PICK",
        "PICKBODY",
        "LOOP1",
        "LOOP2",
        "ASSIGN",
        "GOTO_",
        "R0_",
        "LABEL",
        "none"
};


// constructor
nodeTK::nodeTK(){
    label = none;
}
// parameterized constructor
nodeTK::nodeTK(nonTerms non_terminal){
    label = non_terminal;
}
//adds a new token to the token array
void nodeTK::pushTk(Token token){
    // cout << "token consume: " << token.getInstance() << endl;
    // cout << "the tkindex is : " << tkIndex << endl;
    if(tkIndex < 5){ 
        printOrder.push_back(1);
        tk[tkIndex] = new Token(token.getID(), token.getInstance(), token.getLineNum(), token.GetCharNum());
        tkIndex++;
    }else{
        //cout << "2" << endl;
        //std::cout << "your pushing too many tokens in this node with tk " << token.getInstance() << std::endl;
    }
}


//returns the enumerated label of node
nonTerms nodeTK::getLabel(){
    return label;
}


//adds a new nonterminal child to the nonterminal array
void nodeTK::pushChild(nodeTK* nodeChild){
    if(nodeChild){ // makes sure you dont add more nonterms that what is allowed
        // cout << "child nonterm pushed: " << nodeChild->printLabel() << endl;
        printOrder.push_back(0);
        children[ntIndex] = nodeChild;
        ntIndex++;
    }
    else{
        //std::cout << printLabel() << " " << nodeChild->printLabel() << std::endl;
        //std::cout << "your pushing too many non-terminals in this node or this pointer is null" << std::endl;
    }
}
//returns the token child of the given index
Token* nodeTK::getTk(int index){
    return tk[index];
}

//returns the string version of the label
std::string nodeTK::printLabel(){
    std::string str = myLabels[label];
    return str;
}

//returns the nonterminal child of the given index
nodeTK* nodeTK::getChild(int index){
    return children[index];
}
