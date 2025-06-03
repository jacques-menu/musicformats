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


function usage ()
  {
    echo "An argument is optional."
    echo "Usage is:"
    echo "  $0 ( | optionsString )"
    exit 1
  }


# the MusicFormats directory
export MUSICFORMATS_DIR=${HOME}/JMI_DEVELOPMENT/musicformats-git-dev
echo "--> MUSICFORMATS_DIR: ${MUSICFORMATS_DIR}"
echo


# set the MusicFormats variables
. ${MUSICFORMATS_DIR}/mfdevtools/SetMusicFormatsVariables.zsh


# Write all output to logfile
#exec > ${MUSICFORMATS_DIR}/$(basename $0).log 2>&1


echo "==> PWD is:"
pwd
echo


# MFDEVTOOLS_DIR=${MUSICFORMATS_DIR}/mfdevtools
# echo "MFDEVTOOLS_DIR:          $MFDEVTOOLS_DIR"
# echo
#
# cd ${MFDEVTOOLS_DIR}
# pwd
# echo


# get the argument if any
# -----------------------------------------

case "$#" in
  0)
    OPTIONS_STRING=""
    ;;

  1)
    OPTIONS_STRING="$1"
  ;;

  *)
    usage
  ;;
esac

echo "OPTIONS_STRING : ${OPTIONS_STRING}"
echo


# clean the current directory from previously generaterated files if any
# ---------------------------------------------------------

rm -f *.ly *.pdf *.midi *.log


# select which files to translate
# ---------------------------------------------------------

# echo '$#:                       ' $# " argument(s)"
#
# i=0
# while [ $i -ne $# ]; do
#   echo "$i: argv [$i]"
#   FILES+=argv[$i]
#   i=$(($i+1))
# done
#
# for FILE_NAME in $FILES ; do
#   echo
#   echo "----------------------------"
#   echo "==> FILE_NAME: $FILE_NAME:"
#   echo
# done
#

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



# run xml2ly on a file
# ---------------------------------------------------------

function RunXml2lyOnFile ()
{
  FILE_NAME=$1
  echo
  echo "----------------------------"
  echo "==> RunXml2lyOnFile() - FILE_NAME: $FILE_NAME:"


  # handle file names

  FILE_BASE_NAME=$(basename "$FILE_NAME")
  echo "FILE_BASE_NAME:            $FILE_BASE_NAME"

  SUFFIXLESS_FILE_NAME=$(echo "$FILE_BASE_NAME" | cut -f 1 -d '.')
  echo "SUFFIXLESS_FILE_NAME:      $SUFFIXLESS_FILE_NAME"

  SUFFIXLESS_FILE_NAMES+=($SUFFIXLESS_FILE_NAME)
  echo "SUFFIXLESS_FILE_NAMES:     $SUFFIXLESS_FILE_NAMES"

  MUSICXML_FILE_NAME="$SUFFIXLESS_FILE_NAME.xml"
    # in case the argument contains only 'FOO.', for example
  echo "MUSICXML_FILE_NAME:        $MUSICXML_FILE_NAME"

  MUSICXML_FILE_NAMES_ARRAY=($MUSICXML_FILE_NAMES_ARRAY "$MUSICXML_FILE_NAME")
  echo "MUSICXML_FILE_NAMES_ARRAY: $MUSICXML_FILE_NAMES_ARRAY"


  echo


  # run xmlly

  echo; echo "--> running xml2ly on $MUSICXML_FILE_NAME"
  LILYPOND_FILE_NAME="$SUFFIXLESS_FILE_NAME.ly"
  echo "LILYPOND_FILE_NAME:      $LILYPOND_FILE_NAME"

  xml2ly -lilypond-run-date -lilypond-generation-infos ${OPTIONS_STRING} -output-file-name $LILYPOND_FILE_NAME $MUSICXML_FILE_NAME
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
}


# HandleAllMusicXMLFilesInCurrentDirectory()
# ---------------------------------------------------------

function HandleAllMusicXMLFilesInCurrentDirectory ()
{
  echo "HandleAllMusicXMLFilesInCurrentDirectory()"
  echo "The MusicXML files to be handled are:"
  ls -salGTF *.xml
  echo

  FILES_NBR=$(ls *.xml | wc -l)

  echo "There are $FILES_NBR MusicXML files to be translated"
  echo

  for FILE_NAME in ls *.xml ; do
    echo
    echo "----------------------------"
    echo "==> FILE_NAME: $FILE_NAME:"

    RunXml2lyOnFile ""$FILE_NAME
  done
}


# HandletheMusicXMLFilesPassedAsArguments()
# ---------------------------------------------------------

function HandletheMusicXMLFilesPassedAsArguments ()
{
  echo "HandletheMusicXMLFilesPassedAsArguments()"
  echo "There are ${#[@]} arguments"
  echo '$@:                       ' $@

  FILES_NBR=${#[@]}
  echo "There are $FILES_NBR files to be translated"
  echo

  for FILE_NAME in $@ ; do
    echo
    echo "----------------------------"
    echo "==> FILE_NAME: $FILE_NAME:"

    RunXml2lyOnFile $FILE_NAME
  done
}


# show the resulting files
# ---------------------------------------------------------

function DisplayTheResultingFiles ()
{
  echo
  echo "----------------------------"
  echo "==> MUSICXML_FILE_NAMES_ARRAY: $MUSICXML_FILE_NAMES_ARRAY:"
  echo "----------------------------"
  FILES_NBR=${#MUSICXML_FILE_NAMES_ARRAY[@]}
  echo "There are $FILES_NBR handled MusicXML files:"
  echo
  for FILE_NAME in $MUSICXML_FILE_NAMES_ARRAY ; do
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
}


# display the *.xml files not translated to LilyPond
# ---------------------------------------------------------

function DisplayTheNotTranslatedFiles ()
{
  # display the *.xml files not translated to LilyPond

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
}


# Display the results
# ---------------------------------------------------------

function DisplayTheResults ()
{
  DisplayTheResultingFiles

  DisplayTheNotTranslatedFiles
}


# select which files to translate
# ---------------------------------------------------------

# if (($# == 0 )); then
  # no arguments, all *.xml files in the current directory will be translated
  HandleAllMusicXMLFilesInCurrentDirectory
# else
  # only the files in the arguments will be translated
#   HandletheMusicXMLFilesPassedAsArguments $@
# fi


# Display the results
# ---------------------------------------------------------

DisplayTheResults
