#!/bin/bash

#set -x

function usage ()
  {
    echo "An argument is expected."
    echo "Usage is:"
    echo "  $0 [cmake] DATE"
    exit 1
  }

LIBMUSICFORMATS_GIT=${HOME}/musicformats-git-dev


# Write all output to logfile
# -----------------------------------------

#exec > ${LIBMUSICFORMATS_GIT}/$(basename $0).log 2>&1


# Let's go!
# -----------------------------------------

[ $# != 1 ] && usage

VERSION_DATE=$1

VERSION_DATE_FILE_NAME_ROOT="MusicFormatsVersionDate"

cd ${LIBMUSICFORMATS_GIT}
#echo "==> PWD is:"
#pwd
#echo "==> date is:"
#date
#echo

# VERSION_DATE_FILE_NAME
VERSION_DATE_FILE_NAME="${VERSION_DATE_FILE_NAME_ROOT}.txt"

echo "==> Writing MusicFormats version date ${VERSION_DATE} to ${VERSION_DATE_FILE_NAME}"
echo

echo ${VERSION_DATE} > ${VERSION_DATE_FILE_NAME}

ls -salGTF ${VERSION_DATE_FILE_NAME}
cat ${VERSION_DATE_FILE_NAME}

echo

#VERSION_DATE_HEADER_NAME
VERSION_DATE_HEADER_NAME="${VERSION_DATE_FILE_NAME_ROOT}.h"

echo "==> Writing MusicFormats version date ${VERSION_DATE} to ${VERSION_DATE_HEADER_NAME}"
echo

echo "#define MUSICFORMATS_VERSION_DATE \"${VERSION_DATE}\"" > ${VERSION_DATE_HEADER_NAME}

ls -salGTF ${VERSION_DATE_HEADER_NAME}
cat ${VERSION_DATE_HEADER_NAME}


#set +x
