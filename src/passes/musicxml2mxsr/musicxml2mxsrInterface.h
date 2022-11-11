/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#ifndef ___musicxml2mxsrInterface___
#define ___musicxml2mxsrInterface___

#include "typedefs.h"

#include "mxsrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
void checkDesiredEncoding (
  const string& encoding,
  const string& desiredEncoding);

//______________________________________________________________________________
void checkSXMLFile (
  SXMLFile      sxmlfile,
  const string& context);

//______________________________________________________________________________
/*!
  \brief Converts a MusicXML representation to an MSR.
  \param fileName a file name
  \param msrOpts the MSR options to be used
  \return an error code (\c mfMusicformatsErrorKind::kMusicformatsError_NO_ when success)
*/
EXP Sxmlelement musicxmlFile2mxsr (
  const char*    fileName,
  S_mxsrOahGroup musicxmlOpts);

//______________________________________________________________________________
/*!
  \brief Converts a MusicXML representation to an MSR.
  \param fd a file descriptor
  \param msrOpts the MSR options to be used
  \return an error code (\c mfMusicformatsErrorKind::kMusicformatsError_NO_ when success)
*/

EXP Sxmlelement musicxmlFd2mxsr (
  FILE*          fd,
  S_mxsrOahGroup musicxmlOpts);

//______________________________________________________________________________
/*!
  \brief Converts a MusicXML representation to an MSR.
  \param buff a string containing MusicXML code
  \param msrOpts the MSR options to be used
  \return an error code (\c mfMusicformatsErrorKind::kMusicformatsError_NO_ when success)
*/
EXP Sxmlelement musicxmlString2mxsr (
  const char*    buffer,
  S_mxsrOahGroup musicxmlOpts,
  const string&  passNumber,
  const string&  passDescription);

//_______________________________________________________________________________
EXP SXMLFile createSXMLFileFromFile (
  const char*   fileName,
  const string& passNumber,
  const string& passDescription);

//_______________________________________________________________________________
EXP SXMLFile createSXMLFileFromFd (
  FILE*         fd,
  const string& passNumber,
  const string& passDescription);

//_______________________________________________________________________________
EXP SXMLFile createSXMLFileFromString (
  const char*   buffer,
  const string& passNumber,
  const string& passDescription);


}

#endif
