#!/bin/zsh

#
#   MusicFormats Library
#   Copyright (C) Jacques Menu 2016-2024
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

USER_GUIDE_DIR="${DOCUMENTATION_DIR}/mfuserguide"
USER_GUIDE_NAME="MusicFormatsUserGuide"

MAINTAINANCE_GUIDE_DIR="${DOCUMENTATION_DIR}/mfmaintainanceguide"
MAINTAINANCE_GUIDE_NAME="MusicFormatsMaintainanceGuide"

FIGURES_DIR="${DOCUMENTATION_DIR}/mfmaintainanceguide"
FIGURES_NAME="MusicFormatsFigures"

API_GUIDE_DIR="${DOCUMENTATION_DIR}/mfapiguide"
API_GUIDE_NAME="MusicFormatsAPIGuide"

PDFLATEX=/Library/TeX/texbin/pdflatex

echo "PDFLATEX: ${PDFLATEX}"
echo


cd ${USER_GUIDE_DIR}
echo -n "--> current directory: "; pwd
echo
ls -sal ${USER_GUIDE_DIR}/${USER_GUIDE_NAME}.tex
echo
echo
${PDFLATEX} --file-line-error --synctex=1 \
  ${USER_GUIDE_NAME}.tex


cd ${MAINTAINANCE_GUIDE_DIR}
echo -n "--> current directory: "; pwd
echo
ls -sal ${MAINTAINANCE_GUIDE_DIR}/${MAINTAINANCE_GUIDE_NAME}.tex
echo
echo
${PDFLATEX} --file-line-error --synctex=1 \
  ${MAINTAINANCE_GUIDE_NAME}.tex


cd ${FIGURES_DIR}
echo -n "--> current directory: "; pwd
echo
ls -sal ${FIGURES_DIR}/${FIGURES_NAME}.tex
echo
echo
${PDFLATEX} --file-line-error --synctex=1 \
  ${FIGURES_NAME}.tex


cd ${API_GUIDE_DIR}
echo -n "--> current directory: "; pwd
echo
ls -sal ${API_GUIDE_DIR}/${API_GUIDE_NAME}.tex
echo
echo
${PDFLATEX} --file-line-error --synctex=1 \
  ${API_GUIDE_NAME}.tex


echo "----------------------------------------------"
echo "==> documentation PDF files:"
echo "----------------------------------------------"
echo

ls -salt ${FIGURES_DIR}/${FIGURES_NAME}.pdf
ls -salt ${USER_GUIDE_DIR}/${USER_GUIDE_NAME}.pdf
ls -salt ${MAINTAINANCE_GUIDE_DIR}/${MAINTAINANCE_GUIDE_NAME}.pdf
ls -salt ${API_GUIDE_DIR}/${API_GUIDE_NAME}.pdf
echo


#set +x
