/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>

#include <climits>      // INT_MIN, INT_MAX

#include "mfConstants.h"

#include "mfStaticSettings.h"

#include "msdlEnumTypes.h"

#include "oahOah.h"
#include "oahEarlyOptions.h"

#include "waeOah.h"

#include "msdlInputOah.h"

#include "waeHandlers.h"


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
    std::stringstream ss;

    ss <<
      "MSDL language kind '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMsdlUserLanguageKindsMap.size () - 1 <<
      " known MSDL language kinds are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableMsdlUserLanguageKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

// JMI    oahError (ss.str ());
  }

  result = (*it).second;

  return result;
}

std::string availableMsdlUserLanguageKinds (size_t namesListMaxLength)
{
  std::stringstream ss;

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
      else if (count != msdlUserLanguageKindsMapSize) {
        ss << ", ";
      }
    } // for
  }

  return ss.str ();
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
    std::stringstream ss;

    ss <<
      "MSDL language kind '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMsdlCommentsTypeKindsMap.size () - 1 <<
      " known MSDL language kinds are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableMsdlCommentsTypeKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

// JMI    oahError (ss.str ());
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

std::string availableMsdlCommentsTypeKinds (size_t namesListMaxLength)
{
  std::stringstream ss;

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
      else if (count != msdlCommentsTypeKindsMapSize) {
        ss << ", ";
      }
    } // for
  }

  return ss.str ();
}

//______________________________________________________________________________
void initializeMSDLBasicTypes ()
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
        "Initializing MSDL basic types handling";

      gWaeHandler->waeTraceWithLocationDetails (
        __FILE__, __LINE__,
        ss.str ());
    }
#endif // MF_TRACE_IS_ENABLED

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
