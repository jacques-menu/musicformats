#!/bin/bash

set -x


# the file name

NAME=MaxBarockAmbitus


# run the MFSL script

MFSL_OPTIONS="-trace-inputs"

./${NAME}.mfsl ${MFSL_OPTIONS}
echo
ls -sal ${NAME}.ly


# on Mac OS, 'open' launches the application set by the user
# for a given extension, here '.ly',
# to diplay the file on-screen

open ${NAME}.ly

set +x
