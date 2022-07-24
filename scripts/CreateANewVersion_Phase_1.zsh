#!/bin/zsh

#set -x

function usage ()
  {
    echo "Usage is:"
    echo "  $0 versionNumber versionDate"
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
# scripts/CreateDocumentationPDFs.zsh
echo


echo "--> Add all the files that need it"
addAll
echo


echo "--> Commit a first time to the local repository clone with a ’Pre’ version number:"
git commit -m "Pre ${VERSION_NUMBER}" -a
echo


echo "--> Push this to the MusicFormats repo:"
git push
echo


echo "--> Check at https://github.com/jacques-menu/musicformats/ actions"
echo


echo "--> Now, you should wait until the actions are done and switch to phase 2"
echo
