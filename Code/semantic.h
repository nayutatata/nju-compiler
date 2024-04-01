#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_
#include "ast.h"
#include "symboltable.h"
/*

*/
void semantic_analysis(Node *);
void handle_program(Node *r);
void handle_extdeflist(Node *r);
void handle_extdef(Node *r);
void handle_extdeclist(Node *r,sym_type* var_type);
sym_type* handle_specifier(Node *r);
sym_type* handle_structspecifier(Node *r);
sym_entry* handle_opttag(Node *r);
sym_type* handle_tag(Node *r);
field_node* handle_vardec(Node *r,sym_type* var_type,int dim);
sym_type* handle_funcdec(Node *r,sym_type* return_type);
field_node* handle_varlist(Node *r);
field_node* handle_paramdec(Node *r);
void handle_compst(Node *r, sym_type* func_type);
void handle_stmtlist(Node *r, sym_type* func_type);
void handle_stmt(Node *r, sym_type* func_type);
field_node* handle_deflist(Node *r);
field_node* handle_def(Node *r);
field_node* handle_declist(Node *r, sym_type* type);
field_node* handle_dec(Node *r, sym_type* type);
sym_type* handle_exp(Node *r);
void handle_args(Node *r);
extern frame_t* frame;
extern sym_entry* symtable;
#endif