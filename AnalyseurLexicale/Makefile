CC = gcc
FLEX = ./flex

LIBS = -lm 
CCFLAGS = -Wall -ggdb

OBJ = analyseur_lexical_flex.o util.o

all: test_yylex

test_yylex: test_yylex.c $(OBJ)
	$(CC) $(CCFLAGS) -o test_yylex test_yylex.c $(OBJ)

analyseur_lexical_flex.c: analyseur_lexical.flex
	$(FLEX) -o $@ $^

analyseur_lexical_flex.o: analyseur_lexical_flex.c
	$(CC) $(CCFLAGS) -c $^

util.o: util.c
	$(CC) $(CCFLAGS) -c $^

.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f test_yylex
	- rm -f analyseur_lexical_flex.c
