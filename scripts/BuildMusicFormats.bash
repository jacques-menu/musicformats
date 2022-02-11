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

exec > ${LIBMUSICFORMATS_GIT}/$(basename $0).log 2>&1

echo


# Let's go!
# -----------------------------------------

cd ${LIBMUSICFORMATS_GIT}/build

echo "==> date is:"
date
echo

echo "==> Building musicformats and the samples"
echo

echo "==> PWD is:"
pwd
echo

# Run 'make'
# -----------------------------------------

echo '--> make'
echo
#make --trace
#make -i

#make cmake CMAKEOPT='-DLILY=on -DBMML=on -DMEI=on'

make CMAKE_CXX_FLAGS="${CMAKE_CXX_FLAGS} -DAPPLEDEBUG=yes" JOBS=5
#--debug=j
echo


# Show the results
# -----------------------------------------

echo '--> lib'
echo
ls -salGTF lib
echo

echo '--> bin'
echo
ls -salGTF bin/xml2* bin/msdl # bin/libMultipleInitsTest  bin/Mikrokosmos3Wandering
echo


#set +x
