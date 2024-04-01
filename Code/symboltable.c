#include "symboltable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
frame_t* frame;
static char* print_table[] = {
    "BASIC", "ARRAY", "STRUCT", "FUNC", "TYPE", "EMPTY"};
char* show_info(sym_type* type){
    if (!type)
        return "null";
    sym_kind kind = type->kind;
    char* res = malloc(60);
    res[0] = '\0';
    if (kind == SYM_BASIC) {
        if (type->basic_info==SYM_INT){
            sprintf(res, "int");
        }
        else
            sprintf(res, "float");
        return res;
    }
    if (kind==SYM_ARRAY){
        char* a = show_info(type->array_info.type);
        sprintf(res, "array of %s, dim: %d", a, type->array_info.num);
        return res;
    }
    if (kind==SYM_FUNC){
        field_node* fn = type->func_info.param_types;
        strcat(res, "params: ");
        while (fn) {
            strcat(res, "name: ");
            strcat(res, fn->name);
            strcat(res, " ,type: ");
            strcat(res, show_info(fn->type));
            strcat(res, ". ");
            fn = fn->next;
        }
        strcat(res, "return-type: ");
        strcat(res, show_info(type->func_info.return_type));
        return res;
    }
    if (kind==SYM_STRUCT){
        field_node* fn = type->struct_info;
        while (fn){
            strcat(res, "name: ");
            strcat(res, fn->name);
            strcat(res, " ,type: ");
            strcat(res, show_info(fn->type));
            strcat(res, ". ");
            fn = fn->next;
        }
        return res;
    }
    if (kind==SYM_TYPE){
        return show_info(type->type_info);
    }
}
sym_entry* find_sym_entry(char* name) {
    assert(name != NULL);
    frame_t* now_frame = frame;
    while (now_frame){
        sym_entry* head = now_frame->symtable->next;
        while (head) {
            if (strcmp(name, head->name) == 0) {
                return head;
            }
            head = head->next;
        }
        now_frame = now_frame->next;
    }
    return NULL;
}
sym_entry* find_sym_entry_frame(char* name){
    assert(name != NULL);
    frame_t* now_frame = frame;
    sym_entry* head = now_frame->symtable->next;
    while (head) {
        if (strcmp(name, head->name) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}
type_entry* find_type_entry(char* name){
    assert(name != NULL);
    type_entry* head = typetable->next;
    while (head) {
        if (strcmp(name, head->name) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}
sym_entry* add_sym_entry(sym_entry* entry){
    sym_entry *found = find_sym_entry_frame(entry->name);
    if (found)
        return found;
    assert(symtable == frame->symtable);
    entry->next = symtable->next;
    symtable->next = entry;
    return NULL;
}
type_entry* add_type_entry(type_entry* entry) {
    type_entry* found = find_type_entry(entry->name);
    if (found)
        return found;
    entry->next = typetable->next;
    typetable->next = entry;
    return NULL;
}
void print_symtable(){
    sym_entry *head = symtable->next;
    while (head){
        printf("name: %s, type: %s, line: %d. unique information is followed: %s\n", head->name, print_table[head->type->kind], head->line,show_info(head->type));
        head = head->next;
    }
    puts("======================");
}
void init_frame(){
    frame = malloc(sizeof(frame_t));
    frame->symtable = symtable;
    frame->next = NULL;
}
void init_symtable(){
    symtable = malloc(sizeof(sym_entry)); // dummy node
    symtable->next = NULL;
}
void init_typetable(){
    typetable = malloc(sizeof(type_entry));
    typetable->next = NULL;
}
sym_entry* new_sym_entry(Node* node,sym_type* type){
    assert(node->ntype==_ID);
    sym_entry* res = malloc(sizeof(sym_entry));
    res->line = node->nline;
    res->col = node->ncol;
    if (node->ntype == _ID)
        strcpy(res->name, node->val.name);
    res->type = type;
    return res;
}

sym_type* new_empty_type(){
    sym_type* res = malloc(sizeof(sym_type));
    res->kind = SYM_EMPTY;
    return res;
}
void new_frame(){
    frame_t* new_frame = malloc(sizeof(frame_t));
    new_frame->symtable = malloc(sizeof(sym_entry)); // dummy node
    new_frame->symtable->next = NULL;
    new_frame->next = frame;
    frame = new_frame;
    symtable = new_frame->symtable;
}
void pop_frame(){
    frame = frame->next;
    symtable = frame->symtable;
}
int type_eq(sym_type* t1,sym_type* t2){
    assert(t1 && t2);
    if (t1->kind != t2->kind)
        return 0;
    sym_kind kind = t1->kind;
    if (kind==SYM_BASIC){
        return t1->basic_info == t2->basic_info;
    }
    if (kind==SYM_ARRAY){
        return t1->array_info.num == t2->array_info.num && type_eq(t1->array_info.type, t2->array_info.type);
    }
    if (kind==SYM_FUNC){
        if (!type_eq(t1->func_info.return_type,t2->func_info.return_type))
            return 0;
        if (t1->func_info.num!=t2->func_info.num)
            return 0;
        field_node *l1 = t1->func_info.param_types, *l2 = t2->func_info.param_types;
        while (l1&&l2){
            if (!type_eq(l1->type,l2->type))
                return 0;
            l1 = l1->next, l2 = l2->next;
        }
        return l1 == NULL && l2 == NULL;
    }
    if (kind==SYM_STRUCT){
        field_node *l1 = t1->struct_info, *l2 = t2->struct_info;
        while (l1&&l2){
            if (strcmp(l1->name,l2->name)!=0)
                return 0;
            if (!type_eq(l1->type,l2->type))
                return 0;
            l1 = l1->next, l2 = l2->next;
        }
        return l1 == NULL && l2 == NULL;
    }
    //assert(0);
}
int can_logic(sym_type* t){
    assert(t);
    return t->kind == SYM_BASIC && t->basic_info == SYM_INT;
}
/*
int main(){
    init_symtable();
    sym_entry a, b, c;
    strcpy(a.name, "a1");
    strcpy(b.name, "b1");
    strcpy(c.name, "c1");
    add_entry(&a);
    print_symtable();
    add_entry(&b);
    print_symtable();
    add_entry(&c);
    print_symtable();
}
*/