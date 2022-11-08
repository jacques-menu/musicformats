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

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrMeasures.h"

#include "msrLyrics.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrSyllable msrSyllable::create (
  int                   inputLineNumber,
  S_msrMeasure          upLinkToMeasure,
  msrSyllableKind       syllableKind,
  msrSyllableExtendKind syllableExtendKind,
  const string&         syllableStanzaNumber,
  const Rational&       syllableWholeNotes,
  msrTupletFactor       syllableTupletFactor,
  S_msrStanza           syllableUpLinkToStanza)
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

S_msrSyllable msrSyllable::createWithNextMeasurePuristNumber (
  int                   inputLineNumber,
  S_msrMeasure          upLinkToMeasure,
  msrSyllableKind       syllableKind,
  msrSyllableExtendKind syllableExtendKind,
  const string&         syllableStanzaNumber,
  const Rational&       syllableWholeNotes,
  msrTupletFactor       syllableTupletFactor,
  S_msrStanza           syllableUpLinkToStanza,
  int                   syllableNextMeasurePuristNumber)
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
  int                   inputLineNumber,
  S_msrMeasure          upLinkToMeasure,
  msrSyllableKind       syllableKind,
  msrSyllableExtendKind syllableExtendKind,
  const string&         syllableStanzaNumber,
  const Rational&       syllableWholeNotes,
  msrTupletFactor       syllableTupletFactor,
  S_msrStanza           syllableUpLinkToStanza)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure)
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
      endl;

    ++gIndenter;

    print (gLogStream);

    --gIndenter;
  }
#endif
}

msrSyllable::~msrSyllable ()
{}

S_msrSyllable msrSyllable::createSyllableNewbornClone (
  S_msrPart containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Creating a newborn clone of syllable '" <<
      asString () <<
      "'" <<
      endl;
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
        nullptr, // will be set when syllable is appended to a measure JMI v0.9.66 PIM
        fSyllableKind,
        fSyllableExtendKind,
        fSyllableStanzaNumber,
        fSyllableWholeNotes,
        fSyllableTupletFactor,
        fSyllableUpLinkToStanza);

  // append the lyric texts to the syllable clone
  for (
    list<string>::const_iterator i = fSyllableTextsList.begin ();
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
  S_msrPart containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Creating a newborn clone of syllable '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingPart != nullptr,
    "containingPart is null");

  S_msrSyllable
    syllableDeepClone =
      msrSyllable::create (
        fInputLineNumber,
        nullptr, // will be set when syllable is appended to a measure JMI v0.9.66 PIM
        fSyllableKind,
        fSyllableExtendKind,
        fSyllableStanzaNumber,
        fSyllableWholeNotes,
        fSyllableTupletFactor,
        fSyllableUpLinkToStanza);

  // append the lyric texts to the syllable deep clone
  for (
    list<string>::const_iterator i = fSyllableTextsList.begin ();
    i!=fSyllableTextsList.end ();
    ++i
  ) {
    syllableDeepClone->
      appendLyricTextToSyllable ((*i));
  } // for

  // dont't set 'newbornClone->fSyllableUpLinkToStanza'
  // nor 'newbornClone->fSyllableUpLinkToNote',
  // this will be done by the caller

  syllableDeepClone->fSyllableUpLinkToNote =
    fSyllableUpLinkToNote; // TEMP

  return syllableDeepClone;
}

void msrSyllable:: setSyllableNextMeasurePuristNumber (
  int puristMeasureNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Setting syllable next measure purist number to " <<
      fSyllableNextMeasurePuristNumber <<
      endl;
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
        stringstream s;

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

void msrSyllable::setSyllableMeasurePosition (
  const S_msrMeasure measure,
  const Rational&    measurePosition,
  const string&      context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Setting syllable's position in measure of " << asString () <<
      " to " <<
      measurePosition <<
      " (was " <<
      fMeasureElementMeasurePosition <<
      ") in measure " <<
      measure->asShortString () <<
      " (measureElementMeasureNumber: " <<
      fetchMeasureElementMeasureNumber () <<
      "), context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measurePosition != msrMoment::K_NO_POSITION,
    "measurePosition == msrMoment::K_NO_POSITION");

  // set syllable's position in measure
  fMeasureElementMeasurePosition = measurePosition;
}

void msrSyllable::appendLyricTextToSyllable (const string& text)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending text \"" <<
      text <<
      "\" to the texts list of syllable '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fSyllableTextsList.push_back (
    text);
}

void msrSyllable::appendSyllableToNoteAndSetItsUpLinkToNote (
  S_msrNote note)
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
      endl;

    ++gIndenter;

    gLogStream <<
      asString () <<
    // JMI    "to '" << note->asString () <<
      ", line " << note->getInputLineNumber () <<
      endl;

    --gIndenter;
  }
#endif
}

void msrSyllable::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSyllable::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrSyllable>*
    p =
      dynamic_cast<visitor<S_msrSyllable>*> (v)) {
        S_msrSyllable elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSyllable::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSyllable::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSyllable::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSyllable>*
    p =
      dynamic_cast<visitor<S_msrSyllable>*> (v)) {
        S_msrSyllable elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSyllable::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSyllable::browseData (basevisitor* v)
{}

string msrSyllable::syllableWholeNotesAsMsrString () const
{
  string result;

  if (fSyllableUpLinkToNote) { // JMI
    switch (fSyllableUpLinkToNote->getNoteKind ()) {
      case msrNoteKind::kNote_NO_:
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
          stringstream s;

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

string msrSyllableKindAsString (
  msrSyllableKind syllableKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrSyllableKind& elt)
{
  os << msrSyllableKindAsString (elt);
  return os;
}

string msrSyllableExtendKindAsString (
  msrSyllableExtendKind syllableExtendKind)
{
  string result;

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

ostream& operator << (ostream& os, const msrSyllableExtendKind& elt)
{
  os << msrSyllableExtendKindAsString (elt);
  return os;
}

string msrSyllable::syllableUpLinkToNoteAsString () const
{
  string result;

  if (fSyllableUpLinkToNote) {
    result = fSyllableUpLinkToNote->asString ();
  }
  else {
    result = "[NONE]";
  }

  return result;
}

string msrSyllable::syllableTextsListAsString () const
{
  stringstream s;

  if (fSyllableTextsList.size ()) {
    list<string>::const_iterator
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
  const list<string>& textsList,
  ostream&            os)
{
  os << '[';

  if (textsList.size ()) {
    list<string>::const_iterator
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

string msrSyllable::asString () const
{
  stringstream s;

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

void msrSyllable::print (ostream& os) const
{
  os <<
    "[Syllable" <<
    ", syllableKind: " <<
    msrSyllableKindAsString (fSyllableKind) <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 32;

  os << left <<
    setw (fieldWidth) <<
    "syllableTextsList" << " : ";

  writeTextsList (
    fSyllableTextsList,
    os);
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "syllableKind" << " : " <<
    msrSyllableKindAsString (fSyllableKind) <<
    endl <<
    setw (fieldWidth) <<
    "syllableExtendKind" << " : " <<
    msrSyllableExtendKindAsString (
      fSyllableExtendKind) <<
    endl <<

    setw (fieldWidth) <<
    "fSyllableStanzaNumber" << " : \"" <<
    fSyllableStanzaNumber <<
    "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fSyllableWholeNotes" << " : " <<
    fSyllableWholeNotes <<
    endl <<

    setw (fieldWidth) <<
    "fSyllableTupletFactor" << " : " <<
    fSyllableTupletFactor.asString () <<
    endl <<

    setw (fieldWidth) <<
    "fSyllableNextMeasurePuristNumber" << " : " <<
    fSyllableNextMeasurePuristNumber <<
    endl <<

    setw (fieldWidth) <<
    "syllableUpLinkToNote" << " : " <<
    syllableUpLinkToNoteAsString () <<
    endl <<
    setw (fieldWidth) <<
    "syllableUpLinkToStanza" << " : " <<
    fSyllableUpLinkToStanza->getStanzaName () <<
    endl;

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
  os << endl;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrSyllable& elt)
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
// constants
const string msrStanza::K_NO_STANZA_NUMBER = "-1";
const string msrStanza::K_NO_STANZA_NAME   = "Unknown stanza";

S_msrStanza msrStanza::create (
  int           inputLineNumber,
  const string& stanzaNumber,
  S_msrVoice    stanzaUpLinkToVoice)
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
  const string& stanzaNumber,
  S_msrVoice    stanzaUpLinkToVoice)
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
      endl;
  }
#endif

  fStanzaTextPresent = false;

  fStanzaCurrentMeasureWholeNotesDuration = Rational (0, 1);
}

msrStanza::~msrStanza ()
{}

S_msrStanza msrStanza::createStanzaNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Creating a newborn clone of stanza \"" <<
      getStanzaName () <<
      "\" in voice \"" <<
      containingVoice->getVoiceName () <<
      "\"" <<
      endl;
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
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Creating a deep clone of stanza \"" <<
      getStanzaName () <<
      "\" in voice \"" <<
      containingVoice->getVoiceName () <<
      "\"" <<
      endl;
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
  S_msrSyllable syllable)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending syllable '" << syllable->asString () <<
      "' to stanza " << getStanzaName () <<
      endl;
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

  // update the stanza's current measure whole notes
  fStanzaCurrentMeasureWholeNotesDuration +=syllableSoundingWholeNotes;
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
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // create stanza rest syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        nullptr, // will be set when syllable is appended to a measure JMI v0.9.66 PIM
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
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // create stanza skip syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        nullptr, // will be set when syllable is appended to a measure JMI v0.9.66 PIM
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
      endl;
  }
#endif

  ++gIndenter;

  // create stanza skip syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        nullptr, // will be set when syllable is appended to a measure JMI v0.9.66 PIM
        msrSyllableKind::kSyllableMeasureEnd,
        msrSyllableExtendKind::kSyllableExtendNone,
        fStanzaNumber,
        0, // wholeNotes
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // reset current measure whole notes
  fStanzaCurrentMeasureWholeNotesDuration = Rational (0, 1);

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
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // create stanza melisma syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        nullptr, // will be set when syllable is appended to a measure JMI v0.9.66 PIM
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
      endl;
  }
#endif

  ++gIndenter;

  // create line break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        nullptr, // will be set when syllable is appended to a measure JMI v0.9.66 PIM
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
      endl;
  }
#endif

  ++gIndenter;

  // create page break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        nullptr, // will be set when syllable is appended to a measure JMI v0.9.66 PIM
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

void msrStanza::padUpToCurrentMeasureWholeNotesDurationInStanza (
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
      endl;
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
      endl;
  }

  if (visitor<S_msrStanza>*
    p =
      dynamic_cast<visitor<S_msrStanza>*> (v)) {
        S_msrStanza elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStanza::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStanza::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStanza::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrStanza>*
    p =
      dynamic_cast<visitor<S_msrStanza>*> (v)) {
        S_msrStanza elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStanza::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStanza::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStanza::browseData ()" <<
      endl;
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
      endl;
  }
}

void msrStanza::print (ostream& os) const
{
  os <<
    "[Stanza " << getStanzaName () <<
    " (number \"" <<
    fStanzaNumber <<
    "\", " <<
    fSyllables.size () << " syllables)" <<
    endl;

  ++gIndenter;

  if (! fStanzaTextPresent) {
    os <<
      "(No actual text)" <<
      endl;
  }

  else {
    if (fSyllables.size ()) {
      vector<S_msrSyllable>::const_iterator
        iBegin = fSyllables.begin (),
        iEnd   = fSyllables.end (),
        i      = iBegin;

      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here
      } // for
    }
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrStanza& elt)
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
