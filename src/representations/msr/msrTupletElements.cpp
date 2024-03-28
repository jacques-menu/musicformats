/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include "mfAssert.h"

#include "msrTypesForwardDeclarations.h"

#include "msrMeasureElements.h"
#include "msrMeasures.h"
#include "msrTupletElementsAndTuplets.h"


namespace MusicFormats
{

//______________________________________________________________________________
msrTupletElement::msrTupletElement (
  int inputLineNumber)
    : msrMeasureElement (
        inputLineNumber)
{
  fPositionInTuplet = -1;
}
// msrTupletElement::msrTupletElement (
//   int                 inputLineNumber,
//   const S_msrMeasure& upLinkToMeasure)
//     : msrMeasureElement (
//         inputLineNumber)
// {
//   fPositionInTuplet = -1;
// }

msrTupletElement::~msrTupletElement ()
{}

std::ostream& operator << (std::ostream& os, const S_msrTupletElement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const msrTupletElement& elt)
{
  os << "msrNoteHeadKindAsString (elt)" << std::endl; // v0.9.66
  return os;
}


}
