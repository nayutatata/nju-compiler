/*
    symbol table entry must contain the following information:
    symbol name, symbol position, symbol type(including int, float, array, func, struct), symbol value.
*/
#ifndef _SYMBOLTABLE_H_
#define _SYMBOLTABLE_H_
#include "ast.h"
// the followings are the descriptions of C-- type.
struct field_node;
struct param_node;
typedef enum sym_kind {
    SYM_BASIC,
    SYM_ARRAY,
    SYM_STRUCT,
    SYM_FUNC,
} sym_kind;
typedef struct sym_type {
    sym_kind kind;
    union // details
    {
        struct field_node* struct_info;
        struct {
            int num; // dimensions
            struct sym_type *type;
        } array_info;
        enum
        {
            SYM_INT,
            SYM_FLOAT
        } basic_info;
        struct{
            int num; // number of parameters
            struct param_node *param_types;
            struct sym_type *return_type;
        } func_info;
    };
} sym_type;
typedef struct field_node{
    char name[40];
    sym_type *type;
    struct field_node *next;
} field_node;
typedef struct param_node{
    sym_type *type;
    struct param_node *next;
} param_node;
// the following is definition of symbol table
typedef struct sym_entry
{
    char name[40];
    sym_type *type;
    int line, col;
    struct sym_entry *next;
} sym_entry;

sym_entry *symtable;
sym_entry *find_entry(char *name);
sym_entry *add_entry(sym_entry *); // if already existed, return the duplicate entry. if successful, return NULL
void init_symtable();
sym_entry* new_sym_entry(Node* node,sym_type* type);
void print_symtable();
#endif