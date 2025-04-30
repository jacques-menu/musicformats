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

# the MusicFormats directory
export MUSICFORMATS_DIR=${HOME}/JMI_DEVELOPMENT/musicformats-git-dev
echo "--> MUSICFORMATS_DIR: ${MUSICFORMATS_DIR}"
echo

ls -sal ${MUSICFORMATS_DIR}


# set the MusicFormats variables
. ${MUSICFORMATS_DIR}/mfdevtools/SetMusicFormatsVariables.zsh


echo

# set documentation variables

DOCUMENTATION_DIR=${MUSICFORMATS_DIR}/documentation
echo "--> DOCUMENTATION_DIR: ${DOCUMENTATION_DIR}"
ls -sal ${DOCUMENTATION_DIR}

ARCHITECTURE_NAME="MusicFormatsArchitecture.pdf"

USER_GUIDE_DIR="${DOCUMENTATION_DIR}/mfuserguide"
USER_GUIDE_NAME="MusicFormatsUserGuide"

MAINTAINANCE_GUIDE_DIR="${DOCUMENTATION_DIR}/mfmaintainanceguide"
MAINTAINANCE_GUIDE_NAME="MusicFormatsMaintainanceGuide"

FIGURES_DIR="${DOCUMENTATION_DIR}/mfmaintainanceguide"
FIGURES_NAME="MusicFormatsFigures"

API_GUIDE_DIR="${DOCUMENTATION_DIR}/mfapiguide"
API_GUIDE_NAME="MusicFormatsAPIGuide"


# LaTeX

PDFLATEX_COMMAND=/Library/TeX/texbin/pdflatex

echo "PDFLATEX_COMMAND: ${PDFLATEX_COMMAND}"
echo


# user guide, 3 pdflatex runs

cd ${USER_GUIDE_DIR}
echo -n "--> current directory: "; pwd
echo
ls -sal ${USER_GUIDE_DIR}/${USER_GUIDE_NAME}.tex
echo
echo
${PDFLATEX_COMMAND} --file-line-error --synctex=1 \
  ${USER_GUIDE_NAME}.tex
${PDFLATEX_COMMAND} --file-line-error --synctex=1 \
  ${USER_GUIDE_NAME}.tex
${PDFLATEX_COMMAND} --file-line-error --synctex=1 \
  ${USER_GUIDE_NAME}.tex


# maintainance guide, 3 pdflatex runs

cd ${MAINTAINANCE_GUIDE_DIR}
echo -n "--> current directory: "; pwd
echo
ls -sal ${MAINTAINANCE_GUIDE_DIR}/${MAINTAINANCE_GUIDE_NAME}.tex
echo
echo
${PDFLATEX_COMMAND} --file-line-error --synctex=1 \
  ${MAINTAINANCE_GUIDE_NAME}.tex
${PDFLATEX_COMMAND} --file-line-error --synctex=1 \
  ${MAINTAINANCE_GUIDE_NAME}.tex
${PDFLATEX_COMMAND} --file-line-error --synctex=1 \
  ${MAINTAINANCE_GUIDE_NAME}.tex


# figures, 1 pdflatex run

cd ${FIGURES_DIR}
echo -n "--> current directory: "; pwd
echo
ls -sal ${FIGURES_DIR}/${FIGURES_NAME}.tex
echo
echo
${PDFLATEX_COMMAND} --file-line-error --synctex=1 \
  ${FIGURES_NAME}.tex


# API guide, 1 pdflatex run

cd ${API_GUIDE_DIR}
echo -n "--> current directory: "; pwd
echo
ls -sal ${API_GUIDE_DIR}/${API_GUIDE_NAME}.tex
echo
echo
${PDFLATEX_COMMAND} --file-line-error --synctex=1 \
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
