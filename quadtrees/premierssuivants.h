#ifndef _PREMIER_SUIVANT_
#define _PREMIER_SUIVANT_

#include "symboles.h"

//Q ->	r | g | b | (Q Q Q Q)

int suivants[NB_NON_TERMINAUX+1][NB_TERMINAUX+1];
int premiers[NB_NON_TERMINAUX+1][NB_TERMINAUX+1];

void initialise_premiers(void);

void initialise_suivants(void);

int est_premier(int terminal, int non_terminal);

int est_suivant(int terminal, int non_terminal);

#endif