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

#include "msrMeasureElements.h"



#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
// constants
const string   msrMeasureElement::K_NO_MEASURE_NUMBER = "K_NO_MEASURE_NUMBER";

const rational msrMeasureElement::K_NO_WHOLE_NOTES (-444444, 1);

msrMeasureElement::msrMeasureElement (
  int inputLineNumber)
    : msrElement (inputLineNumber),
      fMeasureElementMomentInMeasure (
        msrMoment::K_NO_POSITION, msrMoment::K_NO_POSITION),
      fMeasureElementMomentInVoice (
        msrMoment::K_NO_POSITION, msrMoment::K_NO_POSITION)
{
  fMeasureElementSoundingWholeNotes = rational (0, 1);

  fMeasureElementMeasureNumber = K_NO_MEASURE_NUMBER;

  fMeasureElementPositionInMeasure = msrMoment::K_NO_POSITION;
  fMeasureElementPositionInVoice   = msrMoment::K_NO_POSITION;
}

msrMeasureElement::~msrMeasureElement ()
{}

void msrMeasureElement::setMeasureElementSoundingWholeNotes (
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
      fMeasureElementMeasureNumber <<
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

  fMeasureElementSoundingWholeNotes = wholeNotes;
}

void msrMeasureElement::setMeasureElementPositionInMeasure (
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
      fMeasureElementPositionInMeasure <<
      "') in measure '" <<
      fMeasureElementMeasureNumber <<
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

  fMeasureElementPositionInMeasure = positionInMeasure;
}

void msrMeasureElement::setMeasureElementPositionInVoice (
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
      fMeasureElementMeasureNumber <<
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

  fMeasureElementPositionInVoice = positionInVoice;
}

void msrMeasureElement::setMeasureElementMomentInMeasure (
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
      fMeasureElementMomentInMeasure.asString () <<
      "') in measure '" <<
      fMeasureElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  fMeasureElementMomentInMeasure = momentInMeasure;
}

void msrMeasureElement::setMeasureElementMomentInVoice (
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
      fMeasureElementMeasureNumber <<
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

  fMeasureElementMomentInVoice = momentInVoice;
}

void msrMeasureElement::assignMeasureElementPositionInVoice (
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
      fMeasureElementMeasureNumber <<
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
      fMeasureElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  fMeasureElementPositionInVoice = positionInVoice;

  // account for it in positionInVoice
  positionInVoice +=
    fMeasureElementSoundingWholeNotes;
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

bool msrMeasureElement::compareMeasureElementsByIncreasingPositionInMeasure (
  const SMARTP<msrMeasureElement>& first,
  const SMARTP<msrMeasureElement>& second)
{
  return
    first->getMeasureElementPositionInMeasure ()
      <
    second->getMeasureElementPositionInMeasure ();
}

void msrMeasureElement::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureElement::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrMeasureElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureElement>*> (v)) {
        S_msrMeasureElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureElement::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrMeasureElement::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureElement::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrMeasureElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureElement>*> (v)) {
        S_msrMeasureElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureElement::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

string msrMeasureElement::asString () const
{
  // this is overriden all in actual elements
  return "??? msrMeasureElement::asString () ???";
}

string msrMeasureElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

void msrMeasureElement::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_msrMeasureElement& elt)
{
  elt->print (os);
  return os;
}


}
