#include "ast.h"
#include "syntax.tab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
Node *root;
static const char *nametable[] = {
    "INT", "FLOAT", "ID", "SEMI", "COMMA", "ASSIGNOP", "RELOP", "PLUS", "MINUS", "STAR", "DIV",
    "AND", "OR", "DOT", "NOT", "TYPE", "LP", "RP", "LB", "RB", "LC", "RC", "STRUCT", "RETURN", "IF", "ELSE", "WHILE",
    "Program", "ExtDefList", "ExtDef", "ExtDecList",
    "Specifier", "StructSpecifier", "OptTag", "Tag",
    "VarDec", "FunDec", "VarList", "ParamDec",
    "CompSt", "StmtList", "Stmt",
    "DefList", "Def", "DecList", "Dec",
    "Exp", "Args", "None"};
int is_token(Symbol s){
    if (s<=_WHILE)
        return 1;
    else
        return 0;
}
Node *newNode()
{
    Node *res = malloc(sizeof(Node));
    memset(res, 0, sizeof(Node));
    res->nline = yylloc.first_line;
    res->ncol == yylloc.first_column;
    res->ccnt = 0;
    memset(res->children, 0, MAX_CH);
    return res;
}
Node* intNode(int a){
    Node *res = newNode();
    res->ntype = _INT;
    res->val.intv = a;
    return res;
}
Node* floatNode(float a){
    Node *res = newNode();
    res->ntype = _FLOAT;
    res->val.floatv = a;
}
Node* relNode(reloptype a){
    Node *res = newNode();
    res->ntype = _RELOP;
    res->val.opv = a;
    return res;
}
Node* typeNode(ttype a){
    Node *res = newNode();
    res->ntype = _TYPE;
    res->val.type = a;
    return res;
}
Node* idNode(char* name){
    Node *res = newNode();
    res->ntype = _ID;
    strcpy(res->val.name, name);
    return res;
}
Node* normalNode(Symbol a){
    Node *res = newNode();
    res->ntype = a;
    return res;
}
void addchild(Node *pa, Node *child)
{
    pa->children[pa->ccnt++] = child;
}
void print_ast(){
    helper(root, 0);
}
void helper(Node* r,int space){
    if (!r)
        return;
    for (int i = 0; i < space;i++)
        printf("  ");
    if (!is_token(r->ntype)){
        printf("%s (%d)\n", nametable[r->ntype],r->nline);
    }
    else{
        if (r->ntype==_ID)
            printf("%s: %s\n", nametable[r->ntype], r->val.name);
        else if (r->ntype==_TYPE){
            if (r->val.type==ttype_float){
                printf("%s: float\n", nametable[r->ntype]);
            }
            else
                printf("%s: int\n", nametable[r->ntype]);
        }
        else if (r->ntype==_FLOAT)
            printf("%s: %f\n", nametable[r->ntype], r->val.floatv);
        else if (r->ntype==_INT)
            printf("%s: %d\n", nametable[r->ntype], r->val.intv);
        else{
            printf("%s\n", nametable[r->ntype]);
        }
    }
    for (int i = 0; i < r->ccnt;i++){
        helper(r->children[i], space + 1);
    }
}
Node* getchild(Node* r,int i){
    if (i<0||i>=r->ccnt)
        return NULL;
    return r->children[i];
}