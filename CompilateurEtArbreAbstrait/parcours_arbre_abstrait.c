#include <stdio.h>
#include "syntabs.h"
#include "util.h"
#include "parcours_arbres_abstrait.h"

extern int portee = P_VARIABLE_GLOBALE;
extern int adresseLocaleCourante = 0;
extern int adresseArgumentCourant = 0;

/*-------------------------------------------------------------------------*/

void parcours_n_prog(n_prog *n){
  parcours_l_dec(n->variables);
  parcours_l_dec(n->fonctions); 
  
}

/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/

void parcours_l_instr(n_l_instr *n){
  if(n){ 
	  parcours_instr(n->tete);
	  parcours_l_instr(n->queue); 
  }
}

/*-------------------------------------------------------------------------*/

void parcours_instr(n_instr *n){
  if(n){
    if(n->type == blocInst) parcours_l_instr(n->u.liste);
    else if(n->type == affecteInst) parcours_instr_affect(n);
    else if(n->type == siInst) parcours_instr_si(n);
    else if(n->type == tantqueInst) parcours_instr_tantque(n);
    else if(n->type == appelInst) parcours_instr_appel(n);
    else if(n->type == retourInst) parcours_instr_retour(n);
    else if(n->type == ecrireInst) parcours_instr_ecrire(n);
  }
}

/*-------------------------------------------------------------------------*/

void parcours_instr_si(n_instr *n){  
  parcours_exp(n->u.si_.test);
  parcours_instr(n->u.si_.alors);
  if(n->u.si_.sinon){
    parcours_instr(n->u.si_.sinon);
  }
}

/*-------------------------------------------------------------------------*/

void parcours_instr_tantque(n_instr *n){
  parcours_exp(n->u.tantque_.test);
  parcours_instr(n->u.tantque_.faire);
}

/*-------------------------------------------------------------------------*/

void parcours_instr_affect(n_instr *n){
  parcours_var(n->u.affecte_.var);
  parcours_exp(n->u.affecte_.exp); 
}

/*-------------------------------------------------------------------------*/

void parcours_instr_appel(n_instr *n){
  parcours_appel(n->u.appel); 
}
/*-------------------------------------------------------------------------*/

void parcours_appel(n_appel *n){
	int fonc_id = rechercheDeclarative(n->fonction);
	if (fonc_id >= 0){
		entreeFonction();
		parcours_l_exp(n->args);  // ATTENTION : après avoir traité les arguments, votre programme doit modifier __excplicitement___
		tabsymboles.base = tabsymboles.sommet;  // la valeur de `portee` pour stocker les var. locales avec la portée P_VARIABLE_LOCALE
		portee = P_VARIABLE_LOCALE;
		sortieFonction(0);
		portee = P_VARIABLE_GLOBALE;
	}else{
		printf("Nom de fonction inconnue : %s\n", n->fonction);
	}
}

/*-------------------------------------------------------------------------*/

void parcours_instr_retour(n_instr *n){
  parcours_exp(n->u.retour_.expression);
}

/*-------------------------------------------------------------------------*/

void parcours_instr_ecrire(n_instr *n){
  parcours_exp(n->u.ecrire_.expression);
}

/*-------------------------------------------------------------------------*/

void parcours_l_exp(n_l_exp *n){
  if(n){
    parcours_exp(n->tete);
    parcours_l_exp(n->queue);
  } 
}

/*-------------------------------------------------------------------------*/

void parcours_exp(n_exp *n){
  if(n->type == varExp) parcours_varExp(n);
  else if(n->type == opExp) parcours_opExp(n);
  else if(n->type == intExp) parcours_intExp(n);
  else if(n->type == appelExp) parcours_appelExp(n);
  else if(n->type == lireExp) parcours_lireExp(n);
}

/*-------------------------------------------------------------------------*/

void parcours_varExp(n_exp *n){
  parcours_var(n->u.var);  
}

/*-------------------------------------------------------------------------*/
void parcours_opExp(n_exp *n){
  if( n->u.opExp_.op1 != NULL ) {
    parcours_exp(n->u.opExp_.op1);
  }
  if( n->u.opExp_.op2 != NULL ) {
    parcours_exp(n->u.opExp_.op2);
  } 
}

/*-------------------------------------------------------------------------*/

void parcours_intExp(n_exp *n){

}

/*-------------------------------------------------------------------------*/
void parcours_lireExp(n_exp *n){

}

/*-------------------------------------------------------------------------*/

void parcours_appelExp(n_exp *n){
  parcours_appel(n->u.appel);
}

/*-------------------------------------------------------------------------*/

void parcours_l_dec(n_l_dec *n){
  if( n ){   
    parcours_dec(n->tete);
    parcours_l_dec(n->queue);   
  }
}

/*-------------------------------------------------------------------------*/

void parcours_dec(n_dec *n){
  if(n){
    if(n->type == foncDec) {
      parcours_foncDec(n);
    }
    else if(n->type == varDec) {
      parcours_varDec(n);
    }
    else if(n->type == tabDec) { 
      parcours_tabDec(n);
    }
  }
}

/*-------------------------------------------------------------------------*/

void parcours_foncDec(n_dec *n){
	int fonc_id = rechercheDeclarative(n->nom);
	if (fonc_id >= 0){
		printf("Nom de fonction deja utilise : %s\n", n->nom);
	}else{
		// Ajout de la fonction
		adresseLocaleCourante = 0;
		tabsymboles.base = tabsymboles.sommet;
		ajouteIdentificateur(n->nom, portee, T_FONCTION, adresseLocaleCourante, 0); // TODO : nombre de parametres en dernier champ
	}
	parcours_l_dec(n->u.foncDec_.param);
	parcours_l_dec(n->u.foncDec_.variables);
	parcours_instr(n->u.foncDec_.corps);
}

/*-------------------------------------------------------------------------*/

void parcours_varDec(n_dec *n){
	int var_id = rechercheDeclarative(n->nom); // On cherche si "nom" existe deja
	if (var_id >= 0){
	// Verifier qu'elles n'ont pas la meme portee   

	}else{
	// Ajout de la variable
	ajouteIdentificateur(n->nom, portee, T_ENTIER, adresseLocaleCourante, 1);
	adresseLocaleCourante += 4;
	}
}

/*-------------------------------------------------------------------------*/

void parcours_tabDec(n_dec *n){
	int var_id = rechercheDeclarative(n->nom);
	if (var_id >= 0)
	{

	}else{
	// Verifier qu'elles n'ont pas la meme portee
	int id =  ajouteIdentificateur(n->nom, portee, T_TABLEAU_ENTIER, 
						 adresseLocaleCourante, n->u.tabDec_.taille);
	adresseLocaleCourante += 4*(n->u.tabDec_.taille);
	}
}

/*-------------------------------------------------------------------------*/

void parcours_var(n_var *n){
  if(n->type == simple) {
    parcours_var_simple(n);
  }
  else if(n->type == indicee) {
    parcours_var_indicee(n);
  }
}

/*-------------------------------------------------------------------------*/
void parcours_var_simple(n_var *n){
	int var_id = rechercheExecutable(n->nom); // On cherche si "nom" existe
	if (id >= 0){ // Si on trouve var_id

	}else{
		printf("Variable non declaree : %s\n", n->nom);
	}
}

/*-------------------------------------------------------------------------*/
void parcours_var_indicee(n_var *n){
	int var_id = rechercheExecutable(n->nom); // On cherche si "nom" existe
	if (id >= 0){ // Si on trouve var_id

	}else{
		printf("Variable non declaree : %s\n", n->nom);
	}
  parcours_exp( n->u.indicee_.indice );
}
/*-------------------------------------------------------------------------*/
