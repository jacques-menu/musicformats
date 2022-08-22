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

DISTRIBUTIONS_DIR=${MUSIC_FORMATS_DEV_DIR}/distributions
echo "--> DISTRIBUTIONS_DIR = ${DISTRIBUTIONS_DIR}"
echo

mkdir -p ${DISTRIBUTIONS_DIR}
ls -sal ${DISTRIBUTIONS_DIR}
echo

DOWNLOADED_MACOS_DISTRIB_NAME="musicformats-macos-distrib"
DOWNLOADED_UBUNTU_DISTRIB_NAME="musicformats-ubuntu-distrib"
DOWNLOADED_WINDOWS_DISTRIB_NAME="musicformats-windows-distrib"

MACOS_DISTRIB_ORG="${DOWNLOADS_DIR}/${DOWNLOADED_MACOS_DISTRIB_NAME}"
UBUNTU_DISTRIB_ORG="${DOWNLOADS_DIR}/${DOWNLOADED_UBUNTU_DISTRIB_NAME}"
WINDOWS_DISTRIB_ORG="${DOWNLOADS_DIR}/${DOWNLOADED_WINDOWS_DISTRIB_NAME}"
echo "MACOS_DISTRIB_ORG: ${MACOS_DISTRIB_ORG}"
ls -sal ${MACOS_DISTRIB_ORG}
echo "UBUNTU_DISTRIB_ORG: ${UBUNTU_DISTRIB_ORG}"
ls -sal ${UBUNTU_DISTRIB_ORG}
echo "WINDOWS_DISTRIB_ORG: ${WINDOWS_DISTRIB_ORG}"
ls -sal ${WINDOWS_DISTRIB_ORG}
echo
echo


# DOWNLOADED_DISTRIBUTIONS="${MACOS_DISTRIB_ORG} ${UBUNTU_DISTRIB_ORG} ${WINDOWS_DISTRIB_ORG}"
# echo "--> DOWNLOADED_DISTRIBUTIONS = ${DOWNLOADED_DISTRIBUTIONS}"
# echo
# ls -sal ${DOWNLOADED_DISTRIBUTIONS}
# echo

function CreateDistribution ()
{
  DISTRIB_ORG_DIR_NAME="$1"
  DISTRIB_ORG_DIR="${DOWNLOADS_DIR}/${DISTRIB_ORG_DIR_NAME}"
  DISTRIB_DEST_DIR_NAME="${DISTRIB_ORG_DIR_NAME}-${VERSION_NUMBER}"
  DISTRIB_DEST_DIR="${DISTRIBUTIONS_DIR}/${DISTRIB_DEST_DIR_NAME}"

  echo "----------------------------------------------"
  echo "Creating documentation distribution, DISTRIB_ORG_DIR: ${DISTRIB_ORG_DIR}, DISTRIB_DEST_DIR: ${DISTRIB_DEST_DIR}"
  echo "----------------------------------------------"
  echo

  echo "DISTRIB_ORG_DIR: ${DISTRIB_ORG_DIR}"
  ls -salR ${DISTRIB_ORG_DIR}
  echo "DISTRIB_DEST_DIR: ${DISTRIB_DEST_DIR}"
  ls -salR ${DISTRIB_DEST_DIR}

  cd ${DISTRIBUTIONS_DIR}
  echo -n "--> current directory: "; pwd
  echo

  # remove DISTRIB_DEST_DIR it it exists
  [ -d ${DISTRIB_DEST_DIR} ] & rm -r ${DISTRIB_DEST_DIR}

  # create DISTRIB_DEST_DIR
  mkdir ${DISTRIB_DEST_DIR}

  # populate it
  cp -p ${DISTRIB_ORG_DIR}/MusicFormatsVersionNumber.txt ${DISTRIB_DEST_DIR}
  cp -pr ${DISTRIB_ORG_DIR}/documentation/*/*.pdf ${DISTRIB_DEST_DIR}
  cp -pr ${DISTRIB_ORG_DIR}/build/bin ${DISTRIB_DEST_DIR}
  cp -pr ${DISTRIB_ORG_DIR}/build/lib ${DISTRIB_DEST_DIR}

  # zip it
  DISTRIB_ZIP_NAME="${DISTRIB_DEST_DIR_NAME}.zip"
  echo "--> DISTRIB_ZIP_NAME = ${DISTRIB_ZIP_NAME}"
  DISTRIB_ZIP="${DISTRIBUTIONS_DIR}/${DISTRIB_ZIP_NAME}"
  echo "--> DISTRIB_ZIP   = ${DISTRIB_ZIP}"
  echo
  zip ${DISTRIB_ZIP} -r ${DISTRIB_DEST_DIR}
  echo

  # show the result
  echo "====> ${DISTRIB_DEST_DIR_NAME} distribution contains:"
  ls -salR ${DISTRIB_DEST_DIR}
  echo

}


echo "----------------------------------------------"
echo "==> Let's go!"
echo "----------------------------------------------"
echo

echo "----------------------------------------------"
echo "==> create the distributions:"
echo "----------------------------------------------"
echo
CreateDistribution "${DOWNLOADED_MACOS_DISTRIB_NAME}"
CreateDistribution "${DOWNLOADED_UBUNTU_DISTRIB_NAME}"
CreateDistribution "${DOWNLOADED_WINDOWS_DISTRIB_NAME}"


echo "----------------------------------------------"
echo "==> final ${DISTRIBUTIONS_DIR} contents:"
echo "----------------------------------------------"
echo

ls -sal ${DISTRIBUTIONS_DIR}
#set +x
