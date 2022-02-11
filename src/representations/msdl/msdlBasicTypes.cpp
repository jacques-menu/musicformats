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

#include "msrBasicTypes.h"
#include "msdlBasicTypes.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "oahOah.h"
#include "waeOah.h"

#include "msdlInputOah.h"

#include "oahEarlyOptions.h"


using namespace std;

namespace MusicFormats
{

// user languages
//______________________________________________________________________________

map<string, msdlUserLanguageKind>
  gGlobalMsdlUserLanguageKindsMap;

void initializeMsdlUserLanguageKindsMap ()
{
  gGlobalMsdlUserLanguageKindsMap ["english"]  = msdlUserLanguageKind::kUserLanguageEnglish; // default
  gGlobalMsdlUserLanguageKindsMap ["francais"] = msdlUserLanguageKind::kUserLanguageFrench;
  gGlobalMsdlUserLanguageKindsMap ["italiano"] = msdlUserLanguageKind::kUserLanguageItalian;
  gGlobalMsdlUserLanguageKindsMap ["deutsch"]  = msdlUserLanguageKind::kUserLanguageGerman;
  gGlobalMsdlUserLanguageKindsMap ["espanol"]  = msdlUserLanguageKind::kUserLanguageSpanish;
}

string msdlUserLanguageKindAsString (
  msdlUserLanguageKind languageKind)
{
  string result;

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

msdlUserLanguageKind msdlUserLanguageKindFromString (const string& theString)
{
  msdlUserLanguageKind result = msdlUserLanguageKind::kUserLanguageEnglish; // MSDL default

  map<string, msdlUserLanguageKind>::const_iterator
    it =
      gGlobalMsdlUserLanguageKindsMap.find (
        theString);

  if (it == gGlobalMsdlUserLanguageKindsMap.end ()) {
    // no, User language kind is unknown in the map
    stringstream s;

    s <<
      "MSDL language kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalMsdlUserLanguageKindsMap.size () - 1 <<
      " known MSDL language kinds are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsdlUserLanguageKinds (K_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

// JMI    oahError (s.str ());
  }

  result = (*it).second;

  return result;
}

string existingMsdlUserLanguageKinds (unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int
    msdlUserLanguageKindsMapSize =
      gGlobalMsdlUserLanguageKindsMap.size ();

  if (msdlUserLanguageKindsMapSize) {
    unsigned int nextToLast =
      msdlUserLanguageKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, msdlUserLanguageKind>::const_iterator i =
        gGlobalMsdlUserLanguageKindsMap.begin ();
      i != gGlobalMsdlUserLanguageKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
        break;
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

map<string, msdlCommentsTypeKind>
  gGlobalMsdlCommentsTypeKindsMap;

msdlCommentsTypeKind msdlCommentsTypeKindFromString (const string& theString)
{
  msdlCommentsTypeKind result = msdlCommentsTypeKind::kCommentsTypePercent; // MSDL default

  map<string, msdlCommentsTypeKind>::const_iterator
    it =
      gGlobalMsdlCommentsTypeKindsMap.find (
        theString);

  if (it == gGlobalMsdlCommentsTypeKindsMap.end ()) {
    // no, keywords language kind is unknown in the map
    stringstream s;

    s <<
      "MSDL language kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gGlobalMsdlCommentsTypeKindsMap.size () - 1 <<
      " known MSDL language kinds are:" <<
      endl;

    ++gIndenter;

    s <<
      existingMsdlCommentsTypeKinds (K_NAMES_LIST_MAX_LENGTH);

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

string msdlCommentsTypeKindAsString (
  msdlCommentsTypeKind languageKind)
{
  string result;

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

string existingMsdlCommentsTypeKinds (unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int
    msdlCommentsTypeKindsMapSize =
      gGlobalMsdlCommentsTypeKindsMap.size ();

  if (msdlCommentsTypeKindsMapSize) {
    unsigned int nextToLast =
      msdlCommentsTypeKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, msdlCommentsTypeKind>::const_iterator i =
        gGlobalMsdlCommentsTypeKindsMap.begin ();
      i != gGlobalMsdlCommentsTypeKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
        break;
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
    if (gGlobalOahEarlyOptions.getEarlyTracingOah () && ! gGlobalWaeOahGroup->getQuiet ()) {
      gLogStream <<
        "Initializing MSDL basic types handling" <<
        endl;
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
