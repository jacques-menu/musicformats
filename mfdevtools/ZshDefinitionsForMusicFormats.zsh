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

# These definitions can be used in your bash setup
# and modified ad libitum to suit your needs.

alias zrc='. ${HOME}/.zrc'

# LaTeX
#----------------------------------------------

export PATH=/Library/TeX/texbin:$PATH

#	my actual work directory
#----------------------------------------------

# MY_WORK_DIR=${HOME}/JMI_Developpement
MY_WORK_DIR=/Volumes/JMI_Volume/JMI_Developpement

# dev branch
#----------------------------------------------

MUSICFORMATS_DIR=${MY_WORK_DIR}/musicformats-git-dev
# echo
# echo "==> MUSICFORMATS_DIR = ${MUSICFORMATS_DIR}"
# echo

alias mfdev="cd ${MUSICFORMATS_DIR}"
alias mfd=mfdev

# devtools
#----------------------------------------------

export PATH=${MUSICFORMATS_DIR}/devtools:$PATH

# working on dev branch
#----------------------------------------------

# DTDS_DIR=${MUSICFORMATS_DIR}/schemas/MusicXML/musicxml-4.0/
DTDS_DIR=${MUSICFORMATS_DIR}/libmusicxml/dtds
alias dtds="cd ${DTDS_DIR}"
SCHEMA_DIR=${DTDS_DIR}/3.1/schema
alias schem="cd ${SCHEMA_DIR}"

LXML_DIR=${MUSICFORMATS_DIR}/libmusicxml

SRC_DIR=${MUSICFORMATS_DIR}/src

LXML_SRC_DIR=${LXML_DIR}/src
LXML_INTERFACE_DIR=${LXML_SRC_DIR}/interface

LANGUAGES_DIR=${MUSICFORMATS_DIR}/src/languages
alias lang="cd ${LANGUAGES_DIR}"

DEVTOOLS_DIR=${MUSICFORMATS_DIR}/devtools
alias tool="cd ${DEVTOOLS_DIR}"

BUILD_DIR=${MUSICFORMATS_DIR}/build
alias build="cd ${BUILD_DIR}"

INCLUDE_DIR_NAME=include
INCLUDE_DIR=${MUSICFORMATS_DIR}/${INCLUDE_DIR_NAME}
alias inc="cd ${INCLUDE_DIR}"

alias rmbuild='cd ${MUSICFORMATS_DIR}/build ; rm -r bin lib libdir; ls -sal'

DOC_DIR=${MUSICFORMATS_DIR}/documentation

SERVICES_DIR=${MUSICFORMATS_DIR}/src/services

LXML_SAMPLES_DIR=${LXML_DIR}/samples
CLI_SAMPLES_DIR=${MUSICFORMATS_DIR}/src/clisamples

MXML_FILES_DIR=${MUSICFORMATS_DIR}/musicxmlfiles

export PATH=${MUSICFORMATS_DIR}/build/bin:$PATH

alias lxmlsrc="cd ${LXML_SRC_DIR}"
alias lxmlinter="cd ${LXML_INTERFACE_DIR}"

alias build="cd ${BUILD_DIR}"

alias doc="cd ${DOC_DIR}"

alias com="cd ${DOC_DIR}/CommonLaTeXFiles"
alias maint="cd ${DOC_DIR}/MusicFormatsMaintainanceGuide"
alias user="cd ${DOC_DIR}/MusicFormatsUserGuide"
alias api="cd ${DOC_DIR}/MusicFormatsAPIGuide"
alias graph="cd ${DOC_DIR}/graphics"

alias intro="cd ${DOC_DIR}/IntroductionToMusicXML"

alias clis="cd ${CLI_SAMPLES_DIR}"
alias lxmlsamp="cd ${LXML_SAMPLES_DIR}"

alias serv="cd ${SERVICES_DIR}"


# choosing the xml2ly MacOS version
#----------------------------------------------

alias useversion='export PATH=~/MusicFormatsForMacOS/bin:$PATH ; type xml2ly'


# validation
#----------------------------------------------

VALIDATION_DIR=${MUSICFORMATS_DIR}/validation

alias valid="cd ${VALIDATION_DIR}"


# source code
#----------------------------------------------

alias src="cd ${SRC_DIR}"

alias mfl="cd ${SRC_DIR}/mflibrary"
alias wae="cd ${SRC_DIR}/wae"
alias oah="cd ${SRC_DIR}/oah"
alias mfc="cd ${SRC_DIR}/components"
alias form="cd ${SRC_DIR}/formats"
alias repr="cd ${SRC_DIR}/representations"
alias pass="cd ${SRC_DIR}/passes"
alias gener="cd ${SRC_DIR}/generators"
alias conv="cd ${SRC_DIR}/converters"
alias interp="cd ${SRC_DIR}/interpreters"

alias msr="cd ${SRC_DIR}/representations/msr"
alias lpsr="cd ${SRC_DIR}/representations/lpsr"
alias bsr="cd ${SRC_DIR}/representations/bsr"
alias mxsr="cd ${SRC_DIR}/representations/mxsr"

alias msdr="cd ${SRC_DIR}/representations/msdr"


# example files
#----------------------------------------------

alias fxml="cd ${MXML_FILES_DIR}"

alias bas="cd ${MXML_FILES_DIR}/basic"
alias orn="cd ${MXML_FILES_DIR}/ornaments"
alias span="cd ${MXML_FILES_DIR}/spanners"
alias harm="cd ${MXML_FILES_DIR}/harmonies"
alias fig="cd ${MXML_FILES_DIR}/figuredbass"
alias grace="cd ${MXML_FILES_DIR}/gracenotes"

alias f31="cd ${MXML_FILES_DIR}/xmlsamples3.1"
alias unof="cd ${MXML_FILES_DIR}/UnofficialTestSuite"
alias regr="cd ${MXML_FILES_DIR}/regression"

alias b2015="cd ${MXML_FILES_DIR}/2015_bana_braille"

alias igno="cd ${MXML_FILES_DIR}/lilypond-ignored"

alias fmfsl="cd ${MUSICFORMATS_DIR}/mfslfiles"

alias fmsdl="cd ${MUSICFORMATS_DIR}/msdlfiles"
alias fbmml="cd ${MUSICFORMATS_DIR}/bmmlfiles"
alias fmei="cd ${MUSICFORMATS_DIR}/meifiles"

alias msdr="cd ${SRC_DIR}/representations/msdr"
alias msdm="cd ${SRC_DIR}/translators/msdl2msr"


# soundnotation
#----------------------------------------------

alias sound='cd ${HOME}/JMI_Developpement/soundnotation'


# EJMA
#----------------------------------------------

alias jazztrad='cd ${HOME}/JMI_Musique/JazzTrad_EJMA_2022_2023'
alias bigband='cd ${HOME}/JMI_Musique/Big-band_EJMA_2022_2023'


# MSDL
#----------------------------------------------

alias msdlc='cd ${SRC_DIR}/converters/msdlconverter'


# MSDR
#----------------------------------------------

alias msdr='cd ${SRC_DIR}/representations/msdr'


# MFSL
#----------------------------------------------

MFSL_DIR=${SRC_DIR}/interpreters/mfslinterpreter
alias mfsld="cd ${MFSL_DIR}"


# iScheme
#----------------------------------------------

ISCM_DIR=${SRC_DIR}/interpreters/ischemeinterpreter
iscmd="cd ${ISCM_DIR}"


# musicxml2ly
#----------------------------------------------

function musix ()
{
#set -x
  DATE=$(date)
  EXECUTABLE=musicxml2ly
#   EXECUTABLE=musicxml2ly_2.23.0-1
  LOG_FILE="z_test_${EXECUTABLE}.log"
  echo "Date: ${DATE}" 2>${LOG_FILE} 1>&2
  type ${EXECUTABLE} 2>${LOG_FILE} 1>&2

  FILE_NAME=${1}
  FILE_BASE_NAME=$(basename "${1}")
  FILENAME_WITHOUT_SUFFIX=${FILE_BASE_NAME/.xml/} # remove .xml suffix
  ADDON_SUFFIX="musicxml2ly"
  INTER="${FILENAME_WITHOUT_SUFFIX}_${ADDON_SUFFIX}.xml"
  LY_OUTPUT="${FILENAME_WITHOUT_SUFFIX}.ly"
  FINAL="${FILENAME_WITHOUT_SUFFIX}_${ADDON_SUFFIX}.ly"
#set -x

  cp -p "${1}" "${INTER}"
  COMMAND="${EXECUTABLE} \"${INTER}\""
  echo "COMMAND = " ${COMMAND}
  eval ${COMMAND} 2>${LOG_FILE} 1>&2

  rm "${INTER}"

  set +x
}


# build MusicFormats
#----------------------------------------------

alias rmcache="cd ${BUILD_DIR}/libdir ; rm *"

#alias rmmakefiles="sudo rm -rf ./libdir/CMakeFiles"
alias rmmakefiles="rm -rf ./libdir/CMakeFiles"

function bmf ()
{
#   set -x
  SCRIPT_NAME=BuildMusicFormats.zsh
  SCRIPT=${DEVTOOLS_DIR}/${SCRIPT_NAME}

  LOGFILE_NAME=${SCRIPT_NAME}.log
  LOGFILE=${BUILD_DIR}/${LOGFILE_NAME}
# 	echo
# 	echo "==> LOGFILE = ${LOGFILE}"
# 	echo

	# execute the SCRIPT

# 	echo "--> Executing ${SCRIPT}"
  zsh ${SCRIPT} $@   #   > ${LOGFILE} 2&>1

	# filter out log results

  egrep \
    --colour=always \
    -i 'BUILD|error|warning|Undefined|referenced from|duplicate symbol|unknown|undeclared|redefinition|expects|multi-character|In file included from|const|unterminated|expected|^' \
    ${LOGFILE} \
    | \
    egrep -v 'forward' | egrep -v 'build' | egrep -v 'Build' | grep -v 'TARGET'
#    \
#     | \
#    egrep -v 'Xcode.app' | egrep -v 'VSTD'

#    -i 'BUILD|error|warning|note:|Undefined|referenced from|duplicate symbol|unknown|undeclared|redefinition' \

#   echo

	# list resulting files

  ls -saltr ${BUILD_DIR}/bin/xml2* ${BUILD_DIR}/bin/msdl* # ${BUILD_DIR}/bin/Mikrokosmos3Wandering

#  ll ${BUILD_DIR}/lib
#   set +x
}

function cbmf ()
{
	clear
	bmf
}

alias cmakall='pushd . ; cd ${BUILD_DIR}/libdir; cmake .. -DALL=on -DAPPLEDEBUG=on ; popd'
#cmake .. -DLILY=on -DBRAILLE=on


# version number
#----------------------------------------------

function smvn ()
{
#  set -x
  SCRIPT_NAME=SetMusicFormatsVersionNumber.zsh
  LOGFILE=${MUSICFORMATS_DIR}/${SCRIPT_NAME}.log
  echo LOGFILE = ${LOGFILE}

  ${DEVTOOLS_DIR}/${SCRIPT_NAME} $@

  echo

#  set +x
}

# version date
#----------------------------------------------

function smvd ()
{
#  set -x
  SCRIPT_NAME=SetMusicFormatsVersionDate.zsh
  LOGFILE=${MUSICFORMATS_DIR}/${SCRIPT_NAME}.log
  echo LOGFILE = ${LOGFILE}

  ${DEVTOOLS_DIR}/${SCRIPT_NAME} $@

  echo

#  set +x
}

# make distributions
#----------------------------------------------

function mmd ()
{
#  set -x
  SCRIPT_NAME=MakeMusicFormatsDistributions.zsh
  LOGFILE=${MUSICFORMATS_DIR}/${SCRIPT_NAME}.log
  echo LOGFILE = ${LOGFILE}

  ${DEVTOOLS_DIR}/${SCRIPT_NAME} $@

  echo

#  ls -saltr ${BUILD_DIR}
#  set +x
}

# clean distributions
#----------------------------------------------

function cmd ()
{
#  set -x
  SCRIPT_NAME=CleanMusicFormatsDistributions.zsh
  LOGFILE=${MUSICFORMATS_DIR}/${SCRIPT_NAME}.log
  echo LOGFILE = ${LOGFILE}

  ${DEVTOOLS_DIR}/${SCRIPT_NAME} $@

  echo

#  ls -saltr ${BUILD_DIR}
#  set +x
}

# check versions
#----------------------------------------------

function checkVersions ()
{
  xml2ly -v
  xml2brl -v
  xml2xml -v
  xml2gmn -v

  Mikrokosmos3Wandering -v
  LilyPondIssue34 -v

  msdlconverter -v
}

# cores
#----------------------------------------------

alias llcores='ls -sal /cores/core.*'
alias rmcores='ls -sal /cores/core.*; sudo rm /cores/core.*; ls -sal /cores/core.*'


# type
#----------------------------------------------

function typ ()
{
  type -s $1
  echo "--"
  type -f $1
}

# rename
#----------------------------------------------

function renam ()
{
  RenameFiles . . $*
}

# grep in sources
#----------------------------------------------

function grh ()
{
#  set -x
  egrep -rn "$1" **/*.h
}

function grc ()
{
#  set -x
  grep -rn "$1" **/*.cpp
}

function grx ()
{
#  set -x
  grep -rn "$1" **/*.xml
}

function grly ()
{
#  set -x
  grep -rn "$1" **/*.ly
}

function grt ()
{
#  set -x
  grep -rn "$1" **/*.tex
}

function grz ()
{
#  set -x
  grep -rn "$1" **/*.zsh
}


# validation
#----------------------------------------------

# create a symbolic link to musicxmlfiles/Makefile in the subdirectories,
# then 'make'

alias maki='clear; make clean; make -i'

alias lly='ll lilypond/*.ly'
alias oly='open lilypond/*.ly'

alias lpdf='ll pdflily/*.pdf'
alias opdf='open pdflily/*.pdf'

alias showlocal='ll *.ly; ll *.pdf; ll *.midi ll *.brf ll *.gmn'
alias rmlocal='rm *.ly; rm *.pdf; rm *.midi rm *.brf rm *.gmn'


# indentation
#----------------------------------------------

function checkGIndenterSrc ()
{
  for F in ${SRC_DIR}/*/* ${SRC_DIR}/*/*/*; do
    if [ ! -d ${F} ]; then
#      echo "===> ${F}:"
      INCRS=$(grep -c '++gIndenter' ${F})
      DECRS=$(grep -c '\-\-gIndenter' ${F})
      if [ ${INCRS} -ne ${DECRS} ]; then
        echo "${F}: INCRS: ${INCRS}, DECRS: ${DECRS}. DIFFERENCE: $(expr ${INCRS} - ${DECRS} )"
        echo
      fi
    fi
  done
}
alias cgis='checkGIndenterSrc'

function checkGIndenterFile ()
{
  for F in ${1}; do
    if [ ! -d ${F} ]; then
#      echo "===> ${F}:"
      INCRS=$(grep -c '++gIndenter' ${F})
      DECRS=$(grep -c '\-\-gIndenter' ${F})
      if [ ${INCRS} -ne ${DECRS} ]; then
        echo "${F}: INCRS: ${INCRS}, DECRS: ${DECRS}. DIFFERENCE: $(expr ${INCRS} - ${DECRS} )"
        echo
      fi
    fi
  done
}
alias cgif='checkGIndenterFile'

function checkGIndenterCrement ()
{
  for F in ${SRC_DIR}/*/* ${SRC_DIR}/*/*/*; do
    if [ ! -d ${F} ]; then
#      echo "===> ${F}:"
      INCRS=$(grep -c gIndenter.increment ${F})
      DECRS=$(grep -c gIndenter.decrement ${F})
      if [ ${INCRS} -ne ${DECRS} ]; then
        echo "${F}: INCRS: ${INCRS}, DECRS: ${DECRS}. DIFFERENCE: $(expr ${INCRS} - ${DECRS} )"
        echo
      fi
    fi
  done
}
alias cgic='checkGIndenterCrement'

### dupauvre
# CheckGIndenterInFile translators/lpsr2lilypond/lpsr2lilypondTranslator.cpp  < translators/lpsr2lilypond/lpsr2lilypondTranslator.cpp > res.txt


# xml2ly
#----------------------------------------------

alias xy='xml2ly'
alias cxy='clear; xml2ly'

alias xlv='xml2ly -version'
alias xlq='xml2ly -query'
alias xlf='xml2ly -find'

alias xl='xml2ly -lilypond-generation-infos'
alias xlh='xml2ly -h | more'
alias xla='xml2ly -auto-output-file-name -lilypond-generation-infos'
alias cxla='clear ; xml2ly -auto-output-file-name -lilypond-generation-infos'

alias ixl='xml2ly -insider -lilypond-generation-infos'
alias ixla='xml2ly -insider -auto-output-file-name -lilypond-generation-infos'
alias cixla='clear; ixla'
alias dxla='lldb xml2ly -- -auto-output-file-name -lilypond-generation-infos'
alias dixla='lldb xml2ly -- -insider -auto-output-file-name -lilypond-generation-infos'
alias cdxla='clear; dxla'
alias cdixla='clear; dixla'

alias ixlh='xml2ly -insider -h | more'
alias dxl='lldb xml2ly -- -lilypond-generation-infos $*'
alias dixl='lldb xml2ly -- -insider -lilypond-generation-infos $*'

alias mzxb='more z_test_xml2brl.log'
alias tzxb='tail -20 z_test_xml2brl.log'


# HelloWorld
#----------------------------------------------

alias xlbh='xl basic/HelloWorld.xml'
alias xlbhf='xl basic/HelloWorld.xml -auto-output-file-name'
alias cxlbhf='clear; xl basic/HelloWorld.xml -auto-output-file-name'
alias ixlbh='ixl basic/HelloWorld.xml'
alias dixlbh='dixl basic/HelloWorld.xml'

alias dxlbh='dxl basic/HelloWorld.xml'

alias xbbh='xb basic/HelloWorld.xml'
alias ixbbh='ixb basic/HelloWorld.xml'
alias dixbbh='dixb basic/HelloWorld.xml'

alias xxbh='xx basic/HelloWorld.xml'
alias ixxbh='ixx basic/HelloWorld.xml'
alias dixxbh='dixx basic/HelloWorld.xml'

alias xgbh='xg basic/HelloWorld.xml'
alias ixgbh='ixg basic/HelloWorld.xml'
alias dixgbh='dixg basic/HelloWorld.xml'

alias xlbh16='xl basic/HelloWorld_UTF_16.xml'


# xml2brl
#----------------------------------------------

alias xbq='xml2brl -query'
alias xbf='xml2brl -find'

alias xb='xml2brl -use-encoding-in-file-name -utf8d'
alias xbh='xb -h | more'
alias xibh='xb -insider -h | more'
alias xba='xb -auto-output-file-name'
alias cxba='clear ; xb -auto-output-file-name'

alias ixb='xb -insider'
alias ixba='xb -insider -auto-output-file-name'
alias dixba='lldb xml2brl -- -insider -auto-output-file-name'

alias dxb='lldb xml2brl -- $*'
alias dixb='lldb xml2brl -- -insider $*'

alias mzxb='more z_test_xml2brl.log'
alias tzxb='tail -20 z_test_xml2brl.log'

alias brl='xml2brl -use-encoding-in-file-name -utf8d'


# xml2xml
#----------------------------------------------

alias xxq='xml2xml -query'
alias xxf='xml2xml -find'

alias xx='xml2xml'
alias xxh='xx -h | more'
alias xxa='xx -auto-output-file-name'
alias cxxa='clear ; xx -auto-output-file-name'

alias ixx='xx -insider'
alias ixxa='xx -insider -auto-output-file-name'
alias dixxa='lldb xx -- -insider -auto-output-file-name'

alias ixxh='xx -insider -h | more'
alias xxm='xx | more'

alias dxx='lldb xml2xml -- $*'
alias dixx='lldb xml2xml -- -insider $*'

alias mzxx='more z_test_xml2xml.log'
alias tzxx='tail -20 z_test_xml2xml.log'


# xml2gmn
#----------------------------------------------

alias xgq='xml2gmn -query'
alias xgf='xml2gmn -find'

alias xg='xml2gmn'
alias xgh='xg -h | more'
alias xga='xg -auto-output-file-name'
alias cxga='clear ; xg -auto-output-file-name'

alias ixg='xg -insider'
alias ixga='xg -insider -auto-output-file-name'
alias dixga='lldb xg -- -insider -auto-output-file-name'

alias ixgh='xg -insider -h | more'
alias xgm='xg | more'

alias dxg='lldb xml2gmn -- $*'
alias dixg='lldb xml2gmn -- -insider $*'

alias mzxg='more z_test_xml2gmn.log'
alias tzxg='tail -20 z_test_xml2gmn.log'


# Mikrokosmos3Wandering
#----------------------------------------------

alias mkk='Mikrokosmos3Wandering'

alias mkkq='Mikrokosmos3Wandering -query'
alias mkkf='Mikrokosmos3Wandering -find'

alias mkkl='Mikrokosmos3Wandering -lilypond'
alias mkkb='Mikrokosmos3Wandering -braille'
alias mkkx='Mikrokosmos3Wandering -musicxml'
alias mkkg='Mikrokosmos3Wandering -guido'

alias mkklh='Mikrokosmos3Wandering -lilypond -h'
alias mkkbh='Mikrokosmos3Wandering -braille -h'
alias mkkmh='Mikrokosmos3Wandering -musicxml -h'
alias mkkgh='Mikrokosmos3Wandering -guido -h'

alias mkklq='Mikrokosmos3Wandering -lilypond -query'
alias mkkbq='Mikrokosmos3Wandering -braille -query'
alias mkkmq='Mikrokosmos3Wandering -musicxml -query'
alias mkkgq='Mikrokosmos3Wandering -guido -query'

alias mkklf='Mikrokosmos3Wandering -lilypond -find'
alias mkkbf='Mikrokosmos3Wandering -braille -find'
alias mkkmf='Mikrokosmos3Wandering -musicxml -find'
alias mkkgf='Mikrokosmos3Wandering -guido -find'

alias imkkl='Mikrokosmos3Wandering -insider -lilypond'
alias imkkb='Mikrokosmos3Wandering -insider -braille'
alias imkkx='Mikrokosmos3Wandering -insider -musicxml'
alias imkkg='Mikrokosmos3Wandering -insider -guido'

alias dimkkl='lldb -- Mikrokosmos3Wandering -insider -lilypond'
alias dimkkb='lldb -- Mikrokosmos3Wandering -insider -braille'
alias dimkkx='lldb -- Mikrokosmos3Wandering -insider -musicxml'
alias dimkkg='lldb -- Mikrokosmos3Wandering -insider -guido'

alias mkkf='Mikrokosmos3Wandering -find'


# LilyPondIssue34
#----------------------------------------------

alias li34='LilyPondIssue34'

alias li34q='LilyPondIssue34 -query'
alias li34f='LilyPondIssue34 -find'

alias li34l='LilyPondIssue34 -lilypond'
alias li34b='LilyPondIssue34 -braille'
alias li34x='LilyPondIssue34 -musicxml'
alias li34g='LilyPondIssue34 -guido'

alias li34lh='LilyPondIssue34 -lilypond -h'
alias li34bh='LilyPondIssue34 -braille -h'
alias li34mh='LilyPondIssue34 -musicxml -h'
alias li34gh='LilyPondIssue34 -guido -h'

alias li34lq='LilyPondIssue34 -lilypond -query'
alias li34bq='LilyPondIssue34 -braille -query'
alias li34mq='LilyPondIssue34 -musicxml -query'
alias li34gq='LilyPondIssue34 -guido -query'

alias li34lf='LilyPondIssue34 -lilypond -find'
alias li34bf='LilyPondIssue34 -braille -find'
alias li34mf='LilyPondIssue34 -musicxml -find'
alias li34gf='LilyPondIssue34 -guido -find'

alias ili34l='LilyPondIssue34 -insider -lilypond'
alias ili34b='LilyPondIssue34 -insider -braille'
alias ili34x='LilyPondIssue34 -insider -musicxml'
alias ili34g='LilyPondIssue34 -insider -guido'

alias dili34l='lldb -- LilyPondIssue34 -insider -lilypond'
alias dili34b='lldb -- LilyPondIssue34 -insider -braille'
alias dili34x='lldb -- LilyPondIssue34 -insider -musicxml'
alias dili34g='lldb -- LilyPondIssue34 -insider -guido'

alias li34f='LilyPondIssue34 -find'


# msdl
#----------------------------------------------

alias msdlg='msdl -generate guido'
alias msdll='msdl -generate lilypond'
alias msdlb='msdl -generate braille'
alias msdlx='msdl -generate musicxml'
alias msdlm='msdl -generate midi'

# imsdl*h
alias imsdlgh='msdl -insider -help -generate guido'
alias imsdllh='msdl -insider -help -generate lilypond'
alias imsdlbh='msdl -insider -help -generate braille'
alias imsdlxh='msdl -insider -help -generate musicxml'
alias imsdlmh='msdl -insider -help -generate midi'

# msdl*h
alias msdlh='msdl -help'

alias msdlgh='msdl -help -generate guido'
alias msdllh='msdl -help -generate lilypond'
alias msdlbh='msdl -help -generate braille'
alias msdlxh='msdl -help -generate musicxml'
alias msdlmh='msdl -help -generate midi'

# imsdl*
alias imsdl='msdl -insider'

alias imsdlg='msdl -insider -generate guido'
alias imsdll='msdl -insider -generate lilypond'
alias imsdlb='msdl -insider -generate braille'
alias imsdlx='msdl -insider -generate musicxml'
alias imsdlm='msdl -insider -generate midi'

# imsdl*h
alias imsdlh='msdl -insider -help'

alias imsdlgh='msdl -insider -help -generate guido'
alias imsdllh='msdl -insider -help -generate lilypond'
alias imsdlbh='msdl -insider -help -generate braille'
alias imsdlxh='msdl -insider -help -generate musicxml'
alias imsdlmh='msdl -insider -help -generate midi'

# dmsdl*
alias dmsdlg='lldb -- msdl -generate guido'
alias dmsdll='lldb -- msdl -generate lilypond'
alias dmsdlb='lldb -- msdl -generate braille'
alias dmsdlx='lldb -- msdl -generate musicxml'
alias dmsdlm='lldb -- msdl -generate midi'

# dimsdl*
alias dimsdlg='lldb -- msdl -insider -generate guido'
alias dimsdll='lldb -- msdl -insider -generate lilypond'
alias dimsdlb='lldb -- msdl -insider -generate braille'
alias dimsdlx='lldb -- msdl -insider -generate musicxml'
alias dimsdlm='lldb -- msdl -insider -generate midi'


# mkkg -generate-bars -generate-stem -generate-comments
# xml2gmn basic/HelloWorld.xml -generate-bars -generate-stem -generate-comments -d=msr


# mfsl
#----------------------------------------------
alias mfslh='mfsl -help'

alias mfslq='mfsl -query'
alias mfslf='mfsl -find'


# git
#----------------------------------------------

alias gb='git branch'
alias gc='git checkout'
alias gp='git push'
alias gbi='git bisect'

alias zrc='. ~/.zshrc'
alias cpbashdefs='cp -p ~/local/BashDefinitionsForMusicFormats.zsh ${DEVTOOLS_DIR}'

alias addall=AddAllMusicFormatsFilesToTheLocalRepository.zsh


#----------------------------------------------

function wclf ()
{
  cd "${SRC_DIR}"
  echo "SRC_DIR = ${SRC_DIR}"

  (wc -l **/* | sort -n) 2> /dev/null    # grep -v ‘wc: ’
}

function wcl ()
{
  cd "${SRC_DIR}"
  echo "SRC_DIR = ${SRC_DIR}"

  (wc -l **/* | sort -n | tail -n 20) 2> /dev/null
}

function wcla ()
{
  cd "${SRC_DIR}"

  DIRS_LIST=

  for DIR in $(ls); do
    DIRS_LIST="${DIRS_LIST} ${DIR}/*"
  done
  echo "DIRS_LIST = ${DIRS_LIST}"

  wc -l ${DIRS_LIST}
}


