#include <stdio.h>
#include "syntabs.h"
#include "util.h"
#include "tabsymboles.h"
#include "parcours_arbre_abstrait.h"

extern int portee;
extern int adresseLocaleCourante;
extern int adresseArgumentCourant;
int adresseGlobaleCourante = 0;
int paramcpt;
int ind_fonc=0;
int cpt_label = 0;
FILE * fp;

/*-------------------------------------------------------------------------*/

void parcours_n_prog(n_prog *n){
	portee = P_VARIABLE_GLOBALE;
	adresseLocaleCourante = 0;
	adresseArgumentCourant = 0;
	fp = fopen ("test.asm", "w+");
	if(showIntel){
		printf("\n%%include \"io.asm\"\nsection .bss\nsinput: resb 255\n");	
		fprintf(fp,"%%include \"io.asm\"\nsection .bss\nsinput: resb 255\n");
	}
	parcours_l_dec(n->variables);
	if(showIntel){
		printf("section .text\nglobal _start\n_start:\n\tcall main\n\tmov eax, 1\n\tint 0x80\n");
		fprintf(fp,"section .text\nglobal _start\n_start:\n\tcall main\n\tmov eax, 1\n\tint 0x80\n");
	}
	parcours_l_dec(n->fonctions);
	fclose(fp);
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
	int id = rechercheExecutable(n->u.affecte_.var->nom);
	if (id !=-1){
		parcours_var(n->u.affecte_.var);
		parcours_exp(n->u.affecte_.exp); 
		if (showIntel){
			printf("\tpop ebx\n");
			fprintf(fp, "\tpop ebx\n");
			if(tabsymboles.tab[id].portee == P_VARIABLE_GLOBALE){
				if(tabsymboles.tab[id].type == T_TABLEAU_ENTIER){
					printf("\tpop eax\n\timul eax, 4\n\tmov[%s + eax], ebx\n",n->u.affecte_.var->nom);
					fprintf(fp, "\tpop eax\n\timul eax, 4\n\tmov[%s + eax], ebx\n",n->u.affecte_.var->nom);
				}else{
					printf("\tmov [v%s], ebx\n",n->u.affecte_.var->nom);
					fprintf(fp, "\tmov [v%s], ebx\n",n->u.affecte_.var->nom);
				}
			}else if (tabsymboles.tab[id].portee == P_VARIABLE_LOCALE){
				printf("\tmov [ebp %d], ebx\n",-4-tabsymboles.tab[id].adresse);
				fprintf(fp, "\tmov [ebp %d], ebx\n",-4-tabsymboles.tab[id].adresse);
			}
		}
	}	
}

/*-------------------------------------------------------------------------*/

void parcours_instr_appel(n_instr *n){
  parcours_appel(n->u.appel); 
}
/*-------------------------------------------------------------------------*/

void parcours_appel(n_appel *n){
	int fonc_id = rechercheExecutable(n->fonction);
	if (fonc_id >= 0){
		parcours_l_exp(n->args);  				
	}else{
		printf("Nom de fonction inconnue : %s\n", n->fonction);
	}
}

/*-------------------------------------------------------------------------*/

void parcours_instr_retour(n_instr *n){
	parcours_exp(n->u.retour_.expression);
	if (showIntel){
		printf("\tpop  eax\n\tmov [ebp + %d], eax\n",8+4*tabsymboles.tab[ind_fonc].complement);
		fprintf(fp,"\tpop  eax\n\tmov [ebp + %d], eax\n",8+4*tabsymboles.tab[ind_fonc].complement);
	}
}

/*-------------------------------------------------------------------------*/

void parcours_instr_ecrire(n_instr *n){
	parcours_exp(n->u.ecrire_.expression);
	if (showIntel){
		printf("\tpop eax\n\tcall iprintLF\n");
		fprintf(fp,"\tpop eax\n\tcall iprintLF\n");
	}
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
	int k = rechercheExecutable(n->u.var->nom);
	if (k ==-1){
		parcours_var(n->u.var);
	} 
	if (showIntel){
		if(tabsymboles.tab[k].portee == P_ARGUMENT){
			printf("\tmov ebx, [ebp + %d]\n\tpush ebx\n",4+4*tabsymboles.tab[ind_fonc].complement-tabsymboles.tab[k].adresse);
			fprintf(fp,"\tmov ebx, [ebp + %d]\n\tpush ebx\n",4+4*tabsymboles.tab[ind_fonc].complement-tabsymboles.tab[k].adresse);
		}else if (tabsymboles.tab[k].portee == P_VARIABLE_GLOBALE) {
			printf("\tmov ebx, [v%s]\n\tpush ebx\n",n->u.var->nom);
			fprintf(fp,"\tmov ebx, [v%s]\n\tpush ebx\n",n->u.var->nom);	
		}else if (tabsymboles.tab[k].portee == P_VARIABLE_LOCALE){
			printf("\tmov ebx, [ebp - %d]\n\tpush ebx\n",4-tabsymboles.tab[k].adresse);
			fprintf(fp,"\tmov ebx, [ebp - %d]\n\tpush ebx\n",4-tabsymboles.tab[k].adresse);
		}
	}
}

/*-------------------------------------------------------------------------*/
void parcours_opExp(n_exp *n){
	if (showIntel){
		char debut[64], suite[64], sinon[64];
		sprintf(debut,"e%d",cpt_label++);
		sprintf(suite,"e%d",cpt_label++);
		sprintf(sinon,"e%d",cpt_label++);
	}
	if( n->u.opExp_.op1 != NULL ) {
		parcours_exp(n->u.opExp_.op1);
	}
	
	if( n->u.opExp_.op2 != NULL ) {
		parcours_exp(n->u.opExp_.op2);
	} 
	
	if (showIntel){
		if(n->u.opExp_.op == plus){
			printf("\tpop ebx\n\tpop eax\n\tadd eax, ebx\n\tpush eax\n");
			fprintf(fp, "\tpop ebx\n\tpop eax\n\tadd eax, ebx\n\tpush eax\n");
		}else if(n->u.opExp_.op == moins){
			printf("\tpop ebx\n\tpop eax\n\tsub eax, ebx\n\tpush eax\n");
			fprintf(fp, "\tpop ebx\n\tpop eax\n\tsub eax, ebx\n\tpush eax\n");
		}else if(n->u.opExp_.op == fois){
			printf("\tpop ebx\n\tpop eax\n\timul ebx\n\tpush eax\n");
			fprintf(fp, "\tpop ebx\n\tpop eax\n\timul ebx\n\tpush eax\n");
		}else if(n->u.opExp_.op == divise){
			printf("\tpop ebx\n\tpop eax\n\tdiv ebx\n\tpush eax\n");
			fprintf(fp, "\tpop ebx\n\tpop eax\n\tdiv ebx\n\tpush eax\n");
		}else if(n->u.opExp_.op == egal){	
			printf("\tpop ebx\n\tpop eax\n\tcmp eax, ebx\n\tje %s\n\tpush 0\n\tjmp %s\n%s:\n\tpush 1\n%s:\n",sinon,suite,sinon,suite);
			fprintf(fp, "\tpop ebx\n\tpop eax\n\tcmp eax, ebx\n\tje %s\n\tpush 0\n\tjmp %s\n%s:\n\tpush 1\n%s:\n",sinon,suite,sinon,suite);
		}else if(n->u.opExp_.op == inferieur){
			printf("\tpop ebx\n\tpop eax\n\tcmp eax, ebx\n\tjl %s\n\tpush 0\n\tjmp %s\n%s:\n\tpush 1\n%s:\n",sinon,suite,sinon,suite);
			fprintf(fp, "\tpop ebx\n\tpop eax\n\tcmp eax, ebx\n\tjl %s\n\tpush 0\n\tjmp %s\n%s:\n\tpush 1\n%s:\n",sinon,suite,sinon,suite);
		}else if(n->u.opExp_.op == ou){
			printf("%s:\n\tpop ebx\n\tpop eax\n\tadd eax, ebx\n\tcmp eax, 0\n\tje %s\n push 1\n\tjmp %s\n\t%s:\n\tpush 0\n\t%s:\n",debut,debut,suite,debut,suite);
			fprintf(fp, "%s:\n\tpop ebx\n\tpop eax\n\tadd eax, ebx\n\tcmp eax, 0\n\tje %s\n push 1\n\tjmp %s\n\t%s:\n\tpush 0\n\t%s:\n",debut,debut,suite,debut,suite);
		}else if(n->u.opExp_.op == et){
			printf("%s:\n\tpop ebx\n\tpop eax\n\timul eax, ebx\n\tcmp eax, 0\n\tje %s\n push 1\n\tjmp %s\n\t%s:\n\tpush 0\n\t%s:\n",debut,debut,suite,debut,suite);
			fprintf(fp, "%s:\n\tpop ebx\n\tpop eax\n\timul eax, ebx\n\tcmp eax, 0\n\tje %s\n push 1\n\tjmp %s\n\t%s:\n\tpush 0\n\t%s:\n",debut,debut,suite,debut,suite);
		}else if(n->u.opExp_.op == non){
			printf("%s:\n\tpop eax\n\tcmp eax, 0\n\tje %s\n\tpush 0\n\tjmp %s\n%s:\n\tpush 1\n%s:",debut,debut,suite,debut,suite);
			fprintf(fp, "%s:\n\tpop eax\n\tcmp eax, 0\n\tje %s\n\tpush 0\n\tjmp %s\n%s:\n\tpush 1\n%s:",debut,debut,suite,debut,suite);
		}
	}
}

/*-------------------------------------------------------------------------*/

void parcours_intExp(n_exp *n){
	if (showIntel){
		printf("\tpush %d\n",n->u.entier);
		fprintf(fp,"\tpush %d\n",n->u.entier);
	}
}

/*-------------------------------------------------------------------------*/
void parcours_lireExp(n_exp *n){
	if (showIntel){
		printf("\tmov eax, sinput\n\tcall readline\n\tmov eax, sinput\n\tcall atoi\n\tpush eax\n");
		fprintf(fp,"\tmov eax, sinput\n\tcall readline\n\tmov eax, sinput\n\tcall atoi\n\tpush eax\n");
	}
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
		paramcpt++;
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
		paramcpt=0;
		int id=ajouteIdentificateur(n->nom, portee, T_FONCTION, adresseLocaleCourante, 0);
		ind_fonc=rechercheExecutable(n->nom);
		entreeFonction();
		if(showIntel){
			printf("%s:\n\tpush ebp\n\tmov ebp, esp\n",n->nom);
			fprintf(fp,"%s:\n\tpush ebp\n\tmov ebp, esp\n",n->nom);
		}		
		parcours_l_dec(n->u.foncDec_.param);
		tabsymboles.tab[id].complement = paramcpt;
		portee = P_VARIABLE_LOCALE;
		parcours_l_dec(n->u.foncDec_.variables);
		parcours_instr(n->u.foncDec_.corps);
		sortieFonction(trace_tabsymb);
		if(showIntel){
			printf("\tpop ebp\n\tret\n");
			fprintf(fp,"\tpop ebp\n\tret\n");
		}
	}
}

/*-------------------------------------------------------------------------*/

void parcours_varDec(n_dec *n){
	int var_id = rechercheDeclarative(n->nom); // On cherche si "nom" existe deja
	// Verifier qu'elles n'ont pas la meme portee
	if((var_id >= 0) && (tabsymboles.tab[var_id].portee != portee) || (var_id == -1)){	
		int adresse = 0;
		if (portee == P_VARIABLE_LOCALE){
			adresse = adresseLocaleCourante;
			adresseLocaleCourante += 4;
		}else if (portee == P_ARGUMENT){
			adresse = adresseArgumentCourant;
			adresseArgumentCourant += 4;
		}else{
			adresse = adresseGlobaleCourante;
			adresseGlobaleCourante += 4;
			if(showIntel){
				printf("\t%s resd 1\n", n->nom);
				fprintf(fp,"\t%s resd 1\n", n->nom);
			}
		}
		ajouteIdentificateur(n->nom, portee, T_ENTIER, adresse, 1);
	}else{
		printf("Variable déjà declarée : %s\n", n->nom);
	}
}

/*-------------------------------------------------------------------------*/

void parcours_tabDec(n_dec *n){
	int var_id = rechercheDeclarative(n->nom);
	// Verifier qu'elles n'ont pas la meme portee
	if((var_id >= 0) && (tabsymboles.tab[var_id].portee != portee) || (var_id == -1)){
		int adresse = 0;
		if (portee == P_VARIABLE_LOCALE){
			  adresse = adresseLocaleCourante;
			  adresseLocaleCourante += 4*(n->u.tabDec_.taille);
		}else if (portee == P_ARGUMENT){
			  adresse = adresseArgumentCourant;
			  adresseArgumentCourant += 4*(n->u.tabDec_.taille);
		}else{
			adresse = adresseGlobaleCourante;
			adresseGlobaleCourante += 4*(n->u.tabDec_.taille);
			if(showIntel){
				printf("\t%s resd %d\n", n->nom,n->u.tabDec_.taille);
				fprintf(fp,"\t%s resd %d\n", n->nom,n->u.tabDec_.taille);
			}
		}	
		ajouteIdentificateur(n->nom, portee, T_TABLEAU_ENTIER, adresse, n->u.tabDec_.taille);
	}else{
		printf("Variable déjà declarée : %s\n", n->nom);
	}
}

/*-------------------------------------------------------------------------*/

void parcours_var(n_var *n){
	//printf("n->nom : %s\n n->type : %d\n simple : %d\n",n->nom,n->type,indicee);
	if(n->type == simple) {
		parcours_var_simple(n);
	}else if(n->type == indicee) {
		parcours_var_indicee(n);
	}
}

/*-------------------------------------------------------------------------*/
void parcours_var_simple(n_var *n){
	int var_id = rechercheExecutable(n->nom); // On cherche si "nom" existe
	if (var_id >= 0){ // Si on trouve var_id
		if(tabsymboles.tab[var_id].type != T_ENTIER){
			printf("Mauvais type (Entier attendu) : %s\n ", n->nom);
		}
	}else{
		printf("Variable non declaree : %s\n", n->nom);
	}
}

/*-------------------------------------------------------------------------*/
void parcours_var_indicee(n_var *n){
	int var_id = rechercheExecutable(n->nom); // On cherche si "nom" existe
	if (var_id >= 0){ // Si on trouve var_id
		if(tabsymboles.tab[var_id].type != T_TABLEAU_ENTIER){
			printf("Mauvais type (Tableau attendu) : %s\n", n->nom);
		}else{
			parcours_exp( n->u.indicee_.indice );
		}
	}else{
		printf("Variable non declaree : %s\n", n->nom);
	}
}
/*-------------------------------------------------------------------------*/
