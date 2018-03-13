#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboles.h"
#include "util.h"
#include "premierssuivants.h"
#include "AnalyseurSyntaxique.h"	

int uniteCourante;
int yylex(void);
extern char *yytext;
FILE *yyin;

int est_premier_uniteCourante(int arg){
	return (est_premier(arg, uniteCourante));
}

int est_suivant_uniteCourante(int arg){
	return est_suivant(arg, uniteCourante);
}

void quadtrees(){
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_quadtrees_)){
		switch(uniteCourante){
			case R : 
				consommer(R,&uniteCourante,trace_xml);
				break;
			case G :
				consommer(G,&uniteCourante,trace_xml);
				break;
			case B :
				consommer(B,&uniteCourante,trace_xml);
				break;
			case PARENTHESE_OUVRANTE :
				consommer(PARENTHESE_OUVRANTE,&uniteCourante,trace_xml);
				quadtrees();
				quadtrees();
				quadtrees();
				quadtrees();
				consommer(PARENTHESE_FERMANTE,&uniteCourante,trace_xml);
				break;
		}
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}