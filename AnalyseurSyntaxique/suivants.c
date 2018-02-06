#include "symboles.h"
#include "suivants.h"

void initialise_suivants(void){
  int i,j;
  
  // Initialiser toutes les cases du tableau à 0
  for(i=0; i <= NB_NON_TERMINAUX; i++)
    for(j=0; j <= NB_TERMINAUX; j++)
      suivants[i][j] = 0;
      
  //exemple      
  suivants[_programme_][FIN] = 1;
  
  // TODO: Compléter le tableau suivants ici
}

int est_suivant(int terminal, int non_terminal)
{
  return suivants[non_terminal][terminal];
}
