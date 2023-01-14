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

#include "mfEnableTracingSetting.h"

#include "mfBool.h"

#include "oahEarlyOptions.h"

#include "mfLanguages.h"

#include "mfWaeHandlersDutch.h"
#include "mfWaeHandlersEnglish.h"
#include "mfWaeHandlersFrench.h"
#include "mfWaeHandlersGerman.h"
#include "mfWaeHandlersItalian.h"
#include "mfWaeHandlersSpanish.h"

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
    std::stringstream s;

    s <<
      "MusicFormats language kind '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMusicFormatsLanguageKindsMap.size () - 1 <<
      " known MusicFormats language kinds are:" <<
      std::endl;

    ++gIndenter;

    s <<
      existingMusicFormatsLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
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

std::string existingMusicFormatsLanguageKinds (size_t namesListMaxLength)
{
  std::stringstream s;

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
        s << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
        break;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != mfLanguageKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

//______________________________________________________________________________
void initializeMusicFormatsLanguages ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
#ifdef MF_TRACING_IS_ENABLED
    if (
      gGlobalOahEarlyOptions.getEarlyTracingOah ()
        &&
      ! gGlobalOahEarlyOptions.getEarlyQuietOption ()
      ) {
      gLogStream <<
        "Initializing MusicFormats languages" <<
        std::endl;
  }
#endif

    // keywords languages handling
    // ------------------------------------------------------

    initializeMusicFormatsLanguageKindsMap ();

    pPrivateThisMethodHasBeenRun = true;
  }
}


}
