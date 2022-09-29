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

RELEASE_DIR=${MUSIC_FORMATS_DEV_DIR}/releases
echo "--> RELEASE_DIR = ${RELEASE_DIR}"
echo

mkdir -p ${RELEASE_DIR}
ls -sal ${RELEASE_DIR}
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


function CreateRelease ()
{
  VERSION_ORG_DIR_NAME="$1"
  VERSION_ORG_DIR="${DOWNLOADS_DIR}/${VERSION_ORG_DIR_NAME}"

#   RELEASE_DEST_DIR_NAME="${VERSION_ORG_DIR_NAME}-${VERSION_NUMBER}"
  RELEASE_DEST_DIR_NAME="${VERSION_ORG_DIR_NAME}_${VERSION_NUMBER}"
  RELEASE_DEST_DIR="${RELEASE_DIR}/${RELEASE_DEST_DIR_NAME}"

  echo "----------------------------------------------"
  echo "Creating ready to use versions, VERSION_ORG_DIR: ${VERSION_ORG_DIR}, RELEASE_DEST_DIR: ${RELEASE_DEST_DIR}"
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
  [ -d ${RELEASE_DEST_DIR} ] & rm -r ${RELEASE_DEST_DIR}

  # create RELEASE_DEST_DIR
  mkdir ${RELEASE_DEST_DIR}

  # populate it
  cp -p ${VERSION_ORG_DIR}/MusicFormatsVersionNumber.txt ${RELEASE_DEST_DIR}
  cp -p ${VERSION_ORG_DIR}/MusicFormatsVersionDate.txt ${RELEASE_DEST_DIR}
  cp -pr ${VERSION_ORG_DIR}/documentation/*/*.pdf ${RELEASE_DEST_DIR}
  cp -pr ${VERSION_ORG_DIR}/build/bin ${RELEASE_DEST_DIR}
  cp -pr ${VERSION_ORG_DIR}/build/lib ${RELEASE_DEST_DIR}

  echo "===> VERSION_ORG_DIR_NAME: ${VERSION_ORG_DIR_NAME},DOWNLOADED_WINDOWS_VERSION_NAME: ${DOWNLOADED_WINDOWS_VERSION_NAME} "
  if [[ ${VERSION_ORG_DIR_NAME} == ${DOWNLOADED_WINDOWS_VERSION_NAME} ]]; then
    # pick libmusicformats.dll from the libdir subdirectory and
    # copy it to the bin directory to simplify launch without installing on Windows
    MUSICFORMATS_DLL=${VERSION_ORG_DIR}/build/lib/libmusicformats.dll
    echo "===> MUSICFORMATS_DLL:"
    ls -sal ${MUSICFORMATS_DLL}

    cp -pr ${MUSICFORMATS_DLL} ${RELEASE_DEST_DIR}/bin
  fi

  # zip it
  RELEASE_ZIP_NAME="${RELEASE_DEST_DIR_NAME}.zip"
  echo "--> RELEASE_ZIP_NAME = ${RELEASE_ZIP_NAME}"
  RELEASE_ZIP="${RELEASE_DIR}/${RELEASE_ZIP_NAME}"
  echo "--> RELEASE_ZIP   = ${RELEASE_ZIP}"
  echo
  zip ${RELEASE_ZIP} --recurse-paths --junk-paths ${RELEASE_DEST_DIR}
  echo

  # show the result
  echo "====> ${RELEASE_DEST_DIR_NAME} RELEASEution contains:"
  ls -salR ${RELEASE_DEST_DIR}
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
# CreateRelease "${DOWNLOADED_MACOS_VERSION_NAME}"
# CreateRelease "${DOWNLOADED_UBUNTU_VERSION_NAME}"
CreateRelease "${DOWNLOADED_WINDOWS_VERSION_NAME}"


echo "----------------------------------------------"
echo "==> final ${RELEASE_DIR} contents:"
echo "----------------------------------------------"
echo

ls -sal ${RELEASE_DIR}
#set +x
