#!/bin/zsh

# create a LaTeX file for the user guide and MusicFormats figures from the maintainance one

echo "Deriving MSRScoreRepresentation from MSRClassesHierarchyPicture"

sed 's/msr//g' MSRClassesHierarchyPicture.tex \
  | \
sed 's/The MSR classes hierarchy/The MSR music score representation/g' \
  > \
MSRScoreRepresentation.tex

ls -sal MSRClassesHierarchyPicture.tex
ls -sal MSRScoreRepresentation.tex
