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

GENERATED_FILES = mfFindParser.h mfFindScanner.cpp mfFindParser.cpp

BISON = bison
FLEX  = flex

CXXFLAGS = -I.. -DMAIN


# implicit target
# ---------------------------------------------------------------------------

mfFindInterpreterCodeByFlexAndBison : $(GENERATED_FILES)


# generation rules
# ---------------------------------------------------------------------------

mfFindScanner.cpp : $(MAKEFILE) mfFindScanner.ll
	$(FLEX) -omfFindScanner.cpp mfFindScanner.ll


mfFindParser.h mfFindParser.cpp : $(MAKEFILE) mfFindParser.yy
	$(BISON) --defines=mfFindParser.h -o mfFindParser.cpp mfFindParser.yy


# clean
# ---------------------------------------------------------------------------

clean:
	rm -f $(GENERATED_FILES) mfFindParser.output location.hh mfFindScanner.log
