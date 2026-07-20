# Compiler Design Lab

**Name:** Oshan
**Roll No:** 230328
**Faculty:** BCE
**Lab:** Compiler Design — FIRST and FOLLOW Set Computation

---

## Title
**Automatic Computation of FIRST and FOLLOW Sets for Context-Free Grammar**

---

## Objective

A C program was designed and implemented that automatically computed the FIRST and FOLLOW sets for the non-terminal symbols of a given Context-Free Grammar. The mathematical framework necessary for constructing predictive top-down LL(1) and bottom-up LR(1)/LALR(1) parsing tables was established through this experiment.

---

## Theory

### FIRST Sets

During syntax analysis, the compiler checked whether the tokens produced by the lexical analyzer satisfied the structural rules defined by the grammar. Computing FIRST sets allowed a deterministic parser to look ahead at incoming tokens and select the correct production branch without backtracking.

FIRST(X) was defined as the set of all terminal symbols that could appear as the first symbol of any string derived from X. If X could derive an empty string (ε), then ε was also added to FIRST(X), represented as `e` in the program.

The algorithm initialized FIRST(X) as an empty set for all non-terminals and iterated through the production rules until no new symbols were added in a complete sweep. For a production X → tα where t was a terminal, t was added to FIRST(X). For X → e, epsilon was added directly. For X → Yα where Y was a non-terminal, all symbols from FIRST(Y) excluding epsilon were merged into FIRST(X), and if FIRST(Y) contained epsilon, the next symbol in α was evaluated. If all consecutive symbols in the body could derive epsilon, epsilon was appended to FIRST(X).

### FOLLOW Sets

FOLLOW(A) was defined as the set of all terminal symbols that could appear immediately to the right of the non-terminal A in some valid sentential form. The end-marker symbol `$` was automatically assigned to the start symbol's FOLLOW set.

The algorithm seeded the start symbol by adding `$` into its FOLLOW set and initialized all other non-terminal FOLLOW sets as empty. It then iterated sweeps through all rules until no sets changed. For each production A → αBβ, all terminals from FIRST(β) excluding epsilon were added into FOLLOW(B). If β could derive epsilon or B appeared at the trailing boundary of the rule, everything in FOLLOW(A) was merged into FOLLOW(B).

---

## Lab Code

### Program 1 — FIRST and FOLLOW Computation (Original Grammar)

Grammar used:
- S → CB
- C → cC | d
- B → bC

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_RULES 20
#define MAX_PROD 10
#define MAX_SYMBOLS 10

typedef struct {
    char head;
    char body[MAX_PROD][MAX_SYMBOLS];
    int prod_count;
} GrammarRule;

GrammarRule grammar[MAX_RULES];
int rule_count = 0;

char first_sets[26][MAX_SYMBOLS * 2];
char follow_sets[26][MAX_SYMBOLS * 2];

bool is_non_terminal(char symbol) {
    return (symbol >= 'A' && symbol <= 'Z');
}

bool add_to_set(char set[], char val) {
    int len = strlen(set);
    for (int i = 0; i < len; i++) {
        if (set[i] == val) return false;
    }
    set[len] = val;
    set[len + 1] = '\0';
    return true;
}

bool merge_sets(char set1[], char set2[], bool ignore_epsilon) {
    bool changed = false;
    int len2 = strlen(set2);
    for (int i = 0; i < len2; i++) {
        if (ignore_epsilon && set2[i] == 'e') continue;
        if (add_to_set(set1, set2[i])) changed = true;
    }
    return changed;
}

bool has_epsilon(char set[]) {
    for (int i = 0; set[i] != '\0'; i++) {
        if (set[i] == 'e') return true;
    }
    return false;
}

int find_rule_index(char head) {
    for (int i = 0; i < rule_count; i++) {
        if (grammar[i].head == head) return i;
    }
    return -1;
}

void compute_first() {
    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < rule_count; i++) {
            char head = grammar[i].head;
            int idx = head - 'A';
            for (int j = 0; j < grammar[i].prod_count; j++) {
                char *prod = grammar[i].body[j];
                int prod_len = strlen(prod);
                if (prod[0] == 'e') {
                    if (add_to_set(first_sets[idx], 'e')) changed = true;
                } else {
                    bool next_symbol = true;
                    for (int k = 0; k < prod_len && next_symbol; k++) {
                        char sym = prod[k];
                        if (!is_non_terminal(sym)) {
                            if (add_to_set(first_sets[idx], sym)) changed = true;
                            next_symbol = false;
                        } else {
                            int sym_idx = sym - 'A';
                            if (merge_sets(first_sets[idx], first_sets[sym_idx], true)) changed = true;
                            if (!has_epsilon(first_sets[sym_idx])) next_symbol = false;
                        }
                    }
                    if (next_symbol) {
                        if (add_to_set(first_sets[idx], 'e')) changed = true;
                    }
                }
            }
        }
    }
}

void compute_follow(char start_symbol) {
    add_to_set(follow_sets[start_symbol - 'A'], '$');
    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < rule_count; i++) {
            char head = grammar[i].head;
            int head_idx = head - 'A';
            for (int j = 0; j < grammar[i].prod_count; j++) {
                char *prod = grammar[i].body[j];
                int prod_len = strlen(prod);
                for (int k = 0; k < prod_len; k++) {
                    char current_sym = prod[k];
                    if (is_non_terminal(current_sym)) {
                        int cur_idx = current_sym - 'A';
                        bool next_symbol = true;
                        for (int n = k + 1; n < prod_len && next_symbol; n++) {
                            char next_sym = prod[n];
                            if (!is_non_terminal(next_sym)) {
                                if (add_to_set(follow_sets[cur_idx], next_sym)) changed = true;
                                next_symbol = false;
                            } else {
                                int next_idx = next_sym - 'A';
                                if (merge_sets(follow_sets[cur_idx], first_sets[next_idx], true)) changed = true;
                                if (!has_epsilon(first_sets[next_idx])) next_symbol = false;
                            }
                        }
                        if (next_symbol) {
                            if (merge_sets(follow_sets[cur_idx], follow_sets[head_idx], false)) changed = true;
                        }
                    }
                }
            }
        }
    }
}

int main() {
    for (int i = 0; i < 26; i++) {
        first_sets[i][0] = '\0';
        follow_sets[i][0] = '\0';
    }

    // S -> CB
    grammar[rule_count].head = 'S';
    strcpy(grammar[rule_count].body[0], "CB");
    grammar[rule_count].prod_count = 1;
    rule_count++;

    // C -> cC | d
    grammar[rule_count].head = 'C';
    strcpy(grammar[rule_count].body[0], "cC");
    strcpy(grammar[rule_count].body[1], "d");
    grammar[rule_count].prod_count = 2;
    rule_count++;

    // B -> bC
    grammar[rule_count].head = 'B';
    strcpy(grammar[rule_count].body[0], "bC");
    grammar[rule_count].prod_count = 1;
    rule_count++;

    char start_symbol = 'S';

    compute_first();
    compute_follow(start_symbol);

    printf("--- FIRST SETS ---\n");
    for (int i = 0; i < rule_count; i++) {
        char head = grammar[i].head;
        printf("FIRST(%c) = { ", head);
        char *s = first_sets[head - 'A'];
        for (int j = 0; s[j] != '\0'; j++) {
            printf("%c%s", s[j], (s[j+1] != '\0') ? ", " : "");
        }
        printf(" }\n");
    }

    printf("\n--- FOLLOW SETS ---\n");
    for (int i = 0; i < rule_count; i++) {
        char head = grammar[i].head;
        printf("FOLLOW(%c) = { ", head);
        char *s = follow_sets[head - 'A'];
        for (int j = 0; s[j] != '\0'; j++) {
            printf("%c%s", s[j], (s[j+1] != '\0') ? ", " : "");
        }
        printf(" }\n");
    }

    return 0;
}
```

---

### Program 2 — Task A: Modified Grammar (Arithmetic Expression Grammar)

Grammar used (single-character mapping applied):
- E → TA  (E' mapped to A)
- A → +TA | e
- T → FB  (T' mapped to B)
- B → \*FB | e
- F → (E) | i  (id mapped to i)

```c
int main() {
    for (int i = 0; i < 26; i++) {
        first_sets[i][0] = '\0';
        follow_sets[i][0] = '\0';
    }

    // E -> TA
    grammar[rule_count].head = 'E';
    strcpy(grammar[rule_count].body[0], "TA");
    grammar[rule_count].prod_count = 1;
    rule_count++;

    // A -> +TA | e   (A represents E')
    grammar[rule_count].head = 'A';
    strcpy(grammar[rule_count].body[0], "+TA");
    strcpy(grammar[rule_count].body[1], "e");
    grammar[rule_count].prod_count = 2;
    rule_count++;

    // T -> FB
    grammar[rule_count].head = 'T';
    strcpy(grammar[rule_count].body[0], "FB");
    grammar[rule_count].prod_count = 1;
    rule_count++;

    // B -> *FB | e   (B represents T')
    grammar[rule_count].head = 'B';
    strcpy(grammar[rule_count].body[0], "*FB");
    strcpy(grammar[rule_count].body[1], "e");
    grammar[rule_count].prod_count = 2;
    rule_count++;

    // F -> (E) | i
    grammar[rule_count].head = 'F';
    strcpy(grammar[rule_count].body[0], "(E)");
    strcpy(grammar[rule_count].body[1], "i");
    grammar[rule_count].prod_count = 2;
    rule_count++;

    char start_symbol = 'E';

    compute_first();
    compute_follow(start_symbol);

    printf("--- FIRST SETS ---\n");
    for (int i = 0; i < rule_count; i++) {
        char head = grammar[i].head;
        printf("FIRST(%c) = { ", head);
        char *s = first_sets[head - 'A'];
        for (int j = 0; s[j] != '\0'; j++) {
            printf("%c%s", s[j], (s[j+1] != '\0') ? ", " : "");
        }
        printf(" }\n");
    }

    printf("\n--- FOLLOW SETS ---\n");
    for (int i = 0; i < rule_count; i++) {
        char head = grammar[i].head;
        printf("FOLLOW(%c) = { ", head);
        char *s = follow_sets[head - 'A'];
        for (int j = 0; s[j] != '\0'; j++) {
            printf("%c%s", s[j], (s[j+1] != '\0') ? ", " : "");
        }
        printf(" }\n");
    }

    return 0;
}
```

---

## Output

### Program 1 Output

```
--- FIRST SETS ---
FIRST(S) = { c, d }
FIRST(C) = { c, d }
FIRST(B) = { b }

--- FOLLOW SETS ---
FOLLOW(S) = { $ }
FOLLOW(C) = { b, $ }
FOLLOW(B) = { $ }
```

### Program 2 Output

```
--- FIRST SETS ---
FIRST(E) = { (, i }
FIRST(A) = { +, e }
FIRST(T) = { (, i }
FIRST(B) = { *, e }
FIRST(F) = { (, i }

--- FOLLOW SETS ---
FOLLOW(E) = { $, ) }
FOLLOW(A) = { $, ) }
FOLLOW(T) = { +, $, ) }
FOLLOW(B) = { +, $, ) }
FOLLOW(F) = { *, +, $, ) }
```

---

## Conclusion

In this lab, a C program was successfully implemented that computed FIRST and FOLLOW sets for any given Context-Free Grammar using iterative fixed-point algorithms. The program represented the grammar as a structured array and applied repeated sweeps until no new symbols were added to any set. For the original grammar with non-terminals S, C, and B, the FIRST and FOLLOW sets were correctly derived. In Task A, the grammar was modified by replacing the hardcoded productions inside `main()` with the standard arithmetic expression grammar using single-character mappings (A for E', B for T', and i for id). The resulting FIRST and FOLLOW sets matched the expected theoretical values, confirming that the implementation correctly handled epsilon productions and cascading FOLLOW propagation. This computation formed the essential foundation for building predictive LL(1) parsing tables used in top-down compiler design.
