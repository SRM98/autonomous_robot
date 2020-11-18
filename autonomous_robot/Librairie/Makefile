#Auteurs : Hugo Lirette, Jennifer Girard, Sergiu R. Mihaltan, Rosanne Larocque et Ines Goddi.

#Note: Ce makefile correspond au makefile que nous nous sommes fait donnes en classe et sur lequel, nous avons effectue des modifications.
#Autre note: le iste gl.developpez.com/tutoriel/outil/makefile/ nous a grandement aide a faire ce makefile.
####### Inclusions #######

#Pour eviter la repetition de code, nous avons inclut toutes les lignes de commandes qui etaient pareille aux deux makefiles dans un meme
#fichier texte.
#Permet d'inclure le makefile en commun
-include ../Makefile_in_common.txt



####### variables #######

#Nom de notre projet.
PROJECTNAME=librairie

# Fichiers sources
#Note: ici, on utilise wildcard pour qu'on puisse utiliser nos "jokers" soit, *.cpp, *.o, etc.
#Cette ligne de commande permet d'aller chercher tous les fichiers cpp qui sont dans le repertoire.
PRJSRC= $(wildcard *.cpp)	

#Compilateur pour la librairie
#Puisque l'on veut produire une librairie statique, on doit remplacer avr-gcc qui produit l'executable
#par un appel a une commande qui se nomme ar pour archive. Note: tire des notes sur le tp8 sur le
#site web de inf1995. Note, on ne peut pas reprendre la variable CC car elle est deja prise dans le
#makefile en commun.
CCC= avr-ar


####### Options de compilation #######

#Toujours selon le site web de inf1995, on utilise les options c(pour creer l'archive), r (pour inserer les fichiers membres) et 
#s (pour produire en plus l'index des fichiers objets) 
FLAGS = crs	

####### Cible (Target) #######

#Nom des cibles par defaut
#On veut que notre cible (sorie), soit notre librairie en .a
TRG=$(PROJECTNAME).a

# Implementation de la cible	
#En gros, ici on dit:
#librairie.a : (tous les fichiers que je veut inclure en .o)
#				(compilateur) (options de compilation) (cible) (dependances)

$(TRG): $(OBJDEPS) 
		$(CCC) $(FLAGS) $(TRG) $(OBJDEPS)

# Verification des dependances (header dependencies)
-include *.d

#Ensemble des executables a produire.
all: $(TRG)

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

