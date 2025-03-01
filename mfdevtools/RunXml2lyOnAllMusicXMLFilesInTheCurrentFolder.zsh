#!/bin/zsh

# clean the current directory from spurious files
# ---------------------------------------------------------

rm *.ly *.pdf *.midi

# translate all MusicXML files

for FILE in $(ls *.xml); do
  echo "==> ${FILE}:"

  FILE_NAME=${FILE}
  FILE_BASE_NAME=$(basename "${FILE}")
  FILENAME_WITHOUT_SUFFIX=${FILE_BASE_NAME/.xml/} # remove .xml suffix


  LILYPOND_FILE_NAME="${FILENAME_WITHOUT_SUFFIX}.ly"

  set -x
  xml2ly -output-file-name ${LILYPOND_FILE_NAME} ${FILE_NAME}
  set +x

  ls -sal ${LILYPOND_FILE_NAME}

  PDF_FILE_NAME="${FILENAME_WITHOUT_SUFFIX}.pdf"

  set -x
  lilypond ${LILYPOND_FILE_NAME}
  set +x

  ls -sal ${PDF_FILE_NAME}

  echo "<<< ==================="
  echo

#     break

done


# show the resulting files
# ---------------------------------------------------------

#   set -x
  FILES_NBR=$(ls *.xml | wc -l | sed 's/ //g')
  echo "----------------------------"
  echo "There are ${FILES_NBR} *.xml files:"
  echo
  ls -salGTF *.xml
  echo
#   set +x

#   set -x
  FILES_NBR=$(ls *.ly | wc -l | sed 's/ //g')
  echo "----------------------------"
  echo "There are ${FILES_NBR} *.ly files:"
  echo
  ls -salGTF -sal *.ly
  echo
#   set +x

#   set -x
  FILES_NBR=$(ls *.pdf | wc -l | sed 's/ //g')
  echo "----------------------------"
  echo "There are ${FILES_NBR} *.pdf files:"
  echo
  ls -salGTF *.pdf
#   set +x


echo; echo


# display *.xml not translated to LilyPond
# ---------------------------------------------------------

echo "----------------------------"
echo "Checking translation to LilyPond"
echo

for FILE in $(ls *.xml); do
#   echo "==> ${FILE}:"

  FILE_NAME=${FILE}
  FILE_BASE_NAME=$(basename "${FILE}")
  FILENAME_WITHOUT_SUFFIX=${FILE_BASE_NAME/.xml/} # remove .xml suffix


  LILYPOND_FILE_NAME="${FILENAME_WITHOUT_SUFFIX}.ly"

  if [ ! -f ${LILYPOND_FILE_NAME} ]; then
    echo "--> ${FILE_NAME} has not been translated to LilyPond"
    echo
  fi
done


# display *.ly not translated to PDF
# ---------------------------------------------------------

echo "----------------------------"
echo "Checking translation to PDF"
echo

for FILE in $(ls *.ly); do
#   echo "==> ${FILE}:"

  FILE_NAME=${FILE}
  FILE_BASE_NAME=$(basename "${FILE}")
  FILENAME_WITHOUT_SUFFIX=${FILE_BASE_NAME/.ly/} # remove .ly suffix


  PDF_FILE_NAME="${FILENAME_WITHOUT_SUFFIX}.pdf"

  if [ ! -f ${PDF_FILE_NAME} ]; then
    echo "--> ${FILE_NAME} has not been translated to PDF"
    echo
  fi
done

