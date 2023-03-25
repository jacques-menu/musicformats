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

LIBMUSICFORMATS_GIT=${HOME}/musicformats-git-dev


# Write all output to logfile
# -----------------------------------------

#exec > ${LIBMUSICFORMATS_GIT}/$(basename $0).log 2>&1

echo


DOCUMENTATION_DIR=${LIBMUSICFORMATS_GIT}/documentation

ARCHITECTURE_NAME="MusicFormatsArchitecture.pdf"
FIGURES_NAME="MusicFormatsFigures"
USER_GUIDE_NAME="MusicFormatsUserGuide"
MAINTENANCE_GUIDE_NAME="MusicFormatsMaintainanceGuide"
API_GUIDE_NAME="MusicFormatsAPIGuide"

FIGURES_DIR="${DOCUMENTATION_DIR}/${FIGURES_NAME}"
USER_GUIDE_DIR="${DOCUMENTATION_DIR}/${USER_GUIDE_NAME}"
MAINTENANCE_GUIDE_DIR="${DOCUMENTATION_DIR}/${MAINTENANCE_GUIDE_NAME}"
API_GUIDE_DIR="${DOCUMENTATION_DIR}/${API_GUIDE_NAME}"

PDFLATEX=/Library/TeX/texbin/pdflatex

echo "PDFLATEX: ${PDFLATEX}"
echo

cd ${FIGURES_DIR}
echo -n "--> current directory: "; pwd
echo
${PDFLATEX} --file-line-error --synctex=1 \
  ${FIGURES_NAME}.tex

cd ${USER_GUIDE_DIR}
echo -n "--> current directory: "; pwd
echo
${PDFLATEX} --file-line-error --synctex=1 \
  ${USER_GUIDE_NAME}.tex

cd ${MAINTENANCE_GUIDE_DIR}
echo -n "--> current directory: "; pwd
echo
${PDFLATEX} --file-line-error --synctex=1 \
  ${MAINTENANCE_GUIDE_NAME}.tex

cd ${API_GUIDE_DIR}
echo -n "--> current directory: "; pwd
echo
${PDFLATEX} --file-line-error --synctex=1 \
  ${API_GUIDE_NAME}.tex


echo "----------------------------------------------"
echo "==> documentation PDF files:"
echo "----------------------------------------------"
echo

ls -salt ${FIGURES_DIR}/${FIGURES_NAME}.pdf
ls -salt ${USER_GUIDE_DIR}/${USER_GUIDE_NAME}.pdf
ls -salt ${MAINTENANCE_GUIDE_DIR}/${MAINTENANCE_GUIDE_NAME}.pdf
ls -salt ${API_GUIDE_DIR}/${API_GUIDE_NAME}.pdf
echo


#set +x
