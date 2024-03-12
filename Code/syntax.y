%locations
%{
#include "lex.yy.c"
void yyerror(char* msg);
%}
%union{
    int intv;
    float floatv;
    reloptype opv;
    char ttype[10];
    char name[NAME_LEN];
}
%token SEMI
%token COMMA
%token ASSIGNOP
%token <ttype>RELOP
%token PLUS
%token MINUS
%token STAR
%token DIV
%token AND
%token OR
%token DOT
%token NOT
%token TYPE
%token LP
%token RP
%token LB
%token RB
%token LC
%token RC
%token STRUCT
%token RETURN
%token IF
%token ELSE
%token WHILE
%token <intv> INT
%token <floatv> FLOAT
%token <name>ID

%right ASSIGNOP
%left AND OR
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT NEG
%left LP RP LB RB DOT

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%
Program : ExtDefList
;
ExtDefList : Specifier ExDecList SEMI
| Specifier SEMI
| Specifier FunDec CompSt
;
ExDecList : VarDec
| VarDec COMMA ExDecList
;

Specifier : TYPE
| StructSpecifier
;
StructSpecifier : STRUCT OptTag LC DefList RC
| STRUCT Tag
;
OptTag : |
ID
;
Tag : ID
;

VarDec : ID
| VarDec LB INT RB
;
FunDec : ID LP VarList RP
| ID LP RP
;
VarList : ParamDec COMMA VarList
| ParamDec
;
ParamDec : Specifier VarDec
;

CompSt : LC DefList StmtList RC
;
StmtList : |
Stmt StmtList
;
Stmt : Exp SEMI
| CompSt
| RETURN Exp SEMI
| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
| IF LP Exp RP Stmt ELSE Stmt
| WHILE LP Exp RP Stmt
;

DefList : |
Def DefList
;
Def : Specifier DecList SEMI
;
DecList : Dec
| Dec COMMA DecList
;
Dec : VarDec
| VarDec ASSIGNOP Exp
;

Exp : Exp ASSIGNOP Exp
| Exp AND Exp
| Exp OR Exp
| Exp RELOP Exp
| Exp PLUS Exp
| Exp MINUS Exp
| Exp STAR Exp
| Exp DIV Exp
| LP Exp RP
| MINUS Exp %prec NEG
| NOT Exp
| ID LP Args RP
| ID LP RP
| Exp LB Exp RB
| Exp DOT ID
| ID
| INT
|FLOAT
;
Args : Exp COMMA Args
| Exp
;
%%
