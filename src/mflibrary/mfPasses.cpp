/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <map>

// #include "mfWaeInterface.h"

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

    case mfPassIDKind::kMfPassID_OptionsAndArgumentsHandling:
      result = "Opt & args";
      break;

    case mfPassIDKind::kMfPassID_DisplayPass:
      result = "Display";
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

EXP mfIndentedStringStream& operator << (
  mfIndentedStringStream& iss, const mfPassIDKind& elt)
{
  iss << mfPassIDKindAsString (elt);
  return iss;
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
    std::stringstream ss;

    ss <<
      "MusicFormats passID kind '" << theString <<
      "' is unknown" <<
      std::endl <<
      "The " <<
      gGlobalMusicFormatsPassIDKindsMap.size () - 1 <<
      " known MusicFormats passID kinds are:" <<
      std::endl;

    ++gIndenter;

    ss <<
      availableMusicFormatsPassIDKinds (K_MF_NAMES_LIST_MAX_LENGTH);

    --gIndenter;

    oahError (ss.str ());
  }

  result = (*it).second;

  return result;
}

std::string availableMusicFormatsPassIDKinds (size_t namesListMaxLength)
{
  std::stringstream ss;

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
      else if (count != mfPassIDKindsMapSize) {
        ss << ", ";
      }
    } // for
  }

  return ss.str ();
}


void initializeMusicFormatsPassIDKindsMap ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
    gGlobalMusicFormatsPassIDKindsMap ["opt&args"] = // JMI v0.9.67
      mfPassIDKind::kMfPassID_OptionsAndArgumentsHandling;

    gGlobalMusicFormatsPassIDKindsMap ["display"] = // JMI v0.9.67
      mfPassIDKind::kMfPassID_DisplayPass;

    gGlobalMusicFormatsPassIDKindsMap ["1"] =
      mfPassIDKind::kMfPassID_1;

    gGlobalMusicFormatsPassIDKindsMap ["2"] =
      mfPassIDKind::kMfPassID_2;
    gGlobalMusicFormatsPassIDKindsMap ["2a"] =
      mfPassIDKind::kMfPassID_2a;
    gGlobalMusicFormatsPassIDKindsMap ["2b"] =
      mfPassIDKind::kMfPassID_2b;

    gGlobalMusicFormatsPassIDKindsMap ["3"] =
      mfPassIDKind::kMfPassID_3;
    gGlobalMusicFormatsPassIDKindsMap ["3a"] =
      mfPassIDKind::kMfPassID_3a;
    gGlobalMusicFormatsPassIDKindsMap ["3b"] =
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
#ifdef MF_TRACE_IS_ENABLED
    if (
      gEarlyOptions.getTraceEarlyOptions ()
        &&
      ! gEarlyOptions.getEarlyQuietOption ()
    ) {
	  	std::stringstream ss;

      ss <<
        "Initializing MusicFormats passIDs" <<
        std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

    // passIDs handling
    // ------------------------------------------------------

    initializeMusicFormatsPassIDKindsMap ();

    pPrivateThisMethodHasBeenRun = true;
  }
}

//______________________________________________________________________________
S_mfPassDescription mfPassDescription::create (
  mfPassIDKind passIDKind,
  std::string  passDescription)
{
  mfPassDescription* o =
    new mfPassDescription (
      passIDKind,
      passDescription);
  assert (o != nullptr);
  return o;
}

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
  std::stringstream ss;

  ss <<
    "fPassIDKind: " << fPassIDKind <<
    ", fPassDescription: " << fPassDescription;

  return ss.str ();
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
// hidden global PassIDKind variable
mfPassIDKind pGlobalCurrentPassIDKind =
  mfPassIDKind::kMfPassID_ALL; // default value for passes trace

EXP void setGlobalCurrentPassIDKind (mfPassIDKind passIDKind)
{
  pGlobalCurrentPassIDKind = passIDKind;
}

EXP mfPassIDKind getGlobalCurrentPassIDKind ()
{
  return pGlobalCurrentPassIDKind;
}


}

