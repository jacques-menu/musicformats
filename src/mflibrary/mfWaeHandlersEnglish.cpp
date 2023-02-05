/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfWaeHandlersEnglish.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_mfWaeHandlerEnglish mfWaeHandlerEnglish::create ()
{
  mfWaeHandlerEnglish* o =
    new mfWaeHandlerEnglish ();
  assert (o != nullptr);
  return o;
}

mfWaeHandlerEnglish::mfWaeHandlerEnglish ()
{}

mfWaeHandlerEnglish::~mfWaeHandlerEnglish ()
{}

std::string mfWaeHandlerEnglish::asString () const
{
  std::stringstream ss;

  ss <<
    "MusicFormats warning and error handler for english";

  return ss.str ();
}

void mfWaeHandlerEnglish::print (std::ostream& os) const
{
  os <<
    "mfWaeHandlerEnglish" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mfWaeHandlerEnglish& elt)
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
