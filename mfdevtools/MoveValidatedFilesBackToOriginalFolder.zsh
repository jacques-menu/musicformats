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


# COPY musicformats-git-dev to a folder with today's date in it's name
# -------------------------------------------------

echo "---> go:"

echo "ARGUMENTS: $@"
echo

CURRENT_DIR_NAME=${PWD}
echo "CURRENT_DIR_NAME:          ${CURRENT_DIR_NAME}"

CURRENT_DIR_NAME_BASENAME=$(basename "${CURRENT_DIR_NAME}")
echo "CURRENT_DIR_NAME_BASENAME: ${CURRENT_DIR_NAME_BASENAME}"

VALIDATED_DIR_NAME="validated_${CURRENT_DIR_NAME_BASENAME}"
echo "VALIDATED_DIR_NAME:        ${VALIDATED_DIR_NAME}"
mkdir -p ${VALIDATED_DIR_NAME}

echo

for FILE_NAME in $@ ; do
#   echo "FILE_NAME:               ${FILE_NAME}"

  FILE_BASE_NAME=$(basename "${FILE_NAME}")
#   echo "FILE_BASE_NAME:          ${FILE_BASE_NAME}"

  FILENAME_WITHOUT_SUFFIX=$(echo "${FILE_BASE_NAME}" | cut -f 1 -d '.')
#   echo "FILENAME_WITHOUT_SUFFIX: ${FILENAME_WITHOUT_SUFFIX}"

#   set -x

  MUSICXML_FILE_NAME="${VALIDATED_DIR_NAME}/${FILENAME_WITHOUT_SUFFIX}.xml"
#   echo "MUSICXML_FILE_NAME:      ${MUSICXML_FILE_NAME}"
  if [ -f ${MUSICXML_FILE_NAME} ]; then
    echo "<-- moving ${MUSICXML_FILE_NAME} from ${VALIDATED_DIR_NAME}"
    mv "${MUSICXML_FILE_NAME}" .
  else
    echo "MUSICXML_FILE_NAME ${MUSICXML_FILE_NAME} does not exist"
  fi

  LILYPOND_FILE_NAME="${VALIDATED_DIR_NAME}/${FILENAME_WITHOUT_SUFFIX}.ly"
#   echo "LILYPOND_FILE_NAME:      ${LILYPOND_FILE_NAME}"
  if [ -f ${LILYPOND_FILE_NAME} ]; then
    echo "<-- moving ${LILYPOND_FILE_NAME} from ${VALIDATED_DIR_NAME}"
    mv "${LILYPOND_FILE_NAME}" .
  else
    echo "LILYPOND_FILE_NAME ${LILYPOND_FILE_NAME} does not exist"
  fi

  PDF_FILE_NAME="${VALIDATED_DIR_NAME}/${FILENAME_WITHOUT_SUFFIX}.pdf"
#   echo "PDF_FILE_NAME:           ${PDF_FILE_NAME}"
  if [ -f ${PDF_FILE_NAME} ]; then
    echo "<-- moving ${PDF_FILE_NAME} from ${VALIDATED_DIR_NAME}"
    mv "${PDF_FILE_NAME}" .
  else
    echo "PDF_FILE_NAME ${PDF_FILE_NAME} does not exist"
  fi

  MIDI_FILE_NAME="${VALIDATED_DIR_NAME}/${FILENAME_WITHOUT_SUFFIX}.midi"
#   echo "MIDI_FILE_NAME:          ${MIDI_FILE_NAME}"
  if [ -f ${MIDI_FILE_NAME} ]; then
    echo "<-- moving ${MIDI_FILE_NAME} from ${VALIDATED_DIR_NAME}"
    mv "${MIDI_FILE_NAME}" .
  else
    echo "MIDI_FILE_NAME ${MIDI_FILE_NAME} does not exist"
  fi

#   set +x

  echo
done

echo "<--- done"
