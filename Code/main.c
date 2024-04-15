#include <stdio.h>
#include "ast.h"
#include <string.h>
#include "symboltable.h"
#include "semantic.h"
#include "ir.h"
#include <assert.h>
extern Node *root;
extern FILE *yyin;
int has_error = 0;
FILE* output = NULL;
int main(int argc, char** argv) {
    if (argc > 1) {
        if (!(yyin=fopen(argv[1],"r"))){
            perror(argv[1]);
            return 1;
        }
    }
    yyrestart(yyin);
    yyparse();
    if (!has_error){
        print_ast();
        init_symtable();
        init_frame();
        //init_typetable();
        add_other();
        semantic_analysis(root);
        //print_symtable();
        init_code_list();
        translate(root);
        if (argc>2)
            output = fopen(argv[2], "w");
        print_code_list(output);
    }
    return 0;
}