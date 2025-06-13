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


# the MusicFormats directory
export MUSICFORMATS_DIR=${HOME}/JMI_DEVELOPMENT/musicformats-git-dev
echo "--> MUSICFORMATS_DIR: ${MUSICFORMATS_DIR}"
echo

ls -sal ${MUSICFORMATS_DIR}


# set the MusicFormats variables
. ${MUSICFORMATS_DIR}/mfdevtools/SetMusicFormatsVariables.zsh


# Write all output to logfile
#exec > ${MUSICFORMATS_DIR}/$(basename $0).log 2>&1


# COPY musicformats-git-dev to a folder with today's date in it's name
# -------------------------------------------------

cd /Users/jacquesmenu/JMI_Developpement
pwd
echo

DEST_DIR="musicformats-git-dev_${MUSICFORMATS_VERSION_NUMBER}_${MUSICFORMATS_VERSION_DATE}"
echo "--> DEST_DIR: ${DEST_DIR}"

# set -x
cp -prf musicformats-git-dev ${DEST_DIR}
# set +x

echo "Current directory is ${PWD}"
ls -ltGTF $* | head -15
