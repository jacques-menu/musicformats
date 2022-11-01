#!/bin/bash

#set -x

function usage ()
  {
    echo "An argument is expected."
    echo "Usage is:"
    echo "  $0 VERSION_NUMBER optionsAndArguments"
    exit 1
  }


# Let's go!
# -----------------------------------------

[ $# == 0 ] && usage

# get version number and skip it
VERSION_NUMBER=$1
shift


# run xml2ly

LOCAL_CLONE_DIR=${HOME}/musicformats_local_clone_${VERSION_NUMBER}

BIN_DIR=${LOCAL_CLONE_DIR}/build/bin

${BIN_DIR}/xml2ly $*


#set +x
