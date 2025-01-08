#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "crypto.h"

/* Define the structure of a node in the AST */
typedef struct AST {
    char op; /* operator ('+', '-', '*', '/', 'n' for number, 'f' for function, 't' for text) */
    double num; /* if op == 'n', the number */
    char *name; /* if op == 'f' or 't', the function name or text */
    struct AST **args; /* if op == 'f', array of AST nodes for function arguments */
    int numArgs; /* if op == 'f', the number of arguments */
    struct AST *left;
    struct AST *right;
} AST;

/* Function to create a new node */
AST *newNode(char op, double num, char *name, AST **args, int numArgs, AST *left, AST *right) {
    AST *node = malloc(sizeof(AST));
    node->op = op;
    node->num = num;
    node->name = name;
    node->args = args;
    node->numArgs = numArgs;
    node->left = left;
    node->right = right;
    return node;
}

/* Function to parse a number and move the string forward */
double parseNum(char **s) {
    double num = 0.0, factor = 0.1;
    while ((**s >= '0' && **s <= '9') || **s == '.') {
        if (**s == '.') {
            (*s)++;
            while (**s >= '0' && **s <= '9') {
                num = num + factor * (**s - '0');
                factor *= 0.1;
                (*s)++;
            }
            return num;
        }
        num = num * 10.0 + (**s - '0');
        (*s)++;
    }
    return num;
}

/* Forward declaration of function */
AST *parseExpr(char **s);

/* Function to parse a factor and move the string forward */
AST *parseFactor(char **s);

/* Function to parse a function with arguments */
AST *parseFunction(char **s) {
    char *name = malloc(13); /* Assuming function names are up to 12 characters long */
    if (name == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }
    /* Copy the function name from the input string */
    int i = 0;
    while ((**s >= 'a' && **s <= 'z') || (**s >= 'A' && **s <= 'Z')) {
        name[i++] = **s;
        (*s)++;
    }
    name[i] = '\0';
    /* Parse the opening parenthesis for function arguments */
    if (**s == '(') {
        (*s)++;
    } else {
        printf("Error: expected '('\n");
        exit(1);
    }
    /* Parse function arguments */
    AST **args = malloc(10 * sizeof(AST*)); /* Assuming up to 10 arguments for a function */
    int numArgs = 0;
    while (**s != ')' && **s != '\0') {
        args[numArgs++] = parseExpr(s);

        /* Check for a comma between arguments */
        if (**s == ',') {
            (*s)++;
        } else if (**s != ')' && **s != '\0') {
            printf("Error: expected ',' or ')'\n");
            exit(1);
        }
    }
    /* Parse the closing parenthesis for function arguments */
    if (**s == ')') {
        (*s)++;
    } else {
        printf("Error: expected ')'\n");
        exit(1);
    }
    return newNode('f', 0, name, args, numArgs, NULL, NULL);
}

/* Function to parse a term and move the string forward */
AST *parseTerm(char **s) {
    /* parse expression within () */
    if (**s == '(') {
        (*s)++;
        AST *node = parseExpr(s);
        if (**s == ')') {
            (*s)++;
            return node;
        }
        printf("Error: expected ')'\n");
        exit(1);
    }
    /* parse text within "" */
    else if (**s == '"') {
        /* Parse text enclosed in double quotes */
        (*s)++;
        char *text = malloc(1024); /* Assuming text can be up to 1023 characters long */
        int i = 0;
        while (**s != '\0' && **s != '"') {
            text[i++] = **s;
            (*s)++;
        }
        if (**s == '"') {
            (*s)++;
            text[i] = '\0';
            return newNode('t', 0, text, NULL, 0, NULL, NULL);
        } else {
            printf("Error: expected '\"'\n");
            exit(1);
        }
    }
    /* parse function() */
    else if ((**s >= 'a' && **s <= 'z') || (**s >= 'A' && **s <= 'Z')) {
        return parseFunction(s);
    }
    /* parse number */
    else {
        return newNode('n', parseNum(s), NULL, NULL, 0, NULL, NULL);
    }
}

/* Function to parse an expression and move the string forward */
AST *parseExpr(char **s) {
    AST *node = parseTerm(s);
    while (**s == '+' || **s == '-') {
        char op = **s;
        (*s)++;
        node = newNode(op, 0, NULL, NULL, 0, node, parseTerm(s));
    }
    return node;
}

/* Function to evaluate the AST */
double eval(AST *node) {
    switch (node->op) {
        case 'n':
            return node->num;
        case '+':
            return eval(node->left) + eval(node->right);
        case '-':
            return eval(node->left) - eval(node->right);
        case '*':
            return eval(node->left) * eval(node->right);
        case '/':
            return eval(node->left) / eval(node->right);
        case 'f': {
            if (strcmp(node->name, "cl") == 0) { /* clear terminal */
                printf("\033[H\033[J");
                return 0.0;
            } else if (strcmp(node->name, "exit") == 0 || strcmp(node->name, "quit") == 0 || strcmp(node->name, "q") == 0) { /* exit program */
                printf("Execution finished\n");
                exit(0);
            } else if (strcmp(node->name, "sin") == 0) { /* sin() function */
                return sin(eval(node->right));
            } else if (strcmp(node->name, "cos") == 0) { /* cos() function */
                return cos(eval(node->right));
            } else if (strcmp(node->name, "pi") == 0) { /* pi constant */
                return M_PI;
            } else if (strcmp(node->name, "e") == 0) { /* e constant */
                return M_E;
            } else if (strcmp(node->name, "printarg") == 0) { /* debug function: print function arguments */
                for (int i = 0; i < node->numArgs; i++) {
                    printf("%lf\n", eval(node->args[i]));
                }
            } else if (strcmp(node->name, "add") == 0) { /* equivalent to + */
                double result = 0.0;
                for (int i = 0; i < node->numArgs; i++) {
                    result += eval(node->args[i]);
                }
                return result;
            } else if (strcmp(node->name, "multiply") == 0) { /* equivalent to * */
                double result = 1.0;
                for (int i = 0; i < node->numArgs; i++) {
                    result *= eval(node->args[i]);
                }
                return result;
            } else if (strcmp(node->name, "caesar") == 0) { /* caesar  */
                if (node->numArgs != 2) {
                    printf("Error: 'caesar' function expects 2 arguments\n");
                    exit(1);
                } else {
                    int shift = (int)eval(node->args[0]);
                    const char *text = node->args[1]->name;
                    char* result = caesar(shift, text);
                    printf("Caesar encryption/decryption: %s\n", result);
                    free(result);  /* Free memory allocated for result */
                    return 0.0; /* Caesar cipher does not return a numerical value */
                }
            } else if (strcmp(node->name, "affine") == 0) {
                if (node->numArgs != 3) {
                    printf("Error: 'affine' function expects 3 arguments\n");
                    exit(1);
                } else {
                    int a = (int)eval(node->args[0]);
                    int b = (int)eval(node->args[1]);
                    const char *text = node->args[2]->name;
                    char* result = affine(a, b, text);
                    printf("Affine encryption/decryption: %s\n", result);
                    free(result);  /* Free memory allocated for result */
                    return 0.0; /* Affine cipher does not return a numerical value */
                }
            } else if (strcmp(node->name, "rsa") == 0) {
                if (node->numArgs != 3) {
                    printf("Error: 'rsa' function expects 3 arguments\n");
                    exit(1);
                } else {
                    const char *prime1 = node->args[0]->name;
                    const char *prime2 = node->args[1]->name;
                    const char *message = node->args[2]->name;
                    rsa(prime1, prime2, message);
                    return 0.0; /* RSA cipher does not return a numerical value */
                }
            } else if (strcmp(node->name, "bigprime") == 0) {
                if (node->numArgs != 1) {
                    printf("Error: 'bigprime' function expects 1 argument\n");
                    exit(1);
                } else {
                    int bits = (int)eval(node->args[0]);
                    printf("%s\n", big_prime(bits));
                    return 0.0; /* prime generation does not return a numerical value */
                }
            } else {
                printf("Error: Unknown function\n");
                exit(1);
            }
        }
        case 't':
            printf("Text: %s\n", node->name);
            return 0; /* Text case, for now, just print the text */
        default:
            return 0; /* error */
    }
}
