/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msrVoiceElements.h"

#include "oahEarlyOptions.h"


namespace MusicFormats
{

//______________________________________________________________________________
msrVoiceElement::msrVoiceElement (
  const mfInputLineNumber& inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrVoiceElement::~msrVoiceElement ()
{}

std::ostream& operator << (std::ostream& os, const S_msrVoiceElement& elt)
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
