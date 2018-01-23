#include<stdio.h>
#include<stdlib.h>
#include"analyseur_xml.h"

int main(int argc, char *argv[])
 {
   noeud *racine1;
   noeud *racine2;
   int resultat;
   int verbose;

   if((argc != 3) && (argc != 4)){
     fprintf(stderr, "usage: %s fichier_xml fichier_xml [v]\n", argv[0]);
     exit(1);

   }
   verbose = (argc == 4);

   yyin = fopen(argv[1], "r");
   if(yyin == NULL){
     fprintf(stderr, "impossible d'ouvrir le fichier %s\n", argv[1]);
     exit(1);
   }
   
   fprintf(stderr, "analyse du fichier : %s\n", argv[1]);
   racine1 = analyseur_xml();


   fclose(yyin);

   yyin = fopen(argv[2], "r");
   if(yyin == NULL){
     fprintf(stderr, "impossible d'ouvrir le fichier %s\n", argv[2]);
     exit(1);
   }
   
   fprintf(stderr, "analyse du fichier : %s\n", argv[2]);
   racine2 = analyseur_xml();

   fclose(yyin);



   /*   affiche_arbre(racine1);
   printf("\n");
   affiche_arbre(racine2);*/
   
   fprintf(stderr, "comparaison des arbres\n");
   resultat = compare_arbres(racine1, racine2, verbose);
   libere_arbre(racine1);
   libere_arbre(racine2);


   if(resultat){ 
     printf("arbres egaux\n");
     return 0;
   }
   else{ 
     printf("arbres différents\n");
     return 1;
   }

 }
