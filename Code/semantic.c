#include "semantic.h"
#include "symboltable.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
void nop(){
    print_symtable();
    fprintf(stderr, "Not implemented yet.\n");
    assert(0);
}
static const char* print_table[] = {
    "You should not go here!!!",
    "var is used before definition.",
    "function is invoked before definition.",
    "redefinition of var or name of var complicts with struct.",
    "redefinition of function.",
    "different type beside '=' .",
    "try to assign to right-side value.",
    "oprand type dismatch or the type has no operator.",
    "return-type dismatch.",
    "arguements dismatch with parameters.",
    "var has no operator '[]' .",
    "var has no operator '()' .",
    "float in '[]' .",
    "var has no operator '.' ",
    "struct has no field like that.",
    "redefinition in struct or initialization of struct field.",
    "redefinition of struct or complicts with var.",
    "unknown struct."
};
static void semantic_error(int idx, int line) {
    printf("Error type %d at Line %d: %s\n", idx, line, print_table[idx]);
}
void semantic_analysis(Node* r){
    handle_program(r);
}
void handle_program(Node* r){
    assert(r->ntype == _Program);
    Node* extdeflist = getchild(r, 0);
    handle_extdeflist(extdeflist);
}
void handle_extdeflist(Node* r){
    if (!r)
        return;
    assert(r->ntype == _ExtDefList);
    Node* extdef = getchild(r, 0);
    Node *extdeflist = getchild(r, 1);
    handle_extdef(extdef);
    handle_extdeflist(extdeflist);
}
void handle_extdef(Node *r){
    assert(r->ntype == _ExtDef);
    Node* specifier = getchild(r, 0);
    sym_type *type = handle_specifier(specifier);
    Node *child1 = getchild(r, 1);
    assert(child1);
    if (child1->ntype == _ExtDecList) {
        handle_extdeclist(child1, type);
    } else if (child1->ntype == _FunDec) {
        sym_type* t = handle_funcdec(child1,type);
        assert(t);
        handle_compst(getchild(r, 2), t);
    }
}
void handle_extdeclist(Node* r,sym_type* type){
    assert(r->ntype == _ExtDecList);
    if (r->ccnt == 1) {
        handle_vardec(getchild(r, 0), type, 0);
    } else {
        handle_vardec(getchild(r, 0),type,0);
        handle_extdeclist(getchild(r, 2),type);
    }
}
sym_type* handle_specifier(Node *r){
    assert(r->ntype == _Specifier);
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
    assert(r->ntype == _StructSpecifier);
    if (r->ccnt == 2) {
        return handle_tag(getchild(r,1));
    }
    sym_entry* se = handle_opttag(getchild(r, 1));
    Node* deflist = getchild(r, 3);
    field_node* flist = handle_deflist(deflist);
    sym_type* res = malloc(sizeof(sym_type));
    res->kind = SYM_STRUCT;
    res->struct_info = flist;
    if (se) {
        se->type->type_info = res;
        add_sym_entry(se);
    }
    return res;
}
// return type is sym_entry, because we need id's name to fill the symtable
sym_entry* handle_opttag(Node* r){ 
    if (!r)
        return NULL;
    assert(r->ntype == _OptTag);
    Node* child = getchild(r, 0);
    sym_entry* detect = find_sym_entry_frame(child->val.name);
    if (detect){
        semantic_error(16, child->nline);
        return NULL;
    }
    sym_type* t = malloc(sizeof(sym_type));
    t->kind = SYM_TYPE;
    sym_entry* res = new_sym_entry(child, t);
    return res;
}
sym_type* handle_tag(Node* r){
    // must search the type table by tag's name to find the sym_type
    assert(r);
    assert(r->ntype == _Tag);
    Node* child = getchild(r, 0);
    sym_entry* detect = find_sym_entry_frame(child->val.name);
    if (detect){
        if (detect->type->kind==SYM_TYPE){
            return detect->type->type_info;
        }
        semantic_error(16, child->nline);  // int a; struct a b;
        return new_empty_type();
    } else {
        semantic_error(17, child->nline);
        return new_empty_type(); // the type not found.
    }
}
field_node* handle_vardec(Node *r,sym_type* type,int dim){ // in fact this return-type is not good, the right one should be sym_type, but I can get it by return-val->type, haha
    assert(r->ntype == _VarDec);
    if (r->ccnt == 1) {
        field_node* res = malloc(sizeof(field_node));
        strcpy(res->name, getchild(r, 0)->val.name);
        res->next = NULL;
        if (dim == 0) {
            res->type = type;
            sym_entry* se = new_sym_entry(getchild(r, 0), type);
            sym_entry* detect = find_sym_entry_frame(se->name);
            if (detect){
                semantic_error(3, getchild(r,0)->nline); // redefinition
                return res; // I don't know whether it can work
            }
            add_sym_entry(se);
            return res;
        }
        sym_type* array_type = malloc(sizeof(sym_type));
        array_type->kind = SYM_ARRAY;
        array_type->array_info.num = dim;
        array_type->array_info.type = type;
        res->type = array_type;
        sym_entry* se = new_sym_entry(getchild(r, 0), array_type);
        sym_entry* detect = find_sym_entry_frame(se->name);
        if (detect) {
            semantic_error(3, r->nline);  // redefinition
            return res;                   // I don't know whether it can work
        }
        add_sym_entry(se);
        return res;
    }
    return handle_vardec(getchild(r, 0), type, dim + 1);
}
sym_type* handle_funcdec(Node* r,sym_type* return_type){
    assert(r->ntype == _FunDec);
    Node* id = getchild(r, 0);  // this is an ID node
    sym_entry* se,*detect = find_sym_entry_frame(id->val.name);
    sym_type* t = malloc(sizeof(sym_type));
    t->kind = SYM_FUNC;
    t->func_info.return_type = return_type;
    if (detect) {
        semantic_error(4, id->nline);
        return new_empty_type();
    }
    if (r->ccnt==3){
        t->func_info.param_types = NULL;
        se = new_sym_entry(id, t);
        add_sym_entry(se);
        return t;
    }
    field_node* type_list = handle_varlist(getchild(r, 2));
    t->func_info.param_types = type_list;
    se = new_sym_entry(id, t);
    add_sym_entry(se);
    return t;
}
field_node* handle_varlist(Node* r){
    assert(r->ntype == _VarList);
    field_node* res = handle_paramdec(getchild(r, 0));
    if (r->ccnt == 1)
        return res;
    res->next = handle_varlist(getchild(r, 2));
    return res;
}
field_node* handle_paramdec(Node* r){
    assert(r->ntype == _ParamDec);
    Node* vardec = getchild(r, 1);
    Node* specifier = getchild(r, 0);
    sym_type* t = handle_specifier(specifier);
    field_node* fn = handle_vardec(vardec, t, 0);
    return fn;
}
void handle_compst(Node* r,sym_type* func_type){
    assert(r->ntype == _CompSt);
    assert(func_type == NULL || func_type->kind == SYM_FUNC || func_type->kind == SYM_EMPTY); // compst may be follow a func definition.
    //new_frame(); // error here, if this is an func compst, I should add params into symtable.
    sym_entry* new_symtable = malloc(sizeof(sym_entry));
    new_symtable->next = NULL;
    if (func_type->kind == SYM_FUNC) {
        field_node* fn = func_type->func_info.param_types;
        while (fn){
            sym_entry* se = find_sym_entry(fn->name);
            se->next = new_symtable->next;
            new_symtable->next = se;
            fn = fn->next;
        }
        new_frame();
        frame->symtable = new_symtable;
        symtable = new_symtable;
        //assert(symtable->next);
        //print_symtable();
    } else
        new_frame();
    Node* deflist = getchild(r, 1);
    Node* stmtlist = getchild(r, 2);
    handle_deflist(deflist);
    handle_stmtlist(stmtlist,func_type);
    pop_frame();
}
void handle_stmtlist(Node* r, sym_type* func_type){ 
    if (!r)
        return;
    assert(r->ntype == _StmtList);
    Node* stmt = getchild(r, 0);
    Node* stmtlist = getchild(r, 1);
    handle_stmt(stmt,func_type);
    handle_stmtlist(stmtlist,func_type);
}
void handle_stmt(Node* r, sym_type* func_type){          // if stmt is a return stmt, then we should check whether the return type is right. so we need a func type
    assert(r->ntype == _Stmt);
    if (r->ccnt==1){
        Node* compst = getchild(r, 0);
        handle_compst(compst,func_type);
        return;
    }
    if (r->ccnt==2){
        Node* exp = getchild(r, 0);
        handle_exp(exp);
        return;
    }
    if (r->ccnt==3){
        Node* exp = getchild(r, 1);
        sym_type* t = handle_exp(exp);
        //printf("%s\n", show_info(t));
        //printf("%s\n", show_info(func_type->func_info.return_type));
        if (func_type->kind == SYM_FUNC) {
            if (!type_eq(t,func_type->func_info.return_type)){
                semantic_error(8, exp->nline);
            }
        }
        return;
    }
    assert(0);
}
field_node* handle_deflist(Node* r){ // in fact we need this to return something for struct definition
    if (!r)
        return NULL;
    assert(r->ntype == _DefList);
    field_node* res = handle_def(getchild(r, 0));
    assert(res);
    field_node* t = handle_deflist(getchild(r, 1));
    if (!t)
        return res;
    t->next = res;
    return t;
}
field_node* handle_def(Node* r){
    assert(r->ntype == _Def);
    Node *specifier = getchild(r, 0), *declist = getchild(r, 1);
    sym_type* type = handle_specifier(specifier);
    field_node* res = handle_declist(declist, type);
    return res;
}
field_node* handle_declist(Node* r,sym_type* type){
    assert(r->ntype == _DecList);
    Node* dec = getchild(r, 0);
    field_node* res = handle_dec(dec, type);
    assert(r);
    if (r->ccnt == 1){
        res->next = NULL;
        return res;
    }
    assert(res);
    res->next = handle_declist(getchild(r, 2), type);
    return res;
}
field_node* handle_dec(Node* r,sym_type* type){
    assert(r->ntype == _Dec);
    Node* vardec = getchild(r, 0);
    assert(r);
    if (r->ccnt == 1) {
        return handle_vardec(vardec,type,0);
    }
    nop(); // VarDec ASSIGNOP Exp not implemented yet.
}
sym_type* handle_exp(Node* r){
    assert(r->ntype == _Exp);
    if (r->ccnt==1){
        Node* child = getchild(r, 0);
        if (child->ntype==_ID){
            sym_entry* detect = find_sym_entry_frame(child->val.name);
            if (!detect){
                semantic_error(1, child->nline);
                return new_empty_type();
            }
            return detect->type;
        }
    }
    nop();
}
void handle_args(Node* r){
    assert(r->ntype == _Args);
    nop();
}