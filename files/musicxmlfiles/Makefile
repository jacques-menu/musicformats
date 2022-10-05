#
# Makefile for testing the musicformats library' xml2ly (or musicxml2ly, by the way)
#

xmlfiles          = $(shell find . -name "*.xml")
xmllily           = $(shell find . -name "*.xml")

lilyout           := $(xmllily:./%.xml=lilypond/%.ly)

pdflily           := $(patsubst ./%.xml, pdflily/%.pdf, $(xmlfiles))
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

OPTIONS   ?= -q

LILYPOND  ?= /opt/local/bin/lilypond
# LILYPOND  ?= /Applications/LilyPond/lilypond-2.23.6/bin/lilypond

.PHONY: lilyignored lily
.DELETE_ON_ERROR:

all:
	@echo "XML2LY   = $(XML2LY)"
	@echo "OPTIONS  = $(OPTIONS)"
	@echo "LILYPOND = $(LILYPOND)"
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

	@#echo "pdflily:"
	@#echo "$(pdflily)"
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

	@$(MAKE) pdflily

clean:
	@[ ! -d lilypond ] || rm -r lilypond
	@[ ! -d pdflily ]  || rm -r pdflily
	@[ ! -d pnglilyv ] || rm -r pnglily

help:
	@echo "Makefile for converting MusicXML files to LilyPond scores."
	@echo "Supplied as part of the MusicFormats library, thanks to Dom Fober for providing it."
	@echo "Output files are written in sub-folders of the current folder."
	@echo
	@echo "Available targets are:"
	@echo "  'all' (default) : call the 'lily' target."
	@echo
	@echo "  'lily'          : converts the set of MusicXML files to LilyPond in folder lilypond"
	@echo
	@echo "  'pdflily'       : converts the output of 'lily' target to pdf in folder pdflily"
	@echo "  'svglily'       : converts the output of 'lily' target to svg in folder svglily"
	@echo "  'pnglily'       : converts the output of 'lily' target to png in folder pnglily"
	@echo "  'pslily'        : converts the output of 'lily' target to ps  in folder pslily"
	@echo "  'epslily'       : converts the output of 'lily' target to eps in folder epslily"
	@echo
	@echo "  'clean'         : removes the sub-folders containing the results"
	@echo
	@echo "Make variables:"
	@echo "  XML2LY=/path/to/xml2ly"
	@echo "  LILYPOND=/path/to/lilypond"
	@echo "  When XML2LY and/or LILYPOND are not set, "
	@echo "  these tools are looked up in the PATH variable,"
	@echo
	@echo "  OPTIONS=..."
	@echo "  By default, OPTIONS contains '-q' (quiet mode), for use by xml2ly."

#########################################################################
lily:
	@type $(XML2LY) > /dev/null || (echo "### $(XML2LY) must be available from your PATH."; false;)
	@$(MAKE) dolily

dolily: $(lilyout)

#########################################################################
lilyclean:
	@echo "lilyout = " $(lilyout) ", " $(shell wc < echo $(lilyout))
	rm $(lilyout)

#########################################################################
pdflily:
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

	@#echo "pdflily:"
	@#echo "$(pdflily)"
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

	@$(MAKE) lily
	@$(MAKE) dopdflily

	@echo

	@echo "Contents of lilypond sub-folder:"
	@[[ -d lilypond ]] &&  ls -sal lilypond/*.ly
	@echo

	@echo "Contents of pdflily sub-folder:"
	@[[ -d pdflily ]] &&  ls -sal pdflily/*.pdf
dopdflily: $(pdflily)

pdflilyclean:
	@echo "pdflily = " $(pdflily) ", " $(shell wc < echo $(pdflily))
	rm $(pdflily)

#########################################################################
svglily:
	@type $(LILYPOND) || (echo "### lilypond must be available from your PATH."; false;)
	@$(MAKE) lily
	@$(MAKE) dosvglily

dosvglily: $(svglily)

svglilyclean:
	@echo "svglily = " $(svglily) ", " $(shell wc < echo $(svglily))
	rm $(svglily)

#########################################################################
pnglily:
	@type $(LILYPOND) || (echo "### lilypond must be available from your PATH."; false;)
	@$(MAKE) lily
	@$(MAKE) dopnglily

dopnglily: $(pnglily)

pnglilyclean:
	@echo "pnglily = " $(pnglily) ", " $(shell wc < echo $(pnglily))
	rm $(pnglily)

#########################################################################
pslily:
	@type $(LILYPOND) || (echo "### lilypond must be available from your PATH."; false;)
	@$(MAKE) lily
	@$(MAKE) dopslily

dopslily: $(pslily)

pslilyclean:
	@echo "pslily = " $(pslily) ", " $(shell wc < echo $(pslily))
	rm $(pslily)

#########################################################################
epslily:
	@type $(LILYPOND) || (echo "### lilypond must be available from your PATH."; false;)
	@$(MAKE) lily
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
lilypond/%.ly: ./%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(XML2LY) $(OPTIONS) -o "$@" "$<"

#lilyignored/%.ly: ./%.xml
#	@[ -d $(@D) ] || mkdir -p $(@D)
#	$(XML2LY) -q -o $@ $<  || true

#########################################################################
# rules for lilypond with pdf output
pdflily/%.pdf: lilypond/%.ly
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(LILYPOND) --pdf -l NONE -s -o "$(basename $@)" "$<"

#########################################################################
# rules for lilypond with svg output
svglily/%.svg: lilypond/%.ly
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(LILYPOND) --svg -l NONE -s -o $(basename $@) $<

#########################################################################
# rules for lilypond with png output
pnglily/%.png: lilypond/%.ly
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(LILYPOND) --png -l NONE -s -o $(basename $@) $<

#########################################################################
# rules for lilypond with ps output
pslily/%.ps: lilypond/%.ly
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(LILYPOND) --ps -l NONE -s -o $(basename $@) $<

#########################################################################
# rules for lilypond with eps output
epslily/%.eps: lilypond/%.ly
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(LILYPOND) --eps -l NONE -s -o $(basename $@) $<

#########################################################################
# rules for xml output validation
%.outly: %.ly
	$(eval tmp := $(patsubst %, %, $<))
	@[ -f  $(tmp) ] || echo $< : new file
	@diff $<  $(tmp) 2>/dev/null >/dev/null || ([ -f  $(tmp) ] && echo "open -t $< $(patsubst %, %, $<) # to check changes"; true)

