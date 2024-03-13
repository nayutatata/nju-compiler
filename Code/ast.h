#ifndef _AST_H_
#define _AST_H_
#include "type.h"
typedef enum
{
    _INT,_FLOAT,_ID,_SEMI,_COMMA,_ASSIGNOP,_RELOP,_PLUS,_MINUS,_STAR,_DIV,
    _AND,_OR,_DOT,_NOT,_TYPE,_LP,_RP,_LB,_RB,_LC,_RC,_STRUCT,_RETURN,_IF,_ELSE,_WHILE,
    _Program,_ExtDefList,_ExtDef,_ExtDecList,
    _Specifier,_StructSpecifier,_OptTag,_Tag,
    _VarDec,_FunDec,_VarList,_ParamDec,
    _CompSt,_StmtList,_Stmt,
    _DefList,_Def,_DecList,_Dec,
    _Exp,_Args,_None
} Symbol; // 49 symbols
typedef union
{
    int intv;            // literal val
    float floatv;        // literal val
    reloptype opv;       // relation operator val
    ttype type;          // type val: ttype_int ttype_float
    char name[NAME_LEN]; // id val
} vtype;

typedef struct Node
{
    Symbol ntype;
    vtype val;
    int row, col;
    struct Node *children[MAX_CH];
    int ccnt;       // children's size
    int nline, ncol;
} Node;

Node *newNode();
Node *intNode(int);
Node *floatNode(float);
Node *typeNode(ttype);
Node *idNode(char *);
Node *normalNode(Symbol);
Node *relNode(reloptype);
void addchild(Node *pa, Node *child);
void print_ast();
void helper(Node*,int);
int is_token(Symbol);
#endif