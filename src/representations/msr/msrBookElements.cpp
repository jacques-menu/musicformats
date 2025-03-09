/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msrBookElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
msrBookElement::msrBookElement (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrBookElement::~msrBookElement ()
{}

std::ostream& operator << (std::ostream& os, const S_msrBookElement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }
  
  return os;
}


}
