#!/bin/zsh

#set -x

# Write all output to logfile
# -----------------------------------------

#exec > ${MUSIC_FORMATS_DEV_DIR}/$(basename $0).log 2>&1


MUSIC_FORMATS_DEV_DIR=${HOME}/musicformats-git-dev
echo "--> MUSIC_FORMATS_DEV_DIR = ${MUSIC_FORMATS_DEV_DIR}"
cd ${MUSIC_FORMATS_DEV_DIR}

VERSION_NUMBER=`cat MusicFormatsVersionNumber.txt | tr '.' '_'`
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

DOWNLOADED_MACOS_VERSION_NAME="musicformats-macos-version"
DOWNLOADED_UBUNTU_VERSION_NAME="musicformats-ubuntu-version"
DOWNLOADED_WINDOWS_VERSION_NAME="musicformats-windows-version"

MACOS_VERSION_ORG="${DOWNLOADS_DIR}/${DOWNLOADED_MACOS_VERSION_NAME}"
UBUNTU_VERSION_ORG="${DOWNLOADS_DIR}/${DOWNLOADED_UBUNTU_VERSION_NAME}"
WINDOWS_VERSION_ORG="${DOWNLOADS_DIR}/${DOWNLOADED_WINDOWS_VERSION_NAME}"

echo "MACOS_VERSION_ORG: ${MACOS_VERSION_ORG}"
ls -sal ${MACOS_VERSION_ORG}
echo "UBUNTU_VERSION_ORG: ${UBUNTU_VERSION_ORG}"
ls -sal ${UBUNTU_VERSION_ORG}
echo "WINDOWS_VERSION_ORG: ${WINDOWS_VERSION_ORG}"
ls -sal ${WINDOWS_VERSION_ORG}
echo
echo


function CreateReadyToUseVersion ()
{
  VERSION_ORG_DIR_NAME="$1"
  VERSION_ORG_DIR="${DOWNLOADS_DIR}/${VERSION_ORG_DIR_NAME}"

#   READY_TO_USE_DEST_DIR_NAME="${VERSION_ORG_DIR_NAME}-${VERSION_NUMBER}"
  READY_TO_USE_DEST_DIR_NAME="${VERSION_ORG_DIR_NAME}_${VERSION_NUMBER}"
  READY_TO_USE_DEST_DIR="${READY_TO_USE_DIR}/${READY_TO_USE_DEST_DIR_NAME}"

  echo "----------------------------------------------"
  echo "Creating ready to use versions, VERSION_ORG_DIR: ${VERSION_ORG_DIR}, READY_TO_USE_DEST_DIR: ${READY_TO_USE_DEST_DIR}"
  echo "----------------------------------------------"
  echo

  echo "VERSION_ORG_DIR: ${VERSION_ORG_DIR}"
  ls -salR ${VERSION_ORG_DIR}
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
  cp -p ${VERSION_ORG_DIR}/MusicFormatsVersionNumber.txt ${READY_TO_USE_DEST_DIR}
  cp -p ${VERSION_ORG_DIR}/MusicFormatsVersionDate.txt ${READY_TO_USE_DEST_DIR}
  cp -pr ${VERSION_ORG_DIR}/documentation/*/*.pdf ${READY_TO_USE_DEST_DIR}
  cp -pr ${VERSION_ORG_DIR}/build/bin ${READY_TO_USE_DEST_DIR}
  cp -pr ${VERSION_ORG_DIR}/build/lib ${READY_TO_USE_DEST_DIR}

  # zip it
  READY_TO_USE_ZIP_NAME="${READY_TO_USE_DEST_DIR_NAME}.zip"
  echo "--> READY_TO_USE_ZIP_NAME = ${READY_TO_USE_ZIP_NAME}"
  READY_TO_USE_ZIP="${READY_TO_USE_DIR}/${READY_TO_USE_ZIP_NAME}"
  echo "--> READY_TO_USE_ZIP   = ${READY_TO_USE_ZIP}"
  echo
  zip ${READY_TO_USE_ZIP} --recurse-paths --junk-paths ${READY_TO_USE_DEST_DIR}
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
CreateReadyToUseVersion "${DOWNLOADED_MACOS_VERSION_NAME}"
CreateReadyToUseVersion "${DOWNLOADED_UBUNTU_VERSION_NAME}"
CreateReadyToUseVersion "${DOWNLOADED_WINDOWS_VERSION_NAME}"


echo "----------------------------------------------"
echo "==> final ${READY_TO_USE_DIR} contents:"
echo "----------------------------------------------"
echo

ls -sal ${READY_TO_USE_DIR}
#set +x
