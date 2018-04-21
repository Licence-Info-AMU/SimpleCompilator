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
n_prog *prog = NULL;

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


void analyseur_syntaxique(char *arg, int verbose) {
	readFile(arg);
	trace_xml = verbose;
	
	initialise_premiers();
	initialise_suivants();

	uniteCourante = yylex();  
	programme();

	if (uniteCourante == FIN) {
		exit(0);
	}
	printf("\nErreur de syntaxe.\n");
}

void analyseur_syntaxique_abs(char *arg) {
	readFile(arg);
	
	initialise_premiers();
	initialise_suivants();

	uniteCourante = yylex(); 
	prog = programme(); 
	affiche_n_prog(prog);

	if (uniteCourante == FIN) {
		exit(0);
	}
	printf("\nErreur de syntaxe.\n");
}

void table_symboles(char *arg) {
	trace_tabsymb = 1;
	showIntel = 0;

	readFile(arg);
	
	initialise_premiers();
	initialise_suivants();

	uniteCourante = yylex(); 
	prog = programme(); 
	parcours_n_prog(prog);

	if (uniteCourante == FIN) {
		exit(0);
	}
	printf("\nErreur de syntaxe.\n");
}

void genere_code_intel(char *arg) {
	trace_tabsymb = 0;
	showIntel = 1;

	readFile(arg);
	
	initialise_premiers();
	initialise_suivants();

	uniteCourante = yylex(); 
	prog = programme(); 
	parcours_n_prog(prog);

	if (uniteCourante == FIN) {
		exit(0);
	}
	printf("\nErreur de syntaxe.\n");
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
				analyseur_syntaxique(argv[2], 1);
			}else if(!strcmp(argv[1],"-l")){
				analyseur_lexical(argv[2]);
			}else if(!strcmp(argv[1],"-a")) {
				analyseur_syntaxique_abs(argv[2]);
			}else if(!strcmp(argv[1],"-t")){
				table_symboles(argv[2]);
			}else if(!strcmp(argv[1],"-n")) {
				genere_code_intel(argv[2]);
			}else{
				usage(argv[0]);
			}
			break;
		default :
			usage(argv[0]);
	}
	
	//C'est la fin !
	if (uniteCourante == FIN) {
		return 0;
	}
	printf("\nErreur de syntaxe !\n");
	return 1;
}
