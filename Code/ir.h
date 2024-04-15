#ifndef _IR_H_
#define _IR_H_
#include "symboltable.h"
#include "ast.h"
#include <stdio.h>
static int linenum = 1;
static int tnum = 0;
typedef enum operand_kind {
    OPE_IMM,
    OPE_VAR,
    OPE_FUNC,
    OPE_LABEL,
    OPE_TEMP,
    OPE_NULL
}
operand_kind;
typedef enum operator_kind {
    OP_REL,
    OP_ASSIGN,
    OP_PLUS,
    OP_MINUS,
    OP_MUL,
    OP_DIV,
    OP_REF,
    OP_ADDR,
    OP_STAR,
    OP_TOSTAR,
    OP_GOTO,
    OP_IF_GOTO,
    OP_RETURN,
    OP_DEC,
    OP_ARG,
    OP_CALL,
    OP_PARAM,
    OP_READ,
    OP_WRITE,
    OP_FUNCDEC,
    OP_LABELDEC,
    OP_NULL
}
operator_kind;
typedef struct operand_t {
    char name[20];      // name of opearand
    operand_kind kind;
    int imm_v;
    float imm_fv;
    int is_arr;
    int is_arg;
    int should_be_array;
} operand_t;
typedef struct operator_t{
    char name[20];      // name of operator
    operator_kind kind;
} operator_t;
typedef struct code_entry {
    operand_t arg1, arg2, result;
    operator_t op, gotoop;
} code_entry;
typedef struct code_list{
    code_entry* lst[2000];
    int num;
} code_list;
code_list clst;
operand_t* new_temp();
operand_t* new_label();
operand_t* new_imm(int val);
void translate(Node* node);
void print_code_list(FILE* file);
void init_code_list();
void add_code(code_entry* ce);
code_entry* new_ce();
void tr_program(Node* root);
void tr_extdeflist(Node* root);
void tr_extdef(Node* root);
void tr_extdeclist(Node* root);
void tr_specifier(Node* root);
void tr_structspecifier(Node* root);
void tr_opttag(Node* root);
void tr_tag(Node* root);
operand_t* tr_vardec(Node* root,int size);
void tr_funcdec(Node* root);
void tr_varlist(Node* root);
void tr_paramdec(Node* root);
void tr_compst(Node* root);
void tr_stmtlist(Node* root);
void tr_stmt(Node* root);
void tr_deflist(Node* root);
void tr_def(Node* root);
void tr_declist(Node* root);
void tr_dec(Node* root);
code_entry* tr_exp(Node* root);
void tr_args(Node* root);
void tr_cond(Node* root, operand_t true_label, operand_t false_label);
operand_t get_addr(Node* root, int dim);
code_entry* process_assign(operand_t a, operand_t b);
operand_t to_value(operand_t ope);
#endif