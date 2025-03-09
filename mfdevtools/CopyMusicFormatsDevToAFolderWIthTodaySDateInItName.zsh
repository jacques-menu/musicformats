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


# COPY musicformats-git-dev to a folder with today's date in it's name
# -------------------------------------------------

VERSION_NUMBER="v0.9.72-beta-1"
echo "VERSION_NUMBER: ${VERSION_NUMBER}"
TIME_STAMP=$(date +"%Y_%m_%d__%H_%M")
echo "TIME_STAMP: ${TIME_STAMP}"
jmid
DEST_DIR="musicformats-git-dev_${VERSION_NUMBER}_${TIME_STAMP}"
# set -x
cp -prf musicformats-git-dev ${DEST_DIR}
# set +x
echo "Current directory is ${PWD}"
ltt

