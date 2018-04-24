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

n_prog * programme(){										//PG
	n_l_dec * odv=NULL;
	n_l_dec * ldf=NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_optDecVariables_) || est_premier_uniteCourante(_listeDecFonctions_)){
		odv=optDecVariables();
		ldf=listeDecFonctions();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cree_n_prog(odv,ldf);
}

n_l_dec * optDecVariables(){									//ODV
	n_l_dec * ldv=NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_listeDecVariables_)){
		ldv=listeDecVariables();
		consommer(POINT_VIRGULE,&uniteCourante,trace_xml);
	}else if (est_suivant_uniteCourante(_optDecVariables_)){	// cas epsilon
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return NULL;
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return ldv;
}

n_l_dec * listeDecVariables(){								//LDV
	n_dec * dv=NULL;
	n_l_dec * ldv=NULL;

	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_declarationVariable_)){
		dv=declarationVariable();
		ldv=listeDecVariablesBis();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cree_n_l_dec(dv,ldv);
}

n_l_dec * listeDecVariablesBis(){							//LDVB
	n_dec * dv=NULL;
	n_l_dec * ldv=NULL;

	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==VIRGULE){
		consommer(VIRGULE,&uniteCourante,trace_xml);
		dv=declarationVariable();
		ldv=listeDecVariablesBis();
	}else if (est_suivant_uniteCourante(_listeDecVariablesBis_)){	// cas epsilon
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return NULL;
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cree_n_l_dec(dv,ldv);
}

n_dec * declarationVariable(){								//DV
	n_dec * var = NULL;
	int ott=-1;
	char * temp2= malloc(sizeof(char)*100);
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==ENTIER){
		consommer(ENTIER,&uniteCourante,trace_xml);
		nom_token(uniteCourante,nom,valeur);
		strcpy(temp2,valeur);
		consommer(ID_VAR,&uniteCourante,trace_xml);
		ott=optTailleTableau();
	}else{
		erreur((char *) __func__);
	}
	
	if (ott > 0){
		var = cree_n_dec_tab(temp2,ott);
	}else{
		var = cree_n_dec_var(temp2);
	}
	
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return var;
}

int optTailleTableau(){									//OTT
	char * temp2= malloc(sizeof(char)*100);
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==CROCHET_OUVRANT){
		consommer(CROCHET_OUVRANT,&uniteCourante,trace_xml);
		nom_token(uniteCourante,nom,valeur);
		strcpy(temp2,valeur);
		consommer(NOMBRE,&uniteCourante,trace_xml);
		consommer(CROCHET_FERMANT,&uniteCourante,trace_xml);
	}else if (est_suivant_uniteCourante(_optTailleTableau_)){	// cas epsilon
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return -1;
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return atoi(temp2);
}

////////////////////////////////////////////////////////////////////////////////////////

n_l_dec *  listeDecFonctions(){								//LDF
	n_dec * df=NULL;
	n_l_dec * ldf=NULL;

	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_declarationFonction_)){
		df=declarationFonction();
		ldf=listeDecFonctions();
	}else if (est_suivant_uniteCourante(_listeDecFonctions_)){	// cas epsilon
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return NULL;
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cree_n_l_dec(df,ldf);
}

n_dec * declarationFonction(){								//DF
	n_l_dec * lp=NULL;
	n_l_dec * odv=NULL;
	n_instr * ib=NULL;
	char * temp2= malloc(sizeof(char)*100);
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==ID_FCT){
		nom_token(uniteCourante,nom,valeur);
		strcpy(temp2,valeur);
		consommer(ID_FCT,&uniteCourante,trace_xml);
		lp=listeParam();
		odv=optDecVariables();
		ib=instructionBloc();
	}else{
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return  cree_n_dec_fonc(temp2,lp,odv,ib);
}

n_l_dec * listeParam(){
	n_l_dec * odv=NULL;										//LP
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==PARENTHESE_OUVRANTE){
		consommer(PARENTHESE_OUVRANTE,&uniteCourante,trace_xml);
		odv=optListeDecVariables();
		consommer(PARENTHESE_FERMANTE,&uniteCourante,trace_xml);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return odv;
}

n_l_dec * optListeDecVariables(){
	n_l_dec * ldv=NULL;								//OLDV
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_listeDecVariables_)){
		ldv=listeDecVariables();
	}else if (est_suivant_uniteCourante(_optListeDecVariables_)){	// cas epsilon
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return NULL;
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return ldv;
}

n_instr * instruction(){
	n_instr * inst=NULL;										//I
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_instructionAffect_)){
		inst=instructionAffect();
	}else if (est_premier_uniteCourante(_instructionBloc_)){
		inst=instructionBloc();
	}else if (est_premier_uniteCourante(_instructionSi_)){
		inst=instructionSi();
	}else if (est_premier_uniteCourante(_instructionTantque_)){
		inst=instructionTantque();
	}else if (est_premier_uniteCourante(_instructionAppel_)){
		inst=instructionAppel();
	}else if (est_premier_uniteCourante(_instructionRetour_)){
		inst=instructionRetour();
	}else if (est_premier_uniteCourante(_instructionEcriture_)){
		inst=instructionEcriture();
	}else if (est_premier_uniteCourante(_instructionVide_)){
		inst=instructionVide();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return inst;
}

n_instr * instructionAffect(){
	n_var *	Var=NULL;
	n_exp *	exp=NULL;					
	//IAFF
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_var_)){
		Var=var();
		consommer(EGAL,&uniteCourante,trace_xml);
		exp=expression();
		consommer(POINT_VIRGULE,&uniteCourante,trace_xml);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cree_n_instr_affect(Var,exp);
}

n_instr * instructionBloc(){
	n_l_instr * li = NULL;									//IB
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==ACCOLADE_OUVRANTE){
		consommer(ACCOLADE_OUVRANTE,&uniteCourante,trace_xml);
		li=listeInstructions();
		consommer(ACCOLADE_FERMANTE,&uniteCourante,trace_xml);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cree_n_instr_bloc(li);
}

n_l_instr * listeInstructions(){
	n_instr * inst = NULL;
	n_l_instr *	li = NULL;							//LI
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_instruction_)){
		inst=instruction();
		li=listeInstructions();
	}else if (est_suivant_uniteCourante(_listeInstructions_)){	// cas epsilon
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return NULL;
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cree_n_l_instr(inst,li);
}

n_instr * instructionSi(){									//ISI
	n_exp * exp = NULL;
	n_instr * ib = NULL;
	n_instr * oSinon = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==SI){
		consommer(SI,&uniteCourante,trace_xml);
		exp=expression();
		consommer(ALORS,&uniteCourante,trace_xml);
		ib=instructionBloc();
		oSinon=optSinon();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cree_n_instr_si(exp,ib,oSinon);
}

n_instr * optSinon(){										//OSINON
	n_instr * ib = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==SINON){
		consommer(SINON,&uniteCourante,trace_xml);
		ib=instructionBloc();
	}else if (est_suivant_uniteCourante(_optSinon_)){	// cas epsilon
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return NULL;
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return ib;
}

n_instr * instructionTantque(){								//ITQ
	n_exp * exp = NULL;
	n_instr * ib = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==TANTQUE){
		consommer(TANTQUE,&uniteCourante,trace_xml);
		exp=expression();
		consommer(FAIRE,&uniteCourante,trace_xml);
		ib=instructionBloc();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cree_n_instr_tantque(exp,ib);
}

n_instr * instructionAppel(){								//IAPP
	n_appel *aFct = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_appelFct_)){
		aFct=appelFct();
		consommer(POINT_VIRGULE,&uniteCourante,trace_xml);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cree_n_instr_appel(aFct);
}

n_instr * instructionRetour(){								//IRET
	n_exp * exp = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==RETOUR){
		consommer(RETOUR,&uniteCourante,trace_xml);
		exp=expression();
		consommer(POINT_VIRGULE,&uniteCourante,trace_xml);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cree_n_instr_retour(exp);
}

n_instr * instructionEcriture(){								//IECR
	n_exp * exp = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==ECRIRE){
		consommer(ECRIRE,&uniteCourante,trace_xml);
		consommer(PARENTHESE_OUVRANTE,&uniteCourante,trace_xml);
		exp=expression();
		consommer(PARENTHESE_FERMANTE,&uniteCourante,trace_xml);
		consommer(POINT_VIRGULE,&uniteCourante,trace_xml);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cree_n_instr_ecrire(exp);
}

n_instr * instructionVide(){									//IVIDE
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==POINT_VIRGULE){
		consommer(POINT_VIRGULE,&uniteCourante,trace_xml);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cree_n_instr_vide();
}

n_exp * expression(){										//EXP
	n_exp * conj = NULL;
	n_exp * eb = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_conjonction_)){
		conj=conjonction();
		eb=expressionBis(conj);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return eb;
}


n_exp * expressionBis(n_exp * herite){									//EXPB
	n_exp * conj = NULL;
	n_exp * eb = NULL;
	n_exp * op = NULL;
	n_exp * exp = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==OU){
		consommer(OU,&uniteCourante,trace_xml);
		conj=conjonction();
		op = cree_n_exp_op(ou,herite,conj);
		eb=expressionBis(op);
	}else if(uniteCourante==INTERROGATION){
		consommer(INTERROGATION,&uniteCourante,trace_xml);
		expression();
		consommer(DEUXPOINTS,&uniteCourante,trace_xml);
		expression();
	}else if (est_suivant_uniteCourante(_expressionBis_)){	// cas epsilon
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return herite;
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return eb;
}

n_exp * conjonction(){										//CONJ
	n_exp * c = NULL;
	n_exp * cb = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_comparaison_)){
		c=comparaison();
		cb=conjonctionBis(c);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cb;
}

n_exp * conjonctionBis(n_exp * herite){									//CONJB
	n_exp * c = NULL;
	n_exp * op = NULL;
	n_exp * cb = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==ET){
		consommer(ET,&uniteCourante,trace_xml);
		c=comparaison();
		op = cree_n_exp_op(et,herite,c);
		cb=conjonctionBis(op);
	}else if (est_suivant_uniteCourante(_conjonctionBis_)){	// cas epsilon
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return herite;
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cb;
}

n_exp * comparaison(){										//COMP
	n_exp * ea = NULL;
	n_exp * cb = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_expArith_)){
		ea=expArith();
		cb=comparaisonBis(ea);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cb;
}

n_exp * comparaisonBis(n_exp * herite){									//COMPB
	n_exp * ea = NULL;
	n_exp * cb = NULL;
	n_exp * op = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==EGAL){
		consommer(EGAL,&uniteCourante,trace_xml);
		ea=expArith();
		op = cree_n_exp_op(egal,herite,ea);
		cb=comparaisonBis(op);
	}else if(uniteCourante==INFERIEUR){
		consommer(INFERIEUR,&uniteCourante,trace_xml);
		ea=expArith();
		op = cree_n_exp_op(inferieur,herite,ea);
		cb=comparaisonBis(op);
	}else if (est_suivant_uniteCourante(_comparaisonBis_)){	// cas epsilon
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return herite;
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cb;
}

n_exp * expArith(){										//E
	n_exp * t = NULL;
	n_exp * eab = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_terme_)){
		t=terme();
		eab=expArithBis(t);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return eab;
}

n_exp * expArithBis(n_exp * herite){										//EB
	n_exp * t = NULL;
	n_exp * eab = NULL;
	n_exp * op = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==PLUS){
		consommer(PLUS,&uniteCourante,trace_xml);
		t=terme();
		op = cree_n_exp_op(plus,herite,t);
		eab=expArithBis(op);
	}else if(uniteCourante==MOINS){
		consommer(MOINS,&uniteCourante,trace_xml);
		t=terme();
		op = cree_n_exp_op(moins,herite,t);
		eab=expArithBis(op);
	}else if (est_suivant_uniteCourante(_expArithBis_)){	// cas epsilon
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return herite;
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return eab;
}

n_exp * terme(){											//T
	n_exp * neg = NULL;
	n_exp * tb = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_negation_)){
		neg=negation();
		tb=termeBis(neg);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return tb;
}

n_exp * termeBis(n_exp * herite){										//TB
	n_exp * neg = NULL;
	n_exp * tb = NULL;
	n_exp * op = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==FOIS){
		consommer(FOIS,&uniteCourante,trace_xml);
		neg=negation();
		op = cree_n_exp_op(fois,herite,neg);
		tb=termeBis(op);
	}else if(uniteCourante==DIVISE){
		consommer(DIVISE,&uniteCourante,trace_xml);
		neg=negation();
		op = cree_n_exp_op(divise,herite,neg);
		tb=termeBis(op);
	}else if (est_suivant_uniteCourante(_termeBis_)){	// cas epsilon
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return herite;
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return tb;
}

n_exp * negation(){										//NEG
	n_exp * op = NULL;
	n_exp * neg = NULL;
	n_exp * car = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==NON){
		consommer(NON,&uniteCourante,trace_xml);
		neg=negation();
		op = cree_n_exp_op(non,neg,NULL);
	}else if(uniteCourante==CARRE){
		consommer(CARRE,&uniteCourante,trace_xml);
		car=negation();
		op = cree_n_exp_op(carre,car,NULL);
	}else if(est_premier_uniteCourante(_facteur_)){
		op = facteur();
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return op;
}

n_exp * facteur(){											//F
	n_exp * exp = NULL;
	char * temp2= malloc(sizeof(char)*100);
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==PARENTHESE_OUVRANTE){
		consommer(PARENTHESE_OUVRANTE,&uniteCourante,trace_xml);
		exp=expression();
		consommer(PARENTHESE_FERMANTE,&uniteCourante,trace_xml);
	}else if (uniteCourante==NOMBRE){
		nom_token(uniteCourante,nom,valeur);
		strcpy(temp2,valeur);
		consommer(NOMBRE,&uniteCourante,trace_xml);
		exp=cree_n_exp_entier(atoi(temp2));
	}else if (est_premier_uniteCourante(_appelFct_)){
		exp=cree_n_exp_appel(appelFct());
	}else if (est_premier_uniteCourante(_var_)){
		exp=cree_n_exp_var(var());
	}else if (uniteCourante==LIRE){
		consommer(LIRE,&uniteCourante,trace_xml);
		exp=cree_n_exp_lire();
		consommer(PARENTHESE_OUVRANTE,&uniteCourante,trace_xml);
		consommer(PARENTHESE_FERMANTE,&uniteCourante,trace_xml);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return exp;
}

n_var * var(){												//VAR
	n_exp * v = NULL;
	n_var *$$ = NULL;
	char * temp2= malloc(sizeof(char)*100);
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==ID_VAR){
		nom_token(uniteCourante,nom,valeur);
		strcpy(temp2,valeur);
		consommer(ID_VAR,&uniteCourante,trace_xml);
		v = optIndice();

		if (v != NULL){
			$$ = cree_n_var_indicee(temp2,v);
			
		}else{			
			$$ = cree_n_var_simple(temp2);		
		}
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return $$;
}

n_exp * optIndice(){										//OIND
	n_exp * exp = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==CROCHET_OUVRANT){
		consommer(CROCHET_OUVRANT,&uniteCourante,trace_xml);
		exp = expression();
		consommer(CROCHET_FERMANT,&uniteCourante,trace_xml);	
	}else if (est_suivant_uniteCourante(_optIndice_)){	// cas epsilon	
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return NULL;
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return exp;
}

n_appel * appelFct(){										//APPF
	n_l_exp * le = NULL;
	char * temp2= malloc(sizeof(char)*100);
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==ID_FCT){
		nom_token(uniteCourante,nom,valeur);
		strcpy(temp2,valeur);
		consommer(ID_FCT,&uniteCourante,trace_xml);
		consommer(PARENTHESE_OUVRANTE,&uniteCourante,trace_xml);
		le=listeExpressions();
		consommer(PARENTHESE_FERMANTE,&uniteCourante,trace_xml);
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cree_n_appel(temp2,le);
}

n_l_exp * listeExpressions(){								//LEXP
	n_exp * exp = NULL;
	n_l_exp * leb = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(est_premier_uniteCourante(_expression_)){
		exp=expression();
		leb=listeExpressionsBis();
	}else if (est_suivant_uniteCourante(_listeExpressions_)){
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return NULL;
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cree_n_l_exp(exp,leb);
}

n_l_exp * listeExpressionsBis(){								//LEXPB
	n_exp * exp = NULL;
	n_l_exp * leb = NULL;
	affiche_balise_ouvrante(__FUNCTION__, trace_xml);
	if(uniteCourante==VIRGULE){
		consommer(VIRGULE,&uniteCourante,trace_xml);
		exp=expression();
		leb=listeExpressionsBis();
	}else if (est_suivant_uniteCourante(_listeExpressionsBis_)){	// cas epsilon
		affiche_balise_fermante(__FUNCTION__, trace_xml);
		return NULL;
	}else{
		erreur((char *) __func__);
	}
	affiche_balise_fermante(__FUNCTION__, trace_xml);
	return cree_n_l_exp(exp,leb);
}
