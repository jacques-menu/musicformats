#!/bin/bash

#set -x

LIBMUSICFORMATS_GIT=${HOME}/musicformats-git-dev


# Write all output to logfile
# -----------------------------------------

#exec > ${LIBMUSICFORMATS_GIT}/$(basename $0).log 2>&1

echo


MUSIC_FORMATS_DEV=${HOME}/musicformats-git-dev

DISTRIB_DIR=${MUSIC_FORMATS_DEV}/distrib

MACOS_DISTRIB="musicformats-macos-distrib"
UBUNTU_DISTRIB="musicformats-ubuntu-distrib"
WINDOWS_DISTRIB="musicformats-windows-distrib"

DISTRIBUTIONS="${HOME}/Downloads/${MACOS_DISTRIB} ${HOME}/Downloads/${UBUNTU_DISTRIB} ${HOME}/Downloads/${WINDOWS_DISTRIB}"


function MakeMacOSDistribution ()
{
  echo "----------------------------------------------"
  echo "Making MacOS distribution"
  echo "----------------------------------------------"
  echo

  MAC_OS_DMG=MusicFormatForMacOS.dmg

  cd ${MACOS_DISTRIB}
  echo -n "--> current directory: "; pwd
  echo

  # make the executables actually executable
  chmod +x build/bin/*
  xattr -d com.apple.quarantine build/bin/*
#  ls -sal build/bin/*

  # flatten files
  [ -e bin ]; rm -r bin
  mv build/bin .
  rm -r build

  mv documentation/*/* documentation
  rmdir documentation/CommonLaTeXFiles
  rmdir documentation/IntroductionToMusicXML
  rmdir documentation/MusicFormatsCLIUserGuide

  echo "--> after files flattening:"
  echo
  ls -salR bin documentation

  cd ..
  echo

  # create the Mac OS DMG image
  [ -e ${MAC_OS_DMG} ]; rm -r ${MAC_OS_DMG}

  hdiutil create ${MAC_OS_DMG} -ov -volname "MusicFormatsForMacOS" -fs APFS -srcfolder ${MACOS_DISTRIB}
  echo

  echo "Mac OS distribution:"
  ls -sal ${MAC_OS_DMG}
}


function MakeUbuntuDistribution ()
{
  echo "----------------------------------------------"
  echo "Making Ubuntu distribution"
  echo "----------------------------------------------"
  echo

  UBUNTU_ZIP=MusicFormatForUbuntu.zip

  cd ${UBUNTU_DISTRIB}

  # make the executables actually executable
  chmod +x build/bin/*

  # flatten files
  mv build/bin .
  mv build/lib .
  rm -r build

  mv documentation/*/* documentation
  rmdir documentation/*

  echo "--> after files flattening:"
  echo
  ls -salR bin documentation

  cd ..
  echo

  # create the Ubuntu zip archive
  zip ${UBUNTU_ZIP} -r ubuntu-distrib

  echo "Ubuntu distribution:"
  ls -sal ${UBUNTU_ZIP}
}


function MakeWindowsDistribution ()
{
  echo "----------------------------------------------"
  echo "Making Windows distribution"
  echo "----------------------------------------------"
  echo

  WINDOWS_ZIP=MusicFormatForWindows.zip

  cd ${WINDOWS_DISTRIB}

  # make the executables actually executable
  chmod +x build/bin/*

  # flatten files
  mv build/bin .
  mv build/lib .
  rm -r build

  mv documentation/*/* documentation
  rmdir documentation/*

  echo "--> after files flattening:"
  echo
  ls -salR bin documentation

  cd ..
  echo

  # create the Windows zip archive
  zip ${WINDOWS_ZIP} -r windows-distrib

  echo "Windows distribution:"
  ls -sal ${WINDOWS_ZIP}
}


# Let's go!
# -----------------------------------------

cd ${DISTRIB_DIR}
echo -n "--> current directory: "; pwd
echo


echo "----------------------------------------------"
echo "distributions:"
echo "----------------------------------------------"
echo

echo "DISTRIBUTIONS = ${DISTRIBUTIONS}"
echo "DISTRIB_DIR   = ${DISTRIB_DIR}"
echo

#  ls -sal ${DISTRIBUTIONS}

# copy the distributions to the MusicFormats dev folder
#  mv -f ${DISTRIBUTIONS} ${DISTRIB_DIR}
cp -pr ${DISTRIBUTIONS} ${DISTRIB_DIR}
echo "${DISTRIB_DIR} contents:"
ls -sal ${DISTRIB_DIR}

echo; echo


MakeMacOSDistribution
echo
MakeUbuntuDistribution
echo
MakeWindowsDistribution

exit

echo "----------------------------------------------"
echo "distrib contents:"
echo "----------------------------------------------"
echo

ls -sal ${DISTRIB_DIR}/*/build/lib
echo


echo "----------------------------------------------"
echo "distrib documentation contents:"
echo "----------------------------------------------"
echo

ls -sal ${DISTRIB_DIR}/*/documentation/*/*.pdf
echo


echo "----------------------------------------------"
echo "distrib bin contents:"
echo "----------------------------------------------"
echo

ls -sal ${DISTRIB_DIR}/*/build/bin


echo "----------------------------------------------"
echo "distrib lib contents:"
echo "----------------------------------------------"
echo

ls -sal ${DISTRIB_DIR}/*/build/lib


#set +x
