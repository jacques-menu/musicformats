/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"



#include "msrRepeatCodas.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrRepeatCoda msrRepeatCoda::create (
  int                 inputLineNumber,
  S_msrSegment        repeatCodaSegment,
  S_msrRepeat         repeatUpLink)
{
  msrRepeatCoda* o =
    new msrRepeatCoda (
      inputLineNumber,
      repeatCodaSegment,
      repeatUpLink);
  assert (o != nullptr);
  return o;
}

msrRepeatCoda::msrRepeatCoda (
  int                 inputLineNumber,
  S_msrSegment        repeatCodaSegment,
  S_msrRepeat         repeatUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    repeatCodaSegment != nullptr,
    "repeatCodaSegment is null");

  fRepeatCodaSegment = repeatCodaSegment;

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    repeatUpLink != nullptr,
    "repeatUpLink is null");

  fRepeatCodaRepeatUpLink = repeatUpLink;
}

msrRepeatCoda::~msrRepeatCoda ()
{}

S_msrRepeatCoda msrRepeatCoda::createRepeatCodaNewbornClone (
  S_msrRepeat containingRepeat)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingRepeat != nullptr,
    "containingRepeat is null");

  S_msrRepeatCoda
    newbornClone = 0; // JMI
    /*
      msrRepeatCoda::create (
        fInputLineNumber,
        containingRepeat->
          getRepeatCoda ()->
            getRepeatCodaSegment (), // JMI
        containingRepeat);
      */

  // segment

  // upLinks

  return newbornClone;
}

S_msrRepeatCoda msrRepeatCoda::createRepeatCodaDeepClone (
  S_msrRepeat containingRepeat)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingRepeat != nullptr,
    "containingRepeat is null");

  S_msrRepeatCoda
    repeatCodaDeepClone = 0; // JMI
    /* JMI
      msrRepeatCoda::create (
        fInputLineNumber,
        containingRepeat->
          getRepeatCoda ()->
            getRepeatCodaSegment (), // JMI
        containingRepeat);
    */

  // segment
  repeatCodaDeepClone->fRepeatCodaSegment =
    fRepeatCodaSegment->
      createSegmentDeepClone (
        fRepeatCodaRepeatUpLink->
          getRepeatVoiceUpLink ());

  // upLinks
  repeatCodaDeepClone->fRepeatCodaRepeatUpLink =
    containingRepeat;

  return repeatCodaDeepClone;
}

/* JMI
void msrRepeatCoda::appendElementToRepeatCoda (
  S_msrElement elem) // JMI ???
{
  fRepeatCodaSegment->
    appendOtherElementToSegment (elem);
}
*/

void msrRepeatCoda::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrRepeatCoda::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrRepeatCoda>*
    p =
      dynamic_cast<visitor<S_msrRepeatCoda>*> (v)) {
        S_msrRepeatCoda elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrRepeatCoda::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRepeatCoda::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrRepeatCoda::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRepeatCoda>*
    p =
      dynamic_cast<visitor<S_msrRepeatCoda>*> (v)) {
        S_msrRepeatCoda elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrRepeatCoda::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRepeatCoda::browseData (basevisitor* v)
{
  // browse the segment
  msrBrowser<msrSegment> browser (v);
  browser.browse (*fRepeatCodaSegment);
}

string msrRepeatCoda::asString () const
{
  stringstream s;

  s <<
    "RepeatCoda" <<
    ", line " << fInputLineNumber <<
    endl;

  return s.str ();
}

void msrRepeatCoda::print (ostream& os) const
{
  os <<
    endl <<
    '[' <<
    asString () <<
    endl;

  ++gIndenter;

  os <<
    fRepeatCodaSegment;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrRepeatCoda& elt)
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
