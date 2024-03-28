/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "languageSpanish.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_languageSpanish languageSpanish::create ()
{
  languageSpanish* obj =
    new languageSpanish ();
  assert (obj != nullptr);
  return obj;
}

languageSpanish::languageSpanish ()
{}

languageSpanish::~languageSpanish ()
{}

std::string languageSpanish::asString () const
{
  std::stringstream ss;

  ss <<
    "MusicFormats warning and error handler for spanish";

  return ss.str ();
}

void languageSpanish::print (std::ostream& os) const
{
  os <<
    "languageSpanish" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_languageSpanish& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}


} // namespace
