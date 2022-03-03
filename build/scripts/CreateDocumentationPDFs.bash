#!/bin/bash

#set -x

LIBMUSICFORMATS_GIT=${HOME}/musicformats-git-dev


# Write all output to logfile
# -----------------------------------------

#exec > ${LIBMUSICFORMATS_GIT}/$(basename $0).log 2>&1

echo


DOCUMENTATION_DIR=${LIBMUSICFORMATS_GIT}/documentation

USER_GUIDE_NAME="MusicFormatsUserGuide"
MAINTENANCE_GUIDE_NAME="MusicFormatsMaintainanceGuide"
API_GUIDE_NAME="MusicFormatsAPIGuide"

USER_GUIDE_DIR="${DOCUMENTATION_DIR}/${USER_GUIDE_NAME}"
MAINTENANCE_GUIDE_DIR="${DOCUMENTATION_DIR}/${MAINTENANCE_GUIDE_NAME}"
API_GUIDE_DIR="${DOCUMENTATION_DIR}/${API_GUIDE_NAME}"


cd ${USER_GUIDE_DIR}
echo -n "--> current directory: "; pwd
echo
pdflatex --file-line-error --synctex=1 \
  ${USER_GUIDE_NAME}.tex

cd ${MAINTENANCE_GUIDE_DIR}
echo -n "--> current directory: "; pwd
echo
pdflatex --file-line-error --synctex=1 \
  ${MAINTENANCE_GUIDE_NAME}.tex

cd ${API_GUIDE_DIR}
echo -n "--> current directory: "; pwd
echo
pdflatex --file-line-error --synctex=1 \
  ${API_GUIDE_NAME}.tex


echo "----------------------------------------------"
echo "==> documentation PDF files:"
echo "----------------------------------------------"
echo

ls -salt ${USER_GUIDE_DIR}/${USER_GUIDE_NAME}.pdf
ls -salt ${MAINTENANCE_GUIDE_DIR}/${MAINTENANCE_GUIDE_NAME}.pdf
ls -salt ${API_GUIDE_DIR}/${API_GUIDE_NAME}.pdf
echo


#set +x
