/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfWaeHandlersSpanish.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_mfWaeHandlerSpanish mfWaeHandlerSpanish::create ()
{
  mfWaeHandlerSpanish* o =
    new mfWaeHandlerSpanish ();
  assert (o != nullptr);
  return o;
}

mfWaeHandlerSpanish::mfWaeHandlerSpanish ()
{}

mfWaeHandlerSpanish::~mfWaeHandlerSpanish ()
{}

std::string mfWaeHandlerSpanish::asString () const
{
  std::stringstream ss;

  ss <<
    "MusicFormats warning and error handler for spanish";

  return ss.str ();
}

void mfWaeHandlerSpanish::print (std::ostream& os) const
{
  os <<
    "mfWaeHandlerSpanish" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mfWaeHandlerSpanish& elt)
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
