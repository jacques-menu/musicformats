#
# Makefile for testing the libmusicxml library
#

# variables
# ------------------------------------------------------------------------

MAKE                 ?= make

MUSICFORMATS_VERSION ?= $(shell cat MusicFormatsVersionToBeValidated.txt)

VALIDATION_DIR_NAME  ?= musicformats_validation_$(MUSICFORMATS_VERSION)

# MusicXML
MUSICXML_FILE_NAMES = $(shell find ../musicxmlfiles -name "*.xml")

validxml = $(patsubst %.xml, %.outxml, $(readout))
validgmn = $(patsubst %.gmn, %.outgmn, $(gmnout))


# libmusicxml2 tools
XMLREAD   ?= xmlread
XML2GUIDO ?= xml2guido

readout  := $(MUSICXML_FILE_NAMES:../%.xml=$(VALIDATION_DIR_NAME)/read/%.xml)
gmnout   := $(MUSICXML_FILE_NAMES:../%.xml=$(VALIDATION_DIR_NAME)/gmn/%.gmn)
guidosvg := $(MUSICXML_FILE_NAMES:../%.xml=$(VALIDATION_DIR_NAME)/guidosvg/%.svg)

# LilyPond
XML2LY ?= xml2ly

MUSICXML_FILE_NAMES_TO_BE_USED = $(shell find ../musicxmlfiles -name "*.xml" | grep -v -f toBeIgnoredForLilyPond.txt)

LILYPOND_FILE_NAMES := $(MUSICXML_FILE_NAMES_TO_BE_USED:../%.xml=$(VALIDATION_DIR_NAME)/lilypond/%.ly)

LILYPOND_PDF_FILE_NAMES  := $(patsubst ../%.xml, $(VALIDATION_DIR_NAME)/lilypdf/%.pdf, $(MUSICXML_FILE_NAMES))
LILYPOND_PNG_FILE_NAMES  := $(patsubst ../%.xml, $(VALIDATION_DIR_NAME)/lilypng/%.png, $(MUSICXML_FILE_NAMES))
LILYPOND_MIDI_FILE_NAMES := $(patsubst ../%.xml, $(VALIDATION_DIR_NAME)/lilypdf/%.midi, $(MUSICXML_FILE_NAMES))

validlily = $(patsubst %.ly, %.outly, $(LILYPOND_FILE_NAMES))

# LilyPond Jianpu
MUSICXML_JIANPU_FILE_NAMES = $(shell find ../musicxmlfiles -name "*.xml" | grep jianpu)

JIANPU_INCLUDE_FILE_NAME = "jianpu10a.ly"
JIANPU_INCLUDE_FILE      = $(shell find ../musicxmlfiles -name "$(JIANPU_INCLUDE_FILE_NAME)")

lilyjianpuout   := $(patsubst ../%.xml, $(VALIDATION_DIR_NAME)/lilyjianpu/%.ly, $(MUSICXML_JIANPU_FILE_NAMES))
lilyjianpupdf   := $(patsubst ../%.xml, $(VALIDATION_DIR_NAME)/lilyjianpupdf/%.pdf, $(MUSICXML_JIANPU_FILE_NAMES))
lilyjianpumidi  := $(patsubst ../%.xml, $(VALIDATION_DIR_NAME)/lilyjianpupdf/%.midi, $(MUSICXML_JIANPU_FILE_NAMES))

validlilyjianpu = $(patsubst %.ly, %.outlilyjianpu, $(lilyjianpuout))

# Braille
XML2BRL ?= xml2brl

BRAILLE_FILE_NAMES_TO_BE_USED = $(shell find ../musicxmlfiles -name "*.xml" | grep -v -f toBeIgnoredForBraille.txt)

xmlfilesforbrl = $(shell find ../musicxmlfiles -name "*.xml" | grep braille | grep -v toBeIgnoredForBraille.txt)
brlout         := $(patsubst ../%.xml, $(VALIDATION_DIR_NAME)/braille/%.brl, $(xmlfilesforbrl))

validbrl       = $(patsubst %.brl, %.outbrl, $(brlout))


# targets
# ------------------------------------------------------------------------

.PHONY: read guido lilyignored lily lilyjianpu brl
.DELETE_ON_ERROR:

all:
	@echo "MUSICFORMATS_VERSION: ${MUSICFORMATS_VERSION}"
	@echo "VALIDATION_DIR_NAME: ${VALIDATION_DIR_NAME}"
	$(MAKE) lily
	@ls -sal ${VALIDATION_DIR_NAME}
# 	$(MAKE) guido
#	$(MAKE) braille
# 	$(MAKE) read
	#echo "LILYPOND_FILE_NAMES: ${LILYPOND_FILE_NAMES}"
	#@echo "MUSICXML_FILE_NAMES_TO_BE_USED: ${MUSICXML_FILE_NAMES_TO_BE_USED}"
# 	echo "MUSICXML_FILE_NAMES_TO_BE_USED: ${MUSICXML_FILE_NAMES_TO_BE_USED}"

clean:
	$(MAKE) lilyignoredclean
	$(MAKE) lilyclean
	$(MAKE) lilypdfclean
	$(MAKE) lilypngclean
	$(MAKE) lilyjianpuclean
	$(MAKE) lilyjianpupdfclean
#	$(MAKE) brlclean

travis:
	$(MAKE) read
	$(MAKE) guido
	$(MAKE) lily

trace:
	@echo $(readout)


# help
# ------------------------------------------------------------------------

help:
	@echo "Makefile for testing the libmusicxml library output."
	@echo "Testing is based on the xml files located in the 'files' folder."
	@echo "Available targets are:"
	@echo "  'all' (default)  : call the 'read', 'guido' and 'lily' targets."
	@echo "  'read'           : reads the set of xml files and writes the corresponding output"
	@echo "  'guido'          : converts the set of xml files to guido"
	@echo "  'guidosvg'       : converts the output of 'guido' target to svg"
	@echo "  'lilyignored'    : converts a subset of xml files to lilypond, but fails as expected"
	@echo "  'lily'           : converts the set of xml files to lilypond"
	@echo "  'lilypng'        : converts the output of 'lily' target to png"
	@echo "  'lilypdf'        : converts the output of 'lily' target to pdf"
	@echo "  'lilyjianpu'     : converts the a subset of xml files to lilypond code using $(JIANPU_INCLUDE_FILE)"
	@echo "  'lilyjianpupdf ' : converts the output of 'lilyjianpu' target to pdf"
	@echo "  'lilyjianpupng'  : converts the output of 'lilyjianpu' target to png"
	@echo "  'braille'        : converts a subset of xml files to BRF (Braille Ready Format) music"
	@echo "Output files are written to the ${VALIDATION_DIR_NAME} folder, "
	@echo "where $(MUSICFORMATS_VERSION) is taken from the MusicFormatsVersionToBeValidated.txt file that you can freely change."
	@echo
	@echo "Validation target:"
	@echo "  'validate OTHER_MUSICFORMATS_VERSION=another_version': compares the current MUSICFORMATS_VERSION output (xml and gmn files) with the one in 'another_version' folder"
#	@echo "  (on Windows, Firefox has to be installed (and opened) in C:/Program Files (x86)/Mozilla Firefox/firefox, otherwise please change the Makefile)"
	@echo
	@echo "Options:"
	@echo "  'XML2GUIDO=/path/to/xml2guido"
	@echo "  'XML2LY=/path/to/xml2ly"
	@echo "  'XML2BRL=/path/to/xml2brl"
	@echo "  'XMLREAD=/path/to/xmlread"
	@echo "when the above variables are not specified, the tools are taken from the current PATH env."


# libmusicml2 tools
# ------------------------------------------------------------------------

read:
	@which $(XMLREAD) > /dev/null || (echo "### xmlread (part of samples) must be available from your PATH."; false;)
	$(MAKE) doread

doread: $(readout)

guido:
	@which $(XML2GUIDO) > /dev/null || (echo "### xml2guido (part of samples) must be available from your PATH."; false;)
	$(MAKE) doguido

doguido: $(gmnout)

guidosvg:
	@which guido2svg || (echo "### guido2svg (part of guidolib project) must be available from your PATH."; false;)
	$(MAKE) guido
	$(MAKE) doguidosvg

doguidosvg: $(guidosvg)


# MusicFormats tools
# ------------------------------------------------------------------------

lily:
	@which $(XML2LY) > /dev/null || (echo "### xml2ly (part of samples) must be available from your PATH."; false;)
	$(MAKE) dolily

dolily: $(LILYPOND_FILE_NAMES)

lilyclean:
	@echo "LILYPOND_FILE_NAMES = " $(LILYPOND_FILE_NAMES) ", " $(shell wc < echo $(LILYPOND_FILE_NAMES))
	rm $(LILYPOND_FILE_NAMES)

lilypdf:
	@which lilypond || (echo "### lilypond must be available from your PATH."; false;)
	$(MAKE) lily
	$(MAKE) dolilypdf

dolilypdf: $(LILYPOND_PDF_FILE_NAMES)

lilypdfclean:
	@echo "LILYPOND_PDF_FILE_NAMES = " $(LILYPOND_PDF_FILE_NAMES) ", " $(shell wc < echo $(LILYPOND_PDF_FILE_NAMES))
	@echo "LILYPOND_MIDI_FILE_NAMES = " $(LILYPOND_MIDI_FILE_NAMES) ", " $(shell wc < echo $(LILYPOND_MIDI_FILE_NAMES))
	rm $(LILYPOND_PDF_FILE_NAMES)
	rm $(LILYPOND_MIDI_FILE_NAMES)

lilypng:
	@which lilypond || (echo "### lilypond must be available from your PATH."; false;)
	$(MAKE) lily
	$(MAKE) dolilypng

dolilypng: $(LILYPOND_PNG_FILE_NAMES)

lilypngclean:
	@echo "LILYPOND_PNG_FILE_NAMES = " $(LILYPOND_PNG_FILE_NAMES) ", " $(shell wc < echo $(LILYPOND_PNG_FILE_NAMES))
	rm $(LILYPOND_PNG_FILE_NAMES)
	rm $(LILYPOND_MIDI_FILE_NAMES)


# MusicFormats with Jianpu
# ------------------------------------------------------------------------

lilyjianpu:
	@which $(XML2LY) > /dev/null || (echo "### xml2ly (part of samples) must be available from your PATH."; false;)
	$(MAKE) dolilyjianpu
	@ls -sal $(lilyjianpuout)

dolilyjianpu: $(lilyjianpuout)

lilyjianpuclean:
	@echo "lilyjianpupdf = " $(lilyjianpupdf) ", " $(shell wc < echo $(lilyjianpupdf))
	@echo "lilyjianpumidi = " $(lilyjianpumidi) ", " $(shell wc < echo $(lilyjianpumidi))
	rm $(lilyjianpuout)

lilyjianpupdf:
	$(MAKE) lilyjianpu
	@echo "JIANPU_INCLUDE_FILE: " $(JIANPU_INCLUDE_FILE)
	@test "z${JIANPU_INCLUDE_FILE}" != "z" || (echo "### $(JIANPU_INCLUDE_FILE_NAME) not found in the validation directory."; false;)
	@echo "lilyjianpupdf = " $(lilyjianpupdf) ", " $(shell wc < echo $(lilyjianpupdf))
	@which lilypond || (echo "### lilypond must be available from your PATH."; false;)
	$(MAKE) dolilyjianpupdf

dolilyjianpupdf: $(lilyjianpupdf)

lilyjianpupdfclean:
	@echo "lilyjianpupdf = " $(lilyjianpupdf) ", " $(shell wc < echo $(lilyjianpupdf))
	@echo "lilyjianpumidi = " $(lilyjianpumidi) ", " $(shell wc < echo $(lilyjianpumidi))
	rm $(lilyjianpupdf)
	rm $(lilyjianpumidi)


# Braille
# ------------------------------------------------------------------------

braille:
	@which $(XML2LY) > /dev/null || (echo "### xml2brl (part of samples) must be available from your PATH."; false;)
	$(MAKE) dobrl

dobrl: $(brlout)

brlclean:
	@echo "brlout = " $(brlout) ", " $(shell wc < echo $(brlout))
	rm $(brlout)


# validation
# ------------------------------------------------------------------------

validate:
	@echo Validating MusicFormats version $(MUSICFORMATS_VERSION) with $(OTHER_MUSICFORMATS_VERSION) $(tmp)
	$(MAKE) validxml
	$(MAKE) validgmn
	$(MAKE) validlily
#	$(MAKE) validlilyjianpu
#	$(MAKE) validbrl

validxml: $(validxml)

validgmn: $(validgmn)

validlily: $(validlily)

validlilyjianpu: $(validlilyjianpu)

validbrl: $(validbrl)


# Rules for libmusicml2 tools
# ------------------------------------------------------------------------

# rules for xmlread
${VALIDATION_DIR_NAME}/read/%.xml: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(XMLREAD) -o $@ $<

# rules for xml2guido
${VALIDATION_DIR_NAME}/gmn/%.gmn: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(XML2GUIDO) -o $@ $<

# rules for guido2svg
${VALIDATION_DIR_NAME}/guidosvg/%.svg: ${VALIDATION_DIR_NAME}/gmn/%.gmn
	@[ -d $(@D) ] || mkdir -p $(@D)
	guido2svg  -o $@ $<


# Rules for LilyPond output
# ------------------------------------------------------------------------

# rules for xml2ly
${VALIDATION_DIR_NAME}/lilypond/%.ly: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(XML2LY) -q -o $@ $<

${VALIDATION_DIR_NAME}/lilyignored/%.ly: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(XML2LY) -q -o $@ $<  || true

# rules for lilypond with pdf output
${VALIDATION_DIR_NAME}/lilypdf/%.pdf: ${VALIDATION_DIR_NAME}/lilypond/%.ly
	@[ -d $(@D) ] || mkdir -p $(@D)
	lilypond --pdf -l NONE -s -o $(basename $@) $<

# rules for lilypond with png output
${VALIDATION_DIR_NAME}/lilypng/%.png: ${VALIDATION_DIR_NAME}/lilypond/%.ly
	@[ -d $(@D) ] || mkdir -p $(@D)
	lilypond --png -l NONE -s -o $(basename $@) $<

# rules for lilypond with jianpu inclusion in output
${VALIDATION_DIR_NAME}/lilyjianpu/%.ly: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(XML2LY) -q -jianpu -o $@ $<

# rules for lilypond pdf output in jianpu notation
${VALIDATION_DIR_NAME}/lilyjianpupdf/%.pdf: ${VALIDATION_DIR_NAME}/lilyjianpu/%.ly
	@[ -d $(@D) ] || mkdir -p $(@D)
	cp -p $(JIANPU_INCLUDE_FILE) $(@D)
	lilypond --pdf -l NONE -s -o $(basename $@) $<


# Rules for Braille output
# ------------------------------------------------------------------------

# rules for xml2brl
${VALIDATION_DIR_NAME}/braille/%.brl: ../%.xml
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(XML2BRL) -q  -o $@ $<


# Rules for MusicXML output validation
# ------------------------------------------------------------------------

# rules for xml output validation
%.outxml: %.xml
	$(eval tmp := $(patsubst $(VALIDATION_DIR_NAME)/%, $(OTHER_MUSICFORMATS_VERSION)/%, $<))
	@[ -f  $(tmp) ] || echo $< : new file
	@diff $<  $(tmp) 2>/dev/null >/dev/null || ([ -f  $(tmp) ] && echo "open -t $< $(patsubst $(VALIDATION_DIR_NAME)/%, $(OTHER_MUSICFORMATS_VERSION)/%, $<) # to check changes"; true)

%.outgmn: %.gmn
	$(eval tmp := $(patsubst $(VALIDATION_DIR_NAME)/%, $(OTHER_MUSICFORMATS_VERSION)/%, $<))
	@[ -f  $(tmp) ] || echo $< : new file
	@diff $<  $(tmp) 2>/dev/null >/dev/null || ([ -f  $(tmp) ] && echo "open -t $< $(patsubst $(VALIDATION_DIR_NAME)/%, $(OTHER_MUSICFORMATS_VERSION)/%, $<) # to check changes"; true)

%.outly: %.ly
	$(eval tmp := $(patsubst $(VALIDATION_DIR_NAME)/%, $(OTHER_MUSICFORMATS_VERSION)/%, $<))
	@[ -f  $(tmp) ] || echo $< : new file
	@diff $<  $(tmp) 2>/dev/null >/dev/null || ([ -f  $(tmp) ] && echo "open -t $< $(patsubst $(VALIDATION_DIR_NAME)/%, $(OTHER_MUSICFORMATS_VERSION)/%, $<) # to check changes"; true)

%.outlilyjianpu: %.ly
	$(eval tmp := $(patsubst $(VALIDATION_DIR_NAME)/%, $(OTHER_MUSICFORMATS_VERSION)/%, $<))
	@[ -f  $(tmp) ] || echo $< : new file
	@diff $<  $(tmp) 2>/dev/null >/dev/null || ([ -f  $(tmp) ] && echo "open -t $< $(patsubst $(VALIDATION_DIR_NAME)/%, $(OTHER_MUSICFORMATS_VERSION)/%, $<) # to check changes"; true)

%.outbrl: %.brl
	$(eval tmp := $(patsubst $(VALIDATION_DIR_NAME)/%, $(OTHER_MUSICFORMATS_VERSION)/%, $<))
	@[ -f  $(tmp) ] || echo $< : new file
	@diff $<  $(tmp) 2>/dev/null >/dev/null || ([ -f  $(tmp) ] && echo "open -t $< $(patsubst $(VALIDATION_DIR_NAME)/%, $(OTHER_MUSICFORMATS_VERSION)/%, $<) # to check changes"; true)
