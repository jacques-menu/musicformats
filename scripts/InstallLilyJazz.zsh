#!/bin/zsh

# script d'installation des polices lilyjazz pour LilyPond
# installé avec MacPorts dans /opt/local

LILYPOND_VERSION=2.25.1
echo "LILYPOND_VERSION: ${LILYPOND_VERSION}"
echo


DOWNLOAD_DIR=/Users/jacquesmenu/JMI_Downloads

cd ${DOWNLOAD_DIR}
echo -n "Dossier où lilyjazz a été téléchargé: " ; pwd
echo


cd lilyjazz-master
echo "On descend dans: ${PWD}, contenant:"
ls -sal
echo


FONTS_DIR=/opt/local/share/lilypond/${LILYPOND_VERSION}/fonts
echo "FONTS_DIR pour l'installation: ${FONTS_DIR}"
echo


# set -x

sudo cp -p \
  otf/lilyjazz-*.otf \
    ${FONTS_DIR}/otf

sudo cp -p \
  supplementary-files/*/lilyjazz-*.otf \
    ${FONTS_DIR}/otf

sudo cp -p \
  svg/lilyjazz-*.* \
    ${FONTS_DIR}/svg

# set +x

echo "Contenu résultant de: ${FONTS_DIR}/otf:"
ls -sal ${FONTS_DIR}/otf
echo

echo "Contenu résultant de: ${FONTS_DIR}/svg:"
ls -sal ${FONTS_DIR}/svg
