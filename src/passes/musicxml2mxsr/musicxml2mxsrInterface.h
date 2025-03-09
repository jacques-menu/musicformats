/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___musicxml2mxsrInterface___
#define ___musicxml2mxsrInterface___

#include "typedefs.h"

#include "mxsr2msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
void checkDesiredEncoding (
  const std::string& encoding,
  const std::string& desiredEncoding);

//______________________________________________________________________________
void checkSXMLFile (
  SXMLFile      sxmlfile,
  const std::string& context);

//______________________________________________________________________________
/*!
  \brief Converts a MusicXML representation to an MSR.
  \param fileName a file name
  \param msrOpts the MSR options to be used
  \return an error code (\c mfMusicformatsErrorKind::kMusicformatsError_NONE when success)
*/
EXP Sxmlelement musicxmlFile2mxsr (
  const char*        fileName,
  S_mxsr2msrOahGroup mxsr2msrOpts,
  mfPassIDKind       passIDKind,
  const std::string& passDescription);

//______________________________________________________________________________
/*!
  \brief Converts a MusicXML representation to an MSR.
  \param fd a file descriptor
  \param msrOpts the MSR options to be used
  \return an error code (\c mfMusicformatsErrorKind::kMusicformatsError_NONE when success)
*/

EXP Sxmlelement musicxmlFd2mxsr (
  FILE*              fd,
  S_mxsr2msrOahGroup mxsr2msrOpts,
  mfPassIDKind       passIDKind,
  const std::string& passDescription);

//______________________________________________________________________________
/*!
  \brief Converts a MusicXML representation to an MSR.
  \param buff a std::string containing MusicXML code
  \param msrOpts the MSR options to be used
  \return an error code (\c mfMusicformatsErrorKind::kMusicformatsError_NONE when success)
*/
EXP Sxmlelement musicxmlString2mxsr ( // JMI UNUSED SAX ???
  const char*        buffer,
  S_mxsr2msrOahGroup mxsr2msrOpts,
  mfPassIDKind       passIDKind,
  const std::string& passDescription);

//_______________________________________________________________________________
EXP SXMLFile createSXMLFileFromFile (
  const char*        fileName,
  mfPassIDKind       passIDKind,
  const std::string& passDescription);

//_______________________________________________________________________________
EXP SXMLFile createSXMLFileFromFd (
  FILE*              fd,
  mfPassIDKind       passIDKind,
  const std::string& passDescription);

//_______________________________________________________________________________
EXP SXMLFile createSXMLFileFromString (
  const char*        buffer,
  mfPassIDKind       passIDKind,
  const std::string& passDescription);


}

#endif // ___musicxml2mxsrInterface___
