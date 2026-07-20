# LAB 2: Lexical Analyzer for Token Recognition Using Flex (Lex)

## Objective

The objective of this lab is to learn the basics of lexical analysis using Flex (Lex) and identify different tokens such as keywords, identifiers, numbers, operators, and separators from source code input.

---

## Theory

Lexical Analysis is the first phase of a compiler. It reads the source code character by character and converts it into meaningful tokens. Flex (Fast Lexical Analyzer Generator) is a tool used to generate lexical analyzers.

The lexical analyzer identifies:

- Keywords
- Identifiers
- Numbers
- Operators
- Separators

---

## Requirements

- macOS / Linux / Windows
- Flex (Lex)
- GCC Compiler

### Installation on macOS

```bash
brew install flex bison
```

---

## Steps to Execute

### Step 1: Save the program file as <name>.l
### Step 2: Generate C Source File ```flex <name>.l``` -> This generates: lex.yy.c
### Step 3: Compile the Program ```gcc lex.yy.c -o <some_name> -lfl``` -> If -lfl gives an error: ```gcc lex.yy.c -o lexer```
### Step 4: Run the Program -> ```./<some_name>

--- 

## Sample Input
```c
int a = 10;

if(a > 5){
    return a;
}
```
## Sample Output

```bash
Keyword: int
Identifier: a
Operator: =
Number: 10
Separator: ;

Keyword: if
Separator: (
Identifier: a
Operator: >
Number: 5
Separator: )

Separator: {
Keyword: return
Identifier: a
Separator: ;
Separator: }
```

## Result
The lexical analyzer was successfully implemented using Flex (Lex). The program correctly identified keywords, identifiers, numbers, operators, and separators from the input source code.

## Conclusion
This lab helped in understanding the working principle of lexical analysis and the use of Flex for generating lexical analyzers in compiler design.