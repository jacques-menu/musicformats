#!/bin/zsh

#
#   MusicFormats Library
#   Copyright (C) Jacques Menu 2016-2025
#
#   This Source Code Form is subject to the terms of the Mozilla Public
#   License, v. 2.0. If a copy of the MPL was not distributed with this
#   file, you can obtain one at http://mozilla.org/MPL/2.0/.
#
#   https://github.com/jacques-menu/musicformats
#

set -x

# the MusicFormats directory
export MUSICFORMATS_DIR=${HOME}/JMI_DEVELOPMENT/musicformats-git-dev
echo "--> MUSICFORMATS_DIR: ${MUSICFORMATS_DIR}"
echo

ls -sal ${MUSICFORMATS_DIR}


# set the MusicFormats variables
. ${MUSICFORMATS_DIR}/mfdevtools/SetMusicFormatsVariables.zsh


# Write all output to logfile
#exec > ${MUSICFORMATS_DIR}/$(basename $0).log 2>&1


# fetch the needed environment
. ${HOME}/JMI_SHELL_SETTINGS/ZshDefinitionsForMusicFormats.zsh


# main function
function addAllToMusicFormatsLocalRepository ()
{
  # MusicFormats version dataa
  git add -f ${MUSICFORMATS_DIR}/MusicFormatsVersionNumber.txt
  git add -f ${MUSICFORMATS_DIR}/MusicFormatsVersionDate.txt

  git add -f ${MUSICFORMATS_DIR}/src/MusicFormatsVersionNumber.h
  git add -f ${MUSICFORMATS_DIR}/src/MusicFormatsVersionDate.h

  # src
  add_src

  # build and other stuff
  add_build
  add_mfdevtools
  add_include

  # documentation
  add_documentation

  // MusicXML files
  add_musicxml_files

  // LilyPond files
  add_lilypond_files

  add_mfsl_files
}


# specific functions to add the needed files
function add_src ()
{
  git add ${SRC_DIR}/*.h

  git add -f ${MFSL_DIR}/Makefile

  git add -f ${ISCM_DIR}/Makefile

  git add ${SRC_DIR}/clisamples/*
  git add ${SRC_DIR}/components/*
  git add ${SRC_DIR}/converters/*/*
  git add ${SRC_DIR}/formats/*/*
  git add ${SRC_DIR}/generators/*/*
  git add ${SRC_DIR}/interpreters/*/*
  git add ${SRC_DIR}/mflibrary/*
  git add ${SRC_DIR}/oah/*
  git add ${SRC_DIR}/passes/*/*
  git add ${SRC_DIR}/representations/*/*
  git add ${SRC_DIR}/wae/*

  git add ${SRC_DIR}/*.ts ${SRC_DIR}/*.js ${SRC_DIR}/*.json
}


function add_build ()
{
  git add    ${BUILD_DIR}/Building.md
  git add -f ${BUILD_DIR}/CMakeLists.txt
  git add    ${BUILD_DIR}/MakePkg.bat
}


function add_mfdevtools ()
{
  cp -p ${HOME}/JMI_ShellSettings/ZshDefinitionsForMusicFormats.zsh ${DEVTOOLS_DIR}

  git add ${DEVTOOLS_DIR}
}


function add_include ()
{
# 	set -x
	echo "--> INCLUDE_DIR: ${INCLUDE_DIR}"

	rm -rf ${INCLUDE_DIR}
	mkdir -p ${INCLUDE_DIR}

	cd ${SRC_DIR}

	rsync -R **/*.h ${INCLUDE_DIR_NAME}

	mv ${INCLUDE_DIR_NAME} ..

  git add ../${INCLUDE_DIR_NAME}/*
#   set +x
}


function add_documentation ()
{
  git add ${DOC_DIR}/libmusicxml2Presentation/*.pdf  # libmusicxml2

  git add ${DOC_DIR}/*/*.tex
  git add ${DOC_DIR}/*/*.ist
  git add ${DOC_DIR}/*/*.pdf
  git add ${DOC_DIR}/*/*/*.pdf
  git add ${DOC_DIR}/*/*.png
#  git add ${DOC_DIR}/*/*.odg

  # copy PDF documentation files to musicformats-documentation-git
  cp -p ${DOC_DIR}/*/*.pdf ../musicformats-documentation-git
}

function add_musicxml_files ()
{
  # the MusicXML files Makefile
  git add -f ${MUSICFORMATS_DIR}/musicxmlfiles/Makefile

  # the MusicXML files in the subdirectories
  git add    ${MUSICFORMATS_DIR}/musicxmlfiles/*/*.xml
  git add    ${MUSICFORMATS_DIR}/mfslfiles/*/*.musicxml

  # the MusicXML files in the validated_* subdirectories
  git add    ${MUSICFORMATS_DIR}/musicxmlfiles/*/validated_*/*.xml
  git add    ${MUSICFORMATS_DIR}/musicxmlfiles/*/validated_*/*.musicxml

  # the MusicXML files in the problematic_* subdirectories
  git add    ${MUSICFORMATS_DIR}/musicxmlfiles/*/problematic_*/*.xml
  git add    ${MUSICFORMATS_DIR}/musicxmlfiles/*/problematic_*/*.musicxml

  # the scripts
  git add    ${MUSICFORMATS_DIR}/musicxmlfiles/*.bash
  git add    ${MUSICFORMATS_DIR}/musicxmlfiles/*.mfsl

  # *.txt files
  git add -f ${MUSICFORMATS_DIR}/musicxmlfiles/*/*.txt
}


function add_lilypond_files ()
{
  # the LilyPond files in the validated_* subdirectories
  git add    ${MUSICFORMATS_DIR}/musicxmlfiles/*/validated_*/*.ly

  # the LilyPond files in the problematic_* subdirectories
  git add    ${MUSICFORMATS_DIR}/musicxmlfiles/*/problematic_*/*.ly

  # B9S
  git add    ${MUSICFORMATS_DIR}/musicxmlfiles/challenging/BeethovenNinthSymphony.ly
}


function add_mfsl_files ()
{
  git add    ${MUSICFORMATS_DIR}/mfslfiles/*.mfsl
  git add    ${MUSICFORMATS_DIR}/mfslfiles/*.xml
  git add    ${MUSICFORMATS_DIR}/mfslfiles/*.musicxml
  git add    ${MUSICFORMATS_DIR}/mfslfiles/*.bash
}


echo "Let's go!'"

addAllToMusicFormatsLocalRepository

set +x
