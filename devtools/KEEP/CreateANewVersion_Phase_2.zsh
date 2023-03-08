#!/bin/zsh

#set -x

function usage ()
  {
    echo "Usage is:"
    echo "  ${SCRIPT_NAME} newVersionNumber newVersionDate"
    echo
    exit 1
  }

#	my actual work directory
#----------------------------------------------

MY_WORK_DIR=${HOME}
. ${MY_WORK_DIR}/JMI_ShellSettings/zsh_20_MusicFormats


# dev directory
#----------------------------------------------

MUSIC_FORMATS_DEV_DIR=${MY_WORK_DIR}/musicformats-git-dev
# echo
# echo "========> MUSIC_FORMATS_DEV_DIR = ${MUSIC_FORMATS_DEV_DIR}"
# echo


# check the arguments number

SCRIPT_NAME="CreateANewVersion_Phase_2.zsh"

[ $# != 2 ] && usage
NEW_VERSION_NUMBER=$1
NEW_VERSION_DATE=$2
echo "--> NEW_VERSION_NUMBER = ${NEW_VERSION_NUMBER}, NEW_VERSION_DATE = ${NEW_VERSION_DATE}"
echo


# let's go

cd ${MUSIC_FORMATS_DEV_DIR}
echo "--> Current directory: ${PWD}:"
echo


echo "--> Create the distributions"
scripts/MakeMusicFormatsDistributions.zsh
echo
ls -sal distrib

set -x

exit

echo "--> Create a new version number and date"
scripts/SetMusicFormatsVersionNumber.zsh "${NEW_VERSION_NUMBER}"
scripts/SetMusicFormatsVersionDate.zsh "${NEW_VERSION_DATE}"
echo


echo "--> Show the result"
scripts/ShowMusicFormatsVersion.zsh
echo
