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

void programme(){										//PG
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_optDecVariables_)){
		optDecVariables();
		listeDecFonctions();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void optDecVariables(){									//ODV
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_listeDecVariables_)){
		listeDecVariables();
		consommer(POINT_VIRGULE,&uniteCourante,trace_xml);
	}else if (est_suivant_uniteCourante(_optDecVariables_)){	// cas epsilon
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void listeDecVariables(){								//LDV
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_declarationVariable_)){
		declarationVariable();
		listeDecVariablesBis();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void listeDecVariablesBis(){							//LDVB
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==VIRGULE){
		consommer(VIRGULE,&uniteCourante,trace_xml);
		declarationVariable();
		listeDecVariablesBis();
	}else if (est_suivant_uniteCourante(_listeDecVariablesBis_)){	// cas epsilon
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void declarationVariable(){								//DV
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==ENTIER){
		consommer(ENTIER,&uniteCourante,trace_xml);
		consommer(ID_VAR,&uniteCourante,trace_xml);
		optTailleTableau();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void optTailleTableau(){								//OTT
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==CROCHET_OUVRANT){
		consommer(CROCHET_OUVRANT,&uniteCourante,trace_xml);
		consommer(NOMBRE,&uniteCourante,trace_xml);
		consommer(CROCHET_FERMANT,&uniteCourante,trace_xml);
	}else if (est_suivant_uniteCourante(_optTailleTableau_)){	// cas epsilon
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void listeDecFonctions(){								//LDF
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_declarationFonction_)){
		declarationFonction();
		listeDecFonctions();
	}else if (est_suivant_uniteCourante(_listeDecFonctions_)){	// cas epsilon
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void declarationVariable(){								//DF
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==ID_FCT){
		consommer(ID_FCT,&uniteCourante,trace_xml);
		listeParam();
		optDecVariables();
		instructionBloc();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void listeParam(){										//LP
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==PARENTHESE_OUVRANTE){
		consommer(PARENTHESE_OUVRANTE,&uniteCourante,trace_xml);
		optListeDecVariables();
		consommer(PARENTHESE_FERMANTE,&uniteCourante,trace_xml);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void optListeDecVariables(){							//OLDV
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_listeDecVariables_)){
		listeDecVariables();
	}else if (est_suivant_uniteCourante(_optListeDecVariables_)){	// cas epsilon
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void instruction(){										//I
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_instructionAffect_)){
		instructionAffect();
	}else if (est_premier_uniteCourante(_instructionBloc_)){
		instructionBloc();
	}else if (est_premier_uniteCourante(_instructionSi_)){
		instructionSi();
	}else if (est_premier_uniteCourante(_instructionTantque_)){
		instructionTantque();
	}else if (est_premier_uniteCourante(_instructionAppel_)){
		instructionAppel();
	}else if (est_premier_uniteCourante(_instructionRetour_)){
		instructionRetour();
	}else if (est_premier_uniteCourante(_instructionEcriture_)){
		instructionEcriture();
	}else if (est_premier_uniteCourante(_instructionVide_)){
		instructionVide();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void instructionAffect(){								//IAFF
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_var_)){
		var();
		consommer(EGAL,&uniteCourante,trace_xml);
		expression();
		consommer(POINT_VIRGULE,&uniteCourante,trace_xml);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void instructionBloc(){									//IB
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==ACCOLADE_OUVRANTE){
		consommer(ACCOLADE_OUVRANTE,&uniteCourante,trace_xml);
		listeInstructions();
		consommer(ACCOLADE_FERMANTE,&uniteCourante,trace_xml);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void listeInstructions(){								//LI
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_instruction_)){
		instruction();
		listeInstructions();
	}else if (est_suivant_uniteCourante(_listeInstructions_)){	// cas epsilon
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void instructionSi(){									//ISI
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==SI){
		consommer(SI,&uniteCourante,trace_xml);
		expression();
		consommer(ALORS,&uniteCourante,trace_xml);
		instructionBloc();
		optSinon();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void optSinon(){										//OSINON
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==SINON){
		consommer(SINON,&uniteCourante,trace_xml);
		instructionBloc();
	}else if (est_suivant_uniteCourante(_optSinon_)){	// cas epsilon
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void instructionTantque(){								//ITQ
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==TANTQUE){
		consommer(TANTQUE,&uniteCourante,trace_xml);
		expression();
		consommer(FAIRE,&uniteCourante,trace_xml);
		instructionBloc();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void instructionAppel(){								//IAPP
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_appelFct_)){
		appelFct();
		consommer(POINT_VIRGULE,&uniteCourante,trace_xml);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void instructionRetour(){								//IRET
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==RETOUR){
		consommer(RETOUR,&uniteCourante,trace_xml);
		expression();
		consommer(POINT_VIRGULE,&uniteCourante,trace_xml);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void instructionEcriture(){								//IECR
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==ECRIRE){
		consommer(ECRIRE,&uniteCourante,trace_xml);
		consommer(PARENTHESE_OUVRANTE,&uniteCourante,trace_xml);
		expression();
		consommer(PARENTHESE_FERMANTE,&uniteCourante,trace_xml);
		consommer(POINT_VIRGULE,&uniteCourante,trace_xml);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void instructionVide(){									//IVIDE
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==POINT_VIRGULE){
		consommer(POINT_VIRGULE,&uniteCourante,trace_xml);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void expression(){										//EXP
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_conjonction_)){
		conjonction();
		expressionBis();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void expressionBis(){									//EXPB
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==OU){
		consommer(OU,&uniteCourante,trace_xml);
		conjonction();
		expressionBis();
	}else if (est_suivant_uniteCourante(_expressionBis_)){	// cas epsilon
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void conjonction(){										//CONJ
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_comparaison_)){
		comparaison();
		conjonctionBis();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void conjonctionBis(){									//CONJB
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==ET){
		consommer(ET,&uniteCourante,trace_xml);
		comparaison();
		conjonctionBis();
	}else if (est_suivant_uniteCourante(_conjonctionBis_)){	// cas epsilon
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void comparaison(){										//COMP
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_expArith_)){
		expArith();
		comparaisonBis();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void comparaisonBis(){									//COMPB
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==EGAL){
		consommer(EGAL,&uniteCourante,trace_xml);
		expArith();
		comparaisonBis();
	}else if(uniteCourante==INFERIEUR){
		consommer(INFERIEUR,&uniteCourante,trace_xml);
		expArith();
		comparaisonBis();
	}else if (est_suivant_uniteCourante(_comparaisonBis_)){	// cas epsilon
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void expArith(){										//E
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_terme_)){
		terme();
		expArithBis();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void expArithBis(){										//EB
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==PLUS){
		consommer(PLUS,&uniteCourante,trace_xml);
		terme();
		expArithBis();
	}else if(uniteCourante==MOINS){
		consommer(MOINS,&uniteCourante,trace_xml);
		terme();
		expArithBis();
	}else if (est_suivant_uniteCourante(_expArithBis_)){	// cas epsilon
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void terme(){											//T
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_negation_)){
		negation();
		termeBis();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void termeBis(){										//TB
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==FOIS){
		consommer(FOIS,&uniteCourante,trace_xml);
		negation();
		termeBis();
	}else if(uniteCourante==DIVISE){
		consommer(DIVISE,&uniteCourante,trace_xml);
		negation();
		termeBis();
	}else if (est_suivant_uniteCourante(_termeBis_)){	// cas epsilon
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void negation(){										//NEG
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==NON){
		consommer(NON,&uniteCourante,trace_xml);
		negation();
	}else if(est_premier_uniteCourante(_facteur_)){
		facteur();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void facteur(){											//F
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==PARENTHESE_OUVRANTE){
		consommer(PARENTHESE_OUVRANTE,&uniteCourante,trace_xml);
		expression();
		consommer(PARENTHESE_FERMANTE,&uniteCourante,trace_xml);
	}else if (uniteCourante==NOMBRE){
		consommer(NOMBRE,&uniteCourante,trace_xml);
	}else if (est_premier_uniteCourante(_appelFct_)){
		appelFct();
	}else if (est_premier_uniteCourante(_var_)){
		var();
	}else if (uniteCourante==LIRE){
		consommer(LIRE,&uniteCourante,trace_xml);
		consommer(PARENTHESE_OUVRANTE,&uniteCourante,trace_xml);
		consommer(PARENTHESE_FERMANTE,&uniteCourante,trace_xml);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void var(){												//VAR
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==ID_VAR){
		consommer(ID_VAR,&uniteCourante,trace_xml);
		optIndice();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void optIndice(){										//OIND
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==CROCHET_OUVRANT){
		consommer(CROCHET_OUVRANT,&uniteCourante,trace_xml);
		expression();
		consommer(CROCHET_FERMANT,&uniteCourante,trace_xml);
	}else if (est_suivant_uniteCourante(_optIndice_)){	// cas epsilon
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void appelFct(){										//APPF
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==ID_FCT){
		consommer(ID_FCT,&uniteCourante,trace_xml);
		consommer(PARENTHESE_OUVRANTE,&uniteCourante,trace_xml);
		listeExpressions();
		consommer(PARENTHESE_FERMANTE,&uniteCourante,trace_xml);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void listeExpressions(){								//LEXP
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_expression_)){
		expression();
		listeExpressionsBis();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}

void listeExpressionsBis(){								//LEXPB
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==VIRGULE){
		consommer(VIRGULE,&uniteCourante,trace_xml);
		expression();
		listeExpressionsBis();
	}else if (est_suivant_uniteCourante(_listeExpressionsBis_)){	// cas epsilon
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
}