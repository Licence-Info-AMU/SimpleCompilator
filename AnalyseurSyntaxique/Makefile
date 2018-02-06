CC = gcc
FLEX = ./flex

LIBS = -lm 
CCFLAGS = -Wall -ggdb

OBJ = analyseur_lexical_flex.o util.o AnalyseurSyntaxique.o
EXEC = compilateur test_yylex

all: $(EXEC)

compilateur: AnalyseurSyntaxique.c $(OBJ)
	$(CC) $(CCFLAGS) -o AnalyseurSyntaxique $(OBJ)

test_yylex: test_yylex.c $(OBJ)
	$(CC) $(CCFLAGS) -o test_yylex $(OBJ)

analyseur_lexical_flex.c: analyseur_lexical.flex
	$(FLEX) -o $@ $^

analyseur_lexical_flex.o: analyseur_lexical_flex.c
	$(CC) $(CCFLAGS) -c $^

AnalyseurSyntaxique.o: AnalyseurSyntaxique.c
	$(CC) $(CCFLAGS) -c $^

util.o: util.c
	$(CC) $(CCFLAGS) -c $^

.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f analyseur_lexical_flex.c
	
distclean: clean
	rm -f $(EXEC)
	
