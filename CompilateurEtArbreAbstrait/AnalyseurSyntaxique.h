#ifndef __ANALYSEURSYNTAXIQUE__
#define __ANALYSEURSYNTAXIQUE__
#include "syntabs.h"

int uniteCourante;
int trace_xml;

int est_premier_uniteCourante(int arg);

int est_suivant_uniteCourante(int arg);

void programme();

void optDecVariables();

void listeDecVariables();

void listeDecVariablesBis();

void declarationVariable();

void optTailleTableau();

void listeDecFonctions();

void declarationFonction();

void listeParam();

void optListeDecVariables();

void instruction();

void instructionAffect();

void instructionBloc();

void listeInstructions();

void instructionSi();

void optSinon();

void instructionTantque();

void instructionAppel();

void instructionRetour();

void instructionEcriture();

void instructionVide();

void expression();

void expressionBis();

void conjonction();

void conjonctionBis();

void comparaison();

void comparaisonBis();

void expArith();

void expArithBis();

void terme();

void termeBis();

void negation();

void facteur();

void var();

void optIndice();

void appelFct();

void listeExpressions();

void listeExpressionsBis();

#endif
