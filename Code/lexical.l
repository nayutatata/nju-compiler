%{
#include "type.h"
#include "syntax.tab.h"
int yycolumn=1;
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
{SEMI} {action("SEMI",yytext);return SEMI;}
{COMMA} {action("COMMA",yytext);return COMMA;}
"<" {
    yylval.opv = LT;
    return RELOP;
}
"<=" {
    yylval.opv=LE;
    return RELOP;
}
">" {
    yylval.opv=GT;
    return RELOP;
}
">=" {
    yylval.opv=GE;
    return RELOP;
}
"==" {
    yylval.opv=EQ;
    return RELOP;
}
"!=" {
    yylval.opv=NE;
    return RELOP;
}
{ASSIGNOP} {action("ASSIGNOP",yytext);return ASSIGNOP;}
{PLUS} {action("PLUS",yytext);return PLUS;}
{MINUS} {action("MINUS",yytext);return MINUS;}
{STAR} {action("STAR",yytext);return STAR;}
{DIV} {action("DIV",yytext);return DIV;}
{AND} {action("AND",yytext);return AND;}
{OR} {action("OR",yytext);return OR;}
{DOT} {action("DOT",yytext);return DOT;}
{NOT} {action("NOT",yytext);return NOT;}
{LP} {action("LP",yytext);return LP;}
{RP} {action("RP",yytext);return RP;}
{LB} {action("LB",yytext);return LB;}
{RB} {action("RB",yytext);return RB;}
{LC} {action("LC",yytext);return LC;}
{RC} {action("RC",yytext);return RC;}
{STRUCT} {action("STRUCT",yytext);return STRUCT;}
{RETURN} {action("RETURN",yytext);return RETURN;}
{IF} {action("IF",yytext);return IF;}
{ELSE} {action("ELSE",yytext);return ELSE;}
{WHILE} {action("WHILE",yytext);return WHILE;}
{TYPE} {action("TYPE",yytext);return TYPE;}
{INT_BASE_10} {action("INT",yytext);yylval.intv=atoi(yytext);return INT;}
{FLOAT} {action("FLOAT",yytext);yylval.floatv=atof(yytext);return FLOAT;}
{ID} {action("ID",yytext);return ID;}
"//".*\n{1} {
    action("comments","");
    yycolumn=1;
}
"/*" {
    char a,b;
    while (1){
        a=input();
        if (a==0){
            printf("Error type A at Line %d: Comment reaches EOF\n",yylineno);
            break;
        }
        else if  (a=='\n') yycolumn=1;
        else yycolumn++;
        b=input();
        if (b==0){
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
    printf("Error type A at Line %d: invalid indentifier \"%s\"\n",yylineno,yytext);
}
. {printf("Error type A at Line %d: Mysterious character \"%s\"\n",yylineno,yytext);}
%%
void action(char* a,char* contain){
    printf("TOKEN: %s  CONTAIN: %s  LINE_NUMBER: %d\n",a,contain,yylineno);
}



