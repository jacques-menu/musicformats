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

# set -x

function usage ()
  {
    echo "Parameter error:"
    echo "  '"$1"'" "is unknown"
    echo "Usage is:"
    echo " $0 "
    echo
    exit 1
  }

#	my actual work directory
#----------------------------------------------

MY_WORK_DIR=${HOME}


# dev branch
#----------------------------------------------

MUSIC_FORMATS_DEV=${MY_WORK_DIR}/musicformats-git-dev
echo
echo "==> MUSIC_FORMATS_DEV = ${MUSIC_FORMATS_DEV}"
echo

BUILD_DIR=${MUSIC_FORMATS_DEV}/build
echo "==> $0: BUILD_DIR          = ${BUILD_DIR}"
echo


#	the install directory
#----------------------------------------------

INSTALL_DIR=/usr/local
echo "==> $0: INSTALL_DIR          = ${INSTALL_DIR}"
echo

INSTALL_BIN_DIR=${INSTALL_DIR}/bin/libmusicformats
INSTALL_LIB_DIR=${INSTALL_DIR}/lib/libmusicformats
INSTALL_INCLUDE_DIR=${INSTALL_DIR}/include/libmusicformats

mkdir -p ${INSTALL_BIN_DIR}
mkdir -p ${INSTALL_LIB_DIR}
mkdir -p ${INSTALL_INCLUDE_DIR}

echo "==> $0: INSTALL_BIN_DIR          = ${INSTALL_BIN_DIR}"
echo "==> $0: INSTALL_LIB_DIR          = ${INSTALL_LIB_DIR}"
echo "==> $0: INSTALL_INCLUDE_DIR      = ${INSTALL_INCLUDE_DIR}"
echo


# Write all output to logfile if desired
# -----------------------------------------

SCRIPT_BASE_NAME="$(basename ${0})"

LOGFILE_NAME=${SCRIPT_BASE_NAME/%.zsh/.zsh.log}
LOGFILE=${BUILD_DIR}/${LOGFILE_NAME}
# echo "==> $0: 0 = ${0}"
# echo "==> $0: SCRIPT_BASE_NAME = ${SCRIPT_BASE_NAME}"
# echo "==> $0: LOGFILE_NAME     = ${LOGFILE_NAME}"
# echo "==> $0: LOGFILE          = ${LOGFILE}"
# echo

# HERE you choose to have an output logfile
# exec > ${LOGFILE} 2>&1


# Let's go!
# -----------------------------------------

cd ${BUILD_DIR}

echo "==> date is:"
date
echo

echo "==> PWD is:"
pwd
echo

echo "==> Copying MusicFormats bin files"
echo
cp -p bin/* ${INSTALL_BIN_DIR}

echo "==> Copying MusicFormats lib files"
echo
cp -p lib/* ${INSTALL_LIB_DIR}

echo "==> Copying MusicFormats include files"
echo
cp -p bin/* ${INSTALL_INCLUDE_DIR}


# Show the results
# -----------------------------------------

mkdir -p ${INSTALL_BIN_DIR}
mkdir -p ${INSTALL_LIB_DIR}
mkdir -p ${INSTALL_INCLUDE_DIR}


echo '--> INSTALL_BIN_DIR: ' ${INSTALL_BIN_DIR}
echo
ls -salGTF ${INSTALL_BIN_DIR}
echo

echo '--> INSTALL_LIB_DIR: ' ${INSTALL_LIB_DIR}
echo
ls -salGTF ${INSTALL_LIB_DIR}
echo

echo '--> INSTALL_INCLUDE_DIR: ' ${INSTALL_INCLUDE_DIR}
echo
ls -salGTF ${INSTALL_INCLUDE_DIR}
echo


# set +x
