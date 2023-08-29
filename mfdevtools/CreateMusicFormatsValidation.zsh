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

# set -x

# Write all output to logfile
# -----------------------------------------

MUSIC_FORMATS_DEV_DIR=/Volumes/JMI_Volume/JMI_Developpement/musicformats-git-dev
echo "--> MUSIC_FORMATS_DEV_DIR = ${MUSIC_FORMATS_DEV_DIR}"
echo

#exec > ${MUSIC_FORMATS_DEV_DIR}/$(basename $0).log 2>&1

# the MusicFormats local repo


# fetch the MusicFormats version number
VERSION_NUMBER=`cat ${MUSIC_FORMATS_DEV_DIR}/MusicFormatsVersionNumber.txt | tr '.' '_'`
# VERSION_NUMBER="v${VERSION_NUMBER}"
echo "--> VERSION_NUMBER = ${VERSION_NUMBER}"


# where to download the ZIP artifacts
VALIDATION_DIR=${MUSIC_FORMATS_DEV_DIR}/validation
echo "--> VALIDATION_DIR = ${VALIDATION_DIR}"
echo
ls -sal "${VALIDATION_DIR}"
echo


# create MF_RELEASES_DIR it if it does not exist
MF_RELEASES_DIR=/Volumes/JMI_Volume/JMI_Developpement/musicformats_releases
echo "--> MF_RELEASES_DIR = ${MF_RELEASES_DIR}"

[[ ! -d "${MF_RELEASES_DIR}" ]] && mkdir "${MF_RELEASES_DIR}"

function CreateValidation ()
{
  OS_NAME="$1"
  echo "----------------------------------------------"
  echo "Creating the MusicFormats release for ${OS_NAME}"
  echo "----------------------------------------------"
  echo

  ARTIFACT_NAME="musicformats-${OS_NAME}-artifact"
  ARTIFACT_DIR="${DOWNLOADS_DIR}/${ARTIFACT_NAME}"
  echo "---> ARTIFACT_NAME: ${ARTIFACT_NAME}"
  echo "---> ARTIFACT_DIR : ${ARTIFACT_DIR}"
  ls -salh "${ARTIFACT_DIR}"
  echo

  VALIDATION_NAME="musicformats-${OS_NAME}-${VERSION_NUMBER}"
  VALIDATION_DIR="${MF_RELEASES_DIR}/${VALIDATION_NAME}"
  echo "---> VALIDATION_NAME: ${VALIDATION_NAME}"
  echo "---> VALIDATION_DIR : ${VALIDATION_DIR}"


  # create VALIDATION_NAME
  mkdir "${VALIDATION_NAME}"
  ls -salh "${VALIDATION_NAME}"
  echo


  # pushd to
  pushd "${VALIDATION_NAME}"
  echo -n "--> current directory: "; pwd
  echo



exit


  # populate VALIDATION_DIR
# set -x
  cp -p ${ARTIFACT_DIR}/MusicFormatsVersionNumber.txt .
  cp -p ${ARTIFACT_DIR}/MusicFormatsVersionDate.txt .
  cp -pr ${ARTIFACT_DIR}/documentation/*/*.pdf .
  cp -pr ${ARTIFACT_DIR}/build/bin .
  cp -pr ${ARTIFACT_DIR}/build/lib .
  cp -pr ${ARTIFACT_DIR}/include .
  ls -salhr .
# set +x
  echo


  # back to previous working directory
  popd
  echo -n "--> current directory: "; pwd
  echo


  # create the zip archive
  VALIDATION_ZIP_NAME="${VALIDATION_NAME}.zip"
  echo "--> VALIDATION_ZIP_NAME = ${VALIDATION_ZIP_NAME}"
  VALIDATION_ZIP_FILE="${MF_RELEASES_DIR}/${VALIDATION_ZIP_NAME}"
  echo "--> VALIDATION_ZIP_FILE = ${VALIDATION_ZIP_FILE}"
  echo

# set -x
  zip --recurse-paths ${VALIDATION_ZIP_NAME} ${VALIDATION_NAME}
  ls -salhr
# set +x
  echo


  # remove VALIDATION_NAME
# set -x
  rm -rf ${VALIDATION_NAME}
# set +x
  echo
}


echo "----------------------------------------------"
echo "==> Let's go!"
echo "----------------------------------------------"
echo

# cd to MF_RELEASES_DIR
cd "${MF_RELEASES_DIR}"
echo -n "--> current directory: "; pwd
echo
ls -sal
echo
echo


echo "----------------------------------------------"
echo "==> create the releases:"
echo "----------------------------------------------"
echo

CreateValidation
echo

echo "----------------------------------------------"
echo "==> final ${MF_RELEASES_DIR} contents:"
echo "----------------------------------------------"
echo

echo "----------------------------------------------"
echo "==> Validation:"
echo "----------------------------------------------"
echo
ls -salht ${MF_RELEASES_DIR}/musicformats-validation*
echo

# set +x
