%{
#include <stdio.h>
#include <string.h>
int line_no = 1;
%}
DIGIT [0-9]
ID [a-zA-Z_][a-zA-Z0-9_]*
KEYWORD int|float|if|else|return
OP ==|!=|<=|>=|\+|\-|\*|\/|>|=
SEPARATOR [;,(){}]

%%
{KEYWORD} { printf("Keyword: %s\n"
, yytext); }
{ID} { printf("Identifier: %s\n"
, yytext); }
{DIGIT}+ { printf("Number: %s\n"
, yytext); }
{OP} { printf("Operator: %s\n"
, yytext); }
{SEPARATOR} { printf("Separator: %s\n"
, yytext); }
\n { line_no++; }
[ \t] ;
. { printf("Unknown symbol: %s\n"
, yytext); }
%%
int main() {
yylex();
return 0;
printf("Enter the code (Ctrl+Z or Ctrl D to end input):\n");
}
int yywrap() {
return 1;
}