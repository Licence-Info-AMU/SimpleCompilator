#ifndef __COMPILATEUR__
#define __COMPILATEUR__

void usage();

void lastUpdate();

void readFile(char *file);

void analyseur_lexical(char *file);

void analyseur_syntaxique(char *arg, int verbose);

void analyseur_syntaxique_abs(char *arg);

void table_symboles(char *arg);

void genere_code_intel(char *arg);

#endif
