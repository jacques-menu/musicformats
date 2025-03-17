/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>

#include "mfInputLinesRange.h"
// #include "mfBasicTypes.h"


namespace MusicFormats
{

//______________________________________________________________________________
mfInputLinesRange::mfInputLinesRange (
  int inputStartLineNumber,
  int inputEndLineNumber)
{
  fInputLineNumber = inputStartLineNumber;
  fInputEndLineNumber = inputEndLineNumber;
}

mfInputLinesRange::mfInputLinesRange (
  int inputStartLineNumber)
{
  fInputLineNumber = inputStartLineNumber;
  fInputEndLineNumber = inputStartLineNumber;
}

mfInputLinesRange::~mfInputLinesRange ()
{}

std::string mfInputLinesRange::asString () const
{
  std::stringstream ss;

  if (fInputEndLineNumber != fInputLineNumber) {
    ss  <<
      "lines " <<
      fInputLineNumber <<
      " .. " <<
      fInputEndLineNumber;
  }
  else {
    ss <<
      "line " << fInputLineNumber;
  }

  return ss.str ();
}

void mfInputLinesRange::print (std::ostream& os) const
{
  os <<
    "[mfInputLinesRange " <<
    fInputLineNumber;

  if (fInputEndLineNumber != fInputLineNumber) {
    os <<
      ", " <<
      fInputEndLineNumber;
  }

  os << ']';
}

std::ostream& operator << (std::ostream& os, const mfInputLinesRange& elt) {
  elt.print (os);
  return os;
}


}

