#!/bin/zsh

#
#   MusicFormats Library
#   Copyright (C) Jacques Menu 2016-2023
#
#   This Source Code Form is subject to the terms of the Mozilla Public
#   License, v. 2.0. If a copy of the MPL was not distributed with this
#   file, you can obtain one at http://mozilla.org/MPL/2.0/.
#
#   https://github.com/jacques-menu/musicformats
#

set -x

# fetch the needed environment
. ${HOME}/JMI_ShellSettings/ZshDefinitionsForMusicFormats.zsh


function addAllToMusicFormatsLocalRepository ()
{
  git add -f ${MUSIC_FORMATS_DEV}/MusicFormatsVersionNumber.txt
  git add -f ${MUSIC_FORMATS_DEV}/MusicFormatsVersionDate.txt

  git add -f ${MUSIC_FORMATS_DEV}/src/MusicFormatsVersionNumber.h
  git add -f ${MUSIC_FORMATS_DEV}/src/MusicFormatsVersionDate.h

  addSrc

  addBuild
  addDevtools
  addInclude

  addDoc

  addFxml
  addFmfsl
}

function addSrc ()
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

function addBuild ()
{
  git add    ${BUILD_DIR}/Building.md
  git add -f ${BUILD_DIR}/CMakeLists.txt
  git add    ${BUILD_DIR}/MakePkg.bat
  git add -f ${BUILD_DIR}/Makefile
}

function addDevtools ()
{
  cp -p ${HOME}/JMI_ShellSettings/ZshDefinitionsForMusicFormats.zsh ${DEVTOOLS_DIR}

  git add ${DEVTOOLS_DIR}
}

function addInclude ()
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

function addDoc ()
{
  git add ${DOC_DIR}/libmusicxml2Presentation/*.pdf  # libmusicxml2

  git add ${DOC_DIR}/*/*.tex
  git add ${DOC_DIR}/*/*.ist
  git add ${DOC_DIR}/*/*.pdf
  git add ${DOC_DIR}/*/*/*.pdf
  git add ${DOC_DIR}/*/*.png
#  git add ${DOC_DIR}/*/*.odg
}

function addFxml ()
{
  git add    ${MUSIC_FORMATS_DEV}/musicxmlfiles/*/*.xml
  git add -f ${MUSIC_FORMATS_DEV}/musicxmlfiles/*/*.txt

  git add -f ${MUSIC_FORMATS_DEV}/musicxmlfiles/Makefile

  git add    ${MUSIC_FORMATS_DEV}/musicxmlfiles/*.bash
  git add    ${MUSIC_FORMATS_DEV}/musicxmlfiles/*.mfsl
}

function addFmfsl ()
{
  git add    ${MUSIC_FORMATS_DEV}/mfslfiles/*.mfsl
  git add    ${MUSIC_FORMATS_DEV}/mfslfiles/*.xml
  git add    ${MUSIC_FORMATS_DEV}/mfslfiles/*.bash
}


echo "Let's go!'"

addAllToMusicFormatsLocalRepository

set +x
