/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrWae.h"

// #include "msrRepeatsEnumTypes.h"
#include "msrRepeats.h"
#include "msrTuplets.h"
#include "msrVoiceConstants.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
/*
  Repeats in MusicXML are applied to all voices in all staves of the current part

  The currentRepeat in each voice is the top of the voice repeats stack

  A repeat is recognized in MusicXML either by:

    - it's start: handleRepeatStart()
    - it's first hooked ending (45b): handleRepeatEndingStart()
        the elements before it are moved to the new repeat's common part
    - it's end: handleRepeatEnd()

  Hooked endings following the first one are added to currentRepeat handleRepeatHookedEndingEnd()

  A hookless ending terminates currentRepeat: handleRepeatHooklessEndingEnd()
    finalize currentRepeat
*/

//______________________________________________________________________________
S_msrRepeatElement msrRepeatElement::create (
  const mfInputLineNumber& inputLineNumber,
  msrSegmentKind           segmentKind)
{
  msrRepeatElement* obj =
    new msrRepeatElement (
      inputLineNumber,
      segmentKind);
  assert (obj != nullptr);
  return obj;
}

S_msrRepeatElement msrRepeatElement::create (
  const mfInputLineNumber& inputLineNumber,
  msrSegmentKind           segmentKind,
  const S_msrRepeat&       upLinkToRepeat)
{
  msrRepeatElement* obj =
    new msrRepeatElement (
      inputLineNumber,
      segmentKind,
      upLinkToRepeat);
  assert (obj != nullptr);
  return obj;
}

msrRepeatElement::msrRepeatElement (
  const mfInputLineNumber& inputLineNumber,
  msrSegmentKind           segmentKind)
    : msrElement (inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a repeat element" <<
      ", upLinkToRepeat: [NULL]" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  initializeRepeatElement (
    inputLineNumber,
    segmentKind);
}

msrRepeatElement::msrRepeatElement (
  const mfInputLineNumber& inputLineNumber,
  msrSegmentKind           segmentKind,
  const S_msrRepeat&       upLinkToRepeat)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    upLinkToRepeat != nullptr,
    "upLinkToRepeat is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a repeat element";

    ss <<
      ", upLinkToRepeat: " <<
      fetchRepeatAsShortString (upLinkToRepeat) <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fRepeatElementUpLinkToRepeat = upLinkToRepeat;

  initializeRepeatElement (
    inputLineNumber,
    segmentKind);
}

msrRepeatElement::~msrRepeatElement ()
{}

void msrRepeatElement::initializeRepeatElement (
  const mfInputLineNumber& inputLineNumber,
  msrSegmentKind           segmentKind)
{
//   // create repeat element segment
//   fRepeatElementSegment =
//     msrSegment::create (
//       inputLineNumber,
//       segmentKind);
}

void msrRepeatElement::appendMeasureToRepeatElement (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure&      measure,
  const std::string&       context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending measure " <<
      measure->asString () <<
      " to repeat element " << asString () <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    measure != nullptr,
    "measure is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fRepeatElementSegment->appendMeasureToSegment (
    measure);
}

// void msrRepeatElement::appendBeatRepeatToRepeatElement (
//   const mfInputLineNumber& inputLineNumber,
//   const S_msrRepeat&       repeat,
//   const std::string&       context)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending repeat " <<
//     // JMI  repeat->asString () <<
//       repeat <<
//       " to repeat element " <<
//       asString () <<
//       " (" << context << ")" <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, mfInputLineNumber (__LINE__),
//     repeat != nullptr,
//     "repeat is NULL");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   fRepeatElementSegment->appendRepeatToSegment (repeat);
// }

void msrRepeatElement::appendBeatRepeatToRepeatElement (
  const mfInputLineNumber& inputLineNumber,
  const S_msrBeatRepeat&   beatRepeat,
  const std::string&       context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending beat repeat " <<
      beatRepeat->asString () <<
      " to repeat element " <<
      asString () <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    beatRepeat != nullptr,
    "beatRepeat is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fRepeatElementSegment->appendBeatRepeatToSegment (
    beatRepeat);
}

void msrRepeatElement::appendMeasureRepeatToRepeatElement (
  const mfInputLineNumber&  inputLineNumber,
  const S_msrMeasureRepeat& measureRepeat,
  const std::string&        context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending measures repeat " <<
      measureRepeat->asString () <<
      " to repeat element " <<
      asString () <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    measureRepeat != nullptr,
    "measureRepeat is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fRepeatElementSegment->appendMeasureRepeatToSegment (
    measureRepeat);
}

void msrRepeatElement::appendMultipleMeasureRestToRepeatElement (
  const mfInputLineNumber&        inputLineNumber,
  const S_msrMultipleMeasureRest& multipleMeasureRest,
  const std::string&              context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending measure measure rest " <<
      multipleMeasureRest->asString () <<
      " to repeat element " <<
      asString () <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    multipleMeasureRest != nullptr,
    "multipleMeasureRest is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fRepeatElementSegment->appendMultipleMeasureRestToSegment (
    multipleMeasureRest);
}

void msrRepeatElement::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrRepeatElement::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrRepeatElement>*> (v)) {
        S_msrRepeatElement elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrRepeatElement::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrRepeatElement::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrRepeatElement::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrRepeatElement>*> (v)) {
        S_msrRepeatElement elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrRepeatElement::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrRepeatElement::browseData (basevisitor* v)
{
  // browse the segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fRepeatElementSegment);
}

std::string msrRepeatElement::asString () const
{
  std::stringstream ss;

  ss <<
    "[??? RepeatElement ???]";

  return ss.str ();
}

void msrRepeatElement::print (std::ostream& os) const
{
  os <<
    "[RepeatElement" <<
    ", line " << fInputLineNumber <<
    std::endl;

//   ++gIndenter;
//
//   // print the uplink to the repeat
//   os <<
//     "fRepeatElementUpLinkToRepeat:";
//
//   if (fRepeatElementUpLinkToRepeat) {
//     ++gIndenter;
//     os << fRepeatElementUpLinkToRepeat;
//     --gIndenter;
//   }
//   else {
//     os << " [NULL]";
//   }
//   os << std::endl;
//
//   // print the segment
//   os <<
//     "fRepeatElementSegment: ";
//   ++gIndenter;
//   os << fRepeatElementSegment;
//   --gIndenter;
//
//   --gIndenter;

  os << ']' << std::endl;
}

void msrRepeatElement::printFull (std::ostream& os) const
{
  os <<
    "[RepeatElement" <<
    ", line " << fInputLineNumber <<
    std::endl;

//   ++gIndenter;
//
//   // print the uplink to the repeat
//   os <<
//     "fRepeatElementUpLinkToRepeat:";
//
//   if (fRepeatElementUpLinkToRepeat) {
//     ++gIndenter;
//     fRepeatElementUpLinkToRepeat->printFull (os);
//     --gIndenter;
//   }
//   else {
//     os << " [NULL]";
//   }
//   os << std::endl;
//
//   // print the segment
//   os <<
//     "fRepeatElementSegment: ";
//   ++gIndenter;
//   os << fRepeatElementSegment;
//   --gIndenter;
//
//   --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrRepeatElement& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const msrRepeatElement& elt)
{
  elt.print (os);

  return os;
}

//______________________________________________________________________________
S_msrRepeatCommonPart msrRepeatCommonPart::create (
  const mfInputLineNumber& inputLineNumber)
{
  msrRepeatCommonPart* obj =
    new msrRepeatCommonPart (
      inputLineNumber);
  assert (obj != nullptr);

  return obj;
}

S_msrRepeatCommonPart msrRepeatCommonPart::createAsWellAsSegment (
  const mfInputLineNumber& inputLineNumber,
  const S_msrRepeat&       upLinkToRepeat)
{
  msrRepeatCommonPart* obj =
    new msrRepeatCommonPart (
      inputLineNumber,
      upLinkToRepeat);
  assert (obj != nullptr);

  // create the repeat element segment
  obj->fRepeatElementSegment =
    msrSegment::create (
      inputLineNumber,
      msrSegmentKind::kSegmentKindInRepeatCommonPart);

  return obj;
}

S_msrRepeatCommonPart msrRepeatCommonPart::createRepeatCommonPartNewbornClone (
  const S_msrVoice& containingVoice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of repeat common part " <<
      asShortString () <<
      " in voice " <<
      containingVoice->asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrRepeatCommonPart
    newbornClone =
      msrRepeatCommonPart::create (
        fInputLineNumber);

  // DON't create the repeat element segment, that will be done upon browsing

  // there no scalar fields to be copied

  return newbornClone;
}

msrRepeatCommonPart::msrRepeatCommonPart (
  const mfInputLineNumber& inputLineNumber)
    : msrRepeatElement (
        inputLineNumber,
        msrSegmentKind::kSegmentKindInRepeatCommonPart)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Initializing repeat common part " <<
      asString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrRepeatCommonPart::msrRepeatCommonPart (
  const mfInputLineNumber& inputLineNumber,
  const S_msrRepeat&       upLinkToRepeat)
    : msrRepeatElement (
        inputLineNumber,
        msrSegmentKind::kSegmentKindInRepeatCommonPart)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    upLinkToRepeat != nullptr,
    "upLinkToRepeat is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fRepeatElementUpLinkToRepeat = upLinkToRepeat;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Initializing repeat common part " <<
      asString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrRepeatCommonPart::~msrRepeatCommonPart ()
{}

// void msrRepeatCommonPart::appendSegmentToRepeatCommonPart (
//   const mfInputLineNumber& inputLineNumber,
//   const S_msrSegment&      segment,
//   const std::string&       context)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending segment " <<
//       segment->asString () <<
//       " to repeat common part " << asString () <<
//       " (" << context << ")" <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, mfInputLineNumber (__LINE__),
//     segment != nullptr,
//     "segment is NULL");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   fRepeatCommonPartElementsList.push_back (segment);
// }

void msrRepeatCommonPart::appendRepeatToRepeatCommonPart (
  const mfInputLineNumber& inputLineNumber,
  const S_msrRepeat&       repeat,
  const std::string&       context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending repeat " <<
      repeat->asString () <<
      " to repeat common part " << asString () <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    repeat != nullptr,
    "repeat is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

//   fRepeatCommonPartElementsList.push_back (repeat);
}

void msrRepeatCommonPart::appendMeasureRepeatToRepeatCommonPart (
  const mfInputLineNumber&  inputLineNumber,
  const S_msrMeasureRepeat& measureRepeat,
  const std::string&        context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending measures repeat " <<
      measureRepeat->asString () <<
      " to repeat common part " << asString () <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    measureRepeat != nullptr,
    "measureRepeat is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

// JMI 0.9.67  fRepeatCommonPartElementsList.push_back (measureRepeat); JMI 0.9.66
}

void msrRepeatCommonPart::appendMultipleMeasureRestToRepeatCommonPart (
  const mfInputLineNumber&        inputLineNumber,
  const S_msrMultipleMeasureRest& multipleMeasureRests,
  const std::string&              context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultipleMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Cascading appending multiple measure rest " <<
      multipleMeasureRests->asString () <<
      " to repeat common part " << asString () <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    multipleMeasureRests != nullptr,
    "multipleMeasureRests is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

//  JMI 0.9.67 fRepeatCommonPartElementsList.push_back (multipleMeasureRests);
}

void msrRepeatCommonPart::appendVoiceElementToRepeatCommonPart (
  const mfInputLineNumber& inputLineNumber,
  const S_msrVoiceElement& voiceElement,
  const std::string&       context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending voice element " <<
      voiceElement->asString () <<
      " to repeat common part " << asString () <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    voiceElement != nullptr,
    "voiceElement is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

//   fRepeatCommonPartElementsList.push_back (voiceElement);
}

S_msrNote msrRepeatCommonPart::fetchRepeatCommonPartFirstNonGraceNote () const
{

  S_msrNote result;

  // fetch the first note in the first measure to which
  // a grace notes group can be attached
  // i.e. one not in a grace notes group itself,
  // possibly inside a chord or tuplet

//   if (fRepeatCommonPartElementsList.size ()) {
//     std::list <S_msrVoiceElement>::const_iterator
//       iBegin = fRepeatCommonPartElementsList.begin (),
//       iEnd   = fRepeatCommonPartElementsList.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       S_msrVoiceElement element = (*i);
//
//       if (
//         S_msrNote note = dynamic_cast<msrNote*>(&(*element))
//         ) {
//         result = note;
//         break;
//       }
//
//       else if (
//         S_msrChord chord = dynamic_cast<msrChord*>(&(*element))
//         ) {
//         // get the chord's first note
//         result = chord->fetchChordFirstNonGraceNote ();
//         break;
//       }
//
//       else if (
//         S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*element))
//         ) {
//         // get the tuplet's first note
//         result = tuplet->fetchTupletFirstNonGraceNote ();
//         break;
//       }
//
//       else if (
//         S_msrClef clef = dynamic_cast<msrClef*>(&(*element))
//         ) {
//         // ignore this clef
//       }
//
//       else if (
//         S_msrKey key = dynamic_cast<msrKey*>(&(*element))
//         ) {
//         // ignore this key
//       }
//
//       else if (
//         S_msrTimeSignature timeSignature = dynamic_cast<msrTimeSignature*>(&(*element))
//         ) {
//         // ignore this time
//       }
//
//       else {
//         std::stringstream ss;
//
//         ss <<
//           "tuplet first element should be a note, " <<
//           "a chord or another tuplet, found instead " <<
//           element->asShortString ();
//
//         msrInternalError (
//           gServiceRunData->getInputSourceName (),
//           fInputLineNumber,
//           __FILE__, mfInputLineNumber (__LINE__),
//           ss.str ());
//       }
//
//       if (++i == iEnd) break;
//     } // for
//   }
//
  return result;
}

void msrRepeatCommonPart::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrRepeatCommonPart::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrRepeatCommonPart>*
    p =
      dynamic_cast<visitor<S_msrRepeatCommonPart>*> (v)) {
        S_msrRepeatCommonPart elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrRepeatCommonPart::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrRepeatCommonPart::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrRepeatCommonPart::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrRepeatCommonPart>*
    p =
      dynamic_cast<visitor<S_msrRepeatCommonPart>*> (v)) {
        S_msrRepeatCommonPart elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrRepeatCommonPart::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrRepeatCommonPart::browseData (basevisitor* v)
{
  // browse the segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fRepeatElementSegment);
}

std::string msrRepeatCommonPart::asString () const
{
  std::stringstream ss;

  ss <<
    "[RepeatCommonPart" <<
    ", fRepeatElementSegment: " <<
    fRepeatElementSegment->asShortString () <<
    ", fRepeatElementUpLinkToRepeat: " <<
    fetchRepeatAsShortString (fRepeatElementUpLinkToRepeat) <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrRepeatCommonPart::print (std::ostream& os) const
{
  os <<
    "[RepeatCommonPart" <<
    ", fRepeatElementUpLinkToRepeat: ";
  if (fRepeatElementUpLinkToRepeat) {
    os <<
      asShortString () <<
    ", line " << fInputLineNumber <<
    std::endl;
  }
  else {
    os << "[EMPTY]";
  }

  ++gIndenter;

  // print the segment
  os <<
    "fRepeatElementSegment:" <<
    std::endl;

  ++gIndenter;
  os <<
    fRepeatElementSegment;
  --gIndenter;

//   // print the elements
//   int repeatCommonPartElementsListSize =
//     fRepeatCommonPartElementsList.size ();
//
//   os <<
//     "repeatCommonPartElementsListSize: ";
//   if (repeatCommonPartElementsListSize) {
//     os <<
//       '(' <<
//       mfSingularOrPlural (
//         repeatCommonPartElementsListSize, "element", "elements") <<
//       ")";
//   }
//   else {
//     os << "[EMPTY]";
//   }
//   os << std::endl;
//
//   if (repeatCommonPartElementsListSize) {
//     os << std::endl;
//
//     ++gIndenter;
//
//     std::list <S_msrVoiceElement>::const_iterator
//       iBegin = fRepeatCommonPartElementsList.begin (),
//       iEnd   = fRepeatCommonPartElementsList.end (),
//       i      = iBegin;
//
//     for ( ; ; ) {
//       //printthe element
//       os << (*i);
//       if (++i == iEnd) break;
//       os << std::endl;
//     } // for
//
//     --gIndenter;
//   }

  --gIndenter;

  os << ']' << std::endl;
}

void msrRepeatCommonPart::printFull (std::ostream& os) const
{
  os <<
    "[RepeatCommonPart" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  os <<
    "fRepeatElementUpLinkToRepeat: " <<
    ", fRepeatElementUpLinkToRepeat: " <<
    fetchRepeatAsShortString (fRepeatElementUpLinkToRepeat) <<
    std::endl << std::endl;

//   // print the elements
//   int repeatCommonPartElementsListSize =
//     fRepeatCommonPartElementsList.size ();
//
//   os <<
//     "fRepeatCommonPartElementsList: ";
//   if (repeatCommonPartElementsListSize) {
//     os <<
//       '(' <<
//       mfSingularOrPlural (
//         repeatCommonPartElementsListSize, "element", "elements") <<
//       ")";
//   }
//   else {
//     os << "[EMPTY]";
//   }
//   os << std::endl;
//
//   if (repeatCommonPartElementsListSize) {
//     os << std::endl;
//
//     ++gIndenter;
//
//     std::list <S_msrVoiceElement>::const_iterator
//       iBegin = fRepeatCommonPartElementsList.begin (),
//       iEnd   = fRepeatCommonPartElementsList.end (),
//       i      = iBegin;
//
//     for ( ; ; ) {
//       // print the element
//       os << (*i);
//       if (++i == iEnd) break;
//       os << std::endl;
//     } // for
//
//     --gIndenter;
//   }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrRepeatCommonPart& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const msrRepeatCommonPart& elt)
{
  elt.print (os);

  return os;
}

//______________________________________________________________________________
S_msrRepeatEnding msrRepeatEnding::create (
  const mfInputLineNumber& inputLineNumber,
  const std::string&       repeatEndingNumber,
    // a string, because if may be "1, 2" for example
  msrRepeatEndingKind      repeatEndingKind)
{
  msrRepeatEnding* obj =
    new msrRepeatEnding (
      inputLineNumber,
      repeatEndingNumber,
      repeatEndingKind);
  assert (obj != nullptr);
  return obj;
}

S_msrRepeatEnding msrRepeatEnding::createAsWellAsSegment (
  const mfInputLineNumber& inputLineNumber,
  const std::string&       repeatEndingNumber,
    // a string, because if may be "1, 2" for example
  msrRepeatEndingKind      repeatEndingKind)
{
  msrRepeatEnding* obj =
    new msrRepeatEnding (
      inputLineNumber,
      repeatEndingNumber,
      repeatEndingKind);
  assert (obj != nullptr);

  // create the repeat element segment
  obj->fRepeatElementSegment =
    msrSegment::create (
      inputLineNumber,
      msrSegmentKind::kSegmentKindInRepeatCommonPart);

  return obj;
}

S_msrRepeatEnding msrRepeatEnding::create (
  const mfInputLineNumber& inputLineNumber,
  const std::string&       repeatEndingNumber,
    // a string, because if may be "1, 2" for example
  msrRepeatEndingKind      repeatEndingKind,
  const S_msrRepeat&       upLinkToRepeat)
{
  msrRepeatEnding* obj =
    new msrRepeatEnding (
      inputLineNumber,
      repeatEndingNumber,
      repeatEndingKind,
      upLinkToRepeat);
  assert (obj != nullptr);
  return obj;
}

S_msrRepeatEnding msrRepeatEnding::createRepeatEndingNewbornClone (
  const S_msrVoice& containingVoice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of repeat ending " <<
      asShortString () <<
      " in voice " <<
      containingVoice->asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrRepeatEnding
    newbornClone =
      msrRepeatEnding::create (
        fInputLineNumber,
        fRepeatEndingNumber,
        fRepeatEndingKind);

  // DON't create the repeat element segment, that will be done upon browsing

  newbornClone-> fRepeatEndingInternalNumber =
    fRepeatEndingInternalNumber;

  return newbornClone;
}

msrRepeatEnding::msrRepeatEnding (
  const mfInputLineNumber& inputLineNumber,
  const std::string&       repeatEndingNumber,
    // a string, because if may be "1, 2" for example
  msrRepeatEndingKind      repeatEndingKind)
    : msrRepeatElement (
        inputLineNumber,
        msrSegmentKind::kSegmentKindInRepeatEnding)
{
  fRepeatEndingNumber = repeatEndingNumber;

  fRepeatEndingInternalNumber = 0;
    // will be set by msrRepeat::addRepeatEnding ()

  fRepeatEndingKind = repeatEndingKind;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Initializing repeat ending " <<
      asString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrRepeatEnding::msrRepeatEnding (
  const mfInputLineNumber& inputLineNumber,
  const std::string&       repeatEndingNumber,
    // a string, because if may be "1, 2" for example
  msrRepeatEndingKind      repeatEndingKind,
  const S_msrRepeat&       upLinkToRepeat)
    : msrRepeatElement (
        inputLineNumber,
        msrSegmentKind::kSegmentKindInRepeatEnding)
{
  fRepeatEndingNumber = repeatEndingNumber;

  fRepeatEndingInternalNumber = 0;
    // will be set by msrRepeat::addRepeatEnding ()

  fRepeatEndingKind = repeatEndingKind;

  fRepeatElementUpLinkToRepeat = upLinkToRepeat;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Initializing repeat ending " <<
      asString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrRepeatEnding::~msrRepeatEnding ()
{}

// void msrRepeatEnding::appendSegmentToRepeatEnding (
//   const mfInputLineNumber& inputLineNumber,
//   const S_msrSegment&      segment,
//   const std::string&       context)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending segment " << segment <<
//       " to repeat ending " << asString () <<
//       " (" << context << ")" <<
//       ", line " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, mfInputLineNumber (__LINE__),
//     segment != nullptr,
//     "segment is NULL");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   fRepeatEndingElementsList.push_back (segment);
// }

void msrRepeatEnding::appendRepeatToRepeatEnding (
  const mfInputLineNumber& inputLineNumber,
  const S_msrRepeat&       repeat,
  const std::string&       context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Appending repeat " << repeat <<
      " to repeat ending " << asString () <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    repeat != nullptr,
    "repeat is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

//   fRepeatEndingElementsList.push_back (repeat);
}

void msrRepeatEnding::appendMeasureRepeatToRepeatEnding (
  const mfInputLineNumber&  inputLineNumber,
  const S_msrMeasureRepeat& measureRepeat,
  const std::string&        context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Appending measures repeat " <<
      measureRepeat->asShortString () <<
      " to repeat ending " << asString () <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    measureRepeat != nullptr,
    "measureRepeat is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

// JMI 0.9.67  fRepeatEndingElementsList.push_back (measureRepeat);
}

void msrRepeatEnding::appendMultipleMeasureRestToRepeatEnding (
  const mfInputLineNumber&        inputLineNumber,
  const S_msrMultipleMeasureRest& multipleMeasureRests,
  const std::string&    context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Cascading appending multiple measure rest " <<
      multipleMeasureRests->asShortString () <<
      " to repeat ending " << asString () <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    multipleMeasureRests != nullptr,
    "multipleMeasureRests is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

//  JMI 0.9.67 fRepeatEndingElementsList.push_back (multipleMeasureRests);
}

void msrRepeatEnding::appendVoiceElementToRepeatEnding (
  const mfInputLineNumber& inputLineNumber,
  const S_msrVoiceElement& voiceElement,
  const std::string&       context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Appending voice element " << voiceElement <<
      " to repeat ending " << asString () <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    voiceElement != nullptr,
    "voiceElement is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

//   fRepeatEndingElementsList.push_back (voiceElement);
}

void msrRepeatEnding::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrRepeatEnding::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrRepeatEnding>*
    p =
      dynamic_cast<visitor<S_msrRepeatEnding>*> (v)) {
        S_msrRepeatEnding elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrRepeatEnding::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrRepeatEnding::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrRepeatEnding::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrRepeatEnding>*
    p =
      dynamic_cast<visitor<S_msrRepeatEnding>*> (v)) {
        S_msrRepeatEnding elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrRepeatEnding::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrRepeatEnding::browseData (basevisitor* v)
{
  // browse the segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fRepeatElementSegment);
}

std::string msrRepeatEnding::asString () const
{
  std::stringstream ss;

  ss <<
    "[RepeatEnding" <<
    ", fRepeatEndingKind: " << fRepeatEndingKind <<
    ", fRepeatElementUpLinkToRepeat: " <<
    fetchRepeatAsShortString (fRepeatElementUpLinkToRepeat) <<
    ", fRepeatEndingNumber: " << fRepeatEndingNumber <<
    ", fRepeatEndingInternalNumber: " << fRepeatEndingInternalNumber <<
    ", fRepeatElementSegment: " <<
    fRepeatElementSegment->asShortString () <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrRepeatEnding::print (std::ostream& os) const
{
  os <<
    "[RepeatEnding" <<
    ", fRepeatEndingKind: " << fRepeatEndingKind <<
    ", fRepeatEndingNumber: " << fRepeatEndingNumber <<
    ", fRepeatEndingInternalNumber: " << fRepeatEndingInternalNumber <<
    "', line " << fInputLineNumber <<
    std::endl;

/*
  ++gIndenter;

  constexpr int fieldWidth = 27;

  os << std::left <<
    std::setw (fieldWidth) <<
    "repeatEndingNumber" <<  ": " <<fRepeatEndingNumber <<
    / * JMI
    std::endl <<
    std::setw (fieldWidth) <<
    "repeatEndingInternalNumber" <<  ": " <<fRepeatEndingInternalNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "repeat upLink: " << " : " <<
    fRepeatElementUpLinkToRepeat->
      asShortString () <<
    '\'' <<
    std::endl <<
    * /
    std::endl;

/ * JMI
  os <<
    "repeat upLink: " <<
    fRepeatElementUpLinkToRepeat->
      asShortString () <<
    std::endl;
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
    os << "[EMPTY]";
  }
  os << std::endl;

  if (repeatEndingElementsListSize) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrVoiceElement>::const_iterator
      iBegin = fRepeatEndingElementsList.begin (),
      iEnd   = fRepeatEndingElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

  --gIndenter;
*/

  os << ']' << std::endl;
}

void msrRepeatEnding::printFull (std::ostream& os) const
{
  os <<
    std::endl <<
    "[RepeatEnding" <<
    ", fRepeatEndingKind: " <<
    msrRepeatEndingKindAsString (
      fRepeatEndingKind) <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

/* JMI
  os <<
    "fRepeatElementUpLinkToRepeat: " <<
    fRepeatElementUpLinkToRepeat->
      asShortString () <<
    std::endl;
*/

  constexpr int fieldWidth = 27;

  os << std::left <<
    std::setw (fieldWidth) <<
    "repeatEndingNumber" <<  ": " <<fRepeatEndingNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "repeatEndingInternalNumber" <<  ": " <<fRepeatEndingInternalNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "repeat upLink" << " : " <<
    fRepeatElementUpLinkToRepeat->asShortString () <<
    '\'' <<
    std::endl << std::endl;

//   // print the elements
//   int repeatEndingElementsListSize =
//     fRepeatEndingElementsList.size ();
//
//   os <<
//     "repeatEndingElementsList: ";
//   if (repeatEndingElementsListSize) {
//     os <<
//       '(' <<
//       mfSingularOrPlural (
//         repeatEndingElementsListSize, "element", "elements") <<
//       ")";
//   }
//   else {
//     os << "[EMPTY]";
//   }
//   os << std::endl;
//
//   if (repeatEndingElementsListSize) {
//     os << std::endl;
//
//     ++gIndenter;
//
//     std::list <S_msrVoiceElement>::const_iterator
//       iBegin = fRepeatEndingElementsList.begin (),
//       iEnd   = fRepeatEndingElementsList.end (),
//       i      = iBegin;
//
//     for ( ; ; ) {
//       // print the element
//       (*i)->print (os);
//       if (++i == iEnd) break;
//       os << std::endl;
//     } // for
//
//     --gIndenter;
//   }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrRepeatEnding& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const msrRepeatEnding& elt)
{
  elt.print (os);

  return os;
}

//______________________________________________________________________________
S_msrRepeat msrRepeat::create (
  const mfInputLineNumber& inputLineNumber,
  int                      repeatTimes)
{
  msrRepeat* obj =
    new msrRepeat (
      inputLineNumber,
      repeatTimes);
  assert (obj != nullptr);
  return obj;

  // create the repeat common part
  S_msrRepeatCommonPart
    repeatCommonPart =
      msrRepeatCommonPart::create (
        inputLineNumber,
        obj);

  // register it in newRepeat
  obj->setRepeatCommonPart ( // JMI 0.9.76
    repeatCommonPart);
}

S_msrRepeat msrRepeat::create (
  const mfInputLineNumber& inputLineNumber,
  int                      repeatTimes,
  const S_msrVoice&        upLinkToVoice)
{
  msrRepeat* obj =
    new msrRepeat (
      inputLineNumber,
      repeatTimes,
      upLinkToVoice);
  assert (obj != nullptr);
  return obj;

  // create the repeat common part
  S_msrRepeatCommonPart
    repeatCommonPart =
      msrRepeatCommonPart::create (
        inputLineNumber,
        obj);

  // register it in newRepeat
  obj->setRepeatCommonPart ( // JMI 0.9.76
    repeatCommonPart);
}

msrRepeat::msrRepeat (
  const mfInputLineNumber& inputLineNumber,
  int                      repeatTimes)
//     : msrVoiceElement (inputLineNumber)
    : msrSegmentElement (inputLineNumber)
{
  initializeRepeat (
    inputLineNumber,
    repeatTimes,
    nullptr); // fRepeatUpLinkToVoice will be set later
}

msrRepeat::msrRepeat (
  const mfInputLineNumber& inputLineNumber,
  int                      repeatTimes,
  const S_msrVoice&        upLinkToVoice)
//     : msrVoiceElement (inputLineNumber)
    : msrSegmentElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    upLinkToVoice != nullptr,
    "upLinkToVoice is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  initializeRepeat (
    inputLineNumber,
    repeatTimes,
    upLinkToVoice);
}

void msrRepeat::initializeRepeat (
  const mfInputLineNumber& inputLineNumber,
  int                      repeatTimes,
  const S_msrVoice&        upLinkToVoice)
{
  fRepeatTimes = repeatTimes;

  fRepeatExplicitStartKind =
    msrRepeatExplicitStartKind::kRepeatExplicitStartUnknown; // default value

  fRepeatEndingsInternalCounter = 0;

  // repeat build phase
  fCurrentRepeatBuildPhaseKind =
    msrRepeatBuildPhaseKind::kRepeatBuildPhaseJustCreated;

  // set uplink to voice
  fRepeatUpLinkToVoice = upLinkToVoice;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Initializing repeat" <<
      ", fRepeatTimes: " << fRepeatTimes <<
//       ", fRepeatCommonPart: " << fRepeatCommonPart->asString () << // CANNOT RUN, HAVOC
//       ", repeatCommonPart: " << repeatCommonPart->asString () << // CANNOT RUN, HAVOC
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

S_msrRepeat msrRepeat::createRepeatNewbornClone (
  const S_msrVoice& containingVoice)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    containingVoice != nullptr,
    "containingVoice is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of repeat " <<
      asShortString () <<
      " in voice " <<
      containingVoice->asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrRepeat
    newbornClone =
      msrRepeat::create (
        fInputLineNumber,
        fRepeatTimes,
        containingVoice);

    // number of repetitions
    newbornClone->fRepeatTimes =
      fRepeatTimes;

    // explicit start?
    newbornClone->fRepeatExplicitStartKind =
      fRepeatExplicitStartKind;

  // DON'T create the repeat common part, that will be done upon browsing

  return newbornClone;
}

msrRepeat::~msrRepeat ()
{}

void msrRepeat::setRepeatCommonPart (
  const S_msrRepeatCommonPart& repeatCommonPart)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    repeatCommonPart != nullptr,
    "repeatCommonPart is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Setting repeat common part to " <<
      repeatCommonPart->asShortString () <<
      " in repeat " <<
      asShortString (); // CANNOT RUN HAVOC ???

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fRepeatCommonPart = repeatCommonPart;

//   // set repeat's build phase
//   fCurrentRepeatBuildPhaseKind =
//     msrRepeatBuildPhaseKind::kRepeatBuildPhaseInCommonPart;
}

void msrRepeat::addRepeatEndingToRepeat (
  const mfInputLineNumber& inputLineNumber,
  const S_msrRepeatEnding& repeatEnding)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    repeatEnding != nullptr,
    "repeatEnding is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Adding ending " <<
      repeatEnding->asString () <<
      " to repeat" <<
      asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayRepeat (
      inputLineNumber,
      "addRepeatEndingToRepeat() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  // get repeat ending kind
  msrRepeatEndingKind
    repeatEndingKind =
      repeatEnding->
        getRepeatEndingKind ();

  // consistency check
  switch (repeatEndingKind) {
    case msrRepeatEndingKind::kRepeatEndingHooked:
      switch (fCurrentRepeatBuildPhaseKind) {
        case msrRepeatBuildPhaseKind::kRepeatBuildPhaseJustCreated:
          {
            std::stringstream ss;

            ss <<
              "cannot add hooked ending " <<
              repeatEnding->asShortString () <<
              " right after a repeat has been created";

            msrError (
              gServiceRunData->getInputSourceName (),
              fInputLineNumber,
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
          break;
        case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInCommonPart:
          fCurrentRepeatBuildPhaseKind =
            msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings;
          break;
        case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings:
          // there can be several successive endings
          break;
        case msrRepeatBuildPhaseKind::kRepeatBuildPhaseCompleted:
          {
            std::stringstream ss;

            ss <<
              "cannot add hooked ending " <<
              repeatEnding->asShortString () <<
              " after a repeat has been completed";

            msrError (
              gServiceRunData->getInputSourceName (),
              fInputLineNumber,
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
          break;
      } // switch
      break;

    case msrRepeatEndingKind::kRepeatEndingHookless:
      switch (fCurrentRepeatBuildPhaseKind) {
        case msrRepeatBuildPhaseKind::kRepeatBuildPhaseJustCreated:
          {
            std::stringstream ss;

            ss <<
              "cannot add hookless repeat ending " <<
              repeatEnding->asShortString () <<
              " right after the repeat has been created";

            msrInternalError (
              gServiceRunData->getInputSourceName (),
              fInputLineNumber,
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
          break;
        case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInCommonPart:
          {
            std::stringstream ss;

            ss <<
              "cannot add hookless repeat ending " <<
              repeatEnding->asShortString () <<
              " right after the repeat common part";

//             msrInternalError ( JMI 0.9.63
            msrInternalWarning (
              gServiceRunData->getInputSourceName (),
              fInputLineNumber,
//               __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
          break;
        case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings:
          // there can be several successive endings
          break;
        case msrRepeatBuildPhaseKind::kRepeatBuildPhaseCompleted:
          {
            std::stringstream ss;

            ss <<
              "cannot add hookless ending " <<
              repeatEnding->asShortString () <<
              " after a repeat has been completed";

            msrError (
              gServiceRunData->getInputSourceName (),
              fInputLineNumber,
              __FILE__, mfInputLineNumber (__LINE__),
              ss.str ());
          }
      } // switch
      break;
  } // switch

  fRepeatEndingsVector.push_back (repeatEnding);

  // set repeat ending internal number
  repeatEnding->
    setRepeatEndingInternalNumber (
      ++ fRepeatEndingsInternalCounter);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayRepeat (
      inputLineNumber,
      "addRepeatEndingToRepeat() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}

// void msrRepeat::appendSegmentToRepeat (
//   const mfInputLineNumber& inputLineNumber,
//   const S_msrSegment&      segment,
//   const std::string&       context)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending segment " <<
//       segment->asString () <<
//       " to repeat " <<
//       asShortString () <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsDetails ()) {
//     displayRepeat (
//       inputLineNumber,
//       "appendSegmentToRepeat() 1");
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   switch (fCurrentRepeatBuildPhaseKind) {
//     case msrRepeatBuildPhaseKind::kRepeatBuildPhaseJustCreated:
//       {
//         std::stringstream ss;
//
//         ss <<
//           "segment " <<
//           segment->asShortString () <<
//           "'cannot be added to a just created repeat" <<
//           " (" << context << ")";
//
//         msrError (
//           gServiceRunData->getInputSourceName (),
//           inputLineNumber,
//           __FILE__, mfInputLineNumber (__LINE__),
//           ss.str ());
//       }
//       break;
//
//     case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInCommonPart:
//         fRepeatCommonPart->
//           appendSegmentToRepeatCommonPart (
//             inputLineNumber,
//             segment,
//             context);
//       break;
//
//     case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings:
//       fRepeatEndingsVector.back ()->
//         appendSegmentToRepeatEnding (
//           inputLineNumber,
//           segment,
//           context);
//       break;
//
//     case msrRepeatBuildPhaseKind::kRepeatBuildPhaseCompleted:
//       {
//         std::stringstream ss;
//
//         ss <<
//           "segment " <<
//           segment->asShortString () <<
//           "'cannot be added to a completed repeat" <<
//           '(' << context << ")";
//
//         msrError (
//           gServiceRunData->getInputSourceName (),
//           inputLineNumber,
//           __FILE__, mfInputLineNumber (__LINE__),
//           ss.str ());
//       }
//       break;
//   } // switch
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceRepeatsDetails ()) {
//     displayRepeat (
//       inputLineNumber,
//       "appendSegmentToRepeat() 2");
//   }
// #endif // MF_TRACE_IS_ENABLED
// }

void msrRepeat::appendRepeatToRepeat (
  const mfInputLineNumber& inputLineNumber,
  const S_msrRepeat&       repeat,
  const std::string&       context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending repeat " <<
      repeat->asString () <<
      " to repeat " <<
      asShortString () <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayRepeat (
      inputLineNumber,
      "appendRepeatToRepeat() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fCurrentRepeatBuildPhaseKind) {
    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseJustCreated:
      {
        std::stringstream ss;

        ss <<
          "repeat " <<
          repeat->asShortString () <<
          "'cannot be added to a just created repeat" <<
          " (" << context << ")";

        msrError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;

    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInCommonPart:
        fRepeatCommonPart->
          appendRepeatToRepeatCommonPart (
            inputLineNumber,
            repeat,
            context);
      break;

    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings:
      fRepeatEndingsVector.back ()->
        appendRepeatToRepeatEnding (
          inputLineNumber,
          repeat,
          context);
      break;

    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseCompleted:
      {
        std::stringstream ss;

        ss <<
          "repeat " <<
          repeat->asShortString () <<
          "'cannot be added to a completed repeat" <<
          '(' << context << ")";

        msrError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayRepeat (
      inputLineNumber,
      "appendRepeatToRepeat() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrRepeat::appendMeasureRepeatToRepeat (
  const mfInputLineNumber&  inputLineNumber,
  const S_msrMeasureRepeat& measureRepeat,
  const std::string&        context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending measure repeat " <<
      measureRepeat->asString () <<
      " to repeat " <<
      asShortString () <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayRepeat (
      inputLineNumber,
      "appendMeasureRepeatToRepeat() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fCurrentRepeatBuildPhaseKind) {
    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseJustCreated:
      {
        std::stringstream ss;

        ss <<
          "measures repeat " <<
          measureRepeat->asShortString () <<
          "'cannot be added to a just created repeat" <<
          " (" << context << ")";

        msrError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;

    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInCommonPart:
        fRepeatCommonPart->
          appendMeasureRepeatToRepeatCommonPart (
            inputLineNumber,
            measureRepeat,
            context);
      break;

    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings:
      fRepeatEndingsVector.back ()->
        appendMeasureRepeatToRepeatEnding (
          inputLineNumber,
          measureRepeat,
          context);
      break;

    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseCompleted:
      {
        std::stringstream ss;

        ss <<
          "measures repeat " <<
          measureRepeat->asShortString () <<
          "'cannot be added to a completed repeat" <<
          '(' << context << ")";

        msrError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayRepeat (
      inputLineNumber,
      "appendMeasureRepeatToRepeat() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrRepeat::appendMultipleMeasureRestToRepeat (
  const mfInputLineNumber&        inputLineNumber,
  const S_msrMultipleMeasureRest& multipleMeasureRests,
  const std::string&              context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Cascading appending multiple measure rest " <<
      multipleMeasureRests->asString () <<
      " to repeat " <<
      asShortString () <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayRepeat (
      inputLineNumber,
      "appendMultipleMeasureRestToRepeat() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fCurrentRepeatBuildPhaseKind) {
    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseJustCreated:
      {
        std::stringstream ss;

        ss <<
          "multiple measure rests " <<
          multipleMeasureRests->asShortString () <<
          "'cannot be added to a just created repeat" <<
          " (" << context << ")";

        msrError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;

    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInCommonPart:
      fRepeatCommonPart->
        appendMultipleMeasureRestToRepeatCommonPart (
          inputLineNumber,
          multipleMeasureRests,
          context);
      break;

    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings:
      fRepeatEndingsVector.back ()->
        appendMultipleMeasureRestToRepeatEnding (
          inputLineNumber,
          multipleMeasureRests,
          context);
      break;

    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseCompleted:
      {
        std::stringstream ss;

        ss <<
          "multiple measure rests " <<
          multipleMeasureRests->asShortString () <<
          "'cannot be added to a completed repeat" <<
          '(' << context << ")";

        msrError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayRepeat (
      inputLineNumber,
      "appendappendMultipleMeasureRestToRepeat() 2");
  }
#endif // MF_TRACE_IS_ENABLED
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
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrRepeat::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrRepeat::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrRepeat::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrRepeat::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrRepeat::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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

  // browse the repeat endings if any
  for (S_msrRepeatEnding repeatEnding : fRepeatEndingsVector) {
    // browse the repeat ending
    msrBrowser<msrRepeatEnding> browser (v);
    browser.browse (*repeatEnding);
  } // for
}

std::string msrRepeat::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[Repeat" <<
    ", fRepeatTimes: " << fRepeatTimes <<
   ", fRepeatExplicitStartKind: " <<
    msrRepeatExplicitStartKindAsString (
      fRepeatExplicitStartKind);
//     ", fCurrentRepeatBuildPhaseKind: " <<
//     repeatBuildPhaseKindAsString (
//       fCurrentRepeatBuildPhaseKind);

  if (fImmediatelyPrecedingRepeat && fImmediatelyPrecedingRepeat != this) { // JMI 0.9.71 loop removal
    ss <<
      ", fImmediatelyPrecedingRepeat: " <<
      fImmediatelyPrecedingRepeat->asShortString ();
  }
/*
  don't print the following repeat if any,
  to avoid an infinite loop
  if (fImmediatelyFollowingRepeat) {
    ss <<
      ", fImmediatelyFollowingRepeat: " <<
      fImmediatelyFollowingRepeat->asShortString ();
  }
*/

//   ss <<
//     ", fRepeatCommonPart: " <<
//     fetchRepeatCommonPartAsShortString (fRepeatCommonPart);

  int repeatEndingsNumber =
    fRepeatEndingsVector.size ();

  ss <<
    ", repeatEndingsNumber: " << repeatEndingsNumber <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

std::string msrRepeat::asString () const
{
  std::stringstream ss;

  ss <<
    "[Repeat" <<
    ", fRepeatTimes: " << fRepeatTimes <<
   ", fRepeatExplicitStartKind: " <<
    msrRepeatExplicitStartKindAsString (
      fRepeatExplicitStartKind);
//     ", fCurrentRepeatBuildPhaseKind: " <<
//     repeatBuildPhaseKindAsString (
//       fCurrentRepeatBuildPhaseKind);

  if (fImmediatelyPrecedingRepeat) {
    ss <<
      ", fImmediatelyPrecedingRepeat: " <<
      fImmediatelyPrecedingRepeat->asShortString ();
  }
  if (fImmediatelyFollowingRepeat) {
    ss <<
      ", fImmediatelyFollowingRepeat: " <<
      fImmediatelyFollowingRepeat->asShortString ();
  }

  ss <<
    ", fRepeatCommonPart: " <<
      fetchRepeatCommonPartAsShortString (fRepeatCommonPart);

  int endingsNumber =
    fRepeatEndingsVector.size ();

  ss <<
    ", endingsNumber: " << endingsNumber;

  if (endingsNumber) {
    std::vector <S_msrRepeatEnding>::const_iterator
      iBegin = fRepeatEndingsVector.begin (),
      iEnd   = fRepeatEndingsVector.end (),
      i      = iBegin;

    ss <<
      ", ";

    for ( ; ; ) {
      // print the repeat ending
      ss << (*i)->asShortString ();
      if (++i == iEnd) break;
      ss << ' ';
    } // for
  }

  ss <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrRepeat::displayRepeat (
  const mfInputLineNumber& inputLineNumber,
  const std::string&       context)
{
  gLog <<
    std::endl <<
    "*********>> Repeat " <<
    asShortString () <<
    " (" << context << ")" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    std::endl;

  ++gIndenter;
  print (gLog);
  --gIndenter;

  gLog <<
    " <<*********" <<
    std::endl << std::endl;
}

void msrRepeat::print (std::ostream& os) const
{
  os <<
    "[Repeat" <<
    ", fRepeatTimes: " << fRepeatTimes <<
    ", fRepeatExplicitStartKind: " <<
    fRepeatExplicitStartKind <<
//     ", fCurrentRepeatBuildPhaseKind: " <<
//     repeatBuildPhaseKindAsString (
//       fCurrentRepeatBuildPhaseKind);
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 18;

  // print the repeat common part
  os <<
    std::setw (fieldWidth) <<
    "fRepeatCommonPart" << ": " <<
    fetchRepeatCommonPartAsShortString (fRepeatCommonPart) <<
    std::endl;

  // print the repeat endings
  int repeatEndingsSize =
    fRepeatEndingsVector.size ();

  os << std::left <<
    std::setw (fieldWidth) <<
    "fRepeatEndingsVector" << ": ";
  if (repeatEndingsSize) {
    os << '(' << repeatEndingsSize << ")";
  }
  else {
    os << "[EMPTY]";
  }
  os << std::endl;

  if (repeatEndingsSize) {
    ++gIndenter;

    std::vector <S_msrRepeatEnding>::const_iterator
      iBegin = fRepeatEndingsVector.begin (),
      iEnd   = fRepeatEndingsVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the repeat ending
      os << (*i);
      if (++i == iEnd) break;
  // JMI    os << std::endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrRepeat::printFull (std::ostream& os) const
{
  os <<
    "[Repeat" <<
    ", fRepeatTimes: " << fRepeatTimes <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 28;

  os << std::left <<
    std::setw (fieldWidth) <<
   "fRepeatExplicitStartKind: " <<
    msrRepeatExplicitStartKindAsString (
      fRepeatExplicitStartKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "repeat ending(s)" << ": " <<
    fRepeatEndingsVector.size () <<
    std::endl;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
//     // print the current repeat build phase
//     os << std::left <<
//       std::setw (fieldWidth) <<
//       "fCurrentRepeatBuildPhaseKind" << ": " <<
//       repeatBuildPhaseKindAsString (
//         fCurrentRepeatBuildPhaseKind) <<
//       std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  os << std::endl;

  // print the immediately preceding and following repeats
  os << std::left <<
    std::setw (fieldWidth) <<
    "fImmediatelyPrecedingRepeat" << ": " <<
      fetchRepeatAsShortString (fImmediatelyPrecedingRepeat) <<
    std::endl << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fImmediatelyFollowingRepeat" << ": " <<
      fetchRepeatAsShortString (fImmediatelyFollowingRepeat) <<
    std::endl << std::endl;

  // print the repeat common part
  os << std::left <<
    std::setw (fieldWidth) <<
    "fRepeatCommonPart" << ": " <<
      fetchRepeatCommonPartAsShortString (fRepeatCommonPart) <<
    std::endl << std::endl;

  // print the repeat endings
  int endingsNumber =
    fRepeatEndingsVector.size ();

  os << std::left <<
    std::setw (fieldWidth) <<
    "fRepeatEndingsVector" << ": ";
  if (endingsNumber > 0) {
    os << '(' << endingsNumber << ")";
  }
  else {
    os << "[EMPTY]";
  }
  os << std::endl;

  if (endingsNumber) {
    ++gIndenter;

    std::vector <S_msrRepeatEnding>::const_iterator
      iBegin = fRepeatEndingsVector.begin (),
      iEnd   = fRepeatEndingsVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the repeat ending
      os << (*i);
      if (++i == iEnd) break;
  // JMI    os << std::endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrRepeat& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}

std::ostream& operator << (std::ostream& os, const msrRepeat& elt)
{
  elt.print (os);

  return os;
}

std::string fetchRepeatAsShortString (const S_msrRepeat& repeat)
{
  std::string result;

  if (repeat) {
    result =
      repeat->asShortString ();
  }
  else {
    result = "\"** REPEAT IS NULL **\"";
  }

  return result;
}

std::string fetchRepeatAsString (const S_msrRepeat& repeat)
{
  std::string result;

  if (repeat) {
    result =
      repeat->asString ();
  }
  else {
    result = "\"** REPEAT IS NULL **\"";
  }

  return result;
}



std::string fetchRepeatCommonPartAsShortString (
  const S_msrRepeatCommonPart& repeatCommonPart)
{
  std::string result;

  if (repeatCommonPart) {
    result =
      repeatCommonPart->asShortString ();
  }
  else {
    result = "\"** REPEAT_COMMON_PART IS NULL **\"";
  }

  return result;
}

std::string fetchepeatCommonPartAsString (
  const S_msrRepeatCommonPart& repeatCommonPart)
{
  std::string result;

  if (repeatCommonPart) {
    result =
      repeatCommonPart->asShortString ();
  }
  else {
    result = "\"** REPEAT_COMMON_PART IS NULL **\"";
  }

  return result;
}



std::string fetchRepeatEndingAsShortString (const S_msrRepeatEnding& repeatEnding)
{
  std::string result;

  if (repeatEnding) {
    result =
      repeatEnding->asString ();
  }
  else {
    result = "\"** REPEAT_ENDING IS NULL **\"";
  }

  return result;
}

std::string fetchRepeatEndingAsString (const S_msrRepeatEnding& repeatEnding)
{
  std::string result;

  if (repeatEnding) {
    result =
      repeatEnding->asShortString ();
  }
  else {
    result = "\"** REPEAT_ENDING IS NULL **\"";
  }

  return result;
}




}

