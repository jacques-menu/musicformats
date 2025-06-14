#===============================================================
# cmake
#===============================================================

cmake_minimum_required(VERSION 3.30.0)

if (CMAKE_CONFIGURATION_TYPES)
  set (CMAKE_CONFIGURATION_TYPES Debug Release)
endif ()

set (CMAKE_VERBOSE_MAKEFILE ON) # JMI

message (STATUS "-----> FOOFOO  CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message (STATUS "-----> FOOFOO  CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")


#===============================================================
# CMAKE libmusicformats
#===============================================================

project(libmusicformats)


#===============================================================
# MusicXML version
#===============================================================

set (MUSICXML_VERSION 4.0)


#===============================================================
# MusicFormats version
#===============================================================

# execute_process(COMMAND cat ../MusicFormatsVersionNumber.txt OUTPUT_VARIABLE MUSICFORMATS_VERSION)
execute_process(COMMAND cat MusicFormatsVersionNumber.txt OUTPUT_VARIABLE MUSICFORMATS_VERSION)
message (STATUS "-----> MUSICFORMATS_VERSION: ${MUSICFORMATS_VERSION}")

macro (get_major_minor_patch version)
	string( REGEX REPLACE "(v[0-9]*)\\.([0-9]*)\\.([0-9]*)" "\\1"  MUSICFORMATS_VERSION_MAJOR ${version} )
	string( REGEX REPLACE "(v[0-9]*)\\.([0-9]*)\\.([0-9]*)" "\\2"  MUSICFORMATS_VERSION_MINOR ${version} )
	string( REGEX REPLACE "(v[0-9]*)\\.([0-9]*)\\.([0-9]*)" "\\3"  MUSICFORMATS_VERSION_PATCH ${version} )
endmacro()

get_major_minor_patch( ${MUSICFORMATS_VERSION} )

set (SO_VERSION ${MUSICFORMATS_VERSION_MAJOR})
message (STATUS "-----> SO_VERSION: ${SO_VERSION}")

set (SHORT_VERSION "v0.9.66")
message (STATUS "-----> SHORT_VERSION: ${SHORT_VERSION}")

set (VERSION_NUMBERS_ONLY "0.9.66")
message (STATUS "-----> VERSION_NUMBERS_ONLY: ${VERSION_NUMBERS_ONLY}")

message (STATUS "Configuring MusicFormats version ${MUSICFORMATS_VERSION_MAJOR}.${MUSICFORMATS_VERSION_MINOR}.${MUSICFORMATS_VERSION_PATCH}")

get_filename_component(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
message (STATUS "-----> ROOT_DIR: ${ROOT_DIR}")


#===============================================================
# generation settings
#===============================================================

option ( GENERATE_MACOS_FRAMEWORK   "Generates a framework on MacOS by default"	off )

option ( GDB 		      "Activates ggdb3 option"	              off )
option ( APPLEDEBUG   "Activates Apple debug option"	        on )

option ( GENERATE_EXECUTABLES "Generates tools"	              on )
option ( GENERATE_STATIC_LIB  "Generates the static library"  on )

if (GENERATE_EXECUTABLES)
	message (STATUS "Generate executabled (forces static library generation)")
	set (GENERATE_STATIC_LIB on)
endif ()

message (STATUS "-----> GENERATE_MACOS_FRAMEWORK: ${GENERATE_MACOS_FRAMEWORK}")
message (STATUS "-----> GENERATE_EXECUTABLES: ${GENERATE_EXECUTABLES}")
message (STATUS "-----> GENERATE_STATIC_LIB: ${GENERATE_STATIC_LIB}")


#===============================================================
# directories
#===============================================================

set (LXML_DOC_DIR ${ROOT_DIR}/libmusicxml/doc)
set (LXML_DOC    ${LXML_DOC_DIR}/presentation/libmusicxml2.pdf)
set (MXML_SCHEMAS_DIR ${ROOT_DIR}/dtds/${MUSICXML_VERSION})

set (BINDIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
set (LIBDIR ${CMAKE_CURRENT_BINARY_DIR}/lib)


#===============================================================
# CXX
#===============================================================

set (CMAKE_CXX_STANDARD 20)
message (STATUS "-----> CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")

message (STATUS "-----> UNIX: ${UNIX}")

if (UNIX)
  if (GDB)
    add_definitions(-Wall -DGCC -ggdb3 -Wno-overloaded-virtual)
  else()
    add_definitions(-Wall -DGCC -O3 -Wno-overloaded-virtual)
  endif()
endif ()

if (NOT APPLE)
  add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=0)
    # otherwise cxx11 ld errors occur on Windows and Linux
endif ()


#===============================================================
# MacOS specific part
#===============================================================

if (APPLE)
	set (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
	set (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "gnu++${CMAKE_CXX_STANDARD}")
	if ( IOS )
		message (STATUS "Generates project for iOS - Use -DIOS=no to change.")
		set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "iPhoneOS")
		#set (CMAKE_OSX_ARCHITECTURES "arm64 x86_64") # unsupported on iOS 11.4.0: armv7 armv7s
#		set (CMAKE_OSX_SYSROOT "iPhoneOS")				# don't know any generic way to address sysroot on ios
		set (IOS_DEPLOYMENT_TARGET 13.0)
		#set (libtype STATIC)
		set (GENERATE_MACOS_FRAMEWORK "true")
		set (CMAKE_MACOSX_RPATH 1)
	elseif ( UNIVERSAL )
		message (STATUS "Generates universal binaries - Use -DUNIVERSAL=no to change. .")
 		set (CMAKE_OSX_ARCHITECTURES "x86_64")
		set (CMAKE_OSX_DEPLOYMENT_TARGET 10.10)
	endif ()

  if (APPLEDEBUG)
		message (STATUS "Generates debug information on Apple - Use -DAPPLEDEBUG=on to change. .")
    add_definitions(-O0 -g)
  endif()
endif ()


#===============================================================
# Windows specific part
#===============================================================

if (WIN32)
  add_definitions(-DWINVER=0x0400 -DWIN32)
  if (MSVC)
    add_definitions(-DMSVC)
  endif ()
endif ()


#===============================================================
# MusicXML directories, sources and headers
#===============================================================

set (MXML_SCHEMAS_DIR ${ROOT_DIR}/libmusicxml/schemas)  ### /${MUSICXML_VERSION}


#===============================================================
# libmusicxml2 directories, sources and headers
#===============================================================

set (LXML_SRC_DIR  ${ROOT_DIR}/libmusicxml/src)
message (STATUS "-----> LXML_SRC_DIR: ${LXML_SRC_DIR}")

set (LXML_SAMPLE_DIR ${ROOT_DIR}/libmusicxml/samples)
message (STATUS "-----> LXML_SAMPLE_DIR: ${LXML_SAMPLE_DIR}")

set (LXML_SRC_FOLDERS factory files elements guido lib operations parser visitors interface)
message (STATUS "-----> LXML_SRC_FOLDERS: ${LXML_SRC_FOLDERS}")


#===============================================================
# fmt directories, sources and headers
#===============================================================

set (FMT_SRC_DIR  ${ROOT_DIR}/fmt/src)
message (STATUS "-----> FMT_SRC_DIR: ${FMT_SRC_DIR}")

set (FMT_INCLUDE_DIR ${ROOT_DIR}/fmt/include/fmt)
message (STATUS "-----> FMT_INCLUDE_DIR: ${FMT_INCLUDE_DIR}")

set (FMT_TEST_DIR ${ROOT_DIR}/fmt/tests)
message (STATUS "-----> FMT_TEST_DIR: ${FMT_TEST_DIR}")


#===============================================================
# MusicFormats directories, sources and headers
#===============================================================

set (MF_SRC_DIR  ${ROOT_DIR}/src)
message (STATUS "-----> MF_SRC_DIR: ${MF_SRC_DIR}")

set (MF_CLI_SAMPLES_DIR ${MF_SRC_DIR}/clisamples)
message (STATUS "-----> MF_CLI_SAMPLES_DIR: ${MF_CLI_SAMPLES_DIR}")

# set (MF_CLI_SERVICES_DIR ${MF_SRC_DIR}/services)
# message (STATUS "-----> MF_CLI_SERVICES_DIR: ${MF_CLI_SERVICES_DIR}")


set (MF_SRC_FOLDERS ${MF_SRC_FOLDERS} mfutilities)

set (MF_SRC_FOLDERS ${MF_SRC_FOLDERS} formats/musicxml formats/guido)

set (MF_SRC_FOLDERS ${MF_SRC_FOLDERS} representations/guido representations/mxsr representations/msr representations/lpsr)

set (MF_SRC_FOLDERS ${MF_SRC_FOLDERS} passes/mxsr2guido passes/mxsr2musicxml passes/musicxml2mxsr passes/mxsr2msr passes/msr2msr passes/msr2lpsr passes/lpsr2lilypond passes/msr2bsr passes/bsr2bsr passes/msr2mxsr passes/mxsr2musicxml)

set (MF_SRC_FOLDERS ${MF_SRC_FOLDERS} languages oah wae manpage)

set (MF_SRC_FOLDERS ${MF_SRC_FOLDERS} generators/Mikrokosmos3Wandering generators/LilyPondIssue34)

set (MF_SRC_FOLDERS ${MF_SRC_FOLDERS} converters/musicxml2guido converters/musicxml2lilypond converters/msr2lilypond converters/musicxml2musicxml converters/msr2musicxml converters/msr2guido)

set (MF_SRC_FOLDERS ${MF_SRC_FOLDERS} interpreters/stringmatcherinterpreter interpreters/mfslinterpreter interpreters/ischemeinterpreter)

set (MF_SRC_FOLDERS ${MF_SRC_FOLDERS} representations/msdl representations/msdr passes/msdl2msr passes/msdl2msr converters/msdl2guido converters/msdl2lilypond converters/msdl2musicxml converters/msdlconverter)

set (MF_SRC_FOLDERS ${MF_SRC_FOLDERS}  representations/bsr representations/braille passes/bsr2braille converters/musicxml2braille converters/msr2braille  converters/msdl2braille)

# set (MF_SRC_FOLDERS ${MF_SRC_FOLDERS} representations/mnxsr passes/mnxsr2msr passes/mnx2mnxsr)

set (MF_SRC_FOLDERS ${MF_SRC_FOLDERS} components)

set (MF_SRC_FOLDERS ${MF_SRC_FOLDERS} services)

message (STATUS "-----> MF_SRC_FOLDERS: ${MF_SRC_FOLDERS}")


#===============================================================
# collect all C++ files to be used
#===============================================================

# add libmusicmxl2 header and source files
foreach (folder ${LXML_SRC_FOLDERS})
  set (PUBLIC_HEADERS ${PUBLIC_HEADERS} "${LXML_SRC_DIR}/${folder}/*.h")
  set (SOURCES        ${SOURCES}        "${LXML_SRC_DIR}/${folder}/*.cpp")
endforeach ()

# add MusicFormats header and source files
foreach (folder ${MF_SRC_FOLDERS})
  set (PUBLIC_HEADERS ${PUBLIC_HEADERS} "${MF_SRC_DIR}/${folder}/*.h")
  set (SOURCES ${SOURCES} "${MF_SRC_DIR}/${folder}/*.cpp")
endforeach ()
message (STATUS "-----> FOOFOO PUBLIC_HEADERS: ${PUBLIC_HEADERS}")
message (STATUS "-----> FOOFOO  SOURCES: ${SOURCES}")

file (GLOB CORE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SOURCES})
# message (STATUS "-----> FOOFOO  CORE_SOURCES: ${CORE_SOURCES}")

file (GLOB CORE_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${PUBLIC_HEADERS})
message (STATUS "-----> FOOFOO  CORE_HEADERS: ${CORE_HEADERS}")

# add libmusicmxl2 include folders
foreach (folder ${LXML_SRC_FOLDERS})
  set (INCLUDES ${INCLUDES} "${LXML_SRC_DIR}/${folder}")
endforeach (folder)

# add MusicFormats include folders
foreach (folder ${MF_SRC_FOLDERS})
  set (INCLUDES ${INCLUDES} "${MF_SRC_DIR}/${folder}")
endforeach (folder)

message (STATUS "-----> FOOFOO  INCLUDES: ${INCLUDES}")


#===============================================================
# set includes
#===============================================================

include_directories (${INCLUDES})
set_source_files_properties (${CORE_HEADERS} PROPERTIES HEADER_FILE_ONLY TRUE)


#===============================================================
# set target
#===============================================================

if (WIN32 OR MSYS)
  enable_language(RC)
  set (LIBCONTENT ${CORE_SOURCES} ${CORE_HEADERS} ${ROOT_DIR}/win32/musicformats/musicformats.rc)
  if (MINGW)
	  set (target musicformats)
  else ()
	  set (target libmusicformats)
  endif ()
else ()
  set (LIBCONTENT ${CORE_SOURCES} ${CORE_HEADERS})
  set (target libmusicformats)
endif ()

message (STATUS "-----> target: ${target}")
message (STATUS "-----> LIBCONTENT: ${LIBCONTENT}")


#===============================================================
# library
#===============================================================

set (staticlib libmusicformatsstatic)

if (NOT IOS)
  message (STATUS "-----> NOT IOS, adding SHARED library ${target}")
	add_library(${target} SHARED ${LIBCONTENT})
	set_target_properties (${target} PROPERTIES
    MUSICFORMATS_VERSION ${MUSICFORMATS_VERSION}
    SO_VERSIONION ${SO_VERSION}
    RUNTIME_OUTPUT_DIRECTORY  ${LIBDIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
    ARCHIVE_OUTPUT_DIRECTORY  ${LIBDIR}
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
    LIBRARY_OUTPUT_DIRECTORY ${LIBDIR}
    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBDIR}
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
    DEFINE_SYMBOL LIBMUSICXML_EXPORTS
    XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
  )
elseif (IOS)
  message (STATUS "-----> IOS, adding SHARED library ${target}")
	add_library(${target} SHARED ${LIBCONTENT})
	set_target_properties (${target} PROPERTIES
    MUSICFORMATS_VERSION ${MUSICFORMATS_VERSION}
    SO_VERSIONION ${SO_VERSION}
    FRAMEWORK ${GENERATE_MACOS_FRAMEWORK}
    FRAMEWORK_VERSION C
    OUTPUT_NAME libmusicformats
    RUNTIME_OUTPUT_DIRECTORY  ${LIBDIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
    ARCHIVE_OUTPUT_DIRECTORY  ${LIBDIR}
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
    LIBRARY_OUTPUT_DIRECTORY ${LIBDIR}
    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBDIR}
    XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET ${IOS_DEPLOYMENT_TARGET}
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
    MACOSX_FRAMEWORK_IDENTIFIER "com.jmi.musicformats"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${SHORT_VERSION}
    MACOSX_FRAMEWORK_BUNDLE_VERSION ${MUSICFORMATS_VERSION}
    INSTALL_NAME_DIR "$(LOCAL_LIBRARY_DIR)/Frameworks"
    BUILD_WITH_INSTALL_NAME_DIR YES
    #XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "@executable_path/Frameworks"
    XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH YES
    DEFINE_SYMBOL LIBMUSICXML_EXPORTS
    XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
    XCODE_ATTRIBUTE_DEFINES_MODULE YES
	)
endif ()

if (NOT WIN32) #AND NOT IOS
  message (STATUS "-----> NOT WIN32, adding STATIC library ${staticlib}")
  add_library(${staticlib} STATIC ${LIBCONTENT})
  set_target_properties (${staticlib} PROPERTIES
    MUSICFORMATS_VERSION ${MUSICFORMATS_VERSION}
    SO_VERSIONION ${SO_VERSION}
    ARCHIVE_OUTPUT_DIRECTORY  ${LIBDIR}
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
    LIBRARY_OUTPUT_DIRECTORY ${LIBDIR}
    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBDIR}
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
    DEFINE_SYMBOL LIBMUSICXML_EXPORTS
    XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
    OUTPUT_NAME musicformats
  )
endif ()

if (GENERATE_MACOS_FRAMEWORK AND APPLE AND NOT IOS)
  set_target_properties (${target} PROPERTIES
    OUTPUT_NAME libmusicformats
    FRAMEWORK ${GENERATE_MACOS_FRAMEWORK}
	  FRAMEWORK_VERSION C
	  MACOSX_FRAMEWORK_IDENTIFIER "com.jmi.musicformats"
	  XCODE_ATTRIBUTE_DEFINES_MODULE YES
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${SHORT_VERSION}
    MACOSX_FRAMEWORK_BUNDLE_VERSION ${VERSION_NUMBERS_ONLY}
    MACOSX_FRAMEWORK_IDENTIFIER ${target}
  )
endif ()


#===============================================================
# documentation
#===============================================================

set (MUSICFORMATS_DOC_DIR ${ROOT_DIR}/documentation)
message (STATUS "-----> MUSICFORMATS_DOC_DIR: ${MUSICFORMATS_DOC_DIR}")

set (MUSICFORMATS_PDF_DOCS IntroductionToMusicXML/IntroductionToMusicXML.pdf libmusicxml2Presentation/libmusicxml2.pdf mfuserguide/MusicFormatsUserGuide.pdf mfmaintenanceguide/MusicFormatsMaintanceGuide.pdf mfapiguide/MusicFormatsAPIGuide.pdf   )
message (STATUS "-----> MUSICFORMATS_PDF_DOCS: ${MUSICFORMATS_PDF_DOCS}")


#===============================================================
# set sample targets
#===============================================================

set (LXML_CLI_SAMPLES xmlversion RandomMusic RandomChords xmlfactory partsummary xml2guido xmliter countnotes readunrolled xml2midi xmlread xmltranspose xmlclone)
message (STATUS "-----> LXML_CLI_SAMPLES: ${LXML_CLI_SAMPLES}")

# set (MF_CLI_SAMPLES displayMusicformatsVersion displayMusicformatsHistory MusicAndHarmonies Mikrokosmos3Wandering LilyPondIssue34 xml2ly xml2brl xml2xml xml2gmn msdl stringmatcher mfsl ischeme mnx2msr)
set (MF_CLI_SAMPLES displayMusicformatsVersion displayMusicformatsHistory MusicAndHarmonies Mikrokosmos3Wandering LilyPondIssue34 xml2ly xml2brl xml2xml xml2gmn msdl stringmatcher mfsl ischeme)
message (STATUS "-----> MF_CLI_SAMPLES: ${MF_CLI_SAMPLES}")

if (APPLE)
  set (LINKTO ${staticlib})
else ()
  set (LINKTO ${target})
endif ()


if (NOT IOS AND GENERATE_EXECUTABLES)
  foreach (sample ${LXML_CLI_SAMPLES})
    message (STATUS "-----> adding libmusicxml executable ${sample}")
    add_executable( ${sample} ${LXML_SAMPLE_DIR}/${sample}.cpp )
    target_link_libraries( ${sample} ${LINKTO})
    set_target_properties (${sample} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY  ${BINDIR}
      RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BINDIR})
    add_dependencies(${sample} ${target})
  endforeach (sample)

  foreach (sample ${MF_CLI_SAMPLES})
    message (STATUS "-----> adding MusicFormats executable ${sample}")
    add_executable( ${sample} ${MF_CLI_SAMPLES_DIR}/${sample}.cpp )
    target_link_libraries( ${sample} ${LINKTO})
    set_target_properties (${sample} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY  ${BINDIR}
      RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BINDIR})
    add_dependencies(${sample} ${target})
  endforeach (sample)
endif ()


#===============================================================
# install setup
#===============================================================

if (PACK)
	set (CMAKE_INSTALL_PREFIX .)
endif ()

set (INSTALL_DOC_DIR  ${CMAKE_INSTALL_PREFIX}/share/libmusicformats)

if (APPLE)
  set (BINDEST bin)
  set (LIBDEST lib)
elseif (WIN32)
  set (BINDEST bin)
  set (LIBDEST bin)
  set (INSTALL_DOC_DIR  .)
else ()
  set (LIBDEST lib)
  set (BINDEST bin)
endif ()

if (IOS)
	install ( TARGETS ${INSTALLED}
		RUNTIME DESTINATION ${BINDEST}
	    LIBRARY DESTINATION ${LIBDEST}
	    ARCHIVE DESTINATION ${LIBDEST}
	    FRAMEWORK DESTINATION lib CONFIGURATIONS Release
	    PUBLIC_HEADER DESTINATION include/libmusicformats
#	    PRIVATE_HEADER DESTINATION PrivateHeaders
	)
endif ()

if (NOT IOS)
  message (STATUS "Install location is ${CMAKE_INSTALL_PREFIX}")
  set (INSTALLED ${target})

  if (NOT WIN32)
    if (GENERATE_STATIC_LIB)
      set (INSTALLED ${INSTALLED}  ${staticlib})
    endif ()
  endif ()

  install ( TARGETS ${INSTALLED}
    RUNTIME DESTINATION ${BINDEST}
      LIBRARY DESTINATION ${LIBDEST}
      ARCHIVE DESTINATION ${LIBDEST}
      FRAMEWORK DESTINATION lib CONFIGURATIONS Release
      PUBLIC_HEADER DESTINATION include/libmusicformats
  )

  if (GENERATE_EXECUTABLES)
    install ( TARGETS ${LXML_SAMPLES}
      RUNTIME DESTINATION ${BINDEST}
      LIBRARY DESTINATION ${LIBDEST}
      ARCHIVE DESTINATION ${LIBDEST}
  )
  endif ()

  #===============================================================
  # install misc files
  #===============================================================

  install (
      DIRECTORY ${MXML_SCHEMAS_DIR} DESTINATION ${INSTALL_DOC_DIR}/schema
      FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
      DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
      PATTERN ".DS_Store" EXCLUDE
  )

  install (
      FILES ${LXML_DOC} DESTINATION ${INSTALL_DOC_DIR}/doc
      PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
  )

  install (
      FILES ${ROOT_DIR}/packages/README.html DESTINATION .
      PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
  )

  install (
      FILES ${ROOT_DIR}/CHANGELOG.txt  DESTINATION .
      PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
  )


  #===============================================================
  # install VS redistributables
  #===============================================================
  if (MSVC)
    include (InstallRequiredSystemLibraries)
  endif ()


  #===============================================================
  # cpack section
  #===============================================================

  set (CPACKDEST libMusicFormats-${MUSICFORMATS_VERSION})
  set (CPACK_PACKAGE_NAME   "libMusicFormats")
  set (CPACK_PACKAGE_VENDOR "Jacques Menu")
  set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "libMusicFormats - a library and tools to convert music scores formats.")
  set (CPACK_PACKAGE_VERSION   ${MUSICFORMATS_VERSION})
  set (CPACK_PACKAGE_VERSION_MAJOR   ${VERSION_MAJOR})
  set (CPACK_PACKAGE_VERSION_MINOR   ${VERSION_MINOR})
  set (CPACK_PACKAGE_VERSION_PATCH   ${VERSION_PATCH})

  set (CPACK_MONOLITHIC_INSTALL true)
  set (CPACK_RESOURCE_FILE_LICENSE  ${ROOT_DIR}/license.txt)
  set (CPACK_PACKAGE_INSTALL_DIRECTORY  ${CPACKDEST})


  #===============================================================
  # NSIS specific section
  #===============================================================

  set (CPACK_NSIS_INSTALL_ROOT "C:\\\\Program Files")
  set (CPACK_NSIS_MODIFY_PATH on)
  #set (CPACK_NSIS_MUI_ICON "${ROOT_DIR}/Resources/Images/FaustLiveIcon.ico")


  #===============================================================
  include (CPack)
  #===============================================================


endif ()
