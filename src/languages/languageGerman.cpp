/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "languageGerman.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_languageGerman languageGerman::create ()
{
  languageGerman* obj =
    new languageGerman ();
  assert (obj != nullptr);
  return obj;
}

languageGerman::languageGerman ()
{}

languageGerman::~languageGerman ()
{}

std::string languageGerman::asString () const
{
  std::stringstream ss;

  ss <<
    "MusicFormats warning and error handler for german";

  return ss.str ();
}

void languageGerman::print (std::ostream& os) const
{
  os <<
    "languageGerman" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_languageGerman& elt)
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
