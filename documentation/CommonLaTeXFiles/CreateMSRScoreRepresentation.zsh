#!/bin/zsh

# create a LaTeX file for the user guide from the maintainance one

sed 's/msr//g' MSRClassesHierarchyPicture.tex \
  | \
sed 's/The MSR classes hierarchy/The MSR music score representation/g' \
  > \
MSRScoreRepresentation.tex

