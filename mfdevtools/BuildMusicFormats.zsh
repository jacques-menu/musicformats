#!/bin/zsh

#
#   MusicFormats Library
#   Copyright (C) Jacques Menu 2016-2023
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
    echo "Parameter error:"
    echo "  '"$1"'" "is unknown"
    echo "Usage is:"
    echo "  $0 [cmake] [clean] [install]"
    echo
    exit 1
  }

#	my actual work directory
#----------------------------------------------

MY_WORK_DIR=${HOME}


# dev branch
#----------------------------------------------

MUSIC_FORMATS_DEV=${MY_WORK_DIR}/musicformats-git-dev
# echo
# echo "==> MUSIC_FORMATS_DEV = ${MUSIC_FORMATS_DEV}"
# echo


# Write all output to logfile
# -----------------------------------------

SCRIPT_BASE_NAME="$(basename ${0})"

LOGFILE_NAME=${SCRIPT_BASE_NAME/%.zsh/.zsh.log}
LOGFILE=${MUSIC_FORMATS_DEV}/build/${LOGFILE_NAME}
# echo "==> $0: 0 = ${0}"
# echo "==> $0: SCRIPT_BASE_NAME = ${SCRIPT_BASE_NAME}"
# echo "==> $0: LOGFILE_NAME     = ${LOGFILE_NAME}"
# echo "==> $0: LOGFILE          = ${LOGFILE}"
# echo

exec > ${LOGFILE} 2>&1


# Let's go!
# -----------------------------------------

cd ${MUSIC_FORMATS_DEV}/build

echo "==> date is:"
date
echo

echo "==> Building musicformats and the samples"
echo

echo "==> PWD is:"
pwd
echo

# Run 'cmake'
# -----------------------------------------

echo '--> cmake'
echo

cmake .
echo


# Run 'make'
# -----------------------------------------

echo '--> make'
echo
#make --trace
#make -i

#make cmake CMAKEOPT='-DLILY=on -DBMML=on -DMEI=on'

make CMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} -DAPPLEDEBUG=yes" JOBS=5
#--debug=j
echo


# Show the results
# -----------------------------------------

echo '--> lib'
echo
ls -salGTF lib
echo

echo '--> bin'
echo
ls -salGTF bin/xml2* bin/msdl # bin/libMultipleInitsTest  bin/Mikrokosmos3Wandering
echo


#set +x
