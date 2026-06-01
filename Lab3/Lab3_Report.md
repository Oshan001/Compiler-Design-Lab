# LAB 3: Token Counter for Identifiers, Keywords & Operators Using Flex

## Objective

The objective of this lab is to develop a lexical analyzer using Flex that scans input source code and:

- Identifies tokens such as keywords, identifiers, numbers, operators, strings, and comments.
- Counts how many of each token type appears in the given input.

---

## Theory

Lexical Analysis is the first phase of a compiler. It reads the source code character by character and groups them into meaningful units called **tokens**. In this experiment, we go one step further — we not only recognize tokens but also **count their frequency**, which is useful for later compiler stages such as optimization and symbol table generation.

**Flex (Fast Lexical Analyzer Generator)** allows us to define token patterns using regular expressions. Each pattern is associated with an action block `{ ... }` that executes when a match is found.

The lexical analyzer identifies and counts:

- Keywords (`int`, `float`, `if`, `while`, etc.)
- Identifiers (variable/function names)
- Numbers (integers and floating-point)
- Strings (double-quoted literals)
- Logical Operators (`&&`, `||`)
- Operators (`+`, `-`, `=`, `==`, `<=`, etc.)
- Delimiters (`{`, `}`, `;`, `,`, `(`, `)`, etc.)
- Comments (`//` single-line and `/* */` multi-line)
- Unknown characters

---

## Requirements

- macOS / Linux / Windows (MinGW)
- Flex (Lexical Analyzer Generator)
- GCC C Compiler

### Installation

**On Windows:**
> Use [MinGW](https://www.mingw-w64.org/) and install Flex via the package manager or download a pre-built binary.

---

## Program

### File: `token_counter.l`

```c
%{
#include <stdio.h>

int keywordCount    = 0;
int identifierCount = 0;
int numberCount     = 0;
int stringCount     = 0;
int logicalOpCount  = 0;
int operatorCount   = 0;
int delimiterCount  = 0;
int commentCount    = 0;
int unknownCount    = 0;
%}

%%

\/\/[^\n]*                          { commentCount++;    printf("COMMENT: %s\n",     yytext); }
\/\*([^*]|\*+[^*/])*\*+\/           { commentCount++;    printf("COMMENT: %s\n",     yytext); }

int|float|char|double|void|if|else|while|for|return|do|break|continue|switch|case|default
                                    { keywordCount++;    printf("KEYWORD: %s\n",     yytext); }

[a-zA-Z_][a-zA-Z0-9_]*             { identifierCount++; printf("IDENTIFIER: %s\n",  yytext); }

[0-9]+(\.[0-9]+)?                   { numberCount++;     printf("NUMBER: %s\n",      yytext); }

\"([^\"\\]|\\.)*\"                  { stringCount++;     printf("STRING: %s\n",      yytext); }

&&|\|\|                             { logicalOpCount++;  printf("LOGICAL_OP: %s\n",  yytext); }

==|!=|<=|>=|<|>|[+\-*\/%]=?|=      { operatorCount++;   printf("OPERATOR: %s\n",    yytext); }

[{};,\[\]()]                        { delimiterCount++;  printf("DELIMITER: %s\n",   yytext); }

[ \t\n]+                            ;

.                                   { unknownCount++;    printf("UNKNOWN: %s\n",     yytext); }

%%

int main() {
    printf("Enter the code (Ctrl+Z then Enter to end input on Windows / Ctrl+D on Linux):\n");
    yylex();

    printf("\n-- Token Counts --\n");
    printf("Keywords    : %d\n", keywordCount);
    printf("Identifiers : %d\n", identifierCount);
    printf("Numbers     : %d\n", numberCount);
    printf("Strings     : %d\n", stringCount);
    printf("Logical Ops : %d\n", logicalOpCount);
    printf("Operators   : %d\n", operatorCount);
    printf("Delimiters  : %d\n", delimiterCount);
    printf("Comments    : %d\n", commentCount);
    printf("Unknown     : %d\n", unknownCount);
    return 0;
}

int yywrap() { return 1; }
```

---

## Steps to Execute

### Step 1: Save the file
Save the program as `token_counter.l`.

### Step 2: Generate C source file using Flex
```bash
flex token_counter.l
```
> This generates `lex.yy.c`

### Step 3: Compile the generated C file
```bash
gcc lex.yy.c -o token_counter
```
./
### Step 4: Run the program
```bash
./token_counter
```
> OR :
> ```bash
> token_counter.exe
> ```

---

## Sample Input

```c
int x = 5;
if (x > 2) x = x + 1;
```

---

## Sample Output

```bash
KEYWORD: int
IDENTIFIER: x
OPERATOR: =
NUMBER: 5
DELIMITER: ;
KEYWORD: if
DELIMITER: (
IDENTIFIER: x
OPERATOR: >
NUMBER: 2
DELIMITER: )
IDENTIFIER: x
OPERATOR: =
IDENTIFIER: x
OPERATOR: +
NUMBER: 1
DELIMITER: ;

-- Token Counts --
Keywords    : 2
Identifiers : 4
Numbers     : 3
Strings     : 0
Logical Ops : 0
Operators   : 4
Delimiters  : 5
Comments    : 0
Unknown     : 0
```

---

## Result

The lexical analyzer was successfully implemented using Flex. The program correctly:

- Detected and printed each token in real time as it was scanned.
- Counted all token types including keywords, identifiers, numbers, operators, delimiters, strings, comments, and unknown characters.
- Displayed a complete token count summary at the end of execution.

---

## Conclusion

This lab provided hands-on experience with Flex for building a token counter as part of compiler design. Key takeaways include:

- Understanding how regular expressions are used to define and detect various token types.
- Learning how Flex action blocks `{ ... }` can be used to maintain counters and produce real-time output.
- Gaining insight into how lexical analysis feeds into later stages of a compiler such as symbol table generation and optimization.
