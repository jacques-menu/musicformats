/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrWae.h"

#include "msrMeasureConstants.h"

#include "msrLyrics.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"

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
  int                    inputStartLineNumber,
  msrSyllableElementKind syllableElementKind,
  const std::string&     syllableElementContents)
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
  int                    inputStartLineNumber,
  msrSyllableElementKind syllableElementKind,
  const std::string&     syllableElementContents)
{
  fInputStartLineNumber = inputStartLineNumber;

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
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

std::string msrSyllableElement::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[SyllableElement" <<
    ", " << fSyllableElementKind <<
    ", \"" << fSyllableElementContents << "\"" <<
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

void msrSyllableElement::print (std::ostream& os) const
{
  os <<
    "[SyllableElement" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 32;

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
    "fInputStartLineNumber" << ": " <<
    fInputStartLineNumber <<
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
  const std::list<msrSyllableElement>& syllableElementsList)
{
  std::stringstream ss;

  if (syllableElementsList.size ()) {
    std::list<msrSyllableElement>::const_iterator
      iBegin = syllableElementsList.begin (),
      iEnd   = syllableElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      ss <<
        (*i).asString ();
      if (++i == iEnd) break;
      ss << ", ";
    } // for
  }

  return ss.str ();
}

//______________________________________________________________________________
S_msrSyllable msrSyllable::create (
  int                    inputLineNumber,
  const S_msrMeasure&    upLinkToMeasure,
  msrSyllableKind        syllableKind,
  msrSyllableExtendKind  syllableExtendKind,
  const std::string&     syllableStanzaNumber,
  const msrWholeNotes&   syllableWholeNotes,
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
  int                    inputLineNumber,
  msrSyllableKind        syllableKind,
  msrSyllableExtendKind  syllableExtendKind,
  const std::string&     syllableStanzaNumber,
  const msrWholeNotes&   syllableWholeNotes,
  const msrTupletFactor& syllableTupletFactor,
  const S_msrStanza&     syllableUpLinkToStanza)
{
  return
    msrSyllable::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      syllableKind,
      syllableExtendKind,
      syllableStanzaNumber,
      syllableWholeNotes,
      syllableTupletFactor,
      syllableUpLinkToStanza);
}

msrSyllable::msrSyllable (
  int                    inputLineNumber,
  const S_msrMeasure&    upLinkToMeasure,
  msrSyllableKind        syllableKind,
  msrSyllableExtendKind  syllableExtendKind,
  const std::string&     syllableStanzaNumber,
  const msrWholeNotes&   syllableWholeNotes,
  const msrTupletFactor& syllableTupletFactor,
  const S_msrStanza&     syllableUpLinkToStanza)
    : msrMeasureElement (
        inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity checks
  mfAssert (
    __FILE__, __LINE__,
    syllableUpLinkToStanza != nullptr,
    "syllableUpLinkToStanza is null");

  switch (syllableKind) {
    case msrSyllableKind::kSyllableNone:
    case msrSyllableKind::kSyllableSingle:
    case msrSyllableKind::kSyllableBegin:
    case msrSyllableKind::kSyllableMiddle:
    case msrSyllableKind::kSyllableEnd:
    case msrSyllableKind::kSyllableOnRestNote:
    case msrSyllableKind::kSyllableSkipOnRestNote:
    case msrSyllableKind::kSyllableSkipOnNonRestNote:
      mfAssert (
        __FILE__, __LINE__,
        syllableWholeNotes.getNumerator () > 0,
        "syllableWholeNotes "
          +
        std::to_string (syllableWholeNotes.getNumerator ())
          +
        " is not positive");
      break;

    case msrSyllableKind::kSyllableMeasureEnd:
      break;

    case msrSyllableKind::kSyllableLineBreak:
      break;
    case msrSyllableKind::kSyllablePageBreak:
      break;
  } // switch
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set syllable's stanza upLink
  fSyllableUpLinkToStanza =
    syllableUpLinkToStanza;

  fSyllableKind = syllableKind;

  fSyllableExtendKind = syllableExtendKind;

  fSyllableStanzaNumber = syllableStanzaNumber;

  // fSyllableUpLinkToNote will be set later
  // by appendSyllableToNoteAndSetItsUpLinkToNote ()

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
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingPart != nullptr,
    "containingPart is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrSyllable
    newbornClone =
      msrSyllable::create (
        fInputStartLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
        fSyllableKind,
        fSyllableExtendKind,
        fSyllableStanzaNumber,
        fSyllableWholeNotes,
        fSyllableTupletFactor,
        fSyllableUpLinkToStanza);

  // append the lyric texts to the syllable clone
  for (msrSyllableElement syllableElement : fSyllableElementsList) {
    newbornClone->
      appendSyllableElementToSyllable (syllableElement);
  } // for

  // don't set 'newbornClone->fSyllableUpLinkToStanza'
  // nor 'newbornClone->fSyllableUpLinkToNote',
  // this will be done by the caller

  newbornClone->fSyllableUpLinkToNote =
    fSyllableUpLinkToNote; // TEMP

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
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingPart != nullptr,
    "containingPart is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrSyllable
    deepClone =
      msrSyllable::create (
        fInputStartLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
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

void msrSyllable::setSyllableUpLinkToMeasure (
  const S_msrMeasure& measure)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    ++gIndenter;

    gLog <<
      "Setting the uplink to measure of syllable " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  fSyllableUpLinkToMeasure = measure;
}

void msrSyllable:: setSyllableMeasureNumber (
  const std::string& measureNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Setting syllable next measure purist number to " <<
      fSyllableMeasureNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fSyllableKind) {
    case msrSyllableKind::kSyllableNone:
    case msrSyllableKind::kSyllableSingle:
    case msrSyllableKind::kSyllableBegin:
    case msrSyllableKind::kSyllableMiddle:
    case msrSyllableKind::kSyllableEnd:
    case msrSyllableKind::kSyllableOnRestNote:
    case msrSyllableKind::kSyllableSkipOnRestNote:
    case msrSyllableKind::kSyllableSkipOnNonRestNote:
    case msrSyllableKind::kSyllableMeasureEnd:
      {
        std::stringstream ss;

        ss <<
          "syllable with measure number '" <<
          measureNumber <<
          "' is no line nor page break"; // JMI v0.9.66

        msrInternalError (
          gServiceRunData->getInputSourceName (),
          fInputStartLineNumber,
          __FILE__, __LINE__,
          ss.str ());
      }
      break;

    case msrSyllableKind::kSyllableLineBreak:
    case msrSyllableKind::kSyllablePageBreak:
      break;
  } // switch

  fSyllableMeasureNumber =
    measureNumber;
}

void msrSyllable::appendSyllableElementToSyllable (
  const msrSyllableElement& syllableElement)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending syllable element " <<
      syllableElement <<
      " to syllable " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fSyllableElementsList.push_back (
    syllableElement);
}

void msrSyllable::appendSyllableToNoteAndSetItsUpLinkToNote (
  const S_msrNote& note)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    note != nullptr,
    "note is empty");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fSyllableUpLinkToNote = note;

/*
  // sanity check JMI ???
  mfAssert (
    __FILE__, __LINE__,
    fSyllableElementsList.size () != 0,
    "fSyllableElementsList is empty");
    */

  // append syllable to note
  note->
    appendSyllableToNote (this);

  // set it upLink to note
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Setting syllable note upLink for:" <<
      asString () <<
      std::endl;
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrSyllable::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrSyllable::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
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
            __FILE__, __LINE__,
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
      __FILE__, __LINE__,
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
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrSyllable::browseData (basevisitor* v)
{}

std::string msrSyllable::syllableWholeNotesAsMsrString () const
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
      case msrNoteKind::kNoteSkipInGraceNotesGroup:
      case msrNoteKind::kNoteInChordInGraceNotesGroup:
      case msrNoteKind::kNoteRegularInChord:
        result =
          wholeNotesAsMsrString (
            fInputStartLineNumber,
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
      // JMI        noteSoundingWholeNotesAsMsrString () <<
              noteDisplayWholeNotesAsMsrString () <<
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
      wholeNotesAsMsrString (
        fInputStartLineNumber,
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

std::string msrSyllable::syllableElementsListAsString () const
{
  std::stringstream ss;

  if (fSyllableElementsList.size ()) {
    std::list<msrSyllableElement>::const_iterator
      iBegin = fSyllableElementsList.begin (),
      iEnd   = fSyllableElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      ss << '\"' << mfDoubleQuoteStringIfNonAlpha ((*i).asString ()) << '\"';
      if (++i == iEnd) break;
      ss << " || ";
    } // for
  }

  return ss.str ();
}

std::string msrSyllable::asString () const
{
  std::stringstream ss;

  ss <<
    "[Syllable " <<
    ", fSyllableElementsList: " <<
    syllableElementsListAsString () <<
    ", fSyllableKind: " << fSyllableKind <<
    ", fSyllableExtendKind: " << fSyllableExtendKind <<
    ", fSyllableStanzaNumber: \"" << fSyllableStanzaNumber << "\"" <<
    ", fSyllableWholeNotes: " << fSyllableWholeNotes.asString () <<
    ", fSyllableTupletFactor: " << fSyllableTupletFactor.asString () <<
    ", fSyllableMeasureNumber: " << fSyllableMeasureNumber <<
    ", line " << fInputStartLineNumber;

  ss <<
    ", " <<
    syllableUpLinkToNoteAsString ();

  switch (fSyllableKind) {
    case msrSyllableKind::kSyllableNone:
      msrInternalError (
        gServiceRunData->getInputSourceName (),
        fInputStartLineNumber,
        __FILE__, __LINE__,
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
    "[Syllable" <<
    " \"" <<
    syllableElementsListAsString () <<
    "\", " << fSyllableKind <<
    ", " << fSyllableExtendKind <<
    ", \"" << fSyllableStanzaNumber << "\"" <<
    ", " << fSyllableWholeNotes.asString () <<
    ", " << fSyllableTupletFactor.asString () <<
    ", fSyllableMeasureNumber: " << fSyllableMeasureNumber <<
    ", line " << fInputStartLineNumber;

  ss << ']';

  return ss.str ();
}

void msrSyllable::print (std::ostream& os) const
{
  os <<
    "[Syllable" <<
    ", fSyllableKind: " <<
    fSyllableKind <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 32;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fSyllableElementsList" << ": " <<
    syllableElementsListAsString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSyllableKind" << ": " <<
    msrSyllableKindAsString (fSyllableKind) <<
    std::endl <<

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
    fSyllableWholeNotes.asString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSyllableTupletFactor" << ": " <<
    fSyllableTupletFactor.asString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSyllableMeasureNumber" << ": " <<
    fSyllableMeasureNumber <<
    std::endl <<

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
        fInputStartLineNumber,
        __FILE__, __LINE__,
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
// constants
const std::string msrStanza::K_STANZA_NUMBER_UNKNOWN_ = "-1";
const std::string msrStanza::K_STANZA_NAME_UNKNOWN_   = "Unknown stanza";

S_msrStanza msrStanza::create (
  int           inputLineNumber,
  const std::string& stanzaNumber,
  const S_msrVoice&    stanzaUpLinkToVoice)
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
  int           inputLineNumber,
  const std::string& stanzaNumber,
  const S_msrVoice&    stanzaUpLinkToVoice)
    : msrElement (inputLineNumber)
{
  // set stanza number and kind
  fStanzaNumber = stanzaNumber;

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    stanzaUpLinkToVoice != nullptr,
    "stanzaUpLinkToVoice is null");
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
    fStanzaNumber;

  fStanzaAlphabeticName =
    fStanzaUpLinkToVoice->getVoiceAlphabeticName () +
    "_Stanza_" +
      mfStringNumbersToEnglishWords (
        mfMakeSingleWordFromString (
          fStanzaNumber));

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Initializing stanza " << getStanzaName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStanzaTextPresent = false;

  fStanzaMeasureCurrentAccumulatedWholeNotesDuration = msrWholeNotes (0, 1);
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

  S_msrStanza
    newbornClone =
      msrStanza::create (
        fInputStartLineNumber,
        fStanzaNumber,
        containingVoice);

  // number

  // kind

  // names
  newbornClone->fStanzaName =
    fStanzaName;
  newbornClone->fStanzaAlphabeticName =
    fStanzaAlphabeticName;

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

  S_msrStanza
    stanzaDeepClone =
      msrStanza::create (
        fInputStartLineNumber,
        fStanzaNumber,
        containingVoice);

  // number

  // kind

  // name
  stanzaDeepClone->fStanzaName =
    fStanzaName;
  stanzaDeepClone->fStanzaAlphabeticName =
    fStanzaAlphabeticName;

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
  const S_msrSyllable& syllable,
  const msrWholeNotes& partDrawingMeasurePosition)
{
  // compute position delta
  msrWholeNotes
    positionsDelta =
      partDrawingMeasurePosition
        -
      fStanzaMeasureCurrentAccumulatedWholeNotesDuration;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending syllable " << syllable <<
      " to stanza " << getStanzaName () <<
      ", partDrawingMeasurePosition: " <<
      partDrawingMeasurePosition.asString () <<
      ", fStanzaMeasureCurrentAccumulatedWholeNotesDuration: " <<
      fStanzaMeasureCurrentAccumulatedWholeNotesDuration.asString () <<
      ", positionsDelta: " <<
      positionsDelta.asString () <<
      ", line " << syllable->getInputStartLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // pad up stanza if relevant
  if (positionsDelta.getNumerator () != 0) {
    // create a skip on rest note syllable
    S_msrSyllable
      skipRestNoteSyllable =
        msrSyllable::create (
          syllable->getInputStartLineNumber (),
          msrSyllableKind::kSyllableSkipOnRestNote,
          msrSyllableExtendKind::kSyllableExtend_NONE,
          fStanzaNumber,
          positionsDelta,
          msrTupletFactor (1, 1),
          this);

    // append it to this stanza
    fSyllables.push_back (skipRestNoteSyllable);

    // account for syllable length
    fStanzaMeasureCurrentAccumulatedWholeNotesDuration +=
      skipRestNoteSyllable->getSyllableWholeNotes ();
  }

  // append syllable to this stanza
  fSyllables.push_back (syllable);

  // account for syllable length
  fStanzaMeasureCurrentAccumulatedWholeNotesDuration +=
    syllable->getSyllableWholeNotes ();

  // set the syllable's stanza uplink
  syllable->
    setSyllableUpLinkToStanza (this);

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
        fInputStartLineNumber,
        __FILE__, __LINE__,
        "syllable type has not been set");
      break;
  } // switch
}

void msrStanza::appendSyllableToStanzaClone (
  const S_msrSyllable& syllable)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending syllable " << syllable <<
      " to stanza clone " << getStanzaName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append syllable to this stanza
  fSyllables.push_back (syllable);

  // account for syllable length
  fStanzaMeasureCurrentAccumulatedWholeNotesDuration +=
    syllable->getSyllableWholeNotes ();

  // set the syllable's stanza uplink
  syllable->
    setSyllableUpLinkToStanza (this);

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
        fInputStartLineNumber,
        __FILE__, __LINE__,
        "syllable type has not been set");
      break;
  } // switch
}

void msrStanza::appendMeasureEndSyllableToStanza (
  int                  inputLineNumber,
  const msrWholeNotes& partDrawingMeasurePosition)
{
  // compute position delta
  msrWholeNotes
    positionsDelta =
      partDrawingMeasurePosition
        -
      fStanzaMeasureCurrentAccumulatedWholeNotesDuration;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending a measure end syllable " <<
      " to stanza " << getStanzaName () <<
      ", partDrawingMeasurePosition: " <<
      partDrawingMeasurePosition <<
      ", fStanzaMeasureCurrentAccumulatedWholeNotesDuration: " <<
      fStanzaMeasureCurrentAccumulatedWholeNotesDuration <<
      ", positionsDelta: " <<
      positionsDelta.asString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // pad up stanza if relevant
  if (positionsDelta.getNumerator () > 0) { // JMI v0.9.70 it may be negative...
    // create a skip on rest note syllable
    S_msrSyllable
      skipRestNoteSyllable =
        msrSyllable::create (
          inputLineNumber,
          msrSyllableKind::kSyllableSkipOnRestNote,
          msrSyllableExtendKind::kSyllableExtend_NONE,
          fStanzaNumber,
          positionsDelta,
          msrTupletFactor (1, 1),
          this);

    // append it to this stanza
    fSyllables.push_back (skipRestNoteSyllable);

    // account for syllable length
    fStanzaMeasureCurrentAccumulatedWholeNotesDuration +=
      skipRestNoteSyllable->getSyllableWholeNotes ();
  }

  // create measure end syllable
  S_msrSyllable
    measureEndSyllable =
      msrSyllable::create (
        inputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
        msrSyllableKind::kSyllableMeasureEnd,
        msrSyllableExtendKind::kSyllableExtend_NONE,
        fStanzaNumber,
        msrWholeNotes (0, 1),
        msrTupletFactor (1, 1),
        this);

  // append syllable to this stanza
  appendSyllableToStanzaClone (measureEndSyllable);

  // reset measure whole notes
  fStanzaMeasureCurrentAccumulatedWholeNotesDuration = msrWholeNotes (0, 1);

  --gIndenter;
}

S_msrSyllable msrStanza::appendLineBreakSyllableToStanza (
  int                inputLineNumber,
  const std::string& measureNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending a 'LineBreak' syllable" <<
      " to stanza " << getStanzaName () <<
      ", measureNumber: " << measureNumber <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // create line break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
        msrSyllableKind::kSyllableLineBreak,
        msrSyllableExtendKind::kSyllableExtend_NONE,
        fStanzaNumber,
        msrWholeNotes (0, 1),
        msrTupletFactor (),
        this);

  syllable->
    setSyllableMeasureNumber (
      measureNumber);

  // append syllable to this stanza
  appendSyllableToStanzaClone (syllable);

  --gIndenter;

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendPageBreakSyllableToStanza (
  int                inputLineNumber,
  const std::string& measureNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending a 'PageBreak' syllable" <<
      " to stanza " << getStanzaName () <<
      ", measureNumber: " << measureNumber <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // create page break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
        msrSyllableKind::kSyllablePageBreak,
        msrSyllableExtendKind::kSyllableExtend_NONE,
        fStanzaNumber,
        msrWholeNotes (0, 1),
        msrTupletFactor (),
        this);

  syllable->
    setSyllableMeasureNumber (
      measureNumber);

  // append syllable to this stanza
  appendSyllableToStanzaClone (syllable);

  --gIndenter;

  // and return it
  return syllable;
}

void msrStanza::appendPaddingNoteToStanza ( // JMI USELESS??? v0.9.70
  int                  inputLineNumber,
  const msrWholeNotes& forwardStepLength)
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
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // JMI TO DO ??? v0.9.70

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
      __FILE__, __LINE__,
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
            __FILE__, __LINE__,
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
      __FILE__, __LINE__,
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
            __FILE__, __LINE__,
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
      __FILE__, __LINE__,
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
      __FILE__, __LINE__,
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
      std::vector<S_msrSyllable>::const_iterator
        iBegin = fSyllables.begin (),
        iEnd   = fSyllables.end (),
        i      = iBegin;

      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no std::endl here
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
