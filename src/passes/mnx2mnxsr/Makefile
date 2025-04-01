#
#  MusicFormats Library
#  Copyright (C) Jacques Menu 2016-2025

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

GENERATED_FILES = mnx2mnxsrParser.h mnx2mnxsrScanner.cpp mnx2mnxsrParser.cpp

BISON = bison
FLEX  = flex

CXXFLAGS = -I.. -DMAIN


# implicit target
# ---------------------------------------------------------------------------

mnx2mnxsrInterpreterCodeByFlexAndBison : $(GENERATED_FILES)


# generation rules
# ---------------------------------------------------------------------------

mnx2mnxsrScanner.cpp : $(MAKEFILE) mnx2mnxsrScanner.ll
	$(FLEX) -omnx2mnxsrScanner.cpp mnx2mnxsrScanner.ll


mnx2mnxsrParser.h mnx2mnxsrParser.cpp : $(MAKEFILE) mnx2mnxsrParser.yy
	$(BISON) --defines=mnx2mnxsrParser.h -o mnx2mnxsrParser.cpp mnx2mnxsrParser.yy


# clean
# ---------------------------------------------------------------------------

clean:
	rm -f $(GENERATED_FILES) mnx2mnxsrParser.output location.hh mnx2mnxsrScanner.log
