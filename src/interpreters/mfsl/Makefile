#
#  MusicFormats Library
#  Copyright (C) Jacques Menu 2016-2022

#  This Source Code Form is subject to the terms of the Mozilla Public
#  License, v. 2.0. If a copy of the MPL was not distributed with this
#  file, You can obtain one at http://mozilla.org/MPL/2.0/.

#  https://github.com/jacques-menu/musicformats
#

#
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
#   $@ The file name of the target of the rule.
#   $% The target member name, when the target is an archive member.
#   $< The name of the first prerequisite.
#

# variables

MAKEFILE = Makefile

GENERATED_FILES  = mfslParser.h mfslScanner.cpp mfslParser.cpp

BISON = bison # -Wcounterexamples -Wother
FLEX  = flex

CXXFLAGS = -I.. -DMAIN


# implicit target
# ---------------------------------------------------------------------------

all : $(GENERATED_FILES)


# generation rules
# ---------------------------------------------------------------------------

mfslScanner.cpp : $(MAKEFILE) mfslScanner.l
	$(FLEX) -omfslScanner.cpp mfslScanner.l


mfslParser.h mfslParser.cpp : $(MAKEFILE) mfslParser.y
	$(BISON) --defines=mfslParser.h -o mfslParser.cpp mfslParser.y


# clean
# ---------------------------------------------------------------------------

clean:
	rm -f $(GENERATED_FILES)
