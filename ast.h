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
AST *parseExpr(char **s);
double eval(AST *node);


#endif /* AST_H */
