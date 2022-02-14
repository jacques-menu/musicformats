#!/bin/bash

#set -x

function usage ()
  {
    echo "Parameter error:"
    echo "  '"$1"'" "is unknown"
    echo "Usage is:"
    echo "  $0 [cmake] [clean] [install]"
    echo
    exit 1
  }

LIBMUSICFORMATS_GIT=${HOME}/musicformats-git-dev


# Write all output to logfile
# -----------------------------------------

#exec > ${LIBMUSICFORMATS_GIT}/$(basename $0).log 2>&1

echo


# Let's go!
# -----------------------------------------

[ $# != 1 ] usage

VERSION_NUMBER=$1

VERSION_NUMBER_FILE_NAME="MusicFormatsVersionNumber.txt"

cd ${LIBMUSICFORMATS_GIT}
echo "==> PWD is:"
pwd
echo

echo "==> date is:"
date
echo

echo "==> Writing MusicFormats version number to ${VERSION_NUMBER} to ${VERSION_NUMBER_FILE_NAME}"
echo

echo -n ${VERSION_NUMBER} > ${VERSION_NUMBER_FILE_NAME}


# Show the results
# -----------------------------------------

echo "--> ${VERSION_NUMBER_FILE_NAME}"
echo
ls -salGTF ${VERSION_NUMBER_FILE_NAME}
echo
cat ${VERSION_NUMBER_FILE_NAME}
echo


#set +x
