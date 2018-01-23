#Par Gaëtan Perrot et Lucas Moragues

SHELL  = /bin/bash
CC     = gcc
CFLAGS = -Wall -W -std=c99 -pedantic
MKDEP   = gcc -MM
LIBS   = 

CFILES  = 
# Mettre ici le nom de l'executable.
EXEC    = main

# Calcul automatique de la liste des fichiers ".o" a partir de CFILES.
OBJECTS := $(CFILES:%.c=%.o)

%.o : %.c
	$(CC) $(CFLAGS) -c $*.c -g

all :: $(EXEC)

$(EXEC) : $(OBJECTS)
	$(CC) -o $@ $^ $(LIBS)

clean ::
	$(RM) *.o *~ $(EXEC) depend

depend : *.c *.h
	$(MKDEP) *.c >| depend

# Inclut le fichier des dependances. 
# Le "-" devant include supprime l'erreur si le fichier est absent.
-include depend

help ::
	@ echo "       make all       pour tout compiler"
	@ echo "       make clean     pour tout nettoyer"
	@ echo "       make distclean     pour tout nettoyer"
	@ echo "       make zip       pour tout sauvegarder"
	@ echo "       make tar       pour tout sauvegarder"

clean ::
	\rm -f *.o core depend

distclean :: clean
	\rm -f *% $(EXEC) depend
	
zip :: distclean
	@N=$$(pwd) ; N=$$(basename "$$N") ;\
	(cd .. && zip -r "$$N.zip" "$$N" --exclude='svg*' --exclude='*.tgz' --exclude='*.zip' && \
	echo "DONE ../$$N.zip")

tar :: distclean
	@N=$$(pwd) ; N=$$(basename "$$N") ;\
	(cd .. && tar cvfz "$$N.tgz" "$$N" --exclude='svg*' --exclude='*.tgz' && \
	 echo "DONE ../$$N.tgz")
