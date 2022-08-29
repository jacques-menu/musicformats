# MusicFormats library

This information is available in french under the name [LISEZ_MOI.md](https://github.com/jacques-menu/musicformats/blob/master/LISEZ_MOI.md).

The [MusicFormats library](https://github.com/jacques-menu/musicformats) is open source software
supplied under the Mozilla Public License 2.0 licence.
It is dedicated to the conversion of music score formats
such as MusicXML, LilyPond, braille music and Guido.
It is written in C++17. Building it from the source code requires a C++ compiler and cmake.

MusicFormats contains converters between these formats such as xml2ly.
Examples of the use of the library to generate output files from scratch are provided.

There is also an interpreter for MFSL (MusicFormats Scripting Language), that can be used
to ease the use and selection of options with the tools provided by MusicFormats.

MusicFormats embeds a version of the [libmusicxml library](https://github.com/grame-cncm/libmusicxml.git),
which supports the MusicXML (textual) format, both for reading and writing.

The documentation contains among other things a user guide :
  https://github.com/jacques-menu/musicformats/blob/master/documentation/MusicFormatsUserGuide/MusicFormatsUserGuide.pdf
and a maintenance guide for developpers :
  https://github.com/jacques-menu/musicformats/blob/master/documentation/MusicFormatsMaintainanceGuide/MusicFormatsMaintainanceGuide.pdf

There are eady-to-use versions of [MusicFormats] for the three main operating systems,
i.e. MacOS™, Linux in its Ubuntu declination and Windows™.
They are in ZIP format and can be downloaded from the repository main page at
https://github.com/jacques-menu/musicformats.
Click on the 'n tags' link at the top of the page to access the various versions.

One can also go directly to:
https://github.com/jacques-menu/musicformats/tags .
Then click on the link to the desired version such as v0.9.65, to access its contents.

Each .zip archive contains:
  - text files containing the version number and release date;
  - the executable tools in the bin subdirectory;
  - binary versions of the library in the lib subdirectory;
  - a PDF introduction to MusicXML;
  - a PDF MusicFormats user guide.

These ready-to-use versions can be accessed directly with URLs such as:
  https://github.com/jacques-menu/musicformats/releases/tag/v0.9.65 .
Replace v0.9.65 by the version number for the desired operations system.
