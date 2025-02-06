/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <iostream>
#include <sstream>

#include "visitor.h"

#include "bsrDynamics.h"

#include "oahOah.h"

#include "bsrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_bsrDynamic bsrDynamic::create (
  int             inputLineNumber,
  msrDynamicKind dynamicKind)
{
  bsrDynamic* obj =
    new bsrDynamic (
      inputLineNumber, dynamicKind);
  assert (obj != nullptr);
  return obj;
}

bsrDynamic::bsrDynamic (
  int             inputLineNumber,
  msrDynamicKind dynamicKind)
    : bsrLineContentsElement (inputLineNumber)
{
  fDynamicKind  = dynamicKind;

  fDynamicsCellsList = buildCellsList ();
}

bsrDynamic::~bsrDynamic ()
{}

S_bsrCellsList bsrDynamic::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (
        fInputLineNumber, kCellWordSign);

  switch (fDynamicKind) {
    case msrDynamicKind::kDynamicF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF));
      break;
    case msrDynamicKind::kDynamicFF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF, kCellF));
      break;
    case msrDynamicKind::kDynamicFFF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF, kCellF, kCellF));
      break;
    case msrDynamicKind::kDynamicFFFF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF, kCellF, kCellF, kCellF));
      break;
    case msrDynamicKind::kDynamicFFFFF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF, kCellF, kCellF, kCellF, kCellF));
      break;
    case msrDynamicKind::kDynamicFFFFFF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF, kCellF, kCellF, kCellF, kCellF, kCellF));
      break;

    case msrDynamicKind::kDynamicP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellP));
      break;
    case msrDynamicKind::kDynamicPP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellP, kCellP));
      break;
    case msrDynamicKind::kDynamicPPP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellP, kCellP, kCellP));
      break;
    case msrDynamicKind::kDynamicPPPP:
      result->
        appendCellsListToCellsList (
         bsrCellsList::create (
            fInputLineNumber, kCellP, kCellP, kCellP, kCellP));
      break;
    case msrDynamicKind::kDynamicPPPPP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellP, kCellP, kCellP, kCellP, kCellP));
      break;
    case msrDynamicKind::kDynamicPPPPPP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellP, kCellP, kCellP, kCellP, kCellP, kCellP));
      break;

    case msrDynamicKind::kDynamicMF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellM, kCellF));
      break;
    case msrDynamicKind::kDynamicMP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellM, kCellP));
      break;
    case msrDynamicKind::kDynamicFP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellM, kCellF));
      break;
    case msrDynamicKind::kDynamicFZ:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF, kCellZ));
      break;
    case msrDynamicKind::kDynamicPF: // JMI
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellF, kCellZ));
      break;
    case msrDynamicKind::kDynamicRF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellR, kCellF));
      break;
    case msrDynamicKind::kDynamicSF:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellS, kCellF));
      break;

    case msrDynamicKind::kDynamicRFZ:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellR, kCellF, kCellZ));
      break;
    case msrDynamicKind::kDynamicSFZ:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellS, kCellF, kCellZ));
      break;
    case msrDynamicKind::kDynamicSFP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellS, kCellF, kCellP));
      break;
    case msrDynamicKind::kDynamicSFPP:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellS, kCellF, kCellP, kCellP));
      break;
    case msrDynamicKind::kDynamicSFFZ:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellS, kCellF, kCellF, kCellZ));
      break;
    case msrDynamicKind::kDynamicSFZP: // JMI
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellS, kCellF, kCellF, kCellZ));
      break;
    case msrDynamicKind::kDynamicN: // JMI
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber, kCellS, kCellF, kCellF, kCellZ));
      break;
    case msrDynamicKind::kDynamic_UNKNOWN_:
      result =
        bsrCellsList::create (fInputLineNumber);
  } // switch

  return result;
}

int bsrDynamic::fetchCellsNumber() const
{
  return fDynamicsCellsList->fetchCellsNumber();
}

void bsrDynamic::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrDynamic::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrDynamic>*
    p =
      dynamic_cast<visitor<S_bsrDynamic>*> (v)) {
        S_bsrDynamic elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrDynamic::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrDynamic::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrDynamic::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrDynamic>*
    p =
      dynamic_cast<visitor<S_bsrDynamic>*> (v)) {
        S_bsrDynamic elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrDynamic::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrDynamic::browseData (basevisitor* v)
{}

std::string bsrDynamic::asString () const
{
  std::stringstream ss;

  ss <<
    "Dynamics" <<
    ", fDynamicKind: " <<
    fDynamicKind <<
    ", fDynamicsCellsList: " <<
    fDynamicsCellsList->asShortString () <<
    ", fSpacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

std::string bsrDynamic::asDebugString () const
{
  std::stringstream ss;

  ss <<
    "Dyn" <<
    ", fDynamicKind: " <<
    fDynamicKind <<
    ", fDynamicsCellsList: " <<
    fDynamicsCellsList->asShortString () <<
    ", fSpacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

void bsrDynamic::print (std::ostream& os) const
{
  os <<
    "Dynamics" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 23;

  os <<
    std::setw (fieldWidth) <<
    "fDynamicKind" << ": " <<
    fDynamicKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fDynamicsCellsList" << ": " <<
    fDynamicsCellsList->asShortString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSpacesBefore" << ": " << fSpacesBefore <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrDynamic& elt)
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
