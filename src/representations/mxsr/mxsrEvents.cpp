/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

#include <map>

#include "mfAssert.h"
#include "mfStringsHandling.h"

#include "mxsrEvents.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
/* this class is purely virtual
S_mxsrEvent mxsrEvent::create (
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
{
  mxsrEvent* obj =
    new mxsrEvent (
      eventInputLineNumber);
  assert (obj != nullptr);
  return obj;
}
*/

mxsrEvent::mxsrEvent (
  const mxsrEventSequentialNumber& eventSequentialNumber,
  const mfInputLineNumber&         eventInputLineNumber)
    : fEventSequentialNumber (
        eventSequentialNumber),
      fEventInputLineNumber (
        eventInputLineNumber)
{}

mxsrEvent::~mxsrEvent ()
{}

// std::ostream& operator << (std::ostream& os, const S_mxsrEvent& elt)
// {
//   if (elt) {
//     elt->print (os);
//   }
//   else {
//     os << "[NULL]" << std::endl;
//   }
//
//   return os;
// }
//
// std::ostream& operator << (std::ostream& os, const mxsrEvent& elt)
// {
//   elt.print (os);
//   return os;
// }

//________________________________________________________________________
// bool compareEventsByIncreasingSequentialNumber (
//   S_mxsrEvent& first,
//   S_mxsrEvent& second)
// {
//   return
//     first->getEventSequentialNumber ()
//       <
//     second->getEventSequentialNumber ();
// }
//
// bool compareEventsByIncreasingInputLineNumber (
//   S_mxsrEvent& first,
//   S_mxsrEvent& second)
// {
//   return
//     first->getEventInputLineNumber ()
//       <
//     second->getEventInputLineNumber ();
// }
//
// bool compareEventsByIncreasingEventNoteSequentialNumber (
//   S_mxsrEvent& first,
//   S_mxsrEvent& second)
// {
//   return
//     first->getNoteSequentialNumber ()
//       <
//     second->getNoteSequentialNumber ();
// }
//
// bool operator< (
//   const S_mxsrEvent& first,
//   const S_mxsrEvent& second)
// {
//   return
//     first->getNoteSequentialNumber ()
//       <
//     second->getNoteSequentialNumber ();
// }
//
// bool compareStaffChangeEventsByIncreasingInputLineNumber (
//   S_mxsrStaffChangeEvent& first,
//   S_mxsrStaffChangeEvent& second)
// {
//   return
//     first->getEventInputLineNumber ()
//       <
//     second->getEventInputLineNumber ();
// }


} // namespace

