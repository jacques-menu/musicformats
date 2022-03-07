#
#  MusicFormats Library
#  Copyright (C) Jacques Menu 2016-2022

#  This Source Code Form is subject to the terms of the Mozilla Public
#  License, v. 2.0. If a copy of the MPL was not distributed with this
#  file, You can obtain one at http://mozilla.org/MPL/2.0/.

#  https://github.com/jacques-menu/musicformats
#


 #https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html

# $@ The file name of the target of the rule.

# $% The target member name, when the target is an archive member.

# $< The name of the first prerequisite.


#	Options utiles pour Flex :
#
#		-v : pour avoir des statistiques sur l'analyseur synthetisé
#
#		-s : supprime la règle d'ECHO par défaut, utile pour repérer les
#			  spécifications incomplètes
#
#		-l : pour un comportement voisin de celui du Lex original
#
#		-L : pour que les "#line" se réfèrent au fichier produit, et non au fichier source
#			  (equivalent à '%option yylineno' dans le fichier source)
#
#		-f : pour avoir un analyseur plus rapide, mais avec des tables plus grosses
#			  (incompatible avec '-L')
#
#		NOTE : Flex 2.5.x veut que "yywrap" soit 'extern "C"', c'est ainsi!


# variables

MAKE_FILE = Makefile

APPL = mfsl

GENERATED_FILES  = mfslParser.h mfslScanner.cpp mfslParser.cpp

OBJECT_FILES  = mfslScanner.o mfslParser.o

BISON = bison # -Wcounterexamples -Wother
FLEX  = flex

CXXFLAGS = -I.. -DMAIN

PREFIX := mfsl


# implicit target

all : $(MAKE_FILE) $(GENERATED_FILES) # $(OBJECT_FILES)


# generation rules

mfslScanner.cpp : mfsl.l mfslParser.h
#	$(FLEX) -omfslScanner.cpp -I -P$(PREFIX) mfsl.l
	$(FLEX) -omfslScanner.cpp -I mfsl.l


mfslParser.h mfslParser.cpp : mfsl.y
	#$(BISON) -o mfslParser.cpp --header=mfslParser.h -p $(PREFIX) mfsl.y
	$(BISON) -o mfslParser.cpp --header=mfslParser.h mfsl.y


# compiling rules

mfslScanner.o : mfslScanner.cpp
	g++ -o $@ $<

#mfsl : $(OBJECT_FILES)
#	g++ $(DEBUG) $(OBJECT_FILES) -lstdc++ -o $(APPL)


# clean

clean:
	rm -f $(APPL) $(GENERATED_FILES) $(OBJECT_FILES)
