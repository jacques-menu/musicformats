#!/bin/bash

#set -x

function usage ()
  {
    echo "An argument is expected."
    echo "Usage is:"
    echo "  $0 VERSION_NUMBER DATE"
    exit 1
  }

LIBMUSICFORMATS_GIT=${HOME}/musicformats-git-dev


# Write all output to logfile
# -----------------------------------------

#exec > ${LIBMUSICFORMATS_GIT}/$(basename $0).log 2>&1

echo


# Let's go!
# -----------------------------------------

[ $# != 2 ] && usage

VERSION_NUMBER=$1

VERSION_DATE=$2


# cd
cd "${LIBMUSICFORMATS_GIT}"


# version number and date
scripts/SetMusicFormatsVersionNumber.bash ${VERSION_NUMBER}
cat MusicFormatsVersionNumber.txt

scripts/SetMusicFormatsVersionDate.bash ${VERSION_DATE}
cat MusicFormatsVersionDate.txt


# documentation
scripts/CreateDocumentationPDFs.bash


# commit
git commit -m "Pre ${VERSION_NUMBER}" -a


# push
git push


# wait until actions are successfull and
# the distributions have been downloaded manually

read


# go ahead

MakeMusicFormatsDistributions.bash


# commit and push again with the new version name

git commit -m "${VERSION_NUMBER}" -a
git push


# create the new version branch locally and remotely

git checkout master
git branch ${VERSION_NUMBER}
git push --set-upstream master ${VERSION_NUMBER}
git branch
git branch -r

