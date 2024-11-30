/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>

#include "mfWrapperTypes.h"


namespace MusicFormats
{

// //______________________________________________________________________________
// mfInputLineNumber::mfInputLineNumber (int value)
// {
//   fValue = value;
// }
//
// mfInputLineNumber::~mfInputLineNumber ()
// {}
//
// mfInputLineNumber::operator int () const
// {
//   return fValue;
// }
//
// Bool mfInputLineNumber::operator == (
//   const mfInputLineNumber& otherInputLineNumber) const
// {
//   return fValue == otherInputLineNumber.fValue;
// }
//
// EXP std::ostream& operator << (std::ostream& os, const mfInputLineNumber& elt)
// {
//   os << elt.getValue ();
//   return os;
// }
//
// //______________________________________________________________________________
// mfInputLocation::mfInputLocation (
//   mfInputLineNumber inputStartLineNumber,
//   mfInputLineNumber inputEndLineNumber)
//     : fInputStartLineNumber (
//         inputStartLineNumber),
//       fInputEndLineNumber (
//         inputEndLineNumber)
// {}
//
// mfInputLocation::~mfInputLocation ()
// {}
//
// std::string mfInputLocation::asString () const
// {
//   std::stringstream ss;
//
//   mfInputLineNumber
//     startInputLineNumber = fInputStartLineNumber.getValue (),
//     endInputLineNumber = fInputEndLineNumber.getValue ();
//
//   if (startInputLineNumber == endInputLineNumber) {
//     ss << "line " << startInputLineNumber;
//   }
//   else {
//     ss << "lines " << startInputLineNumber << ".." << endInputLineNumber;
//   }
//
//   return ss.str ();
// }
//
// void mfInputLocation::print (std::ostream& os) const
// {
//   os << asString ();
// }
//
// EXP std::ostream& operator << (std::ostream& os, const mfInputLocation& elt)
// {
//   elt.print (os);
//   return os;
// }
//
// //______________________________________________________________________________
// mfStaffNumber::mfStaffNumber (int value)
// {
//   fValue = value;
// }
//
// mfStaffNumber::~mfStaffNumber ()
// {}
//
// mfStaffNumber::operator int () const
// {
//   return fValue;
// }
//
// EXP std::ostream& operator << (std::ostream& os, const mfStaffNumber& elt)
// {
//   os << elt.getValue ();
//   return os;
// }
//
// //______________________________________________________________________________
// mfVoiceNumber::mfVoiceNumber (int value)
// {
//   fValue = value;
// }
//
// mfVoiceNumber::~mfVoiceNumber ()
// {}
//
// mfVoiceNumber::operator int () const
// {
//   return fValue;
// }
//
// EXP std::ostream& operator << (std::ostream& os, const mfVoiceNumber& elt)
// {
//   os << elt.getValue ();
//   return os;
// }
//
// //______________________________________________________________________________
// mfMeasureNumber::mfMeasureNumber (const std::string& value)
// {
//   fValue = value;
// }
//
// mfMeasureNumber::~mfMeasureNumber ()
// {}
//
// mfMeasureNumber::operator std::string () const
// {
//   return fValue;
// }
//
// EXP std::ostream& operator << (std::ostream& os, const mfMeasureNumber& elt)
// {
//   os << elt.getValue ();
//   return os;
// }


}

