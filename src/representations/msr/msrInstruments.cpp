/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include "mfStaticSettings.h"

#include "visitor.h"

#include "msrWae.h"

#include "mfStaticSettings.h"

#include "mfAssert.h"
#include "mfServices.h"

#include "msrMeasureConstants.h"

#include "msrInstruments.h"

#include "oahOah.h"
#include "oahEarlyOptions.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrStringTuning msrStringTuning::create (
  int                  inputLineNumber,
  int                  stringTuningNumber,
  msrDiatonicPitchKind stringTuningDiatonicPitchKind,
  msrAlterationKind    stringTuningAlterationKind,
  msrOctaveKind        stringTuningOctaveKind)
{
  msrStringTuning* o =
    new msrStringTuning (
      inputLineNumber,
      stringTuningNumber,
      stringTuningDiatonicPitchKind,
      stringTuningAlterationKind,
      stringTuningOctaveKind);
  assert (o != nullptr);
  return o;
}

msrStringTuning::msrStringTuning (
  int                  inputLineNumber,
  int                  stringTuningNumber,
  msrDiatonicPitchKind stringTuningDiatonicPitchKind,
  msrAlterationKind    stringTuningAlterationKind,
  msrOctaveKind        stringTuningOctaveKind)
    : msrMeasureElement (
        inputLineNumber)
{
  fStringTuningNumber = stringTuningNumber;

  fStringTuningDiatonicPitchKind = stringTuningDiatonicPitchKind;
  fStringTuningAlterationKind    = stringTuningAlterationKind;
  fStringTuningOctaveKind        = stringTuningOctaveKind;
}

msrStringTuning::~msrStringTuning ()
{}

void msrStringTuning::setStringTuningUpLinkToMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of string tuning " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fStringTuningUpLinkToMeasure = measure;
}

void msrStringTuning::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> msrStringTuning::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }

  if (visitor<S_msrStringTuning>*
    p =
      dynamic_cast<visitor<S_msrStringTuning>*> (v)) {
        S_msrStringTuning elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrStringTuning::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
        p->visitStart (elem);
  }
}

void msrStringTuning::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> msrStringTuning::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }

  if (visitor<S_msrStringTuning>*
    p =
      dynamic_cast<visitor<S_msrStringTuning>*> (v)) {
        S_msrStringTuning elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrStringTuning::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
        p->visitEnd (elem);
  }
}

void msrStringTuning::browseData (basevisitor* v)
{}

void msrStringTuning::print (std::ostream& os) const
{
  ++gIndenter;

  const int fieldWidth = 30;

  os << std::left <<
    std::setw (fieldWidth) <<
    "stringTuningNumber" << ": " << fStringTuningNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "stringTuningDiatonicPitchKind" << ": " <<
    msrDiatonicPitchKindAsString (
      fStringTuningDiatonicPitchKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "stringTuningAlterationKind" << ": " <<
    msrAlterationKindAsString (
      fStringTuningAlterationKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "stringTuningOctaveKind" << ": " <<
    msrOctaveKindAsString (fStringTuningOctaveKind) <<
    std::endl << std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msrStringTuning& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrScordatura msrScordatura::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
{
  msrScordatura* o =
    new msrScordatura (
      inputLineNumber,
      upLinkToMeasure);
  assert (o != nullptr);
  return o;
}

S_msrScordatura msrScordatura::create (
  int                 inputLineNumber)
{
  return
    msrScordatura::create (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer); // set later in setMeasureElementUpLinkToMeasure()
}

msrScordatura::msrScordatura (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber)
{}

msrScordatura::~msrScordatura ()
{}

void msrScordatura::setScordaturaUpLinkToMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of scordatura " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fScordaturaUpLinkToMeasure = measure;
}

void msrScordatura::addStringTuningToScordatura (
  const S_msrStringTuning& stringTuning)
{
  fScordaturaStringTuningsList.push_back (
    stringTuning);
}

void msrScordatura::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> msrScordatura::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }

  if (visitor<S_msrScordatura>*
    p =
      dynamic_cast<visitor<S_msrScordatura>*> (v)) {
        S_msrScordatura elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrScordatura::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
        p->visitStart (elem);
  }
}

void msrScordatura::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> msrScordatura::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }

  if (visitor<S_msrScordatura>*
    p =
      dynamic_cast<visitor<S_msrScordatura>*> (v)) {
        S_msrScordatura elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrScordatura::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
        p->visitEnd (elem);
  }
}

void msrScordatura::browseData (basevisitor* v)
{}

void msrScordatura::print (std::ostream& os) const
{
  ++gIndenter;

  os <<
    "Scordatura";

  if (fScordaturaStringTuningsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrStringTuning>::const_iterator
      iBegin = fScordaturaStringTuningsList.begin (),
      iEnd   = fScordaturaStringTuningsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
      " : no string tunings" <<
      std::endl;
  }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msrScordatura& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrAccordionRegistration msrAccordionRegistration::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  int           highDotsNumber,
  int           middleDotsNumber,
  int           lowDotsNumber)
{
  msrAccordionRegistration* o =
    new msrAccordionRegistration (
      inputLineNumber,
      upLinkToMeasure,
      highDotsNumber, middleDotsNumber, lowDotsNumber);
  assert (o != nullptr);
  return o;
}

S_msrAccordionRegistration msrAccordionRegistration::create (
  int           inputLineNumber,
  int           highDotsNumber,
  int           middleDotsNumber,
  int           lowDotsNumber)
{
  return
    msrAccordionRegistration::create (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      highDotsNumber,
      middleDotsNumber,
      lowDotsNumber);
}

msrAccordionRegistration::msrAccordionRegistration (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  int           highDotsNumber,
  int           middleDotsNumber,
  int           lowDotsNumber)
    : msrMeasureElement (
        inputLineNumber)
{
  fHighDotsNumber   = highDotsNumber;
  fMiddleDotsNumber = middleDotsNumber;
  fLowDotsNumber    = lowDotsNumber;
}

msrAccordionRegistration::~msrAccordionRegistration ()
{}

void msrAccordionRegistration::setAccordionRegistrationUpLinkToMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of accordion registration " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fAccordionRegistrationUpLinkToMeasure = measure;
}

void msrAccordionRegistration::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> msrAccordionRegistration::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }

  if (visitor<S_msrAccordionRegistration>*
    p =
      dynamic_cast<visitor<S_msrAccordionRegistration>*> (v)) {
        S_msrAccordionRegistration elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrAccordionRegistration::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
        p->visitStart (elem);
  }
}

void msrAccordionRegistration::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> msrAccordionRegistration::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }

  if (visitor<S_msrAccordionRegistration>*
    p =
      dynamic_cast<visitor<S_msrAccordionRegistration>*> (v)) {
        S_msrAccordionRegistration elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrAccordionRegistration::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
        p->visitEnd (elem);
  }
}

void msrAccordionRegistration::browseData (basevisitor* v)
{}

std::string msrAccordionRegistration::asString () const
{
  std::stringstream ss;

  ss <<
    "AccordionRegistration" <<
    ", highDotsNumber: " << fHighDotsNumber <<
    ", middleDotsNumber: " << fMiddleDotsNumber <<
    ", lowDotsNumber: " << fLowDotsNumber <<
    ", line " << fInputLineNumber;

  return ss.str ();
}

void msrAccordionRegistration::print (std::ostream& os) const
{
  ++gIndenter;

  os << asString () << std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msrAccordionRegistration& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrHarpPedalsTuning msrHarpPedalsTuning::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
{
  msrHarpPedalsTuning* o =
    new msrHarpPedalsTuning (
      inputLineNumber,
      upLinkToMeasure);
  assert (o != nullptr);
  return o;
}

S_msrHarpPedalsTuning msrHarpPedalsTuning::create (
  int                 inputLineNumber)
{
  return
    msrHarpPedalsTuning::create (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer); // set later in setMeasureElementUpLinkToMeasure()
}

msrHarpPedalsTuning::msrHarpPedalsTuning (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber)
{}

msrHarpPedalsTuning::~msrHarpPedalsTuning ()
{}

S_msrHarpPedalsTuning msrHarpPedalsTuning::createHarpPedalsTuningNewbornClone ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
		std::stringstream ss;

    ss <<
      "Creating a newborn clone of a " <<
      asString () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  S_msrHarpPedalsTuning
    newbornClone = 0; // JMI

  return newbornClone;
}

S_msrHarpPedalsTuning msrHarpPedalsTuning::createHarpPedalsTuningDeepClone ()
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
		std::stringstream ss;

    ss <<
      "Creating a newborn clone of a " <<
      asString () <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif

  S_msrHarpPedalsTuning
    harpPedalsTuningDeepClone = nullptr; // JMI v0.9.66

  return harpPedalsTuningDeepClone;
}

void msrHarpPedalsTuning::setHarpPedalsTuningUpLinkToMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of harp pedals tuning " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fHarpPedalsTuningUpLinkToMeasure = measure;
}

void msrHarpPedalsTuning::addPedalTuning (
  int                  inputLineNumber,
  msrDiatonicPitchKind diatonicPitchKind,
  msrAlterationKind    alterationKind)
{
  // is diatonicPitch in the part renaming map?
  std::map<msrDiatonicPitchKind, msrAlterationKind>::const_iterator
    it =
      fHarpPedalsAlterationKindsMap.find (diatonicPitchKind);

  if (it != fHarpPedalsAlterationKindsMap.end ()) {
    std::stringstream ss;

    ss <<
      "pedal tuning '" <<
      msrDiatonicPitchKindAsString (
        diatonicPitchKind) <<
      msrAlterationKindAsString (
        alterationKind) <<
      "' has already been specified";

    msrError (
      gGlobalCurrentServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  fHarpPedalsAlterationKindsMap [diatonicPitchKind] = alterationKind;
}

void msrHarpPedalsTuning::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> msrHarpPedalsTuning::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }

  if (visitor<S_msrHarpPedalsTuning>*
    p =
      dynamic_cast<visitor<S_msrHarpPedalsTuning>*> (v)) {
        S_msrHarpPedalsTuning elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrHarpPedalsTuning::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
        p->visitStart (elem);
  }
}

void msrHarpPedalsTuning::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> msrHarpPedalsTuning::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }

  if (visitor<S_msrHarpPedalsTuning>*
    p =
      dynamic_cast<visitor<S_msrHarpPedalsTuning>*> (v)) {
        S_msrHarpPedalsTuning elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrHarpPedalsTuning::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
        p->visitEnd (elem);
  }
}

void msrHarpPedalsTuning::browseData (basevisitor* v)
{}

std::string msrHarpPedalsTuning::asString () const
{
  std::stringstream ss;

  ss <<
    "[HarpPedalsTuning" <<
    ", line " << fInputLineNumber <<
    ", ";

  if (fHarpPedalsAlterationKindsMap.size ()) {
    ++gIndenter;

    std::map<msrDiatonicPitchKind, msrAlterationKind>::const_iterator
      iBegin = fHarpPedalsAlterationKindsMap.begin (),
      iEnd   = fHarpPedalsAlterationKindsMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the pedal and its alteration
      ss <<
        msrDiatonicPitchKindAsString (
          (*i).first) <<
        ' ' <<
        msrAlterationKindAsString (
          (*i).second);
      if (++i == iEnd) break;
      ss << ", ";
    } // for

    --gIndenter;
  }

  else {
    ss <<
      "[EMPTY]";
  }

  return ss.str ();
}

void msrHarpPedalsTuning::print (std::ostream& os) const
{
  os <<
    "[HarpPedalsTuning" <<
    ", line " << fInputLineNumber;

  if (fHarpPedalsAlterationKindsMap.size ()) {
    ++gIndenter;

    os << std::endl;

    std::map<msrDiatonicPitchKind, msrAlterationKind>::const_iterator
      iBegin = fHarpPedalsAlterationKindsMap.begin (),
      iEnd   = fHarpPedalsAlterationKindsMap.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the pedal and its alteration
      os <<
        msrDiatonicPitchKindAsString (
          (*i).first) <<
        ' ' <<
        msrAlterationKindAsString (
          (*i).second);
      if (++i == iEnd) break;
      os << ", ";
    } // for

    --gIndenter;
  }

  else {
    os <<
      " empty" <<
      std::endl;
  }

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrHarpPedalsTuning& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrPedal msrPedal::create (
  int              inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrPedalTypeKind pedalTypeKind,
  msrPedalLineKind pedalLineKind,
  msrPedalSignKind pedalSignKind)
{
  msrPedal* o =
    new msrPedal (
      inputLineNumber,
      upLinkToMeasure,
      pedalTypeKind,
      pedalLineKind,
      pedalSignKind);
  assert (o != nullptr);
  return o;
}

S_msrPedal msrPedal::create (
  int              inputLineNumber,
  msrPedalTypeKind pedalTypeKind,
  msrPedalLineKind pedalLineKind,
  msrPedalSignKind pedalSignKind)
{
  return
    msrPedal::create (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      pedalTypeKind,
      pedalLineKind,
      pedalSignKind);
}

msrPedal::msrPedal (
  int              inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrPedalTypeKind pedalTypeKind,
  msrPedalLineKind pedalLineKind,
  msrPedalSignKind pedalSignKind)
    : msrMeasureElement (
        inputLineNumber)
{
  fPedalTypeKind = pedalTypeKind;
  fPedalLineKind = pedalLineKind;
  fPedalSignKind = pedalSignKind;
}

msrPedal::~msrPedal ()
{}

void msrPedal::setPedalUpLinkToMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of pedal " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fPedalUpLinkToMeasure = measure;
}

void msrPedal::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> msrPedal::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }

  if (visitor<S_msrPedal>*
    p =
      dynamic_cast<visitor<S_msrPedal>*> (v)) {
        S_msrPedal elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrPedal::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
        p->visitStart (elem);
  }
}

void msrPedal::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> msrPedal::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }

  if (visitor<S_msrPedal>*
    p =
      dynamic_cast<visitor<S_msrPedal>*> (v)) {
        S_msrPedal elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrPedal::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
        p->visitEnd (elem);
  }
}

void msrPedal::browseData (basevisitor* v)
{}

std::string msrPedalTypeKindAsString (
  msrPedalTypeKind pedalTypeKind)
{
  std::string result;

  switch (pedalTypeKind) {
    case msrPedalTypeKind::kPedalType_UNKNOWN:
      result = "kPedalType_UNKNOWN";
      break;
    case msrPedalTypeKind::kPedalTypeStart:
      result = "kPedalTypeStart";
      break;
    case msrPedalTypeKind::kPedalTypeContinue:
      result = "kPedalTypeContinue";
      break;
    case msrPedalTypeKind::kPedalTypeChange:
      result = "kPedalTypeChange";
      break;
    case msrPedalTypeKind::kPedalTypeStop:
      result = "kPedalTypeStop";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrPedalTypeKind& elt)
{
  os << msrPedalTypeKindAsString (elt);
  return os;
}

std::string msrPedalLineKindAsString (
  msrPedalLineKind pedalLineKind)
{
  std::string result;

  switch (pedalLineKind) {
    case msrPedalLineKind::kPedalLineYes:
      result = "kPedalLineYes";
      break;
    case msrPedalLineKind::kPedalLineNo:
      result = "kPedalLineNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrPedalLineKind& elt)
{
  os << msrPedalLineKindAsString (elt);
  return os;
}

std::string msrPedalSignKindAsString (
  msrPedalSignKind pedalSignKind)
{
  std::string result;

  switch (pedalSignKind) {
    case msrPedalSignKind::kPedalSignYes:
      result = "kPedalSignYes";
      break;
    case msrPedalSignKind::kPedalSignNo:
      result = "kPedalSignNo";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrPedalSignKind& elt)
{
  os << msrPedalSignKindAsString (elt);
  return os;
}

void msrPedal::print (std::ostream& os) const
{
  os <<
    "[Pedal" <<
    ", fPedalTypeKind: " << fPedalTypeKind <<
    ", fPedalLineKind: " << fPedalLineKind <<
    ", fPedalSignKind: " << fPedalSignKind <<
    ", line " << fInputLineNumber <<
    ']' <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrPedal& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrDamp msrDamp::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
{
  msrDamp* o =
    new msrDamp (
      inputLineNumber,
      upLinkToMeasure);
  assert (o != nullptr);
  return o;
}

S_msrDamp msrDamp::create (
  int                 inputLineNumber)
{
  return
    msrDamp::create (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer); // set later in setMeasureElementUpLinkToMeasure()
}

msrDamp::msrDamp (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber)
{}

msrDamp::~msrDamp ()
{}

void msrDamp::setDampUpLinkToMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of damp " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fDampUpLinkToMeasure = measure;
}

void msrDamp::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> msrDamp::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }

  if (visitor<S_msrDamp>*
    p =
      dynamic_cast<visitor<S_msrDamp>*> (v)) {
        S_msrDamp elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrDamp::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
        p->visitStart (elem);
  }
}

void msrDamp::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> msrDamp::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }

  if (visitor<S_msrDamp>*
    p =
      dynamic_cast<visitor<S_msrDamp>*> (v)) {
        S_msrDamp elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrDamp::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
        p->visitEnd (elem);
  }
}

void msrDamp::browseData (basevisitor* v)
{}

void msrDamp::print (std::ostream& os) const
{
  os <<
    "Damp" <<
    ", line " << fInputLineNumber <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrDamp& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrDampAll msrDampAll::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
{
  msrDampAll* o =
    new msrDampAll (
      inputLineNumber,
      upLinkToMeasure);
  assert (o != nullptr);
  return o;
}

S_msrDampAll msrDampAll::create (
  int                 inputLineNumber)
{
  return
    msrDampAll::create (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer); // set later in setMeasureElementUpLinkToMeasure()
}

msrDampAll::msrDampAll (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber)
{}

msrDampAll::~msrDampAll ()
{}

void msrDampAll::setDampAllUpLinkToMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");
#endif

#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of dampAll " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fDampAllUpLinkToMeasure = measure;
}

void msrDampAll::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> msrDampAll::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }

  if (visitor<S_msrDampAll>*
    p =
      dynamic_cast<visitor<S_msrDampAll>*> (v)) {
        S_msrDampAll elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrDampAll::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
        p->visitStart (elem);
  }
}

void msrDampAll::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> msrDampAll::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }

  if (visitor<S_msrDampAll>*
    p =
      dynamic_cast<visitor<S_msrDampAll>*> (v)) {
        S_msrDampAll elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrDampAll::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
        p->visitEnd (elem);
  }
}

void msrDampAll::browseData (basevisitor* v)
{}

void msrDampAll::print (std::ostream& os) const
{
  os <<
    "DampAll" <<
    ", line " << fInputLineNumber <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrDampAll& elt)
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
