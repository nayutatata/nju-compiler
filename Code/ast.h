#ifndef _AST_H_
#define _AST_H_
#include "type.h"
typedef struct
{
    ntype type;
    union{
        int intv;
        float floatv;
        reloptype opv;
        char ttype[10];
        char name[NAME_LEN];
    };
    int row, col;

    Node *children[MAX_CH];
    int ccnt;
} Node;

Node *newNode();
void setNode(Node *);
#endif