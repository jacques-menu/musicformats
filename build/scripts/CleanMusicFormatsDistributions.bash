#!/bin/bash

#set -x

LIBMUSICFORMATS_GIT=${HOME}/musicformats-git-dev


# Write all output to logfile
# -----------------------------------------

#exec > ${LIBMUSICFORMATS_GIT}/$(basename $0).log 2>&1

echo


MUSIC_FORMATS_DEV=${HOME}/musicformats-git-dev

DISTRIB_DIR=${MUSIC_FORMATS_DEV}/distrib

DOWNLOADED_MACOS_DISTRIB_NAME="musicformats-macos-distrib"
DOWNLOADED_UBUNTU_DISTRIB_NAME="musicformats-ubuntu-distrib"
DOWNLOADED_WINDOWS_DISTRIB_NAME="musicformats-windows-distrib"

MACOS_DISTRIB_ORG="${DISTRIB_DIR}/musicformats-macos-distrib"
UBUNTU_DISTRIB_ORG="${DISTRIB_DIR}/musicformats-ubuntu-distrib"
WINDOWS_DISTRIB_ORG="${DISTRIB_DIR}/musicformats-windows-distrib"


cd ${DISTRIB_DIR}
echo -n "--> current directory: "; pwd
echo


echo "----------------------------------------------"
echo "==> initial distrib contents:"
echo "----------------------------------------------"
echo

ls -salt ${DISTRIB_DIR}
echo


echo "----------------------------------------------"
echo "==> clean the distributions"
echo "----------------------------------------------"
echo

#rm -r musicformats-*-distrib
rm *.txt
rm *.pdf
rm -r MusicFormatsFor*
echo

echo "----------------------------------------------"
echo "==> final distrib contents:"
echo "----------------------------------------------"
echo

ls -salt ${DISTRIB_DIR}


#set +x
