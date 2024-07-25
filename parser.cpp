#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include "parser.h"
#include "token.h"
#include "Scanner.h"
#include "FSA.h"
#include "node.h"
Token tk;

stack <string> definedVars;

using namespace std;

//this function accepts an string for an expected token and then ends the program.
void error(string expectation){
    cout << "Parser Error: expected " << expectation << " but got " 
        << tk.getInstance() << " which is " << tk.getIDString() 
        << " at line " << tk.getLineNum() << endl;

    exit(-1);
}


nodeTK* parser(ifstream &file) {
    nodeTK * root;
    tk = scanner(file);      // initializing the root and pulling the first token
    root = Program(file);
    
    tk = scanner(file);
    if(tk.getID() == EOFTK){  // checking that it reached the end of the file
        return root;
    }else{
        error("EOF");
        return nullptr;
    }
    return nullptr;
}

nodeTK* Program(ifstream &file) {
    nodeTK * p = new nodeTK(PROGRAM);
    
    p->pushChild(vars(file));
    
    if(!tk.getInstance().compare("tape")){ // checks the rest of the programs main and block
        p->pushTk(tk);
        tk = scanner(file);

        p->pushChild(func(file));
        p->pushChild(block(file));
        return p;
    }else
    { 
        error("'tape'"); 
    }
    return nullptr;
}

nodeTK* func(ifstream &file) {
    if(!tk.getInstance().compare("func"))
    {
        nodeTK * p = new nodeTK(FUNC);
        p->pushTk(tk);
        tk = scanner(file);
        if(tk.getID() == ID_TK){
            p->pushTk(tk);
            tk = scanner(file);
            p->pushChild(block(file));
            return p;
        }else{
            error("'identifier'");
        }
    }
    else 
    {
        return nullptr;
    }
    return nullptr;
}

nodeTK* block(ifstream &file) {
    nodeTK * p = new nodeTK(BLOCK);
    if(tk.getID() == LCURTK){ // processing the block
        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(vars(file));
        p->pushChild(stats(file));
        if(tk.getID() == RCURTK){ 
            p->pushTk(tk);
            tk = scanner(file);
            return p;
        }else{ error("'}'"); }
    }else{ error("'{'"); }
    return nullptr;
}




nodeTK* vars(ifstream &file) {
    if(!tk.getInstance().compare("create")){ // validating start of vars
        nodeTK * p = new nodeTK(VARS);  
        p->pushTk(tk);
        tk = scanner(file);
        if(tk.getID() == ID_TK){ // process identifier
            p->pushTk(tk);
            tk = scanner(file);
            if(tk.getID() == ASN2TK){ // process :=
                p->pushTk(tk);
                tk = scanner(file);
                if(tk.getID() == INT_TK){ // process integer
                    p->pushTk(tk);
                    tk = scanner(file);
                    if(tk.getID() == SEM_TK){ // process semicolon and check for start of another var
                        p->pushTk(tk);
                        tk = scanner(file);
                        p->pushChild(vars(file));
                        return p;
                    }else {
                        error("';'"); 
                    }
                }else {
                    error("integer"); 
                }
            } else if(tk.getID() == SEM_TK){
                p->pushTk(tk);
                tk = scanner(file);
                return p;
            } else {
                error("':=' or ';'"); 
            }
        }else {error("identifier"); }
    }
    else{
        return nullptr; // returns null incase there are no vars
    }
    return nullptr;
}


nodeTK * expr(ifstream &file){
    nodeTK * p = new nodeTK(EXPR);
    p->pushChild(N(file));

    if(tk.getID() == SUB_TK){
        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(expr(file));
        return p;
    }else{
        return p;
    }
    return nullptr;
}

nodeTK * N(ifstream &file){
    nodeTK * p = new nodeTK(N_);
    
    p->pushChild(A(file));
    p->pushChild(Nprime(file));
    return p;
}

nodeTK * Nprime(ifstream &file){

    if(tk.getID() == DIV_TK || tk.getID() == PLU_TK){
        nodeTK * p = new nodeTK(Nprime_);
        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(A(file));
        p->pushChild(Nprime(file));
        return p;
    }
    else
    {
        return nullptr;
    }
    return nullptr;
}

nodeTK * A(ifstream &file){
    nodeTK * p = new nodeTK(A_);

    p->pushChild(M(file));
    if(tk.getID() == MUL_TK){
        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(A(file));
        return p;
    }
    return p;
}

nodeTK * M(ifstream &file){
    nodeTK * p = new nodeTK(M_);

    if(tk.getID() == CRT_TK){
        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(M(file));
        return p;
    }else{
        p->pushChild(R(file));
        return p;
    }
    return nullptr;
}

nodeTK * R(ifstream &file){
    nodeTK * p = new nodeTK(R_);
    
    if(tk.getID() == LPARTK){
        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(expr(file));
        
        if(tk.getID() == RPARTK){
            p->pushTk(tk);
            tk = scanner(file);
            return p;
        }else{
            error("')'");
        }

    }else if(tk.getID() == ID_TK || tk.getID() == INT_TK){
        p->pushTk(tk);
        tk = scanner(file);
        return p;
    }else{
        error("'(' or 'identifier' or 'integer'");
    }
    return nullptr;
}

nodeTK * stats(ifstream &file){
    nodeTK * p = new nodeTK(STATS);

    p->pushChild(stat(file));
    p->pushChild(mstat(file));
    return p;
}

nodeTK * mstat(ifstream &file){

    if(!tk.getInstance().compare("cin")){
        nodeTK * p = new nodeTK(MSTAT);
        p->pushChild(stat(file));
        p->pushChild(mstat(file));
        return p;
    }else if(!tk.getInstance().compare("cout")){
        nodeTK * p = new nodeTK(MSTAT);
        p->pushChild(stat(file));
        p->pushChild(mstat(file));
        return p; 
    }else if(tk.getID() == LCURTK){
        nodeTK * p = new nodeTK(MSTAT);
        p->pushChild(stat(file));
        p->pushChild(mstat(file));
        return p;
    }else if(!tk.getInstance().compare("if")){
        nodeTK * p = new nodeTK(MSTAT);
        p->pushChild(stat(file));
        p->pushChild(mstat(file));
        return p;
    }else if(!tk.getInstance().compare("while")){
        nodeTK * p = new nodeTK(MSTAT);
        p->pushChild(stat(file));
        p->pushChild(mstat(file));
        return p;
    }else if(!tk.getInstance().compare("repeat")){
        nodeTK * p = new nodeTK(MSTAT);
        p->pushChild(stat(file));
        p->pushChild(mstat(file));
        return p;
    }else if(!tk.getInstance().compare("set")){
        nodeTK * p = new nodeTK(MSTAT);
        p->pushChild(stat(file));
        p->pushChild(mstat(file));
        return p;
    }else if(!tk.getInstance().compare("jump")){
        nodeTK * p = new nodeTK(MSTAT);
        p->pushChild(stat(file));
        p->pushChild(mstat(file));
        return p;
    }else if(!tk.getInstance().compare("label")){
        nodeTK * p = new nodeTK(MSTAT);
        p->pushChild(stat(file));
        p->pushChild(mstat(file));
        return p;
    }else if(!tk.getInstance().compare("pick")){
        nodeTK * p = new nodeTK(MSTAT);
        p->pushChild(stat(file));
        p->pushChild(mstat(file));
        return p;
    }else{
        return nullptr; 
    }
    return nullptr;
}

nodeTK * stat(ifstream &file){
    nodeTK * p = new nodeTK(STAT);

    p->pushChild(in(file));
    p->pushChild(out(file));
    if(tk.getID() == LCURTK)
    {
        p->pushChild(block(file));
        return p;
    }
    p->pushChild(If(file));
    p->pushChild(loop1(file));
    p->pushChild(loop2(file));
    p->pushChild(assign(file));
    p->pushChild(goto_(file));
    p->pushChild(label(file));
    p->pushChild(pick(file));
    
    if(tk.getID() == SEM_TK){
        p->pushTk(tk);
        tk = scanner(file);
        return p;
    }else{
        error("';'");
    }
    return nullptr;
}

nodeTK * in(ifstream &file){
    if(!tk.getInstance().compare("cin")){
        nodeTK * p = new nodeTK(IN);

        p->pushTk(tk);
        tk = scanner(file);
        if(tk.getID() == ID_TK){
            p->pushTk(tk);
            tk = scanner(file);
            return p;
        }else{
            error("'indentifier'");
        }
    }else{
        return nullptr;
    }
    return nullptr;
}

nodeTK * out(ifstream &file){
    if(!tk.getInstance().compare("cout")){
        nodeTK * p = new nodeTK(OUT);

        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(expr(file));
        return p;
    }else{
        return nullptr;
    }
    return nullptr;
}

nodeTK * If(ifstream &file){
    if(!tk.getInstance().compare("if")){
        nodeTK * p = new nodeTK(IF_);

        p->pushTk(tk);
        tk = scanner(file);
        if(tk.getID() == LBRATK){
            p->pushTk(tk);
            tk = scanner(file);
            p->pushChild(expr(file));
            p->pushChild(R0(file));
            p->pushChild(expr(file));
            if(tk.getID() == RBRATK){
                p->pushTk(tk);
                tk = scanner(file);
                if(!tk.getInstance().compare("then")){
                    p->pushTk(tk);
                    tk = scanner(file);
                    p->pushChild(stat(file));
                    return p;
                }else{
                    error("'then'");
                }
            }else{
                error("']'");
            }
        }else{
            error("'['");
        }
    }else
    {
        return nullptr;
    }
    return nullptr;
}

nodeTK * loop1(ifstream &file){
    if(!tk.getInstance().compare("while")){
        nodeTK * p = new nodeTK(LOOP1);
        p->pushTk(tk);
        tk = scanner(file);
        if(tk.getID() == LBRATK){
            p->pushTk(tk);
            tk = scanner(file);
            p->pushChild(expr(file));
            p->pushChild(R0(file));
            p->pushChild(expr(file));
            if(tk.getID() == RBRATK){
                p->pushTk(tk);
                tk = scanner(file);
                p->pushChild(stat(file));
                return p;
            }else{
                error("']'");
            }
        }else{
            error("'['");
        }
    }else{
        return nullptr;
    }
    return nullptr;
}


nodeTK * loop2(ifstream &file){
    if(!tk.getInstance().compare("repeat")){
        nodeTK * p = new nodeTK(LOOP2);

        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(stat(file));

        if(!tk.getInstance().compare("until")){
            p->pushTk(tk);
            tk = scanner(file);
            if(tk.getID() == LBRATK){
                p->pushTk(tk);
                tk = scanner(file);
                p->pushChild(expr(file));
                p->pushChild(R0(file));
                p->pushChild(expr(file));
                if(tk.getID() == RBRATK){
                    p->pushTk(tk);
                    tk = scanner(file);
                    return p;
                }else{
                    error("']'");
                }
            }else{
                error("'['");
            }
        }else{
            error("'until'");
        }
    }else{
        return nullptr;
    }
    return nullptr;
}


nodeTK * assign(ifstream &file){
    bool nodeCreated = false;
    nodeTK * p = nullptr;
    if(!tk.getInstance().compare("set")){
        p = new nodeTK(ASSIGN);
        p->pushTk(tk);
        tk = scanner(file);
        nodeCreated = true;
    }
    if(tk.getID() == ID_TK){
        if(nodeCreated == false){
            p = new nodeTK(ASSIGN);
        }
        p->pushTk(tk);
        tk = scanner(file);
        if(tk.getID() == ASN_TK){
            p->pushTk(tk);
            tk = scanner(file);
            p->pushChild(expr(file));
            
            return p;
        }else{
            error("'='");
        }
    }else{
        return nullptr;
    }
    return nullptr;
}

nodeTK * R0(ifstream &file){
    nodeTK * p = new nodeTK(R0_);
    
    if(tk.getID() == GTR_TK){
        p->pushTk(tk);
        tk = scanner(file);
        return p;
    }else if(tk.getID() == LES_TK){
        p->pushTk(tk);
        tk = scanner(file);
        return p;
    }else if(tk.getID() == EQV_TK){
        p->pushTk(tk);
        tk = scanner(file);
        return p;
    }else if(tk.getID() == NEQVTK){
        p->pushTk(tk);
        tk = scanner(file);
        return p;
    }else if(tk.getID() == DOT_TK){
        p->pushTk(tk);
        tk = scanner(file);
        if(tk.getID() == DOT_TK){
            p->pushTk(tk);
            tk = scanner(file);
            if(tk.getID() == DOT_TK){
                p->pushTk(tk);
                tk = scanner(file);
                return p;
            }else{
                error("'.'");
            }
        }else{
            error("'.'");
        }
    }else{
        error("'<' or '>' or '==' or '=!=' or'.'");
    }
    return nullptr;
}


nodeTK * label(ifstream &file){
    if(!tk.getInstance().compare("label")){
        nodeTK * p = new nodeTK(LABEL);
        p->pushTk(tk);
        tk = scanner(file);
        if(tk.getID() == ID_TK){
            p->pushTk(tk);
            tk = scanner(file);
            return p;
        }else{
            error("'identifier'");
        }
    }else{
        return nullptr;
    }
    return nullptr;
}


nodeTK * goto_(ifstream &file){
    if(!tk.getInstance().compare("jump")){
        nodeTK * p = new nodeTK(GOTO_);

        p->pushTk(tk);
        tk = scanner(file);
        if(tk.getID() == ID_TK){
            p->pushTk(tk);
            tk = scanner(file);
            return p;
        }else{
            error("'identifier'");
        }
    }else{
        return nullptr;
    }
    return nullptr;
}

nodeTK * pick(ifstream &file){
    if(!tk.getInstance().compare("pick")){
        nodeTK * p = new nodeTK(PICK);
        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(expr(file));
        p->pushChild(pickbody(file));
        return p;
    }else{
        return nullptr;
    }
    return nullptr;
}

nodeTK * pickbody(ifstream &file){
    nodeTK * p = new nodeTK(PICKBODY);

    p->pushChild(stat(file));
    if(tk.getID() == CLN_TK){
        p->pushTk(tk);
        tk = scanner(file);
        p->pushChild(stat(file));
        return p;
    }else{
        error("':'");
    }
    return nullptr;
}


void printParseTree(nodeTK * root){
    // this is the offset that will appear before each token and nonterminal
    static string offset = "";

    // first display the offset and nonterminal
    cout << offset << root->printLabel() << endl;
    
    //determining whether the last child of a specific node has 
    // been reached and changes the offset depending on that
    if(offset.length() > 1){
        offset.pop_back();

        if(offset.back() == '`') { 
            offset.pop_back();
            offset += " ";
        }
        offset += " ";
    }

    // keeping track the index for each child array
    int tokenIndex = 0;
    int childIndex = 0;

    // this is where the printing happens
    for(unsigned int i = 0; i < root->printOrder.size(); i++){
        string append = "| ";
        if((root->printOrder.size() - i) < 2){ // checks for the last child
            append = "  ";
        }

        if(root->printOrder.at(i) == 1){ // for terminals
            offset += append;
            offset.pop_back(); // making a custom tip for the offset
            offset.pop_back(); 
            if((root->printOrder.size() - i) < 2){
                offset += "`-";
            }else { offset += "|-"; }
            cout << offset << root->getTk(tokenIndex)->getInstance() << endl;

            tokenIndex++;
        }else if(root->printOrder.at(i) == 0){ // for nonterminal
            offset += append;
            offset.pop_back();
            offset.pop_back();
            if((root->printOrder.size() - i) < 2){
                offset += "`-";
            }else { offset += "|-"; }
            //cout << "offset is currently " << offset << endl; 
            printParseTree(root->getChild(childIndex));
            
            childIndex++;
        }else{
            cout << "something went wrong" << endl; // this should never happen
        }
        offset.pop_back(); // erase the custom tip
        offset.pop_back();
    }
}


// this function searches definedVars stack for the entered string. It returns the distance from the
// top of the stack if found and -1 if not
int find(string varInstance){
    int distanceFromTOS = 0;
    stack <string> tempStack;
    const unsigned SIZE = definedVars.size();

    for(unsigned i = 0; i < SIZE; i++){

        if(definedVars.top().compare(varInstance) == 0){
            RefillStack(tempStack);
            return distanceFromTOS;
        }else {
            tempStack.push(definedVars.top());
            definedVars.pop();
            distanceFromTOS++;
            
            if(definedVars.empty() == true){
                RefillStack(tempStack);
                return -1;
            }
        }

    }
    return -1;
}

//This accepts a stack that was used to temporarily store the contents of another stack and refills the original stack
void RefillStack(stack <string> tempStack){
    while(!tempStack.empty()){
        definedVars.push(tempStack.top());
        tempStack.pop();
    }
}


int numBlocks = 0;
stack <int> varCountStack;
map <string, bool> varsMap;

//This function checks that variables are used and created correctly. It considers both scope and definition
void CheckScope(nodeTK * root, int outsideFuncVarCount){
    bool skip = true;
    // first display the offset and nonterminal
    // cout << offset << root->printLabel() << endl;
    static int varCount = 0;

    // keeping track the index for each child array
    int tokenIndex = 0;
    int childIndex = 0;

    // this is where the printing happens
    for(unsigned int i = 0; i < root->printOrder.size(); i++){
        if(root->printOrder.at(i) == 1){ // for terminals

            // cout << root->getTk(tokenIndex)->getInstance() << endl;

            //check if we are dealing with identifiers
            if(root->getTk(tokenIndex)->getID() == ID_TK){
                bool expectDefined = true;
                if(tokenIndex > 0 && (root->getTk(tokenIndex-1)->getInstance().compare("create") == 0 || root->getTk(tokenIndex-1)->getInstance().compare("func") == 0)){//deal with creation of variables here
                    if (find(root->getTk(tokenIndex)->getInstance()) == -1)
                    {
                        definedVars.push(root->getTk(tokenIndex)->getInstance());
                        if(root->getTk(tokenIndex-1)->getInstance().compare("create") == 0){
                            varsMap[root->getTk(tokenIndex)->getInstance()] = false;
                            expectDefined = false;
                        }else{
                            varsMap[root->getTk(tokenIndex)->getInstance()] = true;
                            expectDefined = true;
                        }
                        varCount++;
                    }
                    else
                    {
                        // error: Already exists
                        VarError("AE", root->getTk(tokenIndex)->getInstance());
                    }
                }

                //we check for assignment after creation here with :=, =, and cin and set our var to be defined if so
                if(root->tkIndex > (tokenIndex + 1)){
                    if(root->getTk(tokenIndex+1)->getID() == ASN2TK || root->getTk(tokenIndex+1)->getID() == ASN_TK){
                        if(find(root->getTk(tokenIndex)->getInstance()) == -1){
                            VarError("NE", root->getTk(tokenIndex)->getInstance());
                        }
                        varsMap[root->getTk(tokenIndex)->getInstance()] = true;
                        expectDefined = true;
                    }
                }else if(tokenIndex > 0){
                    if(root->getTk(tokenIndex-1)->getInstance().compare("cin") == 0){
                        if(find(root->getTk(tokenIndex)->getInstance()) == -1){
                            VarError("NE", root->getTk(tokenIndex)->getInstance());
                        }
                        varsMap[root->getTk(tokenIndex)->getInstance()] = true;
                    }
                }

                if(outsideFuncVarCount > 0 && (varCount - outsideFuncVarCount - 1) < find(root->getTk(tokenIndex)->getInstance())){
                    // seems we are in a function and we encounted an indentifier created beyond our scope
                    expectDefined = false;
                }
                //deal with solo identifiers here and check if they are created and defined
                if(find(root->getTk(tokenIndex)->getInstance()) <= varCount && find(root->getTk(tokenIndex)->getInstance()) > -1){
                    if(tokenIndex > 0){
                        if(root->getTk(0)->getInstance().compare("label") == 0 || root->getTk(0)->getInstance().compare("jump") == 0){
                            skip = true;
                        }
                    }

                    if(varsMap[root->getTk(tokenIndex)->getInstance()] == false && expectDefined && skip == false){
                        VarError("ND", root->getTk(tokenIndex)->getInstance());
                    }else{
                        skip = false;
                    }
                }else{
                    // cout << find(root->getTk(tokenIndex)->getInstance());
                    VarError("NE", root->getTk(tokenIndex)->getInstance());
                }
            }
            
            tokenIndex++;
        }else if(root->printOrder.at(i) == 0){ // for nonterminal
            bool newBlock = false;
            bool enteredFunc = false;
            //we check if we entered a new block and then push our currrent var count into the stack
            if(root->getChild(childIndex)->getLabel() == BLOCK){

                if(root->tkIndex > 0){
                    if(root->getTk(0)->getInstance().compare("func") == 0)
                    {
                        outsideFuncVarCount = varCount;
                        enteredFunc = true;
                    }
                }
                newBlock = true;

                varCountStack.push(varCount);
            }
            
            //recursivly call our function to parse the tree just like when we print the tree
            CheckScope(root->getChild(childIndex), outsideFuncVarCount);
            
            //check to see if we entered function so we can deal with use of varibles that can be defined after function
            if(enteredFunc)
            {
                outsideFuncVarCount = 0;
            }

            //We know we are now leaving a block so we remove the proper values from our stack
            if(newBlock)
            {
                int varsToPop = varCount - varCountStack.top();
                for(int i = 0; i < varsToPop; i++)
                {
                    varsMap.erase(definedVars.top());
                    // cout << "popping: " << definedVars.top() << endl;
                    definedVars.pop();
                    varCount--;
                }
            }
            
            childIndex++;
        }else{
            cout << "something went wrong" << endl; // this should never happen
        }
    }
}


// the determins if the specified variable name alread exist within the given vector of variable names
bool exist(string id, vector<Token*> &vector){
    for(unsigned int i = 0; i < vector.size(); i++){
        if(vector.at(i)->getInstance() == id){
            return true;
        }
    }
    
    return false; // return index -1 if item was not found
}
//finds the correct instance of a specified variable
string findMyScope(string id, vector<Token*> &vector){
    for(int i = (vector.size() - 1); i >= 0; i--){ // traverse the vector backwards
        if(vector.at(i)->getInstance() == id){     // if the there is a match the return the asm name of that var
            return vector.at(i)->getTempInst();
        }
    }
    return id; // return itself if no previous one was found. this shouldn't ever happen though
}

static int LabelCntr=0; // counting unique labels generated
static int VarCntr=0; // counting unique temporaries generated
enum nameType{VAR, LAB};
static string name; // for creation of unique names
static vector<string> asmVars; // basically the history of all created vars asm wise
static map<string, string> asmVarsValues;// this contains values set after var creation, later if a Var is not is this list we will set it to -7

//We can generate new labels using the format V#, and labels using the format T#
string newName(nameType what){ 
    if (what==VAR){ // creating new temporary
        name = "V" + to_string(VarCntr++); 
        asmVars.push_back(name);
    }else{ // creating new Label
        name = "T" + to_string(LabelCntr++); 
    }
    return(name);
}

map <string, nodeTK*> funcPointers;

string treeDecider(nodeTK * root, ofstream &asmFile, bool & status, int outsideFuncVarCount){
    string value = "";

    static stack<string> idStack;
    static vector<Token*> idTkVector;
    static int recursionDepth = 0;
    static vector<int> scopeCounts{0};
    bool printedNode = false;

    // keeping track the index for each child array
    int tokenIndex = 0;
    int childIndex = 0;
    

    recursionDepth++;
    // this is where the printing happens
    for(unsigned int i = 0; i < root->printOrder.size(); i++){
        if(root->printOrder.at(i) == 1){ // for terminals

            //check if we are dealing with identifiers
            if(!printedNode){
                // cout << "our current label intance is: " <<  root->getLabel() << endl;
                switch (root->getLabel())
                {
                    case VARS:{ // creating variables 
                        
                        if(exist(root->getTk(1)->getInstance(), idTkVector)){ // first need to see if the variable name exists in an earlier scope
                            root->getTk(1)->setTempInst("V" + to_string(VarCntr));
                            VarCntr++;
                            asmVars.push_back(root->getTk(1)->getTempInst());
                        } //if it does add a unique suffix to the tempInstance so that all vars have distinct names in asm file
                        else{

                            idTkVector.push_back(root->getTk(1));
                            asmVars.push_back(root->getTk(1)->getTempInst());
                        }

                        if( root->getTk(2)->getID() == ASN2TK ){
                            asmVarsValues[root->getTk(1)->getTempInst()] = root->getTk(3)->getTempInst();
                            
                        }
                        printedNode = true; // prevents duplicate printing of the same node
                    }
                        break;
                    case FUNC: { // getting vars
                        string label = root->getTk(1)->getTempInst();
                        funcPointers[label] = root->getChild(0);
                        printedNode = true;
                        break;
                    }
                    case IN: { // getting vars
                        string arg1 = root->getTk(1)->getTempInst();
                        arg1 = findMyScope(arg1, idTkVector);  // need to get correct instance of var(find which scope its reffering to)
                        asmFile << "READ " << arg1 << "\n"; 
                        printedNode = true;
                        break;
                    }
                        
                    case OUT: { // display values
                        string arg1 = treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount); // find correct instance of variable
                        asmFile << "WRITE " << arg1 << "\n"; 
                        printedNode = true;
                        break;
                    }
                        

                    case Nprime_: {
                        
                        string arg0 = newName(VAR);
                        asmFile << "STORE " << arg0 << endl;
                        string arg1 = treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount);

                        asmFile << "LOAD " << arg0 << endl;
                        if (root->getTk(0)->getID() == DIV_TK) {
                            asmFile << "DIV ";
                        }
                        else {
                            asmFile << "ADD ";
                        }

                        asmFile << arg1 << endl;

                        string arg3 = newName(VAR);
                        asmFile << "STORE " << arg3 << "\n";
                        asmFile << "LOAD " << arg3 << "\n";

                        if(root->ntIndex > 1){
                            treeDecider(root->getChild(1), asmFile, status, outsideFuncVarCount); 
                        }

                        
                        value = arg3;
                        
                       
                        printedNode = true;
                        break;
                    }

                    case M_: { // display values
                        string arg1 = treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount); 

                        string ZeroVal = newName(LAB);
                        string OneVal = newName(LAB);
                        string Done = newName(LAB);

                        asmFile << "LOAD " << arg1 << "\n";  
                        asmFile << "BRZERO " << ZeroVal << "\n";
                        asmFile << "SUB 1\n";
                        asmFile << "BRZERO " << OneVal << "\n";
                        asmFile << "LOAD " << arg1 << "\n";
                        asmFile << "MULT -1\n";
                        asmFile << "BR " << Done << "\n";
                        asmFile << ZeroVal << ": LOAD 1\n";
                        asmFile << "BR " << Done << "\n";
                        asmFile << OneVal << ": LOAD 0\n";
                            
                        string arg2 = newName(VAR);    // storing the result in a new temp variable
                        asmFile << Done << ": STORE " << arg2 << "\n";
                        value = arg2;
                        printedNode = true;
                        break;
                    }

                    case R_: {
                        if(root->ntIndex > 0){ // if this is a (<expr>) situation then process the expr
                            string arg1 = treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount);
                            value = arg1;
                        }else{ // if its just a variale or int, then just return the string of it
                        
                            value = root->getTk(0)->getTempInst();
                            if(root->getTk(0)->getID() == ID_TK){
                                value = findMyScope(value, idTkVector); // if its a variable find correct instance.
                            }
                        }
                        printedNode = true;
                    }
                        break;
                    case ASSIGN: { // assignment 

                        string arg1 = treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount);
                        asmFile << "LOAD " << arg1 << endl; // load the value of the expr

                        //we check if we have a set beforehand so that we correctly grab the IDTK
                        if(root->getTk(0)->getInstance().compare("set") == 0){
                            string arg2 = root->getTk(1)->getTempInst(); // get the ID TK after set
                            arg2 = findMyScope(arg2, idTkVector); // get the right instance
                            asmFile << "STORE " << arg2 << "\n"; // store that value in instace
                        }else{
                            string arg2 = root->getTk(0)->getTempInst(); // get the variable
                            arg2 = findMyScope(arg2, idTkVector); // get the right instance
                            asmFile << "STORE " << arg2 << "\n"; // store that value in instace
                        }
                        
                        printedNode = true;
                    }
                        break;
                    case LABEL: { // generating a label
                        string arg1 = root->getTk(1)->getTempInst(); // first getting the variable
                        arg1 = findMyScope(arg1, idTkVector); // then getting the correct instance of it
                        asmFile << arg1 << ": NOOP\n"; // writing the variable as a label to asm    
                        printedNode = true;
                    }
                        break;
                    case GOTO_: { // jump to label
                        string arg1 = root->getTk(1)->getTempInst();
                        if(funcPointers.count(arg1) > 0){
                            treeDecider(funcPointers[arg1], asmFile, status, outsideFuncVarCount);
                        }else{
                            arg1 = findMyScope(arg1, idTkVector); // getting the correct instance of the variable
                            asmFile << "BR " << arg1 << "\n"; // writing a break to the label
                        }
                        
                        printedNode = true;
                    }
                        break;
                    case R0_: { // determining the relational operator
                        if(root->getTk(0)->getID() == GTR_TK){ value = "MOR"; }
                        else if(root->getTk(0)->getID() == EQV_TK){ value = "EQV"; }
                        else if(root->getTk(0)->getID() == LES_TK){ value = "LES"; }
                        else if(root->getTk(0)->getID() == DOT_TK){ value = "EVENODD"; }
                        else { value = "NOTEQV"; }
                    }
                        break;
                    case IF_: { // if statement
                        string arg1 = treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount); // getting the val of the first expr
                        string cond = treeDecider(root->getChild(1), asmFile, status, outsideFuncVarCount); // getting the operator
                        string arg2 = treeDecider(root->getChild(2), asmFile, status, outsideFuncVarCount); // getting the val of the second expr
                        
                        
                        string lab1 = newName(LAB); // generating a label to jump too if condition not met
                        
                        //in the asm language we check for the opposite and jump if true, skiping the expr
                        if(!cond.compare("MOR")){ 
                            asmFile << "LOAD " << arg1 << "\n"; // load the first expression
                            asmFile << "SUB " << arg2 << "\n"; // subing the arg2 from the acc
                            asmFile << "BRZNEG " << lab1 << "\n"; // breaks for greater op
                        } 
                        else if(!cond.compare("EQV")){ // breaks for equivalent op
                            asmFile << "LOAD " << arg1 << "\n"; // load the first expression
                            asmFile << "SUB " << arg2 << "\n"; // subing the arg2 from the acc
                            asmFile << "BRNEG " << lab1 << "\n"; 
                            asmFile << "BRPOS " << lab1 << "\n"; 
                        } 
                        else if(!cond.compare("LES")){ 
                            asmFile << "LOAD " << arg1 << "\n"; // load the first expression
                            asmFile << "SUB " << arg2 << "\n"; // subing the arg2 from the acc
                            asmFile << "BRZPOS " << lab1 << "\n"; // breaks for less op
                        } 
                        else if(!cond.compare("NOTEQV")){ 
                            asmFile << "LOAD " << arg1 << "\n"; // load the first expression
                            asmFile << "SUB " << arg2 << "\n"; // subing the arg2 from the acc
                            asmFile << "BRZERO " << lab1 << "\n"; // break not equal op
                        } 
                        else{ // breaks for "..." op 
                            string lab2 = newName(LAB);
                            string notNeg = newName(LAB);
                            string sub2 = newName(LAB);
                            string prevEven = newName(LAB);
                            string prevOdd = newName(LAB);
                            string notNeg1 = newName(LAB);
                            string sub2again = newName(LAB);
                            string notNeg2 = newName(LAB);
                            string sub2again3 = newName(LAB);
                            string fail = newName(LAB);
                            string bothSame = newName(LAB);
                            

                            asmFile << "LOAD " << arg1 << "\n";
                            asmFile << "BRPOS " << notNeg << "\n";
                            asmFile << "MULT -1\n";
                            asmFile << notNeg << ": NOOP\n";
                            asmFile << sub2 << ": SUB 2\n";
                            asmFile << "BRPOS " << sub2 << "\n";
                            asmFile << "BRZERO " << prevEven << "\n";
                            asmFile << "BRNEG " << prevOdd << "\n";

                            asmFile << prevEven << ": LOAD " << arg2 << "\n";
                            asmFile << "BRPOS " << notNeg1 << "\n";
                            asmFile << "MULT -1\n";
                            asmFile << notNeg1 << ": NOOP\n";
                            asmFile << sub2again << ": SUB 2\n";
                            asmFile << "BRPOS " << sub2again << "\n";
                            asmFile << "BRZERO " << bothSame << "\n";
                            asmFile << "BRNEG " << lab1 << "\n";

                            asmFile << prevOdd << ": LOAD " << arg2 << "\n";
                            asmFile << "BRPOS " << notNeg2 << "\n";
                            asmFile << "MULT -1\n";
                            asmFile << notNeg2 << ": NOOP\n";
                            asmFile << sub2again3 << ": SUB 2\n";
                            asmFile << "BRPOS " << sub2again3 << "\n";
                            asmFile << "BRZERO " << lab1 << "\n";
                            asmFile << "BRNEG " << bothSame << "\n";

                            asmFile << bothSame << ": NOOP\n"; // if both values are the same then we jump infront of the block and let it continue
                        }
                        treeDecider(root->getChild(3), asmFile, status, outsideFuncVarCount);    //the statements in the if statement
                        
                        asmFile << lab1 << ": NOOP\n"; // the label for skipping the instructions
                        printedNode = true;
                    }
                        break;
                    case LOOP1:{
                        string lab0 = newName(LAB);
                        string arg1 = treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount); // getting the val of the first expr
                        string cond = treeDecider(root->getChild(1), asmFile, status, outsideFuncVarCount); // getting the operator
                        string arg2 = treeDecider(root->getChild(2), asmFile, status, outsideFuncVarCount); // getting the val of the second expr
                        string lab1 = newName(LAB); // generating a label to jump too if condition not met

                        asmFile << lab0 << ": NOOP\n";   // like this: this is the label to jump back to the top of the loop

                        //in the asm language we check for the opposite and jump if true, skiping the expr
                        if(!cond.compare("MOR")){ 
                            asmFile << "LOAD " << arg1 << "\n"; // load the first expression
                            asmFile << "SUB " << arg2 << "\n"; // subing the arg2 from the acc
                            asmFile << "BRZNEG " << lab1 << "\n"; // breaks for greater op
                        } 
                        else if(!cond.compare("EQV")){ // breaks for equivalent op
                            asmFile << "LOAD " << arg1 << "\n";  // load the first expression
                            asmFile << "SUB " << arg2 << "\n"; // subing the arg2 from the acc
                            asmFile << "BRNEG " << lab1 << "\n"; 
                            asmFile << "BRPOS " << lab1 << "\n"; 
                        } 
                        else if(!cond.compare("LES")){ 
                            asmFile << "LOAD " << arg1 << "\n"; // load the first expression
                            asmFile << "SUB " << arg2 << "\n"; // subing the arg2 from the acc
                            asmFile << "BRZPOS " << lab1 << "\n"; // breaks for less op
                        } 
                        else if(!cond.compare("NOTEQV")){ 
                            asmFile << "LOAD " << arg1 << "\n"; // load the first expression
                            asmFile << "SUB " << arg2 << "\n"; // subing the arg2 from the acc
                            asmFile << "BRZERO " << lab1 << "\n"; // break not equal op
                        } 
                        else{ // breaks for "..." op 
                            string lab2 = newName(LAB);
                            string notNeg = newName(LAB);
                            string sub2 = newName(LAB);
                            string prevEven = newName(LAB);
                            string prevOdd = newName(LAB);
                            string notNeg1 = newName(LAB);
                            string sub2again = newName(LAB);
                            string notNeg2 = newName(LAB);
                            string sub2again3 = newName(LAB);
                            string fail = newName(LAB);
                            string bothSame = newName(LAB);
                            

                            asmFile << "LOAD " << arg1 << "\n";
                            asmFile << "BRPOS " << notNeg << "\n";
                            asmFile << "MULT -1\n";
                            asmFile << notNeg << ": NOOP\n";
                            asmFile << sub2 << ": SUB 2\n";
                            asmFile << "BRPOS " << sub2 << "\n";
                            asmFile << "BRZERO " << prevEven << "\n";
                            asmFile << "BRNEG " << prevOdd << "\n";

                            asmFile << prevEven << ": LOAD " << arg2 << "\n";
                            asmFile << "BRPOS " << notNeg1 << "\n";
                            asmFile << "MULT -1\n";
                            asmFile << notNeg1 << ": NOOP\n";
                            asmFile << sub2again << ": SUB 2\n";
                            asmFile << "BRPOS " << sub2again << "\n";
                            asmFile << "BRZERO " << bothSame << "\n";
                            asmFile << "BRNEG " << lab1 << "\n";

                            asmFile << prevOdd << ": LOAD " << arg2 << "\n";
                            asmFile << "BRPOS " << notNeg2 << "\n";
                            asmFile << "MULT -1\n";
                            asmFile << notNeg2 << ": NOOP\n";
                            asmFile << sub2again3 << ": SUB 2\n";
                            asmFile << "BRPOS " << sub2again3 << "\n";
                            asmFile << "BRZERO " << lab1 << "\n";
                            asmFile << "BRNEG " << bothSame << "\n";

                            asmFile << bothSame << ": NOOP\n"; // if both values are the same then we jump infront of the block and let it continue
                        }
                        treeDecider(root->getChild(3), asmFile, status, outsideFuncVarCount); // the statment
                        asmFile << "BR " << lab0 << "\n";        // like this: take processing back to the top of the loop
                        
                        asmFile << lab1 << ": NOOP\n";           // label for breaking out of the loop
                        printedNode = true;
                    }
                        break;
                    
                    case LOOP2:{
                        string lab0 = newName(LAB);
                        string arg1 = treeDecider(root->getChild(1), asmFile, status, outsideFuncVarCount); // getting the val of the first expr
                        string cond = treeDecider(root->getChild(2), asmFile, status, outsideFuncVarCount); // getting the operator
                        string arg2 = treeDecider(root->getChild(3), asmFile, status, outsideFuncVarCount); // getting the val of the second expr
                        string lab1 = newName(LAB); // generating a label to jump too if condition not met

                        asmFile << lab0 << ": NOOP\n";   // like this: this is the label to jump back to the top of the loop
                        treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount);// this calls the stat
                        //in the asm language we check for the opposite and jump if true, skiping the expr
                        if(!cond.compare("MOR")){ 
                            asmFile << "LOAD " << arg1 << "\n"; // load the first expression
                            asmFile << "SUB " << arg2 << "\n"; // subing the arg2 from the acc
                            asmFile << "BRPOS " << lab1 << "\n"; // breaks for greater op
                        } 
                        else if(!cond.compare("EQV")){ // breaks for equivalent op
                            asmFile << "LOAD " << arg1 << "\n"; // load the first expression
                            asmFile << "SUB " << arg2 << "\n"; // subing the arg2 from the acc
                            asmFile << "BRNEG " << lab1 << "\n"; 
                            asmFile << "BRPOS " << lab1 << "\n"; 
                        } 
                        else if(!cond.compare("LES")){ 
                            asmFile << "LOAD " << arg1 << "\n"; // load the first expression
                            asmFile << "SUB " << arg2 << "\n"; // subing the arg2 from the acc
                            asmFile << "BRNEG " << lab1 << "\n"; // breaks for less op
                        } 
                        else if(!cond.compare("NOTEQV")){ 
                            asmFile << "LOAD " << arg1 << "\n"; // load the first expression
                            asmFile << "SUB " << arg2 << "\n"; // subing the arg2 from the acc
                            asmFile << "BRZERO " << lab1 << "\n"; // break not equal op
                        } 
                        else{ // breaks for "..." op 
                            string lab2 = newName(LAB);
                            string notNeg = newName(LAB);
                            string sub2 = newName(LAB);
                            string prevEven = newName(LAB);
                            string prevOdd = newName(LAB);
                            string notNeg1 = newName(LAB);
                            string sub2again = newName(LAB);
                            string notNeg2 = newName(LAB);
                            string sub2again3 = newName(LAB);
                            string fail = newName(LAB);
                            string bothSame = newName(LAB);
                            

                            asmFile << "LOAD " << arg1 << "\n";
                            asmFile << "BRPOS " << notNeg << "\n";
                            asmFile << "MULT -1\n";
                            asmFile << notNeg << ": NOOP\n";
                            asmFile << sub2 << ": SUB 2\n";
                            asmFile << "BRPOS " << sub2 << "\n";
                            asmFile << "BRZERO " << prevEven << "\n";
                            asmFile << "BRNEG " << prevOdd << "\n";

                            asmFile << prevEven << ": LOAD " << arg2 << "\n";
                            asmFile << "BRPOS " << notNeg1 << "\n";
                            asmFile << "MULT -1\n";
                            asmFile << notNeg1 << ": NOOP\n";
                            asmFile << sub2again << ": SUB 2\n";
                            asmFile << "BRPOS " << sub2again << "\n";
                            asmFile << "BRZERO " << bothSame << "\n";
                            asmFile << "BRNEG " << lab1 << "\n";

                            asmFile << prevOdd << ": LOAD " << arg2 << "\n";
                            asmFile << "BRPOS " << notNeg2 << "\n";
                            asmFile << "MULT -1\n";
                            asmFile << notNeg2 << ": NOOP\n";
                            asmFile << sub2again3 << ": SUB 2\n";
                            asmFile << "BRPOS " << sub2again3 << "\n";
                            asmFile << "BRZERO " << lab1 << "\n";
                            asmFile << "BRNEG " << bothSame << "\n";

                            asmFile << bothSame << ": NOOP\n"; // if both values are the same then we jump infront of the block and let it continue
                        }
                        asmFile << "BR " << lab0 << "\n";        // like this: take processing back to the top of the loop
                        
                        asmFile << lab1 << ": NOOP\n";           // label for breaking out of the loop
                        printedNode = true;
                        break;
                    }
                        
                    case PICK: {
                        string arg1 = treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount);//this gets the expr
                        
                        
                        asmFile << "LOAD " << arg1 << "\n";
                        treeDecider(root->getChild(1), asmFile, status, outsideFuncVarCount);

                        printedNode = true;
                        break;
                    }
                    

                    default:
                        break;
                }
            }

            tokenIndex++;
        }else if(root->printOrder.at(i) == 0){ // for nonterminal

            if(!printedNode){
                // cout << "our current label intance is: " <<  root->getLabel() << endl;
                switch (root->getLabel()) {
                    case EXPR: {
                        if(root->ntIndex > 1){
                            string arg1 = treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount); 
                            string arg2 = treeDecider(root->getChild(1), asmFile, status, outsideFuncVarCount); 
                            asmFile << "LOAD " << arg1 << "\n";
                            asmFile << "SUB " << arg2 << "\n";      // subbing the acc with arg2
                            string arg3 = newName(VAR);        
                            asmFile << "STORE " << arg3 << "\n";    // saving the value of the subtraction
                            value = arg3;   // this will return the value of arg3
                        }else{ // when there is no subtraction
                            string arg1 = treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount); 
                            value = arg1;                   // returns the value
                        }
                        printedNode = true;
                        break;
                    }

                    case N_: {
                        string arg1 = treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount);
                        value = arg1;
                        
                        string arg2;
                        if (root->ntIndex > 1)
                        {
                            asmFile << "LOAD " << arg1 << "\n";     // loading arg1 into the acc
                            arg2 = treeDecider(root->getChild(1), asmFile, status, outsideFuncVarCount);
                            //asmFile << "LOAD " << arg2 << "\n";     // loading arg1 into the acc
                            value = arg2;
                        }
                        else
                        {
                            //asmFile << "LOAD " << arg1 << "\n";     // loading arg1 into the acc
                        }
                        printedNode = true;
                        break;
                    }
                    
                    case Nprime_: {
                        printedNode = true;
                        break;
                    }


                    case A_: { // samething here too but for addition
                        if(root->ntIndex > 1){
                            string arg1 = treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount);
                            string arg2 = treeDecider(root->getChild(1), asmFile, status, outsideFuncVarCount);
                            asmFile << "LOAD " << arg1 << "\n";
                            asmFile << "MULT " << arg2 << "\n";
                            string arg3 = newName(VAR);
                            asmFile << "STORE " << arg3 << "\n";
                            asmFile << "LOAD " << arg3 << "\n";
                            value = arg3;
                        }else{ // when there is no addition
                            string arg1 = treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount);
                            value = arg1;
                        }
                        printedNode = true;
                        break;
                    }
                    case M_: { // Because M case can start with ^ or without one, we need a case in both the terminals and non terminals
                        string arg1 = treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount);
                        value = arg1;
                        
                        printedNode = true;
                        break;
                    }
                    case STATS: { 
                        if(root->ntIndex > 1){
                            treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount);
                            treeDecider(root->getChild(1), asmFile, status, outsideFuncVarCount);
                        }else{
                            treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount);
                        }
                        
                        printedNode = true;
                        break;
                    }
                    case MSTAT: { 

                        treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount);
                        if(root->ntIndex > 1){
                            treeDecider(root->getChild(1), asmFile, status, outsideFuncVarCount);
                        }
                        printedNode = true;
                        break;
                    }
                    case STAT: { 
                        treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount);
                        printedNode = true;
                        break;
                    }

                    case BLOCK: { 
                        treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount);
                        if (root->ntIndex > 1) {
                            treeDecider(root->getChild(1), asmFile, status, outsideFuncVarCount);
                        }
                        printedNode = true;
                        break;
                    }

                    case PICKBODY: {

                        string lab2 = newName(LAB);
                        string lab3 = newName(LAB);

                        asmFile << "BRZERO " << lab2 << "\n";

                        treeDecider(root->getChild(0), asmFile, status, outsideFuncVarCount);//this gens one stat
                        asmFile << "BR " << lab3 << "\n";

                        asmFile << lab2 << ": NOOP\n";
                        treeDecider(root->getChild(1), asmFile, status, outsideFuncVarCount);//this gens the other

                        asmFile << lab3 << ": NOOP\n";

                        printedNode = true;
                        break;
                    }

                    default:
                        treeDecider(root->getChild(childIndex), asmFile, status, outsideFuncVarCount);  // check child tree
                        break;
                }
            }else if(root->getChild(childIndex)->getLabel() == VARS){
                treeDecider(root->getChild(childIndex), asmFile, status, outsideFuncVarCount);  // check child tree
            }

            
            childIndex++;
        }else{
            cout << "something went wrong" << endl; // this should never happen
        }
    }
    recursionDepth--;
    if(recursionDepth == 0){
        
        asmFile << "STOP" << endl;
        
        for(unsigned int i = 0; i < asmVars.size(); i++){
            string currentVarVal = "-7";
            if(asmVarsValues.count(asmVars.at(i)) > 0){
                currentVarVal = asmVarsValues[asmVars.at(i)];
            }
            asmFile << asmVars.at(i) << " " << currentVarVal << endl;
        }
        status = true;
    }

    return value;
}




// Prints out an error message for errors with variable creation and use
void VarError(string errorType, string instance){
    if(errorType.compare("NE") == 0){
        cout << "P3 ERROR: Variable \"" << instance << "\" does not exist." << endl;
        exit(-1);
    }else if(errorType.compare("ND") == 0){
        cout << "P3 ERROR: Variable \"" << instance << "\" is not defined." << endl;
        exit(-1);

    }else if(errorType.compare("AE") == 0){
        cout << "P3 ERROR: Variable \"" << instance << "\" already exists and cannot be recreated." << endl;
        exit(-1);

    }else{
        cout << "P3 ERROR: some other kind of error with vars." << endl;
        exit(-1);
    }
}
