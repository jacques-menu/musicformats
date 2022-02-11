/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include "msrPartGroupElements.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
msrPartGroupElement::msrPartGroupElement (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrPartGroupElement::~msrPartGroupElement ()
{}

ostream& operator<< (ostream& os, const S_msrPartGroupElement& elt)
{
  elt->print (os);
  return os;
}


}
