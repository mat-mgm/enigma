#ifndef AST_H
#define AST_H

#include <stddef.h>

/* AST struct definition */
typedef struct AST {
  char op;
  double value;
  char *name;
  struct AST **args;
  size_t numArgs;
  struct AST *left;
  struct AST *right;
} AST;

/* Function prototypes */
static AST *newNode(char op, double num, char *name, AST **args, int numArgs, AST *left, AST *right);
static double parseNum(char **s);
static AST *parseExpr(char **s);
static AST *parseFactor(char **s);
static AST *parseFunction(char **s);
static AST *parseTerm(char **s);
static double eval(AST *node);


#endif /* AST_H */
