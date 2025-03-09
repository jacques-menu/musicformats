#!/bin/zsh

#
#   MusicFormats Library
#   Copyright (C) Jacques Menu 2016-2025
#
#   This Source Code Form is subject to the terms of the Mozilla Public
#   License, v. 2.0. If a copy of the MPL was not distributed with this
#   file, you can obtain one at http://mozilla.org/MPL/2.0/.
#
#   https://github.com/jacques-menu/musicformats
#

#set -x

function usage ()
  {
    echo "An argument is expected."
    echo "Usage is:"
    echo "  $0 NUMBER"
    exit 1
  }

LIBMUSICFORMATS_GIT=${HOME}/musicformats-git-dev


# Write all output to logfile
# -----------------------------------------

#exec > ${LIBMUSICFORMATS_GIT}/$(basename $0).log 2>&1


# Let's go!
# -----------------------------------------

[ $# != 1 ] && usage

VERSION_NUMBER=$1

VERSION_NUMBER_FILE_NAME_ROOT="MusicFormatsVersionNumber"


# VERSION_NUMBER_FILE_NAME
cd ${LIBMUSICFORMATS_GIT}
echo "==> PWD is:"
pwd
echo

VERSION_NUMBER_FILE_NAME="${VERSION_NUMBER_FILE_NAME_ROOT}.txt"

echo "==> Writing MusicFormats version number ${VERSION_NUMBER} to ${VERSION_NUMBER_FILE_NAME}"
echo

echo -n ${VERSION_NUMBER} > ${VERSION_NUMBER_FILE_NAME}

ls -salGTF ${VERSION_NUMBER_FILE_NAME}
cat ${VERSION_NUMBER_FILE_NAME}

echo


#VERSION_NUMBER_HEADER_NAME
cd ${LIBMUSICFORMATS_GIT}/src
echo "==> PWD is:"
pwd
echo

VERSION_NUMBER_HEADER_NAME="${VERSION_NUMBER_FILE_NAME_ROOT}.h"

echo "==> Writing MusicFormats version number ${VERSION_NUMBER} to ${VERSION_NUMBER_HEADER_NAME}"
echo

echo "#define MUSICFORMATS_VERSION_NUMBER \"${VERSION_NUMBER}\"" > ${VERSION_NUMBER_HEADER_NAME}

ls -salGTF ${VERSION_NUMBER_HEADER_NAME}
cat ${VERSION_NUMBER_HEADER_NAME}


#set +x
