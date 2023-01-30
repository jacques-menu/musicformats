#!/bin/zsh

# set -x

# Write all output to logfile
# -----------------------------------------

#exec > ${MUSIC_FORMATS_DEV_DIR}/$(basename $0).log 2>&1

echo "----------------------------------------------"
# the MusicFormats local repo
echo "----------------------------------------------"

MUSIC_FORMATS_DEV_DIR=${HOME}/musicformats-git-dev
echo "--> MUSIC_FORMATS_DEV_DIR = ${MUSIC_FORMATS_DEV_DIR}"
cd ${MUSIC_FORMATS_DEV_DIR}


echo "----------------------------------------------"
# the MusicFormats version number
echo "----------------------------------------------"

VERSION_NUMBER=`cat MusicFormatsVersionNumber.txt | tr '.' '_'`
VERSION_NUMBER="v${VERSION_NUMBER}"
echo "--> VERSION_NUMBER = ${VERSION_NUMBER}"


echo "----------------------------------------------"
# where to download the ZIP artifacts
echo "----------------------------------------------"

DOWNLOADS_DIR=${HOME}/JMI_Downloads
echo "--> DOWNLOADS_DIR = ${DOWNLOADS_DIR}"
echo
ls -sal "${DOWNLOADS_DIR}/musicformats-*"
echo


echo "----------------------------------------------"
# where to create the releases ZIP files
echo "----------------------------------------------"

RELEASE_DIR=${MUSIC_FORMATS_DEV_DIR}/releases
echo "--> RELEASE_DIR = ${RELEASE_DIR}"
echo

[[ -d ${RELEASE_DIR} ]]; rm -rf ${RELEASE_DIR}
mkdir ${RELEASE_DIR}
ls -sal ${RELEASE_DIR}
echo


echo "----------------------------------------------"
# the release versions
echo "----------------------------------------------"

DOWNLOADED_MACOS_ARTIFACT_NAME="musicformats-macos-artifact"
DOWNLOADED_UBUNTU_ARTIFACT_NAME="musicformats-ubuntu-artifact"
DOWNLOADED_WINDOWS_ARTIFACT_NAME="musicformats-windows-artifact"

MACOS_VERSION_ORG="${DOWNLOADS_DIR}/${DOWNLOADED_MACOS_ARTIFACT_NAME}"
UBUNTU_VERSION_ORG="${DOWNLOADS_DIR}/${DOWNLOADED_UBUNTU_ARTIFACT_NAME}"
WINDOWS_VERSION_ORG="${DOWNLOADS_DIR}/${DOWNLOADED_WINDOWS_ARTIFACT_NAME}"

echo "MACOS_VERSION_ORG: ${MACOS_VERSION_ORG}"
ls -sal ${MACOS_VERSION_ORG}
echo "UBUNTU_VERSION_ORG: ${UBUNTU_VERSION_ORG}"
ls -sal ${UBUNTU_VERSION_ORG}
echo "WINDOWS_VERSION_ORG: ${WINDOWS_VERSION_ORG}"
ls -sal ${WINDOWS_VERSION_ORG}
echo
echo


echo "----------------------------------------------"
# release creation
echo "----------------------------------------------"

function CreateRelease ()
{
  VERSION_ORG_DIR_NAME="$1"
  VERSION_ORG_DIR="${DOWNLOADS_DIR}/${VERSION_ORG_DIR_NAME}"

  RELEASE_DEST_DIR_NAME="${VERSION_ORG_DIR_NAME}_${VERSION_NUMBER}"
  RELEASE_DEST_DIR="${RELEASE_DIR}/${RELEASE_DEST_DIR_NAME}"

  echo "----------------------------------------------"
  echo "Creating release for ${VERSION_ORG_DIR_NAME}, VERSION_ORG_DIR: ${VERSION_ORG_DIR}, RELEASE_DEST_DIR: ${RELEASE_DEST_DIR}"
  echo "----------------------------------------------"
  echo

  echo "VERSION_ORG_DIR: ${VERSION_ORG_DIR}"
  ls -salR ${VERSION_ORG_DIR}
  echo "RELEASE_DEST_DIR: ${RELEASE_DEST_DIR}"
  ls -salR ${RELEASE_DEST_DIR}

  cd ${RELEASE_DIR}
  echo -n "--> current directory: "; pwd
  echo

  # remove RELEASE_DEST_DIR it it exists
  [[ -d ${RELEASE_DEST_DIR} ]] & rm -r ${RELEASE_DEST_DIR}

  # create RELEASE_DEST_DIR
  mkdir ${RELEASE_DEST_DIR}

  # populate RELEASE_DEST_DIR
# set -x
  cp -p ${VERSION_ORG_DIR}/MusicFormatsVersionNumber.txt ${RELEASE_DEST_DIR}
  cp -p ${VERSION_ORG_DIR}/MusicFormatsVersionDate.txt ${RELEASE_DEST_DIR}
  cp -pr ${VERSION_ORG_DIR}/documentation/*/*.pdf ${RELEASE_DEST_DIR}
  cp -pr ${VERSION_ORG_DIR}/build/bin ${RELEASE_DEST_DIR}
  cp -pr ${VERSION_ORG_DIR}/build/lib ${RELEASE_DEST_DIR}
  cp -pr ${VERSION_ORG_DIR}/build/include ${RELEASE_DEST_DIR}
  ls -salh ${RELEASE_DEST_DIR}/*
# set +x

  echo "===> VERSION_ORG_DIR_NAME: ${VERSION_ORG_DIR_NAME},DOWNLOADED_WINDOWS_ARTIFACT_NAME: ${DOWNLOADED_WINDOWS_ARTIFACT_NAME} "

  # zip RELEASE_DEST_DIR
  RELEASE_ZIP_NAME="${RELEASE_DEST_DIR_NAME}.zip"
  echo "--> RELEASE_ZIP_NAME = ${RELEASE_ZIP_NAME}"
  RELEASE_ZIP="${RELEASE_DIR}/${RELEASE_ZIP_NAME}"
  echo "--> RELEASE_ZIP   = ${RELEASE_ZIP}"
  echo
#   zip ${RELEASE_ZIP} --recurse-paths --junk-paths ${RELEASE_DEST_DIR}
  zip ${RELEASE_ZIP} --recurse-paths ${RELEASE_DEST_DIR}
  echo

  # remove RELEASE_DEST_DIR
#   rm -r ${RELEASE_DEST_DIR}
  echo
}


echo "----------------------------------------------"
echo "==> Let's go!"
echo "----------------------------------------------"
echo

echo "----------------------------------------------"
echo "==> create the realeases:"
echo "----------------------------------------------"
echo

# CreateRelease "${DOWNLOADED_MACOS_ARTIFACT_NAME}"
# CreateRelease "${DOWNLOADED_UBUNTU_ARTIFACT_NAME}"
CreateRelease "${DOWNLOADED_WINDOWS_ARTIFACT_NAME}"
echo

echo "----------------------------------------------"
echo "==> final ${RELEASE_DIR} contents:"
echo "----------------------------------------------"
echo

echo "----------------------------------------------"
echo "==> MacOS:"
echo "----------------------------------------------"
echo
ls -salh ${RELEASE_DIR}/*macos*
ls -salh ${RELEASE_DIR}/*macos*/*
echo

echo "----------------------------------------------"
echo "==> Ubuntu:"
echo "----------------------------------------------"
echo
ls -salh ${RELEASE_DIR}/*ubuntu*
ls -salh ${RELEASE_DIR}/*ubuntu*/*
echo

echo "----------------------------------------------"
echo "==> Windows:"
echo "----------------------------------------------"
echo
ls -salh ${RELEASE_DIR}/*windows*
ls -salh ${RELEASE_DIR}/*windows*/*
echo

# set +x
