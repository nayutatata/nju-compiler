%{
#include "type.h"
#include "ast.h"
#include "syntax.tab.h"
//#define debug
int yycolumn=1;
extern int has_error;
#define YY_USER_ACTION\
    yylloc.first_line=yylloc.last_line=yylineno;\
    yylloc.first_column=yycolumn;\
    yylloc.last_column=yycolumn+yyleng-1;\
    yycolumn+=yyleng;
void action(char*,char*);
%}
%option yylineno
digit [0-9]
letter [_a-zA-Z]
INT_BASE_10 0|[1-9][0-9]*
FLOAT {INT_BASE_10}|{INT_BASE_10}\.[0-9]+
ID {letter}({digit}|{letter})*
SEMI ;
COMMA ,
ASSIGNOP =
RELOP >|<|>=|<=|==|!=
PLUS \+
MINUS \-
STAR \*
DIV \/
AND &&
OR \|\|
DOT \.
NOT !
TYPE int|float
LP \(
RP \)
LB \[
RB \]
LC \{
RC \}
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
SPACE [\r\t ]

%%
\n {yycolumn=1;}
{SPACE} {;}
{SEMI} {action("SEMI",yytext);yylval.node=normalNode(_SEMI);return SEMI;}
{COMMA} {action("COMMA",yytext);yylval.node=normalNode(_COMMA);return COMMA;}
"<" {
    yylval.node=relNode(LT);
    return RELOP;
}
"<=" {
    yylval.node=relNode(LE);
    return RELOP;
}
">" {
    yylval.node=relNode(GT);
    return RELOP;
}
">=" {
    yylval.node=relNode(GE);
    return RELOP;
}
"==" {
    yylval.node=relNode(EQ);
    return RELOP;
}
"!=" {
    yylval.node=relNode(NE);
    return RELOP;
}
{ASSIGNOP} {action("ASSIGNOP",yytext);yylval.node=normalNode(_ASSIGNOP);return ASSIGNOP;}
{PLUS} {action("PLUS",yytext);yylval.node=normalNode(_PLUS);return PLUS;}
{MINUS} {action("MINUS",yytext);yylval.node=normalNode(_MINUS);return MINUS;}
{STAR} {action("STAR",yytext);yylval.node=normalNode(_STAR);return STAR;}
{DIV} {action("DIV",yytext);yylval.node=normalNode(_DIV);return DIV;}
{AND} {action("AND",yytext);yylval.node=normalNode(_AND);return AND;}
{OR} {action("OR",yytext);yylval.node=normalNode(_OR);return OR;}
{DOT} {action("DOT",yytext);yylval.node=normalNode(_DOT);return DOT;}
{NOT} {action("NOT",yytext);yylval.node=normalNode(_NOT);return NOT;}
{LP} {action("LP",yytext);yylval.node=normalNode(_LP);return LP;}
{RP} {action("RP",yytext);yylval.node=normalNode(_RP);return RP;}
{LB} {action("LB",yytext);yylval.node=normalNode(_LB);return LB;}
{RB} {action("RB",yytext);yylval.node=normalNode(_RB);return RB;}
{LC} {action("LC",yytext);yylval.node=normalNode(_LC);return LC;}
{RC} {action("RC",yytext);yylval.node=normalNode(_RC);return RC;}
{STRUCT} {action("STRUCT",yytext);yylval.node=normalNode(_STRUCT);return STRUCT;}
{RETURN} {action("RETURN",yytext);yylval.node=normalNode(_RETURN);return RETURN;}
{IF} {action("IF",yytext);yylval.node=normalNode(_IF);return IF;}
{ELSE} {action("ELSE",yytext);yylval.node=normalNode(_ELSE);return ELSE;}
{WHILE} {action("WHILE",yytext);yylval.node=normalNode(_WHILE);return WHILE;}
{TYPE} {
    action("TYPE",yytext);
    if (strcmp(yytext,"int")==0) yylval.node=typeNode(_INT);
    else yylval.node=typeNode(_FLOAT);
    return TYPE;
}
{INT_BASE_10} {action("INT",yytext);yylval.node=intNode(atoi(yytext));return INT;}
{FLOAT} {action("FLOAT",yytext);yylval.node=floatNode(atof(yytext));return FLOAT;}
{ID} {action("ID",yytext);yylval.node=idNode(yytext);return ID;}
"//".*\n{1} {
    action("comments","");
    yycolumn=1;
}
"/*" {
    char a,b;
    while (1){
        a=input();
        if (a==0){
            has_error=1;
            printf("Error type A at Line %d: Comment reaches EOF\n",yylineno);
            break;
        }
        else if  (a=='\n') yycolumn=1;
        else yycolumn++;
        if (a!='*') continue;
        b=input();
        if (b==0){
            has_error=1;
            printf("Error type A at Line %d: Comment reaches EOF\n",yylineno);
            break;
        }
        else if (b=='\n') yycolumn=1;
        else yycolumn++;
        if (a=='*'&&b=='/'){
            break;
        }
    }
}
{digit}+{letter}+ {
    has_error=1;
    printf("Error type A at Line %d: invalid indentifier \"%s\"\n",yylineno,yytext);
}
. {has_error=1;printf("Error type A at Line %d: Mysterious character \"%s\"\n",yylineno,yytext);}
%%
void action(char* a,char* contain){
#ifdef debug
    printf("TOKEN: %s  CONTAIN: %s  LINE_NUMBER: %d\n",a,contain,yylloc.first_line);
#else
    return;
#endif
}



