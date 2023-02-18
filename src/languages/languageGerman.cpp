/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "languageGerman.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_languageGerman languageGerman::create ()
{
  languageGerman* o =
    new languageGerman ();
  assert (o != nullptr);
  return o;
}

languageGerman::languageGerman ()
{}

languageGerman::~languageGerman ()
{}

std::string languageGerman::asString () const
{
  std::stringstream ss;

  ss <<
    "MusicFormats warning and error handler for german";

  return ss.str ();
}

void languageGerman::print (std::ostream& os) const
{
  os <<
    "languageGerman" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_languageGerman& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}


// //_______________________________________________________________________________
// // pass names
//
// std::string languageGerman::passIDKindAsString (mfPassIDKind passIDKind) const
// {
//   std::string result;
//
//   switch (passIDKind) {
//     case mfPassIDKind::kMfPassID_UNKNOWN:
//       result = "*Pass kMfPassID_UNKNOWN*";
//       break;
//
//     case mfPassIDKind::kMfPassID_ALL:
//       result = "*Pass kMfPassID_ALL*";
//       break;
//
//     case mfPassIDKind::kMfPassID_OptionsAndArgumentsHandling:
//       result = "Opts & Args";
//       break;
//
//     case mfPassIDKind::kMfPassID_DisplayPass:
//       result = "Display";
//       break;
//
//     case mfPassIDKind::kMfPassID_1:
//      result = "Pass 1";
//       break;
//
//     case mfPassIDKind::kMfPassID_2:
//      result = "Pass 2";
//       break;
//     case mfPassIDKind::kMfPassID_2a:
//      result = "Pass 2a";
//       break;
//     case mfPassIDKind::kMfPassID_2b:
//      result = "Pass 2b";
//       break;
//
//     case mfPassIDKind::kMfPassID_3:
//      result = "Pass 3";
//       break;
//     case mfPassIDKind::kMfPassID_3a:
//      result = "Pass 3a";
//       break;
//     case mfPassIDKind::kMfPassID_3b:
//      result = "Pass 3b";
//       break;
//
//     case mfPassIDKind::kMfPassID_4:
//      result = "Pass 4";
//       break;
//     case mfPassIDKind::kMfPassID_4a:
//      result = "Pass 4a";
//       break;
//     case mfPassIDKind::kMfPassID_4b:
//      result = "Pass 4b";
//       break;
//
//     case mfPassIDKind::kMfPassID_5:
//      result = "Pass 5";
//       break;
//   } // switch
//
//   return result;
// }
//
// std::string languageGerman::passOptional () const
// {
//   return "Pass (optional)";
// }
//
// std::string languageGerman::fullVersion () const
// {
//   return "full version";
// }
// std::string languageGerman::summary () const
// {
//   return "summary";
// }
// std::string languageGerman::names () const
// {
//   return "names";
// }
// std::string languageGerman::slices () const
// {
//   return "slices";
// }
//
// //_______________________________________________________________________________
// // quitting after passes
// std::string languageGerman::quittingAfterPass (mfPassIDKind passIDKind) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Quitting after pass " <<
//     passIDKindAsString (passIDKind) <<
//     " as requested";
//
//   return ss.str ();
// }
//
// //_______________________________________________________________________________
// // OAH
//
// std::string languageGerman::handleOptionsAndArgumentsFromArgcArgv () const
// {
//   return "Handle the options and arguments from argc/argv";
// }
//
// //_______________________________________________________________________________
// // passes
//
// std::string languageGerman::createAnMXSRFromAMusicXMLStream () const
// {
//   return "Create an MXSR (MusicXML tree) from a MusicXML file";
// }
// std::string languageGerman::createAnMXSRFromAMusicXMLDescriptor () const
// {
//   return "Create an MXSR (MusicXML tree) from a MusicXML descriptor";
// }
// std::string languageGerman::createAnMXSRFromAMusicXMLBuffer () const
// {
//   return "Create an MXSR (MusicXML tree) from a MusicXML buffer";
// }
//
// std::string languageGerman::createAnMSRSqueletonFromTheMXSR () const
// {
//   return "Create an MSR skeleton from the MXSR";
// }
//
// std::string languageGerman::populateTheMSRSqueletonFromMusicXMLData () const
// {
//   return "Populate the MSR skeleton from MusicXML data";
// }
//
// std::string languageGerman::convertTheFirstMSRIntoASecondMSR () const
// {
//   return "Convert the first MSR into a second MSR";
// }
//
// std::string languageGerman::convertTheSecondMSRIntoAnLPSR () const
// {
//   return "Convert the second MSR into an LPSR";
// }
//
// std::string languageGerman::convertTheLPSRIntoLilyPondCode () const
// {
//   return "Convert the LPSR into LilyPond code";
// }
//
// //_______________________________________________________________________________
// // display
//
// std::string languageGerman::displayTheFirstMSRSkeletonAsText () const
// {
//   return "Display the first MSR skeleton as text";
// }
// std::string languageGerman::displayTheFirstMSRAsText () const
// {
//   return "Display the first MSR as text";
// }
// std::string languageGerman::displayTheSecondMSRAsText () const
// {
//   return "Display the second MSR as text";
// }
// std::string languageGerman::displayTheLPSRAsText () const
// {
//   return "Display the LPSR as text";
// }
//
// std::string languageGerman::displayTheNamesInTheFirstMSR () const
// {
//   return "Display the names in the first MSR";
// }
// std::string languageGerman::displayTheNamesInTheSecondMSR () const
// {
//   return "Display the names in the second MSR";
// }
//
// std::string languageGerman::displayASummaryOfTheFirstMSR () const
// {
//   return "Display a summary of the first MSR";
// }
// std::string languageGerman::displayASummaryOfTheSecondMSR () const
// {
//   return "Display a summary of the second MSR";
// }
//
// //_______________________________________________________________________________
// // timing
//
// std::string languageGerman::timingInformation () const
// {
//   return "Timing information";
// }
// std::string languageGerman::activity () const
// {
//   return "Activity";
// }
// std::string languageGerman::description () const
// {
//   return "Description";
// }
// std::string languageGerman::kind () const
// {
//   return "Kind";
// }
// std::string languageGerman::CPUSeconds () const
// {
//   return "CPU (sec)";
// }
// std::string languageGerman::mandatory () const
// {
//   return "mandatory";
// }
// std::string languageGerman::optional () const
// {
//   return "optional";
// }
// std::string languageGerman::totalSeconds () const
// {
//   return "Total (sec)";
// }
//
// //_______________________________________________________________________________
// // options files
//
// std::string languageGerman::openingOptionsFileForReading (
//   const std::string& optionsFileName) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Opening options file " <<
//     "\"" << optionsFileName << "\"" <<
//     " for reading";
//
//   return ss.str ();
// }
//
// std::string languageGerman::openingIncludeFileForReading (
//   const std::string& includeFileName) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Opening include file " <<
//     "\"" << includeFileName << "\"" <<
//     " for reading";
//
//   return ss.str ();
// }
//
// std::string languageGerman::cannotOpenOptionsFileForReading (
//   const std::string& optionsFileName) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Cannot open LilyPond file " <<
//     "\"" << optionsFileName << "\"" <<
//     " for reading - quitting";
//
//   return ss.str ();
// }
//
// std::string languageGerman::closingOptionsFile (
//   const std::string& optionsFileName) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Closing options file " <<
//     "\"" << optionsFileName << "\"";
//
//   return ss.str ();
// }
//
// //_______________________________________________________________________________
// // output files
//
// std::string languageGerman::openingLilypondFileForWriting (
//   const std::string& inputFileName) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Opening LilyPond file " <<
//     "\"" << inputFileName << "\"" <<
//     " for writing";
//
//   return ss.str ();
// }
//
// std::string languageGerman::openingBrailleMusicFileForWriting (
//   const std::string& inputFileName) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Opening Braille music file " <<
//     "\"" << inputFileName << "\"" <<
//     " for writing";
//
//   return ss.str ();
// }
//
// std::string languageGerman::openingMusicXMLFileForWriting (
//   const std::string& inputFileName) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Opening MusicXML file " <<
//     "\"" << inputFileName << "\"" <<
//     " for writing";
//
//   return ss.str ();
// }
//
// std::string languageGerman::openingGuidoFileForWriting (
//   const std::string& inputFileName) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Opening Guido file " <<
//     "\"" << inputFileName << "\"" <<
//     " for writing";
//
//   return ss.str ();
// }
//
//
// std::string languageGerman::cannotOpenLilypondFileForWriting (
//   const std::string& outputFileName) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Cannot open LilyPond file " <<
//     "\"" << outputFileName << "\"" <<
//     " for writing - quitting";
//
//   return ss.str ();
// }
//
// std::string languageGerman::cannotOpenBrailleMusicFileForWriting (
//   const std::string& outputFileName) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Cannot open Braille music file " <<
//     "\"" << outputFileName << "\"" <<
//     " for writing - quitting";
//
//   return ss.str ();
// }
//
// std::string languageGerman::cannotOpenMusicXMLFileForWriting (
//   const std::string& outputFileName) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Cannot open MusicXML file " <<
//     "\"" << outputFileName << "\"" <<
//     " for writing - quitting";
//
//   return ss.str ();
// }
//
// std::string languageGerman::cannotOpenGuidoFileForWriting (
//   const std::string& outputFileName) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Cannot open Guido file " <<
//     "\"" << outputFileName << "\"" <<
//     " for writing - quitting";
//
//   return ss.str ();
// }
//
//
// std::string languageGerman::closingLilypondFile (
//   const std::string& outputFileName) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Closing LilyPond file " <<
//     "\"" << outputFileName << "\"";
//
//   return ss.str ();
// }
//
// std::string languageGerman::closingBrailleMusicFile (
//   const std::string& outputFileName) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Closing Braille music file " <<
//     "\"" << outputFileName << "\"";
//
//   return ss.str ();
// }
//
// std::string languageGerman::closingMusicXMLFile (
//   const std::string& outputFileName) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Closing MusicXML file " <<
//     "\"" << outputFileName << "\"";
//
//   return ss.str ();
// }
//
// std::string languageGerman::closingGuidoFile (
//   const std::string& outputFileName) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Closing Guido file " <<
//     "\"" << outputFileName << "\"";
//
//   return ss.str ();
// }
//
// //_______________________________________________________________________________
// // include files
//
// std::string languageGerman::cannotOpenIncludeFileForReading (
//   const std::string& includeFileName) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Cannot open include file " <<
//     "\"" << includeFileName << "\"";
//
//   return ss.str ();
// }
//
//
// //_______________________________________________________________________________
// // scripts
//
// std::string languageGerman::cannotOpenScriptForWriting (
//   const std::string& scriptName) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Cannot open script " <<
//     "\"" << scriptName << "\"";
//
//   return ss.str ();
// }
//
//
// //_______________________________________________________________________________
// // MSDL
//
// std::string languageGerman::cannotOpenMSDLFileForReading (
//   const std::string& inputFileName) const
// {
//   std::stringstream ss;
//
//   ss <<
//     "Cannot open MSDL file " <<
//     "\"" << inputFileName << "\"" <<
//     " for writing - quitting";
//
//   return ss.str ();
// }


} // namespace
