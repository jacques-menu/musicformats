/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "languageDutch.h"


namespace MusicFormats
{

//_______________________________________________________________________________
S_languageDutch languageDutch::create ()
{
  languageDutch* obj =
    new languageDutch ();
  assert (obj != nullptr);
  return obj;
}

languageDutch::languageDutch ()
{}

languageDutch::~languageDutch ()
{}

std::string languageDutch::asString () const
{
  std::stringstream ss;

  ss <<
    "MusicFormats warning and error handler for dutch";

  return ss.str ();
}

void languageDutch::print (std::ostream& os) const
{
  os <<
    "languageDutch" <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_languageDutch& elt)
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
