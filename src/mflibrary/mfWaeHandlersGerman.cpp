/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfWaeHandlersGerman.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_mfWaeHandlerGerman mfWaeHandlerGerman::create ()
{
  mfWaeHandlerGerman* o =
    new mfWaeHandlerGerman ();
  assert (o != nullptr);
  return o;
}

mfWaeHandlerGerman::mfWaeHandlerGerman ()
{}

mfWaeHandlerGerman::~mfWaeHandlerGerman ()
{}

std::string mfWaeHandlerGerman::asString () const
{
  std::stringstream s;

  s <<
    "MusicFormats warning and error handler for german";

  return s.str ();
}

void mfWaeHandlerGerman::print (std::ostream& os) const
{
  os <<
    "mfWaeHandlerGerman" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mfWaeHandlerGerman& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}


//_______________________________________________________________________________
// pass names

std::string mfWaeHandlerGerman::passIDKindAsString (mfPassIDKind passIDKind) const
{
  std::string result;

  switch (passIDKind) {
    case mfPassIDKind::kMfPassID_UNKNOWN:
      result = "*Pass kMfPassID_UNKNOWN*";
      break;

    case mfPassIDKind::kMfPassID_ALL:
      result = "*Pass kMfPassID_ALL*";
      break;

    case mfPassIDKind::kMfPassID_0:
      result = "Pass 0";
      break;

    case mfPassIDKind::kMfPassID_Optional:
      result = "opt";
      break;

    case mfPassIDKind::kMfPassID_1:
     result = "Pass 1";
      break;

    case mfPassIDKind::kMfPassID_2:
     result = "Pass 2";
      break;
    case mfPassIDKind::kMfPassID_2a:
     result = "Pass 2a";
      break;
    case mfPassIDKind::kMfPassID_2b:
     result = "Pass 2b";
      break;

    case mfPassIDKind::kMfPassID_3:
     result = "Pass 3";
      break;
    case mfPassIDKind::kMfPassID_3a:
     result = "Pass 3a";
      break;
    case mfPassIDKind::kMfPassID_3b:
     result = "Pass 3b";
      break;

    case mfPassIDKind::kMfPassID_4:
     result = "Pass 4";
      break;
    case mfPassIDKind::kMfPassID_4a:
     result = "Pass 4a";
      break;
    case mfPassIDKind::kMfPassID_4b:
     result = "Pass 4b";
      break;

    case mfPassIDKind::kMfPassID_5:
     result = "Pass 5";
      break;
  } // switch

  return result;
}

// std::string mfWaeHandlerGerman::passName (mfPassIDKind passIDKind) const
// {
//   return "Pass 1";
// }

std::string mfWaeHandlerGerman::passOptional () const
{
  return "Pass (optional)";
}

std::string mfWaeHandlerGerman::fullVersion () const
{
  return "full version";
}
std::string mfWaeHandlerGerman::summary () const
{
  return "summary";
}
std::string mfWaeHandlerGerman::names () const
{
  return "names";
}
std::string mfWaeHandlerGerman::slices () const
{
  return "slices";
}

//_______________________________________________________________________________
// quitting after passes
std::string mfWaeHandlerGerman::quittingAfterPass (mfPassIDKind passIDKind) const
{
  std::stringstream s;

  s <<
    "Quitting after pass " <<
    passIDKindAsString (passIDKind) <<
    " as requested";

  return s.str ();
}

//_______________________________________________________________________________
// OAH

std::string mfWaeHandlerGerman::handleOptionsAndArgumentsFromArgcArgv () const
{
  return "Handle the options and arguments from argc/argv";
}

//_______________________________________________________________________________
// passes

std::string mfWaeHandlerGerman::createAnMXSRFromAMusicXMLFile () const
{
  return "Create an MXSR from a MusicXML file";
}
std::string mfWaeHandlerGerman::createAnMXSRFromAMusicXMLDescriptor () const
{
  return "Create an MXSR from a MusicXML descriptor";
}
std::string mfWaeHandlerGerman::createAnMXSRFromAMusicXMLBuffer () const
{
  return "Create an MXSR from a MusicXML buffer";
}

std::string mfWaeHandlerGerman::createAnMSRSqueletonFromTheMXSR () const
{
  return "Create an MSR skeleton from the MXSR";
}

std::string mfWaeHandlerGerman::populateTheMSRSqueletonFromMusicXMLData () const
{
  return "Populate the MSR skeleton from MusicXML data";
}

std::string mfWaeHandlerGerman::convertTheFirstMSRIntoASecondMSR () const
{
  return "Convert the first MSR into a second MSR";
}

std::string mfWaeHandlerGerman::convertTheSecondMSRIntoAnLPSR () const
{
  return "Convert the second MSR into an LPSR";
}

std::string mfWaeHandlerGerman::convertTheLPSRIntoLilyPondCode () const
{
  return "Convert the LPSR into LilyPond code";
}

//_______________________________________________________________________________
// display

std::string mfWaeHandlerGerman::displayTheFirstMSRSkeletonAsText () const
{
  return "Display the first MSR skeleton as text";
}
std::string mfWaeHandlerGerman::displayTheFirstMSRAsText () const
{
  return "Display the first MSR as text";
}
std::string mfWaeHandlerGerman::displayTheSecondMSRAsText () const
{
  return "Display the second MSR as text";
}
std::string mfWaeHandlerGerman::displayTheLPSRAsText () const
{
  return "Display the LPSR as text";
}

std::string mfWaeHandlerGerman::displayTheNamesInTheFirstMSR () const
{
  return "Display the names in the first MSR";
}
std::string mfWaeHandlerGerman::displayTheNamesInTheSecondMSR () const
{
  return "Display the names in the second MSR";
}

std::string mfWaeHandlerGerman::displayASummaryOfTheFirstMSR () const
{
  return "Display a summary of the first MSR";
}
std::string mfWaeHandlerGerman::displayASummaryOfTheSecondMSR () const
{
  return "Display a summary of the second MSR";
}

//_______________________________________________________________________________
// timing

std::string mfWaeHandlerGerman::timingInformation () const
{
  return "Timing information";
}
std::string mfWaeHandlerGerman::activity () const
{
  return "Activity";
}
std::string mfWaeHandlerGerman::description () const
{
  return "Description";
}
std::string mfWaeHandlerGerman::kind () const
{
  return "Kind";
}
std::string mfWaeHandlerGerman::CPUSeconds () const
{
  return "CPU (sec)";
}
std::string mfWaeHandlerGerman::mandatory () const
{
  return "mandatory";
}
std::string mfWaeHandlerGerman::optional () const
{
  return "optional";
}
std::string mfWaeHandlerGerman::totalSeconds () const
{
  return "Total (sec)";
}

//_______________________________________________________________________________
// options files

std::string mfWaeHandlerGerman::openingOptionsFileForReading (
  const std::string& optionsFileName) const
{
  std::stringstream s;

  s <<
    "Opening options file " <<
    "\"" << optionsFileName << "\"" <<
    " for reading";

  return s.str ();
}

std::string mfWaeHandlerGerman::openingIncludeFileForReading (
  const std::string& includeFileName) const
{
  std::stringstream s;

  s <<
    "Opening include file " <<
    "\"" << includeFileName << "\"" <<
    " for reading";

  return s.str ();
}

std::string mfWaeHandlerGerman::cannotOpenOptionsFileForReading (
  const std::string& optionsFileName) const
{
  std::stringstream s;

  s <<
    "Cannot open LilyPond file " <<
    "\"" << optionsFileName << "\"" <<
    " for reading - quitting";

  return s.str ();
}

std::string mfWaeHandlerGerman::closingOptionsFile (
  const std::string& optionsFileName) const
{
  std::stringstream s;

  s <<
    "Closing options file " <<
    "\"" << optionsFileName << "\"";

  return s.str ();
}

//_______________________________________________________________________________
// output files

std::string mfWaeHandlerGerman::openingLilypondFileForWriting (
  const std::string& inputFileName) const
{
  std::stringstream s;

  s <<
    "Opening LilyPond file " <<
    "\"" << inputFileName << "\"" <<
    " for writing";

  return s.str ();
}

std::string mfWaeHandlerGerman::openingBrailleMusicFileForWriting (
  const std::string& inputFileName) const
{
  std::stringstream s;

  s <<
    "Opening Braille music file " <<
    "\"" << inputFileName << "\"" <<
    " for writing";

  return s.str ();
}

std::string mfWaeHandlerGerman::openingMusicXMLFileForWriting (
  const std::string& inputFileName) const
{
  std::stringstream s;

  s <<
    "Opening MusicXML file " <<
    "\"" << inputFileName << "\"" <<
    " for writing";

  return s.str ();
}

std::string mfWaeHandlerGerman::openingGuidoFileForWriting (
  const std::string& inputFileName) const
{
  std::stringstream s;

  s <<
    "Opening Guido file " <<
    "\"" << inputFileName << "\"" <<
    " for writing";

  return s.str ();
}


std::string mfWaeHandlerGerman::cannotOpenLilypondFileForWriting (
  const std::string& outputFileName) const
{
  std::stringstream s;

  s <<
    "Cannot open LilyPond file " <<
    "\"" << outputFileName << "\"" <<
    " for writing - quitting";

  return s.str ();
}

std::string mfWaeHandlerGerman::cannotOpenBrailleMusicFileForWriting (
  const std::string& outputFileName) const
{
  std::stringstream s;

  s <<
    "Cannot open Braille music file " <<
    "\"" << outputFileName << "\"" <<
    " for writing - quitting";

  return s.str ();
}

std::string mfWaeHandlerGerman::cannotOpenMusicXMLFileForWriting (
  const std::string& outputFileName) const
{
  std::stringstream s;

  s <<
    "Cannot open MusicXML file " <<
    "\"" << outputFileName << "\"" <<
    " for writing - quitting";

  return s.str ();
}

std::string mfWaeHandlerGerman::cannotOpenGuidoFileForWriting (
  const std::string& outputFileName) const
{
  std::stringstream s;

  s <<
    "Cannot open Guido file " <<
    "\"" << outputFileName << "\"" <<
    " for writing - quitting";

  return s.str ();
}


std::string mfWaeHandlerGerman::closingLilypondFile (
  const std::string& outputFileName) const
{
  std::stringstream s;

  s <<
    "Closing LilyPond file " <<
    "\"" << outputFileName << "\"";

  return s.str ();
}

std::string mfWaeHandlerGerman::closingBrailleMusicFile (
  const std::string& outputFileName) const
{
  std::stringstream s;

  s <<
    "Closing Braille music file " <<
    "\"" << outputFileName << "\"";

  return s.str ();
}

std::string mfWaeHandlerGerman::closingMusicXMLFile (
  const std::string& outputFileName) const
{
  std::stringstream s;

  s <<
    "Closing MusicXML file " <<
    "\"" << outputFileName << "\"";

  return s.str ();
}

std::string mfWaeHandlerGerman::closingGuidoFile (
  const std::string& outputFileName) const
{
  std::stringstream s;

  s <<
    "Closing Guido file " <<
    "\"" << outputFileName << "\"";

  return s.str ();
}

//_______________________________________________________________________________
// include files

std::string mfWaeHandlerGerman::cannotOpenIncludeFileForReading (
  const std::string& includeFileName) const
{
  std::stringstream s;

  s <<
    "Cannot open include file " <<
    "\"" << includeFileName << "\"";

  return s.str ();
}


//_______________________________________________________________________________
// scripts

std::string mfWaeHandlerGerman::cannotOpenScriptForWriting (
  const std::string& scriptName) const
{
  std::stringstream s;

  s <<
    "Cannot open script " <<
    "\"" << scriptName << "\"";

  return s.str ();
}


//_______________________________________________________________________________
// MSDL

std::string mfWaeHandlerGerman::cannotOpenMSDLFileForReading (
  const std::string& inputFileName) const
{
  std::stringstream s;

  s <<
    "Cannot open MSDL file " <<
    "\"" << inputFileName << "\"" <<
    " for writing - quitting";

  return s.str ();
}


} // namespace
