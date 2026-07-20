%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
int yyparse(void);
void yyerror(const char *s);

int isError = 0;
%}

%token IF ELSE ID RELOP ASSIGN NUM SEMICOLON

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%start stmt

%%

stmt
    : if_stmt '\n'
      {
          if (!isError)
              printf("Valid\n");
      }
    ;

if_stmt
    : IF '(' cond ')' stmt_block %prec LOWER_THAN_ELSE
    | IF '(' cond ')' stmt_block ELSE stmt_block
    ;

stmt_block
    : ID ASSIGN expr SEMICOLON
    | if_stmt
    ;

cond
    : ID RELOP ID
    | ID RELOP NUM
    ;

expr
    : ID
    | NUM
    ;

%%

void yyerror(const char *s)
{
    int c;
    isError = 1;

    printf("Syntax Error: Invalid statement\n");

    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int main(void)
{
    printf("=== IF-ELSE Statement Parser ===\n");
    printf("Enter one statement per line, e.g.:\n");
    printf("if (a>b) x=y;\n");
    printf("if (a>b) x=y; else x=z;\n");
    printf("Press Ctrl+C (Linux) or Ctrl+Z (Windows) to exit.\n");

    while (1)
    {
        isError = 0;
        printf("> ");
        yyparse();
    }

    return 0;
}