/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfWaeHandlersItalian.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_mfWaeHandlerItalian mfWaeHandlerItalian::create ()
{
  mfWaeHandlerItalian* o =
    new mfWaeHandlerItalian ();
  assert (o != nullptr);
  return o;
}

mfWaeHandlerItalian::mfWaeHandlerItalian ()
{}

mfWaeHandlerItalian::~mfWaeHandlerItalian ()
{}

std::string mfWaeHandlerItalian::asString () const
{
  std::stringstream s;

  s <<
    "MusicFormats warning and error handler for italian";

  return s.str ();
}

void mfWaeHandlerItalian::print (std::ostream& os) const
{
  os <<
    "mfWaeHandlerItalian" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_mfWaeHandlerItalian& elt)
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
