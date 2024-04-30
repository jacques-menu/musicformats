#
#  MusicFormats Library
#  Copyright (C) Jacques Menu 2016-2024

#  This Source Code Form is subject to the terms of the Mozilla Public
#  License, v. 2.0. If a copy of the MPL was not distributed with this
#  file, You can obtain one at http://mozilla.org/MPL/2.0/.

#  https://github.com/jacques-menu/musicformats
#

#
# https://www.gnu.org/software/make/manual/html_node/Automatic-Choices.html
#   $@ The file name of the target of the rule.
#   $% The target member name, when the target is an archive member.
#   $< The name of the first prerequisite.
#

# variables
# ---------------------------------------------------------------------------

MAKEFILE = Makefile

GENERATED_FILES  = mfslParser.h mfslScanner.cpp mfslParser.cpp

BISON = bison
FLEX  = flex

CXXFLAGS = -I.. -DMAIN


# implicit target
# ---------------------------------------------------------------------------

mfslInterpreterCodeByFlexAndBison : $(GENERATED_FILES)


# generation rules
# ---------------------------------------------------------------------------

mfslScanner.cpp : $(MAKEFILE) mfslScanner.ll
	$(FLEX) -omfslScanner.cpp mfslScanner.ll


mfslParser.h mfslParser.cpp : $(MAKEFILE) mfslParser.yy
	$(BISON) --defines=mfslParser.h -o mfslParser.cpp mfslParser.yy


# clean
# ---------------------------------------------------------------------------

clean:
	rm -f $(GENERATED_FILES) mfslParser.output location.hh mfslScanner.log
