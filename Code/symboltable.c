#include "symboltable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
static char* print_table[] = {
    "BASIC","ARRAY","STRUCT","FUNC","TYPE","EMPTY"
};
sym_entry* find_sym_entry(char* name) {
    assert(name != NULL);
    sym_entry *head = symtable->next;
    while (head){
        if (strcmp(name,head->name)==0){
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
    sym_entry *found = find_sym_entry(entry->name);
    if (found)
        return found;
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
        printf("name: %s, type: %s, line: %d\n", head->name, print_table[head->type->kind], head->line);
        head = head->next;
    }
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