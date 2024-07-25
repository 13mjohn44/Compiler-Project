#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <vector>
// #include <bits/stdc++.h>
#include "token.h"

enum nonTerms{
    PARSER,
    PROGRAM,
    FUNC,
    BLOCK,
    VARS,
    EXPR,
    N_,
    Nprime_,
    A_,
    M_,
    R_,
    STATS,
    STAT,
    MSTAT,
    IN,
    OUT,
    IF_,
    PICK,
    PICKBODY,
    LOOP1,
    LOOP2,
    ASSIGN,
    GOTO_,
    R0_,
    LABEL,
    none
};




class nodeTK{
    public:
    nonTerms label;
    std::vector<int> printOrder;
    int tkIndex = 0;
    Token* tk[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
    int ntIndex = 0;
    nodeTK* children[4] = {nullptr, nullptr, nullptr, nullptr};
    //std::string offset = "";

    nodeTK();
    nodeTK(nonTerms);
    void pushTk(Token);
    void pushChild(nodeTK*);
    nonTerms getLabel();
    std::string printLabel();
    Token* getTk(int);
    nodeTK* getChild(int);
    void preOrder();
};

#endif //NODE_H