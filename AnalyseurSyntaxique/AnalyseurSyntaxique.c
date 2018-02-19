#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboles.h"
#include "util.h"
#include "premiers.h"
#include "suivants.h"
#include "AnalyseurSyntaxique.h"	

int uniteCourante;
int yylex(void);
extern char *yytext;
FILE *yyin;

int est_premier_uniteCourante(int arg){
	return (est_premier(arg, uniteCourante) || est_premier(arg, EPSILON));
}

int est_suivant_uniteCourante(int arg){
	return est_suivant(arg, uniteCourante);
}

void programme(){
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_optDecVariables_)){
		optDecVariables();
		listeDecFonctions();
	}
	else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void optDecVariables(){
	
}

void listeDecFonctions(){
	
}
