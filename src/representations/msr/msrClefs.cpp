/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <regex>
#include <sstream>

#include "mfStaticSettings.h"

#include "visitor.h"

#include "mfAssert.h"
#include "mfServices.h"

#include "msrMeasureConstants.h"

#include "msrWae.h"

#include "msrClefsKeysTimeSignatures.h"

#include "oahOah.h"

#include "oahEarlyOptions.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

// clefs
//______________________________________________________________________________
std::map<std::string, msrClefKind>
  gGlobalClefKindsMap;

std::list<std::string>
  gClefKindsNamesList;

std::string msrClefKindAsString (
  msrClefKind clefKind)
{
  std::string result;

  switch (clefKind) {
    case msrClefKind::kClef_UNKNOWN_:
      result = "kClef_UNKNOWN_";
      break;
    case msrClefKind::kClefTreble:
      result = "kClefTreble";
      break;
    case msrClefKind::kClefSoprano:
      result = "kClefSoprano";
      break;
    case msrClefKind::kClefMezzoSoprano:
      result = "kClefMezzoSoprano";
      break;
    case msrClefKind::kClefAlto:
      result = "kClefAlto";
      break;
    case msrClefKind::kClefTenor:
      result = "kClefTenor";
      break;
    case msrClefKind::kClefBaritone:
      result = "kClefBaritone";
      break;
    case msrClefKind::kClefBass:
      result = "kClefBass";
      break;
    case msrClefKind::kClefTrebleLine1:
      result = "kClefTrebleLine1";
      break;
    case msrClefKind::kClefTrebleMinus15:
      result = "kClefTrebleMinus15";
      break;
    case msrClefKind::kClefTrebleMinus8:
      result = "kClefTrebleMinus8";
      break;
    case msrClefKind::kClefTreblePlus8:
      result = "kClefTreblePlus8";
      break;
    case msrClefKind::kClefTreblePlus15:
      result = "kClefTreblePlus15";
      break;
    case msrClefKind::kClefBassMinus15:
      result = "kClefBassMinus15";
      break;
    case msrClefKind::kClefBassMinus8:
      result = "kClefBassMinus8";
      break;
    case msrClefKind::kClefBassPlus8:
      result = "kClefBassPlus8";
      break;
    case msrClefKind::kClefBassPlus15:
      result = "kClefBassPlus15";
      break;
    case msrClefKind::kClefVarbaritone:
      result = "kClefVarbaritone";
      break;
    case msrClefKind::kClefTablature4:
      result = "kClefTablature4";
      break;
    case msrClefKind::kClefTablature5:
      result = "kClefTablature5";
      break;
    case msrClefKind::kClefTablature6:
      result = "kClefTablature6";
      break;
    case msrClefKind::kClefTablature7:
      result = "kClefTablature7";
      break;
    case msrClefKind::kClefPercussion:
      result = "kClefPercussion";
      break;
    case msrClefKind::kClefJianpu:
      result = "kClefJianpu";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrClefKind& elt)
{
  os << msrClefKindAsString (elt);
  return os;
}

msrClefKind msrClefKindFromString (
  int           inputLineNumber,
  const std::string& clefString)
{
  msrClefKind result = msrClefKind::kClef_UNKNOWN_;

  if      (clefString == "treble")
    result = msrClefKind::kClefTreble;
  else if (clefString == "soprano")
    result = msrClefKind::kClefSoprano;
  else if (clefString == "mezzosoprano")
    result = msrClefKind::kClefMezzoSoprano;
  else if (clefString == "alto")
    result = msrClefKind::kClefAlto;
  else if (clefString == "tenor")
    result = msrClefKind::kClefTenor;
  else if (clefString == "baritone")
    result = msrClefKind::kClefBaritone;
  else if (clefString == "bass")
    result = msrClefKind::kClefBass;
  else if (clefString == "treble1")
    result = msrClefKind::kClefTrebleLine1;
  else if (clefString == "treble-15")
    result = msrClefKind::kClefTrebleMinus15;
  else if (clefString == "treble-8")
    result = msrClefKind::kClefTrebleMinus8;
  else if (clefString == "treble+8")
    result = msrClefKind::kClefTreblePlus8;
  else if (clefString == "treble+15")
    result = msrClefKind::kClefTreblePlus15;
  else if (clefString == "bass-15")
    result = msrClefKind::kClefBassMinus15;
  else if (clefString == "bass-8")
    result = msrClefKind::kClefBassMinus8;
  else if (clefString == "bass+8")
    result = msrClefKind::kClefBassPlus8;
  else if (clefString == "bass+15")
    result = msrClefKind::kClefBassPlus15;
  else if (clefString == "varbaritone")
    result = msrClefKind::kClefVarbaritone;
  else if (clefString == "tab4")
    result = msrClefKind::kClefTablature4;
  else if (clefString == "tab5")
    result = msrClefKind::kClefTablature5;
  else if (clefString == "tab6")
    result = msrClefKind::kClefTablature6;
  else if (clefString == "tab7")
    result = msrClefKind::kClefTablature7;
  else if (clefString == "percussion")
    result = msrClefKind::kClefPercussion;
  else if (clefString == "jianpu")
    result = msrClefKind::kClefJianpu;
  else {
    std::stringstream ss;

    ss <<
      "clef string \"" <<
      clefString <<
      "\" is unknown" <<
      ", line: " << inputLineNumber;

    msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  return result;
}

void initializeClefKinds ()
{
  // protect library against multiple initializations
  static Bool pPrivateThisMethodHasBeenRun (false);

  if (! pPrivateThisMethodHasBeenRun) {
  //  gGlobalClefKindsMap [gClefKindsNamesList.back ()] = kMajorClef;

    gClefKindsNamesList.push_back ("treble");
    gGlobalClefKindsMap ["treble"] = msrClefKind::kClefTreble;
    gClefKindsNamesList.push_back ("soprano");
    gGlobalClefKindsMap ["soprano"] = msrClefKind::kClefSoprano;
    gClefKindsNamesList.push_back ("mezzosoprano");
    gGlobalClefKindsMap ["mezzosoprano"] = msrClefKind::kClefMezzoSoprano;
    gClefKindsNamesList.push_back ("alto");
    gGlobalClefKindsMap ["alto"] = msrClefKind::kClefAlto;
    gClefKindsNamesList.push_back ("tenor");
    gGlobalClefKindsMap ["tenor"] = msrClefKind::kClefTenor;
    gClefKindsNamesList.push_back ("baritone");
    gGlobalClefKindsMap ["baritone"] = msrClefKind::kClefBaritone;
    gClefKindsNamesList.push_back ("bass");
    gGlobalClefKindsMap ["bass"] = msrClefKind::kClefBass;
    gClefKindsNamesList.push_back ("treble1");
    gGlobalClefKindsMap ["treble1"] = msrClefKind::kClefTrebleLine1;
    gClefKindsNamesList.push_back ("treble-15");
    gGlobalClefKindsMap ["treble-15"] = msrClefKind::kClefTrebleMinus15;
    gClefKindsNamesList.push_back ("treble-8");
    gGlobalClefKindsMap ["treble-8"] = msrClefKind::kClefTrebleMinus8;
    gClefKindsNamesList.push_back ("treble+8");
    gGlobalClefKindsMap ["treble+8"] = msrClefKind::kClefTreblePlus8;
    gClefKindsNamesList.push_back ("treble+15");
    gGlobalClefKindsMap ["treble+15"] = msrClefKind::kClefTreblePlus15;
    gClefKindsNamesList.push_back ("bass-15");
    gGlobalClefKindsMap ["bass-15"] = msrClefKind::kClefBassMinus15;
    gClefKindsNamesList.push_back ("bass-8");
    gGlobalClefKindsMap ["bass-8"] = msrClefKind::kClefBassMinus8;
    gClefKindsNamesList.push_back ("bass+8");
    gGlobalClefKindsMap ["bass+8"] = msrClefKind::kClefBassPlus8;
    gClefKindsNamesList.push_back ("bass+15");
    gGlobalClefKindsMap ["bass+15"] = msrClefKind::kClefBassPlus15;
    gClefKindsNamesList.push_back ("varbaritone");
    gGlobalClefKindsMap ["varbaritone"] = msrClefKind::kClefVarbaritone;
    gClefKindsNamesList.push_back ("tab4");
    gGlobalClefKindsMap ["tab4"] = msrClefKind::kClefTablature4;
    gClefKindsNamesList.push_back ("tab5");
    gGlobalClefKindsMap ["tab5"] = msrClefKind::kClefTablature5;
    gClefKindsNamesList.push_back ("tab6");
    gGlobalClefKindsMap ["tab6"] = msrClefKind::kClefTablature6;
    gClefKindsNamesList.push_back ("tab7");
    gGlobalClefKindsMap ["tab7"] = msrClefKind::kClefTablature7;
    gClefKindsNamesList.push_back ("percussion");
    gGlobalClefKindsMap ["percussion"] = msrClefKind::kClefPercussion;
    gClefKindsNamesList.push_back ("jianpu");
    gGlobalClefKindsMap ["jianpu"] = msrClefKind::kClefJianpu;

    pPrivateThisMethodHasBeenRun = true;
  }
}

std::string availableClefKinds (size_t namesListMaxLength)
{
  std::stringstream ss;

  size_t clefKindsMapSize =
    gGlobalClefKindsMap.size ();

  if (clefKindsMapSize) {
    size_t nextToLast =
      clefKindsMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::map<std::string, msrClefKind>::const_iterator i =
        gGlobalClefKindsMap.begin ();
      i != gGlobalClefKindsMap.end ();
      ++i
    ) {
      std::string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        ss << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        ss << gIndenter.getSpacer ();
      }
      ss << theString;

      if (count == nextToLast) {
        ss << " and ";
      }
      else if (count != clefKindsMapSize) {
        ss << ", ";
      }
    } // for
  }

  return ss.str ();
}

std::string availableClefKindsNames (size_t namesListMaxLength)
{
  std::stringstream ss;

  size_t clefKindsNamesMapSize =
    gClefKindsNamesList.size ();

  if (clefKindsNamesMapSize) {
    size_t nextToLast =
      clefKindsNamesMapSize - 1;

    size_t count = 0;
    size_t cumulatedLength = 0;

    for (
      std::list<std::string>::const_iterator i =
        gClefKindsNamesList.begin ();
      i != gClefKindsNamesList.end ();
      ++i
    ) {
      std::string theString = (*i);

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        ss << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        ss << gIndenter.getSpacer ();
      }
      ss << theString;

      if (count == nextToLast) {
        ss << " and ";
      }
      else if (count != clefKindsNamesMapSize) {
        ss << ", ";
      }
    } // for
  }

  return ss.str ();
}

//______________________________________________________________________________
S_msrClef msrClef::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrClefKind         clefKind,
  int                 clefStaffNumber)
{
  msrClef* obj =
    new msrClef (
      inputLineNumber,
      upLinkToMeasure,
      clefKind,
      clefStaffNumber);
  assert (obj != nullptr);
  return obj;
}

S_msrClef msrClef::create (
  int                 inputLineNumber,
  msrClefKind         clefKind,
  int                 clefStaffNumber)
{
  return
    msrClef::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      clefKind,
      clefStaffNumber);
}

msrClef::msrClef (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrClefKind         clefKind,
  int                 clefStaffNumber)
    : msrMeasureElement (
        inputLineNumber)
{
  fClefKind        = clefKind;
  fClefStaffNumber = clefStaffNumber;
}

msrClef::~msrClef ()
{}

S_msrClef msrClef::createClefFromString (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const std::string&  clefString,
  int                 clefLineNumber)
{
  /*
    Handles clefString à la LilyPond, such as 'treble' or 'baritone'
  */

  S_msrClef result;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating clef from string \"" <<
      clefString <<
      "', line " << inputLineNumber <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string regularExpression (
    "[[:space:]]*"
    "([[:alpha:]]+)" // clefName
    "[[:space:]]*"
    );

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "regularExpression: " <<
      regularExpression <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (clefString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for clef string \"" << clefString <<
      "\" with std::regex \"" << regularExpression <<
      "\":" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLog <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    gLog << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  //  Handles clefString à la LilyPond, such as c [major] or bes minor

  if (smSize != 2) {
    std::stringstream ss;

    ss <<
      "clefString \"" << clefString <<
      "\" is ill-formed";

    msrError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  std::string clefName = sm [1];

  // compute the clefKind from the clefName
  msrClefKind
    clefKind =
      msrClefKindFromString (
        inputLineNumber,
        clefName);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "clefName: \"" <<
      clefName <<
      "\"" <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the clef
  result =
    msrClef::create (
      inputLineNumber,
      upLinkToMeasure,
      clefKind,
      clefLineNumber);

  return result;
}

void msrClef::setClefUpLinkToMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceClefs ()) {
    ++gIndenter;

    gLog <<
      "Setting the uplink to measure of clef " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  fClefUpLinkToMeasure = measure;
}

Bool msrClef::clefIsATablatureClef () const
{
  switch (fClefKind) {
    case msrClefKind::kClefTablature4:
    case msrClefKind::kClefTablature5:
    case msrClefKind::kClefTablature6:
    case msrClefKind::kClefTablature7:
      return true;
      break;
    default:
      return false;
  } // switch
}

Bool msrClef::clefIsAPercussionClef () const
{
  switch (fClefKind) {
    case msrClefKind::kClefPercussion:
      return true;
      break;
    default:
      return false;
  } // switch
}

Bool msrClef::isEqualTo (S_msrClef otherClef) const
{
  if (! otherClef) {
    return false;
  }

  return
    fClefKind == otherClef->fClefKind
      &&
    fClefStaffNumber == otherClef->fClefStaffNumber;
}

void msrClef::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrClef::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrClef::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
//             gServiceRunData->getCurrentMeasureNumber (),
//             gServiceRunData->getScoreMeasuresNumber ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrClef::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrClef::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
//       gServiceRunData->getCurrentMeasureNumber (),
//       gServiceRunData->getScoreMeasuresNumber ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrClef::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
//             gServiceRunData->getCurrentMeasureNumber (),
//             gServiceRunData->getScoreMeasuresNumber ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrClef::browseData (basevisitor* v)
{}

std::string msrClef::asString () const
{
  std::stringstream ss;

  ss <<
    "[Clef" <<
    ", fClefKind: " << msrClefKindAsString (fClefKind) <<
    ", fClefStaffNumber: " << fClefStaffNumber <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

std::string msrClef::asShortStringForMeasuresSlices () const
{
  std::stringstream ss;

  ss <<
    '[' <<
    msrClefKindAsString (fClefKind) <<
// JMI    ", fClefStaffNumber: " << fClefStaffNumber <<
// JMI    ", " << fClefStaffNumber <<
    ']';

  return ss.str ();
}

void msrClef::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrClef& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
