/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mxsrNoteEvents.h"


namespace MusicFormats
{

//________________________________________________________________________
/* this class is purely virtual
mxsrNoteEvent mxsrNoteEvent::create (
  const mxsrNoteSequentialNumber&  noteSequentialNumber,
  const mfStaffNumber&             noteStaffNumber,
  const mfVoiceNumber&             noteVoiceNumber,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
{
  mxsrNoteEvent* obj =
    new mxsrNoteEvent (
      noteSequentialNumber,
      noteStaffNumber,
      noteVoiceNumber,
      eventSequentialNumber,
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}
*/


mxsrNoteEvent::mxsrNoteEvent (
  const mxsrNoteSequentialNumber&  noteSequentialNumber,
  const mfStaffNumber&             noteStaffNumber,
  const mfVoiceNumber&             noteVoiceNumber,
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
    : mxsrEvent (
        eventSequentialNumber,
        eventInputLineNumber),
      fNoteSequentialNumber (
        noteSequentialNumber)
{
  fNoteStaffNumber = noteStaffNumber;
  fNoteVoiceNumber = noteVoiceNumber;
}

mxsrNoteEvent::~mxsrNoteEvent ()
{}


} // namespace

