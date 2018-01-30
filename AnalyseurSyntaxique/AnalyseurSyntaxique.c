#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboles.h"
#include "util.h"

int uniteCourante;

int yylex(void);
extern char *yytext;
FILE *yyin;

void etatE_prime();
void etatT();
void etatT_prime();
void etatF();

void etatE(){
	etatT();
	etatE_prime();
}

void etatE_prime(){
	if(uniteCourante == PLUS){
		uniteCourante = yylex();
		etatE();	
	}
}

void etatT(){
	etatF();
	etatT_prime();
}

void etatT_prime(){
	if(uniteCourante == FOIS){
		uniteCourante = yylex();
		etatT();	
	}
}

void etatF(){
	if(uniteCourante == PARENTHESE_OUVRANTE){
		uniteCourante = yylex();
		etatE();
		if(uniteCourante == PARENTHESE_FERMANTE){
			uniteCourante = yylex();
		}else{
			printf("(%s) Erreur de syntaxe ) attendu\n",yytext);
			exit(-1);
		}
	}else if(uniteCourante == NOMBRE){
		uniteCourante = yylex();
	}else{
		printf("(%s) Erreur de syntaxe ( ou NOMBRE attendu\n",yytext);
		exit(-1);
	}
}

int main(int argc,char * argv[]){
	if (argc-1 != 1){
		printf("attendu %s chemin_du_fichier\n",argv[0]);
	}
	yyin = fopen(argv[1],"r");
	if(yyin == NULL){
		fprintf(stderr,"impossible d'ouvrir le fichier %s\n",argv[1]);
		exit(1);
	}
	uniteCourante = yylex();
	etatE();
	if(uniteCourante == FIN){
		printf("Youpi !\n");
	}else{
		printf("(%s) Erreur de syntaxe FIN attendu\n",yytext);
		exit(-1);
	}
	return 0;
}
