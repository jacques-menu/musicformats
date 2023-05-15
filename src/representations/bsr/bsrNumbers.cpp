/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw()), set::precision(), ...
#include <iostream>
#include <sstream>

#include "visitor.h"

#include "bsrNumbers.h"

#include "oahOah.h"

#include "bsrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_bsrNumber bsrNumber::create (
  int       inputLineNumber,
  int       numberValue,
  bsrNumberSignIsNeededKind
            numberSignIsNeededKind)
{
  bsrNumber* obj =
    new bsrNumber (
      inputLineNumber, numberValue, numberSignIsNeededKind);
  assert (obj != nullptr);
  return obj;
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
      bsrCellsList::create (fInputStartLineNumber);

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
    gLog << // JMI
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
        fInputStartLineNumber,
        cellKind));

    n = div;
  } // while

  if (numberValueIsNegative) {
    result->prependCellsListToCellsList (
      bsrCellsList::create (
        fInputStartLineNumber,
        kCell_ac_plus)); // JMI ??? other plus sign?
  }

  return result;
}

S_bsrCellsList bsrNumber::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputStartLineNumber);

  // append number sign if needed
  switch (fNumberSignIsNeededKind) {
    case bsrNumberSignIsNeededKind::kNumberSignIsNeededYes:
      result->appendCellsListToCellsList (
        bsrCellsList::create (
          fInputStartLineNumber,
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
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrNumber::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrNumber>*
    p =
      dynamic_cast<visitor<S_bsrNumber>*> (v)) {
        S_bsrNumber elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrNumber::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrNumber::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrNumber::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrNumber>*
    p =
      dynamic_cast<visitor<S_bsrNumber>*> (v)) {
        S_bsrNumber elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrNumber::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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
  std::stringstream ss;

  ss <<
    "Number" <<
    ", numberValue: " << fNumberValue <<
    ", numberSignIsNeeded: " <<
    bsrNumberSignIsNeededKindAsString (
      fNumberSignIsNeededKind) <<
    ", numberCellsList: " <<
    fNumberCellsList->asShortString () <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputStartLineNumber;

  return ss.str ();
}

std::string bsrNumber::asDebugString () const
{
  std::stringstream ss;

  ss <<
    "N" << // JMI
    fNumberValue <<
    ", numberSignIsNeeded: " <<
    bsrNumberSignIsNeededKindAsString (
      fNumberSignIsNeededKind) <<
    ", spacesBefore: " << fSpacesBefore;

  return ss.str ();
}

void bsrNumber::print (std::ostream& os) const
{
  os <<
    "Number" <<
    ", line " << fInputStartLineNumber <<
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
    os << "[NULL]" << std::endl;
  }

  return os;
}


}
