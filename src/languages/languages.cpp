/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>

// #include "mfConstants.h"

#include "mfPreprocessorSettings.h"

#include "mfBool.h"
#include "mfConstants.h"

#include "languages.h"

#include "oahEarlyOptions.h"

#include "oahWae.h"

#include "languageDutch.h"
#include "languageEnglish.h"
#include "languageFrench.h"
#include "languageGerman.h"
#include "languageItalian.h"
#include "languageSpanish.h"

#include "waeHandlers.h"


namespace MusicFormats
{

// the MusicFormats languages
//______________________________________________________________________________

std::map <std::string, mfLanguageKind>
  gGlobalMusicFormatsLanguageKindsMap;

mfLanguageKind mfLanguageKindFromString (const std::string& theString)
{
  mfLanguageKind result =
    mfLanguageKind::kMusicFormatsLanguageEnglish; // MusicFormats default

  std::map <std::string, mfLanguageKind>::const_iterator
    it =
      gGlobalMusicFormatsLanguageKindsMap.find (
        theString);

  if (it == gGlobalMusicFormatsLanguageKindsMap.end ()) {
    // no, keywords language kind is unknown in the map
    std::stringstream ss;

    ss <<
      "MusicFormats language kind '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMusicFormatsLanguageKindsMap.size () - 1 <<
      " known MusicFormats language kinds are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableMusicFormatsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
  }

  result = (*it).second;

  return result;
}

void initializeMusicFormatsLanguageKindsMap ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
    gGlobalMusicFormatsLanguageKindsMap ["english"]   =
      mfLanguageKind::kMusicFormatsLanguageEnglish;

    gGlobalMusicFormatsLanguageKindsMap ["francais"]  =
      mfLanguageKind::kMusicFormatsLanguageFrench;

    gGlobalMusicFormatsLanguageKindsMap ["italiano"]  =
      mfLanguageKind::kMusicFormatsLanguageItalian;

    gGlobalMusicFormatsLanguageKindsMap ["deutsch"]   =
      mfLanguageKind::kMusicFormatsLanguageGerman;

    gGlobalMusicFormatsLanguageKindsMap ["espanol"]   =
      mfLanguageKind::kMusicFormatsLanguageSpanish;

    gGlobalMusicFormatsLanguageKindsMap ["dutch"] =
      mfLanguageKind::kMusicFormatsLanguageDutch;

    pPrivateThisMethodHasBeenRun = true;
  }
}

std::string mfLanguageKindAsString (
  mfLanguageKind languageKind)
{
  std::string result;

  switch (languageKind) {
    case mfLanguageKind::kMusicFormatsLanguage_UNKNOWN_:
      result = "*kMusicFormatsLanguage_UNKNOWN_*";
      break;
    case mfLanguageKind::kMusicFormatsLanguageEnglish: // default value
      result = "english";
      break;
    case mfLanguageKind::kMusicFormatsLanguageFrench:
      result = "francais";
      break;
    case mfLanguageKind::kMusicFormatsLanguageItalian:
      result = "italiano";
      break;
    case mfLanguageKind::kMusicFormatsLanguageGerman:
      result = "deutsch";
      break;
    case mfLanguageKind::kMusicFormatsLanguageSpanish:
      result = "espanol";
      break;
    case mfLanguageKind::kMusicFormatsLanguageDutch:
      result = "dutch";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mfLanguageKind& elt)
{
  os << mfLanguageKindAsString (elt);
  return os;
}

std::string availableMusicFormatsLanguageKinds (size_t namesListMaxLength)
{
  std::stringstream ss;

  size_t
    mfLanguageKindsMapSize =
      gGlobalMusicFormatsLanguageKindsMap.size ();

  if (mfLanguageKindsMapSize) {
    size_t nextToLast =
      mfLanguageKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::pair <std::string, mfLanguageKind>
        thePair :
          gGlobalMusicFormatsLanguageKindsMap
    ) {
      std::string theString = thePair.first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        ss << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
        break;
      }

      if (count == 1) {
        ss << gIndenter.getSpacer ();
      }
      ss << theString;

      if (count == nextToLast) {
        ss << " and ";
      }
      else if (count != mfLanguageKindsMapSize) {
        ss << ", ";
      }
    } // for
  }

  return ss.str ();
}

//______________________________________________________________________________
void initializeMusicFormatsLanguages ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
#ifdef MF_TRACE_IS_ENABLED
    if (
      gEarlyOptions.getTraceEarlyOptions ()
        &&
      ! gEarlyOptions.getEarlyQuietOption ()
    ) {
      std::stringstream ss;

      ss <<
        "Initializing MusicFormats languages";

      gWaeHandler->waeTraceWithoutInputLocation (
        __FILE__, mfInputLineNumber (__LINE__),
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

    // keywords languages handling
    // ------------------------------------------------------

    initializeMusicFormatsLanguageKindsMap ();

    pPrivateThisMethodHasBeenRun = true;
  }
}

//_______________________________________________________________________________
/* this class is purely virtual
S_language language::create ()
{
  language* obj =
    new language ();
  assert (obj != nullptr);
  return obj;
}
*/

language::language ()
{}

language::~language ()
{}

std::string language::asString () const
{
  std::stringstream ss;

  ss <<
    "MSDL WAE lexical handler";

  return ss.str ();
}

void language::print (std::ostream& os) const
{
  os <<
    "language" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_language& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

//_______________________________________________________________________________
// pass names

std::string language::passIDKindAsString (mfPassIDKind passIDKind) const
{
  std::string result;

  switch (passIDKind) {
    case mfPassIDKind::kMfPassID_UNKNOWN_:
      result = "*Pass kMfPassID_UNKNOWN_*";
      break;

    case mfPassIDKind::kMfPassID_ALL:
      result = "*Pass kMfPassID_ALL*";
      break;

    case mfPassIDKind::kMfPassID_OptionsAndArgumentsHandling:
      result = "Opts & Args";
      break;

    case mfPassIDKind::kMfPassID_DisplayPass:
      result = "Display";
      break;

    case mfPassIDKind::kMfPassID_1:
      result = "Pass 1";
      break;

    case mfPassIDKind::kMfPassID_2:
      result = "Pass 2";
      break;

    case mfPassIDKind::kMfPassID_3:
      result = "Pass 3";
      break;

    case mfPassIDKind::kMfPassID_4:
      result = "Pass 4";
      break;

    case mfPassIDKind::kMfPassID_5:
      result = "Pass 5";
      break;

    case mfPassIDKind::kMfPassID_6:
      result = "Pass 6";
      break;

    case mfPassIDKind::kMfPassID_7:
      result = "Pass 7";
      break;
  } // switch

  return result;
}

std::string language::passOptional () const
{
  return "Pass (optional)";
}

std::string language::fullVersion () const
{
  return "full version";
}
std::string language::summary () const
{
  return "summary";
}
std::string language::names () const
{
  return "names";
}
std::string language::slices () const
{
  return "slices";
}

//_______________________________________________________________________________
// quitting after passes
std::string language::quittingAfterPass (mfPassIDKind passIDKind) const
{
  std::stringstream ss;

  ss <<
    '\n' <<
    "*** Quitting after pass " <<
    passIDKindAsString (passIDKind) <<
    " as requested ***";

  return ss.str ();
}

//_______________________________________________________________________________
// OAH

std::string language::handleOptionsAndArgumentsFromArgcArgv () const
{
  return "Handle the options and arguments from argc/argv";
}

//_______________________________________________________________________________
// passes

std::string language::convertAMusicXMLStreamIntoAnMXSR () const
{
  return "Convert a MusicXML text stream into an MXSR (MusicXML elements tree)";
}
std::string language::convertAMusicXMLDescriptorIntoAnMXSR () const
{
  return "Convert a MusicXML descriptor into an MXSR (MusicXML elements tree)";
}
std::string language::convertAMusicXMLBufferIntoAnMXSR () const
{
  return "Convert a MusicXML buffer into an MXSR (MusicXML elements tree)";
}

std::string language::convertTheMXSRIntoAnMSRSkeleton () const
{
  return "Convert the MXSR into an MSR skeleton";
}

std::string language::populateTheMSRSkeletonFromMusicXMLData () const
{
  return "Populate the MSR skeleton from the MXSR";
}

std::string language::convertTheFirstMSRIntoASecondMSR () const
{
  return "Convert the first MSR into a second MSR";
}

std::string language::convertTheSecondMSRIntoAnLPSR () const
{
  return "Convert the second MSR into an LPSR";
}

std::string language::convertTheFirstMSRIntoAnLPSR () const
{
  return "Convert the first MSR into an LPSR";
}

std::string language::convertTheLPSRIntoLilyPondCode () const
{
  return "Convert the LPSR into LilyPond code";
}

//_______________________________________________________________________________
// display

std::string language::displayTheFirstMSRSkeletonAsText () const
{
  return "Display the first MSR skeleton as text";
}
std::string language::displayTheFirstMSRAsText () const
{
  return "Display the first MSR as text";
}
std::string language::displayTheSecondMSRAsText () const
{
  return "Display the second MSR as text";
}
std::string language::displayTheMSREmbededInTheLPSRAsText () const
{
  return "Display the third MSR as text";
}
std::string language::displayTheLPSRAsText () const
{
  return "Display the LPSR as text";
}

std::string language::displayTheNamesInTheFirstMSR () const
{
  return "Display the names in the first MSR";
}
std::string language::displayTheNamesInTheSecondMSR () const
{
  return "Display the names in the second MSR";
}
std::string language::displayTheNamesInTheLPSR () const
{
  return "Display the names in the LPSR";
}

std::string language::displayASummaryOfTheFirstMSR () const
{
  return "Display a summary of the first MSR";
}
std::string language::displayASummaryOfTheSecondMSR () const
{
  return "Display a summary of the second MSR";
}
std::string language::displayASummaryOfTheLPSR () const
{
  return "Display a summary of the LPSR";
}

std::string language::displayAFlatViewOfTheFirstMSR () const
{
  return "Display a voices flat view of the first MSR";
}
std::string language::displayAFlatViewOfTheSecondMSR () const
{
  return "Display  a voices flat view of the second MSR";
}
std::string language::displayAFlatViewOfTheLPSR () const
{
  return "Display  a voices flat view of the LPSR";
}

std::string language::displayTheSlicesOfTheFirstMSR () const
{
  return "Display the measures slices of the first MSR";
}
std::string language::displayTheSlicesOfTheSecondMSR () const
{
  return "Display the measures slices of the second MSR";
}
std::string language::displayTheSlicesOfTheLPSR () const
{
  return "Display the measures slices of the LPSR";
}

std::string language::displayTheBSRAsText () const
{
  return "Display the BSR as text";
}
std::string language::displayTheFirstBSRAsText () const
{
  return "Display the first BSR as text";
}
std::string language::displayTheFinalizedBSRAsText () const
{
  return "Display the finalized BSR as text";
}

//_______________________________________________________________________________
// timing

std::string language::timingInformation () const
{
  return "Timing information";
}
std::string language::activity () const
{
  return "Activity";
}
std::string language::description () const
{
  return "Description";
}
std::string language::kind () const
{
  return "Kind";
}
std::string language::CPUSeconds () const
{
  return "CPU (sec)";
}
std::string language::mandatory () const
{
  return "mandatory";
}
std::string language::optional () const
{
  return "optional";
}
std::string language::totalSeconds () const
{
  return "Total (sec)";
}

//_______________________________________________________________________________
// options files

std::string language::openingOptionsFileForReading (
  const std::string& optionsFileName) const
{
  std::stringstream ss;

  ss <<
    "Opening options file " <<
    "\"" << optionsFileName << "\"" <<
    " for reading";

  return ss.str ();
}

std::string language::openingIncludeFileForReading (
  const std::string& includeFileName) const
{
  std::stringstream ss;

  ss <<
    "Opening include file " <<
    "\"" << includeFileName << "\"" <<
    " for reading";

  return ss.str ();
}

std::string language::cannotOpenOptionsFileForReading (
  const std::string& optionsFileName) const
{
  std::stringstream ss;

  ss <<
    "Cannot open LilyPond file " <<
    "\"" << optionsFileName << "\"" <<
    " for reading - quitting";

  return ss.str ();
}

std::string language::closingOptionsFile (
  const std::string& optionsFileName) const
{
  std::stringstream ss;

  ss <<
    "Closing options file " <<
    "\"" << optionsFileName << "\"";

  return ss.str ();
}

//_______________________________________________________________________________
// output files

std::string language::openingLilypondFileForWriting (
  const std::string& inputFileName) const
{
  std::stringstream ss;

  ss <<
    "Opening LilyPond file " <<
    "\"" << inputFileName << "\"" <<
    " for writing";

  return ss.str ();
}

std::string language::openingBrailleMusicFileForWriting (
  const std::string& inputFileName) const
{
  std::stringstream ss;

  ss <<
    "Opening Braille music file " <<
    "\"" << inputFileName << "\"" <<
    " for writing";

  return ss.str ();
}

std::string language::openingMusicXMLFileForWriting (
  const std::string& inputFileName) const
{
  std::stringstream ss;

  ss <<
    "Opening MusicXML file " <<
    "\"" << inputFileName << "\"" <<
    " for writing";

  return ss.str ();
}

std::string language::openingGuidoFileForWriting (
  const std::string& inputFileName) const
{
  std::stringstream ss;

  ss <<
    "Opening Guido file " <<
    "\"" << inputFileName << "\"" <<
    " for writing";

  return ss.str ();
}


std::string language::cannotOpenLilypondFileForWriting (
  const std::string& outputFileName) const
{
  std::stringstream ss;

  ss <<
    "Cannot open LilyPond file " <<
    "\"" << outputFileName << "\"" <<
    " for writing - quitting";

  return ss.str ();
}

std::string language::cannotOpenBrailleMusicFileForWriting (
  const std::string& outputFileName) const
{
  std::stringstream ss;

  ss <<
    "Cannot open Braille music file " <<
    "\"" << outputFileName << "\"" <<
    " for writing - quitting";

  return ss.str ();
}

std::string language::cannotOpenMusicXMLFileForWriting (
  const std::string& outputFileName) const
{
  std::stringstream ss;

  ss <<
    "Cannot open MusicXML file " <<
    "\"" << outputFileName << "\"" <<
    " for writing - quitting";

  return ss.str ();
}

std::string language::cannotOpenGuidoFileForWriting (
  const std::string& outputFileName) const
{
  std::stringstream ss;

  ss <<
    "Cannot open Guido file " <<
    "\"" << outputFileName << "\"" <<
    " for writing - quitting";

  return ss.str ();
}


std::string language::closingLilypondFile (
  const std::string& outputFileName) const
{
  std::stringstream ss;

  ss <<
    "Closing LilyPond file " <<
    "\"" << outputFileName << "\"";

  return ss.str ();
}

std::string language::closingBrailleMusicFile (
  const std::string& outputFileName) const
{
  std::stringstream ss;

  ss <<
    "Closing Braille music file " <<
    "\"" << outputFileName << "\"";

  return ss.str ();
}

std::string language::closingMusicXMLFile (
  const std::string& outputFileName) const
{
  std::stringstream ss;

  ss <<
    "Closing MusicXML file " <<
    "\"" << outputFileName << "\"";

  return ss.str ();
}

std::string language::closingGuidoFile (
  const std::string& outputFileName) const
{
  std::stringstream ss;

  ss <<
    "Closing Guido file " <<
    "\"" << outputFileName << "\"";

  return ss.str ();
}

//_______________________________________________________________________________
// include files

std::string language::cannotOpenIncludeFileForReading (
  const std::string& includeFileName) const
{
  std::stringstream ss;

  ss <<
    "Cannot open include file " <<
    "\"" << includeFileName << "\"";

  return ss.str ();
}


//_______________________________________________________________________________
// scripts

std::string language::cannotOpenScriptForWriting (
  const std::string& scriptName) const
{
  std::stringstream ss;

  ss <<
    "Cannot open script " <<
    "\"" << scriptName << "\"";

  return ss.str ();
}


//_______________________________________________________________________________
// MSDL

std::string language::cannotOpenMSDLFileForReading (
  const std::string& inputFileName) const
{
  std::stringstream ss;

  ss <<
    "Cannot open MSDL file " <<
    "\"" << inputFileName << "\"" <<
    " for writing - quitting";

  return ss.str ();
}

//________________________________________________________________________
// hidden global language variable
S_language pGlobalLanguage =
  languageEnglish::create (); // default value

EXP void setGlobalLanguage (mfLanguageKind languageKind)
{
  switch (languageKind) {
    case mfLanguageKind::kMusicFormatsLanguage_UNKNOWN_:
      {
        std::stringstream ss;

        ss <<
          "Attempt at setting fEarlyLanguageKind to kMusicFormatsLanguage_UNKNOWN_" <<
          std::endl;

        oahError (ss.str ());
      }
      break;
    case mfLanguageKind::kMusicFormatsLanguageEnglish:
      pGlobalLanguage = languageEnglish::create ();
      break;
    case mfLanguageKind::kMusicFormatsLanguageFrench:
      pGlobalLanguage = languageFrench::create ();
      break;
    case mfLanguageKind::kMusicFormatsLanguageItalian:
      pGlobalLanguage = languageItalian::create ();
      break;
    case mfLanguageKind::kMusicFormatsLanguageGerman:
      pGlobalLanguage = languageGerman::create ();
      break;
    case mfLanguageKind::kMusicFormatsLanguageSpanish:
      pGlobalLanguage = languageSpanish::create ();
      break;
    case mfLanguageKind::kMusicFormatsLanguageDutch:
      pGlobalLanguage = languageDutch::create ();
      break;
  } // switch
}

EXP S_language getGlobalLanguage ()
{
  return pGlobalLanguage;
}

//________________________________________________________________________
void initializeLanguages ()
{
  pGlobalLanguage =
    languageEnglish::create (); // default value
}


} // namespace
