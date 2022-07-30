#!/bin/zsh

#set -x

LIBMUSICFORMATS_GIT=${HOME}/musicformats-git-dev


# Write all output to logfile
# -----------------------------------------

#exec > ${LIBMUSICFORMATS_GIT}/$(basename $0).log 2>&1


# Let's go!
# -----------------------------------------

cd ${LIBMUSICFORMATS_GIT}
echo "==> PWD is:"
pwd
echo

echo "Version number:"
cat MusicFormatsVersionNumber.txt

echo "Version date:"
cat MusicFormatsVersionDate.txt

#set +x
