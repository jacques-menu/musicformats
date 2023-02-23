/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <iomanip>      // setw()), set::precision(), ...
#include <sstream>

#include "visitor.h"

#include "mfStaticSettings.h"

#include "mfServices.h"

#include "bsrKeys.h"
#include "bsrNumbers.h"

#include "bsrWae.h"

#include "oahOah.h"

#include "bsrOah.h"

#include "waeHandlers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string bsrKeyKindAsString (
  bsrKeyKind keyKind)
{
  std::string result;

  switch (keyKind) {
    case bsrKeyKind::kKeyKindNone:
      result = "keyKindNone***";
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

std::ostream& operator << (std::ostream& os, const bsrKeyKind& elt)
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
    std::stringstream ss;

    ss <<
      "BSR key inconsistency:" <<
      "keyKind: " << bsrKeyKindAsString (fKeyKind) <<
      "numberOfAlterations: " << fNumberOfAlterations;

    bsrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  fKeyCellsList = buildCellsList ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceKeys ()) {
    std::stringstream ss;

    ss <<
      "Creating bsrKey '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
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
              bsrNumberSignIsNeededKind::kNumberSignIsNeededYes);

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
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrKey::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrKey>*
    p =
      dynamic_cast<visitor<S_bsrKey>*> (v)) {
        S_bsrKey elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrKey::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrKey::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrKey::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrKey>*
    p =
      dynamic_cast<visitor<S_bsrKey>*> (v)) {
        S_bsrKey elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrKey::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrKey::browseData (basevisitor* v)
{}

std::string bsrKey::asString () const
{
  std::stringstream ss;

  ss <<
    "Key" <<
    ", " << bsrKeyKindAsString (fKeyKind) <<
    ", numberOfAlterations: " << fNumberOfAlterations <<
    ", keyCellsList: " << fKeyCellsList->asShortString () <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

std::string bsrKey::asDebugString () const
{
  std::stringstream ss;

  ss << "[KEY ";

  if (fNumberOfAlterations > 0) {
    switch (fKeyKind) {
      case bsrKeyKind::kKeyKindNone:
        break;

      case bsrKeyKind::kKeyKindFlats:
        switch (fNumberOfAlterations) {
          case 1:
            ss << "F major";
            break;
          case 2:
            ss << "Bb major";
            break;
          case 3:
            ss << "Eb major";
            break;
          case 4:
            ss << "Ab major";
            break;
          case 5:
            ss << "Db major";
            break;
          case 6:
            ss << "Gb major";
            break;
          case 7:
            ss << "Cb major";
            break;
        } // switch
        break;

      case bsrKeyKind::kKeyKindNaturals:
        break;

      case bsrKeyKind::kKeyKindSharps:
        switch (fNumberOfAlterations) {
          case 1:
            ss << "G major";
            break;
          case 2:
            ss << "D major";
            break;
          case 3:
            ss << "A major";
            break;
          case 4:
            ss << "E major";
            break;
          case 5:
            ss << "B major";
            break;
          case 6:
            ss << "F# major";
            break;
          case 7:
            ss << "C# major";
            break;
        } // switch
        break;
    } // switch
  }
  else {
    ss << "C major";
  }

  ss << ']';

  return ss.str ();
}

void bsrKey::print (std::ostream& os) const
{
  os <<
    "Key" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 20;

  os <<
    std::setw (fieldWidth) <<
    "keyKind" << ": " << bsrKeyKindAsString (fKeyKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "numberOfAlterations" << ": " << fNumberOfAlterations <<
    std::endl <<
    std::setw (fieldWidth) <<
    "keyCellsList" << ": " << fKeyCellsList->asShortString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "spacesBefore" << ": " << fSpacesBefore <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrKey& elt)
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
