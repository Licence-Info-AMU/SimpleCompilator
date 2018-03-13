/*
 * Analyseur lexical du compilateur L en FLEX
 */
 
%{
#include "symboles.h"
%}
%option yylineno
%option nounput
%option noinput	

/* Déclarations à compléter ... */
lettre [a-zA-Z]
chiffre [0-9]
nom [a-zA-Z0-9_$]*
%%
"\n"				{	} /* Retours chariot */
"("					{ return PARENTHESE_OUVRANTE; }
")"					{ return PARENTHESE_FERMANTE; }
"R"					{ return R;}
"G"					{ return G;}
"B"					{ return B;}
<<EOF>>				{ return FIN; }  /* Fin de fichier */
%%
int yywrap(){
	return 1;
}
