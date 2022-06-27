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

GENERATED_FILES  = ischemeParser.h ischemeScanner.cpp ischemeParser.cpp

BISON = bison
FLEX  = flex

CXXFLAGS = -I.. -DMAIN


# implicit target
# ---------------------------------------------------------------------------

ischemeInterpreterCodeByFlexAndBison : $(GENERATED_FILES)


# generation rules
# ---------------------------------------------------------------------------

ischemeScanner.cpp : $(MAKEFILE) ischemeScanner.ll
	$(FLEX) -oischemeScanner.cpp ischemeScanner.ll


ischemeParser.h ischemeParser.cpp : $(MAKEFILE) ischemeParser.yy
	$(BISON) --defines=ischemeParser.h -o ischemeParser.cpp ischemeParser.yy


# clean
# ---------------------------------------------------------------------------

clean:
	rm -f $(GENERATED_FILES) ischemeParser.output location.hh ischemeScanner.log
