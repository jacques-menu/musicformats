# MusicFormats library

This information is available in french under the name [LISEZ_MOI.md](https://github.com/jacques-menu/musicformats/blob/master/LISEZ_MOI.md).

The [MusicFormats library](https://github.com/jacques-menu/musicformats) is open source software
supplied under the Mozilla Public License 2.0 licence.
It is dedicated to the conversion of music score formats
such as MusicXML, LilyPond, braille music and Guido.
It is written in C++20. Building it from the source code requires a C++ compiler and `cmake`.

MusicFormats contains converters between these formats such as xml2ly.
Examples of the use of the library to generate output files from scratch are provided.

There is also an interpreter for MFSL (MusicFormats Scripting Language), that can be used
to ease the use and selection of options with the tools provided by MusicFormats.

MusicFormats embeds a version of the [libmusicxml library](https://github.com/grame-cncm/libmusicxml.git),
which supports the MusicXML (textual) format, both for reading and writing.

To build the library, use:

    cd <local MusicFormats repository

    cmake -S cmakefiles -B build ; cd build ; make ; ls -salR .

The documentation contains among other things a user guide :
  https://github.com/jacques-menu/musicformats/blob/master/documentation/MusicFormatsUserGuide/MusicFormatsUserGuide.pdf
and a maintenance guide for developpers :
  https://github.com/jacques-menu/musicformats/blob/master/documentation/MusicFormatsMaintainanceGuide/MusicFormatsMaintainanceGuide.pdf

The MusicFormats releases contain ready-to-be-used versions of [MusicFormats] for the three main operating systems,
i.e. MacOS™, Linux in its Ubuntu declination and Windows™.
They are in ZIP format and can be downloaded:
  - from the repository 'releases' page at
https://github.com/jacques-menu/musicformats/releases;
  - or manually by clicking on the 'Releases' link in the right pane of the repository's main window..

Each `.zip` archive contains:
  - text files containing the version number and release date;
  - binary versions of the library in the `lib` subdirectory;
  - the executable tools in the `bin` subdirectory;
  - the C++ header files in the `ìnclude`subdirectory;
  - the MusicFormats user guide PDF file.

These ready-to-use versions can be accessed directly with URLs such as:
  https://github.com/jacques-menu/musicformats/releases/tag/v0.9.70.
Replace v0.9.70 by the version number of the desired release.
