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
S_msrSyllable msrSyllable::create (
  int                       inputLineNumber,
  const S_msrMeasure&       upLinkToMeasure,
  msrSyllableKind           syllableKind,
  msrSyllableExtendTypeKind syllableExtendKind,
  const std::string&        syllableStanzaNumber,
  const msrWholeNotes&      syllableWholeNotes,
  const msrTupletFactor&    syllableTupletFactor,
  const S_msrStanza&        syllableUpLinkToStanza)
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
  int                       inputLineNumber,
  msrSyllableKind           syllableKind,
  msrSyllableExtendTypeKind syllableExtendKind,
  const std::string&        syllableStanzaNumber,
  const msrWholeNotes&      syllableWholeNotes,
  const msrTupletFactor&    syllableTupletFactor,
  const S_msrStanza&        syllableUpLinkToStanza)
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

// S_msrSyllable msrSyllable::createWithNextMeasurePuristNumber ( // JMI superflous v0.9.68
//   int                       inputLineNumber,
//   const S_msrMeasure&       upLinkToMeasure,
//   msrSyllableKind           syllableKind,
//   msrSyllableExtendTypeKind syllableExtendKind,
//   const std::string&        syllableStanzaNumber,
//   const msrWholeNotes&      syllableWholeNotes,
//   const msrTupletFactor&    syllableTupletFactor,
//   const S_msrStanza&        syllableUpLinkToStanza,
//   int                       syllableNextMeasurePuristNumber)
// {
//   msrSyllable* obj =
//     msrSyllable::create (
//       inputLineNumber,
//       upLinkToMeasure,
//       syllableKind,
//       syllableExtendKind,
//       syllableStanzaNumber,
//       syllableWholeNotes,
//       syllableTupletFactor,
//       syllableUpLinkToStanza);
//   assert (obj != nullptr);
//
//   obj->
//     setSyllableNextMeasurePuristNumber (
//       syllableNextMeasurePuristNumber);
//
//   return obj;
// }

msrSyllable::msrSyllable (
  int                       inputLineNumber,
  const S_msrMeasure&       upLinkToMeasure,
  msrSyllableKind           syllableKind,
  msrSyllableExtendTypeKind syllableExtendKind,
  const std::string&        syllableStanzaNumber,
  const msrWholeNotes&      syllableWholeNotes,
  const msrTupletFactor&    syllableTupletFactor,
  const S_msrStanza&        syllableUpLinkToStanza)
    : msrMeasureElement (
        inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    syllableUpLinkToStanza != nullptr,
    "syllableUpLinkToStanza is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Creating a syllable containing:" <<
      std::endl;

    ++gIndenter;

    print (gLog);

    --gIndenter;
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
  for (
    std::list<std::string>::const_iterator i = fSyllableTextsList.begin ();
    i!=fSyllableTextsList.end ();
    ++i
  ) {
    newbornClone->
      appendLyricTextToSyllable ((*i));
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
  for (
    std::list<std::string>::const_iterator i = fSyllableTextsList.begin ();
    i!=fSyllableTextsList.end ();
    ++i
  ) {
    deepClone->
      appendLyricTextToSyllable ((*i));
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

void msrSyllable:: setSyllableNextMeasurePuristNumber (
  int puristMeasureNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Setting syllable next measure purist number to " <<
      fSyllableNextMeasurePuristNumber;

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
    case msrSyllableKind::kSyllableSkipRestNote:
    case msrSyllableKind::kSyllableSkipNonRestNote:
    case msrSyllableKind::kSyllableMeasureEnd:
      {
        std::stringstream ss;

        ss <<
          "syllable with next measure purist number '" <<
          puristMeasureNumber <<
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

  fSyllableNextMeasurePuristNumber =
    puristMeasureNumber;
}

// void msrSyllable::setMeasurePosition (
//   const S_msrMeasure measure,
//   const msrWholeNotes&    measurePosition,
//   const std::string&      context)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasurePositions ()) {
//     gLog <<
//       "Setting syllable's measure position of " << asString () <<
//       " to " <<
//       measurePosition.asString () <<
//       " (was " <<
//       fMeasurePosition.asString () <<
//       ") in measure " <<
//       measure->asShortString () <<
//       " (measureElementMeasureNumber: " <<
//       fBarLineUpLinkToMeasure->getMeasureNumber () <<
//       "), context: \"" <<
//       context <<
//       "\"" <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     measurePosition != K_MEASURE_POSITION_UNKNOWN_,
//     "measurePosition == K_MEASURE_POSITION_UNKNOWN_");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   // set syllable's measure position
//   fMeasurePosition = measurePosition;
// }

void msrSyllable::appendLyricTextToSyllable (const std::string& text)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending text \"" <<
      text <<
      "\" to the texts list of syllable '" <<
      asString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fSyllableTextsList.push_back (
    text);
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
    fSyllableTextsList.size () != 0,
    "fSyllableTextsList is empty");
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
      std::endl;

    ++gIndenter;

    gLog <<
      asString () <<
    // JMI    "to '" << note->asString () <<
      ", line " << note->getInputStartLineNumber () <<
      std::endl;

    --gIndenter;
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

std::string msrSyllableExtendTypeKindAsString (
  msrSyllableExtendTypeKind syllableExtendKind)
{
  std::string result;

  switch (syllableExtendKind) {
    case msrSyllableExtendTypeKind::kSyllableExtendType_NONE:
      result = "kSyllableExtendType_NONE";
      break;
    case msrSyllableExtendTypeKind::kSyllableExtendTypeAbsent:
      result = "kSyllableExtendTypeAbsent";
      break;
    case msrSyllableExtendTypeKind::kSyllableExtendTypeStart:
      result = "kSyllableExtendTypeStart";
      break;
    case msrSyllableExtendTypeKind::kSyllableExtendTypeContinue:
      result = "kSyllableExtendTypeContinue";
      break;
    case msrSyllableExtendTypeKind::kSyllableExtendTypeStop:
      result = "kSyllableExtendTypeStop";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrSyllableExtendTypeKind& elt)
{
  os << msrSyllableExtendTypeKindAsString (elt);
  return os;
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

std::string msrSyllable::syllableTextsListAsString () const
{
  std::stringstream ss;

  if (fSyllableTextsList.size ()) {
    std::list<std::string>::const_iterator
      iBegin = fSyllableTextsList.begin (),
      iEnd   = fSyllableTextsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      ss << mfDoubleQuoteStringIfNonAlpha (*i);
      if (++i == iEnd) break;
      ss << ", ";
    } // for
  }

  return ss.str ();
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
  std::stringstream ss;

  ss <<
    "[Syllable " <<
    ", syllableTextsList: ";

  writeTextsList (
    fSyllableTextsList,
    ss);

  ss <<
    ", fSyllableKind: " << fSyllableKind <<
    ", syllableExtendKind: " << fSyllableExtendKind <<
    ", fSyllableStanzaNumber: \"" << fSyllableStanzaNumber << "\"" <<
    ", fSyllableWholeNotes: " << fSyllableWholeNotes.asString () <<
    ", fSyllableTupletFactor: " << fSyllableTupletFactor.asString () <<
    ", fSyllableNextMeasurePuristNumber: " << fSyllableNextMeasurePuristNumber <<
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
    case msrSyllableKind::kSyllableSkipRestNote:
    case msrSyllableKind::kSyllableSkipNonRestNote:
    case msrSyllableKind::kSyllableMeasureEnd:
    case msrSyllableKind::kSyllableLineBreak:
    case msrSyllableKind::kSyllablePageBreak:
      break;
  } // switch

  ss << // JMI LENK
    ", attached to note: ";
  if (fSyllableUpLinkToNote) {
    ss <<
      fSyllableUpLinkToNote->asShortString ();
  }
  else {
    ss << "[NULL]";
  }

  ss <<
    ", in stanza: ";
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

void msrSyllable::print (std::ostream& os) const
{
  os <<
    "[Syllable" <<
    ", syllableKind: " <<
    msrSyllableKindAsString (fSyllableKind) <<
    ", line " << fInputStartLineNumber <<
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
    msrSyllableExtendTypeKindAsString (
      fSyllableExtendKind) <<
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
  const S_msrSyllable& syllable)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending syllable " << syllable->asString () <<
      " to stanza " << getStanzaName ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
        gServiceRunData->getInputSourceName (),
        fInputStartLineNumber,
        __FILE__, __LINE__,
        "syllable type has not been set");
      break;
  } // switch

/*
  // get the syllable's sounding whole notes JMI
  msrWholeNotes
    syllableSoundingWholeNotes =
      syllable->
        getSyllableUpLinkToNote ()->
          getSoundingWholeNotes ();

  // update the stanza's measure whole notes
  fStanzaMeasureCurrentAccumulatedWholeNotesDuration +=syllableSoundingWholeNotes;
  */
}

// S_msrSyllable msrStanza::appendRestSyllableToStanza (
//   int             inputLineNumber,
//   const msrWholeNotes& wholeNotes)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceLyrics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending 'Rest' syllable" <<
//       " to stanza " << getStanzaName () <<
//       ", whole notes: " << wholeNotes.asString () <<
//       ", line " << inputLineNumber <<
//       std::endl;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   // create stanza rest syllable
//   S_msrSyllable
//     syllable =
//       msrSyllable::create (
//         inputLineNumber,
//         gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
//         msrSyllableKind::kSyllableSkipRestNote,
//         msrSyllableExtendTypeKind::kSyllableExtendType_NONE,
//         fStanzaNumber,
//         wholeNotes,
//         msrTupletFactor (),
//         this);
//
//   // append syllable to this stanza
//   appendSyllableToStanza (syllable);
//
//   --gIndenter;
//
//   // and return it
//   return syllable;
// }
//
// S_msrSyllable msrStanza::appendSkipSyllableToStanza (
//   int             inputLineNumber,
//   const msrWholeNotes& wholeNotes)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceLyrics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending 'Skip' syllable " <<
//       " to stanza " << getStanzaName () <<
//       ", whole notes: " << wholeNotes.asString () <<
//       ", line " << inputLineNumber <<
//       std::endl;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   // create stanza skip syllable
//   S_msrSyllable
//     syllable =
//       msrSyllable::create (
//         inputLineNumber,
//         gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
//         msrSyllableKind::kSyllableSkipRestNote,
//         msrSyllableExtendTypeKind::kSyllableExtendType_NONE,
//         fStanzaNumber,
//         wholeNotes,
//         msrTupletFactor (),
//         this);
//
//   // append syllable to this stanza
//   appendSyllableToStanza (syllable);
//
//   --gIndenter;
//
//   // and return it
//   return syllable;
// }

S_msrSyllable msrStanza::appendMeasureEndSyllableToStanza (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending 'Measure end' syllable " <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // create stanza skip syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
        msrSyllableKind::kSyllableMeasureEnd,
        msrSyllableExtendTypeKind::kSyllableExtendType_NONE,
        fStanzaNumber,
        msrWholeNotes (0, 1),
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // reset measure whole notes
  fStanzaMeasureCurrentAccumulatedWholeNotesDuration = msrWholeNotes (0, 1);

  --gIndenter;

  // and return it
  return syllable;
}

// S_msrSyllable msrStanza::appendMelismaSyllableToStanza (
//   int             inputLineNumber,
//   msrSyllableKind syllableKind,
//   const msrWholeNotes& wholeNotes)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceLyrics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Appending '" <<
//       msrSyllableKindAsString (syllableKind) <<
//       "' syllable" <<
//       " to stanza " << getStanzaName () <<
//       ", whole notes: " << wholeNotes.asString () <<
//       ", line " << inputLineNumber <<
//       std::endl;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   // create stanza melisma syllable
//   S_msrSyllable
//     syllable =
//       msrSyllable::create (
//         inputLineNumber,
//         gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
//         syllableKind,
//         msrSyllableExtendTypeKind::kSyllableExtendType_NONE,
//         fStanzaNumber,
//         wholeNotes,
//         msrTupletFactor (),
//         this);
//
//   // append syllable to this stanza
//   appendSyllableToStanza (syllable);
//
//   --gIndenter;
//
//   // and return it
//   return syllable;
// }

S_msrSyllable msrStanza::appendLineBreakSyllableToStanza (
  int inputLineNumber,
  int nextMeasurePuristNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending a 'LineBreak' syllable" <<
      " to stanza " << getStanzaName () <<
      ", nextMeasurePuristNumber: " << nextMeasurePuristNumber <<
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
        msrSyllableExtendTypeKind::kSyllableExtendType_NONE,
        fStanzaNumber,
        msrWholeNotes (0, 1),
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending a 'PageBreak' syllable" <<
      " to stanza " << getStanzaName () <<
      ", nextMeasurePuristNumber: " << nextMeasurePuristNumber <<
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
        msrSyllableExtendTypeKind::kSyllableExtendType_NONE,
        fStanzaNumber,
        msrWholeNotes (0, 1),
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

// void msrStanza::padUpToMeasureCurrentAccumulatedWholeNotesDurationInStanza (
//   int                  inputLineNumber,
//   const msrWholeNotes& wholeNotes)
// {
//   // JMI ??? v0.9.68
// }

void msrStanza::appendPaddingNoteToStanza (
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

  // JMI TO DO ???

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
