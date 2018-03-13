#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboles.h"
#include "util.h"

int yylex(void);
extern char *yytext;
FILE *yyin;

int main(int argc, char **argv) {  
  int uniteCourante;
  char nom[100];
  char valeur[100];  
  
  yyin = fopen(argv[1], "r");
  if(yyin == NULL){
    fprintf(stderr, "impossible d'ouvrir le fichier %s\n", argv[1]);
    exit(1);
  }
  do {
    uniteCourante = yylex();  
    nom_token( uniteCourante, nom, valeur );
    printf("%s\t%s\t%s\n", yytext, nom, valeur);
  } while (uniteCourante != FIN);
  
  return 0;
}
