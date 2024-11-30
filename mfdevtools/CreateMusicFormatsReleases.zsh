#!/bin/zsh

#
#   MusicFormats Library
#   Copyright (C) Jacques Menu 2016-2024
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
DOWNLOADS_DIR=${HOME}/JMI_Downloads
echo "--> DOWNLOADS_DIR = ${DOWNLOADS_DIR}"
echo
ls -sal "${DOWNLOADS_DIR}/musicformats-*-latest-artifact"
echo


# create MF_RELEASES_DIR it if it does not exist
MF_RELEASES_DIR=/Volumes/JMI_Volume/JMI_Developpement/musicformats_releases
echo "--> MF_RELEASES_DIR = ${MF_RELEASES_DIR}"

[[ ! -d "${MF_RELEASES_DIR}" ]] && mkdir "${MF_RELEASES_DIR}"

function CreateReleaseForOS ()
{
  OS_NAME="$1"
  echo "----------------------------------------------"
  echo "Creating the MusicFormats release for ${OS_NAME}"
  echo "----------------------------------------------"
  echo

  ARTIFACT_NAME="musicformats-${OS_NAME}-latest-artifact"
  ARTIFACT_DIR="${DOWNLOADS_DIR}/${ARTIFACT_NAME}"
  echo "---> ARTIFACT_NAME: ${ARTIFACT_NAME}"
  echo "---> ARTIFACT_DIR : ${ARTIFACT_DIR}"
  ls -salh "${ARTIFACT_DIR}"
  echo

  OS_RELEASE_NAME="musicformats-${OS_NAME}-${VERSION_NUMBER}"
  OS_RELEASE_DIR="${MF_RELEASES_DIR}/${OS_RELEASE_NAME}"
  echo "---> OS_RELEASE_NAME: ${OS_RELEASE_NAME}"
  echo "---> OS_RELEASE_DIR : ${OS_RELEASE_DIR}"


  # create OS_RELEASE_NAME
  mkdir "${OS_RELEASE_NAME}"
  ls -salh "${OS_RELEASE_NAME}"
  echo


  # pushd to
  pushd "${OS_RELEASE_NAME}"
  echo -n "--> current directory: "; pwd
  echo


  # populate OS_RELEASE_DIR
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
  OS_RELEASE_ZIP_NAME="${OS_RELEASE_NAME}.zip"
  echo "--> OS_RELEASE_ZIP_NAME = ${OS_RELEASE_ZIP_NAME}"
  OS_RELEASE_ZIP_FILE="${MF_RELEASES_DIR}/${OS_RELEASE_ZIP_NAME}"
  echo "--> OS_RELEASE_ZIP_FILE = ${OS_RELEASE_ZIP_FILE}"
  echo

# set -x
  zip --recurse-paths ${OS_RELEASE_ZIP_NAME} ${OS_RELEASE_NAME}
  ls -salhr
# set +x
  echo


  # remove OS_RELEASE_NAME
# set -x
  rm -rf ${OS_RELEASE_NAME}
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

CreateReleaseForOS "macos"
CreateReleaseForOS "ubuntu"
CreateReleaseForOS "windows"
echo

echo "----------------------------------------------"
echo "==> final ${MF_RELEASES_DIR} contents:"
echo "----------------------------------------------"
echo

cd ${MF_RELEASES_DIR}
pwd
echo

echo "----------------------------------------------"
echo "==> MacOS:"
echo "----------------------------------------------"
echo
ls -salht musicformats-macos-*.zip
echo

echo "----------------------------------------------"
echo "==> Ubuntu:"
echo "----------------------------------------------"
echo
ls -salht musicformats-ubuntu-*.zip
echo

echo "----------------------------------------------"
echo "==> Windows:"
echo "----------------------------------------------"
echo
ls -salht musicformats-windows-*.zip
echo

# set +x
