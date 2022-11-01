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

#include "msrClefs.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrClef msrClef::create (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure,
  msrClefKind  clefKind,
  int          clefStaffNumber)
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

msrClef::msrClef (
  int          inputLineNumber,
  S_msrMeasure upLinkToMeasure,
  msrClefKind  clefKind,
  int          clefStaffNumber)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure)
{
  fClefKind        = clefKind;
  fClefStaffNumber = clefStaffNumber;
}

msrClef::~msrClef ()
{}

//________________________________________________________________________
S_msrClef msrClef::createClefFromString (
  int           inputLineNumber,
  S_msrMeasure  upLinkToMeasure,
  const string& clefString,
  int           clefLineNumber)
{
  /*
    Handles clefString à la LilyPond, such as 'treble' or 'baritone'
  */

  S_msrClef result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Creating clef from string \"" <<
      clefString <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*"
    "([[:alpha:]]+)" // clefName
    "[[:space:]]*"
    );

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "regularExpression = " <<
      regularExpression <<
      endl;
  }
#endif

  regex  e (regularExpression);
  smatch sm;

  regex_match (clefString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for clef string \"" << clefString <<
      "\" with regex \"" << regularExpression <<
      "\":" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  //  Handles clefString à la LilyPond, such as c [major] or bes minor

  if (smSize != 2) {
    stringstream s;

    s <<
      "clefString \"" << clefString <<
      "\" is ill-formed";

    msrError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  string clefName = sm [1];

  // compute the clefKind from the clefName
  msrClefKind
    clefKind =
      clefKindFromString (
        inputLineNumber,
        clefName);

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "clefName = \"" <<
      beatsNumber <<
      "\"" <<
      endl;
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
      endl;
  }

  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrClef::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrClef::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrClef::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrClef::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrClef::browseData (basevisitor* v)
{}

string msrClef::asString () const
{
  stringstream s;

  s <<
    "[Clef" <<
    ", fClefKind: " << msrClefKindAsString (fClefKind) <<
    ", fClefStaffNumber: " << fClefStaffNumber <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

string msrClef::asShortStringForMeasuresSlices () const
{
  stringstream s;

  s <<
    '[' <<
    msrClefKindAsString (fClefKind) <<
// JMI    ", fClefStaffNumber: " << fClefStaffNumber <<
// JMI    ", " << fClefStaffNumber <<
    ']';

  return s.str ();
}

void msrClef::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator << (ostream& os, const S_msrClef& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}


}
