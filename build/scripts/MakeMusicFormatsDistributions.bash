#!/bin/bash

#set -x

LIBMUSICFORMATS_GIT=${HOME}/musicformats-git-dev


# Write all output to logfile
# -----------------------------------------

#exec > ${LIBMUSICFORMATS_GIT}/$(basename $0).log 2>&1

echo


MUSIC_FORMATS_DEV=${HOME}/musicformats-git-dev

DISTRIB_DIR=${MUSIC_FORMATS_DEV}/distrib

DOWNLOADED_MACOS_DISTRIB_NAME="/musicformats-macos-distrib"
DOWNLOADED_UBUNTU_DISTRIB_NAME="/musicformats-ubuntu-distrib"
DOWNLOADED_WINDOWS_DISTRIB_NAME="/musicformats-windows-distrib"

MACOS_DISTRIB_ORG="${DISTRIB_DIR}/musicformats-macos-distrib"
UBUNTU_DISTRIB_ORG="${DISTRIB_DIR}/musicformats-ubuntu-distrib"
WINDOWS_DISTRIB_ORG="${DISTRIB_DIR}/musicformats-windows-distrib"

DOWNLOADED_DISTRIBUTIONS="${HOME}/Downloads/${DOWNLOADED_MACOS_DISTRIB_NAME} ${HOME}/Downloads/${DOWNLOADED_UBUNTU_DISTRIB_NAME}   ${HOME}/Downloads/${DOWNLOADED_WINDOWS_DISTRIB_NAME}"


function MakeDocumentationDistribution ()
{
  echo "----------------------------------------------"
  echo "Making documentation distribution"
  echo "----------------------------------------------"
  echo

  echo -n "--> current directory: "; pwd
  echo

   # copy documentation files from MACOS_DISTRIB_ORG to DISTRIB_DIR
  cp -p ${MACOS_DISTRIB_ORG}/MusicFormatsVersionNumber.txt ${DISTRIB_DIR}

  cp -p ${MACOS_DISTRIB_ORG}/documentation/*/*.pdf ${DISTRIB_DIR}

  # create the Mac OS documentation zip archive
#  zip MAC_OS_ZIP -r ${MAC_OS_DMG}

  echo "Mac OS documentation distribution:"
  ls -sal ${DISTRIB_DIR}
}

function MakeMacOSDistribution ()
{
  echo "----------------------------------------------"
  echo "Making MacOS distribution"
  echo "----------------------------------------------"
  echo

  echo "--> MACOS_DISTRIB_ORG   = ${MACOS_DISTRIB_ORG}"
  echo

  MAC_OS_DMG_NAME="MusicFormatsForMacOS"

  MAC_OS_DMG="${DISTRIB_DIR}/${MAC_OS_DMG_NAME}.dmg"
  echo "--> MAC_OS_DMG   = ${MAC_OS_DMG}"
  echo

  MAC_OS_ZIP="${DISTRIB_DIR}/MusicFormatForMacOS.zip"
  echo "--> MAC_OS_DMG   = ${MAC_OS_DMG}"
  echo

  echo -n "--> current directory: "; pwd
  echo

  MACOS_DISTRIB_WORK="${DISTRIB_DIR}/MusicFormatForMacOS"
  echo "--> MACOS_DISTRIB_WORK   = ${MACOS_DISTRIB_WORK}"
  echo

  [ -e ${MACOS_DISTRIB_WORK} ]; rm -r ${MACOS_DISTRIB_WORK}

  mkdir ${MACOS_DISTRIB_WORK}

  echo "--> ${MACOS_DISTRIB_WORK} initial contents:"
  ls -sal ${MACOS_DISTRIB_WORK}
  echo

  cd ${MACOS_DISTRIB_WORK}
  echo -n "--> current directory: "; pwd
  echo

  # copy files from MACOS_DISTRIB_ORG to MACOS_DISTRIB_WORK
  cp -p ${MACOS_DISTRIB_ORG}/MusicFormatsVersionNumber.txt ${MACOS_DISTRIB_WORK}

  cp -pr ${MACOS_DISTRIB_ORG}/build/bin ${MACOS_DISTRIB_WORK}

#  mkdir ${MACOS_DISTRIB_WORK}/documentation
#  cp -p ${MACOS_DISTRIB_ORG}/documentation/*/*.pdf ${MACOS_DISTRIB_WORK}/documentation

  echo "--> after files copy:"
  echo
  ls -salR
  echo

  # make the executables actually executable
  chmod +x bin/*
  xattr -d com.apple.quarantine bin/*

  echo "--> after making the executables actually executable:"
  echo
  ls -salR
  echo

  # check executability
  bin/xml2ly -contact
  echo

  # create the Mac OS DMG image
  cd ${DISTRIB_DIR}
  echo -n "--> current directory: "; pwd
  echo

  [ -e ${MAC_OS_DMG} ]; rm -r ${MAC_OS_DMG}

  hdiutil create ${MAC_OS_DMG} -ov -volname ${MAC_OS_DMG_NAME} -fs APFS -srcfolder ${MACOS_DISTRIB_WORK}
  echo

  echo "Mac OS distribution DMG:"
  ls -sal ${MAC_OS_ZIP}

  # create the Mac OS zip archive
  zip MAC_OS_ZIP -r ${MAC_OS_DMG}

  echo "Mac OS distribution:"
  ls -sal ${MAC_OS_ZIP}
}


function MakeUbuntuDistribution ()
{
  echo "----------------------------------------------"
  echo "Making Ubuntu distribution"
  echo "----------------------------------------------"
  echo

  echo "--> UBUNTU_DISTRIB_ORG   = ${UBUNTU_DISTRIB_ORG}"
  echo

  UBUNTU_ZIP="${DISTRIB_DIR}/MusicFormatForUbuntu.zip"
  echo "--> UBUNTU_ZIP   = ${UBUNTU_ZIP}"
  echo

  echo -n "--> current directory: "; pwd
  echo

  UBUNTU_DISTRIB_WORK="${DISTRIB_DIR}/MusicFormatForUbuntu"
  echo "--> UBUNTU_DISTRIB_WORK   = ${UBUNTU_DISTRIB_WORK}"
  echo

  [ -e ${UBUNTU_DISTRIB_WORK} ]; rm -r ${UBUNTU_DISTRIB_WORK}

  mkdir ${UBUNTU_DISTRIB_WORK}

  echo "--> ${UBUNTU_DISTRIB_WORK} initial contents:"
  ls -sal ${UBUNTU_DISTRIB_WORK}
  echo

  cd ${UBUNTU_DISTRIB_WORK}
  echo -n "--> current directory: "; pwd
  echo

  # copy files from UBUNTU_DISTRIB_ORG to UBUNTU_DISTRIB_WORK
  cp -p ${UBUNTU_DISTRIB_ORG}/MusicFormatsVersionNumber.txt ${UBUNTU_DISTRIB_WORK}

  cp -pr ${UBUNTU_DISTRIB_ORG}/build/bin ${UBUNTU_DISTRIB_WORK}
  cp -pr ${UBUNTU_DISTRIB_ORG}/build/lib ${UBUNTU_DISTRIB_WORK}

#  mkdir ${UBUNTU_DISTRIB_WORK}/documentation
#  cp -p ${UBUNTU_DISTRIB_ORG}/documentation/*/*.pdf ${UBUNTU_DISTRIB_WORK}/documentation

  echo "--> after files copy:"
  echo
  ls -salR
  echo

  # create the Ubuntu zip archive
  cd ${DISTRIB_DIR}
  echo -n "--> current directory: "; pwd
  echo

  zip ${UBUNTU_DISTRIB_WORK} -r ${UBUNTU_ZIP}

  echo "Ubuntu distribution:"
  ls -sal ${UBUNTU_ZIP}
}


function MakeWindowsDistribution ()
{
  echo "----------------------------------------------"
  echo "Making Windows distribution"
  echo "----------------------------------------------"
  echo

  echo "--> WINDOWS_DISTRIB_ORG   = ${WINDOWS_DISTRIB_ORG}"
  echo

  WINDOWS_ZIP="${DISTRIB_DIR}/MusicFormatForWindows.zip"
  echo "--> WINDOWS_ZIP   = ${WINDOWS_ZIP}"
  echo

  echo -n "--> current directory: "; pwd
  echo

  WINDOWS_DISTRIB_WORK="${DISTRIB_DIR}/MusicFormatForWindows"
  echo "--> WINDOWS_DISTRIB_WORK   = ${WINDOWS_DISTRIB_WORK}"
  echo

  [ -e ${WINDOWS_DISTRIB_WORK} ]; rm -r ${WINDOWS_DISTRIB_WORK}

  mkdir ${WINDOWS_DISTRIB_WORK}

  echo "--> ${WINDOWS_DISTRIB_WORK} initial contents:"
  ls -sal ${WINDOWS_DISTRIB_WORK}
  echo

  cd ${WINDOWS_DISTRIB_WORK}
  echo -n "--> current directory: "; pwd
  echo

  # copy files from WINDOWS_DISTRIB_ORG to WINDOWS_DISTRIB_WORK
  cp -p ${WINDOWS_DISTRIB_ORG}/MusicFormatsVersionNumber.txt ${WINDOWS_DISTRIB_WORK}

  cp -pr ${WINDOWS_DISTRIB_ORG}/build/bin ${WINDOWS_DISTRIB_WORK}
  cp -pr ${WINDOWS_DISTRIB_ORG}/build/lib ${WINDOWS_DISTRIB_WORK}

#  mkdir ${WINDOWS_DISTRIB_WORK}/documentation
#  cp -p ${WINDOWS_DISTRIB_ORG}/documentation/*/*.pdf ${WINDOWS_DISTRIB_WORK}/documentation

  echo "--> after files copy:"
  echo
  ls -salR
  echo

  # create the Windows zip archive
  cd ${DISTRIB_DIR}
  echo -n "--> current directory: "; pwd
  echo

  zip ${WINDOWS_DISTRIB_WORK} -r ${WINDOWS_ZIP}

  echo "Windows distribution:"
  ls -sal ${WINDOWS_ZIP}
}


# Let's go!
# -----------------------------------------

cd ${DISTRIB_DIR}
echo -n "--> current directory: "; pwd
echo


echo "----------------------------------------------"
echo "downloaded distributions:"
echo "----------------------------------------------"
echo

echo "--> DOWNLOADED_DISTRIBUTIONS = ${DOWNLOADED_DISTRIBUTIONS}"
echo
ls -sal ${DOWNLOADED_DISTRIBUTIONS}
echo

echo "----------------------------------------------"
echo "distrib directory:"
echo "----------------------------------------------"
echo

echo "--> DISTRIB_DIR   = ${DISTRIB_DIR}"
echo

echo "--> ${DISTRIB_DIR} initial contents:"
ls -sal ${DISTRIB_DIR}
echo

# copy the distributions to the MusicFormats distrib folder
cp -pr ${DOWNLOADED_DISTRIBUTIONS} ${DISTRIB_DIR}

echo "--> ${DISTRIB_DIR} contents after copy:"
ls -sal ${DISTRIB_DIR}
echo; echo


echo "----------------------------------------------"
echo "make the distributions:"
echo "----------------------------------------------"
echo

MakeDocumentationDistribution
echo

MakeMacOSDistribution
echo

MakeUbuntuDistribution
echo

MakeWindowsDistribution

echo "----------------------------------------------"
echo "distrib contents:"
echo "----------------------------------------------"
echo

ls -salt ${DISTRIB_DIR}/*.txt ${DISTRIB_DIR}/*.pdf ${DISTRIB_DIR}/MusicFormatFor*
echo


#set +x
