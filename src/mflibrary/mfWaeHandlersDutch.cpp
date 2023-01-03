/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfWaeHandlersDutch.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_mfWaeHandlerDutch mfWaeHandlerDutch::create ()
{
  mfWaeHandlerDutch* o =
    new mfWaeHandlerDutch ();
  assert (o != nullptr);
  return o;
}

mfWaeHandlerDutch::mfWaeHandlerDutch ()
{}

mfWaeHandlerDutch::~mfWaeHandlerDutch ()
{}

std::string mfWaeHandlerDutch::asString () const
{
  std::stringstream s;

  s <<
    "MusicFormats warning and error handler for dutch";

  return s.str ();
}

void mfWaeHandlerDutch::print (std::ostream& os) const
{
  os <<
    "mfWaeHandlerDutch" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mfWaeHandlerDutch& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}



} // namespace
