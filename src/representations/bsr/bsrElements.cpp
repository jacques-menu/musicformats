/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "bsrElements.h"

#include "oahOah.h"

#include "bsrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//_______________________________________________________________________________
/* this class is purely virtual
S_bsrElement bsrElement::create (
  const mfInputLineNumber& inputLineNumber)
{
  bsrElement* obj =
    new bsrElement (
      inputLineNumber);
  assert (obj != nullptr);
  return obj;
}
*/

bsrElement::bsrElement (
  const mfInputLineNumber& inputLineNumber)
    : msrElement (inputLineNumber)
{}

bsrElement::~bsrElement ()
{}

std::ostream& operator << (std::ostream& os, const S_bsrElement& elt)
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
