/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "msrVoiceElements.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
msrVoiceElement::msrVoiceElement (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrVoiceElement::~msrVoiceElement ()
{}

ostream& operator<< (ostream& os, const S_msrVoiceElement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}


}
