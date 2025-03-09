#!/bin/zsh

#
#   MusicFormats Library
#   Copyright (C) Jacques Menu 2016-2025
#
#   This Source Code Form is subject to the terms of the Mozilla Public
#   License, v. 2.0. If a copy of the MPL was not distributed with this
#   file, you can obtain one at http://mozilla.org/MPL/2.0/.
#
#   https://github.com/jacques-menu/musicformats
#


# clean the current directory from spurious files
# ---------------------------------------------------------

rm *.ly *.pdf *.midi


# select which files to translate
# ---------------------------------------------------------

echo '$#:                       ' $# " argument(s)"

i=0
while [ $i -ne $# ]; do
  echo "$i: argv [$i]"
  FILES+=argv[$i]
  i=$(($i+1))
done

for FILE_NAME in $FILES ; do
  echo
  echo "----------------------------"
  echo "==> FILE_NAME: $FILE_NAME:"
  echo
done


# argn=$#
# i=0
#
# for arg do
#    i=$(( i + 1 ))
#
# #    if [ "$i" -le "$argn" ]; then
#        set -- "$FILES" "$1"
# #    else
# # #        set -- "$@" "/pathtofile/$arg/log.log"
# #        set -- "$@" "/pathtofile/$arg/log.log"
# #    fi
#    shift
# done
#
# echo ${#[FILES]}
#
# echo "$FILES"
#
# for FILE_NAME in $FILES ; do
#   echo
#   echo "----------------------------"
#   echo "==> FILE_NAME: $FILE_NAME:"
# done


exit


# The parameters *, @ and argv are arrays containing all the positional parameters;
# thus ‘$argv[n]’, etc., is equivalent to simply ‘$n’.

#   a=(1 2 3 4 9)
#   echo ${#a[@]}
#
#   b=($a 27)
#   echo ${#b[@]}
#
#   echo $a

# c=(*.xml)
# echo $c
# echo ${#c[@]}
#
# echo "There are ${#c[@]} files:"
# for FILE_NAME in $c; do
#   echo "===> $FILE_NAME:"
#   ls -salGTF $FILE_NAME
# done

# array of directories names in $PWD:
# jacquesmenu@macstudio:/Volumes/JMI_Volume/JMI_Developpement/musicformats-git-dev/musicxmlfiles/gracenotesandchords > pwd_list=( "${(s:/:)${PWD/#$HOME/~}}" )
# jacquesmenu@macstudio:/Volumes/JMI_Volume/JMI_Developpement/musicformats-git-dev/musicxmlfiles/gracenotesandchords > echo $pwd_list
# Volumes JMI_Volume JMI_Developpement musicformats-git-dev musicxmlfiles gracenotesandchords


if [ $# -eq 0 ]; then
  # no arguments, all *.xml files will be translated
  FILE_NAMES=(*.xml)
else
  # only the files in the arguments will be translated
  FILE_NAMES=$*
fi
echo "FILE_NAMES:                $FILE_NAMES"

FILES_NBR=$(ls $FILE_NAMES | wc -l | sed 's/ //g')
# FILES_NBR=${#FILE_NAMES[@]}
echo "There are $FILES_NBR files to be translated"
echo


# translate all selected MusicXML files
# ---------------------------------------------------------

# for FILE_NAME in $FILE_NAMES ; do
for FILE_NAME in $FILES ; do
  echo
echo "----------------------------"
  echo "==> FILE_NAME: $FILE_NAME:"


  # handle file names

  FILE_BASE_NAME=$(basename "$FILE_NAME")
  echo "FILE_BASE_NAME:          $FILE_BASE_NAME"

  SUFFIXLESS_FILE_NAME=$(echo "$FILE_BASE_NAME" | cut -f 1 -d '.')
  echo "SUFFIXLESS_FILE_NAME:    $SUFFIXLESS_FILE_NAME"

  SUFFIXLESS_FILE_NAMES+=($SUFFIXLESS_FILE_NAME)
  echo "SUFFIXLESS_FILE_NAMES:   $SUFFIXLESS_FILE_NAMES"

  MUSICXML_FILE_NAME="$SUFFIXLESS_FILE_NAME.xml"
    # in case the argument contains only 'FOO.', for example
  echo "MUSICXML_FILE_NAME:      $MUSICXML_FILE_NAME"

  MUSICXML_FILE_NAMES=($MUSICXML_FILE_NAMES $MUSICXML_FILE_NAME)
  echo "MUSICXML_FILE_NAMES:     $MUSICXML_FILE_NAMES"


  echo


  # run xmlly

  echo; echo "--> running xml2ly on $MUSICXML_FILE_NAME"
  LILYPOND_FILE_NAME="$SUFFIXLESS_FILE_NAME.ly"
  echo "LILYPOND_FILE_NAME:      $LILYPOND_FILE_NAME"

  xml2ly -output-file-name $LILYPOND_FILE_NAME $MUSICXML_FILE_NAME
  if [ -f $LILYPOND_FILE_NAME ]; then
    echo
    echo "--> resulting LilyPond file:"
    ls -sal $LILYPOND_FILE_NAME
    LILYPOND_FILE_NAMES+=($LILYPOND_FILE_NAME)
    echo "LILYPOND_FILE_NAMES:   $LILYPOND_FILE_NAMES"
  fi

  echo


  # run lilypond

  echo; echo "--> running lilypond on $LILYPOND_FILE_NAME"
  PDF_FILE_NAME="$SUFFIXLESS_FILE_NAME.pdf"
  echo "PDF_FILE_NAME:           $PDF_FILE_NAME"

  lilypond $LILYPOND_FILE_NAME
  if [ -f $PDF_FILE_NAME ]; then
    echo
    echo "--> resulting PDF file:"
    ls -sal $PDF_FILE_NAME
    PDF_FILE_NAMES+=($PDF_FILE_NAME)
    echo "PDF_FILE_NAMES:        $PDF_FILE_NAMES"
  fi


  echo


#   break

done


echo


# show the resulting files
# ---------------------------------------------------------

echo
echo "----------------------------"
FILES_NBR=${#MUSICXML_FILE_NAMES[@]}
echo "There are $FILES_NBR handled *.xml files:"
echo
for FILE_NAME in $MUSICXML_FILE_NAMES ; do
  if [ -f $FILE_NAME ]; then
    ls -salGTF $FILE_NAME
  fi
done
echo

echo
echo "----------------------------"
FILES_NBR=${#LILYPOND_FILE_NAMES[@]}
echo "There are $FILES_NBR resulting *.ly files:"
echo
for FILE_NAME in $LILYPOND_FILE_NAMES ; do
  if [ -f $FILE_NAME ]; then
    ls -salGTF $FILE_NAME
  fi
done
echo

echo
FILES_NBR=${#PDF_FILE_NAMES[@]}
echo "----------------------------"
echo "There are $FILES_NBR resulting *.pdf files:"
echo
for FILE_NAME in $PDF_FILE_NAMES ; do
  if [ -f $FILE_NAME ]; then
    ls -salGTF $FILE_NAME
  fi
done
echo


echo


# display the *.xml files not translated to LilyPond
# ---------------------------------------------------------

echo "----------------------------"
echo "Checking translation to LilyPond"
echo

for FILE_NAME in $SUFFIXLESS_FILE_NAMES; do
#   echo "==> $FILE_NAME:"
  if [ -f $FILE_NAME.xml ]; then
    if [ ! -f $FILE_NAME.ly ]; then
      echo "--> $FILE_NAME.xml has not been translated to LilyPond"
      echo
    fi
  fi
done


echo; echo


# display the *.ly filesnot translated to PDF
# ---------------------------------------------------------

echo "----------------------------"
echo "Checking translation to PDF"
echo

for FILE_NAME in $SUFFIXLESS_FILE_NAMES; do
#   echo "==> $FILE_NAME:"
  if [ -f $FILE_NAME.ly ]; then
    if [ ! -f $FILE_NAME.pdf ]; then
      echo "--> $FILE_NAME.ly has not been translated to PDF"
      echo
    fi
  fi
done
