/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

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

#include "msrRepeatsEnumTypes.h"
#include "msrRepeats.h"
#include "msrTuplets.h"

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
  const S_msrRepeat& upLinkToRepeat)
{
  msrRepeatCommonPart* obj =
    new msrRepeatCommonPart (
      inputLineNumber,
      upLinkToRepeat);
  assert (obj != nullptr);
  return obj;
}

msrRepeatCommonPart::msrRepeatCommonPart (
  int         inputLineNumber,
  const S_msrRepeat& upLinkToRepeat)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToRepeat != nullptr,
    "upLinkToRepeat is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fRepeatCommonPartUpLinkToRepeat = upLinkToRepeat;
}

msrRepeatCommonPart::~msrRepeatCommonPart ()
{}

void msrRepeatCommonPart::appendSegmentToRepeatCommonPart (
  int           inputLineNumber,
  const S_msrSegment&  segment,
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSegments ()) {
    std::stringstream ss;

    ss <<
      "Appending segment '" <<
      segment->asString () <<
      "' to repeat common part '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    segment != nullptr,
    "segment is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fRepeatCommonPartElementsList.push_back (segment);
}

void msrRepeatCommonPart::appendRepeatToRepeatCommonPart (
  int           inputLineNumber,
  const S_msrRepeat& repeat,
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending repeat '" <<
    // JMI  repeat->asString () <<
      repeat <<
      "' to repeat common part '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    repeat != nullptr,
    "repeat is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fRepeatCommonPartElementsList.push_back (repeat);
}

void msrRepeatCommonPart::appendMeasureRepeatToRepeatCommonPart (
  int                inputLineNumber,
  const S_msrMeasureRepeat& measureRepeat,
  const std::string&      context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasureRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending measures repeat '" <<
      measureRepeat->asString () <<
      "' to repeat common part '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeat != nullptr,
    "measureRepeat is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

// JMI v0.9.67  fRepeatCommonPartElementsList.push_back (measureRepeat); JMI v0.9.66
}

void msrRepeatCommonPart::appendMultiMeasureRestToRepeatCommonPart (
  int                   inputLineNumber,
  const S_msrMultiMeasureRest& multiMeasureRests,
  const std::string&    context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMultiMeasureRests ()) {
    std::stringstream ss;

    ss <<
      "Appending multi-measure rest " <<
      multiMeasureRests->asString () <<
      " to repeat common part " << asString () <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    multiMeasureRests != nullptr,
    "multiMeasureRests is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

//  JMI v0.9.67 fRepeatCommonPartElementsList.push_back (multiMeasureRests);
}

void msrRepeatCommonPart::appendVoiceElementToRepeatCommonPart (
  int               inputLineNumber,
  const S_msrVoiceElement& voiceElement,
  const std::string&     context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending voice element '" <<
      voiceElement->asString () <<
      "' to repeat common part '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voiceElement != nullptr,
    "voiceElement is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

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
    std::list <S_msrVoiceElement>::const_iterator
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
        std::stringstream ss;

        ss <<
          "tuplet first element should be a note, a chord or another tuplet, found instead '" <<
          element->asShortString () <<
          "'";

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          fInputStartLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }

      if (++i == iEnd) break;
    } // for
  }

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
      __FILE__, __LINE__,
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
            __FILE__, __LINE__,
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
      __FILE__, __LINE__,
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
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrRepeatCommonPart::browseData (basevisitor* v)
{
  // browse the elements
  if (fRepeatCommonPartElementsList.size ()) {
    for (
      std::list <S_msrVoiceElement>::const_iterator i = fRepeatCommonPartElementsList.begin ();
      i != fRepeatCommonPartElementsList.end ();
      ++i
  ) {
      // browse the element
      msrBrowser<msrVoiceElement> browser (v);
      browser.browse (*(*i));
    } // for
  }
}

std::string msrRepeatCommonPart::asString () const
{
  std::stringstream ss;

  ss <<
    "[RepeatCommonPart" <<
    ", repeat upLink: '" <<
    fRepeatCommonPartUpLinkToRepeat->
      asShortString () <<
    "', line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

void msrRepeatCommonPart::printFull (std::ostream& os) const
{
  os <<
    "[RepeatCommonPart" <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  os <<
    "fRepeatCommonPartUpLinkToRepeat: '" <<
    fRepeatCommonPartUpLinkToRepeat->
      asShortString () <<
      '\'' <<
    std::endl << std::endl;

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
    os << "[EMPTY]";
  }
  os << std::endl;

  if (repeatCommonPartElementsListSize) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrVoiceElement>::const_iterator
      iBegin = fRepeatCommonPartElementsList.begin (),
      iEnd   = fRepeatCommonPartElementsList.end (),
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

  os << ']' << std::endl;
}

void msrRepeatCommonPart::print (std::ostream& os) const
{
  os <<
    "[RepeatCommonPart" <<
//     ", repeat upLink: '" <<
//     fRepeatCommonPartUpLinkToRepeat->
//       asShortString () <<
    "', line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

/* JMI
  os <<
    "repeat upLink: '" <<
    fRepeatCommonPartUpLinkToRepeat->
      asShortString () <<
      '\'' <<
    std::endl;
*/

  // print the elements
  int repeatCommonPartElementsListSize =
    fRepeatCommonPartElementsList.size ();

  os <<
    "repeatCommonPartElementsListSize: ";
  if (repeatCommonPartElementsListSize) {
    os <<
      '(' <<
      mfSingularOrPlural (
        repeatCommonPartElementsListSize, "element", "elements") <<
      ")";
  }
  else {
    os << "[EMPTY]";
  }
  os << std::endl;

  if (repeatCommonPartElementsListSize) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrVoiceElement>::const_iterator
      iBegin = fRepeatCommonPartElementsList.begin (),
      iEnd   = fRepeatCommonPartElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // short print the element
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

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

//______________________________________________________________________________
S_msrRepeatEnding msrRepeatEnding::create (
  int                 inputLineNumber,
  const std::string&  repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind repeatEndingKind,
  const S_msrRepeat&  upLinkToRepeat)
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

msrRepeatEnding::msrRepeatEnding (
  int                 inputLineNumber,
  const std::string&  repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind repeatEndingKind,
  const S_msrRepeat&  upLinkToRepeat)
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
  const S_msrSegment&  segment,
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Appending segment '" << segment <<
      "' to repeat ending '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    segment != nullptr,
    "segment is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fRepeatEndingElementsList.push_back (segment);
}

void msrRepeatEnding::appendRepeatToRepeatEnding (
  int           inputLineNumber,
  const S_msrRepeat& repeat,
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Appending repeat '" << repeat <<
      "' to repeat ending '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    repeat != nullptr,
    "repeat is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fRepeatEndingElementsList.push_back (repeat);
}

void msrRepeatEnding::appendMeasureRepeatToRepeatEnding (
  int                inputLineNumber,
  const S_msrMeasureRepeat& measureRepeat,
  const std::string&      context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Appending measures repeat '" <<
      measureRepeat->asShortString () <<
      "' to repeat ending '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measureRepeat != nullptr,
    "measureRepeat is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

// JMI v0.9.67  fRepeatEndingElementsList.push_back (measureRepeat);
}

void msrRepeatEnding::appendMultiMeasureRestToRepeatEnding (
  int                   inputLineNumber,
  const S_msrMultiMeasureRest& multiMeasureRests,
  const std::string&    context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Appending multi-measure rest " <<
      multiMeasureRests->asShortString () <<
      " to repeat ending " << asString () <<
      " (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    multiMeasureRests != nullptr,
    "multiMeasureRests is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

//  JMI v0.9.67 fRepeatEndingElementsList.push_back (multiMeasureRests);
}

void msrRepeatEnding::appendVoiceElementToRepeatEnding (
  int               inputLineNumber,
  const S_msrVoiceElement& voiceElement,
  const std::string&     context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceVoices ()) {
    std::stringstream ss;

    ss <<
      "Appending voice element '" << voiceElement <<
      "' to repeat ending '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    voiceElement != nullptr,
    "voiceElement is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fRepeatEndingElementsList.push_back (voiceElement);
}

void msrRepeatEnding::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrRepeatEnding::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
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
            __FILE__, __LINE__,
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
      __FILE__, __LINE__,
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
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrRepeatEnding::browseData (basevisitor* v)
{
  // browse the elements
  if (fRepeatEndingElementsList.size ()) {
    for (
      std::list <S_msrVoiceElement>::const_iterator i = fRepeatEndingElementsList.begin ();
      i != fRepeatEndingElementsList.end ();
      ++i
  ) {
      // browse the element
      msrBrowser<msrVoiceElement> browser (v);
      browser.browse (*(*i));
    } // for
  }
}

std::string msrRepeatEnding::asString () const
{
  std::stringstream ss;

  ss <<
    "[RepeatEnding" <<
    ", " <<
    fRepeatEndingKind <<
    ", RepeatEndingUpLinkToRepeat: '";

//   if (fRepeatEndingUpLinkToRepeat) { JMI v0.9.64
//     ss <<
//       fRepeatEndingUpLinkToRepeat->
//         asShortString ();
//   }
//   else {
//     ss <<
//       "[NULL]";
//   }

  ss <<
    ", repeatEndingNumber: " << fRepeatEndingNumber <<
    ", repeatEndingInternalNumber: " << fRepeatEndingInternalNumber <<
    "', line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

void msrRepeatEnding::printFull (std::ostream& os) const
{
  os <<
    std::endl <<
    "[RepeatEnding" <<
    ", repeatEndingKind" << ": " <<
    msrRepeatEndingKindAsString (
      fRepeatEndingKind) <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

/* JMI
  os <<
    "repeat upLink: '" <<
    fRepeatEndingUpLinkToRepeat->
      asShortString () <<
      '\'' <<
    std::endl;
*/

  const int fieldWidth = 27;

  os << std::left <<
    std::setw (fieldWidth) <<
    "repeatEndingNumber" <<  ": " <<fRepeatEndingNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "repeatEndingInternalNumber" <<  ": " <<fRepeatEndingInternalNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "repeat upLink" << " : '" <<
    fRepeatEndingUpLinkToRepeat->
      asShortString () <<
    '\'' <<
    std::endl << std::endl;

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
      (*i)->print (os);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrRepeatEnding::print (std::ostream& os) const
{
  os <<
    "[RepeatEnding" <<
    ", fRepeatEndingKind: " <<
    fRepeatEndingKind <<
    ", repeatEndingNumber: " << fRepeatEndingNumber <<
    ", repeatEndingInternalNumber: " << fRepeatEndingInternalNumber <<
    "', line " << fInputStartLineNumber <<
    std::endl;

/*
  ++gIndenter;

  const int fieldWidth = 27;

  os << std::left <<
    std::setw (fieldWidth) <<
    "repeatEndingNumber" <<  ": " <<fRepeatEndingNumber <<
    / * JMI
    std::endl <<
    std::setw (fieldWidth) <<
    "repeatEndingInternalNumber" <<  ": " <<fRepeatEndingInternalNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "repeat upLink: " << " : '" <<
    fRepeatEndingUpLinkToRepeat->
      asShortString () <<
    '\'' <<
    std::endl <<
    * /
    std::endl;

/ * JMI
  os <<
    "repeat upLink: '" <<
    fRepeatEndingUpLinkToRepeat->
      asShortString () <<
      '\'' <<
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
    os << "[NONE]";
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

//______________________________________________________________________________
S_msrRepeat msrRepeat::create (
  int               inputLineNumber,
  int               repeatTimes,
  const S_msrVoice& upLinkToVoice)
{
  msrRepeat* obj =
    new msrRepeat (
      inputLineNumber,
      repeatTimes,
      upLinkToVoice);
  assert (obj != nullptr);
  return obj;
}

msrRepeat::msrRepeat (
  int               inputLineNumber,
  int               repeatTimes,
  const S_msrVoice& upLinkToVoice)
    : msrVoiceElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToVoice != nullptr,
    "upLinkToVoice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fRepeatUpLinkToVoice = upLinkToVoice;

  fRepeatTimes = repeatTimes;

  fRepeatExplicitStartKind =
    msrRepeatExplicitStartKind::kRepeatExplicitStartUnknown; // default value

  fRepeatEndingsInternalCounter = 0;

  // repeat build phase
  fCurrentRepeatBuildPhaseKind =
    msrRepeatBuildPhaseKind::kRepeatBuildPhaseJustCreated;
}

S_msrRepeat msrRepeat::createRepeatNewbornClone (
  const S_msrVoice& containingVoice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of a repeat";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrRepeat
    newbornClone =
      msrRepeat::create (
        fInputStartLineNumber,
        fRepeatTimes,
        containingVoice);

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
    __FILE__, __LINE__,
    repeatCommonPart != nullptr,
    "repeatCommonPart is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Setting repeat common part containing " <<
      mfSingularOrPlural (
        repeatCommonPart->
          getRepeatCommonPartElementsList ().size (),
        "element",
        "elements");

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fRepeatCommonPart = repeatCommonPart;

  // set repeat's build phase
  fCurrentRepeatBuildPhaseKind =
    msrRepeatBuildPhaseKind::kRepeatBuildPhaseInCommonPart;
}

void msrRepeat::addRepeatEndingToRepeat (
  int               inputLineNumber,
  const S_msrRepeatEnding& repeatEnding)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    repeatEnding != nullptr,
    "repeatEnding is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Adding ending '" <<
      repeatEnding->asString () <<
      "' to repeat" <<
      "' to repeat '" <<
      asShortString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
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
              "cannot add hooked ending '" <<
              repeatEnding->asShortString () <<
              "' right after a repeat has been created";

            msrError (
              gServiceRunData->getInputSourceName (),
              fInputStartLineNumber,
              __FILE__, __LINE__,
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
              "cannot add hooked ending '" <<
              repeatEnding->asShortString () <<
              "' after a repeat has been completed";

            msrError (
              gServiceRunData->getInputSourceName (),
              fInputStartLineNumber,
              __FILE__, __LINE__,
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
              fInputStartLineNumber,
              __FILE__, __LINE__,
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

//             msrInternalError ( JMI v0.9.63
            msrInternalWarning (
              gServiceRunData->getInputSourceName (),
              fInputStartLineNumber,
//               __FILE__, __LINE__,
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
              "cannot add hookless ending '" <<
              repeatEnding->asShortString () <<
              "' after a repeat has been completed";

            msrError (
              gServiceRunData->getInputSourceName (),
              fInputStartLineNumber,
              __FILE__, __LINE__,
              ss.str ());
          }
      } // switch
      break;
  } // switch

  fRepeatEndings.push_back (repeatEnding);

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

void msrRepeat::appendSegmentToRepeat (
  int           inputLineNumber,
  const S_msrSegment&  segment,
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending segment '" <<
      segment->asString () <<
      "' to repeat '" <<
      asShortString () <<
      '\'' <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayRepeat (
      inputLineNumber,
      "appendSegmentToRepeat() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fCurrentRepeatBuildPhaseKind) {
    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseJustCreated:
      {
        std::stringstream ss;

        ss <<
          "segment '" <<
          segment->asShortString () <<
          "'cannot be added to a just created repeat" <<
          " (" << context << ")";

        msrError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }
      break;

    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInCommonPart:
        fRepeatCommonPart->
          appendSegmentToRepeatCommonPart (
            inputLineNumber,
            segment,
            context);
      break;

    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings:
      fRepeatEndings.back ()->
        appendSegmentToRepeatEnding (
          inputLineNumber,
          segment,
          context);
      break;

    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseCompleted:
      {
        std::stringstream ss;

        ss <<
          "segment '" <<
          segment->asShortString () <<
          "'cannot be added to a completed repeat" <<
          '(' << context << ")";

        msrError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayRepeat (
      inputLineNumber,
      "appendSegmentToRepeat() 2");
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrRepeat::appendRepeatToRepeat (
  int           inputLineNumber,
  const S_msrRepeat& repeat,
  const std::string& context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending repeat '" <<
      repeat->asString () <<
      "' to repeat '" <<
      asShortString () <<
      '\'' <<
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
          "repeat '" <<
          repeat->asShortString () <<
          "'cannot be added to a just created repeat" <<
          " (" << context << ")";

        msrError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
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
      fRepeatEndings.back ()->
        appendRepeatToRepeatEnding (
          inputLineNumber,
          repeat,
          context);
      break;

    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseCompleted:
      {
        std::stringstream ss;

        ss <<
          "repeat '" <<
          repeat->asShortString () <<
          "'cannot be added to a completed repeat" <<
          '(' << context << ")";

        msrError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
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
  int                inputLineNumber,
  const S_msrMeasureRepeat& measureRepeat,
  const std::string&      context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending measures repeat '" <<
      measureRepeat->asString () <<
      "' to repeat '" <<
      asShortString () <<
      '\'' <<
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
          "measures repeat '" <<
          measureRepeat->asShortString () <<
          "'cannot be added to a just created repeat" <<
          " (" << context << ")";

        msrError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
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
      fRepeatEndings.back ()->
        appendMeasureRepeatToRepeatEnding (
          inputLineNumber,
          measureRepeat,
          context);
      break;

    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseCompleted:
      {
        std::stringstream ss;

        ss <<
          "measures repeat '" <<
          measureRepeat->asShortString () <<
          "'cannot be added to a completed repeat" <<
          '(' << context << ")";

        msrError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
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

void msrRepeat::appendMultiMeasureRestToRepeat (
  int                   inputLineNumber,
  const S_msrMultiMeasureRest& multiMeasureRests,
  const std::string&    context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeats ()) {
    std::stringstream ss;

    ss <<
      "Appending multi-measure rest " <<
      multiMeasureRests->asString () <<
      " to repeat " <<
      asShortString () <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayRepeat (
      inputLineNumber,
      "appendMultiMeasureRestToRepeat() 1");
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fCurrentRepeatBuildPhaseKind) {
    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseJustCreated:
      {
        std::stringstream ss;

        ss <<
          "multi-measure rests '" <<
          multiMeasureRests->asShortString () <<
          "'cannot be added to a just created repeat" <<
          " (" << context << ")";

        msrError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }
      break;

    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInCommonPart:
      fRepeatCommonPart->
        appendMultiMeasureRestToRepeatCommonPart (
          inputLineNumber,
          multiMeasureRests,
          context);
      break;

    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseInEndings:
      fRepeatEndings.back ()->
        appendMultiMeasureRestToRepeatEnding (
          inputLineNumber,
          multiMeasureRests,
          context);
      break;

    case msrRepeatBuildPhaseKind::kRepeatBuildPhaseCompleted:
      {
        std::stringstream ss;

        ss <<
          "multi-measure rests '" <<
          multiMeasureRests->asShortString () <<
          "'cannot be added to a completed repeat" <<
          '(' << context << ")";

        msrError (
          gServiceRunData->getInputSourceName (),
          inputLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }
      break;
  } // switch

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRepeatsDetails ()) {
    displayRepeat (
      inputLineNumber,
      "appendMultiMeasureRestToRepeat() 2");
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
      __FILE__, __LINE__,
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
            __FILE__, __LINE__,
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
      __FILE__, __LINE__,
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
            __FILE__, __LINE__,
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

  // browse the alternatives
  for (
    std::vector <S_msrRepeatEnding>::const_iterator i = fRepeatEndings.begin ();
    i != fRepeatEndings.end ();
    ++i
  ) {
    // browse the alternative
    msrBrowser<msrRepeatEnding> browser (v);
    browser.browse (*(*i));
  } // for
}

std::string msrRepeat::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[Repeat" <<
    ", " << fRepeatTimes << " times" <<
   ", fRepeatExplicitStartKind: " <<
    msrRepeatExplicitStartKindAsString (
      fRepeatExplicitStartKind);
//     ", fCurrentRepeatBuildPhaseKind: " <<
//     repeatBuildPhaseKindAsString (
//       fCurrentRepeatBuildPhaseKind);

  if (fImmediatelyPrecedingRepeat && fImmediatelyPrecedingRepeat != this) { // JMI v0.9.71 loop removal
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

  ss <<
    ", fRepeatCommonPart: ";
  if (fRepeatCommonPart) {
    ss <<
      mfSingularOrPlural (
        fRepeatCommonPart->
          getRepeatCommonPartElementsList ().size (),
        "element",
        "elements");
  }
  else {
    ss <<
      "[NULL]";
  }

  int repeatEndingsNumber =
    fRepeatEndings.size ();

  ss <<
    ", repeatEndingsNumber: " << repeatEndingsNumber <<
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

std::string msrRepeat::asString () const
{
  std::stringstream ss;

  ss <<
    "[Repeat" <<
    ", " << fRepeatTimes << " times" <<
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
    ", fRepeatCommonPart: ";
  if (fRepeatCommonPart) {
    ss <<
      fRepeatCommonPart->asString ();
  }
  else {
    ss <<
      "[NULL]";
  }

  int endingsNumber =
    fRepeatEndings.size ();

  ss <<
    ", endingsNumber: " << endingsNumber;

  if (endingsNumber) {
    std::vector <S_msrRepeatEnding>::const_iterator
      iBegin = fRepeatEndings.begin (),
      iEnd   = fRepeatEndings.end (),
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
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

void msrRepeat::displayRepeat (
  int           inputLineNumber,
  const std::string& context)
{
  gLog <<
    std::endl <<
    "*********>> Repeat '" <<
    asShortString () <<
    "' (" << context << ")" <<
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

void msrRepeat::printFull (std::ostream& os) const
{
  os <<
    "[Repeat" <<
    ", " << fRepeatTimes << " times" <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 28;

  os << std::left <<
    std::setw (fieldWidth) <<
   "fRepeatExplicitStartKind: " <<
    msrRepeatExplicitStartKindAsString (
      fRepeatExplicitStartKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "repeat ending(s)" << ": " <<
    fRepeatEndings.size () <<
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
    "fImmediatelyPrecedingRepeat" << ": ";
  if (fImmediatelyPrecedingRepeat) {
    os <<
      fImmediatelyPrecedingRepeat->asShortString ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fImmediatelyFollowingRepeat" << ": ";
  if (fImmediatelyFollowingRepeat) {
    os <<
      fImmediatelyFollowingRepeat->asShortString ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl << std::endl;

  // print the repeat common part
  os << std::left <<
    std::setw (fieldWidth) <<
    "fRepeatCommonPart" << ": ";
  if (fRepeatCommonPart) {
    os <<
      fRepeatCommonPart <<
      std::endl;
  }
  else {
    os << "[NULL]";
  }
  os << std::endl << std::endl;

  // print the repeat endings
  int endingsNumber =
    fRepeatEndings.size ();

  os <<
    "fRepeatEndings: ";
  if (endingsNumber > 0) {
    os << '(' << endingsNumber << ")";
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  if (endingsNumber) {
    ++gIndenter;

    std::vector <S_msrRepeatEnding>::const_iterator
      iBegin = fRepeatEndings.begin (),
      iEnd   = fRepeatEndings.end (),
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

void msrRepeat::print (std::ostream& os) const
{
  os <<
    "[Repeat" <<
    ", " << fRepeatTimes << " times" <<
    ", fRepeatExplicitStartKind: " <<
    fRepeatExplicitStartKind <<
//     ", fCurrentRepeatBuildPhaseKind: " <<
//     repeatBuildPhaseKindAsString (
//       fCurrentRepeatBuildPhaseKind);
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 18;

  // short print the repeat common part
  os <<
    std::setw (fieldWidth) <<
    "fRepeatCommonPart" << ": ";
  if (fRepeatCommonPart) {
    os << std::endl;
    ++gIndenter;
    os << fRepeatCommonPart;
    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  // short print the repeat endings
  int repeatEndingsSize =
    fRepeatEndings.size ();

  os <<
    std::setw (fieldWidth) <<
    "fRepeatEndings" << ": ";
  if (repeatEndingsSize) {
    os << '(' << repeatEndingsSize << ")";
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  if (repeatEndingsSize) {
    ++gIndenter;

    std::vector <S_msrRepeatEnding>::const_iterator
      iBegin = fRepeatEndings.begin (),
      iEnd   = fRepeatEndings.end (),
      i      = iBegin;

    for ( ; ; ) {
      // short print the repeat ending
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


}

