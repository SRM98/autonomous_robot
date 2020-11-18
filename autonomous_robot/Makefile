########       AVR Project Makefile       ########
#####                                        #####
#####      Makefile produit et ecrit par     #####
#####   Simon Barrette & Jerome Collin pour  #####
-include Makefile_in_common.txt


# Nom de votre projet
# (utilisez un seul mot, exemple: 'monprojet')
PROJECTNAME=test

# Fichiers sources
# Utilisez le suffixe .cpp pour les fichiers C++
# Listez tous les fichiers a compiler, separes par
# un espace. exemple: 'tp1.c tp2.cpp':
PRJSRC= integrationMain_test.cpp

# Inclusions additionnels (ex: -I/path/to/mydir)
INC= -I ./Librairie/

# Libraires a lier (ex: -lmylib)
LIBS= Librairie/librairie.a


####### Cible (Target) #######

#Nom des cibles par defaut
TRG=$(PROJECTNAME).out




####### Creation des commandes du Makefile ####### 



# Implementation de la cible
$(TRG): $(OBJDEPS)
		$(CC) $(LDFLAGS) -o $(TRG) $(OBJDEPS) \
		-lm $(LIBS)

# Verification des dependances (header dependencies)
-include *.d


all:$(TRG)

# Make install permet de compiler le projet puis
# d'ecrire le programme en memoire flash dans votre
# microcontroleur. Celui-ci doit etre branche par cable USB
install: $(HEXROMTRG)				
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMERID)   \
	-p $(MCU) -P -e -U flash:w:$(HEXROMTRG)

# Make clean permet d'effacer tout les fichiers generes
# lors de la compilation
clean:
	$(REMOVE) $(TRG) $(TRG).map $(OBJDEPS) $(HEXTRG) *.d

