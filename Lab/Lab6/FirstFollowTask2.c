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
        if (set[i] == val)
            return false;
    }

    set[len] = val;
    set[len + 1] = '\0';

    return true;
}

bool merge_sets(char set1[], char set2[], bool ignore_epsilon) {
    bool changed = false;
    int len2 = strlen(set2);

    for (int i = 0; i < len2; i++) {
        if (ignore_epsilon && set2[i] == 'e')
            continue;

        if (add_to_set(set1, set2[i]))
            changed = true;
    }

    return changed;
}

bool has_epsilon(char set[]) {
    for (int i = 0; set[i] != '\0'; i++) {
        if (set[i] == 'e')
            return true;
    }

    return false;
}

void compute_first() {
    bool changed = true;

    while (changed) {
        changed = false;

        for (int i = 0; i < rule_count; i++) {
            char head = grammar[i].head;
            int head_idx = head - 'A';

            for (int j = 0; j < grammar[i].prod_count; j++) {
                char *prod = grammar[i].body[j];
                int prod_len = strlen(prod);

                // Epsilon production
                if (prod[0] == 'e') {
                    if (add_to_set(first_sets[head_idx], 'e'))
                        changed = true;
                }
                else {
                    bool all_nullable = true;

                    for (int k = 0; k < prod_len; k++) {
                        char sym = prod[k];

                        if (!is_non_terminal(sym)) {
                            if (add_to_set(first_sets[head_idx], sym))
                                changed = true;

                            all_nullable = false;
                            break;
                        }
                        else {
                            int sym_idx = sym - 'A';

                            if (merge_sets(
                                    first_sets[head_idx],
                                    first_sets[sym_idx],
                                    true))
                                changed = true;

                            if (!has_epsilon(first_sets[sym_idx])) {
                                all_nullable = false;
                                break;
                            }
                        }
                    }

                    if (all_nullable) {
                        if (add_to_set(first_sets[head_idx], 'e'))
                            changed = true;
                    }
                }
            }
        }
    }
}

void compute_follow(char start_symbol) {
    // Add $ to FOLLOW(start_symbol)
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
                        bool all_nullable = true;

                        // FIRST of the remaining symbols
                        for (int n = k + 1; n < prod_len; n++) {
                            char next_sym = prod[n];

                            if (!is_non_terminal(next_sym)) {
                                if (add_to_set(
                                        follow_sets[cur_idx],
                                        next_sym))
                                    changed = true;

                                all_nullable = false;
                                break;
                            }
                            else {
                                int next_idx = next_sym - 'A';

                                if (merge_sets(
                                        follow_sets[cur_idx],
                                        first_sets[next_idx],
                                        true))
                                    changed = true;

                                if (!has_epsilon(first_sets[next_idx])) {
                                    all_nullable = false;
                                    break;
                                }
                            }
                        }

                        // If remaining symbols can derive epsilon,
                        // add FOLLOW(head)
                        if (all_nullable) {
                            if (merge_sets(
                                    follow_sets[cur_idx],
                                    follow_sets[head_idx],
                                    false))
                                changed = true;
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

    /* 
    Grammar  :
E → T E' 
E' → + T E' | e 
T → F T' 
T' → * F T' | e 
F → ( E ) | id 
    */

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