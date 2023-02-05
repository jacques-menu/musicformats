/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>

#include "mfConstants.h"

#include "mfStaticSettings.h"

#include "mfBool.h"

#include "mfLanguages.h"

#include "mfWaeHandlersDutch.h"
#include "mfWaeHandlersEnglish.h"
#include "mfWaeHandlersFrench.h"
#include "mfWaeHandlersGerman.h"
#include "mfWaeHandlersItalian.h"
#include "mfWaeHandlersSpanish.h"

#include "oahEarlyOptions.h"

#include "oahWae.h"


namespace MusicFormats
{

// the MusicFormats languages
//______________________________________________________________________________

std::map<std::string, mfLanguageKind>
  gGlobalMusicFormatsLanguageKindsMap;

mfLanguageKind mfLanguageKindFromString (const std::string& theString)
{
  mfLanguageKind result =
    mfLanguageKind::kMusicFormatsLanguageEnglish; // MusicFormats default

  std::map<std::string, mfLanguageKind>::const_iterator
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
    case mfLanguageKind::kMusicFormatsLanguage_UNKNOWN:
      result = "*kMusicFormatsLanguage_UNKNOWN*";
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
      std::pair<std::string, mfLanguageKind>
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
// void testMfIndentedStringstream ()
// {
//   mfIndentedStringStream iss;
//
//   // populate the mfIndentedStringStream
//   iss <<
//     "iss 1" <<
//     std::endl;
//
//   ++gIndenter;
//   iss <<
//     "iss 1.1" <<
//     std::endl <<
//     "iss 1.2" <<
//     std::endl;
//   --gIndenter;
//
//   iss <<
//     "iss 2" <<
//     std::endl;
//
//   ++gIndenter;
//   iss <<
//     "iss 2.1" <<
//     std::endl;
//
//   iss <<
//     "iss 2.2" <<
//     std::endl;
//   {
//     ++gIndenter;
//     iss <<
//       "iss 2.2.1" <<
//       std::endl <<
//       "iss 2.2.2" <<
//       std::endl;
//     --gIndenter;
//   }
//
//   iss <<
//     "iss 2.3" <<
//     std::endl;
//
//   --gIndenter;
//
//   iss <<
//     "iss 3" <<
//     std::endl;
//
//
//   // display the mfIndentedStringStream contents
// //   gLogStream <<
// //     "iss.str ():" <<
// //     std::endl;
// //
// //   ++gIndenter;
//   gLogStream <<
//     iss.str () <<
//     std::endl;
// //   --gIndenter;
// }

//______________________________________________________________________________
void initializeMusicFormatsLanguages ()
{
//   testMfIndentedStringstream (); // JMI v0.9.66

  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
#ifdef MF_TRACING_IS_ENABLED
    if (
      gGlobalOahEarlyOptions.getEarlyTracingOah ()
        &&
      ! gGlobalOahEarlyOptions.getEarlyQuietOption ()
    ) {
	  	std::stringstream ss;

      ss <<
        "Initializing MusicFormats languages" <<
        std::endl;

      gWaeHandler->waeTrace (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif

    // keywords languages handling
    // ------------------------------------------------------

    initializeMusicFormatsLanguageKindsMap ();

    pPrivateThisMethodHasBeenRun = true;
  }
}


}
