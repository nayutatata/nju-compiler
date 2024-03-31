#include <stdio.h>
#include "ast.h"
#include "symboltable.h"
#include "semantic.h"
#include <assert.h>
extern Node *root;
extern FILE *yyin;
int has_error = 0;
int main(int argc, char **argv)
{
    if (argc>1){
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
        init_typetable();
        semantic_analysis(root);
        print_symtable();
    }
    return 0;
}