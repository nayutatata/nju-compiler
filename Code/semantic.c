/*
remaining 2 errors about structure:
    1. var's redefinition in struct
    2. assign exp in struct
*/
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
    "operand type dismatch or the type has no operator.",
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
static int error_vis[2000];
static void semantic_error(int idx, int line) {
    if (!error_vis[line])
        error_vis[line]=1,printf("Error type %d at Line %d: %s\n", idx, line, print_table[idx]);
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
        handle_compst(getchild(r, 2), t,1);
    }
}
void handle_extdeclist(Node* r,sym_type* type){
    assert(r->ntype == _ExtDecList);
    if (r->ccnt == 1) {
        handle_vardec(getchild(r, 0), type, 0,0);
    } else {
        handle_vardec(getchild(r, 0),type,0,0);
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
    new_frame();
    field_node* flist = handle_deflist(deflist,1);
    pop_frame();
    sym_type* res = malloc(sizeof(sym_type));
    res->kind = SYM_STRUCT;
    res->struct_info = flist;
    if (se) {
        se->type->type_info = res;
        sym_entry* detect = find_sym_entry(se->name);
        if (detect){
            semantic_error(3, r->nline);
            return res;
        }
        //add_sym_entry(se);
        // struct definition should be global
        frame_t* now_frame = frame;
        while (now_frame){
            sym_entry* now_table = now_frame->symtable;
            sym_entry* now_entry = malloc(sizeof(sym_entry));
            memcpy(now_entry, se, sizeof(sym_entry)); // still a light copy, the type not fully copied.
            now_entry->next = now_table->next;
            now_table->next = now_entry;
            now_frame = now_frame->next;
        }
    }
    return res;
}
// return type is sym_entry, because we need id's name to fill the symtable
sym_entry* handle_opttag(Node* r){ 
    if (!r)
        return NULL;
    assert(r->ntype == _OptTag);
    Node* child = getchild(r, 0);
    sym_entry* detect = find_sym_entry(child->val.name);
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
    sym_entry* detect = find_sym_entry(child->val.name);
    if (detect){
        if (detect->type->kind==SYM_TYPE){
            //puts(show_info(detect->type->type_info));
            return detect->type->type_info;
        }
        semantic_error(16, child->nline);  // int a; struct a b;
        return new_empty_type();
    } else {
        semantic_error(17, child->nline);
        return new_empty_type(); // the type not found.
    }
}
field_node* handle_vardec(Node *r,sym_type* type,int dim,int is_struct){ // in fact this return-type is not good, the right one should be sym_type, but I can get it by return-val->type, haha
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
                if (is_struct){
                    semantic_error(15, getchild(r, 0)->nline);
                    return res;
                }
                semantic_error(3, getchild(r,0)->nline); // redefinition
                //puts(show_info(res->type));
                return res;  // I don't know whether it can work
            }
            else{
                sym_entry* struct_name = find_sym_entry(se->name);
                if (struct_name&&struct_name->type->kind==SYM_TYPE){
                    semantic_error(3, getchild(r, 0)->nline);
                    return res;
                }
            }
            add_sym_entry(se);// actually if it is a parameter, then it should not be added. some error here.
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
            if (is_struct) {
                semantic_error(15, getchild(r, 0)->nline);
                return res;
            }
            semantic_error(3, getchild(r,0)->nline);  // redefinition
            return res;                   // I don't know whether it can work
        } else { // if name of var conflicts with struct name.
            sym_entry* struct_name = find_sym_entry(se->name);
            if (struct_name && struct_name->type->kind == SYM_TYPE) {
                semantic_error(3, getchild(r, 0)->nline);
                return res;
            }
        }
        add_sym_entry(se);
        return res;
    }
    return handle_vardec(getchild(r, 0), type, dim + 1,is_struct);
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
        //return new_empty_type();
    }
    if (r->ccnt==3){
        t->func_info.param_types = NULL;
        t->func_info.num = 0;
        se = new_sym_entry(id, t);
        add_sym_entry(se);
        new_frame();
        return t;
    }
    se = new_sym_entry(id, t);
    add_sym_entry(se);
    new_frame();
    field_node* type_list = handle_varlist(getchild(r, 2));
    t->func_info.param_types = type_list;
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
    field_node* fn = handle_vardec(vardec, t, 0,0);
    return fn;
}
void handle_compst(Node* r,sym_type* func_type,int func_first){
    assert(r->ntype == _CompSt);
    assert(func_type == NULL || func_type->kind == SYM_FUNC || func_type->kind == SYM_EMPTY); // compst may be follow a func definition.
    //new_frame(); // error here, if this is an func compst, I should add params into symtable.
    sym_entry* new_symtable = malloc(sizeof(sym_entry));
    new_symtable->next = NULL;
    /*
    if (func_type!=NULL&&func_type->kind == SYM_FUNC) {
        field_node* fn = func_type->func_info.param_types;
        while (fn){
            sym_entry* se = find_sym_entry(fn->name);
            sym_entry* s = malloc(sizeof(sym_entry));  // never impact the symtable before.
            memcpy(s, se,sizeof(sym_entry));
            s->next = new_symtable->next;
            new_symtable->next = s;
            fn = fn->next;
        }
        new_frame();
        frame->symtable = new_symtable;
        symtable = new_symtable;
    }*/
    if (!func_first)
        new_frame();
    Node* deflist = getchild(r, 1);
    Node* stmtlist = getchild(r, 2);
    handle_deflist(deflist,0);
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
        handle_compst(compst,func_type,0);
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
        if (func_type!=NULL&&func_type->kind == SYM_FUNC) {
            if (!type_eq(t,func_type->func_info.return_type)){
                semantic_error(8, exp->nline);
            }
        }
        return;
    }
    if (r->ccnt>=5){
        Node* exp = getchild(r, 2);
        sym_type* exp_type = handle_exp(exp);
        if (!can_logic(exp_type)){
            semantic_error(7, exp->nline);
        }
        Node* stmt1 = getchild(r, 4);
        handle_stmt(stmt1,func_type);
        if (r->ccnt==7)
            handle_stmt(getchild(r, 6), func_type);
        return;
    }
    assert(0);
}
// we should distinguish struct definition and common local definition. 
field_node* handle_deflist(Node* r,int is_struct){ // in fact we need this to return something for struct definition
    if (!r)
        return NULL;
    assert(r->ntype == _DefList);
    field_node* res = handle_def(getchild(r, 0),is_struct);
    assert(res);
    field_node* t = handle_deflist(getchild(r, 1),is_struct);
    if (!t)
        return res;
    field_node* tail = res;
    while (tail->next){
        tail = tail->next;
    }
    tail->next = t;
    return res;
}
field_node* handle_def(Node* r,int is_struct){
    assert(r->ntype == _Def);
    Node *specifier = getchild(r, 0), *declist = getchild(r, 1);
    sym_type* type = handle_specifier(specifier);
    field_node* res = handle_declist(declist, type,is_struct);
    return res;
}
field_node* handle_declist(Node* r,sym_type* type,int is_struct){
    assert(r->ntype == _DecList);
    Node* dec = getchild(r, 0);
    field_node* res = handle_dec(dec, type,is_struct);
    assert(r);
    if (r->ccnt == 1){
        res->next = NULL;
        return res;
    }
    assert(res);
    res->next = handle_declist(getchild(r, 2), type,is_struct);
    return res;
}
field_node* handle_dec(Node* r,sym_type* type,int is_struct){
    assert(r->ntype == _Dec);
    Node* vardec = getchild(r, 0);
    field_node* fn = handle_vardec(vardec,type,0,is_struct);
    if (r->ccnt==1)
        return fn;
    Node* exp = getchild(r,2);
    if (is_struct){
        semantic_error(15, vardec->nline);
    }
    sym_type* exp_type = handle_exp(exp);
    if (!type_eq(exp_type,fn->type)){
        semantic_error(5, exp->nline);
    }
    return fn;
}
sym_type* handle_exp(Node* r){
    assert(r->ntype == _Exp);
    if (r->ccnt==1){
        Node* child = getchild(r, 0);
        if (child->ntype==_ID){
            sym_entry* detect = find_sym_entry(child->val.name);
            if (!detect){
                semantic_error(1, child->nline);
                return new_empty_type();
            }
            return detect->type;
        }
        if (child->ntype==_INT){
            sym_type* t = malloc(sizeof(sym_type));
            t->kind=SYM_BASIC;
            t->basic_info = SYM_INT;
            return t;
        } else if (child->ntype==_FLOAT){
            sym_type* t = malloc(sizeof(sym_type));
            t->kind = SYM_BASIC;
            t->basic_info = SYM_FLOAT;
            return t;
        }
        assert(0);
    }
    else if (r->ccnt==2){
        Node* exp1 = getchild(r, 1);
        sym_type* exp_type = handle_exp(exp1);
        Node* op = getchild(r, 0);
        if (op->ntype==_NOT){
            if (!can_logic(exp_type)){
                semantic_error(7, exp1->nline);
            }
        }
        return exp_type;
    }
    else if (r->ccnt==3){
        Node *child0 = getchild(r, 0), *child2 = getchild(r, 2);
        if (child0->ntype==_Exp&&child2->ntype==_Exp){
            Node* op = getchild(r, 1);
            sym_type *exp_type0 = handle_exp(child0), *exp_type2 = handle_exp(child2);
            if (op->ntype==_ASSIGNOP){
                int lhs = 0;
                if (child0->ccnt==1&&getchild(child0,0)->ntype==_ID)
                    lhs = 1;
                if (child0->ccnt==3&&getchild(child0,1)->ntype==_DOT)
                    lhs = 1;
                if (child0->ccnt==4&&getchild(child0,1)->ntype==_LB)
                    lhs = 1;
                if (!lhs){
                    semantic_error(6, child0->nline);
                    //return new_empty_type();
                }
                if (!type_eq(exp_type0,exp_type2)){
                    semantic_error(5, child0->nline);
                }
                return exp_type0;
            } else if (op->ntype == _AND || op->ntype == _OR) {
                if (!can_logic(exp_type0)||!can_logic(exp_type2)){
                    semantic_error(7, child0->nline);
                    sym_type* rest = malloc(sizeof(sym_type));
                    rest->kind = SYM_BASIC;
                    rest->basic_info = SYM_INT;
                    return rest;  // I don't know whether it's right.
                }
                return exp_type0;
            } else if (!type_eq(exp_type0, exp_type2)) {
                semantic_error(7, child0->nline);
                //return exp_type0;
            }
            if (op->ntype==_RELOP){
                sym_type* rest = malloc(sizeof(sym_type));
                rest->kind = SYM_BASIC;
                rest->basic_info = SYM_INT;
                return rest;
            }
            return exp_type0;
        }
        else if (child0->ntype==_LP){
            return handle_exp(getchild(r, 1));
        }
        else if (child0->ntype==_Exp){ // struct
            sym_type* struct_type = handle_exp(child0);
            if (struct_type->kind != SYM_STRUCT) {
                semantic_error(13, child0->nline);
                puts(show_info(struct_type));
                return new_empty_type();
            }
            field_node* finder = struct_type->struct_info;
            while (finder){
                if (strcmp(finder->name,child2->val.name)==0){
                    break;
                }
                finder = finder->next;
            }
            if (!finder){
                semantic_error(14, child2->nline);
                return new_empty_type();
            }
            return finder->type;
        }
        else if (child0->ntype==_ID){
            sym_entry* se = find_sym_entry(child0->val.name);
            if (!se){
                semantic_error(2, child0->nline);
                return new_empty_type();
            }
            if (se->type->kind!=SYM_FUNC){
                semantic_error(11, child0->nline);
                return new_empty_type();
            }
            if (se->type->func_info.num!=0){
                semantic_error(9, child0->nline);
                return se->type->func_info.return_type;
            }
            return se->type->func_info.return_type;
        }
        assert(0);
    } else if (r->ccnt == 4) {
        Node *child0 = getchild(r, 0), *child2 = getchild(r, 2);
        if (child0->ntype==_ID){
            sym_entry* se = find_sym_entry(child0->val.name);
            if (!se) {
                semantic_error(2, child0->nline);
                return new_empty_type();
            }
            if (se->type->kind != SYM_FUNC) {
                semantic_error(11, child0->nline);
                return new_empty_type();
            }
            field_node* args = handle_args(child2);
            field_node* params = se->type->func_info.param_types;
            while (args&&params){
                if (!type_eq(args->type,params->type)){
                    break;
                }
                args = args->next, params = params->next;
            }
            if (args!=NULL||params!=NULL){
                semantic_error(9, child0->nline);
            }
            return se->type->func_info.return_type;
        }
        if (child0->ntype==_Exp){
            sym_type *exp_type0 = handle_exp(child0), *exp_type2 = handle_exp(child2);
            if (exp_type0->kind!=SYM_ARRAY){
                semantic_error(10, child0->nline);
                return new_empty_type();
            }
            sym_type* basic = exp_type0->array_info.type;
            if (!can_logic(exp_type2)) {
                semantic_error(12, child2->nline);
            }
            return basic;
        }
        assert(0);
    }
    assert(0);
}
field_node* handle_args(Node* r){
    assert(r->ntype == _Args);
    Node* exp = getchild(r, 0);
    sym_type* exp_type = handle_exp(exp);
    field_node* res = malloc(sizeof(field_node));
    strcpy(res->name, "null");
    res->type = exp_type;
    if (r->ccnt==1){
        res->next = NULL;
        return res;
    }
    Node* args = getchild(r,2);
    res->next = handle_args(args);
    return res;
}