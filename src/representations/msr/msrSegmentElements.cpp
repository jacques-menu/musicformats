/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <climits>      // INT_MIN, INT_MAX

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"

#include "msrSegmentElements.h"



#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
// constants
const string   msrSegmentElement::K_NO_MEASURE_NUMBER = "K_NO_MEASURE_NUMBER";

const rational msrSegmentElement::K_NO_WHOLE_NOTES (-444444, 1);

msrSegmentElement::msrSegmentElement (
  int inputLineNumber)
    : msrElement (inputLineNumber),
      fSegmentElementMomentInMeasure (
        msrMoment::K_NO_POSITION, msrMoment::K_NO_POSITION),
      fSegmentElementMomentInVoice (
        msrMoment::K_NO_POSITION, msrMoment::K_NO_POSITION)
{
  fSegmentElementSoundingWholeNotes = rational (0, 1);

  fSegmentElementMeasureNumber = K_NO_MEASURE_NUMBER;

  fSegmentElementPositionInMeasure = msrMoment::K_NO_POSITION;
  fSegmentElementPositionInVoice   = msrMoment::K_NO_POSITION;
}

msrSegmentElement::~msrSegmentElement ()
{}

void msrSegmentElement::setSegmentElementSoundingWholeNotes (
  const rational& wholeNotes,
  const string&   context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting measure element sounding whole notes of " <<
      asString () <<
      " to '" << wholeNotes <<
      "' in measure '" <<
      fSegmentElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    wholeNotes != K_NO_WHOLE_NOTES,
    "wholeNotes == K_NO_WHOLE_NOTES");

  fSegmentElementSoundingWholeNotes = wholeNotes;
}

void msrSegmentElement::setSegmentElementPositionInMeasure (
  const rational& positionInMeasure,
  const string&   context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting measure element position in measure of " <<
      asString () <<
      " to '" << positionInMeasure <<
      "' (was '" <<
      fSegmentElementPositionInMeasure <<
      "') in measure '" <<
      fSegmentElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif
/* JMI
  if (positionInMeasure == msrMoment::K_NO_POSITION) abort ();

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    positionInMeasure != msrMoment::K_NO_POSITION,
    "positionInMeasure == msrMoment::K_NO_POSITION");
*/

  fSegmentElementPositionInMeasure = positionInMeasure;
}

void msrSegmentElement::setSegmentElementPositionInVoice (
  const rational& positionInVoice,
  const string&   context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting measure element position in voice of " <<
      asString () <<
      " to '" << positionInVoice <<
      "' in measure '" <<
      fSegmentElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    positionInVoice != msrMoment::K_NO_POSITION,
    "positionInVoice == msrMoment::K_NO_POSITION");

  fSegmentElementPositionInVoice = positionInVoice;
}

void msrSegmentElement::setSegmentElementMomentInMeasure (
  const msrMoment& momentInMeasure,
  const string&    context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMomentsInMeasures ()) {
    gLogStream <<
      "Setting measure element moment in measure of " <<
      asString () <<
      " to '" << momentInMeasure <<
      "' (was '" <<
      fSegmentElementMomentInMeasure.asString () <<
      "') in measure '" <<
      fSegmentElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  fSegmentElementMomentInMeasure = momentInMeasure;
}

void msrSegmentElement::setSegmentElementMomentInVoice (
  const msrMoment& momentInVoice,
  const string&    context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMomentsInMeasures ()) {
    gLogStream <<
      "Setting measure element moment in voice of " <<
      asString () <<
      " to '" << momentInVoice <<
      "' in measure '" <<
      fSegmentElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    momentInVoice != msrMoment::K_NO_MOMENT,
    "momentInVoice == msrMoment::K_NO_MOMENT");

  fSegmentElementMomentInVoice = momentInVoice;
}

void msrSegmentElement::assignSegmentElementPositionInVoice (
  rational&     positionInVoice,
  const string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Assigning measure element position in voice of " <<
      asString () <<
      " to '" << positionInVoice <<
      "' in measure '" <<
      fSegmentElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    positionInVoice != msrMoment::K_NO_POSITION,
    "positionInVoice == msrMoment::K_NO_POSITION");

  // set measure element position in voice
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting measure element position in voice of " <<
      asString () <<
      " to '" << positionInVoice <<
      "' in measure '" <<
      fSegmentElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  fSegmentElementPositionInVoice = positionInVoice;

  // account for it in positionInVoice
  positionInVoice +=
    fSegmentElementSoundingWholeNotes;
  positionInVoice.rationalise ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Position in voice becomes " <<
      positionInVoice <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif
}

bool msrSegmentElement::compareSegmentElementsByIncreasingPositionInMeasure (
  const SMARTP<msrSegmentElement>& first,
  const SMARTP<msrSegmentElement>& second)
{
  return
    first->getSegmentElementPositionInMeasure ()
      <
    second->getSegmentElementPositionInMeasure ();
}

void msrSegmentElement::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSegmentElement::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrSegmentElement>*
    p =
      dynamic_cast<visitor<S_msrSegmentElement>*> (v)) {
        S_msrSegmentElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSegmentElement::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrSegmentElement::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSegmentElement::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrSegmentElement>*
    p =
      dynamic_cast<visitor<S_msrSegmentElement>*> (v)) {
        S_msrSegmentElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSegmentElement::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

string msrSegmentElement::asString () const
{
  // this is overriden all in actual elements
  return "??? msrSegmentElement::asString () ???";
}

string msrSegmentElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

void msrSegmentElement::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_msrSegmentElement& elt)
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
