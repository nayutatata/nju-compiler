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
    SYM_TYPE,
    SYM_EMPTY
} sym_kind;
typedef struct sym_type {
    sym_kind kind;
    union // details
    {
        struct field_node* struct_info;
        struct {
            int num; // dimensions
            struct sym_type *type;
            int size[30]; // size of each dimension
        } array_info;
        enum
        {
            SYM_INT,
            SYM_FLOAT
        } basic_info;
        struct{
            int num; // number of parameters
            struct field_node *param_types;
            struct sym_type *return_type;
        } func_info;
        struct sym_type* type_info;
    };
} sym_type;
typedef struct field_node{
    char name[40];
    sym_type *type;
    struct field_node *next;
} field_node;
// the following is definition of symbol table
typedef struct sym_entry
{
    char name[40];
    sym_type *type;
    int line, col;
    struct sym_entry *next;
    int is_array_param;
} sym_entry;
typedef struct type_entry{
    char name[40];
    sym_entry* type;
    struct type_entry* next;
} type_entry;
typedef struct frame_t{
    sym_entry* symtable;
    struct frame_t* next;
} frame_t;
sym_entry* symtable;
type_entry* typetable;
frame_t* frame;
sym_entry* find_sym_entry(char* name);
sym_entry *add_sym_entry(sym_entry * entry); // if already existed, return the duplicate entry. if successful, return NULL
sym_entry* find_sym_entry_frame(char* name);
type_entry* find_type_entry(char* name);
type_entry* add_type_entry(type_entry*);
void init_symtable();
void init_typetable();
sym_entry* new_sym_entry(Node* node, sym_type* type);
void print_symtable();
sym_type* new_empty_type();
void init_frame();
void new_frame();
void pop_frame();
int type_eq(sym_type* t1, sym_type* t2);
char* show_info(sym_type* type);
int can_logic(sym_type* type);
void add_other();
#endif