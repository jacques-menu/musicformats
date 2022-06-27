#!/bin/bash

#set -x

function usage ()
  {
    echo "Parameter error:"
    echo "  '"$1"'" "is unknown"
    echo "Usage is:"
    echo "  $0 [cmake] [clean] [install]"
    echo
    exit 1
  }

LIBMUSICFORMATS_GIT=${HOME}/musicformats-git-dev


# Write all output to logfile
# -----------------------------------------

#exec > ${LIBMUSICFORMATS_GIT}/$(basename $0).log 2>&1

echo


# Let's go!
# -----------------------------------------

# [ $# != 1 ] usage

VERSION_NUMBER=$1

# VERSION_NUMBER_FILE_NAME="MusicFormatsVersionNumber.txt"

cd ${LIBMUSICFORMATS_GIT}
echo "==> PWD is:"
pwd
echo

git checkout master
git push --set-upstream origin ${VERSION_NUMBER}


# Show the results
# -----------------------------------------

echo "==> The branches are:"
git branch
echo

#set +x
