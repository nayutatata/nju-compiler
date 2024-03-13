%locations
%{
#include "lex.yy.c"
#include "ast.h"
void yyerror();
int has_raise[1000];
extern Node* root;
extern int has_error;
#define addnc(n) \
    addchild($$,$##n)
%}
%union{
    Node* node;
}
%token<node> SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV AND OR DOT NOT TYPE LP RP LB RB LC RC STRUCT RETURN IF ELSE WHILE INT FLOAT ID
%type <node> Program ExtDefList ExtDef Specifier ExtDecList FunDec CompSt VarDec StructSpecifier OptTag DefList Tag VarList ParamDec StmtList Stmt Exp Def DecList Dec Args

%right ASSIGNOP
%left AND OR
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT NEG
%left LP RP LB RB DOT

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%start Program

%%
Program : ExtDefList{
    $$=normalNode(_Program);
    printf("line: %d\n",$$->nline);
    printf("loc: %d\n",@$.first_line);
    addchild($$,$1);
    root=$$;
}
| error {yyerror(),$$=NULL,root=NULL;}
;
ExtDefList : {
    $$=NULL;
}
| ExtDef ExtDefList{
    $$=normalNode(_ExtDefList);
    addchild($$,$1);
    addchild($$,$2);
}
| error {yyerror();$$=NULL;}
;
ExtDef : Specifier ExtDecList SEMI{
    $$=normalNode(_ExtDef);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| Specifier SEMI{
    $$=normalNode(_ExtDef);
    addchild($$,$1);
    addchild($$,$2);
}
| Specifier FunDec CompSt{
    $$=normalNode(_ExtDef);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| error {yyerror(),$$=NULL;}
;
ExtDecList : VarDec{
    $$=normalNode(_ExtDecList);
    addchild($$,$1);
}
| VarDec COMMA ExtDecList{
    $$=normalNode(_ExtDecList);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| error {yyerror(),$$=NULL;}
;

Specifier : TYPE{
    $$=normalNode(_Specifier);
    addchild($$,$1);
}
| StructSpecifier{
    $$=normalNode(_Specifier);
    addchild($$,$1);
}
| error {yyerror(),$$=NULL;}
;
StructSpecifier : STRUCT OptTag LC DefList RC{
    $$ = normalNode(_StructSpecifier);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
    addchild($$,$4);
    addchild($$,$5);
}
| STRUCT Tag{
    $$ = normalNode(_StructSpecifier);
    addchild($$,$1);
    addchild($$,$2);
}
| error {yyerror(),$$=NULL;}
;
OptTag : {
    $$=NULL;
}
|ID{
    $$=normalNode(_OptTag);
    addchild($$,$1);
}
| error {yyerror(),$$=NULL;}
;
Tag : ID{
    $$=normalNode(_Tag);
    addchild($$,$1);
}
| error {yyerror(),$$=NULL;}
;

VarDec : ID{
    $$=normalNode(_VarDec);
    addchild($$,$1);
}
| VarDec LB INT RB{
    $$=normalNode(_VarDec);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
    addchild($$,$4);
}
| error {yyerror(),$$=NULL;}
;
FunDec : ID LP VarList RP{
    $$=normalNode(_FunDec);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
    addchild($$,$4);
}
| ID LP RP{
    $$=normalNode(_FunDec);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| error {yyerror(),$$=NULL;}
;
VarList : ParamDec COMMA VarList{
    $$=normalNode(_VarList);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| ParamDec{
    $$=normalNode(_VarList);
    addchild($$,$1);
}
| error {yyerror(),$$=NULL;}
;
ParamDec : Specifier VarDec{
    $$=normalNode(_ParamDec);
    addchild($$,$1);
    addchild($$,$2);
}
| error {yyerror(),$$=NULL;}
;

CompSt : LC DefList StmtList RC{
    $$=normalNode(_CompSt);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
    addchild($$,$4);
}
| error {yyerror(),$$=NULL;}
;
StmtList : {
    $$=NULL;
}
| Stmt StmtList{
    $$=normalNode(_StmtList);
    addchild($$,$1);
    addchild($$,$2);
}
| error {yyerror(),$$=NULL;}
;
Stmt : Exp SEMI{
    $$=normalNode(_Stmt);
    addchild($$,$1);
    addchild($$,$2);
}
| CompSt{
    $$=normalNode(_Stmt);
    addchild($$,$1);
}
| RETURN Exp SEMI{
    $$=normalNode(_Stmt);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE{
    $$=normalNode(_Stmt);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
    addchild($$,$4);
    addchild($$,$5);
}
| IF LP Exp RP Stmt ELSE Stmt{
    $$=normalNode(_Stmt);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
    addchild($$,$4);
    addchild($$,$5);
    addchild($$,$6);
    addchild($$,$7);
}
| WHILE LP Exp RP Stmt{
    $$=normalNode(_Stmt);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
    addchild($$,$4);
    addchild($$,$5);
}
| error {yyerror(),$$=NULL;}
;

DefList : {
    $$=NULL;
}
| Def DefList{
    $$=normalNode(_DefList);
    addchild($$,$1);
    addchild($$,$2);
}
| error {yyerror(),$$=NULL;}
;
Def : Specifier DecList SEMI{
    $$=normalNode(_Def);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| error {yyerror(),$$=NULL;}
;
DecList : Dec{
    $$=normalNode(_DecList);
    addchild($$,$1);
}
| Dec COMMA DecList{
    $$=normalNode(_DecList);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| error {yyerror(),$$=NULL;}
;
Dec : VarDec{
    $$=normalNode(_Dec);
    addchild($$,$1);
}
| VarDec ASSIGNOP Exp{
    $$=normalNode(_Dec);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| error {yyerror(),$$=NULL;}
;

Exp : Exp ASSIGNOP Exp{
    $$=normalNode(_Exp);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| Exp AND Exp{
    $$=normalNode(_Exp);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| Exp OR Exp{
    $$=normalNode(_Exp);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| Exp RELOP Exp{
    $$=normalNode(_Exp);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| Exp PLUS Exp{
    $$=normalNode(_Exp);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| Exp MINUS Exp{
    $$=normalNode(_Exp);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| Exp STAR Exp{
    $$=normalNode(_Exp);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| Exp DIV Exp{
    $$=normalNode(_Exp);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| LP Exp RP{
    $$=normalNode(_Exp);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| MINUS Exp %prec NEG{
    $$=normalNode(_Exp);
    addchild($$,$1);
    addchild($$,$2);
}
| NOT Exp{
    $$=normalNode(_Exp);
    addchild($$,$1);
    addchild($$,$2);
}
| ID LP Args RP{
    $$=normalNode(_Exp);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
    addchild($$,$4);
}
| ID LP RP{
    $$=normalNode(_Exp);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| Exp LB Exp RB{
    $$=normalNode(_Exp);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
    addchild($$,$4);
}
| Exp DOT ID{
    $$=normalNode(_Exp);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| ID{
    $$=normalNode(_Exp);
    addchild($$,$1);
}
| INT{
    $$=normalNode(_Exp);
    addchild($$,$1);
}
| FLOAT{
    $$=normalNode(_Exp);
    addchild($$,$1);
}
| error {yyerror(),$$=NULL;}
;
Args : Exp COMMA Args{
    $$=normalNode(_Args);
    addchild($$,$1);
    addchild($$,$2);
    addchild($$,$3);
}
| Exp{
    $$=normalNode(_Args);
    addchild($$,$1);
}
| error {yyerror(),$$=NULL;}
;
%%
void yyerror(){
    has_error=1;
    if (!has_raise[yylloc.first_line])
        printf("error type B at Line %d.\n",yylloc.first_line),has_raise[yylloc.first_line]=1;
}