/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfWaeHandlersGerman.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_mfWaeHandlerGerman mfWaeHandlerGerman::create ()
{
  mfWaeHandlerGerman* o =
    new mfWaeHandlerGerman ();
  assert (o != nullptr);
  return o;
}

mfWaeHandlerGerman::mfWaeHandlerGerman ()
{}

mfWaeHandlerGerman::~mfWaeHandlerGerman ()
{}

std::string mfWaeHandlerGerman::asString () const
{
  std::stringstream s;

  s <<
    "MusicFormats warning and error handler for german";

  return s.str ();
}

void mfWaeHandlerGerman::print (std::ostream& os) const
{
  os <<
    "mfWaeHandlerGerman" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mfWaeHandlerGerman& elt)
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
