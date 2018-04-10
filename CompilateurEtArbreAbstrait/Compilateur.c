#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboles.h"
#include "util.h"
#include "AnalyseurSyntaxique.h"
#include "premiers.h"
#include "suivants.h"
#include "affiche_arbre_abstrait.h"
#include "tabsymboles.h"
#include "parcours_arbre_abstrait.h"

int yylex(void);
extern char *yytext;
FILE *yyin;

void usage(){
	printf("Usage : %s [-l ou -s] file l\n-l : Analyseur lexical, -s : Analyseur syntaxique\n",__FILE__);
	exit(1);	
}

void lastUpdate(){
	printf("%s\nLast Update : %s %s\n",__FILE__,__DATE__,__TIME__);	
}

void readFile(char *file){
	yyin = fopen(file, "r");
	if(yyin == NULL) {
		fprintf(stderr, "impossible d'ouvrir le fichier %s !\n", file);
		exit(1);
	}
}

void analyseur_lexical(char *file){
	int uniteCourante;
	char nom[100];
	char valeur[100];  

	readFile(file);
	do{
		uniteCourante = yylex();  
		nom_token( uniteCourante, nom, valeur );
		printf("%s\t%s\t%s\n", yytext, nom, valeur);
	} while (uniteCourante != FIN);
	exit(0);
}

int main(int argc, char **argv){  
	//lastUpdate();
	trace_xml = 0;
	//Arguments
	switch(argc) {
		case 1 :
			usage(argv[0]);	
			break;
		case 2 :
			readFile(argv[1]);
			break;
		case 3 :
			if(!strcmp(argv[1],"-s")){
				trace_xml = 1;
				readFile(argv[2]);
			}else if(!strcmp(argv[1],"-l")){
				analyseur_lexical(argv[2]);
			}else{
				usage(argv[0]);
			}
			break;
		default :
			usage(argv[0]);
	}
	
	//Initialsiation
	initialise_premiers();
	initialise_suivants();

	uniteCourante = yylex();  
	n_prog * prog = programme();
	affiche_n_prog(prog);
	//parcours_n_prog(prog);
	
	//C'est la fin !
	if (uniteCourante == FIN) {
		return 0;
	}
	printf("\nErreur de syntaxe !\n");
	return 1;
}
