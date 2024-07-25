#ifndef  PARSER_H
#define  PARSER_H

#include <stack>
#include "node.h"

using namespace std;


// vector <string> definedVars;

bool IsVarDefined(string);
void VarError(string, string);
void RefillStack(stack <string> tempStack);
void CheckScope(nodeTK *, int);
void printContentsStack();

// these are all the nonterminals for our parser.
nodeTK * parser(ifstream &);
nodeTK * Program(ifstream &);
nodeTK * func(ifstream &);
nodeTK * block(ifstream &);
nodeTK * vars(ifstream &);
nodeTK * expr(ifstream &);
nodeTK * N(ifstream &);
nodeTK * Nprime(ifstream &);
nodeTK * A(ifstream &);
nodeTK * M(ifstream &);
nodeTK * R(ifstream &);
nodeTK * stats(ifstream &);
nodeTK * mstat(ifstream &);
nodeTK * stat(ifstream &);
nodeTK * in(ifstream &);
nodeTK * out(ifstream &);
nodeTK * block(ifstream &);
nodeTK * If(ifstream &);
nodeTK * loop1(ifstream &);
nodeTK * loop2(ifstream &);
nodeTK * assign(ifstream &);
nodeTK * goto_(ifstream &);
nodeTK * label(ifstream &);
nodeTK * pick(ifstream &);
nodeTK * R0(ifstream &);
nodeTK * pickbody(ifstream &);
void printParseTree(nodeTK *);
int find(string, stack<string>&);

string treeDecider(nodeTK *, ofstream &, bool &, int);



#endif //  PARSER_H