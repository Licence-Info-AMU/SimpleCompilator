#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboles.h"

int yylex();

extern char *yytext;   // déclaré dans analyseur_lexical
extern int  yylineno;  // déclaré dans analyseur_lexical
int indent_xml = 0;
int indent_step = 1; // mettre à 0 pour ne pas indenter

/*******************************************************************************
 * Affiche le message d'alerte donné en paramètre, avec le numéro de ligne 
 ******************************************************************************/
void warning(char *message) {
  fprintf (stderr, "WARNING ligne %d : ", yylineno);
  fprintf (stderr, "%s\n", message);
}

/*******************************************************************************
 * Affiche le message d'erreur donné en paramètre, avec le numéro de ligne 
 ******************************************************************************/
void erreur(char *message) {
  fprintf (stderr, "ERREUR ligne %d : ", yylineno);
  fprintf (stderr, "%s\n", message);
  exit(1);
}

/*******************************************************************************
 * Affiche le message d'alerte donné en paramètre, avec le numéro de ligne.
 * Le message d'alerte peut contenir un %s variable, qui sera donné en 
 * argument
 ******************************************************************************/
void warning_1s(char *message, char *s) {
  fprintf (stderr, "WARNING ligne %d : ", yylineno);
  fprintf( stderr, message, s );
  fprintf( stderr, "\n" );
}

/*******************************************************************************
 * Affiche le message d'erreur donné en paramètre, avec le numéro de ligne.
 * Le message d'erreur peut contenir un %s variable, qui sera donné en 
 * argument s
 ******************************************************************************/
void erreur_1s(char *message, char *s) {
  fprintf( stderr, "Ligne %d : ", yylineno );
  fprintf( stderr, message, s );
  fprintf( stderr, "\n" );
  exit(1);
}

/*******************************************************************************
 * 
 ******************************************************************************/
char *duplique_chaine(char *src) {
  char *dest = malloc(sizeof(char) * strlen(src));
  strcpy(dest, src);
  return dest;
}

/*******************************************************************************
 * 
 ******************************************************************************/
void indent() {
    int i;
    for( i = 0; i < indent_xml; i++ ) {
      printf( "  " );
    }    
}
/*******************************************************************************
 * 
 ******************************************************************************/
void affiche_balise_ouvrante(const char *fct_, int trace_xml) {
  if( trace_xml ) {
    indent();
    indent_xml += indent_step ;
	  fprintf (stdout, "<%s>\n", fct_);
	}  
}

/*******************************************************************************
 * 
 ******************************************************************************/
void affiche_balise_fermante(const char *fct_, int trace_xml) {
  if(trace_xml) {
    indent_xml -= indent_step ;
    indent();
    fprintf (stdout, "</%s>\n", fct_);
  }
}

/*******************************************************************************
 * 
 ******************************************************************************/
void affiche_texte(char *texte_, int trace_xml) {
  if(trace_xml) {
    indent();
    fprintf (stdout, "%s\n", texte_);
  }
}

/*******************************************************************************
 * 
 ******************************************************************************/
void affiche_xml_texte( char *texte_ ) {
  int i = 0;
  while( texte_[ i ] != '\0' ) {
    if( texte_[ i ] == '<' ) {
      fprintf( stdout, "&lt;" );
    }
    else if( texte_[ i ] == '>' ) {
      fprintf( stdout, "&gt;" );
    }
    else if( texte_[ i ] == '&' ) {
      fprintf( stdout, "&amp;" );
    }
    else {
      putchar( texte_[i] );
    }
    i++;
  }
}

/*******************************************************************************
 * 
 ******************************************************************************/
void affiche_element(char *fct_, char *texte_, int trace_xml) {
  if(trace_xml) {
    indent();
    fprintf (stdout, "<%s>", fct_ );
    affiche_xml_texte( texte_ );
    fprintf (stdout, "</%s>\n", fct_ );
  }
}

/*******************************************************************************
 * Fonction auxiliaire appelée par l'analyseur syntaxique tout simplement pour 
 * afficher des messages d'erreur et l'arbre XML 
 ******************************************************************************/
void nom_token( int token, char *nom, char *valeur ) {
  int i;    
  char *tableMotsClefs[] = {"si", "alors", "sinon", "tantque", "faire", "entier", "retour", "lire", "ecrire"};
  int codeMotClefs[] = {SI, ALORS, SINON, TANTQUE, FAIRE, ENTIER, RETOUR, LIRE, ECRIRE};
  int nbMotsClefs = 9;
  
  strcpy( nom, "symbole" );
  if(token == POINT_VIRGULE) strcpy( valeur, "POINT_VIRGULE");
  else if(token == PLUS) strcpy(valeur, "PLUS");
  else if(token == MOINS) strcpy(valeur, "MOINS");
  else if(token == FOIS) strcpy(valeur, "FOIS");
  else if(token == DIVISE) strcpy(valeur, "DIVISE");
  else if(token == PARENTHESE_OUVRANTE) strcpy(valeur, "PARENTHESE_OUVRANTE");
  else if(token == PARENTHESE_FERMANTE) strcpy(valeur, "PARENTHESE_FERMANTE");
  else if(token == CROCHET_OUVRANT) strcpy(valeur, "CROCHET_OUVRANT");
  else if(token == CROCHET_FERMANT) strcpy(valeur, "CROCHET_FERMANT");
  else if(token == ACCOLADE_OUVRANTE) strcpy(valeur, "ACCOLADE_OUVRANTE");
  else if(token == ACCOLADE_FERMANTE) strcpy(valeur, "ACCOLADE_FERMANTE");
  else if(token == EGAL) strcpy(valeur, "EGAL");
  else if(token == INFERIEUR) strcpy(valeur, "INFERIEUR");
  else if(token == ET) strcpy(valeur, "ET");
  else if(token == OU) strcpy(valeur, "OU");
  else if(token == NON) strcpy(valeur, "NON"); 
  else if(token == FIN) strcpy(valeur, "FIN");
  else if(token == VIRGULE) strcpy(valeur, "VIRGULE");

  else if( token == ID_VAR ) {
    strcpy( nom, "id_variable" );  
    strcpy( valeur, yytext );        
  }
  else if( token == ID_FCT ) {
    strcpy( nom, "id_fonction" );    
    strcpy( valeur, yytext );    
  }
  else if( token == NOMBRE ) {
    strcpy( nom, "nombre" );
    strcpy( valeur, yytext ); 
  }
  else {
    strcpy( nom, "mot_clef" );
    for(i=0; i < nbMotsClefs; i++){
      if( token ==  codeMotClefs[i] ){
        strcpy( valeur, tableMotsClefs[i] );
        break;
      }
    }
  }  
}
/*******************************************************************************
 * Fonction auxiliaire appelée par le compilo en mode -l, pour tester l'analyseur
 * lexical et, étant donné un programme en entrée, afficher la liste des tokens.
 ******************************************************************************/
void test_yylex_internal(FILE *yyin) {
  int uniteCourante;
  char nom[100];
  char valeur[100];  
  do {
    uniteCourante = yylex();  
    nom_token( uniteCourante, nom, valeur );
    printf("%s\t%s\t%s\n", yytext, nom, valeur);
  } while (uniteCourante != FIN);
}
