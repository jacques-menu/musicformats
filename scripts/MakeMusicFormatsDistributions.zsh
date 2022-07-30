#!/bin/zsh

#set -x

LIBMUSICFORMATS_GIT=${HOME}/musicformats-git-dev


# Write all output to logfile
# -----------------------------------------

#exec > ${LIBMUSICFORMATS_GIT}/$(basename $0).log 2>&1

echo


MUSIC_FORMATS_DEV=${HOME}/musicformats-git-dev

DISTRIB_DIR=${MUSIC_FORMATS_DEV}/distrib
echo "--> ${DISTRIB_DIR} = ${DISTRIB_DIR}"
echo

mkdir -p ${DISTRIB_DIR}
ls -sal ${DISTRIB_DIR}
echo

DOWNLOADED_MACOS_DISTRIB_NAME="musicformats-macos-distrib"
DOWNLOADED_UBUNTU_DISTRIB_NAME="musicformats-ubuntu-distrib"
DOWNLOADED_WINDOWS_DISTRIB_NAME="musicformats-windows-distrib"

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

  cd ${DISTRIB_DIR}
  echo -n "--> current directory: "; pwd
  echo

  echo "--> copy documentation files from ${MACOS_DISTRIB_ORG} to ${DISTRIB_DIR}"
  cp -p ${MACOS_DISTRIB_ORG}/MusicFormatsVersionNumber.txt ${DISTRIB_DIR}

  cp -p ${MACOS_DISTRIB_ORG}/documentation/*/*.pdf ${DISTRIB_DIR}

  echo "Mac OS documentation distribution:"
  ls -sal ${DISTRIB_DIR}
  echo
}


function MakeMacOSDistribution ()
{
  echo "----------------------------------------------"
  echo "Making MacOS distribution"
  echo "----------------------------------------------"
  echo

  cd ${DISTRIB_DIR}
  echo -n "--> current directory: "; pwd
  echo

  echo "--> MACOS_DISTRIB_ORG   = ${MACOS_DISTRIB_ORG}"
  echo

#  MAC_OS_DMG_NAME="MusicFormatsForMacOS"

#  MAC_OS_DMG="${DISTRIB_DIR}/${MAC_OS_DMG_NAME}.dmg"
#  echo "--> MAC_OS_DMG   = ${MAC_OS_DMG}"
#  echo

  MAC_OS_ZIP_NAME="MusicFormatsForMacOS.zip"
  echo "--> MAC_OS_ZIP_NAME   = ${MAC_OS_ZIP_NAME}"
  echo

  MAC_OS_ZIP="${DISTRIB_DIR}/${MAC_OS_ZIP_NAME}"
  echo "--> MAC_OS_ZIP_NAME   = ${MAC_OS_ZIP_NAME}"
  echo

  MACOS_DISTRIB_WORK_NAME="MusicFormatsForMacOS"
  echo "--> MACOS_DISTRIB_WORK_NAME   = ${MACOS_DISTRIB_WORK_NAME}"
  echo

  MACOS_DISTRIB_WORK="${DISTRIB_DIR}/${MACOS_DISTRIB_WORK_NAME}"
  echo "--> MACOS_DISTRIB_WORK   = ${MACOS_DISTRIB_WORK}"
  echo

set -x
  echo "--> make sure ${MACOS_DISTRIB_WORK_NAME} is empty"
  [ -e ${MACOS_DISTRIB_WORK_NAME} ]; rm -r ${MACOS_DISTRIB_WORK_NAME}
set +x

  mkdir ${MACOS_DISTRIB_WORK_NAME}
  echo

  echo "--> ${MACOS_DISTRIB_WORK} initial contents:"
  ls -sal ${MACOS_DISTRIB_WORK}
  echo

  echo "--> cd ${MACOS_DISTRIB_WORK}"
  cd ${MACOS_DISTRIB_WORK}
  echo -n "--> current directory: "; pwd
  echo

  echo "--> copy files from ${MACOS_DISTRIB_ORG_NAME} to ${MACOS_DISTRIB_WORK_NAME}"
  cp -p ${MACOS_DISTRIB_ORG}/MusicFormatsVersionNumber.txt ${MACOS_DISTRIB_WORK}

  cp -pr ${MACOS_DISTRIB_ORG}/build/bin ${MACOS_DISTRIB_WORK}
  echo

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

  echo "--> create the Mac OS zip archive"
  cd ${DISTRIB_DIR}
  echo -n "--> current directory: "; pwd
  echo

  zip ${MAC_OS_ZIP_NAME} -r ${MACOS_DISTRIB_WORK_NAME}
  echo

  echo "Mac OS distribution zip:"
  ls -sal ${MAC_OS_ZIP}
  echo

#  echo "--> create the Mac OS DMG"
#  [ -e ${MAC_OS_DMG} ]; rm -r ${MAC_OS_DMG}

#  hdiutil create ${MAC_OS_DMG} -ov -volname ${MAC_OS_DMG_NAME} -fs APFS -srcfolder ${MAC_OS_ZIP}
#  echo
#
#  echo "Mac OS distribution DMG:"
#  ls -sal ${MAC_OS_DMG}
}


function MakeUbuntuDistribution ()
{
  echo "----------------------------------------------"
  echo "Making Ubuntu distribution"
  echo "----------------------------------------------"
  echo

  cd ${DISTRIB_DIR}
  echo -n "--> current directory: "; pwd
  echo

  echo "--> UBUNTU_DISTRIB_ORG   = ${UBUNTU_DISTRIB_ORG}"
  echo

  UBUNTU_ZIP_NAME="MusicFormatsForUbuntu.zip"
  echo "--> UBUNTU_ZIP_NAME   = ${UBUNTU_ZIP_NAME}"
  echo

  UBUNTU_ZIP="${DISTRIB_DIR}/${DISTRIB_DIR}"
  echo "--> UBUNTU_ZIP   = ${UBUNTU_ZIP_NAME}"
  echo

  UBUNTU_DISTRIB_WORK_NAME="MusicFormatsForUbuntu"
  echo "--> UBUNTU_DISTRIB_WORK_NAME   = ${UBUNTU_DISTRIB_WORK_NAME}"
  echo

  UBUNTU_DISTRIB_WORK="${DISTRIB_DIR}/${UBUNTU_DISTRIB_WORK_NAME}"
  echo "--> UBUNTU_DISTRIB_WORK   = ${UBUNTU_DISTRIB_WORK}"
  echo

  echo "--> make sure ${UBUNTU_DISTRIB_WORK_NAME} is empty"
  [ -e ${UBUNTU_DISTRIB_WORK_NAME} ]; rm -r ${UBUNTU_DISTRIB_WORK_NAME}

  mkdir ${UBUNTU_DISTRIB_WORK_NAME}
  echo

  echo "--> ${UBUNTU_DISTRIB_WORK_NAME} initial contents:"
  ls -sal ${UBUNTU_DISTRIB_WORK_NAME}
  echo

  echo "--> cd ${UBUNTU_DISTRIB_WORK}"
  cd ${UBUNTU_DISTRIB_WORK}
  echo -n "--> current directory: "; pwd
  echo

  echo "--> copy files from ${UBUNTU_DISTRIB_ORG_NAME} to ${UBUNTU_DISTRIB_WORK_NAME}"
  cp -p ${UBUNTU_DISTRIB_ORG}/MusicFormatsVersionNumber.txt ${UBUNTU_DISTRIB_WORK}

  cp -pr ${UBUNTU_DISTRIB_ORG}/build/bin ${UBUNTU_DISTRIB_WORK}
  cp -pr ${UBUNTU_DISTRIB_ORG}/build/lib ${UBUNTU_DISTRIB_WORK}
  echo

  echo "--> after files copy:"
  echo
  ls -salR
  echo

  echo "--> create the Ubuntu zip archive"
  cd ${DISTRIB_DIR}
  echo -n "--> current directory: "; pwd
  echo

  zip ${UBUNTU_ZIP_NAME} -r ${UBUNTU_DISTRIB_WORK_NAME}
  echo

  echo "Ubuntu distribution:"
  ls -sal ${UBUNTU_ZIP}
  echo
}


function MakeWindowsDistribution ()
{
  echo "----------------------------------------------"
  echo "Making Windows distribution"
  echo "----------------------------------------------"
  echo

  cd ${DISTRIB_DIR}
  echo -n "--> current directory: "; pwd
  echo

  echo "--> WINDOWS_DISTRIB_ORG   = ${WINDOWS_DISTRIB_ORG}"
  echo

  WINDOWS_ZIP_NAME="MusicFormatsForWindows.zip"
  echo "--> WINDOWS_ZIP_NAME   = ${WINDOWS_ZIP_NAME}"
  echo

  WINDOWS_ZIP="${DISTRIB_DIR}/${WINDOWS_ZIP_NAME}"
  echo "--> WINDOWS_ZIP   = ${WINDOWS_ZIP}"
  echo

  WINDOWS_DISTRIB_WORK_NAME="MusicFormatsForWindows"
  echo "--> WINDOWS_DISTRIB_WORK_NAME   = ${WINDOWS_DISTRIB_WORK_NAME}"
  echo

  WINDOWS_DISTRIB_WORK="${DISTRIB_DIR}/${WINDOWS_DISTRIB_WORK_NAME}"
  echo "--> WINDOWS_DISTRIB_WORK   = ${WINDOWS_DISTRIB_WORK}"
  echo

  echo "--> make sure ${WINDOWS_DISTRIB_WORK_NAME} is empty"
  [ -e ${WINDOWS_DISTRIB_WORK_NAME} ]; rm -r ${WINDOWS_DISTRIB_WORK_NAME}

  mkdir ${WINDOWS_DISTRIB_WORK_NAME}
  echo

  echo "--> ${WINDOWS_DISTRIB_WORK_NAME} initial contents:"
  ls -sal ${WINDOWS_DISTRIB_WORK_NAME}
  echo

  echo "--> cd ${WINDOWS_DISTRIB_WORK}"
  cd ${WINDOWS_DISTRIB_WORK}
  echo -n "--> current directory: "; pwd
  echo

  echo "--> copy files from ${WINDOWS_DISTRIB_ORG_NAME} to ${WINDOWS_DISTRIB_WORK_NAME}"
  cp -p ${WINDOWS_DISTRIB_ORG}/MusicFormatsVersionNumber.txt ${WINDOWS_DISTRIB_WORK}

  cp -pr ${WINDOWS_DISTRIB_ORG}/build/bin ${WINDOWS_DISTRIB_WORK}
  cp -pr ${WINDOWS_DISTRIB_ORG}/build/lib ${WINDOWS_DISTRIB_WORK}
  echo

  echo "--> after files copy:"
  echo
  ls -salR
  echo

  echo "--> create the Windows zip archive"
  cd ${DISTRIB_DIR}
  echo -n "--> current directory: "; pwd
  echo

  zip ${WINDOWS_ZIP_NAME} -r ${WINDOWS_DISTRIB_WORK_NAME}
  echo

  echo "Windows distribution:"
  ls -sal ${WINDOWS_ZIP}
  echo
}


echo "----------------------------------------------"
echo "==> Let's go!"
echo "----------------------------------------------"
echo

cd ${DISTRIB_DIR}
echo -n "--> current directory: "; pwd
echo

echo "----------------------------------------------"
echo "==> downloaded distributions:"
echo "----------------------------------------------"
echo

echo "--> DOWNLOADED_DISTRIBUTIONS = ${DOWNLOADED_DISTRIBUTIONS}"
echo
ls -sal ${DOWNLOADED_DISTRIBUTIONS}
echo

echo "----------------------------------------------"
echo "==> distrib directory:"
echo "----------------------------------------------"
echo

echo "--> DISTRIB_DIR   = ${DISTRIB_DIR}"
echo

echo "--> initial ${DISTRIB_DIR} contents:"
ls -sal ${DISTRIB_DIR}
echo

echo "--> copy the distributions to the MusicFormats distrib folder"
cp -pr ${DOWNLOADED_DISTRIBUTIONS} ${DISTRIB_DIR}
echo

echo "--> ${DISTRIB_DIR} contents after copy:"
ls -sal ${DISTRIB_DIR}
echo; echo


echo "----------------------------------------------"
echo "==> make the distributions:"
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
echo "==> final distrib contents:"
echo "----------------------------------------------"
echo

ls -salt ${DISTRIB_DIR}/*.txt ${DISTRIB_DIR}/*.pdf ${DISTRIB_DIR}/MusicFormatsFor*


#set +x
