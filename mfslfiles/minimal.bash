#!/bin/bash

set -x

MINIMAL_LY=Minimal.ly

./minimal.mfsl > ${MINIMAL_LY}

ls -sal ${MINIMAL_LY}

# on Mac OS, 'open' launches the application set by the user
# for a given extension, here '.ly',
# to diplay the file on-screen

open ${MINIMAL_LY}

set +x
