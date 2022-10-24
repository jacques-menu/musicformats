/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrRepeats.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
/*
  Repeats in MusicXML are applied to all voices in all staves of the current part

  The currentRepeat in each voice is the top of the voice repeats stack

  A repeat is recognized in MusicXML either by:

    - it's start: handleRepeatStart
    - it's first hooked ending (45b): handleRepeatEndingStart
        the elements before is moved to the new repeat's common part
    - it's end: handleRepeatEnd

  Hooked endings following the first one are added to currentRepeat handleRepeatHookedEndingEnd

  A hookless ending terminates currentRepeat: handleRepeatHooklessEndingEnd
    finalize currentRepeat
*/

//______________________________________________________________________________
S_msrRepeatCommonPart msrRepeatCommonPart::create (
  int         inputLineNumber,
  S_msrRepeat upLinkToRepeat)
{
  msrRepeatCommonPart* o =
    new msrRepeatCommonPart (
      inputLineNumber,
      upLinkToRepeat);
  assert (o != nullptr);
  return o;
}

msrRepeatCommonPart::msrRepeatCommonPart (
  int         inputLineNumber,
  S_msrRepeat upLinkToRepeat)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToRepeat != nullptr,
    "upLinkToRepeat is null");

  fRepeatCommonPartUpLinkToRepeat = upLinkToRepeat;
}

msrRepeatCommonPart::~msrRepeatCommonPart ()
{}

void msrRepeatCommonPart::appendSegmentToRepeatCommonPart (
  int           inputLineNumber,
  S_msrSegment  segment,
  const string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSegments ()) {
    gLogStream <<
      "Appending segment '" <<
      segment->asString () <<
      "' to repeat common part '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    segment != nullptr,
    "segment is null");

  fRepeatCommonPartElementsList.push_back (segment);
}

void msrRepeatCommonPart::appendRepeatToRepeatCommonPart (
  int           inputLineNumber,
  S_msrRepeat   repeat,
  const string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending repeat '" <<
    // JMI  repeat->asString () <<
      repeat <<
      "' to repeat common part '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    repeat != nullptr,
    "repeat is null");

  fRepeatCommonPartElementsList.push_back (repeat);
}

void msrRepeatCommonPart::appendMeasureRepeatToRepeatCommonPart (
  int                inputLineNumber,
  S_msrMeasureRepeat measureRepeat,
  const string&      context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasureRepeats ()) {
    gLogStream <<
      "Appending measures repeat '" <<
      measureRepeat->asString () <<
      "' to repeat common part '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeat != nullptr,
    "measureRepeat is null");

// KAKA  fRepeatCommonPartElementsList.push_back (measureRepeat);
}

void msrRepeatCommonPart::appendMultipleFullBarRestsToRepeatCommonPart (
  int                   inputLineNumber,
  S_msrMultipleFullBarRests multipleFullBarRests,
  const string&         context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMultipleFullBarRests ()) {
    gLogStream <<
      "Appending multiple full-bar rests " <<
      multipleFullBarRests->asString () <<
      " to repeat common part " << asString () <<
      " (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    multipleFullBarRests != nullptr,
    "multipleFullBarRests is null");

//  KAKA fRepeatCommonPartElementsList.push_back (multipleFullBarRests);
}

void msrRepeatCommonPart::appendVoiceElementToRepeatCommonPart (
  int               inputLineNumber,
  S_msrVoiceElement voiceElement,
  const string&     context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending voice element '" <<
      voiceElement->asString () <<
      "' to repeat common part '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voiceElement != nullptr,
    "voiceElement is null");

  fRepeatCommonPartElementsList.push_back (voiceElement);
}

S_msrNote msrRepeatCommonPart::fetchRepeatCommonPartFirstNonGraceNote () const
{

  S_msrNote result;

  // fetch the first note in the first measure to which
  // a grace notes group can be attached
  // i.e. one not in a grace notes group itself,
  // possibly inside a chord or tuplet

  if (fRepeatCommonPartElementsList.size ()) {
    list<S_msrVoiceElement>::const_iterator
      iBegin = fRepeatCommonPartElementsList.begin (),
      iEnd   = fRepeatCommonPartElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrVoiceElement element = (*i);

      if (
        S_msrNote note = dynamic_cast<msrNote*>(&(*element))
        ) {
        result = note;
        break;
      }

      else if (
        S_msrChord chord = dynamic_cast<msrChord*>(&(*element))
        ) {
        // get the chord's first note
        result = chord->fetchChordFirstNonGraceNote ();
        break;
      }

      else if (
        S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*element))
        ) {
        // get the tuplet's first note
        result = tuplet->fetchTupletFirstNonGraceNote ();
        break;
      }

      else if (
        S_msrClef clef = dynamic_cast<msrClef*>(&(*element))
        ) {
        // ignore this clef
      }

      else if (
        S_msrKey key = dynamic_cast<msrKey*>(&(*element))
        ) {
        // ignore this key
      }

      else if (
        S_msrTimeSignature timeSignature = dynamic_cast<msrTimeSignature*>(&(*element))
        ) {
        // ignore this time
      }

      else {
        stringstream s;

        s <<
          "tuplet first element should be a note, a chord or another tuplet, found instead '" <<
          element->asShortString () <<
          "'";

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      if (++i == iEnd) break;
    } // for
  }

  return result;
}

void msrRepeatCommonPart::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrRepeatCommonPart::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrRepeatCommonPart>*
    p =
      dynamic_cast<visitor<S_msrRepeatCommonPart>*> (v)) {
        S_msrRepeatCommonPart elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrRepeatCommonPart::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRepeatCommonPart::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrRepeatCommonPart::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRepeatCommonPart>*
    p =
      dynamic_cast<visitor<S_msrRepeatCommonPart>*> (v)) {
        S_msrRepeatCommonPart elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrRepeatCommonPart::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRepeatCommonPart::browseData (basevisitor* v)
{
  // browse the elements
  if (fRepeatCommonPartElementsList.size ()) {
    for (
      list<S_msrVoiceElement>::const_iterator i = fRepeatCommonPartElementsList.begin ();
      i != fRepeatCommonPartElementsList.end ();
      ++i
  ) {
      // browse the element
      msrBrowser<msrVoiceElement> browser (v);
      browser.browse (*(*i));
    } // for
  }
}

string msrRepeatCommonPart::asString () const
{
  stringstream s;

  s <<
    "[RepeatCommonPart" <<
    ", repeat upLink: '" <<
    fRepeatCommonPartUpLinkToRepeat->
      asShortString () <<
    "', line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrRepeatCommonPart::print (ostream& os) const
{
  os <<
    "[RepeatCommonPart" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  os <<
    "repeat upLink: '" <<
    fRepeatCommonPartUpLinkToRepeat->
      asShortString () <<
      "'" <<
    endl << endl;

  // print the elements
  int repeatCommonPartElementsListSize =
    fRepeatCommonPartElementsList.size ();

  os <<
    "fRepeatCommonPartElementsList: ";
  if (repeatCommonPartElementsListSize) {
    os <<
      '(' <<
      mfSingularOrPlural (
        repeatCommonPartElementsListSize, "element", "elements") <<
      ")";
  }
  else {
    os << "empty";
  }
  os << endl;

  if (repeatCommonPartElementsListSize) {
    os << endl;

    ++gIndenter;

    list<S_msrVoiceElement>::const_iterator
      iBegin = fRepeatCommonPartElementsList.begin (),
      iEnd   = fRepeatCommonPartElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << ']' << endl;
}

void msrRepeatCommonPart::printShort (ostream& os) const
{
  os <<
    "[RepeatCommonPart" <<
//     ", repeat upLink: '" <<
//     fRepeatCommonPartUpLinkToRepeat->
//       asShortString () <<
    "', line " << fInputLineNumber <<
    endl;

  ++gIndenter;

/* JMI
  os <<
    "repeat upLink: '" <<
    fRepeatCommonPartUpLinkToRepeat->
      asShortString () <<
      "'" <<
    endl;
*/

  // print the elements
  int repeatCommonPartElementsListSize =
    fRepeatCommonPartElementsList.size ();

  os <<
    "repeatCommonPartElementsList: ";
  if (repeatCommonPartElementsListSize) {
    os <<
      '(' <<
      mfSingularOrPlural (
        repeatCommonPartElementsListSize, "element", "elements") <<
      ")";
  }
  else {
    os << "empty";
  }
  os << endl;

  if (repeatCommonPartElementsListSize) {
    os << endl;

    ++gIndenter;

    list<S_msrVoiceElement>::const_iterator
      iBegin = fRepeatCommonPartElementsList.begin (),
      iEnd   = fRepeatCommonPartElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // short print the element
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrRepeatCommonPart& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }
  
  return os;
}

//______________________________________________________________________________
S_msrRepeatEnding msrRepeatEnding::create (
  int                 inputLineNumber,
  const string&       repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind repeatEndingKind,
  S_msrRepeat         upLinkToRepeat)
{
  msrRepeatEnding* o =
    new msrRepeatEnding (
      inputLineNumber,
      repeatEndingNumber,
      repeatEndingKind,
      upLinkToRepeat);
  assert (o != nullptr);
  return o;
}

msrRepeatEnding::msrRepeatEnding (
  int                 inputLineNumber,
  const string&       repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind repeatEndingKind,
  S_msrRepeat         upLinkToRepeat)
    : msrElement (inputLineNumber)
{
  fRepeatEndingNumber = repeatEndingNumber;

  fRepeatEndingInternalNumber = 0;
    // will be set by msrRepeat::addRepeatEnding ()

  fRepeatEndingKind = repeatEndingKind;

  fRepeatEndingUpLinkToRepeat = upLinkToRepeat;
}

msrRepeatEnding::~msrRepeatEnding ()
{}

void msrRepeatEnding::appendSegmentToRepeatEnding (
  int           inputLineNumber,
  S_msrSegment  segment,
  const string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending segment '" << segment <<
      "' to repeat ending '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    segment != nullptr,
    "segment is null");

  fRepeatEndingElementsList.push_back (segment);
}

void msrRepeatEnding::appendRepeatToRepeatEnding (
  int           inputLineNumber,
  S_msrRepeat   repeat,
  const string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending repeat '" << repeat <<
      "' to repeat ending '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    repeat != nullptr,
    "repeat is null");

  fRepeatEndingElementsList.push_back (repeat);
}

void msrRepeatEnding::appendMeasureRepeatToRepeatEnding (
  int                inputLineNumber,
  S_msrMeasureRepeat measureRepeat,
  const string&      context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending measures repeat '" <<
      measureRepeat->asShortString () <<
      "' to repeat ending '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeat != nullptr,
    "measureRepeat is null");

// KAKA  fRepeatEndingElementsList.push_back (measureRepeat);
}

void msrRepeatEnding::appendMultipleFullBarRestsToRepeatEnding (
  int                   inputLineNumber,
  S_msrMultipleFullBarRests multipleFullBarRests,
  const string&         context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending multiple full-bar rests " <<
      multipleFullBarRests->asShortString () <<
      " to repeat ending " << asString () <<
      " (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    multipleFullBarRests != nullptr,
    "multipleFullBarRests is null");

//  KAKA fRepeatEndingElementsList.push_back (multipleFullBarRests);
}

void msrRepeatEnding::appendVoiceElementToRepeatEnding (
  int               inputLineNumber,
  S_msrVoiceElement voiceElement,
  const string&     context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending voice element '" << voiceElement <<
      "' to repeat ending '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voiceElement != nullptr,
    "voiceElement is null");

  fRepeatEndingElementsList.push_back (voiceElement);
}

void msrRepeatEnding::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrRepeatEnding::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrRepeatEnding>*
    p =
      dynamic_cast<visitor<S_msrRepeatEnding>*> (v)) {
        S_msrRepeatEnding elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrRepeatEnding::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRepeatEnding::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrRepeatEnding::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRepeatEnding>*
    p =
      dynamic_cast<visitor<S_msrRepeatEnding>*> (v)) {
        S_msrRepeatEnding elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrRepeatEnding::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRepeatEnding::browseData (basevisitor* v)
{
  // browse the elements
  if (fRepeatEndingElementsList.size ()) {
    for (
      list<S_msrVoiceElement>::const_iterator i = fRepeatEndingElementsList.begin ();
      i != fRepeatEndingElementsList.end ();
      ++i
  ) {
      // browse the element
      msrBrowser<msrVoiceElement> browser (v);
      browser.browse (*(*i));
    } // for
  }
}

string msrRepeatEnding::asString () const
{
  stringstream s;

  s <<
    "[RepeatEnding" <<
    ", " <<
    fRepeatEndingKind <<
    ", RepeatEndingUpLinkToRepeat: '";

//   if (fRepeatEndingUpLinkToRepeat) { JMI v0.9.64
//     s <<
//       fRepeatEndingUpLinkToRepeat->
//         asShortString ();
//   }
//   else {
//     s <<
//       "[NONE]";
//   }

  s <<
    ", repeatEndingNumber: " << fRepeatEndingNumber <<
    ", repeatEndingInternalNumber: " << fRepeatEndingInternalNumber <<
    "', line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrRepeatEnding::print (ostream& os) const
{
  os <<
    endl <<
    "[RepeatEnding" <<
    ", repeatEndingKind" << " : " <<
    msrRepeatEndingKindAsString (
      fRepeatEndingKind) <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

/* JMI
  os <<
    "repeat upLink: '" <<
    fRepeatEndingUpLinkToRepeat->
      asShortString () <<
      "'" <<
    endl;
*/

  const int fieldWidth = 27;

  os << left <<
    setw (fieldWidth) <<
    "repeatEndingNumber" <<  " : " <<fRepeatEndingNumber <<
    endl <<
    setw (fieldWidth) <<
    "repeatEndingInternalNumber" <<  " : " <<fRepeatEndingInternalNumber <<
    endl <<
    setw (fieldWidth) <<
    "repeat upLink" << " : '" <<
    fRepeatEndingUpLinkToRepeat->
      asShortString () <<
    "'" <<
    endl << endl;

  // print the elements
  int repeatEndingElementsListSize =
    fRepeatEndingElementsList.size ();

  os <<
    "repeatEndingElementsList: ";
  if (repeatEndingElementsListSize) {
    os <<
      '(' <<
      mfSingularOrPlural (
        repeatEndingElementsListSize, "element", "elements") <<
      ")";
  }
  else {
    os << "empty";
  }
  os << endl;

  if (repeatEndingElementsListSize) {
    os << endl;

    ++gIndenter;

    list<S_msrVoiceElement>::const_iterator
      iBegin = fRepeatEndingElementsList.begin (),
      iEnd   = fRepeatEndingElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the element
      (*i)->print (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << ']' << endl;
}

void msrRepeatEnding::printShort (ostream& os) const
{
  os <<
    "[RepeatEnding" <<
    ", fRepeatEndingKind: " <<
    fRepeatEndingKind <<
    ", repeatEndingNumber: " << fRepeatEndingNumber <<
    ", repeatEndingInternalNumber: " << fRepeatEndingInternalNumber <<
    "', line " << fInputLineNumber <<
    endl;

/*
  ++gIndenter;

  const int fieldWidth = 27;

  os << left <<
    setw (fieldWidth) <<
    "repeatEndingNumber" <<  " : " <<fRepeatEndingNumber <<
    / * JMI
    endl <<
    setw (fieldWidth) <<
    "repeatEndingInternalNumber" <<  " : " <<fRepeatEndingInternalNumber <<
    endl <<
    setw (fieldWidth) <<
    "repeat upLink: " << " : '" <<
    fRepeatEndingUpLinkToRepeat->
      asShortString () <<
    "'" <<
    endl <<
    * /
    endl;

/ * JMI
  os <<
    "repeat upLink: '" <<
    fRepeatEndingUpLinkToRepeat->
      asShortString () <<
      "'" <<
    endl;
* /

  // print the elements
  int repeatEndingElementsListSize =
    fRepeatEndingElementsList.size ();

  os <<
    "repeatEndingElementsList: ";
  if (repeatEndingElementsListSize) {
    os <<
      '(' <<
      mfSingularOrPlural (
        repeatEndingElementsListSize, "element", "elements") <<
      ")";
  }
  else {
    os << "[NONE]";
  }
  os << endl;

  if (repeatEndingElementsListSize) {
    os << endl;

    ++gIndenter;

    list<S_msrVoiceElement>::const_iterator
      iBegin = fRepeatEndingElementsList.begin (),
      iEnd   = fRepeatEndingElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the element
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
*/

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrRepeatEnding& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }
  
  return os;
}

//______________________________________________________________________________
S_msrRepeat msrRepeat::create (
  int        inputLineNumber,
  int        repeatTimes,
  S_msrVoice upLinkToVoice)
{
  msrRepeat* o =
    new msrRepeat (
      inputLineNumber,
      repeatTimes,
      upLinkToVoice);
  assert (o != nullptr);
  return o;
}

msrRepeat::msrRepeat (
  int        inputLineNumber,
  int        repeatTimes,
  S_msrVoice upLinkToVoice)
    : msrVoiceElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToVoice != nullptr,
    "upLinkToVoice is null");

  fRepeatUpLinkToVoice = upLinkToVoice;

  fRepeatTimes = repeatTimes;

  fRepeatExplicitStartKind = kRepeatExplicitStartNo; // default value

  fRepeatEndingsInternalCounter = 0;

  // repeat build phase
  fCurrentRepeatBuildPhaseKind =
    msrRepeat::kRepeatBuildPhaseJustCreated;
}

S_msrRepeat msrRepeat::createRepeatNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Creating a newborn clone of a repeat" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrRepeat
    newbornClone =
      msrRepeat::create (
        fInputLineNumber,
        fRepeatTimes,
        containingVoice);

  return newbornClone;
}

msrRepeat::~msrRepeat ()
{}

void msrRepeat::setRepeatCommonPart (
  S_msrRepeatCommonPart repeatCommonPart)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    repeatCommonPart != nullptr,
    "repeatCommonPart is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Setting repeat common part containing " <<
      mfSingularOrPlural (
        repeatCommonPart->
          getRepeatCommonPartElementsList ().size (),
        "element",
        "elements") <<
      endl;
  }
#endif

  fRepeatCommonPart = repeatCommonPart;

  // set repeat's build phase
  fCurrentRepeatBuildPhaseKind =
    msrRepeat::kRepeatBuildPhaseInCommonPart;
}

void msrRepeat::addRepeatEndingToRepeat (
  int               inputLineNumber,
  S_msrRepeatEnding repeatEnding)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    repeatEnding != nullptr,
    "repeatEnding is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Adding ending '" <<
      repeatEnding->asString () <<
      "' to repeat" <<
      "' to repeat '" <<
      asShortString () <<
      "'" <<
      endl;
  }
#endif

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayRepeat (
      inputLineNumber,
      "addRepeatEndingToRepeat() 1");
  }
#endif

  // get repeat ending kind
  msrRepeatEndingKind
    repeatEndingKind =
      repeatEnding->
        getRepeatEndingKind ();

  // consistency check
  switch (repeatEndingKind) {
    case msrRepeatEndingKind::kRepeatEndingHooked:
      switch (fCurrentRepeatBuildPhaseKind) {
        case msrRepeat::kRepeatBuildPhaseJustCreated:
          {
            stringstream s;

            s <<
              "cannot add hooked ending '" <<
              repeatEnding->asShortString () <<
              "' right after a repeat has been created";

            msrError (
              gGlobalServiceRunData->getInputSourceName (),
              fInputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
        case msrRepeat::kRepeatBuildPhaseInCommonPart:
          fCurrentRepeatBuildPhaseKind =
            msrRepeat::kRepeatBuildPhaseInEndings;
          break;
        case msrRepeat::kRepeatBuildPhaseInEndings:
          // there can be several successive endings
          break;
        case msrRepeat::kRepeatBuildPhaseCompleted:
          {
            stringstream s;

            s <<
              "cannot add hooked ending '" <<
              repeatEnding->asShortString () <<
              "' after a repeat has been completed";

            msrError (
              gGlobalServiceRunData->getInputSourceName (),
              fInputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;

    case msrRepeatEndingKind::kRepeatEndingHookless:
      switch (fCurrentRepeatBuildPhaseKind) {
        case msrRepeat::kRepeatBuildPhaseJustCreated:
          {
            stringstream s;

            s <<
              "cannot add hookless repeat ending " <<
              repeatEnding->asShortString () <<
              " right after the repeat has been created";

            msrInternalError (
              gGlobalServiceRunData->getInputSourceName (),
              fInputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
        case msrRepeat::kRepeatBuildPhaseInCommonPart:
          {
            stringstream s;

            s <<
              "cannot add hookless repeat ending " <<
              repeatEnding->asShortString () <<
              " right after the repeat common part";

//             msrInternalError ( JMI v0.9.63
            msrInternalWarning (
              gGlobalServiceRunData->getInputSourceName (),
              fInputLineNumber,
//               __FILE__, __LINE__,
              s.str ());
          }
          break;
        case msrRepeat::kRepeatBuildPhaseInEndings:
          // there can be several successive endings
          break;
        case msrRepeat::kRepeatBuildPhaseCompleted:
          {
            stringstream s;

            s <<
              "cannot add hookless ending '" <<
              repeatEnding->asShortString () <<
              "' after a repeat has been completed";

            msrError (
              gGlobalServiceRunData->getInputSourceName (),
              fInputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
      } // switch
      break;
  } // switch

  fRepeatEndings.push_back (repeatEnding);

  // set repeat ending internal number
  repeatEnding->
    setRepeatEndingInternalNumber (
      ++ fRepeatEndingsInternalCounter);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeatsDetails ()) {
    displayRepeat (
      inputLineNumber,
      "addRepeatEndingToRepeat() 2");
  }
#endif
}

void msrRepeat::appendSegmentToRepeat (
  int           inputLineNumber,
  S_msrSegment  segment,
  const string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending segment '" <<
      segment->asString () <<
      "' to repeat '" <<
      asShortString () <<
      "'" <<
      endl;

    displayRepeat (
      inputLineNumber,
      "appendSegmentToRepeat() 1");
  }
#endif

  switch (fCurrentRepeatBuildPhaseKind) {
    case msrRepeat::kRepeatBuildPhaseJustCreated:
      {
        stringstream s;

        s <<
          "segment '" <<
          segment->asShortString () <<
          "'cannot be added to a just created repeat" <<
          " (" << context << ")";

        msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrRepeat::kRepeatBuildPhaseInCommonPart:
        fRepeatCommonPart->
          appendSegmentToRepeatCommonPart (
            inputLineNumber,
            segment,
            context);
      break;

    case msrRepeat::kRepeatBuildPhaseInEndings:
      fRepeatEndings.back ()->
        appendSegmentToRepeatEnding (
          inputLineNumber,
          segment,
          context);
      break;

    case msrRepeat::kRepeatBuildPhaseCompleted:
      {
        stringstream s;

        s <<
          "segment '" <<
          segment->asShortString () <<
          "'cannot be added to a completed repeat" <<
          '(' << context << ")";

        msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayRepeat (
      inputLineNumber,
      "appendSegmentToRepeat() 2");
  }
#endif
}

void msrRepeat::appendRepeatToRepeat (
  int           inputLineNumber,
  S_msrRepeat   repeat,
  const string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending repeat '" <<
      repeat->asString () <<
      "' to repeat '" <<
      asShortString () <<
      "'" <<
      endl;

    displayRepeat (
      inputLineNumber,
      "appendRepeatToRepeat() 1");
  }
#endif

  switch (fCurrentRepeatBuildPhaseKind) {
    case msrRepeat::kRepeatBuildPhaseJustCreated:
      {
        stringstream s;

        s <<
          "repeat '" <<
          repeat->asShortString () <<
          "'cannot be added to a just created repeat" <<
          " (" << context << ")";

        msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrRepeat::kRepeatBuildPhaseInCommonPart:
        fRepeatCommonPart->
          appendRepeatToRepeatCommonPart (
            inputLineNumber,
            repeat,
            context);
      break;

    case msrRepeat::kRepeatBuildPhaseInEndings:
      fRepeatEndings.back ()->
        appendRepeatToRepeatEnding (
          inputLineNumber,
          repeat,
          context);
      break;

    case msrRepeat::kRepeatBuildPhaseCompleted:
      {
        stringstream s;

        s <<
          "repeat '" <<
          repeat->asShortString () <<
          "'cannot be added to a completed repeat" <<
          '(' << context << ")";

        msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayRepeat (
      inputLineNumber,
      "appendRepeatToRepeat() 2");
  }
#endif
}

void msrRepeat::appendMeasureRepeatToRepeat (
  int                inputLineNumber,
  S_msrMeasureRepeat measureRepeat,
  const string&      context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending measures repeat '" <<
      measureRepeat->asString () <<
      "' to repeat '" <<
      asShortString () <<
      "'" <<
      endl;

    displayRepeat (
      inputLineNumber,
      "appendMeasureRepeatToRepeat() 1");
  }
#endif

  switch (fCurrentRepeatBuildPhaseKind) {
    case msrRepeat::kRepeatBuildPhaseJustCreated:
      {
        stringstream s;

        s <<
          "measures repeat '" <<
          measureRepeat->asShortString () <<
          "'cannot be added to a just created repeat" <<
          " (" << context << ")";

        msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrRepeat::kRepeatBuildPhaseInCommonPart:
        fRepeatCommonPart->
          appendMeasureRepeatToRepeatCommonPart (
            inputLineNumber,
            measureRepeat,
            context);
      break;

    case msrRepeat::kRepeatBuildPhaseInEndings:
      fRepeatEndings.back ()->
        appendMeasureRepeatToRepeatEnding (
          inputLineNumber,
          measureRepeat,
          context);
      break;

    case msrRepeat::kRepeatBuildPhaseCompleted:
      {
        stringstream s;

        s <<
          "measures repeat '" <<
          measureRepeat->asShortString () <<
          "'cannot be added to a completed repeat" <<
          '(' << context << ")";

        msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayRepeat (
      inputLineNumber,
      "appendMeasureRepeatToRepeat() 2");
  }
#endif
}

void msrRepeat::appendMultipleFullBarRestsToRepeat (
  int                   inputLineNumber,
  S_msrMultipleFullBarRests multipleFullBarRests,
  const string&         context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    gLogStream <<
      "Appending multiple full-bar rests " <<
      multipleFullBarRests->asString () <<
      " to repeat " <<
      asShortString () <<
      endl;

    displayRepeat (
      inputLineNumber,
      "appendMultipleFullBarRestsToRepeat() 1");
  }
#endif

  switch (fCurrentRepeatBuildPhaseKind) {
    case msrRepeat::kRepeatBuildPhaseJustCreated:
      {
        stringstream s;

        s <<
          "multiple full-bar rests '" <<
          multipleFullBarRests->asShortString () <<
          "'cannot be added to a just created repeat" <<
          " (" << context << ")";

        msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrRepeat::kRepeatBuildPhaseInCommonPart:
      fRepeatCommonPart->
        appendMultipleFullBarRestsToRepeatCommonPart (
          inputLineNumber,
          multipleFullBarRests,
          context);
      break;

    case msrRepeat::kRepeatBuildPhaseInEndings:
      fRepeatEndings.back ()->
        appendMultipleFullBarRestsToRepeatEnding (
          inputLineNumber,
          multipleFullBarRests,
          context);
      break;

    case msrRepeat::kRepeatBuildPhaseCompleted:
      {
        stringstream s;

        s <<
          "multiple full-bar rests '" <<
          multipleFullBarRests->asShortString () <<
          "'cannot be added to a completed repeat" <<
          '(' << context << ")";

        msrError (
          gGlobalServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
    displayRepeat (
      inputLineNumber,
      "appendMultipleFullBarRestsToRepeat() 2");
  }
#endif
}

S_msrNote msrRepeat::fetchRepeatFirstNonGraceNote () const
{
  S_msrNote result;

  if (fRepeatCommonPart) {
    result =
      fRepeatCommonPart->
        fetchRepeatCommonPartFirstNonGraceNote ();
  }

  return result;
}

void msrRepeat::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrRepeat::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrRepeat::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRepeat::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrRepeat::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrRepeat::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRepeat::browseData (basevisitor* v)
{
  if (fRepeatCommonPart) {
  // browse the common part
    msrBrowser<msrRepeatCommonPart> browser (v);
    browser.browse (*fRepeatCommonPart);
  }

  // browse the alternatives
  for (
    vector<S_msrRepeatEnding>::const_iterator i = fRepeatEndings.begin ();
    i != fRepeatEndings.end ();
    ++i
  ) {
    // browse the alternative
    msrBrowser<msrRepeatEnding> browser (v);
    browser.browse (*(*i));
  } // for
}

string msrRepeat::repeatExplicitStartKindAsString (
  msrRepeatExplicitStartKind repeatExplicitStartKind)
{
  string result;

  switch (repeatExplicitStartKind) {
    case msrRepeat::kRepeatExplicitStartNo:
      result = "kRepeatExplicitStartNo";
      break;
    case msrRepeat::kRepeatExplicitStartYes:
      result = "kRepeatExplicitStartYes";
      break;
  } // switch

  return result;
}

string msrRepeat::repeatBuildPhaseKindAsString (
  msrRepeatBuildPhaseKind repeatBuildPhaseKind)
{
  string result;

  switch (repeatBuildPhaseKind) {
    case msrRepeat::kRepeatBuildPhaseJustCreated:
      result = "kRepeatBuildPhaseJustCreated";
      break;
    case msrRepeat::kRepeatBuildPhaseInCommonPart:
      result = "kRepeatBuildPhaseInCommonPart";
      break;
    case msrRepeat::kRepeatBuildPhaseInEndings:
      result = "kRepeatBuildPhaseInEndings";
      break;
    case msrRepeat::kRepeatBuildPhaseCompleted:
      result = "kRepeatBuildPhaseCompleted";
      break;
  } // switch

  return result;
}

string msrRepeat::asShortString () const
{
  stringstream s;

  s <<
    "Repeat" <<
    ", " << fRepeatTimes << " times" <<
   ", fRepeatExplicitStartKind: " <<
    repeatExplicitStartKindAsString (
      fRepeatExplicitStartKind);
//     ", fCurrentRepeatBuildPhaseKind: " <<
//     repeatBuildPhaseKindAsString (
//       fCurrentRepeatBuildPhaseKind);

  if (fImmediatelyPrecedingRepeat) {
    s <<
      ", fImmediatelyPrecedingRepeat: " <<
      fImmediatelyPrecedingRepeat->asShortString ();
  }
/*
  don't print the following repeat if any,
  to avoid an infinite loop
  if (fImmediatelyFollowingRepeat) {
    s <<
      ", fImmediatelyFollowingRepeat: " <<
      fImmediatelyFollowingRepeat->asShortString ();
  }
*/

  s <<
    ", fRepeatCommonPart: ";
  if (fRepeatCommonPart) {
    s <<
      mfSingularOrPlural (
        fRepeatCommonPart->
          getRepeatCommonPartElementsList ().size (),
        "element",
        "elements");
  }
  else {
    s <<
      "[NONE]";
  }

  int repeatEndingsNumber =
    fRepeatEndings.size ();

  s <<
    ", repeatEndingsNumber: " << repeatEndingsNumber <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string msrRepeat::asString () const
{
  stringstream s;

  s <<
    "[Repeat" <<
    ", " << fRepeatTimes << " times" <<
   ", fRepeatExplicitStartKind:: " <<
    repeatExplicitStartKindAsString (
      fRepeatExplicitStartKind);
//     ", fCurrentRepeatBuildPhaseKind: " <<
//     repeatBuildPhaseKindAsString (
//       fCurrentRepeatBuildPhaseKind);

  if (fImmediatelyPrecedingRepeat) {
    s <<
      ", fImmediatelyPrecedingRepeat: " <<
      fImmediatelyPrecedingRepeat->asShortString ();
  }
  if (fImmediatelyFollowingRepeat) {
    s <<
      ", fImmediatelyFollowingRepeat: " <<
      fImmediatelyFollowingRepeat->asShortString ();
  }

  s <<
    ", fRepeatCommonPart: ";
  if (fRepeatCommonPart) {
    s <<
      fRepeatCommonPart->asString ();
  }
  else {
    s <<
      "[NONE]";
  }

  int endingsNumber =
    fRepeatEndings.size ();

  s <<
    ", endingsNumber: " << endingsNumber;

  if (endingsNumber) {
    vector<S_msrRepeatEnding>::const_iterator
      iBegin = fRepeatEndings.begin (),
      iEnd   = fRepeatEndings.end (),
      i      = iBegin;

    s <<
      ", ";

    for ( ; ; ) {
      // print the repeat ending
      s << (*i)->asShortString ();
      if (++i == iEnd) break;
      s << ' ';
    } // for
  }

  s <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrRepeat::displayRepeat (
  int           inputLineNumber,
  const string& context)
{
  gLogStream <<
    endl <<
    "*********>> Repeat '" <<
    asShortString () <<
    "' (" << context << ")" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    endl;

  ++gIndenter;
  print (gLogStream);
  --gIndenter;

  gLogStream <<
    " <<*********" <<
    endl << endl;
}

void msrRepeat::print (ostream& os) const
{
  os <<
    "[Repeat" <<
    ", " << fRepeatTimes << " times" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 28;

  os << left <<
    setw (fieldWidth) <<
   "fRepeatExplicitStartKind" << " : " <<
    repeatExplicitStartKindAsString (
      fRepeatExplicitStartKind) <<
    endl <<
    setw (fieldWidth) <<
    "repeat ending(s)" << " : " <<
    fRepeatEndings.size () <<
    endl;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRepeats ()) {
//     // print the current repeat build phase
//     os << left <<
//       setw (fieldWidth) <<
//       "fCurrentRepeatBuildPhaseKind" << " : " <<
//       repeatBuildPhaseKindAsString (
//         fCurrentRepeatBuildPhaseKind) <<
//       endl;
  }
#endif

  os << endl;

  // print the immediately preceding and following repeats
  os << left <<
    setw (fieldWidth) <<
    "fImmediatelyPrecedingRepeat" << " : ";
  if (fImmediatelyPrecedingRepeat) {
    os <<
      fImmediatelyPrecedingRepeat->asShortString ();
  }
  else {
    os << "[NONE]";
  }
  os << endl << endl;

  os << left <<
    setw (fieldWidth) <<
    "fImmediatelyFollowingRepeat" << " : ";
  if (fImmediatelyFollowingRepeat) {
    os <<
      fImmediatelyFollowingRepeat->asShortString ();
  }
  else {
    os << "[NONE]";
  }
  os << endl << endl;

  // print the repeat common part
  os << left <<
    setw (fieldWidth) <<
    "fRepeatCommonPart" << " : ";
  if (fRepeatCommonPart) {
    os <<
      fRepeatCommonPart <<
      endl;
  }
  else {
    os << "[NONE]";
  }
  os << endl << endl;

  // print the repeat endings
  int endingsNumber =
    fRepeatEndings.size ();

  os <<
    "fRepeatEndings: ";
  if (endingsNumber) {
    os << '(' << endingsNumber << ")";
  }
  else {
    os << "[NONE]";
  }
  os << endl;

  if (endingsNumber) {
    ++gIndenter;

    vector<S_msrRepeatEnding>::const_iterator
      iBegin = fRepeatEndings.begin (),
      iEnd   = fRepeatEndings.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the repeat ending
      os << (*i);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << ']' << endl;
}

void msrRepeat::printShort (ostream& os) const
{
  os <<
    "[Repeat" <<
    ", " << fRepeatTimes << " times" <<
    ", fRepeatExplicitStartKind:: " <<
    fRepeatExplicitStartKind <<
//     ", fCurrentRepeatBuildPhaseKind: " <<
//     repeatBuildPhaseKindAsString (
//       fCurrentRepeatBuildPhaseKind);
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 18;

  // short print the repeat common part
  os <<
    setw (fieldWidth) <<
    "fRepeatCommonPart" << " : ";
  if (fRepeatCommonPart) {
    os << endl;
    ++gIndenter;
    fRepeatCommonPart->printShort (os);
    --gIndenter;
  }
  else {
    os << "[NONE]" << endl;
  }

  // short print the repeat endings
  int repeatEndingsSize =
    fRepeatEndings.size ();

  os <<
    setw (fieldWidth) <<
    "fRepeatEndings" << " : ";
  if (repeatEndingsSize) {
    os << '(' << repeatEndingsSize << ")";
  }
  else {
    os << "[NONE]";
  }
  os << endl;

  if (repeatEndingsSize) {
    ++gIndenter;

    vector<S_msrRepeatEnding>::const_iterator
      iBegin = fRepeatEndings.begin (),
      iEnd   = fRepeatEndings.end (),
      i      = iBegin;

    for ( ; ; ) {
      // short print the repeat ending
      (*i)->printShort (os);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << "]" << endl;
}

ostream& operator << (ostream& os, const S_msrRepeat& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }
  
  return os;
}


}

