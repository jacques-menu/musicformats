/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <iomanip>      // setw()), set::precision(), ...
#include <sstream>

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfServiceRunData.h"

#include "bsrKeys.h"
#include "bsrNumbers.h"

#include "bsrWae.h"

#include "oahOah.h"

#include "bsrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
string bsrKeyKindAsString (
  bsrKeyKind keyKind)
{
  string result;

  switch (keyKind) {
    case bsrKeyKind::kKeyKindNone:
      result = "***keyKindNone***";
      break;
    case bsrKeyKind::kKeyKindFlats:
      result = "kKeyKindFlats";
      break;
    case bsrKeyKind::kKeyKindNaturals:
      result = "kKeyKindNaturals";
      break;
    case bsrKeyKind::kKeyKindSharps:
      result = "kKeyKindSharps";
      break;
  } // switch

  return result;
}

ostream& operator<< (ostream& os, bsrKeyKind& elt)
{
  os << bsrKeyKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_bsrKey bsrKey::create (
  int          inputLineNumber,
  bsrKeyKind   keyKind,
  size_t numberOfAlterations)
{
  bsrKey* o =
    new bsrKey (
      inputLineNumber, keyKind, numberOfAlterations);
  assert (o != nullptr);

  return o;
}

bsrKey::bsrKey (
  int          inputLineNumber,
  bsrKeyKind   keyKind,
  size_t numberOfAlterations)
    : bsrLineContentsElement (inputLineNumber)
{
  fKeyKind             = keyKind;
  fNumberOfAlterations = numberOfAlterations;

  // consistency check
  if (
    fKeyKind == bsrKeyKind::kKeyKindNaturals
      &&
    fNumberOfAlterations != 0
  ) {
    stringstream s;

    s <<
      "BSR key inconsistency:" <<
      "keyKind: " << bsrKeyKindAsString (fKeyKind) <<
      "numberOfAlterations: " << fNumberOfAlterations;

    bsrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fKeyCellsList = buildCellsList ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Creating bsrKey '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      endl;
  }
#endif
}

bsrKey::~bsrKey ()
{}


S_bsrCellsList bsrKey::keyKindAsCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  switch (fKeyKind) {
    case bsrKeyKind::kKeyKindNone:
      break;
    case bsrKeyKind::kKeyKindFlats:
      result->
        appendCellKindToCellsList (
          kCellFlat);
      break;
    case bsrKeyKind::kKeyKindNaturals:
      result->
        appendCellKindToCellsList (
          kCellNatural);
      break;
    case bsrKeyKind::kKeyKindSharps:
      result->
        appendCellKindToCellsList (
          kCellSharp);
      break;
  } // switch

  return result;
}

S_bsrCellsList bsrKey::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (
        fInputLineNumber);

  switch (fNumberOfAlterations) {
    case 0:
      break;

    case 1:
    case 2:
    case 3:
      // create as many flat or sharp signs as needed
      for (size_t i = 1; i <= fNumberOfAlterations; ++i) {
        result->appendCellsListToCellsList (
          keyKindAsCellsList ());
      } // for
      break;

    default:
      {
        // create the number of alterations
        S_bsrNumber
          number =
            bsrNumber::create (
              fInputLineNumber,
              fNumberOfAlterations,
              bsrNumber::kNumberSignIsNeededYes);

        // append it to result
        result->appendCellsListToCellsList (
          number->fetchCellsList ());

        // append the flat or sharp sign to result
        result->appendCellsListToCellsList (
          keyKindAsCellsList ());
      }
  } // switch

  return result;
}

int bsrKey::fetchCellsNumber() const
{
  return fKeyCellsList->fetchCellsNumber();
}

void bsrKey::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrKey::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrKey>*
    p =
      dynamic_cast<visitor<S_bsrKey>*> (v)) {
        S_bsrKey elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrKey::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrKey::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrKey::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrKey>*
    p =
      dynamic_cast<visitor<S_bsrKey>*> (v)) {
        S_bsrKey elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrKey::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrKey::browseData (basevisitor* v)
{}

string bsrKey::asString () const
{
  stringstream s;

  s <<
    "Key" <<
    ", " << bsrKeyKindAsString (fKeyKind) <<
    ", numberOfAlterations: " << fNumberOfAlterations <<
    ", keyCellsList: " << fKeyCellsList->asShortString () <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string bsrKey::asDebugString () const
{
  stringstream s;

  s << "[KEY ";

  if (fNumberOfAlterations > 0) {
    switch (fKeyKind) {
      case bsrKeyKind::kKeyKindNone:
        break;

      case bsrKeyKind::kKeyKindFlats:
        switch (fNumberOfAlterations) {
          case 1:
            s << "F major";
            break;
          case 2:
            s << "Bb major";
            break;
          case 3:
            s << "Eb major";
            break;
          case 4:
            s << "Ab major";
            break;
          case 5:
            s << "Db major";
            break;
          case 6:
            s << "Gb major";
            break;
          case 7:
            s << "Cb major";
            break;
        } // switch
        break;

      case bsrKeyKind::kKeyKindNaturals:
        break;

      case bsrKeyKind::kKeyKindSharps:
        switch (fNumberOfAlterations) {
          case 1:
            s << "G major";
            break;
          case 2:
            s << "D major";
            break;
          case 3:
            s << "A major";
            break;
          case 4:
            s << "E major";
            break;
          case 5:
            s << "B major";
            break;
          case 6:
            s << "F# major";
            break;
          case 7:
            s << "C# major";
            break;
        } // switch
        break;
    } // switch
  }
  else {
    s << "C major";
  }

  s << "]";

  return s.str ();
}

void bsrKey::print (ostream& os) const
{
  os <<
    "Key" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 20;

  os <<
    setw (fieldWidth) <<
    "keyKind" << " : " << bsrKeyKindAsString (fKeyKind) <<
    endl <<
    setw (fieldWidth) <<
    "numberOfAlterations" << " : " << fNumberOfAlterations <<
    endl <<
    setw (fieldWidth) <<
    "keyCellsList" << " : " << fKeyCellsList->asShortString () <<
    endl <<
    setw (fieldWidth) <<
    "spacesBefore" << " : " << fSpacesBefore <<
    endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_bsrKey& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}


}
