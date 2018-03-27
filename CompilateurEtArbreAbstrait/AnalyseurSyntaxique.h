#ifndef __ANALYSEURSYNTAXIQUE__
#define __ANALYSEURSYNTAXIQUE__
#include "syntabs.h"

int uniteCourante;
int trace_xml;
char nom[100];
char valeur[100];
int est_premier_uniteCourante(int arg);

int est_suivant_uniteCourante(int arg);

n_prog * programme();

n_l_dec * optDecVariables();

n_l_dec * listeDecVariables();

n_l_dec * listeDecVariablesBis();

n_dec * declarationVariable();

int optTailleTableau();

n_l_dec *  listeDecFonctions();

n_dec * declarationFonction();

n_l_dec * listeParam();

n_l_dec * optListeDecVariables();

n_instr * instruction();

n_instr * instructionAffect();

n_instr * instructionBloc();

n_l_instr * listeInstructions();

n_instr * instructionSi();

n_instr * optSinon();

n_instr * instructionTantque();

n_instr * instructionAppel();

n_instr * instructionRetour();

n_instr * instructionEcriture();

n_instr * instructionVide();

n_exp * expression();

n_exp * expressionBis(n_exp * herite);

n_exp * conjonction();

n_exp * conjonctionBis(n_exp * herite);

n_exp * comparaison();

n_exp * comparaisonBis(n_exp * herite);

n_exp * expArith();

n_exp * expArithBis(n_exp * herite);

n_exp * terme();

n_exp * termeBis(n_exp * herite);

n_exp * negation();

n_exp * facteur();

n_var * var();

n_var * optIndice();

n_appel * appelFct();

n_l_exp * listeExpressions();

n_l_exp * listeExpressionsBis();

#endif
