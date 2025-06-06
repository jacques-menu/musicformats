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

# set -x

# the MusicFormats directory
export MUSICFORMATS_DIR=${HOME}/JMI_DEVELOPMENT/musicformats-git-dev
echo "--> MUSICFORMATS_DIR: ${MUSICFORMATS_DIR}"
echo

ls -sal ${MUSICFORMATS_DIR}


# set the MusicFormats variables
. ${MUSICFORMATS_DIR}/mfdevtools/SetMusicFormatsVariables.zsh


# Write all output to logfile
#exec > ${MUSICFORMATS_DIR}/$(basename $0).log 2>&1


# where to download the GitHub ZIP artifacts
DOWNLOADS_DIR=${HOME}/Downloads
echo "--> DOWNLOADS_DIR: ${DOWNLOADS_DIR}"
echo
ls -sal "${DOWNLOADS_DIR}/musicformats-*-artifact"
echo


# create MUSICFORMATS_RELEASES_DIR it if it does not exist
MUSICFORMATS_RELEASES_DIR=$MUSICFORMATS_DIR/../musicformats_releases
echo "--> MUSICFORMATS_RELEASES_DIR: ${MUSICFORMATS_RELEASES_DIR}"

[[ ! -d "${MUSICFORMATS_RELEASES_DIR}" ]] && mkdir "${MUSICFORMATS_RELEASES_DIR}"


# a function to create the releases
function CreateReleaseForOS ()
{
  OS_NAME="$1"
  echo "----------------------------------------------"
  echo "Creating the MusicFormats release for ${OS_NAME}"
  echo "----------------------------------------------"
  echo

  ARTIFACT_NAME="musicformats-${OS_NAME}-artifact"
  echo "---> ARTIFACT_NAME: ${ARTIFACT_NAME}"
  ARTIFACT_DIR="${DOWNLOADS_DIR}/${ARTIFACT_NAME}"
  echo "---> ARTIFACT_DIR : ${ARTIFACT_DIR}"
  ls -salh "${ARTIFACT_DIR}"
  echo

  OS_RELEASE_NAME="musicformats-${OS_NAME}-${MUSICFORMATS_VERSION_NUMBER}"
  echo "---> OS_RELEASE_NAME: ${OS_RELEASE_NAME}"
  OS_RELEASE_DIR="${MUSICFORMATS_RELEASES_DIR}/${OS_RELEASE_NAME}"
  echo "---> OS_RELEASE_DIR : ${OS_RELEASE_DIR}"


  # create OS_RELEASE_NAME
  mkdir -p "${OS_RELEASE_NAME}"
  ls -salh "${OS_RELEASE_NAME}"
  echo


  # pushd to
  pushd "${OS_RELEASE_NAME}"
  echo -n "--> current directory: "; pwd
  echo


  # populate OS_RELEASE_DIR
# set -x
  cp -p  ${ARTIFACT_DIR}/MusicFormatsVersionNumber.txt .
  cp -p  ${ARTIFACT_DIR}/MusicFormatsVersionDate.txt .
#   cp -pr ${ARTIFACT_DIR}/*.pdf .
  cp -pr ${ARTIFACT_DIR}/bin .
  cp -pr ${ARTIFACT_DIR}/lib .
#   cp -pr ${ARTIFACT_DIR}/include .
  ls -salhr .
# set +x
  echo


  # back to previous working directory
  popd
  echo -n "--> current directory: "; pwd
  echo


  # create the zip archive
  OS_RELEASE_ZIP_NAME="${OS_RELEASE_NAME}.zip"
  echo "--> OS_RELEASE_ZIP_NAME: ${OS_RELEASE_ZIP_NAME}"
  OS_RELEASE_ZIP_FILE="${MUSICFORMATS_RELEASES_DIR}/${OS_RELEASE_ZIP_NAME}"
  echo "--> OS_RELEASE_ZIP_FILE: ${OS_RELEASE_ZIP_FILE}"
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

# cd to MUSICFORMATS_RELEASES_DIR
cd "${MUSICFORMATS_RELEASES_DIR}"
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
echo "==> final ${MUSICFORMATS_RELEASES_DIR} contents:"
echo "----------------------------------------------"
echo

cd ${MUSICFORMATS_RELEASES_DIR}
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
