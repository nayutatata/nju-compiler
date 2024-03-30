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
void handle_opttag(Node *r);
sym_type* handle_tag(Node *r);
void handle_vardec(Node *r,sym_type* var_type,int dim);
void handle_funcdec(Node *r,sym_type* return_type);
void handle_varlist(Node *r);
void handle_paramdec(Node *r);
void handle_compst(Node *r);
void handle_stmtlist(Node *r);
void handle_stmt(Node *r);
void handle_deflist(Node *r);
void handle_def(Node *r);
void handle_declist(Node *r);
void handle_dec(Node *r);
void handle_exp(Node *r);
void handle_args(Node *r);
#endif