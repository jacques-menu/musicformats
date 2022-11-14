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

#include <regex>

#include "visitor.h"

#include "mfServiceRunData.h"

#include "msrWae.h"

// #include "msrMeasures.h"

#include "msrClefs.h"

#include "oahOah.h"

#include "msrOah.h"


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
    case msrClefKind::kClef_NO_:
      result = "***kClef_NO_***";
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
  msrClefKind result = msrClefKind::kClef_NO_;

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
    std::stringstream s;

    s <<
      "clef std::string \"" <<
      clefString <<
      "\" is unknown" <<
      ", line = " << inputLineNumber;

    msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
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

std::string existingClefKinds (size_t namesListMaxLength)
{
  std::stringstream s;

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
        s << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != clefKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

std::string existingClefKindsNames (size_t namesListMaxLength)
{
  std::stringstream s;

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
        s << std::endl << gIndenter.getSpacer ();
        cumulatedLength = 0;
      }

      if (count == 1) {
        s << gIndenter.getSpacer ();
      }
      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != clefKindsNamesMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

//______________________________________________________________________________
S_msrClef msrClef::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrClefKind         clefKind,
  int                 clefStaffNumber)
{
  msrClef* o =
    new msrClef (
      inputLineNumber,
      upLinkToMeasure,
      clefKind,
      clefStaffNumber);
  assert (o != nullptr);
  return o;
}

S_msrClef msrClef::create (
  int                 inputLineNumber,
  msrClefKind         clefKind,
  int                 clefStaffNumber)
{
  return
    msrClef::create (
      inputLineNumber,
      gNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      clefKind,
      clefStaffNumber);
}

msrClef::msrClef (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrClefKind         clefKind,
  int                 clefStaffNumber)
    : msrMeasureElementLambda (
        inputLineNumber,
        upLinkToMeasure)
{
  fClefKind        = clefKind;
  fClefStaffNumber = clefStaffNumber;
}

msrClef::~msrClef ()
{}

S_msrClef msrClef::createClefFromString (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const std::string& clefString,
  int                 clefLineNumber)
{
  /*
    Handles clefString à la LilyPond, such as 'treble' or 'baritone'
  */

  S_msrClef result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Creating clef from std::string \"" <<
      clefString <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

  std::string regularExpression (
    "[[:space:]]*"
    "([[:alpha:]]+)" // clefName
    "[[:space:]]*"
    );

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "regularExpression = " <<
      regularExpression <<
      std::endl;
  }
#endif

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (clefString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for clef std::string \"" << clefString <<
      "\" with std::regex \"" << regularExpression <<
      "\":" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    gLogStream << std::endl;

    --gIndenter;
  }
#endif

  //  Handles clefString à la LilyPond, such as c [major] or bes minor

  if (smSize != 2) {
    std::stringstream s;

    s <<
      "clefString \"" << clefString <<
      "\" is ill-formed";

    msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  std::string clefName = sm [1];

  // compute the clefKind from the clefName
  msrClefKind
    clefKind =
      msrClefKindFromString (
        inputLineNumber,
        clefName);

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "clefName = \"" <<
      beatsNumber <<
      "\"" <<
      std::endl;
  }
#endif

  // create the clef
  result =
    msrClef::create (
      inputLineNumber,
      upLinkToMeasure,
      clefKind,
      clefLineNumber);

  return result;
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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrClef::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrClef::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrClef::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrClef::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrClef::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrClef::browseData (basevisitor* v)
{}

std::string msrClef::asString () const
{
  std::stringstream s;

  s <<
    "[Clef" <<
    ", fClefKind: " << msrClefKindAsString (fClefKind) <<
    ", fClefStaffNumber: " << fClefStaffNumber <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

std::string msrClef::asShortStringForMeasuresSlices () const
{
  std::stringstream s;

  s <<
    '[' <<
    msrClefKindAsString (fClefKind) <<
// JMI    ", fClefStaffNumber: " << fClefStaffNumber <<
// JMI    ", " << fClefStaffNumber <<
    ']';

  return s.str ();
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
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
