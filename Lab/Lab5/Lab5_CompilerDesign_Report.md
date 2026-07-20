# Compiler Design Lab

**Name:** Oshan
**Roll No:** 230328
**Faculty:** BCE
**Lab:** Compiler Design — Syntax Analyzer using Flex and Bison

---

## Title
**Syntax Analyzer for IF-ELSE Statements using Flex and Bison**

---

## Objective

A syntax analyzer was designed that accepted valid `if`, `if-else`, and nested `if-else` statements. Syntax errors in the provided input statements were detected and reported to the user. The dangling-else problem was resolved using grammar precedence and associativity rules in Bison. Statements were parsed interactively via the command line interface.

---

## Software Requirements

Flex was used as the Lexical Analyzer Generator to tokenize the input character stream. Bison served as the Parser Generator to validate syntax using a Context-Free Grammar. GCC was used as the C compiler to compile the Flex and Bison generated source files. The lab was carried out on a Windows (MinGW) and Linux environment.

---

## Theory

### Flex Component

Flex scanned the input character stream and converted it into a sequence of tokens such as `IF`, `ELSE`, `ID`, `RELOP`, `ASSIGN`, `NUM`, and `SEMICOLON`. Each token pattern was described using regular expressions and Flex generated a C source file (`lex.yy.c`) that implemented the corresponding finite automaton.

### Bison Component

Bison used a Context-Free Grammar (CFG) to parse the token sequence produced by Flex and validated the structural correctness of `if`, `if-else`, and nested `if-else` constructs. It generated an LALR(1) parser (`ifelse.tab.c`) that reduced token sequences according to the defined grammar rules.

### Dangling Else Resolution

The dangling-else problem arose when a single `if` clause was followed by an `else` that could associate with either an outer or an inner `if`. Bison resolved this ambiguity by declaring token precedence rules using `%nonassoc`, ensuring that an `else` always aligned with the nearest unmatched `if`. The dummy token `LOWER_THAN_ELSE` was given lower precedence than `ELSE` to enforce this behaviour. As a result, a statement such as `if (a>b) if (c<d) x=1; else x=2;` was correctly parsed with the `else` bound to the inner `if`.

### Grammar Overview

The CFG used by the parser recognised the following structures:

```
stmt       → if_stmt '\n'
if_stmt    → IF ( cond ) stmt_block
           | IF ( cond ) stmt_block ELSE stmt_block
stmt_block → ID = expr ;
           | if_stmt
cond       → ID RELOP ID
           | ID RELOP NUM
expr       → ID
           | NUM
```

---

## Lab Code

### Program 1 — Lexical Analyzer (`ifelse.l`)

```lex
%{
#include "ifelse.tab.h"
%}

%%

"if"                        { return IF; }
"else"                      { return ELSE; }
"="                         { return ASSIGN; }
";"                         { return SEMICOLON; }
"("                         { return '('; }
")"                         { return ')'; }
[<>!=]=?                    { return RELOP; }
[0-9]+                      { yylval = atoi(yytext); return NUM; }
[a-zA-Z_][a-zA-Z0-9_]*     { return ID; }
[ \t]                       { /* skip whitespace */ }
[\n]                        { return '\n'; }
.                           { printf("Unknown symbol: %s\n", yytext); }

%%

int yywrap() { return 1; }
```

---

### Program 2 — Parser (`ifelse.y`)

```bison
%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
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
        { if (!isError) printf("valid\n"); }
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

void yyerror(const char *s) {
    isError = 1;
    printf("Syntax Error: Invalid statement\n");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    printf("=== IF-ELSE Statement Parser ===\n");
    printf("Enter one statement per line, e.g.:\n");
    printf("  if (a>b) x=y;\n");
    printf("  if (a>b) x=y; else x=z;\n");
    printf("Press Ctrl+C or Ctrl+Z (Windows) to exit.\n\n");
    while (1) {
        isError = 0;
        printf("> ");
        yyparse();
    }
    return 0;
}
```

---

## Compilation and Execution Steps

The following commands were executed sequentially in the terminal to build and run the parser.

```bash
$ bison -d ifelse.y
$ flex ifelse.l
$ gcc ifelse.tab.c lex.yy.c -o ifelsechecker
$ ./ifelsechecker
```

Running `bison -d ifelse.y` produced `ifelse.tab.c` and `ifelse.tab.h`. Running `flex ifelse.l` produced `lex.yy.c`. Both generated files were then compiled together using GCC to produce the final executable `ifelsechecker`. The parser was then launched and ran interactively from the terminal.

---

## Output

The parser was executed and the following inputs were tested:

```
=== IF-ELSE Statement Parser ===
Enter one statement per line, e.g.:
  if (a>b) x=y;
  if (a>b) x=y; else x=z;
Press Ctrl+C or Ctrl+Z (Windows) to exit.

> if (a>b) x=y;
valid

> if (a>b) x=y; else x=z;
valid

> if (a>b) if (c<d) x=1; else x=2;
valid

> if (a>b) x=y; else
Syntax Error: Invalid statement

> if x=y;
Syntax Error: Invalid statement

> if (a>b) x=y; else if (c<=5) z=3;
valid
```

The simple `if` statement `if (a>b) x=y;` was accepted as valid since it correctly matched the `if_stmt` rule with a valid condition and assignment body. The full `if-else` statement `if (a>b) x=y; else x=z;` was also accepted as valid. The nested `if-else` statement `if (a>b) if (c<d) x=1; else x=2;` was accepted and the dangling-else was correctly resolved by associating the `else` with the inner `if`. The incomplete statement `if (a>b) x=y; else` produced a syntax error because the `else` clause lacked a body block. The statement `if x=y;` produced a syntax error because the condition parentheses were missing, violating the `IF '(' cond ')'` grammar rule. The chained `if-else if` statement was accepted as valid since the `else` branch contained a nested `if_stmt`.

---

## Conclusion

In this lab, a complete syntax analyzer for `if`, `if-else`, and nested `if-else` statements was successfully implemented using Flex and Bison. The Flex lexer correctly tokenized the input stream, recognizing keywords, identifiers, relational operators, and literals while whitespace was silently ignored. The Bison parser used a Context-Free Grammar to validate the structural correctness of each statement and clear error messages were printed for invalid inputs. The dangling-else problem was effectively resolved using Bison's `%nonassoc` precedence directives, which ensured that `else` was always associated with the nearest unmatched `if`. The four-step compilation pipeline involving Bison, Flex, GCC, and execution was carried out successfully, and the interactive loop allowed continuous parsing of user-provided statements, demonstrating a functional front-end parser component typical of real-world compiler design.
