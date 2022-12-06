/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw()), set::precision(), ...
#include <iostream>
#include <sstream>

#include "visitor.h"

#include "bsrNumbers.h"

#include "oahOah.h"

#include "bsrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_bsrNumber bsrNumber::create (
  int       inputLineNumber,
  int       numberValue,
  bsrNumberSignIsNeededKind
            numberSignIsNeededKind)
{
  bsrNumber* o =
    new bsrNumber (
      inputLineNumber, numberValue, numberSignIsNeededKind);
  assert (o != nullptr);
  return o;
}

bsrNumber::bsrNumber (
  int       inputLineNumber,
  int       numberValue,
  bsrNumberSignIsNeededKind
            numberSignIsNeededKind)
    : bsrLineContentsElement (inputLineNumber)
{
  fNumberValue = numberValue;

  fNumberSignIsNeededKind = numberSignIsNeededKind;

  fNumberCellsList = buildCellsList ();
}

bsrNumber::~bsrNumber ()
{}

S_bsrCellsList bsrNumber::numberValueAsCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  int n = fNumberValue;

  Bool numberValueIsNegative (false);

  if (n < 0) {
    numberValueIsNegative = true;
    n = -n;
  }

  while (n > 0) {
    int div = n / 10;
    int mod = n % 10;

/* JMI
    gLogStream << // JMI
      "% ==> numberValueAsCellsList ()" <<
      ", n: " << n <<
      ", div: " << div <<
      ", mod: " << mod <<
      std::endl;
*/

    bsrCellKind cellKind = bsrCellKind::kCellUnknown; // SEE TICINO

    switch (mod) {
      case 1: cellKind = kCell1; break;
      case 2: cellKind = kCell2; break;
      case 3: cellKind = kCell3; break;
      case 4: cellKind = kCell4; break;
      case 5: cellKind = kCell5; break;
      case 6: cellKind = kCell6; break;
      case 7: cellKind = kCell7; break;
      case 8: cellKind = kCell8; break;
      case 9: cellKind = kCell9; break;
      case 0: cellKind = kCell0; break;
      default:
        ;
    } // switch

    result->prependCellsListToCellsList (
      bsrCellsList::create (
        fInputLineNumber,
        cellKind));

    n = div;
  } // while

  if (numberValueIsNegative) {
    result->prependCellsListToCellsList (
      bsrCellsList::create (
        fInputLineNumber,
        kCell_ac_plus)); // JMI ??? other plus sign?
  }

  return result;
}

S_bsrCellsList bsrNumber::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  // append number sign if needed
  switch (fNumberSignIsNeededKind) {
    case bsrNumberSignIsNeededKind::kNumberSignIsNeededYes:
      result->appendCellsListToCellsList (
        bsrCellsList::create (
          fInputLineNumber,
          kCellNumberSign));
      break;
    case bsrNumberSignIsNeededKind::kNumberSignIsNeededNo:
      break;
  } // switch

  // append number value
  result->appendCellsListToCellsList (
    numberValueAsCellsList ());

  return result;
}

int bsrNumber::fetchCellsNumber() const
{
  return fNumberCellsList->fetchCellsNumber();
}

void bsrNumber::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrNumber::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrNumber>*
    p =
      dynamic_cast<visitor<S_bsrNumber>*> (v)) {
        S_bsrNumber elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrNumber::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrNumber::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrNumber::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrNumber>*
    p =
      dynamic_cast<visitor<S_bsrNumber>*> (v)) {
        S_bsrNumber elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrNumber::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrNumber::browseData (basevisitor* v)
{}

std::string bsrNumberSignIsNeededKindAsString (
  bsrNumberSignIsNeededKind numberSignIsNeededKind)
{
  std::string result;

  switch (numberSignIsNeededKind) {
    case bsrNumberSignIsNeededKind::kNumberSignIsNeededYes:
      result = "numberSignIsNeededYes";
      break;
    case bsrNumberSignIsNeededKind::kNumberSignIsNeededNo:
      result = "numberSignIsNeededNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const bsrNumberSignIsNeededKind& elt)
{
  os << bsrNumberSignIsNeededKindAsString (elt);
  return os;
}

std::string bsrNumber::asString () const
{
  std::stringstream s;

  s <<
    "Number" <<
    ", numberValue: " << fNumberValue <<
    ", numberSignIsNeeded: " <<
    bsrNumberSignIsNeededKindAsString (
      fNumberSignIsNeededKind) <<
    ", numberCellsList: " <<
    fNumberCellsList->asShortString () <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

std::string bsrNumber::asDebugString () const
{
  std::stringstream s;

  s <<
    "N" << // JMI
    fNumberValue <<
    ", numberSignIsNeeded: " <<
    bsrNumberSignIsNeededKindAsString (
      fNumberSignIsNeededKind) <<
    ", spacesBefore: " << fSpacesBefore;

  return s.str ();
}

void bsrNumber::print (std::ostream& os) const
{
  os <<
    "Number" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 21;

  os <<
    std::setw (fieldWidth) <<
    ", numberValue" << ": " << fNumberValue <<
    std::endl <<
    std::setw (fieldWidth) <<
    ", numberSignIsNeeded" << ": " <<
    bsrNumberSignIsNeededKindAsString (
      fNumberSignIsNeededKind) <<
    std::setw (fieldWidth) <<
    ", numberCellsList" << ": " <<
    fNumberCellsList->asShortString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "spacesBefore" << ": " << fSpacesBefore <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrNumber& elt)
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
