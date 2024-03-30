#include "semantic.h"
#include "symboltable.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
void nop(){
    print_symtable();
    fprintf(stderr, "Not implemented yet.\n");
    assert(0);
}
static void sematic_error(int idx,int line){
    printf("Error type %d at Line %d.\n", idx, line);
}
void semantic_analysis(Node* r){
    handle_program(r);
}
void handle_program(Node* r){
    Node *extdeflist = getchild(r, 0);
    handle_extdeflist(extdeflist);
}
void handle_extdeflist(Node* r){
    if (!r)
        return;
    Node* extdef = getchild(r, 0);
    Node *extdeflist = getchild(r, 1);
    handle_extdef(extdef);
    handle_extdeflist(extdeflist);
}
void handle_extdef(Node *r){
    Node *specifier = getchild(r, 0);
    sym_type *type = handle_specifier(specifier);
    Node *child1 = getchild(r, 1);
    if (child1->ntype == _ExtDecList){
        handle_extdeclist(child1, type);
    }
    else if (child1->ntype == _FunDec) {
        handle_funcdec(child1,type);
        handle_compst(getchild(r, 2));
    }
}
void handle_extdeclist(Node* r,sym_type* type){
    if (r->ccnt==1){
        handle_vardec(getchild(r, 0),type,0);
    }
    else{
        handle_vardec(getchild(r, 0),type,0);
        handle_extdeclist(getchild(r, 2),type);
    }
}
sym_type* handle_specifier(Node *r){
    Node* child = getchild(r, 0);
    sym_type* res = malloc(sizeof(sym_type));
    if (child->ntype == _TYPE) {
        res->kind = SYM_BASIC;
        if (child->val.type == ttype_int){
            res->basic_info = _INT;
            return res;
        } else if (child->val.type == ttype_float) {
            res->basic_info = _FLOAT;
            return res;
        }
        fprintf(stderr, "You should not go here.");
        assert(0);
    }
    else{
        return handle_structspecifier(child);
    }
}
sym_type* handle_structspecifier(Node* r){
    if (r->ccnt==2){
        return handle_tag(r);
    }
    
}
void handle_opttag(Node* r){
    nop();
}
sym_type* handle_tag(Node* r){
    // must search the type table by tag's name to find the sym_type
    nop();
}
void handle_vardec(Node *r,sym_type* type,int dim){
    if (r->ccnt==1){
        if (dim==0){
            sym_entry* se = new_sym_entry(r, type);
            sym_entry* detect = add_entry(se);
            if (detect){
                sematic_error(3, r->nline); // redefinition
            }
            return;
        }
        sym_type* array_type = malloc(sizeof(sym_type));
        array_type->kind = SYM_ARRAY;
        array_type->array_info.num = dim;
        array_type->array_info.type = type;
        sym_entry* se = new_sym_entry(r, type);
        sym_entry* detect = add_entry(se);
        if (detect) {
            sematic_error(3, r->nline);  // redefinition
        }
        return;
    }
    handle_vardec(getchild(r, 0), type, dim + 1);
}
void handle_funcdec(Node* r,sym_type* return_type){
    nop();
}
void handle_varlist(Node* r){
    nop();
}
void handle_paramdec(Node* r){
    nop();
}
void handle_compst(Node* r){
    nop();
}
void handle_stmtlist(Node* r){
    nop();
}
void handle_stmt(Node* r){
    nop();
}
void handle_deflist(Node* r){
    nop();
}
void handle_def(Node* r){
    nop();
}
void handle_declist(Node* r){
    nop();
}
void handle_dec(Node* r){
    nop();
}
void handle_exp(Node* r){
    nop();
}
void handle_args(Node* r){
    nop();
}