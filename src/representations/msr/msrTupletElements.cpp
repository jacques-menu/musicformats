/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include "mfAssert.h"

#include "msrTupletElements.h"


namespace MusicFormats
{

//______________________________________________________________________________
msrTupletElement::msrTupletElement (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber)
{
  fPositionInTuplet = -1;
}

// msrTupletElement::msrTupletElement ()
// {}

msrTupletElement::~msrTupletElement ()
{}

void msrTupletElement::setMeasureElementUpLinkToMeasure (
  const S_msrMeasure& measure)
{
  setTupletElementUpLinkToMeasure (measure);
}

S_msrMeasure msrTupletElement::getMeasureElementUpLinkToMeasure () const
{
  return getTupletElementUpLinkToMeasure ();
}

void msrTupletElement::setTupletElementUpLinkToMeasure (
  const S_msrMeasure& measure)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of tuplet element " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fTupletElementUpLinkToMeasure = measure;
}

S_msrMeasure msrTupletElement::getTupletElementUpLinkToMeasure () const
{
  return fTupletElementUpLinkToMeasure;
}

std::ostream& operator << (std::ostream& os, const S_msrTupletElement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const msrTupletElement& elt)
{
  os << "msrNoteHeadKindAsString (elt)" << std::endl; // v0.9.66
  return os;
}


}
