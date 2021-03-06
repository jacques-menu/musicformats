/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include "msrTupletElements.h"

// JMI #include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
msrTupletElement::msrTupletElement (
  int inputLineNumber)
    : msrMeasureElement (inputLineNumber)
{
  fPositionInTuplet = -1;
}

msrTupletElement::~msrTupletElement ()
{}

ostream& operator<< (ostream& os, const S_msrTupletElement& elt)
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
