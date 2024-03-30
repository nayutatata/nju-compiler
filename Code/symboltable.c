#include "symboltable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
static char* print_table[] = {
    "BASIC","ARRAY","STRUCT","FUNC"
};
sym_entry* find_entry(char* name) {
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
sym_entry* add_entry(sym_entry* entry){
    sym_entry *found = find_entry(entry->name);
    if (found)
        return found;
    entry->next = symtable->next;
    symtable->next = entry;
    return NULL;
}
void print_symtable(){
    sym_entry *head = symtable->next;
    while (head){
        printf("name: %s, type: %s, line: %d\n", head->name,print_table[head->type->kind],head->line);
        head = head->next;
    }
}
void init_symtable(){
    symtable = malloc(sizeof(sym_entry)); // dummy node
    symtable->next = NULL;
}
sym_entry* new_sym_entry(Node* node,sym_type* type){
    sym_entry* res = malloc(sizeof(sym_entry));
    res->line = node->nline;
    res->col = node->ncol;
    if (node->ntype == _ID)
        strcpy(res->name, node->val.name);
    res->type = type;
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