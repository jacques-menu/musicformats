/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>

#include <climits>      // INT_MIN, INT_MAX

#include "mfConstants.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msdlEnumTypes.h"

#include "oahOah.h"
#include "waeOah.h"

#include "msdlInputOah.h"

#include "oahEarlyOptions.h"


namespace MusicFormats
{

// user languages
//______________________________________________________________________________

std::map<std::string, msdlUserLanguageKind>
  gGlobalMsdlUserLanguageKindsMap;

void initializeMsdlUserLanguageKindsMap ()
{
  gGlobalMsdlUserLanguageKindsMap ["english"]  = msdlUserLanguageKind::kUserLanguageEnglish; // default
  gGlobalMsdlUserLanguageKindsMap ["francais"] = msdlUserLanguageKind::kUserLanguageFrench;
  gGlobalMsdlUserLanguageKindsMap ["italiano"] = msdlUserLanguageKind::kUserLanguageItalian;
  gGlobalMsdlUserLanguageKindsMap ["deutsch"]  = msdlUserLanguageKind::kUserLanguageGerman;
  gGlobalMsdlUserLanguageKindsMap ["espanol"]  = msdlUserLanguageKind::kUserLanguageSpanish;
}

std::string msdlUserLanguageKindAsString (
  msdlUserLanguageKind languageKind)
{
  std::string result;

  switch (languageKind) {
    case msdlUserLanguageKind::kUserLanguageEnglish: // default value
      result = "userLanguageEnglish";
      break;
    case msdlUserLanguageKind::kUserLanguageFrench:
      result = "userLanguageFrench";
      break;
    case msdlUserLanguageKind::kUserLanguageItalian:
      result = "userLanguageItalian";
      break;
    case msdlUserLanguageKind::kUserLanguageGerman:
      result = "userLanguageGerman";
      break;
    case msdlUserLanguageKind::kUserLanguageSpanish:
      result = "userLanguageSpanish";
      break;
    case msdlUserLanguageKind::kUserLanguageDutch:
      result = "userLanguageDutch";
      break;
  } // switch

  return result;
}

msdlUserLanguageKind msdlUserLanguageKindFromString (const std::string& theString)
{
  msdlUserLanguageKind result = msdlUserLanguageKind::kUserLanguageEnglish; // MSDL default

  std::map<std::string, msdlUserLanguageKind>::const_iterator
    it =
      gGlobalMsdlUserLanguageKindsMap.find (
        theString);

  if (it == gGlobalMsdlUserLanguageKindsMap.end ()) {
    // no, User language kind is unknown in the map
    std::stringstream s;

    s <<
      "MSDL language kind '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMsdlUserLanguageKindsMap.size () - 1 <<
      " known MSDL language kinds are:" <<
      std::endl;

    ++gIndenter;

    s <<
      existingMsdlUserLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

// JMI    oahError (s.str ());
  }

  result = (*it).second;

  return result;
}

std::string existingMsdlUserLanguageKinds (size_t namesListMaxLength)
{
  std::stringstream s;

  size_t
    msdlUserLanguageKindsMapSize =
      gGlobalMsdlUserLanguageKindsMap.size ();

  if (msdlUserLanguageKindsMapSize) {
    size_t nextToLast =
      msdlUserLanguageKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::map<std::string, msdlUserLanguageKind>::const_iterator i =
        gGlobalMsdlUserLanguageKindsMap.begin ();
      i != gGlobalMsdlUserLanguageKindsMap.end ();
      ++i
    ) {
      std::string theString = (*i).first;

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
      else if (count != msdlUserLanguageKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// comments types
//______________________________________________________________________________

std::map<std::string, msdlCommentsTypeKind>
  gGlobalMsdlCommentsTypeKindsMap;

msdlCommentsTypeKind msdlCommentsTypeKindFromString (const std::string& theString)
{
  msdlCommentsTypeKind result = msdlCommentsTypeKind::kCommentsTypePercent; // MSDL default

  std::map<std::string, msdlCommentsTypeKind>::const_iterator
    it =
      gGlobalMsdlCommentsTypeKindsMap.find (
        theString);

  if (it == gGlobalMsdlCommentsTypeKindsMap.end ()) {
    // no, keywords language kind is unknown in the map
    std::stringstream s;

    s <<
      "MSDL language kind '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMsdlCommentsTypeKindsMap.size () - 1 <<
      " known MSDL language kinds are:" <<
      std::endl;

    ++gIndenter;

    s <<
      existingMsdlCommentsTypeKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

// JMI    oahError (s.str ());
  }

  result = (*it).second;

  return result;
}

void initializeMsdlCommentsTypeKindsMap ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
    gGlobalMsdlCommentsTypeKindsMap ["percent"] = msdlCommentsTypeKind::kCommentsTypePercent;
    gGlobalMsdlCommentsTypeKindsMap ["star"]    = msdlCommentsTypeKind::kCommentsTypeStar;

    pPrivateThisMethodHasBeenRun = true;
  }
}

std::string msdlCommentsTypeKindAsString (
  msdlCommentsTypeKind languageKind)
{
  std::string result;

  switch (languageKind) {
    case msdlCommentsTypeKind::kCommentsTypePercent: // MSDL default
      result = "commentsTypePercent";
      break;
    case msdlCommentsTypeKind::kCommentsTypeStar:
      result = "commentsTypePercent";
      break;
  } // switch

  return result;
}

std::string existingMsdlCommentsTypeKinds (size_t namesListMaxLength)
{
  std::stringstream s;

  size_t
    msdlCommentsTypeKindsMapSize =
      gGlobalMsdlCommentsTypeKindsMap.size ();

  if (msdlCommentsTypeKindsMapSize) {
    size_t nextToLast =
      msdlCommentsTypeKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::map<std::string, msdlCommentsTypeKind>::const_iterator i =
        gGlobalMsdlCommentsTypeKindsMap.begin ();
      i != gGlobalMsdlCommentsTypeKindsMap.end ();
      ++i
    ) {
      std::string theString = (*i).first;

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
      else if (count != msdlCommentsTypeKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

//______________________________________________________________________________
void initializeMSDLBasicTypes ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalOahEarlyOptions.getEarlyTracingOah () && ! gGlobalOahEarlyOptions.getEarlyQuietOption ()) {
      gLogStream <<
        "Initializing MSDL basic types handling" <<
        std::endl;
    }
#endif

    // MSDL user languages handling
    // ------------------------------------------------------

    initializeMsdlUserLanguageKindsMap ();

    // MSDL keywords languages handling
    // ------------------------------------------------------

    initializeMsdlKeywordsLanguageKindsMap ();

    // MSDL comments types handling
    // ------------------------------------------------------

    initializeMsdlCommentsTypeKindsMap ();

/* JMI
    // MSDL pitches languages handling
    // ------------------------------------------------------

    initializeMsdlPitchesLanguageKindsMap ();
*/

    pPrivateThisMethodHasBeenRun = true;
  }
}


}
