/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>

#include "mxsrStaffChangeEvents.h"


namespace MusicFormats
{

//________________________________________________________________________
std::string mxsrStaffChangeEventKindAsString (
  mxsrStaffChangeEventKind noteStaffChangeEventKind)
{
  std::string result;

  switch (noteStaffChangeEventKind) {
    case mxsrStaffChangeEventKind::kStaffChangeEvent_NONE:
      result = "kStaffChangeEvent_NONE";
      break;
    case mxsrStaffChangeEventKind::kStaffChangeEventTakeOff:
      result = "kStaffChangeEventTakeOff";
      break;
    case mxsrStaffChangeEventKind::kStaffChangeEventLanding:
      result = "kStaffChangeEventLanding";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const mxsrStaffChangeEventKind& elt)
{
  os << mxsrStaffChangeEventKindAsString (elt);
  return os;
}

//________________________________________________________________________
S_mxsrStaffChangeEvent mxsrStaffChangeEvent::create (
  mxsrStaffChangeEventKind         staffChangeEventKind,
  const mxsrNoteSequentialNumber & noteSequentialNumber,
  const mfStaffNumber&             noteStaffNumber,
  const mfVoiceNumber&             noteVoiceNumber,
  const mfStaffNumber&             takeOffStaffNumber,
  const mfInputLineNumber&         takeOffInputLineNumber, // superflous ???
  const mfStaffNumber&             landingStaffNumber,
  const mfInputLineNumber&         landingInputLineNumber, // superflous ???
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
{
  mxsrStaffChangeEvent* obj =
    new mxsrStaffChangeEvent (
      staffChangeEventKind,
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      takeOffStaffNumber,
      takeOffInputLineNumber,
      landingStaffNumber,
      landingInputLineNumber,
      eventSequentialNumber,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}

mxsrStaffChangeEvent::mxsrStaffChangeEvent (
  mxsrStaffChangeEventKind         staffChangeEventKind,
  const mxsrNoteSequentialNumber & noteSequentialNumber,
  const mfStaffNumber&             noteStaffNumber,
  const mfVoiceNumber&             noteVoiceNumber,
  const mfStaffNumber&             takeOffStaffNumber,
  const mfInputLineNumber&         takeOffInputLineNumber, // superflous ???
  const mfStaffNumber&             landingStaffNumber,
  const mfInputLineNumber&         landingInputLineNumber, // superflous ???
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
  : mxsrNoteEvent (
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      eventSequentialNumber,
      eventInputLineNumber)
{
  gLog <<
    "======>>>> " <<
    "takeOffStaffNumber: " << takeOffStaffNumber <<
    ", landingStaffNumber: " << landingStaffNumber <<
    std::endl;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  if (landingStaffNumber == takeOffStaffNumber) {
    std::stringstream ss;

    ss <<
      "staff change event" <<
      staffChangeEventKind <<
      " to one and the same staff number " <<
      landingStaffNumber;

    mfAssert (
      __FILE__, __LINE__,
      false,
      ss.str ());
  }
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fStaffChangeEventKind = staffChangeEventKind;

  fTakeOffStaffNumber = takeOffStaffNumber;
  fLandingStaffNumber = landingStaffNumber;

  fTakeOffInputLineNumber = takeOffInputLineNumber;
  fLandingInputLineNumber = landingInputLineNumber;
}

mxsrStaffChangeEvent::~mxsrStaffChangeEvent ()
{}

std::string mxsrStaffChangeEvent::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[StaffChangeEvent" <<
    ", fStaffChangeEventKind: " << fStaffChangeEventKind <<
    ", fEventInputLineNumber: " << fEventInputLineNumber <<

    ", fTakeOffInputLineNumber: " << fTakeOffInputLineNumber <<
    ", fLandingInputLineNumber: " << fLandingInputLineNumber <<
    ", fNoteSequentialNumber: " << fNoteSequentialNumber <<
    ", fNoteStaffNumber: " << fNoteStaffNumber <<
    ", fNoteVoiceNumber: " << fNoteVoiceNumber <<
    ", staff number change: " <<
    fTakeOffStaffNumber << " ->> " << fLandingStaffNumber <<
    ", fLandingInputLineNumber: " << fLandingInputLineNumber <<

    ", fEventSequentialNumber: " << fEventSequentialNumber <<
    ']';

  return ss.str ();
}

std::string mxsrStaffChangeEvent::asString () const
{
  return asShortString ();
}

void mxsrStaffChangeEvent::print (std::ostream& os) const
{
  os <<
    "[StaffChangeEvent" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 29;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fStaffChangeEventKind" << ": " << fStaffChangeEventKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fEventInputLineNumber" << ": " << fEventInputLineNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "staff change" << ": " <<
    fTakeOffStaffNumber << "/" << "" << fNoteVoiceNumber <<
    " ->> " <<
    fLandingStaffNumber << "/" << "" << fNoteVoiceNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteSequentialNumber" << ": " << fNoteSequentialNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteStaffNumber" << ": " << fNoteStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteVoiceNumber" << ": " << fNoteVoiceNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTakeOffStaffNumber" << ": " << fTakeOffStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fTakeOffInputLineNumber" << ": " << fTakeOffInputLineNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fLandingStaffNumber" << ": " << fLandingStaffNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fLandingInputLineNumber" << ": " << fLandingInputLineNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fEventSequentialNumber" << ": " << fEventSequentialNumber <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mxsrStaffChangeEvent& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const mxsrStaffChangeEvent& elt)
{
  elt.print (os);
  return os;
}


} // namespace

