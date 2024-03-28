/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "languageItalian.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_languageItalian languageItalian::create ()
{
  languageItalian* obj =
    new languageItalian ();
  assert (obj != nullptr);
  return obj;
}

languageItalian::languageItalian ()
{}

languageItalian::~languageItalian ()
{}

std::string languageItalian::asString () const
{
  std::stringstream ss;

  ss <<
    "MusicFormats warning and error handler for italian";

  return ss.str ();
}

void languageItalian::print (std::ostream& os) const
{
  os <<
    "languageItalian" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_languageItalian& elt)
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
