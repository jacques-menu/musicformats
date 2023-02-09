#!/bin/zsh

# set -x

function usage ()
{
  echo "Usage is:"
  echo "  $0 first second [fileName]+"
  echo
  exit 1
}

if [[ "$#" -le 2 ]]; then usage; fi


# Write all output to logfile
# -----------------------------------------

#exec > ${LIBMUSICFORMATS_GIT}/$(basename $0).log 2>&1


# get the arguments
# -----------------------------------------

FIRST="$1"
echo "FIRST : ${FIRST}"
SECOND="$2"
echo "SECOND: ${SECOND}"
echo


# -----------------------------------------
echo "Let's go!"
# -----------------------------------------
echo

# for FILE in $@
# do printf 'Something with "%s"\n' "${FILE}"
# done

# for FILE in ${@:3:$#}
# do printf 'Something with "%s"\n' "${FILE}"
# done

for FILE in ${@:3:$#}
do
#   echo "FILE: ${FILE}:"
#   ls -sal ${FILE}

  FIRST_OCCURRENCES_NBR=$(egrep -c "${FIRST}" ${FILE})
  SECOND_OCCURRENCES_NBR=$(egrep -c "${SECOND}" ${FILE})

#   [[ ${FIRST_OCCURRENCES_NBR} -gt 0 && ${SECOND_OCCURRENCES_NBR} -eq 0 ]]; echo "==> CHECK ${FILE}"
  if [[ (${FIRST_OCCURRENCES_NBR} -gt 0) && (${SECOND_OCCURRENCES_NBR} -eq 0) ]]
  then
    echo
    echo "==> CHECK ${FILE}!"
    echo "      ${FIRST_OCCURRENCES_NBR} "${FIRST}", ${SECOND_OCCURRENCES_NBR} ${SECOND}"
    echo
  fi
done
echo

# set +x
