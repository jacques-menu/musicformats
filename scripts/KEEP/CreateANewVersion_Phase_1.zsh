#!/bin/zsh

#set -x

function usage ()
  {
    echo "Usage is:"
    echo "  ${SCRIPT_NAME} versionNumber versionDate"
    echo
    exit 1
  }

#	my actual work directory
#----------------------------------------------

MY_WORK_DIR=${HOME}
. ${MY_WORK_DIR}/JMI_ShellSettings/zsh_20_MusicFormats


# dev branch
#----------------------------------------------

MUSIC_FORMATS_DEV=${MY_WORK_DIR}/musicformats-git-dev
# echo
# echo "========> MUSIC_FORMATS_DEV = ${MUSIC_FORMATS_DEV}"
# echo


# check the arguments number

SCRIPT_NAME="CreateANewVersion_Phase_1.zsh"

[ $# != 2 ] && usage
VERSION_NUMBER=$1
VERSION_DATE=$2
echo "--> VERSION_NUMBER = ${VERSION_NUMBER}, VERSION_DATE = ${VERSION_DATE}"
echo


# let's go

cd ${MUSIC_FORMATS_DEV}
echo "--> Current directory: ${PWD}:"
echo


echo "--> Current version number and date:"
cat MusicFormatsVersionNumber.txt
echo
cat MusicFormatsVersionDate.txt
echo


echo "--> Create the documentation PDF files"
scripts/CreateDocumentationPDFs.zsh
echo


echo "--> Add all the files that need it"
addAll
echo


echo "--> Commit to the local repository clone:"
git commit -m "${VERSION_NUMBER}" -a
echo


echo "--> Push to the MusicFormats repo:"
git push
echo


echo "--> Now, you should wait until the actions are done,"
echo "    check at https://github.com/jacques-menu/musicformats/actions"
echo


echo "--> After that, download the three resulting musicformats-*-distrib archives locally,"
echo "    and proceed to phase 2"
