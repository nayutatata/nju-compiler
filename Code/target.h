#ifndef _TARGET_H_
#define _TARGET_H_
#include "ir.h"
#include <stdlib.h>
typedef struct record_t{
    char name[20]; // name of variable
    int off;    // offset relative to ebp
    struct record_t* next;
} record_t;
void gen_target();
void gen_one(code_entry* ce);
void gen_assign(code_entry* ce);
void gen_plus(code_entry* ce);
void gen_minus(code_entry* ce);
void gen_mul(code_entry* ce);
void gen_div(code_entry* ce);
void gen_star(code_entry* ce);
void gen_tostar(code_entry* ce);
void gen_goto(code_entry* ce);
void gen_return(code_entry* ce);
void gen_call(code_entry* ce);
void gen_if_goto(code_entry* ce);
void output_target(FILE*);
void gen_funcdec(code_entry* ce);
void gen_labeldec(code_entry* ce);
void gen_write(code_entry* ce);
void gen_read(code_entry* ce);
void gen_param(code_entry* ce);
void gen_arg(code_entry* ce);
void gen_dec(code_entry* ce);
void gen_addr(code_entry* ce);
extern code_list clst;
#endif