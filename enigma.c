/* ENIGMA */
/* libraries */
#include <stdio.h>
#include <string.h>
#include "ast.h"

/* functions */
static int actio(void);
static int lectio(int argc, char *argv[]);
static void help(void);
static void title(void);

/* global variables */
static const float version = 0.01;
static char buffer[2048];

/* execute interactive mode, parsing and evaluation user inut */
int actio(void) {
  while (1) {
    printf("  >> ");
    fflush(stdout); /* Ensure the prompt is printed */
    //(void)fgets(buffer, sizeof(buffer), stdin);
    //if (feof(stdin) || ferror(stdin)) {
    //  perror("Error reading from STDIN");
    //  return 1;
    //}
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
      /* Check if the error was due to EOF or a real error */
      if (feof(stdin)) {
          /* Handle EOF (if needed) */
          fprintf(stderr, "\nEnd of input reached\n");
          return 1; /* Return on error or EOF */
      } else if (ferror(stdin)) {
          /* Handle input error */
          perror("Error reading from STDIN");
          return 1; /* Return on error or EOF */
      }
      return 1; /* Return on error or EOF */
    }

    /* Process the input */
    char *p = buffer;
    AST *ast = parseExpr(&p);
    printf("%f\n", eval(ast));
  }
  return 0;
}

/* read expressions from command arguments  */
int lectio(int argc, char *argv[]) {
  /* concatenate all arguments */
  for (int i = 2; i < argc; i++) {
    strcat(buffer, argv[i]);
    strcat(buffer, " ");
  }
  /* parse and output expression */
  char *p = buffer;
  AST *ast = parseExpr(&p);
  printf("%f\n", eval(ast));
  return 0;
}

/* print help message */ 
void help(void) {
  printf("Usage: enigma [options] [arguments]\n"
        "If no argument is passed, interactive mode is executed.\n"
        "\t-h,\t--help\t\t\t\t\tPrint this help message\n"
        "\t-v,\t--version\t\t\t\tPrint version information\n"
        "\t-i,\t--input\t\t\t\t\tInput expression\n");
}

/* print ascii title */
void title(void){
  printf("\033[1;32m");
  printf("    ______      _                      \n");
  printf("   / ____/___  (_)___ _____ ___  ____ _\n");
  printf("  / __/ / __ \\/ / __ `/ __ `__ \\/ __ `/ \n");
  printf(" / /___/ / / / / /_/ / / / / / / /_/ /  \n");
  printf("/_____/_/ /_/_/\\__, /_/ /_/ /_/\\__,_/    \n");
  printf("              /____/                      \n");
  printf("\033[0m");
}

int main(int argc, char *argv[]) {
  switch(argc) {
    case 1:
      /* 0 arguments: Interactive mode */
      title();
      return actio();
    case 2: {
      /* 1 argument: options */
      if (strcmp(argv[1],"-h")==0 || strcmp(argv[1],"--help")==0) help(); /* help message */
      else if (strcmp(argv[1],"-v")==0 || strcmp(argv[1],"--version")==0) printf("enigma v%.2f\n", version); /* version info */
      else help(); /* help message */
    }
    case 3: {
      /* 2 arguments: input option */
      if (strcmp(argv[1],"-i")==0 || strcmp(argv[1],"--input")==0) return lectio(argc, argv); /* read argument as input */
    }
    default: return 0;
  }
}
