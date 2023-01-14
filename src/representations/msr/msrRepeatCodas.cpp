/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfEnableSanityChecksSetting.h"

#include "visitor.h"

#include "mfEnableTracingSetting.h"

#include "mfAssert.h"



#include "msrRepeatCodas.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrRepeatCoda msrRepeatCoda::create (
  int                 inputLineNumber,
  const S_msrSegment& repeatCodaSegment,
  const S_msrRepeat&  upLinkToRepeat)
{
  msrRepeatCoda* o =
    new msrRepeatCoda (
      inputLineNumber,
      repeatCodaSegment,
      upLinkToRepeat);
  assert (o != nullptr);
  return o;
}

msrRepeatCoda::msrRepeatCoda (
  int                 inputLineNumber,
  const S_msrSegment& repeatCodaSegment,
  const S_msrRepeat&  upLinkToRepeat)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    repeatCodaSegment != nullptr,
    "repeatCodaSegment is null");
#endif

  fRepeatCodaSegment = repeatCodaSegment;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToRepeat != nullptr,
    "upLinkToRepeat is null");
#endif

  fRepeatCodaUpLinkToRepeat = upLinkToRepeat;
}

msrRepeatCoda::~msrRepeatCoda ()
{}

S_msrRepeatCoda msrRepeatCoda::createRepeatCodaNewbornClone (
  const S_msrRepeat& containingRepeat)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a newborn clone of a " <<
      asString () <<
      std::endl;
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingRepeat != nullptr,
    "containingRepeat is null");
#endif

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
  const S_msrRepeat& containingRepeat)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a newborn clone of a " <<
      asString () <<
      std::endl;
  }
#endif

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingRepeat != nullptr,
    "containingRepeat is null");
#endif

  S_msrRepeatCoda
    deepClone = nullptr; // JMI v0.9.66
    /* JMI
      msrRepeatCoda::create (
        fInputLineNumber,
        containingRepeat->
          getRepeatCoda ()->
            getRepeatCodaSegment (), // JMI
        containingRepeat);
    */

  // segment
  deepClone->fRepeatCodaSegment =
    fRepeatCodaSegment->
      createSegmentDeepClone (
        fRepeatCodaUpLinkToRepeat->
          getRepeatUpLinkToVoice ());

  // upLinks
  deepClone->fRepeatCodaUpLinkToRepeat =
    containingRepeat;

  return deepClone;
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
      std::endl;
  }

  if (visitor<S_msrRepeatCoda>*
    p =
      dynamic_cast<visitor<S_msrRepeatCoda>*> (v)) {
        S_msrRepeatCoda elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrRepeatCoda::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrRepeatCoda::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrRepeatCoda::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrRepeatCoda>*
    p =
      dynamic_cast<visitor<S_msrRepeatCoda>*> (v)) {
        S_msrRepeatCoda elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrRepeatCoda::visitEnd ()" <<
            std::endl;
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

std::string msrRepeatCoda::asString () const
{
  std::stringstream s;

  s <<
    "RepeatCoda" <<
    ", line " << fInputLineNumber <<
    std::endl;

  return s.str ();
}

void msrRepeatCoda::print (std::ostream& os) const
{
  os <<
    std::endl <<
    '[' <<
    asString () <<
    std::endl;

  ++gIndenter;

  os <<
    fRepeatCodaSegment;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrRepeatCoda& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
