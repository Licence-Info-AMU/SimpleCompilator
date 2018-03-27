CC = gcc
FLEX = ./flex

LIBS = -lm 
CCFLAGS = -Wall -ggdb

OBJ = analyseur_lexical_flex.o util.o Compilateur.o AnalyseurSyntaxique.o premiers.o suivants.o syntabs.o affiche_arbre_abstrait.o
EXEC = compilateur test_yylex

all :: $(EXEC)

compilateur: Compilateur.c $(OBJ)
	$(CC) $(CCFLAGS) -o Compilateur $(OBJ)

test_yylex: test_yylex.c $(OBJ)
	$(CC) $(CCFLAGS) -o test_yylex $(OBJ)

analyseur_lexical_flex.c: analyseur_lexical.flex
	$(FLEX) -o $@ $^

analyseur_lexical_flex.o: analyseur_lexical_flex.c
	$(CC) $(CCFLAGS) -c $^

Compilateur.o: Compilateur.c
	$(CC) $(CCFLAGS) -c $^

util.o: util.c
	$(CC) $(CCFLAGS) -c $^

clean ::
	- rm -f $(OBJ)
	- rm -f analyseur_lexical_flex.c
	
distclean :: clean
	- rm -f $(EXEC)
	
zip :: distclean
	@N=$$(pwd) ; N=$$(basename "$$N") ;\
	(cd .. && zip -r "$$N.zip" "$$N" --exclude='svg*' --exclude='*.tgz' --exclude='*.zip' && \
	echo "DONE ../$$N.zip")

tar :: distclean
	@N=$$(pwd) ; N=$$(basename "$$N") ;\
	(cd .. && tar cvfz "$$N.tgz" "$$N" --exclude='svg*' --exclude='*.tgz' && \
	 echo "DONE ../$$N.tgz")
	 
help ::
	@ echo "       make all       pour tout compiler"
	@ echo "       make clean     pour tout nettoyer"
	@ echo "       make distclean     pour tout nettoyer"
	@ echo "       make zip       pour tout sauvegarder"
	@ echo "       make tar       pour tout sauvegarder"
	
