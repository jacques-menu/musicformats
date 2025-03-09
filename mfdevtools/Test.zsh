#!/bin/zsh

# sh script.sh alpha beta gamma zeta -->
# ssh -t alpha ssh -t beta ssh -t gamma /pathtofile/zeta/log.log

# The last command in the script would execute this as a command.


# argn=$#
# i=0
#
# for arg do
#    shift
#    i=$(( i + 1 ))
#
#    if [ "$i" -lt "$argn" ]; then
#        set -- "$@" ssh -t "$arg"
#    else
# #        set -- "$@" "/pathtofile/$arg/log.log"
#        set -- "$@" "/pathtofile/$arg/log.log"
#    fi
# done
#
# command "$@"


argn=$#
i=0

for arg do
   i=$(( i + 1 ))

#    if [ "$i" -le "$argn" ]; then
       set -- "$FILES" "$1"
#    else
# #        set -- "$@" "/pathtofile/$arg/log.log"
#        set -- "$@" "/pathtofile/$arg/log.log"
#    fi
   shift
done

echo ${#[FILES]}

echo "$FILES"

for FILE_NAME in $FILES ; do
  echo
  echo "----------------------------"
  echo "==> FILE_NAME: $FILE_NAME:"
done
