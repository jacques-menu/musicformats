/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "languageEnglish.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_languageEnglish languageEnglish::create ()
{
  languageEnglish* obj =
    new languageEnglish ();
  assert (obj != nullptr);
  return obj;
}

languageEnglish::languageEnglish ()
{}

languageEnglish::~languageEnglish ()
{}

std::string languageEnglish::asString () const
{
  std::stringstream ss;

  ss <<
    "MusicFormats warning and error handler for english";

  return ss.str ();
}

void languageEnglish::print (std::ostream& os) const
{
  os <<
    "languageEnglish" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_languageEnglish& elt)
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
