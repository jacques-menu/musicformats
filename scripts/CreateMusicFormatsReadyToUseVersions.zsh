#!/bin/zsh

#set -x

# Write all output to logfile
# -----------------------------------------

#exec > ${MUSIC_FORMATS_DEV_DIR}/$(basename $0).log 2>&1

echo


MUSIC_FORMATS_DEV_DIR=${HOME}/musicformats-git-dev
echo "--> MUSIC_FORMATS_DEV_DIR = ${MUSIC_FORMATS_DEV_DIR}"
cd ${MUSIC_FORMATS_DEV_DIR}

VERSION_NUMBER=`cat MusicFormatsVersionNumber.txt`
echo "--> VERSION_NUMBER = ${VERSION_NUMBER}"

DOWNLOADS_DIR=${HOME}/JMI_Downloads
echo "--> DOWNLOADS_DIR = ${DOWNLOADS_DIR}"
echo

READY_TO_USE_DIR=${MUSIC_FORMATS_DEV_DIR}/readytouse
echo "--> READY_TO_USE_DIR = ${READY_TO_USE_DIR}"
echo

mkdir -p ${READY_TO_USE_DIR}
ls -sal ${READY_TO_USE_DIR}
echo

DOWNLOADED_MACOS_READY_TO_USE_NAME="musicformats-macos-ready-to-use"
DOWNLOADED_UBUNTU_READY_TO_USE_NAME="musicformats-ubuntu-ready-to-use"
DOWNLOADED_WINDOWS_READY_TO_USE_NAME="musicformats-windows-ready-to-use"

MACOS_READY_TO_USE_ORG="${DOWNLOADS_DIR}/${DOWNLOADED_MACOS_READY_TO_USE_NAME}"
UBUNTU_READY_TO_USE_ORG="${DOWNLOADS_DIR}/${DOWNLOADED_UBUNTU_READY_TO_USE_NAME}"
WINDOWS_READY_TO_USE_ORG="${DOWNLOADS_DIR}/${DOWNLOADED_WINDOWS_READY_TO_USE_NAME}"
echo "MACOS_READY_TO_USE_ORG: ${MACOS_READY_TO_USE_ORG}"
ls -sal ${MACOS_READY_TO_USE_ORG}
echo "UBUNTU_READY_TO_USE_ORG: ${UBUNTU_READY_TO_USE_ORG}"
ls -sal ${UBUNTU_READY_TO_USE_ORG}
echo "WINDOWS_READY_TO_USE_ORG: ${WINDOWS_READY_TO_USE_ORG}"
ls -sal ${WINDOWS_READY_TO_USE_ORG}
echo
echo


# DOWNLOADED_READY_TO_USE="${MACOS_READY_TO_USE_ORG} ${UBUNTU_READY_TO_USE_ORG} ${WINDOWS_READY_TO_USE_ORG}"
# echo "--> DOWNLOADED_READY_TO_USE = ${DOWNLOADED_READY_TO_USE}"
# echo
# ls -sal ${DOWNLOADED_READY_TO_USE}
# echo

function CreateReadyToUseVersion ()
{
  READY_TO_USE_ORG_DIR_NAME="$1"
  READY_TO_USE_ORG_DIR="${DOWNLOADS_DIR}/${READY_TO_USE_ORG_DIR_NAME}"
  READY_TO_USE_DEST_DIR_NAME="${READY_TO_USE_ORG_DIR_NAME}-${VERSION_NUMBER}"
  READY_TO_USE_DEST_DIR="${READY_TO_USE_DIR}/${READY_TO_USE_DEST_DIR_NAME}"

  echo "----------------------------------------------"
  echo "Creating ready to use versions, READY_TO_USE_ORG_DIR: ${READY_TO_USE_ORG_DIR}, READY_TO_USE_DEST_DIR: ${READY_TO_USE_DEST_DIR}"
  echo "----------------------------------------------"
  echo

  echo "READY_TO_USE_ORG_DIR: ${READY_TO_USE_ORG_DIR}"
  ls -salR ${READY_TO_USE_ORG_DIR}
  echo "READY_TO_USE_DEST_DIR: ${READY_TO_USE_DEST_DIR}"
  ls -salR ${READY_TO_USE_DEST_DIR}

  cd ${READY_TO_USE_DIR}
  echo -n "--> current directory: "; pwd
  echo

  # remove READY_TO_USE_DEST_DIR it it exists
  [ -d ${READY_TO_USE_DEST_DIR} ] & rm -r ${READY_TO_USE_DEST_DIR}

  # create READY_TO_USE_DEST_DIR
  mkdir ${READY_TO_USE_DEST_DIR}

  # populate it
  cp -p ${READY_TO_USE_ORG_DIR}/MusicFormatsVersionNumber.txt ${READY_TO_USE_DEST_DIR}
  cp -pr ${READY_TO_USE_ORG_DIR}/documentation/*/*.pdf ${READY_TO_USE_DEST_DIR}
  cp -pr ${READY_TO_USE_ORG_DIR}/build/bin ${READY_TO_USE_DEST_DIR}
  cp -pr ${READY_TO_USE_ORG_DIR}/build/lib ${READY_TO_USE_DEST_DIR}

  # zip it
  READY_TO_USE_ZIP_NAME="${READY_TO_USE_DEST_DIR_NAME}.zip"
  echo "--> READY_TO_USE_ZIP_NAME = ${READY_TO_USE_ZIP_NAME}"
  READY_TO_USE_ZIP="${READY_TO_USE_DIR}/${READY_TO_USE_ZIP_NAME}"
  echo "--> READY_TO_USE_ZIP   = ${READY_TO_USE_ZIP}"
  echo
  zip ${READY_TO_USE_ZIP} -r ${READY_TO_USE_DEST_DIR}
  echo

  # show the result
  echo "====> ${READY_TO_USE_DEST_DIR_NAME} READY_TO_USEution contains:"
  ls -salR ${READY_TO_USE_DEST_DIR}
  echo

}


echo "----------------------------------------------"
echo "==> Let's go!"
echo "----------------------------------------------"
echo

echo "----------------------------------------------"
echo "==> create the readytouse versions:"
echo "----------------------------------------------"
echo
CreateReadyToUseVersion "${DOWNLOADED_MACOS_READY_TO_USE_NAME}"
CreateReadyToUseVersion "${DOWNLOADED_UBUNTU_READY_TO_USE_NAME}"
CreateReadyToUseVersion "${DOWNLOADED_WINDOWS_READY_TO_USE_NAME}"


echo "----------------------------------------------"
echo "==> final ${READY_TO_USE_DIR} contents:"
echo "----------------------------------------------"
echo

ls -sal ${READY_TO_USE_DIR}
#set +x
