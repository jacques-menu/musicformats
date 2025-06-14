/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "msrBrowsers.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrLyrics.h"
#include "msrMeasureConstants.h"

#include "oahOah.h"

#include "msrOah.h"
#include "waeOah.h"

#include "msrWae.h"
#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
std::string msrSyllableElementKindAsString (
  msrSyllableElementKind syllableElementKind)
{
  std::string result;

  switch (syllableElementKind) {
    case msrSyllableElementKind::kSyllableElementText:
      result = "kSyllableElementText";
      break;

    case msrSyllableElementKind::kSyllableElementElision:
      result = "kSyllableElementElision";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrSyllableElementKind& elt)
{
  os << msrSyllableElementKindAsString (elt);
  return os;
}

//______________________________________________________________________________
std::string msrSyllableKindAsString (
  msrSyllableKind syllableKind)
{
  std::string result;

  switch (syllableKind) {
    case msrSyllableKind::kSyllableNone:
      result = "kSyllableNone";
      break;

    case msrSyllableKind::kSyllableSingle:
      result = "kSyllableSingle";
      break;

    case msrSyllableKind::kSyllableBegin:
      result = "kSyllableBegin";
      break;
    case msrSyllableKind::kSyllableMiddle:
      result = "kSyllableMiddle";
      break;
    case msrSyllableKind::kSyllableEnd:
      result = "kSyllableEnd";
      break;

    case msrSyllableKind::kSyllableOnRestNote:
      result = "kSyllableOnRestNote";
      break;

    case msrSyllableKind::kSyllableSkipOnRestNote:
      result = "kSyllableSkipOnRestNote";
      break;
    case msrSyllableKind::kSyllableSkipOnNonRestNote:
      result = "kSyllableSkipOnNonRestNote";
      break;

    case msrSyllableKind::kSyllableMeasureEnd:
      result = "kSyllableMeasureEnd";
      break;

    case msrSyllableKind::kSyllableLineBreak:
      result = "kSyllableLineBreak";
      break;
    case msrSyllableKind::kSyllablePageBreak:
      result = "kSyllablePageBreak";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrSyllableKind& elt)
{
  os << msrSyllableKindAsString (elt);
  return os;
}

//______________________________________________________________________________
std::string msrSyllableExtendKindAsString (
  msrSyllableExtendKind syllableExtendKind)
{
  std::string result;

  switch (syllableExtendKind) {
    case msrSyllableExtendKind::kSyllableExtend_NONE:
      result = "kSyllableExtend_NONE";
      break;
    case msrSyllableExtendKind::kSyllableExtendTypeLess:
      result = "kSyllableExtendTypeLess";
      break;
    case msrSyllableExtendKind::kSyllableExtendTypeStart:
      result = "kSyllableExtendTypeStart";
      break;
    case msrSyllableExtendKind::kSyllableExtendTypeContinue:
      result = "kSyllableExtendTypeContinue";
      break;
    case msrSyllableExtendKind::kSyllableExtendTypeStop:
      result = "kSyllableExtendTypeStop";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrSyllableExtendKind& elt)
{
  os << msrSyllableExtendKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrSyllableElement msrSyllableElement::create (
  const mfInputLineNumber& inputStartLineNumber,
  msrSyllableElementKind   syllableElementKind,
  const std::string&       syllableElementContents)
{
  msrSyllableElement* obj =
    new msrSyllableElement (
      inputStartLineNumber,
      syllableElementKind,
      syllableElementContents);
  assert (obj != nullptr);
  return obj;
}

msrSyllableElement::msrSyllableElement (
  const mfInputLineNumber& inputStartLineNumber,
  msrSyllableElementKind   syllableElementKind,
  const std::string&       syllableElementContents)
{
  fInputLineNumber = inputStartLineNumber;

  fSyllableElementKind = syllableElementKind;

  fSyllableElementContents = syllableElementContents;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Creating a syllable element containing: " <<
      asString () <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED
}

msrSyllableElement::~msrSyllableElement ()
{}

std::string msrSyllableElement::asString () const
{
  std::stringstream ss;

  ss <<
    "[SyllableElement" <<
    ", fSyllableElementKind: " << fSyllableElementKind <<
    ", fSyllableElementContents: \"" << fSyllableElementContents << "\"" <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

std::string msrSyllableElement::asShortString () const
{
  std::stringstream ss;

  ss <<
//     '[' <<
    "\"" << fSyllableElementContents << "\"" <<
    ", " <<
    fSyllableElementKind <<
    ", line " << fInputLineNumber;
//     ']';

  return ss.str ();
}

void msrSyllableElement::print (std::ostream& os) const
{
  os <<
    "[SyllableElement" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 32;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fSyllableElementKind" << ": " <<
    fSyllableElementKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSyllableElementContents" << ": \"" <<
    fSyllableElementContents <<
    "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fInputLineNumber" << ": " <<
    fInputLineNumber <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const msrSyllableElement& elt)
{
  elt.print (os);
  return os;
}

//______________________________________________________________________________
std::string syllableElementsListAsString (
  const std::list <msrSyllableElement>& syllableElementsList)
{
  std::stringstream ss;

  if (syllableElementsList.size ()) {
    std::list <msrSyllableElement>::const_iterator
      iBegin = syllableElementsList.begin (),
      iEnd   = syllableElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      ss << (*i).asString ();
      if (++i == iEnd) break;
      ss << ", ";
    } // for
  }

  return ss.str ();
}

std::string syllableElementsListAsShortString (
  const std::list <msrSyllableElement>& syllableElementsList)
{
  std::stringstream ss;

  if (syllableElementsList.size ()) {
    std::list <msrSyllableElement>::const_iterator
      iBegin = syllableElementsList.begin (),
      iEnd   = syllableElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      ss <<
        (*i).asShortString ();
      if (++i == iEnd) break;
      ss << ", ";
    } // for
  }

  return ss.str ();
}

//______________________________________________________________________________
S_msrSyllable msrSyllable::create (
  const mfInputLineNumber& inputLineNumber,
  msrSyllableKind        syllableKind,
  msrSyllableExtendKind  syllableExtendKind,
  const mfStanzaNumber&  syllableStanzaNumber,
  const mfWholeNotes&    syllableWholeNotes,
  const msrTupletFactor& syllableTupletFactor)
{
  msrSyllable* obj =
    new msrSyllable (
      inputLineNumber,
      syllableKind,
      syllableExtendKind,
      syllableStanzaNumber,
      syllableWholeNotes,
      syllableTupletFactor);
  assert (obj != nullptr);
  return obj;
}

S_msrSyllable msrSyllable::create (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure&    upLinkToMeasure,
  msrSyllableKind        syllableKind,
  msrSyllableExtendKind  syllableExtendKind,
  const mfStanzaNumber&  syllableStanzaNumber,
  const mfWholeNotes&    syllableWholeNotes,
  const msrTupletFactor& syllableTupletFactor,
  const S_msrStanza&     syllableUpLinkToStanza)
{
  msrSyllable* obj =
    new msrSyllable (
      inputLineNumber,
      upLinkToMeasure,
      syllableKind,
      syllableExtendKind,
      syllableStanzaNumber,
      syllableWholeNotes,
      syllableTupletFactor,
      syllableUpLinkToStanza);
  assert (obj != nullptr);
  return obj;
}

S_msrSyllable msrSyllable::create (
  const mfInputLineNumber& inputLineNumber,
  msrSyllableKind        syllableKind,
  msrSyllableExtendKind  syllableExtendKind,
  const mfStanzaNumber&  syllableStanzaNumber,
  const mfWholeNotes&    syllableWholeNotes,
  const msrTupletFactor& syllableTupletFactor,
  const S_msrStanza&     syllableUpLinkToStanza)
{
  return
    msrSyllable::create (
      inputLineNumber,
      gNullMeasure, // set later in setSyllableUpLinkToMeasure()
      syllableKind,
      syllableExtendKind,
      syllableStanzaNumber,
      syllableWholeNotes,
      syllableTupletFactor,
      syllableUpLinkToStanza);
}

msrSyllable::msrSyllable (
  const mfInputLineNumber& inputLineNumber,
  msrSyllableKind        syllableKind,
  msrSyllableExtendKind  syllableExtendKind,
  const mfStanzaNumber&  syllableStanzaNumber,
  const mfWholeNotes&    syllableWholeNotes,
  const msrTupletFactor& syllableTupletFactor)
    : msrElement (
        inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity checks
  switch (syllableKind) {
    case msrSyllableKind::kSyllableNone:
    case msrSyllableKind::kSyllableSingle:
    case msrSyllableKind::kSyllableBegin:
    case msrSyllableKind::kSyllableMiddle:
    case msrSyllableKind::kSyllableEnd:
    case msrSyllableKind::kSyllableOnRestNote:
    case msrSyllableKind::kSyllableSkipOnRestNote:
    case msrSyllableKind::kSyllableSkipOnNonRestNote:
#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
      if (gWaeOahGroup->getMaintainanceRun ()) { // MAINTAINANCE_RUN
        mfAssert ( // JMI 0.9.70 BABASSE
          __FILE__, mfInputLineNumber (__LINE__),
          syllableWholeNotes.getNumerator () > 0,
          "syllableWholeNotes "
            +
          std::to_string (syllableWholeNotes.getNumerator ())
            +
          " is not positive");
        }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED
      break;

    case msrSyllableKind::kSyllableMeasureEnd:
      break;

    case msrSyllableKind::kSyllableLineBreak:
      break;
    case msrSyllableKind::kSyllablePageBreak:
      break;
  } // switch
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fSyllableKind = syllableKind;

  fSyllableExtendKind = syllableExtendKind;

  fSyllableStanzaNumber = syllableStanzaNumber;

  fSyllableWholeNotes = syllableWholeNotes;

  fSyllableTupletFactor = syllableTupletFactor;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Creating a syllable containing: " <<
      asString () <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED
}

msrSyllable::msrSyllable (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure&    upLinkToMeasure,
  msrSyllableKind        syllableKind,
  msrSyllableExtendKind  syllableExtendKind,
  const mfStanzaNumber&  syllableStanzaNumber,
  const mfWholeNotes&    syllableWholeNotes,
  const msrTupletFactor& syllableTupletFactor,
  const S_msrStanza&     syllableUpLinkToStanza)
    : msrElement (
        inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity checks
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    syllableUpLinkToStanza != nullptr,
    "syllableUpLinkToStanza is NULL");

  switch (syllableKind) {
    case msrSyllableKind::kSyllableNone:
    case msrSyllableKind::kSyllableSingle:
    case msrSyllableKind::kSyllableBegin:
    case msrSyllableKind::kSyllableMiddle:
    case msrSyllableKind::kSyllableEnd:
    case msrSyllableKind::kSyllableOnRestNote:
    case msrSyllableKind::kSyllableSkipOnRestNote:
    case msrSyllableKind::kSyllableSkipOnNonRestNote:
#ifdef MF_MAINTAINANCE_RUNS_ARE_ENABLED
      if (gWaeOahGroup->getMaintainanceRun ()) { // MAINTAINANCE_RUN
        mfAssert ( // JMI 0.9.70 BABASSE
          __FILE__, mfInputLineNumber (__LINE__),
          syllableWholeNotes.getNumerator () > 0,
          "syllableWholeNotes "
            +
          std::to_string (syllableWholeNotes.getNumerator ())
            +
          " is not positive");
        }
#endif // MF_MAINTAINANCE_RUNS_ARE_ENABLED
      break;

    case msrSyllableKind::kSyllableMeasureEnd:
      break;

    case msrSyllableKind::kSyllableLineBreak:
      break;
    case msrSyllableKind::kSyllablePageBreak:
      break;
  } // switch
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set syllable's upLink to stanza
  fSyllableUpLinkToStanza =
    syllableUpLinkToStanza;

  // set syllable's upLink to measure
  fSyllableUpLinkToMeasure =
    upLinkToMeasure;

  fSyllableKind = syllableKind;

  fSyllableExtendKind = syllableExtendKind;

  fSyllableStanzaNumber = syllableStanzaNumber;

  fSyllableWholeNotes = syllableWholeNotes;

  fSyllableTupletFactor = syllableTupletFactor;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Creating a syllable containing: " <<
      asString () <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED
}

msrSyllable::~msrSyllable ()
{}

S_msrSyllable msrSyllable::createSyllableNewbornClone (
  const S_msrPart& containingPart)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of syllable '" <<
      asString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    containingPart != nullptr,
    "containingPart is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrSyllable
    newbornClone =
      msrSyllable::create (
        fInputLineNumber,
        fSyllableKind,
        fSyllableExtendKind,
        fSyllableStanzaNumber,
        fSyllableWholeNotes,
        fSyllableTupletFactor);

  // append the lyric texts to the syllable clone
  for (msrSyllableElement syllableElement : fSyllableElementsList) {
    newbornClone->
      appendSyllableElementToSyllable (syllableElement);
  } // for

  // don't set 'newbornClone->fSyllableUpLinkToStanza'
  // nor 'newbornClone->fSyllableUpLinkToNote',
  // this will be done by the caller

//   newbornClone->fSyllableUpLinkToNote =
//     fSyllableUpLinkToNote; // TEMP

  return newbornClone;
}

S_msrSyllable msrSyllable::createSyllableDeepClone (
  const S_msrPart& containingPart)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of syllable '" <<
      asString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    containingPart != nullptr,
    "containingPart is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrSyllable
    deepClone =
      msrSyllable::create (
        fInputLineNumber,
        gNullMeasure, // set later in setSyllableUpLinkToMeasure()
        fSyllableKind,
        fSyllableExtendKind,
        fSyllableStanzaNumber,
        fSyllableWholeNotes,
        fSyllableTupletFactor,
        fSyllableUpLinkToStanza);

  // append the lyric texts to the syllable deep clone
  for (msrSyllableElement syllableElement : fSyllableElementsList) {
    deepClone->
      appendSyllableElementToSyllable (syllableElement);
  } // for

  // don't set 'newbornClone->fSyllableUpLinkToStanza'
  // nor 'newbornClone->fSyllableUpLinkToNote',
  // this will be done by the caller

  deepClone->fSyllableUpLinkToNote =
    fSyllableUpLinkToNote; // TEMP

  return deepClone;
}

// void msrSyllable::setSyllableUpLinkToMeasure ( // JMI UNUSED ??? 0.9.70 BABASSE
//   const S_msrMeasure& measure)
// {
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, mfInputLineNumber (__LINE__),
//     measure != nullptr,
//     "measure is NULL");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceLyrics ()) {
//     ++gIndenter;
//
//     gLog <<
//       "Setting the uplink to measure of syllable " <<
//       asString () <<
//       " to measure " << measure->asString () <<
//       "' in measure '" <<
//       measure->asString () <<
//       std::endl;
//
//     --gIndenter;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fSyllableUpLinkToMeasure = measure;
// }

// void msrSyllable::setSyllableMeasureNumber (
//   const mfMeasureNumber& measureNumber)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceLyrics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Setting syllable next measure purist number to " <<
//       fSyllableMeasureNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   switch (fSyllableKind) {
//     case msrSyllableKind::kSyllableNone:
//     case msrSyllableKind::kSyllableSingle:
//     case msrSyllableKind::kSyllableBegin:
//     case msrSyllableKind::kSyllableMiddle:
//     case msrSyllableKind::kSyllableEnd:
//     case msrSyllableKind::kSyllableOnRestNote:
//     case msrSyllableKind::kSyllableSkipOnRestNote:
//     case msrSyllableKind::kSyllableSkipOnNonRestNote:
//       {
//         std::stringstream ss;
//
//         ss <<
//           "syllable with measure number '" <<
//           measureNumber <<
//           "' is no line nor page break" <<
//           ", line " << fInputLineNumber; // JMI 0.9.70
//
//         msrInternalError (
//           gServiceRunData->getInputSourceName (),
//           fInputLineNumber,
//           __FILE__, mfInputLineNumber (__LINE__),
//           ss.str ());
//       }
//       break;
//
//     case msrSyllableKind::kSyllableMeasureEnd:
//     case msrSyllableKind::kSyllableLineBreak:
//     case msrSyllableKind::kSyllablePageBreak:
//       break;
//   } // switch
//
//   fSyllableMeasureNumber =
//     measureNumber;
// }
//
int msrSyllable::fetchSyllableMeasurePuristNumber () const // JMI 0.9.72 supeflous???
{
  return 5315;

//     fSyllableUpLinkToMeasure->
//       getMeasurePuristNumber ();

//     fSyllableUpLinkToNote->
//       getMeasureElementUpLinkToMeasure ()->
//         getMeasurePuristNumber ();
}

void msrSyllable::appendSyllableElementToSyllable (
  const msrSyllableElement& syllableElement)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending syllable element " <<
      syllableElement.asString () <<
      " to syllable " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fSyllableElementsList.push_back (
    syllableElement);
}

void msrSyllable::setSyllableUpLinkToNote (const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Setting syllable note upLink of " <<
      asString () <<
      " to " <<
      note->asString () <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

  fSyllableUpLinkToNote = note;
}

void msrSyllable::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSyllable::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrSyllable>*
    p =
      dynamic_cast<visitor<S_msrSyllable>*> (v)) {
        S_msrSyllable elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrSyllable::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrSyllable::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSyllable::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrSyllable>*
    p =
      dynamic_cast<visitor<S_msrSyllable>*> (v)) {
        S_msrSyllable elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrSyllable::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrSyllable::browseData (basevisitor* v)
{}

std::string msrSyllable::syllableWholeNotesPitchAndOctaveAsString () const // JMI OCTAVE??? BABASSE
{
  std::string result;

  if (fSyllableUpLinkToNote) { // JMI
    switch (fSyllableUpLinkToNote->getNoteKind ()) {
      case msrNoteKind::kNote_UNKNOWN_:
      case msrNoteKind::kNoteRestInMeasure:
      case msrNoteKind::kNoteUnpitchedInMeasure:
      case msrNoteKind::kNoteRegularInMeasure:
      case msrNoteKind::kNoteInDoubleTremolo:
      case msrNoteKind::kNoteRegularInGraceNotesGroup:
      case msrNoteKind::kNoteRestInGraceNotesGroup:
      case msrNoteKind::kNoteSkipInGraceNotesGroup:
      case msrNoteKind::kNoteInChordInGraceNotesGroup:
      case msrNoteKind::kNoteRegularInChord:
        result =
          wholeNotesPitchAndOctaveAsString (
            fInputLineNumber,
            fSyllableWholeNotes);
        break;

      case msrNoteKind::kNoteSkipInMeasure:
      case msrNoteKind::kNoteRegularInTuplet:
      case msrNoteKind::kNoteRestInTuplet:
      case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      case msrNoteKind::kNoteUnpitchedInTuplet:
        {
          std::stringstream ss;

          ss <<
            fSyllableUpLinkToNote->
      // JMI        noteSoundingWholeNotesPitchAndOctaveAsString () <<
              noteDisplayWholeNotesPitchAndOctaveAsString () <<
            "*" <<
            fSyllableTupletFactor.getTupletNormalNotes () <<
            '/' <<
            fSyllableTupletFactor.getTupletActualNotes ();

          result = ss.str ();
        }
        break;
    } // switch
  }

  else {
    result =
      wholeNotesPitchAndOctaveAsString (
        fInputLineNumber,
        fSyllableWholeNotes);
  }

  return result;
}

std::string msrSyllable::syllableUpLinkToNoteAsString () const
{
  std::string result;

  if (fSyllableUpLinkToNote) {
    result = fSyllableUpLinkToNote->asString ();
  }
  else {
    result = "[NULL]";
  }

  return result;
}

std::string msrSyllable::syllableUpLinkToNoteAsShortString () const
{
  std::string result;

  if (fSyllableUpLinkToNote) {
    result = fSyllableUpLinkToNote->asShortString ();
  }
  else {
    result = "[NULL]";
  }

  return result;
}

// std::string msrSyllable::syllableElementsListAsString () const
// {
//   std::stringstream ss;
//
//   if (fSyllableElementsList.size ()) {
//     std::list <msrSyllableElement>::const_iterator
//       iBegin = fSyllableElementsList.begin (),
//       iEnd   = fSyllableElementsList.end (),
//       i      = iBegin;
//
//     for ( ; ; ) {
//       ss << mfDoubleQuoteStringIfNonAlpha ((*i).asString ());
//       if (++i == iEnd) break;
//       ss << " || ";
//     } // for
//   }
//
//   return ss.str ();
// }
//
// std::string msrSyllable::syllableElementsListAsShortString () const
// {
//   std::stringstream ss;
//
//   if (fSyllableElementsList.size ()) {
//     std::list <msrSyllableElement>::const_iterator
//       iBegin = fSyllableElementsList.begin (),
//       iEnd   = fSyllableElementsList.end (),
//       i      = iBegin;
//
//     for ( ; ; ) {
//       ss << mfDoubleQuoteStringIfNonAlpha ((*i).asString ());
//       if (++i == iEnd) break;
//       ss << " || ";
//     } // for
//   }
//
//   return ss.str ();
// }

std::string msrSyllable::asString () const
{
  std::stringstream ss;

  ss <<
    "[Syllable " <<
    ", fSyllableElementsList: " <<
    syllableElementsListAsString (fSyllableElementsList) <<
    ", fSyllableKind: " << fSyllableKind <<
    ", fSyllableExtendKind: " << fSyllableExtendKind <<
    ", fSyllableStanzaNumber: \"" << fSyllableStanzaNumber << "\"" <<
    ", fSyllableWholeNotes: " << fSyllableWholeNotes.asFractionString () <<
    ", fSyllableTupletFactor: " << fSyllableTupletFactor.asFractionString ();

  ss <<
    ", fSyllableUpLinkToMeasure: ";
    if (fSyllableUpLinkToMeasure) {
      ss <<
        fSyllableUpLinkToMeasure->asString ();
    }
    else {
      ss << "[NULL]";
    }

  ss <<
    ", line " << fInputLineNumber;

  ss <<
    ", " <<
    syllableUpLinkToNoteAsString ();

  switch (fSyllableKind) {
    case msrSyllableKind::kSyllableNone:
      msrInternalError (
        gServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, mfInputLineNumber (__LINE__),
        "syllable type has not been set");
      break;

    case msrSyllableKind::kSyllableSingle:
    case msrSyllableKind::kSyllableBegin:
    case msrSyllableKind::kSyllableMiddle:
    case msrSyllableKind::kSyllableEnd:
    case msrSyllableKind::kSyllableOnRestNote:
    case msrSyllableKind::kSyllableSkipOnRestNote:
    case msrSyllableKind::kSyllableSkipOnNonRestNote:
    case msrSyllableKind::kSyllableMeasureEnd:
    case msrSyllableKind::kSyllableLineBreak:
    case msrSyllableKind::kSyllablePageBreak:
      break;
  } // switch

  ss << // JMI LENK
    ", fSyllableUpLinkToNote: ";
  if (fSyllableUpLinkToNote) {
    ss <<
      fSyllableUpLinkToNote->asShortString ();
  }
  else {
    ss << "[NULL]";
  }

  ss <<
    ", fSyllableUpLinkToStanza: ";
  if (fSyllableUpLinkToStanza) {
    ss <<
      fSyllableUpLinkToStanza->getStanzaName ();
  }
  else {
    ss << "[NULL]";
  }

  ss << ']';

  return ss.str ();
}

std::string msrSyllable::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[Syllable " <<
    syllableElementsListAsShortString (fSyllableElementsList) <<
    "\", " << fSyllableKind <<
    ", " << fSyllableKind <<
    ", " << fSyllableExtendKind <<
    ", \"" << fSyllableStanzaNumber << "\"" <<
    ", fSyllableWholeNotes: " << fSyllableWholeNotes.asFractionString () <<
    ", fSyllableTupletFactor: " << fSyllableTupletFactor.asFractionString ();

//   ss <<
//     ", fSyllableUpLinkToMeasure: ";
//     if (fSyllableUpLinkToMeasure) {
//       ss <<
//         fSyllableUpLinkToMeasure->asString ();
//     }
//     else {
//       ss << "[NULL]";
//     }

  ss <<
    ", line " << fInputLineNumber;

  ss << ']';

  return ss.str ();
}

void msrSyllable::print (std::ostream& os) const
{
  os <<
    "[Syllable" <<
    ", line " << fInputLineNumber << ":" <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 25;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fSyllableKind" << ": " << fSyllableKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSyllableExtendKind" << ": " << fSyllableExtendKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSyllableStanzaNumber" << ": \"" << fSyllableStanzaNumber << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSyllableWholeNotes" << ": " << fSyllableWholeNotes.asFractionString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSyllableTupletFactor" << ": " << fSyllableTupletFactor.asFractionString () <<
    std::endl;

  os << std::left <<
    "fSyllableUpLinkToMeasure" << ": ";
  if (fSyllableUpLinkToMeasure) {
    os <<
      fSyllableUpLinkToMeasure->asString ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fSyllableElementsList" <<
    std::endl;

  ++gIndenter;
  for (msrSyllableElement syllableElement : fSyllableElementsList) {
    os <<
      syllableElement;
  } // for
  --gIndenter;

  --gIndenter;

  os << ']' << std::endl;
}

void msrSyllable::printFull (std::ostream& os) const
{
  os <<
    "[Syllable" <<
    ", fSyllableKind: " <<
    fSyllableKind <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 32;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fSyllableElementsList" << ": " <<
    syllableElementsListAsString (fSyllableElementsList) <<
    std::endl <<

//     std::setw (fieldWidth) <<
//     "fSyllableKind" << ": " <<
//     msrSyllableKindAsString (fSyllableKind) <<
//     std::endl <<

    std::setw (fieldWidth) <<
    "fSyllableExtendKind" << ": " <<
    fSyllableExtendKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSyllableStanzaNumber" << ": \"" <<
    fSyllableStanzaNumber <<
    "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSyllableWholeNotes" << ": " <<
    fSyllableWholeNotes.asFractionString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSyllableTupletFactor" << ": " <<
    fSyllableTupletFactor <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fSyllableUpLinkToMeasure" << ": ";
  if (fSyllableUpLinkToMeasure) {
    os <<
      fSyllableUpLinkToMeasure->asString ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "syllableUpLinkToNote" << ": " <<
    syllableUpLinkToNoteAsString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "syllableUpLinkToStanza" << ": " <<
    fSyllableUpLinkToStanza->getStanzaName () <<
    std::endl;

  switch (fSyllableKind) { // JMI
    case msrSyllableKind::kSyllableNone:
      msrInternalError (
        gServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, mfInputLineNumber (__LINE__),
        "syllable type has not been set");
      break;

    case msrSyllableKind::kSyllableSingle:
    case msrSyllableKind::kSyllableBegin:
    case msrSyllableKind::kSyllableMiddle:
    case msrSyllableKind::kSyllableEnd:
    case msrSyllableKind::kSyllableOnRestNote:
    case msrSyllableKind::kSyllableSkipOnRestNote:
    case msrSyllableKind::kSyllableSkipOnNonRestNote:
    case msrSyllableKind::kSyllableMeasureEnd:
    case msrSyllableKind::kSyllableLineBreak:
    case msrSyllableKind::kSyllablePageBreak:
      break;
  } // switch
  os << std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrSyllable& elt)
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
S_msrStanza msrStanza::create (
  const mfInputLineNumber& inputLineNumber,
  const mfStanzaNumber& stanzaNumber,
  const S_msrVoice&  stanzaUpLinkToVoice)
{
  msrStanza* obj =
    new msrStanza (
      inputLineNumber,
      stanzaNumber,
      stanzaUpLinkToVoice);
  assert (obj != nullptr);
  return obj;
}

msrStanza::msrStanza (
  const mfInputLineNumber& inputLineNumber,
  const mfStanzaNumber& stanzaNumber,
  const S_msrVoice&  stanzaUpLinkToVoice)
    : msrElement (inputLineNumber)
{
  // set stanza number and kind
  fStanzaNumber = stanzaNumber;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    stanzaUpLinkToVoice != nullptr,
    "stanzaUpLinkToVoice is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set stanza's voice upLink
  fStanzaUpLinkToVoice =
    stanzaUpLinkToVoice;

  // do other initializations
  initializeStanza ();
}

void msrStanza::initializeStanza ()
{
  fStanzaName =
    fStanzaUpLinkToVoice->getVoiceName () +
    "_Stanza_" +
    mfStanzaNumberAsString (fStanzaNumber);

  fStanzaPathLikeName =
    fStanzaUpLinkToVoice->getVoicePathLikeName () +
    "_Stanza_" +
      mfStringNumbersToEnglishWords (
        mfMakeSingleWordFromString (
          mfStanzaNumberAsString (fStanzaNumber)));

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Initializing stanza " << getStanzaName ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStanzaTextPresent = false;

  fStanzaMeasureCurrentPositionInMeasure = K_POSITION_IN_MEASURE_ZERO;
}

msrStanza::~msrStanza ()
{}

S_msrStanza msrStanza::createStanzaNewbornClone (
  const S_msrVoice& containingVoice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of stanza \"" <<
      getStanzaName () <<
      "\" in voice \"" <<
      containingVoice->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    containingVoice != nullptr,
    "containingVoice is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrStanza
    newbornClone =
      msrStanza::create (
        fInputLineNumber,
        fStanzaNumber,
        containingVoice);

  // number

  // kind

  // names
  newbornClone->fStanzaName =
    fStanzaName;
  newbornClone->fStanzaPathLikeName =
    fStanzaPathLikeName;

  // contents
  newbornClone->fStanzaTextPresent =
    fStanzaTextPresent;

  // upLinks
  newbornClone->fStanzaUpLinkToVoice =
    containingVoice;

  return newbornClone;
}

S_msrStanza msrStanza::createStanzaDeepClone (
  const S_msrVoice& containingVoice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Creating a deep clone of stanza \"" <<
      getStanzaName () <<
      "\" in voice \"" <<
      containingVoice->getVoiceName () <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    containingVoice != nullptr,
    "containingVoice is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrStanza
    stanzaDeepClone =
      msrStanza::create (
        fInputLineNumber,
        fStanzaNumber,
        containingVoice);

  // number

  // kind

  // name
  stanzaDeepClone->fStanzaName =
    fStanzaName;
  stanzaDeepClone->fStanzaPathLikeName =
    fStanzaPathLikeName;

  // contents
  int n = fSyllables.size ();
  for (int i = 0; i < n; ++i) {
    stanzaDeepClone->fSyllables.push_back (
      fSyllables [i]->
        createSyllableDeepClone (
          containingVoice->
            fetchVoiceUpLinkToPart ()));
  } // for

  stanzaDeepClone->fStanzaTextPresent =
    fStanzaTextPresent;

  // upLinks
  stanzaDeepClone->fStanzaUpLinkToVoice =
    containingVoice;

  return stanzaDeepClone;
}

void msrStanza::appendSyllableToStanza (
  const S_msrSyllable&       syllable,
  const S_msrMeasure&        upLinkToMeasure,
  const mfPositionInMeasure& partCurrentDrawingPositionInMeasure)
{
/*
  upLinkToMeasure is needed here, to obtain the next measure's purist number
*/

  // compute position delta
  mfWholeNotes
    positionsDelta =
      partCurrentDrawingPositionInMeasure.asWholeNotes ()
        -
      fStanzaMeasureCurrentPositionInMeasure.asWholeNotes ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending syllable " << syllable->asString () <<
      " to stanza " << getStanzaName () <<
      ", partCurrentDrawingPositionInMeasure: " <<
      partCurrentDrawingPositionInMeasure.asString () <<
      ", fStanzaMeasureCurrentPositionInMeasure: " <<
      fStanzaMeasureCurrentPositionInMeasure.asString () <<
      ", positionsDelta: " <<
      positionsDelta.asString () <<
      ", line " << syllable->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   // pad up stanza if relevant
//   if (positionsDelta.getNumerator () != 0) { // JMI 0.9.70 BABASSE
//     // create a skip on rest note syllable
//     S_msrSyllable
//       skipRestNoteSyllable =
//         msrSyllable::create (
//           syllable->getInputLineNumber (),
//           msrSyllableKind::kSyllableSkipOnRestNote,
//           msrSyllableExtendKind::kSyllableExtend_NONE,
//           fStanzaNumber,
//           positionsDelta,
//           msrTupletFactor (1, 1),
//           this);
//
//     // append it to this stanza
//     fSyllables.push_back (skipRestNoteSyllable);
//
//     // account for syllable length
//     fStanzaMeasureCurrentPositionInMeasure +=
//       skipRestNoteSyllable->getSyllableWholeNotes ();
//   }

  // append syllable to this stanza
  fSyllables.push_back (syllable);

  // account for syllable length
  fStanzaMeasureCurrentPositionInMeasure +=
    syllable->getSyllableWholeNotes ();

  // set the syllable's stanza uplink
  syllable->
    setSyllableUpLinkToStanza (this);

  // set the syllable's measure uplink
  syllable->
    setSyllableUpLinkToMeasure (upLinkToMeasure);

  // does this stanza contain text?
  switch (syllable->getSyllableKind ()) {

    case msrSyllableKind::kSyllableSingle:
    case msrSyllableKind::kSyllableBegin:
    case msrSyllableKind::kSyllableMiddle:
    case msrSyllableKind::kSyllableEnd:
      // only now, in case addSyllableToStanza () is called
      // from LPSR for example
      fStanzaTextPresent = true;
      break;

    case msrSyllableKind::kSyllableOnRestNote:
    case msrSyllableKind::kSyllableSkipOnRestNote:
    case msrSyllableKind::kSyllableSkipOnNonRestNote:
    case msrSyllableKind::kSyllableMeasureEnd:
    case msrSyllableKind::kSyllableLineBreak:
    case msrSyllableKind::kSyllablePageBreak:
      break;

    case msrSyllableKind::kSyllableNone:
      msrInternalError (
        gServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, mfInputLineNumber (__LINE__),
        "syllable type has not been set");
      break;
  } // switch
}

void msrStanza::appendSyllableToStanzaClone (
  const S_msrSyllable& syllable,
  const S_msrMeasure&  upLinkToMeasure)
{
/*
  upLinkToMeasure is needed here, to obtain the next measure's purist number
*/

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending syllable " << syllable->asString () <<
      " to stanza clone " << getStanzaName ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append syllable to this stanza
  fSyllables.push_back (syllable);

  // account for syllable length
  fStanzaMeasureCurrentPositionInMeasure +=
    syllable->getSyllableWholeNotes ();

  // set the syllable's stanza uplink
  syllable->
    setSyllableUpLinkToStanza (this);

//   // set the syllable's measure uplink
//   syllable->
//     setSyllableUpLinkToMeasure (upLinkToMeasure);

  // does this stanza contain text?
  switch (syllable->getSyllableKind ()) {

    case msrSyllableKind::kSyllableSingle:
    case msrSyllableKind::kSyllableBegin:
    case msrSyllableKind::kSyllableMiddle:
    case msrSyllableKind::kSyllableEnd:
      // only now, in case addSyllableToStanza () is called
      // from LPSR for example
      fStanzaTextPresent = true;
      break;

    case msrSyllableKind::kSyllableOnRestNote:
    case msrSyllableKind::kSyllableSkipOnRestNote:
    case msrSyllableKind::kSyllableSkipOnNonRestNote:
    case msrSyllableKind::kSyllableMeasureEnd:
    case msrSyllableKind::kSyllableLineBreak:
    case msrSyllableKind::kSyllablePageBreak:
      break;

    case msrSyllableKind::kSyllableNone:
      msrInternalError (
        gServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, mfInputLineNumber (__LINE__),
        "syllable type has not been set");
      break;
  } // switch
}

void msrStanza::appendMeasureEndSyllableToStanza (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure&        upLinkToMeasure,
  const mfPositionInMeasure& partCurrentDrawingPositionInMeasure)
{
  // compute position delta
  mfWholeNotes
    positionsDelta =
      partCurrentDrawingPositionInMeasure.asWholeNotes ()
        -
      fStanzaMeasureCurrentPositionInMeasure.asWholeNotes ();

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending a measure end syllable " <<
      " to stanza " << getStanzaName ();

    ss <<
        ", upLinkToMeasure: ";
    if (upLinkToMeasure) {
      ss <<
        upLinkToMeasure->asShortString ();
    }
    else {
      ss << "[NULL]";
    }

    ss <<
      ", partCurrentDrawingPositionInMeasure: " <<
      partCurrentDrawingPositionInMeasure <<
      ", fStanzaMeasureCurrentPositionInMeasure: " <<
      fStanzaMeasureCurrentPositionInMeasure <<
      ", positionsDelta: " <<
      positionsDelta.asString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

//   // pad up stanza if relevant
//   if (positionsDelta.getNumerator () > 0) { // JMI 0.9.70 it may be negative...
//     // create a skip on rest note syllable
//     S_msrSyllable
//       skipRestNoteSyllable =
//         msrSyllable::create (
//           inputLineNumber,
//           msrSyllableKind::kSyllableSkipOnRestNote,
//           msrSyllableExtendKind::kSyllableExtend_NONE,
//           fStanzaNumber,
//           positionsDelta,
//           msrTupletFactor (1, 1),
//           this);
//
//     // append it to this stanza
//     fSyllables.push_back (skipRestNoteSyllable);
//
//     // account for syllable length
//     fStanzaMeasureCurrentPositionInMeasure +=
//       skipRestNoteSyllable->getSyllableWholeNotes ();
//   }

  // create measure end syllable
  S_msrSyllable
    measureEndSyllable =
      msrSyllable::create (
        inputLineNumber,
        upLinkToMeasure,
        msrSyllableKind::kSyllableMeasureEnd,
        msrSyllableExtendKind::kSyllableExtend_NONE,
        fStanzaNumber,
        K_WHOLE_NOTES_ZERO,
        msrTupletFactor (1, 1),
        this);

//   // set measureEndSyllable uplink to measure
//   measureEndSyllable->
//     setSyllableMeasureNumber (
//       measureNumber);

  // append measureEndSyllable to this stanza clone
  appendSyllableToStanzaClone (
    measureEndSyllable,
    upLinkToMeasure);

  // reset measure whole notes
  fStanzaMeasureCurrentPositionInMeasure =
    mfPositionInMeasure::createFromWholeNotes (
      K_WHOLE_NOTES_ZERO);

  --gIndenter;
}

S_msrSyllable msrStanza::appendLineBreakSyllableToStanza (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending a 'LineBreak' syllable" <<
      " to stanza " << getStanzaName () <<
      ", upLinkToMeasure: " << upLinkToMeasure->asShortString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // create line break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        upLinkToMeasure,
        msrSyllableKind::kSyllableLineBreak,
        msrSyllableExtendKind::kSyllableExtend_NONE,
        fStanzaNumber,
        K_WHOLE_NOTES_ZERO,
        msrTupletFactor (),
        this);

  // append syllable to this stanza clone
  appendSyllableToStanzaClone (
    syllable,
    upLinkToMeasure);

  --gIndenter;

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendPageBreakSyllableToStanza (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending a 'PageBreak' syllable" <<
      " to stanza " << getStanzaName () <<
      ", upLinkToMeasure: " << upLinkToMeasure->asShortString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // create page break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        upLinkToMeasure,
        msrSyllableKind::kSyllablePageBreak,
        msrSyllableExtendKind::kSyllableExtend_NONE,
        fStanzaNumber,
        K_WHOLE_NOTES_ZERO,
        msrTupletFactor (),
        this);

  // append syllable to this stanza clone
  appendSyllableToStanzaClone (
    syllable,
    upLinkToMeasure);

  --gIndenter;

  // and return it
  return syllable;
}

void msrStanza::appendPaddingNoteToStanza ( // JMI USELESS??? 0.9.70
  const mfInputLineNumber& inputLineNumber,
  const mfWholeNotes& forwardStepLength)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending padding note" <<
      ", forwardStepLength: " <<
      forwardStepLength <<
      ", to stanza \"" <<
      fStanzaName <<
      "\" in voice \"" <<
      fStanzaUpLinkToVoice->getVoiceName () <<
      "\", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // JMI TO DO ??? 0.9.70

  --gIndenter;
}

void msrStanza::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrStanza::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrStanza>*
    p =
      dynamic_cast<visitor<S_msrStanza>*> (v)) {
        S_msrStanza elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrStanza::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrStanza::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrStanza::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrStanza>*
    p =
      dynamic_cast<visitor<S_msrStanza>*> (v)) {
        S_msrStanza elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrStanza::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrStanza::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrStanza::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // browse the syllables
  int n = fSyllables.size ();
  for (int i = 0; i < n; ++i) {
    // browse the syllable
    msrBrowser<msrSyllable> browser (v);
    browser.browse (*fSyllables [i]);
  } // for

  --gIndenter;

#ifdef MF_TRACING_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% <== msrStanza::browseData ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrStanza::print (std::ostream& os) const
{
  os <<
    "[Stanza " << getStanzaName () <<
    " (number \"" <<
    fStanzaNumber <<
    "\", " <<
    fSyllables.size () << " syllables)" <<
    std::endl;

  ++gIndenter;

  if (! fStanzaTextPresent) {
    os <<
      "(No actual text)" <<
      std::endl;
  }

  else {
    if (fSyllables.size ()) {
      for (S_msrSyllable syllable : fSyllables) {
//         syllable->print (os);
        os <<
          syllable->asShortString () <<
          std::endl;
      } // for
    }
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrStanza::printFull (std::ostream& os) const
{
  os <<
    "[Stanza " << getStanzaName () <<
    " (number \"" <<
    fStanzaNumber <<
    "\", " <<
    fSyllables.size () << " syllables)" <<
    std::endl;

  ++gIndenter;

  if (! fStanzaTextPresent) {
    os <<
      "(No actual text)" <<
      std::endl;
  }

  else {
    if (fSyllables.size ()) {
      for (S_msrSyllable syllable : fSyllables) {
        syllable->printFull (os);
      } // for
    }
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrStanza& elt)
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
