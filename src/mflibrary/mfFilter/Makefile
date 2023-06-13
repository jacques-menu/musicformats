#
#  MusicFormats Library
#  Copyright (C) Jacques Menu 2016-2022

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

GENERATED_FILES = mfFilterParser.h mfFilterScanner.cpp mfFilterParser.cpp

BISON = bison
FLEX  = flex

CXXFLAGS = -I.. -DMAIN


# implicit target
# ---------------------------------------------------------------------------

mfFilterInterpreterCodeByFlexAndBison : $(GENERATED_FILES)


# generation rules
# ---------------------------------------------------------------------------

mfFilterScanner.cpp : $(MAKEFILE) mfFilterScanner.ll
	$(FLEX) -omfFilterScanner.cpp mfFilterScanner.ll


mfFilterParser.h mfFilterParser.cpp : $(MAKEFILE) mfFilterParser.yy
	$(BISON) --defines=mfFilterParser.h -o mfFilterParser.cpp mfFilterParser.yy


# clean
# ---------------------------------------------------------------------------

clean:
	rm -f $(GENERATED_FILES) mfFilterParser.output location.hh mfFilterScanner.log
