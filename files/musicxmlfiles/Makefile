#
# Makefile for testing the musicformats library' xml2ly (or musicxml2ly, by the way)
#

xmlfiles          = $(shell find . -name "*.xml")
xmllily           = $(shell find . -name "*.xml")

lilyout           := $(xmllily:./%.xml=lilypond/%.ly)

pdflily           := $(patsubst ./%.xml, pdflily/%.pdf, $(xmlfiles))
pnglily           := $(patsubst ./%.xml, pnglily/%.png, $(xmlfiles))
midilily          := $(patsubst ./%.xml, pdflily/%.midi, $(xmlfiles))

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

#LILYPOND  ?= /opt/local/bin/lilypond
  # 2.23.5 (running Guile 1.8)

LILYPOND  ?= /Applications/LilyPond/lilypond-2.23.6/bin/lilypond
  # 2.23.6 (running Guile 2.2)

.PHONY: lilyignored lily
.DELETE_ON_ERROR:

all:
	@echo "XML2LY   = $(XML2LY)"
	@echo "OPTIONS  = $(OPTIONS)"
	@echo "LILYPOND = $(LILYPOND)"
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

	@$(MAKE) pdflily

clean:
	@[ ! -d lilypond ] || rm -r lilypond
	@[ ! -d pdflily ]  || rm -r pdflily
	@[ ! -d pnglilyv ] || rm -r pnglily
	@[ ! -d midilily ] || rm -r midilily

help:
	@echo "Makefile for converting MusicXML files to LilyPond scores."
	@echo "Supplied as part of the MusicFormats library, thanks to Dom Fober for providing it."
	@echo "Output files are written in sub-folders of the current folder."
	@echo
	@echo "Available targets are:"
	@echo "  'all' (default) : call the 'lily' target."
	@echo
	@echo "  'lily'          : converts the set of MusicXML files to lilypond in folder "
	@#echo "  'lilyignored'   : converts a subset of MusicXML files to lilypond, but fails as expected"
	@echo
	@echo "  'pnglily'       : converts the output of 'lily' target to png in folder pnglily"
	@echo "  'pdflily'       : converts the output of 'lily' target to pdf in folder pdflily"
	@echo "  'midilily'      : converts the output of 'lily' target to pdf in folder midilily"
	@echo
	@echo "  'clean'         : removes the sub-folders containing the results"
	@echo
	@echo "Options:"
	@echo "  'XML2LY=/path/to/xml2ly"
	@echo "  'OPTIONS=..."
	@echo "  'LILYPOND=/path/to/lilypond"
	@echo "  When those options are not specified, the tools are taken from the current PATH variable,"
	@echo "  and OPTIONS contains '-q' (quiet mode) for xml2ly."

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

	@$(MAKE) lily
	@$(MAKE) dopdflily

	@echo

	@echo "Contents of lilypond sub-folder:"
	@[[ -d lilypond ]] &&  ls -sal lilypond/*.ly
	@echo

	@echo "Contents of pdflily sub-folder:"
	@[[ -d pdflily ]] &&  ls -sal pdflily/*.pdf
dopdflily: $(pdflily)

#########################################################################
pdflilyclean:
	@echo "pdflily = " $(pdflily) ", " $(shell wc < echo $(pdflily))
	@echo "midilily = " $(midilily) ", " $(shell wc < echo $(midilily))
	rm $(pdflily)
	rm $(midilily)

#########################################################################
pnglily:
	@type $(LILYPOND) || (echo "### lilypond must be available from your PATH."; false;)
	@$(MAKE) lily
	@$(MAKE) dopnglily

dopnglily: $(pnglily)

#########################################################################
pnglilyclean:
	@echo "pnglily = " $(pnglily) ", " $(shell wc < echo $(pnglily))
	rm $(pnglily)
	rm $(midilily)

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
# rules for lilypond with png output
pnglily/%.png: lilypond/%.ly
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(LILYPOND) --png -l NONE -s -o $(basename $@) $<

#########################################################################
# rules for xml output validation
%.outly: %.ly
	$(eval tmp := $(patsubst %, %, $<))
	@[ -f  $(tmp) ] || echo $< : new file
	@diff $<  $(tmp) 2>/dev/null >/dev/null || ([ -f  $(tmp) ] && echo "open -t $< $(patsubst %, %, $<) # to check changes"; true)

