#
# Makefile for testing the musicformats library' xml2ly (or musicxml2ly, by the way)
#

# SHELL := bash

xmlfiles          = $(shell find . -name "*.xml")
xmllily           = $(shell find . -name "*.xml")

lilyout           := $(xmllily:./%.xml=%.ly)

pdffiles          := $(patsubst ./%.xml, %.pdf, $(xmlfiles))
svglily           := $(patsubst ./%.xml, svglily/%.svg, $(xmlfiles))
pnglily           := $(patsubst ./%.xml, pnglily/%.png, $(xmlfiles))
pslily            := $(patsubst ./%.xml, pslily/%.ps, $(xmlfiles))
epslily           := $(patsubst ./%.xml, epslily/%.eps, $(xmlfiles))

validlily 	      = $(patsubst %.ly, %.outly, $(lilyout))

# In makefiles, $@ is the name of the target being generated,
# and $< the first prerequisite (usually a source file).
# You can find a list of all these special variables in the GNU Make manual

MAKE      ?= make

XML2LY    ?= xml2ly
#XML2LY    ?= time xml2ly
#XML2LY    ?= musicxml2ly
#XML2LY    ?= time musicxml2ly

XML2LY_OPTIONS   ?= -q -lilypond-run-date -lilypond-generation-infos

LILYPOND  ?= /opt/homebrew/bin/lilypond
# LILYPOND  ?= /opt/local/bin/lilypond
# LILYPOND  ?= /Applications/LilyPond/lilypond-2.23.6/bin/lilypond

.PHONY: lilyignored lilypondfiles
.DELETE_ON_ERROR:

#########################################################################
all:
	@echo "XML2LY         = $(XML2LY)"
	@echo "XML2LY_OPTIONS = $(XML2LY_OPTIONS)"
	@echo "LILYPOND       = $(LILYPOND)"
	@$(LILYPOND) --version
	@echo

	@#echo "xmlfiles:"
	@#echo "$(xmlfiles)"
	@#echo

	@#echo "xmllily:"
	@#echo "$(xmllily)"
	@#echo

	@#echo "lilyout:"
	@#echo "$(lilyout)"
	@#echo

	@#echo "pdffiles:"
	@#echo "$(pdffiles)"
	@#echo

	@#echo "svglily:"
	@#echo "$(svglily)"
	@#echo

	@#echo "pnglily:"
	@#echo "$(pnglily)"
	@#echo

	@#echo "pslily:"
	@#echo "$(pslily)"
	@#echo

	@#echo "epslily:"
	@#echo "$(epslily)"
	@#echo

	@$(MAKE) pdffiles

#########################################################################
help:
	@echo "Makefile for converting MusicXML files to LilyPond scores."
	@echo "Supplied as part of the MusicFormats library, thanks to Dom Fober for providing it."
	@echo "Output files are written in sub-folders of the current folder."
	@echo
	@echo "Available targets are:"
	@echo "  'all' (default) : call the 'lilypondfiles' target."
	@echo
	@echo "  'lilypondfiles' : converts the set of MusicXML files to LilyPond in folder lilypond"
	@echo
	@echo "  'pdffiles'      : converts the output of 'lilypondfiles' target to pdf"
	@echo "  'svglily'       : converts the output of 'lilypondfiles' target to svg"
	@echo "  'pnglily'       : converts the output of 'lilypondfiles' target to pngy"
	@echo "  'pslily'        : converts the output of 'lilypondfiles' target to ps"
	@echo "  'epslily'       : converts the output of 'lilypondfiles' target to eps"
	@echo
	@echo "  'check'         : displays which files failed to be translated"
	@echo
	@echo "  'clean'         : removes the xml2ly-generated files in this folder and its sub-folders"
	@echo
	@echo "Make variables:"
	@echo "  XML2LY=/path/to/xml2ly"
	@echo "  LILYPOND=/path/to/lilypond"
	@echo "  When XML2LY and/or LILYPOND are not set, "
	@echo "  these tools are looked up in the PATH variable,"
	@echo
	@echo "  XML2LY_OPTIONS=..."
	@echo "  By default, XML2LY_OPTIONS contains '-q' (quiet mode), for use by xml2ly."

#########################################################################
clean:
	@[ ! -d lilypond ] || rm -r lilypond
	@[ ! -d pdffiles ]  || rm -r pdffiles
	@[ ! -d pnglilyv ] || rm -r pnglily
	rm *.ly *.pdf *.midi

#########################################################################
# https://www.gnu.org/software/make/manual/html_node/Foreach-Function.html

MUSICXML_FILES        := @ls *.xml
MUSICXML_FILES_NUMBER := @(ls -sal *.xml | wc -l)

check:
	@printf "%s %s " "There are"
	@(ls -sal *.xml | wc -l)
	@printf " MusicXML files"
	@(ls -sal *.xml)
	@echo
	@printf "There are "
	@(ls -sal *.ly | wc -l)
	@printf " LilyPond files"
	@(ls -sal *.ly)
	@echo
	@printf "There are "
	@(ls -sal *.pdf | wc -l)
	@printf " PDF files"
	@(ls -sal *.pdf)
	@echo
	@echo "Files not translated:"
	$(foreach FILE,$(MUSICXML_FILES),@echo $(FILE))

#########################################################################
lilypondfiles:
	@type $(XML2LY) > /dev/null || (echo "### $(XML2LY) must be available from your PATH."; false;)
	@$(MAKE) dolily

dolily: $(lilyout)

#########################################################################
lilyclean:
	@echo "lilyout = " $(lilyout) ", " $(shell wc < echo $(lilyout))
	rm $(lilyout)

#########################################################################
pdffiles:
	@#type $(XML2LY) || (echo "### $(XML2LY) must be available from your PATH."; false;)
	@echo "$(XML2LY) version is: "
	@$(XML2LY) --version | head -1
	@echo

	@#type $(LILYPOND) || (echo "### lilypond must be available from your PATH."; false;)
	@echo "lilypond version is: "
	@$(LILYPOND) --version | head -1
	@echo

	@#echo "xmlfiles:"
	@#echo "$(xmlfiles)"
	@#echo

	@#echo "xmllily:"
	@#echo "$(xmllily)"
	@#echo

	@#echo "lilyout:"
	@#echo "$(lilyout)"
	@#echo

	@#echo "pdffiles:"
	@#echo "$(pdffiles)"
	@#echo

	@#echo "svglily:"
	@#echo "$(svglily)"
	@#echo

	@#echo "pnglily:"
	@#echo "$(pnglily)"
	@#echo

	@#echo "pslily:"
	@#echo "$(pslily)"
	@#echo

	@#echo "epslily:"
	@#echo "$(epslily)"
	@#echo

	@$(MAKE) lilypondfiles
	@$(MAKE) dopdffiles

	@echo

	@echo "Contents of ${PWD} folder:"
	@ls -sal *.xml | wc -l ; echo " files"
	@echo "--------------------------------------------"
	@ls -sal *.xml
	@echo

	@echo "LilyPond files:"
	@ls -sal *.ly | wc -l ; echo " files"
	@echo "--------------------------------------------"
	@ls -sal *.ly
	@echo

	@echo "PDF files:"
	@ls -sal *.pdf | wc -l ; echo " files"
	@echo "--------------------------------------------"
	@ls -sal *.pdf

dopdffiles: $(pdffiles)

pdffilesclean:
	@echo "pdffiles = " $(pdffiles) ", " $(shell wc < echo $(pdffiles))
	rm $(pdffiles)

#########################################################################
svglily:
	@type $(LILYPOND) || (echo "### lilypond must be available from your PATH."; false;)
	@$(MAKE) lilypondfiles
	@$(MAKE) dosvglily

dosvglily: $(svglily)

svglilyclean:
	@echo "svglily = " $(svglily) ", " $(shell wc < echo $(svglily))
	rm $(svglily)

#########################################################################
pnglily:
	@type $(LILYPOND) || (echo "### lilypond must be available from your PATH."; false;)
	@$(MAKE) lilypondfiles
	@$(MAKE) dopnglily

dopnglily: $(pnglily)

pnglilyclean:
	@echo "pnglily = " $(pnglily) ", " $(shell wc < echo $(pnglily))
	rm $(pnglily)

#########################################################################
pslily:
	@type $(LILYPOND) || (echo "### lilypond must be available from your PATH."; false;)
	@$(MAKE) lilypondfiles
	@$(MAKE) dopslily

dopslily: $(pslily)

pslilyclean:
	@echo "pslily = " $(pslily) ", " $(shell wc < echo $(pslily))
	rm $(pslily)

#########################################################################
epslily:
	@type $(LILYPOND) || (echo "### lilypond must be available from your PATH."; false;)
	@$(MAKE) lilypondfiles
	@$(MAKE) doepslily

doepslily: $(epslily)

epslilyclean:
	@echo "epslily = " $(epslily) ", " $(shell wc < echo $(epslily))
	rm $(epslily)

#########################################################################
validate:
	@echo Validating version $(version) with $(VERSION) $(tmp)
	@$(MAKE) validlily

validlily: $(validlily)

#########################################################################
# rules for XML2LY
%.ly: ./%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(XML2LY) $(XML2LY_OPTIONS) -o "$@" "$<"

#lilyignored/%.ly: ./%.xml
#	@[ -d $(@D) ] || mkdir -p $(@D)
#	$(XML2LY) -q -o $@ $< || true

#########################################################################
# rules for lilypond with pdf output
%.pdf: %.ly
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(LILYPOND) --pdf -l NONE -s -o "$(basename $@)" "$<"

#########################################################################
# rules for lilypond with svg output
svglily/%.svg: %.ly
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(LILYPOND) --svg -l NONE -s -o $(basename $@) $<

#########################################################################
# rules for lilypond with png output
pnglily/%.png: %.ly
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(LILYPOND) --png -l NONE -s -o $(basename $@) $<

#########################################################################
# rules for lilypond with ps output
pslily/%.ps: %.ly
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(LILYPOND) --ps -l NONE -s -o $(basename $@) $<

#########################################################################
# rules for lilypond with eps output
epslily/%.eps: %.ly
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(LILYPOND) --eps -l NONE -s -o $(basename $@) $<

#########################################################################
# rules for xml output validation
%.outly: %.ly
	$(eval tmp := $(patsubst %, %, $<))
	@[ -f $(tmp) ] || echo $< : new file
	@diff $< $(tmp) 2>/dev/null >/dev/null || ([ -f $(tmp) ] && echo "open -t $< $(patsubst %, %, $<) # to check changes"; true)

