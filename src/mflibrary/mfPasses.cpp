/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <map>

#include "mfWaeInterface.h"

#include "mfConstants.h"
#include "mfIndentedTextOutput.h"
#include "mfPasses.h"

#include "oahWae.h"


namespace MusicFormats
{

//______________________________________________________________________________

std::map<std::string, mfPassIDKind>
  gGlobalMusicFormatsPassIDKindsMap;

std::string mfPassIDKindAsString (
  mfPassIDKind passIDKind)
{
  std::string result;

  // these strings are used in the command line options
  switch (passIDKind) {
    case mfPassIDKind::kMfPassID_UNKNOWN:
      result = "*kMfPassID_UNKNOWN*";
      break;

    case mfPassIDKind::kMfPassID_ALL:
      result = "*kMfPassID_ALL*";
      break;

    case mfPassIDKind::kMfPassID_0:
      result = "0";
      break;

    case mfPassIDKind::kMfPassID_1:
      result = "1";
      break;

    case mfPassIDKind::kMfPassID_2:
      result = "2";
      break;
    case mfPassIDKind::kMfPassID_2a:
      result = "2a";
      break;
    case mfPassIDKind::kMfPassID_2b:
      result = "2b";
      break;

    case mfPassIDKind::kMfPassID_3:
      result = "3";
      break;
    case mfPassIDKind::kMfPassID_3a:
      result = "3a";
      break;
    case mfPassIDKind::kMfPassID_3b:
      result = "3b";
      break;

    case mfPassIDKind::kMfPassID_4:
      result = "4";
      break;
    case mfPassIDKind::kMfPassID_4a:
      result = "4a";
      break;
    case mfPassIDKind::kMfPassID_4b:
      result = "4b";
      break;

    case mfPassIDKind::kMfPassID_5:
      result = "5";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mfPassIDKind elt)
{
  os << mfPassIDKindAsString (elt);
  return os;
}

mfPassIDKind mfPassIDKindFromString (const std::string& theString)
{
  mfPassIDKind result =
    mfPassIDKind::kMfPassID_UNKNOWN;

  std::map<std::string, mfPassIDKind>::const_iterator
    it =
      gGlobalMusicFormatsPassIDKindsMap.find (
        theString);

  if (it == gGlobalMusicFormatsPassIDKindsMap.end ()) {
    // no, keywords passID kind is unknown in the map
    std::stringstream s;

    s <<
      "MusicFormats passID kind '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMusicFormatsPassIDKindsMap.size () - 1 <<
      " known MusicFormats passID kinds are:" <<
      std::endl;

    ++gIndenter;

    s <<
      availableMusicFormatsPassIDKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (s.str ());
  }

  result = (*it).second;

  return result;
}

std::string availableMusicFormatsPassIDKinds (size_t namesListMaxLength)
{
  std::stringstream s;

  size_t
    mfPassIDKindsMapSize =
      gGlobalMusicFormatsPassIDKindsMap.size ();

  if (mfPassIDKindsMapSize) {
    size_t nextToLast =
      mfPassIDKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::pair<std::string, mfPassIDKind>
        thePair :
          gGlobalMusicFormatsPassIDKindsMap
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
      else if (count != mfPassIDKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}


void initializeMusicFormatsPassIDKindsMap ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
    gGlobalMusicFormatsPassIDKindsMap ["0"]   =
      mfPassIDKind::kMfPassID_0;

    gGlobalMusicFormatsPassIDKindsMap ["1"]   =
      mfPassIDKind::kMfPassID_1;

    gGlobalMusicFormatsPassIDKindsMap ["2"]  =
      mfPassIDKind::kMfPassID_2;
    gGlobalMusicFormatsPassIDKindsMap ["2a"]  =
      mfPassIDKind::kMfPassID_2a;
    gGlobalMusicFormatsPassIDKindsMap ["2b"]   =
      mfPassIDKind::kMfPassID_2b;

    gGlobalMusicFormatsPassIDKindsMap ["3"]   =
      mfPassIDKind::kMfPassID_3;
    gGlobalMusicFormatsPassIDKindsMap ["3a"]   =
      mfPassIDKind::kMfPassID_3a;
    gGlobalMusicFormatsPassIDKindsMap ["3b"]   =
      mfPassIDKind::kMfPassID_3b;

    gGlobalMusicFormatsPassIDKindsMap ["4"] =
      mfPassIDKind::kMfPassID_4;
    gGlobalMusicFormatsPassIDKindsMap ["4a"] =
      mfPassIDKind::kMfPassID_4a;
    gGlobalMusicFormatsPassIDKindsMap ["4b"] =
      mfPassIDKind::kMfPassID_4b;

    gGlobalMusicFormatsPassIDKindsMap ["5"] =
      mfPassIDKind::kMfPassID_5;

    pPrivateThisMethodHasBeenRun = true;
  }
}

void initializeMusicFormatsPassIDs ()
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
        "Initializing MusicFormats passIDs" <<
        std::endl;
  }
#endif

    // passIDs handling
    // ------------------------------------------------------

    initializeMusicFormatsPassIDKindsMap ();

    pPrivateThisMethodHasBeenRun = true;
  }
}

//______________________________________________________________________________
mfPassDescription::mfPassDescription (
  mfPassIDKind passIDKind,
  std::string  passDescription)
{
  fPassIDKind      = passIDKind;
  fPassDescription = passDescription;
}

mfPassDescription::~mfPassDescription ()
{}

std::string mfPassDescription::asString () const
{
  std::stringstream s;

  s <<
    "fPassIDKind: " << fPassIDKind <<
    ", fPassDescription: " << fPassDescription;

  return s.str ();
}

void mfPassDescription::print (std::ostream& os) const
{
  os << asString ();
}

std::ostream& operator << (std::ostream& os, const S_mfPassDescription& elt) {
  os << elt->asString ();
  return os;
}

//______________________________________________________________________________
// global variable

EXP mfPassIDKind gGlobalCurrentPassIDKind =
  mfPassIDKind::kMfPassID_ALL; // default value for passes tracing

extern EXP void setGlobalCurrentPassIDKind (mfPassIDKind passIDKind)
{
  gGlobalCurrentPassIDKind = passIDKind;
}

extern EXP mfPassIDKind getGlobalCurrentPassIDKind ()
{
  return gGlobalCurrentPassIDKind;
}


}

