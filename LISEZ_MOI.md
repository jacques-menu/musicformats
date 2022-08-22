# Librarie MusicFormats

La [librarie MusicFormats](https://github.com/jacques-menu/musicformats)
est un logiciel ouvert mis à disposition sous la licence Mozilla Public License 2.0.
Elle dédiée à la conversion de formats de description de partitions musicales
comme MusicXML, LilyPond, musique braille et Guido.
Elle est écrite en C++17. Pour la construire à partir du code source,
il faut un compilateur C++ et l'outil cmake.

MusicFormats contient des  convertisseurs entre ces formats comme xml2ly.
Des examples de l'emploi de la librarie pour generer des fichier ex-nihilo sont fournis.

Il y a aussi un interpréteur for MFSL (MusicFormats Scripting Language),
qui peut être utilisé pour faciliter l'emploi et la selection des options
avec les outils fournis par MusicFormats.

MusicFormats contient une version de la [librarie libmusicxml2],
disponible à (https://github.com/grame-cncm/libmusicxml.git),
qui permet la lecture et l'écriture du format MusicXML (textuel).

La documentation contient entre autres un guide d'utilisation en anglais :
  https://github.com/jacques-menu/musicformats/blob/master/documentation/MusicFormatsUserGuide/MusicFormatsUserGuide.pdf
et un guide de maintenance pour les développeurs :
  https://github.com/jacques-menu/musicformats/blob/master/documentation/MusicFormatsMaintainanceGuide/MusicFormatsMaintainanceGuide.pdf

Des versions directement utilisables de [MusicFormats]
pour les trois systèmes d'exploitation principaux,
soit MacOS™, Linux dans sa déclinaison Ubuntu et Windows™,
peuvent être téléchargées depuis la page d'accueil du dépôt GitHub à
https://github.com/jacques-menu/musicformats.
Sous 'Releases' dans la colonne de droite,
cliquer sur le lien 'n tags' link pour accéder aux diverses versions.
On peut aussi aller directement à :
https://github.com/jacques-menu/musicformats/tags
Ensuite cliquer sur le lien vers la version voulue, comme v0.9.65, pour accéder à son contenu.

Chaque archive .zip contient:
  - a fichier texte contenant le numéro de version;
  - les outils exécutables dans le sous-dossier build/bin;
  - des versions binaires de la librairie dans le sous-dossier build/lib;
  - un  introduction à MusicXML en PDF;
  - le guide de l'utilisateur de MusicFormats en PDF.

Ces distributions peuvent être accédées directement avec des URL comme:
https://github.com/jacques-menu/musicformats/releases/download/v0.9.65/musicformats-macos-v0.9.65-distrib.zip.
https://github.com/jacques-menu/musicformats/releases/download/v0.9.65/musicformats-linux-v0.9.65-distrib.zip.
https://github.com/jacques-menu/musicformats/releases/download/v0.9.65/musicformats-windows-v0.9.65-distrib.zip.
Remplacer v0.9.65 par le numéro de version pour le système d'exploitation désiré.
