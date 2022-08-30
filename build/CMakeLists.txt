#===============================================================
# CMAKE musicformats
#===============================================================

project(musicformats)
cmake_minimum_required(VERSION 2.4)
if(CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES Debug Release)
endif()


#===============================================================
# prevent execution in build folder
if (EXISTS .nocmake)
	message (FATAL_ERROR "Can't execute cmake in build folder, .nocmake found")
endif()


#===============================================================
# MusicXML versions management
set (XMLVERSION 4.0)


#===============================================================
# MusicFormats versions management
#set (VERSION 0.9.61) # now set from a text file, next line:
execute_process(COMMAND cat ../../MusicFormatsVersionNumber.txt OUTPUT_VARIABLE VERSION)
message (STATUS "VERSION: ${VERSION}")

macro (get_major_minor_patch version_num)
	string( REGEX REPLACE "([0-9]*)\\.([0-9]*)\\.([0-9]*)" "\\1"  VERSION_MAJOR ${version_num} )
	string( REGEX REPLACE "([0-9]*)\\.([0-9]*)\\.([0-9]*)" "\\2"  VERSION_MINOR ${version_num} )
	string( REGEX REPLACE "([0-9]*)\\.([0-9]*)\\.([0-9]*)" "\\3"  VERSION_PATCH ${version_num} )
endmacro()

get_major_minor_patch( ${VERSION} )
set (SOVERS ${VERSION_MAJOR})
set (STRVERS "v"${VERSION})
set (SSTRVERS ${VERSION})

message (STATUS "Configuring version ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

get_filename_component(ROOT ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)


#===============================================================
option ( LIB 		      "Generates the musicxml library by default"     	on )
option ( LILY         "Generates lilypond part"       	                on )
option ( BRAILLE      "Generates braille part"	                        on )
option ( MXML         "Generates MusicXML part"	                        on )
option ( GUIDO        "Generates GUIDO part"	                          on )
option ( MSDL         "Generates MSDL part"	                            on )
option ( ALL 		      "set LIB, LILY, BRAILLE, MXML, GUIDO and MSDL on" off )
option ( FMWK 	  	  "Generates a framework on MacOS by default"	      off )
option ( GDB 		      "Activates ggdb3 option"	                        off )
option ( APPLEDEBUG   "Activates Apple debug option"	                  on )

if (ALL)
	set (LIB on)
	set (LILY on)
	set (BRAILLE on)
	set (MXML on)
	set (GUIDO on)
	set (MSDL on)
endif()

if (MSDL)
	message (STATUS "MSDL is on")
	set (LIB on)
	set (LILY on)
	set (BRAILLE on)
	set (MXML on)
	set (GUIDO on)
else ()
  message (STATUS "MSDL is off")
endif()

if (LILY OR BRAILLE OR MXML OR GUIDO)
	set (LIB on)
	set (MSR on)
endif()

if (LIB)
	message (STATUS "Generates the musicxml library")
endif()
if (LILY)
	message (STATUS "Generates the lilypond part")
endif()
if (BRAILLE)
	message (STATUS "Generates the braille part")
endif()
if (MXML)
	message (STATUS "Generates the MusicXML part")
endif()
if (GUIDO)
	message (STATUS "Generates the GUIDO part")
endif()
if (MSDL)
	message (STATUS "Generates the MSDL part")
endif()


#===============================================================
get_filename_component(ROOT ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
set (DOCDIR ${ROOT}/documentation)
set (DOC    ${DOCDIR}/presentation/libmusicxml2.pdf ${DOCDIR}/IntroductionToMusicXML/IntroductionToMusicXML.pdf ${DOCDIR}/MusicFormatsCLIUserGuide/MusicFormatsCLIUserGuide.pdf ${DOCDIR}/MusicFormatsAPIUserGuide/MusicFormatsAPIUserGuide.pdf ${DOCDIR}/MusicFormatsMaintainanceGuide/MusicFormatsMaintainanceGuide.pdf)

set (BINDIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set (LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)


#===============================================================
# set (CMAKE_CXX_STANDARD 14) # C++14
set (CMAKE_CXX_STANDARD 17) # C++17 for <filesystem>

if(UNIX)
  if (GDB)
    add_definitions(-Wall -DGCC -ggdb3 -Wno-overloaded-virtual)
  else()
    add_definitions(-Wall -DGCC -O3 -Wno-overloaded-virtual)
  endif()
endif()


###===============================================================
# mac os specific part
###===============================================================
if(APPLE)
	if ( IOS )
		message (STATUS "Generates project for iOS - Use -DIOS=no to change.")
		set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "iPhoneOS")
		set (CMAKE_OSX_ARCHITECTURES "arm64") # unsupported on iOS 11.4.0: armv7 armv7s
		set (CMAKE_OSX_SYSROOT "iphoneos")
		set (CMAKE_CXX_FLAGS "-miphoneos-version-min=11.2")
		set (libtype STATIC)
		set (FMWK "false")
	else()
		set (CMAKE_OSX_ARCHITECTURES "x86_64")
		set (CMAKE_OSX_DEPLOYMENT_TARGET 10.12)

#		add_definitions(--analyze --analyzer-output text) # Run the static analyzer JMI

	endif()

  if (APPLEDEBUG)
		message (STATUS "Generates debug information on Apple - Use -DAPPLEDEBUG=on to change. .")
    add_definitions(-O0 -g)
  endif()


#===============================================================
# windows specific part
#===============================================================
elseif(WIN32)
  add_definitions(-DWINVER=0x0400 -DWIN32)
  if(MSVC)
    add_definitions(-DMSVC)
  endif()
endif()


#===============================================================
# set directories, src and headers.
set (MF_SRC_DIR ${ROOT}/src)

set (SCHEMA ${ROOT}/libmusicxml/schemas)  ### /${XMLVERSION}
set (LXML_SRC_DIR ${ROOT}/libmusicxml/src)

set (LXML_SAMPLE_DIR ${ROOT}/libmusicxml/samples)

set (MF_CLI_SAMPLES_DIR ${ROOT}/src/clisamples)

set (LXML_FOLDERS factory files elements guido lib operations parser visitors interface)

set (SRC_FOLDERS interfaces)

if (LIB)
#	set (SRC_FOLDERS ${SRC_FOLDERS} elements/xml)
    add_definitions(-DXML)
endif()

set (SRC_FOLDERS ${SRC_FOLDERS} mflibrary components oah interpreters/mfslinterpreter interpreters/ischemeinterpreter wae manpage)

if (MSR)
	set (SRC_FOLDERS ${SRC_FOLDERS} passes/musicxml2mxsr representations/mxsr passes/mxsr2msr representations/msr passes/msr2msr generators/Mikrokosmos3Wandering generators/LilyPondIssue34)
endif()

if (LILY)
	set (SRC_FOLDERS ${SRC_FOLDERS} passes/msr2lpsr representations/lpsr passes/lpsr2lilypond converters/musicxml2lilypond converters/msr2lilypond)
endif()

if (BRAILLE)
	set (SRC_FOLDERS ${SRC_FOLDERS} passes/msr2bsr representations/bsr representations/braille passes/bsr2bsr passes/bsr2braille converters/musicxml2braille converters/msr2braille)
endif()

if (MXML OR GUIDO)
	set (SRC_FOLDERS ${SRC_FOLDERS} passes/msr2mxsr)
endif()

if (MXML)
	set (SRC_FOLDERS ${SRC_FOLDERS} formats/musicxml passes/mxsr2musicxml converters/musicxml2musicxml converters/msr2musicxml)
endif()

if (GUIDO)
	set (SRC_FOLDERS ${SRC_FOLDERS} representations/guido passes/mxsr2guido converters/musicxml2guido formats/guido converters/msr2guido)
endif()

if (MSDL)
	set (SRC_FOLDERS ${SRC_FOLDERS} representations/msdl representations/msdr passes/msdl2msr passes/msdl2msr converters/msdl2braille converters/msdl2guido converters/msdl2lilypond converters/msdl2musicxml converters/msdlconverter)
endif()

message (STATUS "SRC_FOLDERS: ${SRC_FOLDERS}")
message (STATUS "LXML_FOLDERS: ${LXML_FOLDERS}")

foreach(folder ${LXML_FOLDERS})
  set(SRC ${SRC} "${LXML_SRC_DIR}/${folder}/*.cpp")        # add source files
endforeach()

foreach(folder ${LXML_FOLDERS})
  set(HEADERS ${HEADERS} "${LXML_SRC_DIR}/${folder}/*.h")  # add header files
endforeach()

foreach(folder ${SRC_FOLDERS})
  set(HEADERS ${HEADERS} "${MF_SRC_DIR}/${folder}/*.h")    # add header files
endforeach()

foreach(folder ${SRC_FOLDERS})
  set(SRC ${SRC} "${MF_SRC_DIR}/${folder}/*.cpp")          # add source files
endforeach()

file (GLOB CORESRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SRC})
file (GLOB COREH   RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${HEADERS})

foreach(folder ${LXML_FOLDERS})
  set(INCL ${INCL} "${LXML_SRC_DIR}/${folder}")            # add include folders
endforeach(folder)

foreach(folder ${SRC_FOLDERS})
  set(INCL ${INCL} "${MF_SRC_DIR}/${folder}")              # add include folders
endforeach(folder)


#===============================================================
# set includes
include_directories( ${INCL})
set_source_files_properties (${COREH} PROPERTIES HEADER_FILE_ONLY TRUE)

message (STATUS "INCL: ${INCL}")


#===============================================================
if(WIN32 OR MSYS)
  enable_language(RC)
  set(LIBCONTENT ${CORESRC} ${COREH} ${ROOT}/win32/musicformats/musicformats.rc)
else()
  set(LIBCONTENT ${CORESRC} ${COREH})
endif()

set(target musicformats)
set(staticlib musicformatsstatic)

if (NOT IOS)
add_library(${target} SHARED ${LIBCONTENT})
set_target_properties (${target} PROPERTIES
			VERSION ${VERSION}
			SOVERSION ${SOVERS}
			ARCHIVE_OUTPUT_DIRECTORY  ${LIBDIR}
			ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
			LIBRARY_OUTPUT_DIRECTORY ${LIBDIR}
			LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBDIR}
			PUBLIC_HEADER "${COREH}"
			DEFINE_SYMBOL LIBMUSICXML_EXPORTS
			XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
)
endif()

if (NOT (WIN32 OR WIN64)) # JMI v0.9.66
add_library(${staticlib} STATIC ${LIBCONTENT})
set_target_properties (${staticlib} PROPERTIES
			VERSION ${VERSION}
			SOVERSION ${SOVERS}
			ARCHIVE_OUTPUT_DIRECTORY  ${LIBDIR}
			ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
			LIBRARY_OUTPUT_DIRECTORY ${LIBDIR}
			LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBDIR}
			PUBLIC_HEADER "${COREH}"
			DEFINE_SYMBOL LIBMUSICXML_EXPORTS
			XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
			OUTPUT_NAME musicformats
)
endif()

if (FMWK AND APPLE AND NOT IOS)
set_target_properties (${target} PROPERTIES
      OUTPUT_NAME musicformats
      FRAMEWORK ${FMWK}
      FRAMEWORK_VERSION ${VERSION}
      MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${STRVERS}
      MACOSX_FRAMEWORK_BUNDLE_VERSION ${SSTRVERS}
      MACOSX_FRAMEWORK_IDENTIFIER ${target}
)
endif()

#===============================================================
# set sample targets
set (LXML_CLI_SAMPLES xmlversion RandomMusic RandomChords xmlfactory partsummary xml2guido xmliter countnotes readunrolled xml2midi xmlread xmltranspose xmlclone)

set (MF_CLI_SAMPLES displayMusicformatsVersion displayMusicformatsHistory MusicAndHarmonies Mikrokosmos3Wandering LilyPondIssue34 mfsl ischeme)

set (LXML_TOOLS xml2guido xmlread xmltranspose xmlversion)

if (LILY)
	set (MF_CLI_SAMPLES ${MF_CLI_SAMPLES}   xml2ly)
endif()

if (BRAILLE)
	set (MF_CLI_SAMPLES ${MF_CLI_SAMPLES}   xml2brl)
endif()

if (MXML)
	set (MF_CLI_SAMPLES ${MF_CLI_SAMPLES}   xml2xml)
endif()

if (GUIDO)
	set (MF_CLI_SAMPLES ${MF_CLI_SAMPLES}   xml2gmn)
endif()

if (MSDL)
	set (MF_CLI_SAMPLES ${MF_CLI_SAMPLES}   msdl)
endif()


if (APPLE)
  set (LINKTO ${staticlib})
else()
  set (LINKTO ${target})
endif()

set (LINKTO ${target}) # JMI v0.9.66

message (STATUS "LXML_CLI_SAMPLES: ${LXML_CLI_SAMPLES}")
message (STATUS "MF_CLI_SAMPLES: ${MF_CLI_SAMPLES}")

message (STATUS "LXML_TOOLS: ${LXML_TOOLS}")

message (STATUS "LINKTO: ${LINKTO}")


if (NOT IOS)
  foreach(sample ${LXML_CLI_SAMPLES})
    add_executable( ${sample} ${LXML_SAMPLE_DIR}/${sample}.cpp )
    target_link_libraries( ${sample} ${LINKTO})
    set_target_properties (${sample} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY  ${BINDIR}
      RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BINDIR})
    add_dependencies(${sample} ${target})
  endforeach(sample)

  foreach(sample ${MF_CLI_SAMPLES})
    add_executable( ${sample} ${MF_CLI_SAMPLES_DIR}/${sample}.cpp )
    target_link_libraries( ${sample} ${LINKTO})
    set_target_properties (${sample} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY  ${BINDIR}
      RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BINDIR})
    add_dependencies(${sample} ${target})
  endforeach(sample)

#   if (WIN32 OR WIN64 OR MSYS) # JMI v0.9.66
#     file (COPY ${LIBDIR}/musicformats.dll DESTINATION ${BINDIR})
#   endif()
endif()


#===============================================================
# install setup
if (PACK)
	set (CMAKE_INSTALL_PREFIX .)
endif()

set (DOCDIR  ${CMAKE_INSTALL_PREFIX}/share/musicformats)
if (APPLE)
  set (BINDEST bin)
  set (LIBDEST lib)
elseif (WIN32)
  set (BINDEST bin)
  set (LIBDEST bin)
  set (DOCDIR  .)
else ()
  set (LIBDEST lib)
  set (BINDEST bin)
endif ()


if (NOT IOS)
message (STATUS "Install location is ${CMAKE_INSTALL_PREFIX}")
if (WIN32)
	set (INSTALLED ${target})
else()
	set (INSTALLED ${target}  ${staticlib})
endif()
install ( TARGETS ${INSTALLED}
	RUNTIME DESTINATION ${BINDEST}
    LIBRARY DESTINATION ${LIBDEST}
    ARCHIVE DESTINATION ${LIBDEST}
    FRAMEWORK DESTINATION lib CONFIGURATIONS Release
    PUBLIC_HEADER DESTINATION include/musicformats
)

install ( TARGETS ${LXML_TOOLS}
    RUNTIME DESTINATION ${BINDEST}
    LIBRARY DESTINATION ${LIBDEST}
    ARCHIVE DESTINATION ${LIBDEST}
)


#===============================================================
# install misc files
install (
    DIRECTORY ${SCHEMA} DESTINATION ${DOCDIR}/schema
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
    DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
      GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    PATTERN ".DS_Store" EXCLUDE
)

install (
    FILES ${DOC} DESTINATION ${DOCDIR}/documentation
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)

install (
    FILES ${ROOT}/packages/README.html DESTINATION .
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)

install (
    FILES ${ROOT}/libmusicxml/CHANGELOG.txt  DESTINATION .
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)


#===============================================================
# install VS redistributables
if (WIN32)
if (${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
	set (VS "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/redist/x64/Microsoft.VC140.CRT")
elseif (${CMAKE_GENERATOR} STREQUAL "Visual Studio 15 2017 Win64")
	set (VS "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Redist/MSVC/14.16.27012/x64/Microsoft.VC141.CRT")
else()
	set (VS "/unknown/visual/studio/version")
endif()
install (
    FILES  "${VS}/msvcp140.dll" "${VS}/vcruntime140.dll" DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)
endif()


#===============================================================
# cpack section
#===============================================================
set (CPACKDEST libMusicFormats-${VERSION})
set (CPACK_PACKAGE_NAME   "libMusicFormats")
set (CPACK_PACKAGE_VENDOR "Jacques Menu")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "libMusicFormats - a library and tools to convert music scores formats.")
set (CPACK_PACKAGE_VERSION   ${VERSION})
set (CPACK_PACKAGE_VERSION_MAJOR   ${VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR   ${VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH   ${VERSION_PATCH})

set (CPACK_MONOLITHIC_INSTALL true)
set (CPACK_RESOURCE_FILE_LICENSE  ${ROOT}/LICENSE)
set (CPACK_PACKAGE_INSTALL_DIRECTORY  ${CPACKDEST})


#===============================================================
# NSIS specific section
set (CPACK_NSIS_INSTALL_ROOT "C:\\\\Program Files")
set (CPACK_NSIS_MODIFY_PATH on)
#set (CPACK_NSIS_MUI_ICON "${ROOT}/Resources/Images/FaustLiveIcon.ico")

include (CPack)


endif()
