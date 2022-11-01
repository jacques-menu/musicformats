#!/bin/bash

#set -x

function usage ()
  {
    echo "An argument is expected."
    echo "Usage is:"
    echo "  $0 VERSION_NUMBER"
    exit 1
  }


# Let's go!
# -----------------------------------------

[ $# != 1 ] && usage

VERSION_NUMBER=$1

LOCAL_CLONE_DIR=${HOME}/musicformats_local_clone_${VERSION_NUMBER}

BUILD_DIR=${LOCAL_CLONE_DIR}/build


# clone
git clone \
  -b ${VERSION_NUMBER} \
  https://github.com/jacques-menu/musicformats \
  ${LOCAL_CLONE_DIR}

ls -sal ${LOCAL_CLONE_DIR}


# build
cd ${BUILD_DIR}

make


# show the executables
ls -sal bin


#set +x
