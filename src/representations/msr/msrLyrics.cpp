/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>

#include "visitor.h"

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msrWae.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrMeasureConstants.h"

#include "msrLyrics.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrSyllable msrSyllable::create (
  int                    inputLineNumber,
  const S_msrMeasure&    upLinkToMeasure,
  msrSyllableKind        syllableKind,
  msrSyllableExtendKind  syllableExtendKind,
  const std::string&     syllableStanzaNumber,
  const Rational&        syllableWholeNotes,
  const msrTupletFactor& syllableTupletFactor,
  const S_msrStanza&     syllableUpLinkToStanza)
{
  msrSyllable* o =
    new msrSyllable (
      inputLineNumber,
      upLinkToMeasure,
      syllableKind,
      syllableExtendKind,
      syllableStanzaNumber,
      syllableWholeNotes,
      syllableTupletFactor,
      syllableUpLinkToStanza);
  assert (o != nullptr);
  return o;
}

S_msrSyllable msrSyllable::create (
  int                    inputLineNumber,
  msrSyllableKind        syllableKind,
  msrSyllableExtendKind  syllableExtendKind,
  const std::string&     syllableStanzaNumber,
  const Rational&        syllableWholeNotes,
  const msrTupletFactor& syllableTupletFactor,
  const S_msrStanza&     syllableUpLinkToStanza)
{
  return
    msrSyllable::create (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      syllableKind,
      syllableExtendKind,
      syllableStanzaNumber,
      syllableWholeNotes,
      syllableTupletFactor,
      syllableUpLinkToStanza);
}

S_msrSyllable msrSyllable::createWithNextMeasurePuristNumber ( // JMI superflous??? v0.9.66
  int                    inputLineNumber,
  const S_msrMeasure&    upLinkToMeasure,
  msrSyllableKind        syllableKind,
  msrSyllableExtendKind  syllableExtendKind,
  const std::string&     syllableStanzaNumber,
  const Rational&        syllableWholeNotes,
  const msrTupletFactor& syllableTupletFactor,
  const S_msrStanza&     syllableUpLinkToStanza,
  int                    syllableNextMeasurePuristNumber)
{
  msrSyllable* o =
    msrSyllable::create (
      inputLineNumber,
      upLinkToMeasure,
      syllableKind,
      syllableExtendKind,
      syllableStanzaNumber,
      syllableWholeNotes,
      syllableTupletFactor,
      syllableUpLinkToStanza);
  assert (o != nullptr);

  o->
    setSyllableNextMeasurePuristNumber (
      syllableNextMeasurePuristNumber);

  return o;
}

msrSyllable::msrSyllable (
  int                    inputLineNumber,
  const S_msrMeasure&    upLinkToMeasure,
  msrSyllableKind        syllableKind,
  msrSyllableExtendKind  syllableExtendKind,
  const std::string&     syllableStanzaNumber,
  const Rational&        syllableWholeNotes,
  const msrTupletFactor& syllableTupletFactor,
  const S_msrStanza&     syllableUpLinkToStanza)
    : msrMeasureElement (
        inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    syllableUpLinkToStanza != nullptr,
    "syllableUpLinkToStanza is null");

  // set syllable's stanza upLink
  fSyllableUpLinkToStanza =
    syllableUpLinkToStanza;

  fSyllableKind = syllableKind;

  fSyllableExtendKind = syllableExtendKind;

  fSyllableStanzaNumber = syllableStanzaNumber;

  // fSyllableUpLinkToNote will be set
  // by appendSyllableToNoteAndSetItsUpLinkToNote () later

  fSyllableWholeNotes = syllableWholeNotes;

  fSyllableTupletFactor = syllableTupletFactor;

  fSyllableNextMeasurePuristNumber = -1;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Creating a syllable containing:" <<
      std::endl;

    ++gIndenter;

    print (gLogStream);

    --gIndenter;
  }
#endif
}

msrSyllable::~msrSyllable ()
{}

S_msrSyllable msrSyllable::createSyllableNewbornClone (
  const S_msrPart& containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Creating a newborn clone of syllable '" <<
      asString () <<
      "'" <<
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingPart != nullptr,
    "containingPart is null");

  S_msrSyllable
    newbornClone =
      msrSyllable::create (
        fInputLineNumber,
        gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        fSyllableKind,
        fSyllableExtendKind,
        fSyllableStanzaNumber,
        fSyllableWholeNotes,
        fSyllableTupletFactor,
        fSyllableUpLinkToStanza);

  // append the lyric texts to the syllable clone
  for (
    std::list<std::string>::const_iterator i = fSyllableTextsList.begin ();
    i!=fSyllableTextsList.end ();
    ++i
  ) {
    newbornClone->
      appendLyricTextToSyllable ((*i));
  } // for

  // dont't set 'newbornClone->fSyllableUpLinkToStanza'
  // nor 'newbornClone->fSyllableUpLinkToNote',
  // this will be done by the caller

  newbornClone->fSyllableUpLinkToNote =
    fSyllableUpLinkToNote; // TEMP

  return newbornClone;
}

S_msrSyllable msrSyllable::createSyllableDeepClone (
  const S_msrPart& containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Creating a newborn clone of syllable '" <<
      asString () <<
      "'" <<
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingPart != nullptr,
    "containingPart is null");

  S_msrSyllable
    deepClone =
      msrSyllable::create (
        fInputLineNumber,
        gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        fSyllableKind,
        fSyllableExtendKind,
        fSyllableStanzaNumber,
        fSyllableWholeNotes,
        fSyllableTupletFactor,
        fSyllableUpLinkToStanza);

  // append the lyric texts to the syllable deep clone
  for (
    std::list<std::string>::const_iterator i = fSyllableTextsList.begin ();
    i!=fSyllableTextsList.end ();
    ++i
  ) {
    deepClone->
      appendLyricTextToSyllable ((*i));
  } // for

  // dont't set 'newbornClone->fSyllableUpLinkToStanza'
  // nor 'newbornClone->fSyllableUpLinkToNote',
  // this will be done by the caller

  deepClone->fSyllableUpLinkToNote =
    fSyllableUpLinkToNote; // TEMP

  return deepClone;
}

void msrSyllable::setSyllableUpLinkToMeasure (
  const S_msrMeasure& measure)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of syllable " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fSyllableUpLinkToMeasure = measure;
}

void msrSyllable:: setSyllableNextMeasurePuristNumber (
  int puristMeasureNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Setting syllable next measure purist number to " <<
      fSyllableNextMeasurePuristNumber <<
      std::endl;
  }
#endif

  switch (fSyllableKind) {
    case msrSyllableKind::kSyllableNone:
    case msrSyllableKind::kSyllableSingle:
    case msrSyllableKind::kSyllableBegin:
    case msrSyllableKind::kSyllableMiddle:
    case msrSyllableKind::kSyllableEnd:
    case msrSyllableKind::kSyllableOnRestNote:
    case msrSyllableKind::kSyllableSkipRestNote:
    case msrSyllableKind::kSyllableSkipNonRestNote:
    case msrSyllableKind::kSyllableMeasureEnd:
      {
        std::stringstream s;

        s <<
          "syllable with next measure purist number '" <<
          puristMeasureNumber <<
          "' is no line nor page break"; // JMI v0.9.66

        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrSyllableKind::kSyllableLineBreak:
    case msrSyllableKind::kSyllablePageBreak:
      break;
  } // switch

  fSyllableNextMeasurePuristNumber =
    puristMeasureNumber;
}

// void msrSyllable::setMeasureElementMeasurePosition (
//   const S_msrMeasure measure,
//   const Rational&    measurePosition,
//   const std::string&      context)
// {
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
//     gLogStream <<
//       "Setting syllable's measure position of " << asString () <<
//       " to " <<
//       measurePosition <<
//       " (was " <<
//       fMeasureElementMeasurePosition <<
//       ") in measure " <<
//       measure->asShortString () <<
//       " (measureElementMeasureNumber: " <<
//       fBarLineUpLinkToMeasure->getMeasureNumber () <<
//       "), context: \"" <<
//       context <<
//       "\"" <<
//       std::endl;
//   }
// #endif
//
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     measurePosition != msrMoment::K_MEASURE_POSITION_UNKNOWN,
//     "measurePosition == msrMoment::K_MEASURE_POSITION_UNKNOWN");
//
//   // set syllable's measure position
//   fMeasureElementMeasurePosition = measurePosition;
// }

void msrSyllable::appendLyricTextToSyllable (const std::string& text)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending text \"" <<
      text <<
      "\" to the texts list of syllable '" <<
      asString () <<
      "'" <<
      std::endl;
  }
#endif

  fSyllableTextsList.push_back (
    text);
}

void msrSyllable::appendSyllableToNoteAndSetItsUpLinkToNote (
  const S_msrNote& note)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    note != nullptr,
    "note is empty");

  fSyllableUpLinkToNote = note;

/*
  // sanity check JMI ???
  mfAssert (
    __FILE__, __LINE__,
    fSyllableTextsList.size () != 0,
    "fSyllableTextsList is empty");
    */

  // append syllable to note
  note->
    appendSyllableToNote (this);

  // set it upLink to note
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Setting syllable note upLink for:" <<
      std::endl;

    ++gIndenter;

    gLogStream <<
      asString () <<
    // JMI    "to '" << note->asString () <<
      ", line " << note->getInputLineNumber () <<
      std::endl;

    --gIndenter;
  }
#endif
}

void msrSyllable::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSyllable::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrSyllable>*
    p =
      dynamic_cast<visitor<S_msrSyllable>*> (v)) {
        S_msrSyllable elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSyllable::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrSyllable::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSyllable::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrSyllable>*
    p =
      dynamic_cast<visitor<S_msrSyllable>*> (v)) {
        S_msrSyllable elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSyllable::visitEnd ()" <<
            std::endl;
        }
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
      case msrNoteKind::kNote_UNKNOWN:
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
            fInputLineNumber,
            fSyllableWholeNotes);
        break;

      case msrNoteKind::kNoteSkipInMeasure:
      case msrNoteKind::kNoteRegularInTuplet:
      case msrNoteKind::kNoteRestInTuplet:
      case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      case msrNoteKind::kNoteUnpitchedInTuplet:
        {
          std::stringstream s;

          s <<
            fSyllableUpLinkToNote->
      // JMI        noteSoundingWholeNotesAsMsrString () <<
              noteDisplayWholeNotesAsMsrString () <<
            "*" <<
            fSyllableTupletFactor.getTupletNormalNotes () <<
            '/' <<
            fSyllableTupletFactor.getTupletActualNotes ();

          result = s.str ();
        }
        break;
    } // switch
  }

  else {
    result =
      wholeNotesAsMsrString (
        fInputLineNumber,
          fSyllableWholeNotes);
  }

  return result;
}

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

    case msrSyllableKind::kSyllableSkipRestNote:
      result = "kSyllableSkipRestNote";
      break;
    case msrSyllableKind::kSyllableSkipNonRestNote:
      result = "kSyllableSkipNonRestNote";
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

std::string msrSyllableExtendKindAsString (
  msrSyllableExtendKind syllableExtendKind)
{
  std::string result;

  switch (syllableExtendKind) {
    case msrSyllableExtendKind::kSyllableExtendNone:
      result = "kSyllableExtendNone";
      break;
    case msrSyllableExtendKind::kSyllableExtendEmpty:
      result = "kSyllableExtendEmpty";
      break;
    case msrSyllableExtendKind::kSyllableExtendSingle:
      result = "kSyllableExtendSingle";
      break;
    case msrSyllableExtendKind::kSyllableExtendStart:
      result = "kSyllableExtendStart";
      break;
    case msrSyllableExtendKind::kSyllableExtendContinue:
      result = "kSyllableExtendContinue";
      break;
    case msrSyllableExtendKind::kSyllableExtendStop:
      result = "kSyllableExtendStop";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrSyllableExtendKind& elt)
{
  os << msrSyllableExtendKindAsString (elt);
  return os;
}

std::string msrSyllable::syllableUpLinkToNoteAsString () const
{
  std::string result;

  if (fSyllableUpLinkToNote) {
    result = fSyllableUpLinkToNote->asString ();
  }
  else {
    result = "[NONE]";
  }

  return result;
}

std::string msrSyllable::syllableTextsListAsString () const
{
  std::stringstream s;

  if (fSyllableTextsList.size ()) {
    std::list<std::string>::const_iterator
      iBegin = fSyllableTextsList.begin (),
      iEnd   = fSyllableTextsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s << mfDoubleQuoteStringIfNonAlpha (*i);
      if (++i == iEnd) break;
      s << ", ";
    } // for
  }

  return s.str ();
}

void msrSyllable::writeTextsList (
  const std::list<std::string>& textsList,
  std::ostream&                 os)
{
  os << '[';

  if (textsList.size ()) {
    std::list<std::string>::const_iterator
      iBegin = textsList.begin (),
      iEnd   = textsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << mfDoubleQuoteStringIfNonAlpha (*i);
      if (++i == iEnd) break;
      os << ", ";
    } // for
  }

  os << ']';
}

std::string msrSyllable::asString () const
{
  std::stringstream s;

  s <<
    "Syllable '" <<
    fSyllableKind <<
    "', syllableExtendKind: " <<
      fSyllableExtendKind <<
    ", fSyllableStanzaNumber: \"" << fSyllableStanzaNumber << "\"" <<
    ", fSyllableWholeNotes: " << fSyllableWholeNotes <<
    ", fSyllableTupletFactor: " << fSyllableTupletFactor <<
    ", fSyllableNextMeasurePuristNumber: " << fSyllableNextMeasurePuristNumber <<
    ", line " << fInputLineNumber <<
    ", syllableTextsList: ";

  writeTextsList (
    fSyllableTextsList,
    s);

  s <<
    ", " <<
    syllableUpLinkToNoteAsString ();

  switch (fSyllableKind) {
    case msrSyllableKind::kSyllableNone:
      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "syllable type has not been set");
      break;

    case msrSyllableKind::kSyllableSingle:
    case msrSyllableKind::kSyllableBegin:
    case msrSyllableKind::kSyllableMiddle:
    case msrSyllableKind::kSyllableEnd:
    case msrSyllableKind::kSyllableOnRestNote:
    case msrSyllableKind::kSyllableSkipRestNote:
    case msrSyllableKind::kSyllableSkipNonRestNote:
    case msrSyllableKind::kSyllableMeasureEnd:
    case msrSyllableKind::kSyllableLineBreak:
    case msrSyllableKind::kSyllablePageBreak:
      break;
  } // switch

  s << // JMI LENK
    ", attached to note: ";
  if (fSyllableUpLinkToNote) {
    s <<
      fSyllableUpLinkToNote->asShortString ();
  }
  else {
    s << "[NONE]";
  }

  s <<
    ", in stanza: ";
  if (fSyllableUpLinkToStanza) {
    s <<
      fSyllableUpLinkToStanza->getStanzaName ();
  }
  else {
    s << "[NONE]";
  }

  return s.str ();
}

void msrSyllable::print (std::ostream& os) const
{
  os <<
    "[Syllable" <<
    ", syllableKind: " <<
    msrSyllableKindAsString (fSyllableKind) <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 32;

  os << std::left <<
    std::setw (fieldWidth) <<
    "syllableTextsList" << ": ";

  writeTextsList (
    fSyllableTextsList,
    os);
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "syllableKind" << ": " <<
    msrSyllableKindAsString (fSyllableKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "syllableExtendKind" << ": " <<
    msrSyllableExtendKindAsString (
      fSyllableExtendKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSyllableStanzaNumber" << ": \"" <<
    fSyllableStanzaNumber <<
    "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSyllableWholeNotes" << ": " <<
    fSyllableWholeNotes <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSyllableTupletFactor" << ": " <<
    fSyllableTupletFactor.asString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fSyllableNextMeasurePuristNumber" << ": " <<
    fSyllableNextMeasurePuristNumber <<
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
        gGlobalServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "syllable type has not been set");
      break;

    case msrSyllableKind::kSyllableSingle:
    case msrSyllableKind::kSyllableBegin:
    case msrSyllableKind::kSyllableMiddle:
    case msrSyllableKind::kSyllableEnd:
    case msrSyllableKind::kSyllableOnRestNote:
    case msrSyllableKind::kSyllableSkipRestNote:
    case msrSyllableKind::kSyllableSkipNonRestNote:
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
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
// constants
const std::string msrStanza::K_STANZA_NUMBER_UNKNOWN = "-1";
const std::string msrStanza::K_STANZA_NAME_UNKNOWN   = "Unknown stanza";

S_msrStanza msrStanza::create (
  int           inputLineNumber,
  const std::string& stanzaNumber,
  const S_msrVoice&    stanzaUpLinkToVoice)
{
  msrStanza* o =
    new msrStanza (
      inputLineNumber,
      stanzaNumber,
      stanzaUpLinkToVoice);
  assert (o != nullptr);
  return o;
}

msrStanza::msrStanza (
  int           inputLineNumber,
  const std::string& stanzaNumber,
  const S_msrVoice&    stanzaUpLinkToVoice)
    : msrElement (inputLineNumber)
{
  // set stanza number and kind
  fStanzaNumber = stanzaNumber;

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    stanzaUpLinkToVoice != nullptr,
    "stanzaUpLinkToVoice is null");

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
      mfStringNumbersToEnglishWords (
        mfMakeSingleWordFromString (
          fStanzaNumber));

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Initializing stanza " << getStanzaName () <<
      std::endl;
  }
#endif

  fStanzaTextPresent = false;

  fStanzaMeasureWholeNotesDuration = Rational (0, 1);
}

msrStanza::~msrStanza ()
{}

S_msrStanza msrStanza::createStanzaNewbornClone (
  const S_msrVoice& containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Creating a newborn clone of stanza \"" <<
      getStanzaName () <<
      "\" in voice \"" <<
      containingVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrStanza
    newbornClone =
      msrStanza::create (
        fInputLineNumber,
        fStanzaNumber,
        containingVoice);

  // number

  // kind

  // name
  newbornClone->fStanzaName =
    fStanzaName;

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Creating a deep clone of stanza \"" <<
      getStanzaName () <<
      "\" in voice \"" <<
      containingVoice->getVoiceName () <<
      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");

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
  const S_msrSyllable& syllable)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending syllable '" << syllable->asString () <<
      "' to stanza " << getStanzaName () <<
      std::endl;
  }
#endif

  // append the syllable to this stanza
  fSyllables.push_back (syllable);

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
    case msrSyllableKind::kSyllableSkipRestNote:
    case msrSyllableKind::kSyllableSkipNonRestNote:
    case msrSyllableKind::kSyllableMeasureEnd:
    case msrSyllableKind::kSyllableLineBreak:
    case msrSyllableKind::kSyllablePageBreak:
      break;

    case msrSyllableKind::kSyllableNone:
      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "syllable type has not been set");
      break;
  } // switch

/*
  // get the syllable's sounding whole notes JMI
  Rational
    syllableSoundingWholeNotes =
      syllable->
        getSyllableUpLinkToNote ()->
          getMeasureElementSoundingWholeNotes ();

  // update the stanza's measure whole notes
  fStanzaMeasureWholeNotesDuration +=syllableSoundingWholeNotes;
  */
}

S_msrSyllable msrStanza::appendRestSyllableToStanza (
  int             inputLineNumber,
  const Rational& wholeNotes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending 'Rest' syllable" <<
      " to stanza " << getStanzaName () <<
      ", whole notes: " << wholeNotes <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // create stanza rest syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        msrSyllableKind::kSyllableSkipRestNote,
        msrSyllableExtendKind::kSyllableExtendNone,
        fStanzaNumber,
        wholeNotes,
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  --gIndenter;

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendSkipSyllableToStanza (
  int             inputLineNumber,
  const Rational& wholeNotes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending 'Skip' syllable " <<
      " to stanza " << getStanzaName () <<
      ", whole notes: " << wholeNotes <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // create stanza skip syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        msrSyllableKind::kSyllableSkipRestNote,
        msrSyllableExtendKind::kSyllableExtendNone,
        fStanzaNumber,
        wholeNotes,
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  --gIndenter;

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendMeasureEndSyllableToStanza (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending 'Measure end' syllable " <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // create stanza skip syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        msrSyllableKind::kSyllableMeasureEnd,
        msrSyllableExtendKind::kSyllableExtendNone,
        fStanzaNumber,
        0, // wholeNotes
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // reset measure whole notes
  fStanzaMeasureWholeNotesDuration = Rational (0, 1);

  --gIndenter;

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendMelismaSyllableToStanza (
  int             inputLineNumber,
  msrSyllableKind syllableKind,
  const Rational& wholeNotes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending '" <<
      msrSyllableKindAsString (syllableKind) <<
      "' syllable" <<
      " to stanza " << getStanzaName () <<
      ", whole notes: " << wholeNotes <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // create stanza melisma syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        syllableKind,
        msrSyllableExtendKind::kSyllableExtendNone,
        fStanzaNumber,
        wholeNotes,
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  --gIndenter;

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendLineBreakSyllableToStanza (
  int inputLineNumber,
  int nextMeasurePuristNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending a 'LineBreak' syllable" <<
      " to stanza " << getStanzaName () <<
      ", nextMeasurePuristNumber: " << nextMeasurePuristNumber <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // create line break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        msrSyllableKind::kSyllableLineBreak,
        msrSyllableExtendKind::kSyllableExtendNone,
        fStanzaNumber,
        0, // whole notes
        msrTupletFactor (),
        this);

  syllable->
    setSyllableNextMeasurePuristNumber (
      nextMeasurePuristNumber);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  --gIndenter;

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendPageBreakSyllableToStanza (
  int inputLineNumber,
  int nextMeasurePuristNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending a 'PageBreak' syllable" <<
      " to stanza " << getStanzaName () <<
      ", nextMeasurePuristNumber: " << nextMeasurePuristNumber <<
      ", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // create page break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
        msrSyllableKind::kSyllablePageBreak,
        msrSyllableExtendKind::kSyllableExtendNone,
        fStanzaNumber,
        0, // whole notes
        msrTupletFactor (),
        this);

  syllable->
    setSyllableNextMeasurePuristNumber (
      nextMeasurePuristNumber);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  --gIndenter;

  // and return it
  return syllable;
}

void msrStanza::padUpToMeasureWholeNotesDurationInStanza (
  int             inputLineNumber,
  const Rational& wholeNotes)
{
  // JMI ???
}

void msrStanza::appendPaddingNoteToStanza (
  int             inputLineNumber,
  const Rational& forwardStepLength)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending padding note" <<
      ", forwardStepLength: " <<
      forwardStepLength <<
      ", to stanza \"" <<
      fStanzaName <<
      "\" in voice \"" <<
      fStanzaUpLinkToVoice->getVoiceName () <<
      "\", line " << inputLineNumber <<
      std::endl;
  }
#endif

  ++gIndenter;

  // JMI TO DO ???

  --gIndenter;
}

void msrStanza::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStanza::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrStanza>*
    p =
      dynamic_cast<visitor<S_msrStanza>*> (v)) {
        S_msrStanza elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStanza::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrStanza::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStanza::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrStanza>*
    p =
      dynamic_cast<visitor<S_msrStanza>*> (v)) {
        S_msrStanza elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStanza::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrStanza::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStanza::browseData ()" <<
      std::endl;
  }

  ++gIndenter;

  // browse the syllables
  int n = fSyllables.size ();
  for (int i = 0; i < n; ++i) {
    // browse the syllable
    msrBrowser<msrSyllable> browser (v);
    browser.browse (*fSyllables [i]);
  } // for

  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% <== msrStanza::browseData ()" <<
      std::endl;
  }
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
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
