#
#
#

.PHONY: android


#===============================================================
#	variables
#===============================================================
MAKE      ?= make --debug # JMI
CMAKE     ?= cmake # --help-full

LIBDIR    ?= libdir
IOSDIR    ?= iosdir

MAKEFILE  ?= Makefile

VERSION   = 0.9.66

CMAKEOPTS ?=

TOOLS := LilyPondIssue34 Mikrokosmos3Wandering MusicAndHarmonies RandomChords RandomMusic countnotes displayMusicformatsHistory displayMusicformatsVersion msdlconverter partsummary readunrolled xml2brl xml2gmn xml2guido xml2ly xml2midi xml2xml xmlclone xmlfactory xmliter xmlread xmltranspose xmlversion


#===============================================================
#	default target
#===============================================================
all :
	$(MAKE) libmusicformats


#===============================================================
# help
#===============================================================
help :
	@echo "libmusicformats makefile - Available targets:"
	@echo "   all (default)  build the libmusicformats library for the current platform,"
	@echo "                     and the libmusicformats tools"
	@echo "   android        build a static library for Android"
	@echo "   ios            build a static library for iOS"
	@echo "   wasm           build the wasm library"
	@echo "   minimal        build the library with only guido support (no tools and no static library)"
	@echo
	@echo "Misc:"
	@echo "   cmake          re-generates the cmake project"
# 	@echo "   format         source code formatting using clang-format"
	@echo "   install        install library, tools and headers"
	@echo "   localinstall   install the tools to ~/bin"
	@echo "   package        create the libmusicformats-$(VERSION) package"
	@echo
	@echo "Options:"
	@echo "   CMAKEOPTS      cmake options passed to cmake by the 'cmake' target, currently '$(CMAKEOPTS)'"
	@echo "   GENERATOR      the cmake generator, currently '$(GENERATOR)'"
	@echo "   LIBDIR         the generation folder, currently '$(LIBDIR)'"
	@echo "   MSVC           [Windows only] the windows generator, currently '$(MSVC)'"
	@echo "   PREFIX         the install location prefix, currently '$(PREFIX)'"
	@echo
	@echo "CMake options:"
	@echo "   FMWK           [MacOS only] Generates a framework on MacOS. Default is off"
	@echo
	@echo "Clean:"
	@echo "   clean          remove the bin, lib and libdir folders"
	@echo


#===============================================================
# building the library
#===============================================================
libmusicformats: $(LIBDIR) $(LIBDIR)/$(MAKEFILE)
	@echo "====> Creating libmusicformats..."
	$(CMAKE) --build $(LIBDIR) --config Release

$(LIBDIR):
	@echo "====> Creating $(LIBDIR)..."
	mkdir $(LIBDIR)

$(LIBDIR)/$(MAKEFILE): CMakeLists.txt
	@echo "====> Creating $(LIBDIR)/$(MAKEFILE)..."
	cd $(LIBDIR) && $(CMAKE) .. $(CMAKEOPTS) $(GENERATOR)


#===============================================================
# building the javascript library
#===============================================================
wasm:
	$(MAKE) -C ../javascript

#===============================================================
minimal:
#===============================================================
	$(MAKE) cmake CMAKEOPTS="-DGENTOOLS=off -DGENSTATIC=off"
	$(CMAKE) --build $(LIBDIR) --config Release


#===============================================================
# building the library for iOS
#===============================================================
ios :
	$(MAKE) libmusicformats LIBDIR=$(IOSDIR) CMAKEOPTS="-DIOS=on"


#===============================================================
# re-generating the cmake project
#===============================================================
cmake : $(LIBDIR)
	cd $(LIBDIR) && $(CMAKE) .. $(CMAKEOPTS)


#===============================================================
# building the library for Android
#===============================================================
android :
	ndk-build -C android
	cp android/libs/armeabi-v7a/libmusicformats.so android/libmusicformats.armeabi-v7a.so
	cp android/libs/x86/libmusicformats.so android/libmusicformats.x86.so


#===============================================================
# src code formatting
#===============================================================
# format :
# 	@which clang-format >/dev/null || (echo "clang-format must be available from your path to run the 'format' target"; false)
# 	find ../src -path ../src/parser -prune -o -iname '*.cpp' -execdir clang-format -i -style=file {} \;
# 	find ../src -path ../src/parser -prune -o -iname '*.h' -execdir clang-format -i -style=file {} \;


#===============================================================
# install
#===============================================================
installLog := $(LIBDIR)/install_manifest.txt
install:
	cd $(LIBDIR) && $(CMAKE) .. -DCMAKE_INSTALL_PREFIX=$(PREFIX)
	$(CMAKE) --build $(LIBDIR) --config Release --target install

uninstall: installedfiles = $(shell cat $(installLog))
uninstall: $(installLog)
	rm -f $(installedfiles) $(installLog)

localinstall:
	cd bin && cp $(TOOLS) $(HOME)/bin


#===============================================================
# packaging
#===============================================================
PACKDIR := ../packages
PACK := $(PACKDIR)/libmusicformats-$(VERSION)
package:
	cmake --build $(LIBDIR) --config Release --target package

winpack:
	$(MAKE) win64
	cd win64 && cpack -G NSIS64
	mv win64/libmusicformats-*.exe .

macpack: $(PACKDIR)/README.html
	-[ -d $(PACK) ] && rm -rf $(PACK)
	$(MAKE) install PREFIX=../$(PACK)
	hdiutil create $(PACK).dmg -fs HFS+ -srcfolder $(PACK) -format UDBZ -ov

linuxpack:
	@echo Linux packaging not implemented.

$(PACKDIR)/README.html: $(PACKDIR)/README.md
	echo "<!DOCTYPE html><html><xmp>" > $@
	cat $< >> $@
	echo "</xmp>" >> $@
	echo "<script src=http://strapdownjs.com/v/0.2/strapdown.js></script>" >> $@
	echo "</html>" >> $@


#===============================================================
# clean
#===============================================================
clean:
	-[ -d bin ] && rm -rf bin
	-[ -d lib ] && rm -rf lib
	-[ -d $(LIBDIR) ] && rm -rf $(LIBDIR)
