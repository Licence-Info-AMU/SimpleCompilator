#include "premierssuivants.h"

void initialise_premiers(void){
	for(int i=0; i <= NB_NON_TERMINAUX; i++)
		for(int j=0; j <= NB_TERMINAUX; j++)
			premiers[i][j] = 0;

	premiers[_quadtrees_][R] = 1;
	premiers[_quadtrees_][G] = 1;
	premiers[_quadtrees_][B] = 1;
	premiers[_quadtrees_][PARENTHESE_OUVRANTE] = 1;
}

void initialise_suivants(void){
	for(int i=0; i <= NB_NON_TERMINAUX; i++)
		for(int j=0; j <= NB_TERMINAUX; j++)
			suivants[i][j] = 0;

	suivants[_quadtrees_][R] = 1;
	suivants[_quadtrees_][G] = 1;
	suivants[_quadtrees_][B] = 1;
	suivants[_quadtrees_][PARENTHESE_FERMANTE] = 1;
	suivants[_quadtrees_][FIN] = 1;
}

int est_suivant(int non_terminal, int terminal){
	return suivants[non_terminal][terminal];
}

int est_premier(int non_terminal, int terminal){
	return premiers[non_terminal][terminal];
}