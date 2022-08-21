/****************************************************/
/* File: main.c                                     */
/* Main program for TINY compiler                   */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
/* set NO_PARSE to TRUE to get a scanner-only compiler */
#define NO_PARSE FALSE
/* set NO_ANALYZE to TRUE to get a parser-only compiler */
#define NO_ANALYZE FALSE

/* set NO_CODE to TRUE to get a compiler that does not
 * generate code
 */
#define NO_CODE FALSE

#include "util.h"
#if NO_PARSE
#include "scan.h"
#else
#include "parse.h"
#if !NO_ANALYZE
#include "analyze.h"
#if !NO_CODE
#include "qgen.h"
#include "quad.h"
#include "cgen.h"
#include "code.h"
#include "binary.h"
#endif
#endif
#endif

/* allocate global variables */
int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;
FILE * binary;

/* allocate and set tracing flags */

int EchoSource = FALSE;
int TraceScan = FALSE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceQuadruples = TRUE;
int TraceCode = TRUE;

int Error = FALSE;

int main( int argc, char * argv[] )
{ TreeNode * syntaxTree;
  QuadrupleList * quadrupleList;
  InstructList * instructList;
  char pgm[120]; /* source code file name */
  if (argc != 2)
    { fprintf(stderr,"usage: %s <filename>\n",argv[0]);
      exit(1);
    }
  strcpy(pgm,argv[1]) ;
  if (strchr (pgm, '.') == NULL)
     strcat(pgm,".cm");
  source = fopen(pgm,"r");
  if (source==NULL)
  { fprintf(stderr,"File %s not found\n",pgm);
    exit(1);
  }
  listing = stdout; /* send listing to screen */
  fprintf(listing,"\nC- COMPILATION: %s\n",pgm);
#if NO_PARSE
  printf("\nGeneration Token List...\n");
  while (getToken()!=ENDFILE);
#else
  printf("\nGeneration Syntax Tree...\n");
  syntaxTree = parse();
  if (TraceParse) {
    fprintf(listing,"\nSyntax tree:\n\n");
    printTree(syntaxTree);
  }
#if !NO_ANALYZE
  if (! Error)
  { if (TraceAnalyze)
    printf("\nGeneration Symbol Table...\n");
    buildSymtab(syntaxTree);
  }
#if !NO_CODE
  if (! Error)
  { 
    printf("\nGeneration Quadruples...\n");
    quadrupleList = quadGen(syntaxTree);
    if (TraceQuadruples) 
    {
    printf("\nQuadruples:\n\n");
    quad_print();
    }
    printf("\nGeneration Assembly Code...\n");

    instructList = codeGen(quadrupleList);


    int fnlen = strcspn(pgm,".");

    char * codefile;
    codefile = (char *) calloc(fnlen+4, sizeof(char));
    strncpy(codefile,pgm,fnlen);
    strcat(codefile,".asm");
    code = fopen(codefile,"w");
    if (code == NULL)
    { printf("Unable to open %s\n",codefile);
      exit(1);
    }
    instructList_print();
    fclose(code);

    printf("\nGeneration Binary...\n\n");

    char * binaryfile;
    binaryfile = (char *) calloc(fnlen+4, sizeof(char));
    strncpy(binaryfile,pgm,fnlen);
    strcat(binaryfile,".bin");
    binary = fopen(binaryfile,"w");
    if (binary == NULL)
    { printf("Unable to open %s\n",binaryfile);
      exit(1);
    }
    instructListToBinary(instructList);
    fclose(binary);


  }
#endif
#endif
#endif
  fclose(source);
  return 0;
}
