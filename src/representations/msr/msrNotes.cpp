/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <regex>

#include "visitor.h"

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "oahWae.h"
#include "msrWae.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrArticulations.h"
#include "msrDalSegnos.h"
#include "msrGlissandos.h"
#include "msrPitchesNames.h"
#include "msrSlides.h"
#include "msrTechnicals.h"

#include "msrNotes.h"

#include "oahOah.h"

#include "msrOah.h"

#include "oahEarlyOptions.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

// notes
//______________________________________________________________________________
string msrNoteKindAsString (
  msrNoteKind noteKind)
{
  string result;

  switch (noteKind) {
    case msrNoteKind::kNote_NO_:
      result = "***kNote_NO_***";
      break;

    // in measures
    case msrNoteKind::kNoteRegularInMeasure:
      result = "kNoteRegularInMeasure";
      break;

    case msrNoteKind::kNoteRestInMeasure:
      result = "kNoteRestInMeasure";
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      result = "kNoteSkipInMeasure";
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      result = "kNoteUnpitchedInMeasure";
      break;

    // in chords
    case msrNoteKind::kNoteRegularInChord:
      result = "kNoteRegularInChord";
      break;

    // in tuplets
    case msrNoteKind::kNoteRegularInTuplet:
      result = "kNoteRegularInTuplet";
      break;

    case msrNoteKind::kNoteRestInTuplet:
      result = "kNoteRestInTuplet";
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      result = "kNoteUnpitchedInTuplet";
      break;

    // in grace notes groups
    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      result = "kNoteRegularInGraceNotesGroup";
      break;
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      result = "kNoteSkipInGraceNotesGroup";
      break;

    // in chords in grace notes groups
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      result = "kNoteInChordInGraceNotesGroup";
      break;

    // in tuplets in grace notes groups
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      result = "kNoteInTupletInGraceNotesGroup";
      break;

    // in double-tremolos
    case msrNoteKind::kNoteInDoubleTremolo:
      result = "kNoteInDoubleTremolo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrNoteKind& elt)
{
  os << msrNoteKindAsString (elt);
  return os;
}

// solo notes or rests
//______________________________________________________________________________

string msrSoloNoteOrRestInVoiceKindAsString (
  msrSoloNoteOrRestInVoiceKind soloNoteOrRestInVoiceKind)
{
  string result;

  switch (soloNoteOrRestInVoiceKind) {
    case msrSoloNoteOrRestInVoiceKind::kSoloNoteOrRestInVoiceYes:
      result = "kSoloNoteOrRestInVoiceYes";
      break;
    case msrSoloNoteOrRestInVoiceKind::kSoloNoteOrRestInVoiceNo:
      result = "kSoloNoteOrRestInVoiceNo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrSoloNoteOrRestInVoiceKind& elt)
{
  os << msrSoloNoteOrRestInVoiceKindAsString (elt);
  return os;
}

string msrSoloNoteOrRestInStaffKindAsString (
  msrSoloNoteOrRestInStaffKind soloNoteOrRestInStaffKind)
{
  string result;

  switch (soloNoteOrRestInStaffKind) {
    case msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffYes:
      result = "kSoloNoteOrRestInStaffYes";
      break;
    case msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffPartially:
      result = "kSoloNoteOrRestInStaffPartially";
      break;
    case msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffNo:
      result = "kSoloNoteOrRestInStaffNo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrSoloNoteOrRestInStaffKind& elt)
{
  os << msrSoloNoteOrRestInStaffKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrNote msrNote::create (
  int                        inputLineNumber,
  S_msrMeasure               upLinkToMeasure,

  const string&              noteMeasureNumber,

  msrNoteKind                noteKind,

  msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,
  msrOctaveKind              noteOctaveKind,

  const Rational&            noteSoundingWholeNotes,
  const Rational&            noteDisplayWholeNotes,

  int                        noteDotsNumber,

  msrDurationKind            noteGraphicDurationKind,

  msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
  msrOctaveKind              noteDisplayOctaveKind,

  msrNoteIsACueNoteKind      msrNoteIsACueNoteKind,

  msrPrintObjectKind         notePrintObjectKind,

  msrNoteHeadKind            noteHeadKind,
  msrNoteHeadFilledKind      msrNoteHeadFilledKind,
  msrNoteHeadParenthesesKind msrNoteHeadParenthesesKind)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      upLinkToMeasure,

      noteMeasureNumber,

      noteKind,

      noteQuarterTonesPitchKind,
      noteOctaveKind,

      noteSoundingWholeNotes,
      noteDisplayWholeNotes,

      noteDotsNumber,

      noteGraphicDurationKind,

      noteQuarterTonesDisplayPitchKind,
      noteDisplayOctaveKind,

      msrNoteIsACueNoteKind,

      notePrintObjectKind,

      noteHeadKind,
      msrNoteHeadFilledKind,
      msrNoteHeadParenthesesKind);
  assert (o != nullptr);

  return o;
}

msrNote::msrNote (
  int                        inputLineNumber,
  S_msrMeasure               upLinkToMeasure,

  const string&              noteMeasureNumber,

  msrNoteKind                noteKind,

  msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,
  msrOctaveKind              noteOctaveKind,

  const Rational&            noteSoundingWholeNotes,
  const Rational&            noteDisplayWholeNotes,

  int                        noteDotsNumber,

  msrDurationKind            noteGraphicDurationKind,

  msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
  msrOctaveKind              noteDisplayOctaveKind,

  msrNoteIsACueNoteKind      msrNoteIsACueNoteKind,

  msrPrintObjectKind         notePrintObjectKind,

  msrNoteHeadKind            noteHeadKind,
  msrNoteHeadFilledKind      msrNoteHeadFilledKind,
  msrNoteHeadParenthesesKind msrNoteHeadParenthesesKind)
  : msrTupletElement (
      inputLineNumber,
      upLinkToMeasure),
    fNoteColorAlphaRGB ("", "")
{
  fetchMeasureElementMeasureNumber () = noteMeasureNumber;

  // basic note description
  fNoteKind = noteKind;

  fNoteQuarterTonesPitchKind  = noteQuarterTonesPitchKind;
  fNoteOctaveKind = noteOctaveKind;

  fNoteDotsNumber = noteDotsNumber;

  fNoteGraphicDurationKind = noteGraphicDurationKind;

  fNoteTupletFactor = Rational (1, 1);

  fNoteQuarterTonesDisplayPitchKind = noteQuarterTonesDisplayPitchKind;
  fNoteDisplayOctaveKind            = noteDisplayOctaveKind;

  fNoteIsACueNoteKind   = msrNoteIsACueNoteKind;

  fNotePrintObjectKind = notePrintObjectKind;

  fNoteHeadKind            = noteHeadKind;
  fNoteHeadFilledKind      = msrNoteHeadFilledKind;
  fNoteHeadParenthesesKind = msrNoteHeadParenthesesKind;

  fNoteDisplayWholeNotes  = noteDisplayWholeNotes;

  // only now, to be able to display the note as a string
  doSetMeasureElementSoundingWholeNotes (
    noteSoundingWholeNotes,
    "msrNote::msrNote()");

  // do other initializations
  initializeNote ();
}

void msrNote::initializeNote ()
{
  // rests handling
  // ------------------------------------------------------

  if (fetchNoteIsARest () && fNoteDisplayOctaveKind != msrOctaveKind::kOctave_NO_) {
    // this note is a pitched rest:
    // copy the display octave to the note octave, // JMI
    // to be used in octave relative code generation
    fNoteQuarterTonesPitchKind = fNoteQuarterTonesDisplayPitchKind;
    fNoteOctaveKind = fNoteDisplayOctaveKind;
  }

  // note accidentals
  // ------------------------------------------------------

  fNoteAccidentalKind =
    msrAccidentalKind::kAccidentalNone; // default value

  fNoteEditorialAccidentalKind =
    msrEditorialAccidentalKind::kEditorialAccidentalNo; // default value

  fNoteCautionaryAccidentalKind =
    msrCautionaryAccidentalKind::kCautionaryAccidentalNo; // default value

  // note context
  // ------------------------------------------------------

  fNoteMultipleFullBarRestsSequenceNumber = -1;

//   fNoteColorAlphaRGBHasBenSet = false; JMI v0.9.64

  // note lyrics
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
    gLogStream <<
      endl <<
      "Initializing a note" <<
      ", kind: ";
    if (fNoteKind == msrNoteKind::kNote_NO_)
      gLogStream <<
        "not yet known";
    else
      gLogStream << fNoteKind;
    gLogStream <<
      ", line " << fInputLineNumber << ":" <<
      endl;

    ++gIndenter;

    const int fieldWidth = 30;

    gLogStream <<
      left <<
        setw (fieldWidth) <<
        "fNoteQuarterTonesPitchKind" << " = " <<
        msrQuarterTonesPitchKindAsStringInLanguage (
          fNoteQuarterTonesPitchKind,
          gGlobalMsrOahGroup->
            getMsrQuarterTonesPitchesLanguageKind ()) <<
        endl;

    gLogStream <<
      left <<
        setw (fieldWidth) <<
        "fMeasureElementSoundingWholeNotes" << " = " <<
        fMeasureElementSoundingWholeNotes <<
        endl;

    gLogStream <<
      left <<
        setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << " = " <<
        fNoteDisplayWholeNotes <<
        endl;

    gLogStream <<
      left <<
        setw (fieldWidth) <<
        "fNoteDotsNumber" << " = " <<
        fNoteDotsNumber <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteGraphicDurationKind" << " = " <<
        fNoteGraphicDurationKind;

    gLogStream <<
      left <<
        setw (fieldWidth) <<
        "fNoteTupletFactor" << " = " <<
        fNoteTupletFactor <<
        endl;

    gLogStream <<
      endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteOctaveKind" << " = " <<
        fNoteOctaveKind <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteDisplayOctaveKind" << " = " <<
        fNoteDisplayOctaveKind <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteIsACueNoteKind" << " = " <<
         fNoteIsACueNoteKind <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNotePrintObjectKind" << " = " <<
         fNotePrintObjectKind <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteHeadKind" << " = " <<
         fNoteHeadKind <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteHeadFilledKind" << " = " <<
         fNoteHeadFilledKind <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteHeadParenthesesKind" << " = " <<
         fNoteHeadParenthesesKind <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteAccidentalKind" << " = " <<
        fNoteAccidentalKind <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteEditorialAccidentalKind" << " = " <<
        fNoteEditorialAccidentalKind <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteCautionaryAccidentalKind" << " = " <<
        fNoteCautionaryAccidentalKind <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteBelongsToAChord" << " = " <<
         fNoteBelongsToAChord <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteBelongsToATuplet" << " = " <<
         fNoteBelongsToATuplet <<
        endl << endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteOccupiesAFullMeasure" << " = " <<
         fNoteOccupiesAFullMeasure <<
        endl << endl;

    --gIndenter;
  }
#endif

  // solo note or rest?
  // ------------------------------------------------------

  fNoteSoloNoteOrRestInVoiceKind =
    msrSoloNoteOrRestInVoiceKind::kSoloNoteOrRestInVoiceNo;

  fNoteSoloNoteOrRestInStaffKind =
    msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffNo;
}

msrNote::~msrNote ()
{}

//________________________________________________________________________
S_msrMeasure msrNote::fetchNoteUpLinkToMeasure () const
{
  S_msrMeasure result;

  switch (fNoteKind) {
    case msrNoteKind::kNote_NO_:
      break;

    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteUnpitchedInMeasure:
      result = fMeasureElementUpLinkToMeasure;
      break;

    case msrNoteKind::kNoteRegularInChord:
      if (fNoteDirectUpLinkToChord) {
        result =
          fNoteDirectUpLinkToChord->
            getMeasureElementUpLinkToMeasure ();
      }
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      if (fNoteDirectUpLinkToTuplet) {
        result =
          fNoteDirectUpLinkToTuplet->
            getMeasureElementUpLinkToMeasure ();
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      // JMI
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    /* JMi
      if (fNoteDirectUpLinkToGraceNotesGroup) {
        result =
          fNoteDirectUpLinkToGraceNotesGroup->
            getGraceNotesGroupUpLinkToVoice ();
              / * JMI
              getGraceNotesGroupUpLinkToNote ()->
              fetchNoteUpLinkToVoice ();
              * /
      }
    */
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      break;
  } // switch

  return result;
}

// grace notes group upLink
S_msrGraceNotesGroup msrNote::fetchNoteUpLinkToGraceNotesGroup () const
{
  S_msrGraceNotesGroup result;

  switch (fNoteKind) {
    case msrNoteKind::kNote_NO_:
      break;

    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteRegularInChord:
      result = fNoteDirectUpLinkToGraceNotesGroup;
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      if (fNoteDirectUpLinkToTuplet) {
      /* JMI
        result =
          fNoteDirectUpLinkToTuplet->
            getMeasureElementUpLinkToMeasure ()->
              fetchMeasureUpLinkToVoice ();
      */
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    /* JMi
      if (fNoteDirectUpLinkToGraceNotesGroup) {
        result =
          fNoteDirectUpLinkToGraceNotesGroup->
            getGraceNotesGroupUpLinkToVoice ();
              / * JMI
              getGraceNotesGroupUpLinkToNote ()->
              fetchNoteUpLinkToVoice ();
              * /
      }
    */
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      break;
  } // switch

  return result;
}

S_msrVoice msrNote::fetchNoteUpLinkToVoice () const
{
  S_msrVoice result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> fetchNoteUpLinkToVoice() for note: " <<
      asMinimalString () <<
      endl;
  }
#endif

  switch (fNoteKind) {
    case msrNoteKind::kNote_NO_:
      break;

    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteRegularInChord:
      if (fMeasureElementUpLinkToMeasure) {
        result =
          fMeasureElementUpLinkToMeasure->
            fetchMeasureUpLinkToVoice ();
      }
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      if (fNoteDirectUpLinkToTuplet) {
        S_msrMeasure
          tupletDirectUpLinkToMeasure =
            fNoteDirectUpLinkToTuplet->
              getMeasureElementUpLinkToMeasure ();

        if (tupletDirectUpLinkToMeasure) {
          result =
            tupletDirectUpLinkToMeasure->
              fetchMeasureUpLinkToVoice ();
        }
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    /* JMi
      if (fNoteDirectUpLinkToGraceNotesGroup) {
        result =
          fNoteDirectUpLinkToGraceNotesGroup->
            getGraceNotesGroupUpLinkToVoice ();
              / * JMI
              getGraceNotesGroupUpLinkToNote ()->
              fetchNoteUpLinkToVoice ();
              * /
      }
    */
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      break;
  } // switch

  return result;
}

S_msrStaff msrNote::fetchUpLinkToNoteToStaff () const
{
  S_msrStaff result;

  if (fMeasureElementUpLinkToMeasure) {
    result =
      fMeasureElementUpLinkToMeasure->
        fetchMeasureUpLinkToStaff ();
  }

  return result;
}

S_msrPart msrNote::fetchUpLinkToNoteToPart () const
{
  S_msrPart result;

  if (fMeasureElementUpLinkToMeasure) {
    result =
      fMeasureElementUpLinkToMeasure->
        fetchMeasureUpLinkToPart ();
  }

  return result;
}

S_msrPartGroup msrNote::fetchNoteUpLinkToPartGroup () const
{
  S_msrPartGroup result;

  if (fMeasureElementUpLinkToMeasure) {
    result =
      fMeasureElementUpLinkToMeasure->
        fetchMeasureUpLinkToPartGroup ();
  }

  return result;
}

S_msrScore msrNote::fetchUpLinkToNoteToScore () const
{
  S_msrScore result;

  if (fMeasureElementUpLinkToMeasure) {
    result =
      fMeasureElementUpLinkToMeasure->
        fetchMeasureUpLinkToScore ();
  }

  return result;
}

void msrNote::setNoteKind (msrNoteKind noteKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Setting the kind of note " <<
      asString () <<
      " to '" <<
      msrNoteKindAsString (noteKind) <<
      '\'' <<
      endl;
  }
#endif

  fNoteKind = noteKind;
}

void msrNote::setNoteColorAlphaRGB (
  msrColorAlphaRGB& noteColorAlphaRGB)
{
  fNoteColorAlphaRGB          = noteColorAlphaRGB;
  fNoteColorAlphaRGBHasBenSet = true;
}

S_msrNote msrNote::createNoteNewbornClone (
  S_msrPart containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Creating a newborn clone of note " <<
      asString () <<
      " in part " <<
      containingPart->
        getPartCombinedName () <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingPart != nullptr,
    "containingPart is null");

  S_msrNote
    newbornClone =
      msrNote::create (
        fInputLineNumber,
        nullptr, // will be set when note is appended to a measure JMI v0.9.66 PIM

        fetchMeasureElementMeasureNumber (),

        fNoteKind,

        fNoteQuarterTonesPitchKind,
        fNoteOctaveKind,

        fMeasureElementSoundingWholeNotes,
        fNoteDisplayWholeNotes,

        fNoteDotsNumber,

        fNoteGraphicDurationKind,

        fNoteQuarterTonesDisplayPitchKind,
        fNoteDisplayOctaveKind,

        fNoteIsACueNoteKind,

        fNotePrintObjectKind,

        fNoteHeadKind,
        fNoteHeadFilledKind,
        fNoteHeadParenthesesKind);

  // basic note description
  // ------------------------------------------------------

  newbornClone->fNoteOctaveShift =
    fNoteOctaveShift;

  // accidentals
  // ------------------------------------------------------

  newbornClone->fNoteAccidentalKind =
    fNoteAccidentalKind;

  newbornClone->fNoteEditorialAccidentalKind =
    fNoteEditorialAccidentalKind;

  newbornClone->fNoteCautionaryAccidentalKind =
    fNoteCautionaryAccidentalKind;

  // chord member?
  // ------------------------------------------------------

  newbornClone->
    fNoteBelongsToAChord =
      fNoteBelongsToAChord;

  // tuplet member?
  // ------------------------------------------------------

  newbornClone->
    fNoteBelongsToATuplet =
      fNoteBelongsToATuplet;

  // note measure information
  // ------------------------------------------------------

  newbornClone->
    fNoteOccupiesAFullMeasure =
      fNoteOccupiesAFullMeasure;

  // multiple rest member?
  // ------------------------------------------------------

  newbornClone->
    fNoteBelongsToAMultipleFullBarRests =
      fNoteBelongsToAMultipleFullBarRests;

  newbornClone->
    fNoteMultipleFullBarRestsSequenceNumber =
      fNoteMultipleFullBarRestsSequenceNumber;

  // note lyrics
  // ------------------------------------------------------

  // stem
  // ------------------------------------------------------

  newbornClone->fNoteStem = // JMI
    // share this data
    fNoteStem;

  // beams
  // ------------------------------------------------------

  // articulations
  // ------------------------------------------------------

  // technicals
  // ------------------------------------------------------

  // ornaments
  // ------------------------------------------------------

  // single tremolo
  // ------------------------------------------------------

  // tie
  // ------------------------------------------------------

  newbornClone->fNoteTie = // JMI
    // share this data
    fNoteTie;

  // dynamics
  // ------------------------------------------------------

  // words
  // ------------------------------------------------------

  // slurs
  // ------------------------------------------------------

  // ligatures
  // ------------------------------------------------------

  // harmony
  // ------------------------------------------------------

  // note redundant information (for speed)
  // ------------------------------------------------------

  newbornClone->fNoteIsStemless =
    fNoteIsStemless;

  newbornClone->fNoteIsAChordsFirstMemberNote =
    fNoteIsAChordsFirstMemberNote;

  newbornClone->fNoteIsFirstNoteInADoubleTremolo =
    fNoteIsFirstNoteInADoubleTremolo;
  newbornClone->fNoteIsSecondNoteInADoubleTremolo =
    fNoteIsSecondNoteInADoubleTremolo;

  newbornClone->fNoteTrillOrnament =
    fNoteTrillOrnament;
  newbornClone->fNoteIsFollowedByGraceNotesGroup =
    fNoteIsFollowedByGraceNotesGroup;

  newbornClone->fNoteDashesOrnament =
    fNoteDashesOrnament;

  newbornClone->fNoteWavyLineSpannerStart =
    fNoteWavyLineSpannerStart;
  newbornClone->fNoteWavyLineSpannerStop =
    fNoteWavyLineSpannerStop;

  newbornClone->fNoteDelayedTurnOrnament =
    fNoteDelayedTurnOrnament;
  newbornClone->fNoteDelayedInvertedTurnOrnament =
    fNoteDelayedInvertedTurnOrnament;

  // note RGB color
  // ------------------------------------------------------

  newbornClone->fNoteColorAlphaRGB =
    fNoteColorAlphaRGB;
  newbornClone->fNoteColorAlphaRGBHasBenSet =
    fNoteColorAlphaRGBHasBenSet;

  // solo note or rest?
  // ------------------------------------------------------

  newbornClone->fNoteSoloNoteOrRestInVoiceKind =
    fNoteSoloNoteOrRestInVoiceKind;
  newbornClone->fNoteSoloNoteOrRestInStaffKind =
    fNoteSoloNoteOrRestInStaffKind;

  return newbornClone;
}

S_msrNote msrNote::createNoteDeepClone (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Creating a deep clone of note " <<
      asString () <<
      /* JMI
      " in part " <<
      containingPart->
        getPartCombinedName () <<
        */
      endl;
  }
#endif

/* JMI
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");
    */

  S_msrNote
    noteDeepClone =
      msrNote::create (
        fInputLineNumber,
        nullptr, // will be set when note is appended to a measure JMI v0.9.66 PIM

        fetchMeasureElementMeasureNumber (),

        fNoteKind,

        fNoteQuarterTonesPitchKind,
        fNoteOctaveKind,

        fMeasureElementSoundingWholeNotes,
        fNoteDisplayWholeNotes,

        fNoteDotsNumber,

        fNoteGraphicDurationKind,

        fNoteQuarterTonesDisplayPitchKind,
        fNoteDisplayOctaveKind,

        fNoteIsACueNoteKind,

        fNotePrintObjectKind,

        fNoteHeadKind,
        fNoteHeadFilledKind,
        fNoteHeadParenthesesKind);

  // basic note description
  // ------------------------------------------------------

  noteDeepClone->fNoteOctaveShift = // JMI
    fNoteOctaveShift;

  // accidentals
  // ------------------------------------------------------

  noteDeepClone->fNoteAccidentalKind =
    fNoteAccidentalKind;

  noteDeepClone->fNoteEditorialAccidentalKind =
    fNoteEditorialAccidentalKind;

  noteDeepClone->fNoteCautionaryAccidentalKind =
    fNoteCautionaryAccidentalKind;

  // chord member?
  // ------------------------------------------------------

  noteDeepClone->
    fNoteBelongsToAChord =
      fNoteBelongsToAChord;

  // tuplet member?
  // ------------------------------------------------------

  noteDeepClone->
    fNoteBelongsToATuplet =
      fNoteBelongsToATuplet;

  // note measure information
  // ------------------------------------------------------

  noteDeepClone->
    fNoteOccupiesAFullMeasure =
      fNoteOccupiesAFullMeasure;

  // multiple rest member?
  // ------------------------------------------------------

  noteDeepClone->
    fNoteBelongsToAMultipleFullBarRests =
      fNoteBelongsToAMultipleFullBarRests;

  noteDeepClone->
    fNoteMultipleFullBarRestsSequenceNumber =
      fNoteMultipleFullBarRestsSequenceNumber;

  // note lyrics
  // ------------------------------------------------------

/* JMI
  {
    list<S_msrSyllable>::const_iterator i;
    for (i=fNoteSyllables.begin (); i!=fNoteSyllables.end (); ++i) {
      // share this data
      noteDeepClone->
        fNoteSyllables.push_back ((*i));
    } // for
  }
*/

  // stem
  // ------------------------------------------------------

  noteDeepClone->fNoteStem = // JMI
    // share this data
    fNoteStem;

  // beams
  // ------------------------------------------------------

  {
    list<S_msrBeam>::const_iterator i;
    for (i=fNoteBeams.begin (); i!=fNoteBeams.end (); ++i) {
      // share this data
      noteDeepClone->
        fNoteBeams.push_back ((*i));
    } // for
  }

  // articulations
  // ------------------------------------------------------

  for (
    list<S_msrArticulation>::const_iterator i=
      fNoteArticulations.begin ();
      i!=fNoteArticulations.end ();
      ++i
  ) {
    // share this data
    noteDeepClone->
      fNoteArticulations.push_back ((*i));
  } // for

  // spanners
  // ------------------------------------------------------

  for (
    list<S_msrSpanner>::const_iterator i=
      fNoteSpanners.begin ();
      i!=fNoteSpanners.end ();
      ++i
  ) {
    // share this data
    noteDeepClone->
      fNoteSpanners.push_back ((*i));
  } // for

  // technicals
  // ------------------------------------------------------

  {
    list<S_msrTechnical>::const_iterator i;
    for (
      i=fNoteTechnicals.begin ();
      i!=fNoteTechnicals.end ();
      ++i
  ) {
      // share this data
      noteDeepClone->
        fNoteTechnicals.push_back ((*i));
    } // for
  }

  {
    list<S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i=fNoteTechnicalWithIntegers.begin ();
      i!=fNoteTechnicalWithIntegers.end ();
      ++i
  ) {
      // share this data
      noteDeepClone->
        fNoteTechnicalWithIntegers.push_back ((*i));
    } // for
  }

  {
    list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=fNoteTechnicalWithStrings.begin ();
      i!=fNoteTechnicalWithStrings.end ();
      ++i
  ) {
      // share this data
      noteDeepClone->
        fNoteTechnicalWithStrings.push_back ((*i));
    } // for
  }

  // ornaments
  // ------------------------------------------------------

  {
    list<S_msrOrnament>::const_iterator i;
    for (i=fNoteOrnaments.begin (); i!=fNoteOrnaments.end (); ++i) {
      // share this data
      noteDeepClone->
        fNoteOrnaments.push_back ((*i));
    } // for
  }

  // glissandos
  // ------------------------------------------------------

  {
    list<S_msrGlissando>::const_iterator i;
    for (i=fNoteGlissandos.begin (); i!=fNoteGlissandos.end (); ++i) {
      // share this data
      noteDeepClone->
        fNoteGlissandos.push_back ((*i));
    } // for
  }

  // slides
  // ------------------------------------------------------

  {
    list<S_msrSlide>::const_iterator i;
    for (i=fNoteSlides.begin (); i!=fNoteSlides.end (); ++i) {
      // share this data
      noteDeepClone->
        fNoteSlides.push_back ((*i));
    } // for
  }

  // grace notes
  // ------------------------------------------------------

  noteDeepClone->fNoteGraceNotesGroupBefore =
    fNoteGraceNotesGroupBefore;
  noteDeepClone->fNoteGraceNotesGroupAfter =
    fNoteGraceNotesGroupAfter;

/* JMI
  // after grace notes
  // ------------------------------------------------------

  noteDeepClone->fNoteAfterGraceNotesGroup =
    fNoteAfterGraceNotesGroup;
*/

  // single tremolo
  // ------------------------------------------------------

  noteDeepClone->fNoteSingleTremolo =
    fNoteSingleTremolo;

  // tie
  // ------------------------------------------------------

  noteDeepClone->fNoteTie = // JMI
    // share this data
    fNoteTie;

  // dynamics
  // ------------------------------------------------------

  {
    list<S_msrDynamic>::const_iterator i;
    for (i=fNoteDynamics.begin (); i!=fNoteDynamics.end (); ++i) {
      // share this data
      noteDeepClone->
        fNoteDynamics.push_back ((*i));
    } // for
  }

  {
    list<S_msrOtherDynamic>::const_iterator i;
    for (i=fNoteOtherDynamics.begin (); i!=fNoteOtherDynamics.end (); ++i) {
      // share this data
      noteDeepClone->
        fNoteOtherDynamics.push_back ((*i));
    } // for
  }

  // slashes
  // ------------------------------------------------------

  {
    list<S_msrSlash>::const_iterator i;
    for (i=fNoteSlashes.begin (); i!=fNoteSlashes.end (); ++i) {
      // share this data
      noteDeepClone->
        fNoteSlashes.push_back ((*i));
    } // for
  }

  // cresc/decresc
  // ------------------------------------------------------

  {
    for (S_msrCrescDecresc crescDecresc : fNoteCrescDecrescs) {
      // share this data
      noteDeepClone->
        fNoteCrescDecrescs.push_back (crescDecresc);
    } // for
  }

  // wedges
  // ------------------------------------------------------

  {
    list<S_msrWedge>::const_iterator i;
    for (i=fNoteWedges.begin (); i!=fNoteWedges.end (); ++i) {
      // share this data
      noteDeepClone->
        fNoteWedges.push_back ((*i));
    } // for
  }

  // eyeglasses
  // ------------------------------------------------------

  {
    list<S_msrEyeGlasses>::const_iterator i;
    for (i=fNoteEyeGlasses.begin (); i!=fNoteEyeGlasses.end (); ++i) {
      // share this data
      noteDeepClone->
        fNoteEyeGlasses.push_back ((*i));
    } // for
  }

  // damps
  // ------------------------------------------------------

  {
    list<S_msrDamp>::const_iterator i;
    for (i=fNoteDamps.begin (); i!=fNoteDamps.end (); ++i) {
      // share this data
      noteDeepClone->
        fNoteDamps.push_back ((*i));
    } // for
  }

  // damp alls
  // ------------------------------------------------------

  {
    list<S_msrDampAll>::const_iterator i;
    for (i=fNoteDampAlls.begin (); i!=fNoteDampAlls.end (); ++i) {
      // share this data
      noteDeepClone->
        fNoteDampAlls.push_back ((*i));
    } // for
  }

  // scordaturas
  // ------------------------------------------------------

  {
    list<S_msrScordatura>::const_iterator i;
    for (i=fNoteScordaturas.begin (); i!=fNoteScordaturas.end (); ++i) {
      // share this data
      noteDeepClone->
        fNoteScordaturas.push_back ((*i));
    } // for
  }

  // words
  // ------------------------------------------------------

  {
    list<S_msrWords>::const_iterator i;
    for (i=fNoteWords.begin (); i!=fNoteWords.end (); ++i) {
      // share this data
      noteDeepClone->
        fNoteWords.push_back ((*i));
    } // for
  }

  // slurs
  // ------------------------------------------------------

  {
    list<S_msrSlur>::const_iterator i;
    for (i=fNoteSlurs.begin (); i!=fNoteSlurs.end (); ++i) {
      // share this data
      noteDeepClone->
        fNoteSlurs.push_back ((*i));
    } // for
  }

  // ligatures
  // ------------------------------------------------------

  {
    list<S_msrLigature>::const_iterator i;
    for (i=fNoteLigatures.begin (); i!=fNoteLigatures.end (); ++i) {
      // share this data
      noteDeepClone->
        fNoteLigatures.push_back ((*i));
    } // for
  }

  // pedals
  // ------------------------------------------------------

  {
    list<S_msrPedal>::const_iterator i;
    for (i=fNotePedals.begin (); i!=fNotePedals.end (); ++i) {
      // share this data
      noteDeepClone->
        fNotePedals.push_back ((*i));
    } // for
  }

  // harmonies
  // ------------------------------------------------------

  noteDeepClone->fNoteHarmoniesList = // JMI ???
    fNoteHarmoniesList;

  // figured bass elements
  // ------------------------------------------------------

  noteDeepClone->fNoteFiguredBassesList = // JMI ???
    fNoteFiguredBassesList;

  // note measure information
  // ------------------------------------------------------

  noteDeepClone->
    fetchMeasureElementMeasureNumber () =
      fetchMeasureElementMeasureNumber ();
  noteDeepClone->
    fMeasureElementMeasurePosition =
      fMeasureElementMeasurePosition;
  noteDeepClone->
    fMeasureElementVoicePosition =
      fMeasureElementVoicePosition;

  noteDeepClone->
    fNoteOccupiesAFullMeasure =
      fNoteOccupiesAFullMeasure;

  // note redundant information (for speed)
  // ------------------------------------------------------

  noteDeepClone->fNoteIsStemless =
    fNoteIsStemless;

  noteDeepClone->fNoteIsAChordsFirstMemberNote =
    fNoteIsAChordsFirstMemberNote;

  noteDeepClone->fNoteIsFirstNoteInADoubleTremolo =
    fNoteIsFirstNoteInADoubleTremolo;
  noteDeepClone->fNoteIsSecondNoteInADoubleTremolo =
    fNoteIsSecondNoteInADoubleTremolo;

  noteDeepClone->fNoteTrillOrnament =
    fNoteTrillOrnament;
  noteDeepClone->fNoteIsFollowedByGraceNotesGroup =
    fNoteIsFollowedByGraceNotesGroup;

  noteDeepClone->fNoteDashesOrnament =
    fNoteDashesOrnament;

  noteDeepClone->fNoteWavyLineSpannerStart =
    fNoteWavyLineSpannerStart;
  noteDeepClone->fNoteWavyLineSpannerStop =
    fNoteWavyLineSpannerStop;

  noteDeepClone->fNoteDelayedTurnOrnament =
    fNoteDelayedTurnOrnament;
  noteDeepClone->fNoteDelayedInvertedTurnOrnament =
    fNoteDelayedInvertedTurnOrnament;

  // note color
  // ------------------------------------------------------

  noteDeepClone->fNoteColorAlphaRGB =
    fNoteColorAlphaRGB;
  noteDeepClone->fNoteColorAlphaRGBHasBenSet =
    fNoteColorAlphaRGBHasBenSet;

  // solo note or rest?
  // ------------------------------------------------------

  noteDeepClone->fNoteSoloNoteOrRestInVoiceKind =
    fNoteSoloNoteOrRestInVoiceKind;
  noteDeepClone->fNoteSoloNoteOrRestInStaffKind =
    fNoteSoloNoteOrRestInStaffKind;

  return noteDeepClone;
}

S_msrNote msrNote::createRestNote (
  int             inputLineNumber,
  const string&   noteMeasureNumber,
  const Rational& soundingWholeNotes,
  const Rational& displayWholeNotes,
  int             dotsNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      nullptr, // will be set when note is appended to a measure JMI v0.9.66 PIM

      noteMeasureNumber,

      msrNoteKind::kNoteRestInMeasure, // noteKind

      msrQuarterTonesPitchKind::kQTP_NO_,
      msrOctaveKind::kOctave_NO_, // noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrDurationKind::kDuration_NO_, // noteGraphicDuration

      msrQuarterTonesPitchKind::kQTP_NO_, // noteDisplayQuarterTonesPitch
      msrOctaveKind::kOctave_NO_, // noteDisplayOctave,

      msrNoteIsACueNoteKind::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // default value

      msrNoteHeadKind::kNoteHeadNormal, // JMI
      msrNoteHeadFilledKind::kNoteHeadFilledYes, // JMI
      msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo); // JMI
  assert (o != nullptr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRestNotes ()) {
    gLogStream <<
      "Creating rest note " <<
      o->asShortString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  return o;
}

S_msrNote msrNote::createSkipNote (
  int             inputLineNumber,
  const string&   noteMeasureNumber,
  const Rational& soundingWholeNotes,
  const Rational& displayWholeNotes,
  int             dotsNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      nullptr, // will be set when note is appended to a measure JMI v0.9.66 PIM

      noteMeasureNumber,

      msrNoteKind::kNoteSkipInMeasure, // noteKind

      msrQuarterTonesPitchKind::kQTP_NO_,
      msrOctaveKind::kOctave_NO_, // noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrDurationKind::kDuration_NO_, // noteGraphicDuration

      msrQuarterTonesPitchKind::kQTP_NO_, // noteDisplayQuarterTonesPitch
      msrOctaveKind::kOctave_NO_, // noteDisplayOctave,

      msrNoteIsACueNoteKind::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // default value

      msrNoteHeadKind::kNoteHeadNormal, // JMI
      msrNoteHeadFilledKind::kNoteHeadFilledYes, // JMI
      msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo); // JMI
  assert (o != nullptr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSkipNotes () || gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Creating skip note " <<
      o->asString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  return o;
}

S_msrNote msrNote::createGraceSkipNote (
  int             inputLineNumber,
  const string&   noteMeasureNumber,
  const Rational& soundingWholeNotes,
  const Rational& displayWholeNotes,
  int             dotsNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      nullptr, // will be set when note is appended to a measure JMI v0.9.66 PIM

      noteMeasureNumber,

      msrNoteKind::kNoteSkipInGraceNotesGroup, // noteKind

      msrQuarterTonesPitchKind::kQTP_NO_,
      msrOctaveKind::kOctave_NO_, // noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrDurationKind::kDuration_NO_, // noteGraphicDuration

      msrQuarterTonesPitchKind::kQTP_NO_, // noteDisplayQuarterTonesPitch
      msrOctaveKind::kOctave_NO_, // noteDisplayOctave,

      msrNoteIsACueNoteKind::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // default value

      msrNoteHeadKind::kNoteHeadNormal, // JMI
      msrNoteHeadFilledKind::kNoteHeadFilledYes, // JMI
      msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo); // JMI
  assert (o != nullptr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSkipNotes () || gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Creating grace skip note " <<
      o->asString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  return o;
}

//________________________________________________________________________
S_msrNote msrNote::createRestNoteWithOctave (
  int             inputLineNumber,
  const string&   noteMeasureNumber,
  msrOctaveKind   noteOctave,
  const Rational& soundingWholeNotes,
  const Rational& displayWholeNotes,
  int             dotsNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      nullptr, // will be set when note is appended to a measure JMI v0.9.66 PIM

      noteMeasureNumber,

      msrNoteKind::kNoteRestInMeasure, // noteKind

      msrQuarterTonesPitchKind::kQTP_Rest,
      noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrDurationKind::kDuration_NO_, // noteGraphicDuration

      msrQuarterTonesPitchKind::kQTP_Rest,  // noteQuarterTonesDisplayPitchKind
      msrOctaveKind::kOctave_NO_,  // noteDisplayOctaveKind

      msrNoteIsACueNoteKind::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // default value

      msrNoteHeadKind::kNoteHeadNormal, // JMI
      msrNoteHeadFilledKind::kNoteHeadFilledYes, // JMI
      msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo); // JMI
  assert (o != nullptr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceRestNotes ()) {
    gLogStream <<
      "Creating rest note " <<
      o->asShortString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  return o;
}

//________________________________________________________________________
S_msrNote msrNote::createSkipNoteWithOctave (
  int             inputLineNumber,
  const string&   noteMeasureNumber,
  msrOctaveKind   noteOctave,
  const Rational& soundingWholeNotes,
  const Rational& displayWholeNotes,
  int             dotsNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      nullptr, // will be set when note is appended to a measure JMI v0.9.66 PIM

      noteMeasureNumber,

      msrNoteKind::kNoteSkipInMeasure, // noteKind

      msrQuarterTonesPitchKind::kQTP_Skip,
      noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrDurationKind::kDuration_NO_, // noteGraphicDuration JMI ???

      msrQuarterTonesPitchKind::kQTP_Skip,  // noteQuarterTonesDisplayPitchKind
      noteOctave,

      msrNoteIsACueNoteKind::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // default value

      msrNoteHeadKind::kNoteHeadNormal, // JMI
      msrNoteHeadFilledKind::kNoteHeadFilledYes, // JMI
      msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo); // JMI
  assert (o != nullptr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSkipNotes ()) {
    gLogStream <<
      "Creating skip note with octave " <<
      o->asShortString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  return o;
}

//________________________________________________________________________
S_msrNote msrNote::createRegularNote (
  int                      inputLineNumber,
  const string&            noteMeasureNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  msrOctaveKind            noteOctaveKind,
  const Rational&          soundingWholeNotes,
  const Rational&          displayWholeNotes,
  int                      dotsNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      nullptr, // will be set when note is appended to a measure JMI v0.9.66 PIM

      noteMeasureNumber,

      msrNoteKind::kNoteRegularInMeasure, // noteKind

      quarterTonesPitchKind,
      noteOctaveKind,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      wholeNotesAsDurationKind (displayWholeNotes), // JMI caution for tuplet members...

      quarterTonesPitchKind,
      noteOctaveKind, // JMI ???

      msrNoteIsACueNoteKind::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // default value

      msrNoteHeadKind::kNoteHeadNormal, // JMI
      msrNoteHeadFilledKind::kNoteHeadFilledYes, // JMI
      msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo); // JMI
  assert (o != nullptr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Creating regular note " <<
      o->asShortString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  return o;
}

//________________________________________________________________________
S_msrNote msrNote::createRestFromString (
  int           inputLineNumber,
  const string& restString,
  const string& restMeasureNumber)
{
  // handling restString à la LilyPond, such as "r4.."

  S_msrNote result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Creating rest from string \"" <<
      restString <<
      "\", restMeasureNumber: '" <<
      restMeasureNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*"
    "r" //
    "[[:space:]]*"
    "([[:digit:]]+)" // restDuration
    "[[:space:]]*"
    "(\\.*)"         // restDots
    "[[:space:]]*"
    );

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "regularExpression = " <<
      regularExpression <<
      endl;
  }
#endif

  regex  e (regularExpression);
  smatch sm;

  regex_match (restString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for rest string \"" << restString <<
      "\" with regex \"" << regularExpression <<
      "\":" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  // handling restString à la LilyPond, such as "r4.."

  if (smSize != 3) {
    stringstream s;

    s <<
      "restString \"" << restString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  string
    restDuration = sm [1],
    restDots     = sm [2];

  size_t dotsNumber = restDots.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "restDuration = \"" <<
      restDuration <<
      "\"" <<
      endl <<

      "restDots = \"" <<
      restDots <<
      "\"" <<
      endl <<
      "dotsNumber = " <<
      dotsNumber <<
      endl;
  }
#endif

  // compute the restDurationKind from restDuration
  msrDurationKind
    restDurationKind =
      msrDurationKindFromString (
        inputLineNumber,
        restDuration);

  // compute the duration whole notes from restDurationKind
  Rational
     durationKindFromMslpString =
       msrDurationKindAsWholeNotes (
         restDurationKind);

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "durationKindFromMslpString = " <<
      durationKindFromMslpString <<
      endl;
  }
#endif

  result =
    msrNote::createRestNote (
      inputLineNumber,
      restMeasureNumber,
      durationKindFromMslpString, // soundingWholeNotes
      durationKindFromMslpString, // displayWholeNotes
      dotsNumber);

  return result;
}

//________________________________________________________________________
S_msrNote msrNote::createSkipFromString (
  int           inputLineNumber,
  const string& skipString,
  const string& skipMeasureNumber)
{
  // handling skipString à la LilyPond, such as "s4.."

  S_msrNote result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Creating skip from string \"" <<
      skipString <<
      "\", skipMeasureNumber: '" <<
      skipMeasureNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*"
    "s" //
    "[[:space:]]*"
    "([[:digit:]]+)" // skipDuration
    "[[:space:]]*"
    "(\\.*)"         // skipDots
    "[[:space:]]*"
    );

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "regularExpression = " <<
      regularExpression <<
      endl;
  }
#endif

  regex  e (regularExpression);
  smatch sm;

  regex_match (skipString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for skip string \"" << skipString <<
      "\" with regex \"" << regularExpression <<
      "\":" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  // handling skipString à la LilyPond, such as "s4.."

  if (smSize != 3) {
    stringstream s;

    s <<
      "skipString \"" << skipString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  string
    skipDuration = sm [1],
    skipDots     = sm [2];

  size_t dotsNumber = skipDots.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "skipDuration = \"" <<
      skipDuration <<
      "\"" <<
      endl <<

      "skipDots = \"" <<
      skipDots <<
      "\"" <<
      endl <<
      "dotsNumber = " <<
      dotsNumber <<
      endl;
  }
#endif

  // compute the skipDurationKind from skipDuration
  msrDurationKind
    skipDurationKind =
      msrDurationKindFromString (
        inputLineNumber,
        skipDuration);

  // compute the duration whole notes from skipDurationKind
  Rational
     durationKindFromMslpString =
       msrDurationKindAsWholeNotes (
         skipDurationKind);

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "durationKindFromMslpString = " <<
      durationKindFromMslpString <<
      endl;
  }
#endif

  result =
//    msrNote::createSkipNote (
    msrNote::createRestNote (
      inputLineNumber,
      skipMeasureNumber,
      durationKindFromMslpString, // soundingWholeNotes
      durationKindFromMslpString, // displayWholeNotes
      dotsNumber);

  return result;
}

//________________________________________________________________________
S_msrNote msrNote::createNoteFromString (
  int           inputLineNumber,
  const string& noteString,
  const string& noteMeasureNumber)
{
  // handling noteString à la LilyPond, such as "bes,4.."

  S_msrNote result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Creating note from string \"" <<
      noteString <<
      "\", noteMeasureNumber: '" <<
      noteMeasureNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  // noteString contains note specification
  // decipher it to extract its components

  string regularExpression (
    "[[:space:]]*"
    "([[:lower:]]+)" // notePitch
    "[[:space:]]*"
    "([,|']*)"       // noteOctaveIndication
    "[[:space:]]*"
    "([[:digit:]]+)" // noteDuration
    "[[:space:]]*"
    "(\\.*)"         // noteDots
    "[[:space:]]*"
    );

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "regularExpression = " <<
      regularExpression <<
      endl;
  }
#endif

  regex  e (regularExpression);
  smatch sm;

  regex_match (noteString, sm, e);

  size_t smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for note string \"" << noteString <<
      "\" with regex \"" << regularExpression <<
      "\":" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  // handling noteString à la LilyPond, such as "bes,4.."

  if (smSize != 5) {
    stringstream s;

    s <<
      "noteString \"" << noteString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  string
    notePitch            = sm [1],
    noteOctaveIndication = sm [2],
    noteDuration         = sm [3],
    noteDots             = sm [4];

  size_t dotsNumber = noteDots.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "notePitch = \"" <<
      notePitch <<
      "\"" <<
      endl <<

      "noteOctaveIndication = \"" <<
      noteOctaveIndication <<
      "\"" <<
      endl <<

      "noteDuration = \"" <<
      noteDuration <<
      "\"" <<
      endl <<

      "noteDots = \"" <<
      noteDots <<
      "\"" <<
      endl <<
      "dotsNumber = " <<
      dotsNumber <<
      endl;
  }
#endif

  // fetch the quarternotes pitches kind
  msrQuarterTonesPitchKind
    quarterTonesPitchKind =
      quarterTonesPitchKindFromString (
        gGlobalMsrOahGroup->
          getMsrQuarterTonesPitchesLanguageKind (),
        notePitch);

  // compute the octave from noteOctaveIndication
  msrOctaveKind
    octaveKind =
      msrOctaveKindFromCommasOrQuotes (
        inputLineNumber,
        noteOctaveIndication);

  // compute the noteDurationKind from noteDuration
  msrDurationKind
    noteDurationKind =
      msrDurationKindFromString (
        inputLineNumber,
        noteDuration);

  // compute the duration whole notes from noteDurationKind
  Rational
     durationKindFromMslpString =
       msrDurationKindAsWholeNotes (
         noteDurationKind);

#ifdef TRACING_IS_ENABLED
  if (gGlobalOahEarlyOptions.getEarlyTracingOah ()) {
    gLogStream <<
      "quarterTonesPitchKind = " <<
      msrQuarterTonesPitchKindAsString (quarterTonesPitchKind) <<
      endl <<
      "octaveKind = " <<
      msrOctaveKindAsString (octaveKind) <<
      endl <<
      "durationKindFromMslpString = " <<
      durationKindFromMslpString <<
      endl;
  }
#endif

  result =
    msrNote::createRegularNote (
      inputLineNumber,
      noteMeasureNumber,
      quarterTonesPitchKind, // msrQuarterTonesPitchKind::kQTP_D_Natural
      octaveKind, // msrOctaveKind::kOctave3
      durationKindFromMslpString, // soundingWholeNotes
      durationKindFromMslpString, // displayWholeNotes
      dotsNumber);

  return result;
}

//________________________________________________________________________
S_msrNote msrNote::createNoteFromSemiTonesPitchAndOctave (
  int                          inputLineNumber,
  S_msrSemiTonesPitchAndOctave semiTonesPitchAndOctave)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    semiTonesPitchAndOctave != nullptr,
    "semiTonesPitchAndOctave is null");

  msrNote * o =
    new msrNote (
      inputLineNumber,
      nullptr, // will be set when note is appended to a measure JMI v0.9.66 PIM

      K_NO_MEASURE_NUMBER, // JMI ???

      msrNoteKind::kNoteRegularInMeasure, // noteKind

      quarterTonesPitchKindFromSemiTonesPitchKind (
        semiTonesPitchAndOctave->
          getSemiTonesPitchKind ()),
      semiTonesPitchAndOctave->
        getOctaveKind (),

      Rational (0, 1), // soundingWholeNotes,
      Rational (0, 1), // displayWholeNotes,

      0, // dotsNumber,

      msrDurationKind::kDuration_NO_, // noteGraphicDuration

      msrQuarterTonesPitchKind::kQTP_NO_, // noteDisplayQuarterTonesPitch
      msrOctaveKind::kOctave_NO_, // noteDisplayOctave,

      msrNoteIsACueNoteKind::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // default value

      msrNoteHeadKind::kNoteHeadNormal, // JMI
      msrNoteHeadFilledKind::kNoteHeadFilledYes, // JMI
      msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo); // JMI
  assert (o != nullptr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesOctaveEntry ()) {
    gLogStream <<
      "Creating note " <<
      o->asShortString () <<
      " from semitones pitch and octave " <<
      semiTonesPitchAndOctave->asString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  return o;
}

// //________________________________________________________________________
// void msrNote::setNoteMeasurePosition (
//   const S_msrMeasure measure,
//   const Rational&    measurePosition,
//   const string&      context)
// {
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
//     gLogStream <<
//       "Setting note's position in measure of " << asString () <<
//       " to " <<
//       measurePosition <<
//       " (was " <<
//       fMeasureElementMeasurePosition <<
//       ") in measure " <<
//       measure->asShortString () <<
//       " (measureElementMeasureNumber: " <<
//       fetchMeasureElementMeasureNumber () <<
//       "), context: \"" <<
//       context <<
//       "\"" <<
//       endl;
//   }
// #endif
//
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     measurePosition != msrMoment::K_NO_POSITION,
//     "measurePosition == msrMoment::K_NO_POSITION");
//
//   // set note's position in measure
//   fMeasureElementMeasurePosition = measurePosition;
//
// //   gLogStream << "++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
// //   print (gLogStream); // JMI v0.9.66
// //   gLogStream << "++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
//
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     fMeasureElementUpLinkToMeasure != nullptr,
//     "fMeasureElementUpLinkToMeasure is null");
//
//   // compute note's position in voice
//   Rational
//      voicePosition =
//       fMeasureElementUpLinkToMeasure->getMeasureVoicePosition ()
//         +
//       measurePosition;
//
//   // set note's position in voice
//   msrMeasureElement::setMeasureElementVoicePosition (
//     voicePosition,
//     context);
//
//   // update current position in voice
//   S_msrVoice
//     voice =
//       fMeasureElementUpLinkToMeasure->
//         fetchMeasureUpLinkToVoice ();
//
//   voice->
//     incrementCurrentVoicePosition (
//       fMeasureElementSoundingWholeNotes);
//
//   // are there harmonies attached to this note?
// // JMI v0.9.66
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
//     gLogStream << "fNoteHarmoniesList.size (): " << fNoteHarmoniesList.size () << endl;
//   }
// #endif
//
//   if (fNoteHarmoniesList.size ()) {
//     list<S_msrHarmony>::const_iterator i;
//     for (S_msrHarmony harmony : fNoteHarmoniesList) {
//       // set the harmony position in measure, taking it's offset into account
//       harmony->
//         setHarmonyMeasurePosition (
//           measure,
//           measurePosition,
//           "msrNote::setNoteMeasurePosition()");
//     } // for
//   }
//
//   // are there figured bass elements attached to this note?
//   if (fNoteFiguredBassesList.size ()) {
//     list<S_msrFiguredBass>::const_iterator i;
//     for (S_msrFiguredBass figuredBass : fNoteFiguredBassesList) {
//       // set the figured bass element position in measure
//       figuredBass->
//         setFiguredBassMeasurePosition (
//           measure,
//           measurePosition,
//           "msrNote::setNoteMeasurePosition()");
//     } // for
//   }
//
//   // are there dal segnos attached to this note?
//   if (fNoteDalSegnos.size ()) {
//     for (S_msrDalSegno dalSegno : fNoteDalSegnos) {
//       // set the dal segno position in measure
//       dalSegno->
//         setDalSegnoMeasurePosition (
//           measure,
//           measurePosition,
//           "msrNote::setNoteMeasurePosition()");
//     } // for
//   }
// }

void msrNote::setNoteOccupiesAFullMeasure ()
{
  fNoteOccupiesAFullMeasure = true;
}

string msrNote::noteSoundingWholeNotesAsMsrString () const
{
  string result;

  if (fMeasureElementSoundingWholeNotes.getNumerator () == 0)
    result = " ***no sounding whole notes***";
  else
    result =
      wholeNotesAsMsrString (
        fInputLineNumber,
        fMeasureElementSoundingWholeNotes);

  return result;
  }

string msrNote::noteDisplayWholeNotesAsMsrString () const
{
  string result;

  if (fNoteDisplayWholeNotes.getNumerator () == 0) {
    result = " ***no display whole notes***";
  }
  else {
    result =
      wholeNotesAsMsrString (
        fInputLineNumber,
        fNoteDisplayWholeNotes);
  }

  return result;
}

string msrNoteHeadKindAsString (
  msrNoteHeadKind noteHeadKind)
{
  string result;

  switch (noteHeadKind) {
    case msrNoteHeadKind::kNoteHeadSlash:
      result = "kNoteHeadSlash";
      break;
    case msrNoteHeadKind::kNoteHeadTriangle:
      result = "kNoteHeadTriangle";
      break;
    case msrNoteHeadKind::kNoteHeadDiamond:
      result = "kNoteHeadDiamond";
      break;
    case msrNoteHeadKind::kNoteHeadSquare:
      result = "kNoteHeadSquare";
      break;
    case msrNoteHeadKind::kNoteHeadCross:
      result = "kNoteHeadCross";
      break;
    case msrNoteHeadKind::kNoteHeadX:
      result = "kNoteHeadX";
      break;
    case msrNoteHeadKind::kNoteHeadCircleX:
      result = "kNoteHeadCircleX";
      break;
    case msrNoteHeadKind::kNoteHeadInvertedTriangle:
      result = "kNoteHeadInvertedTriangle";
      break;
    case msrNoteHeadKind::kNoteHeadArrowDown:
      result = "kNoteHeadArrowDown";
      break;
    case msrNoteHeadKind::kNoteHeadArrowUp:
      result = "kNoteHeadArrowUp";
      break;
    case msrNoteHeadKind::kNoteHeadSlashed:
      result = "kNoteHeadSlashed";
      break;
    case msrNoteHeadKind::kNoteHeadBackSlashed:
      result = "kNoteHeadBackSlashed";
      break;
    case msrNoteHeadKind::kNoteHeadNormal:
      result = "msrNoteHeadKind::kNoteHeadNormal";
      break;
    case msrNoteHeadKind::kNoteHeadCluster:
      result = "kNoteHeadCluster";
      break;
    case msrNoteHeadKind::kNoteHeadCircleDot:
      result = "kNoteHeadCircleDot";
      break;
    case msrNoteHeadKind::kNoteHeadLeftTriangle:
      result = "kNoteHeadLeftTriangle";
      break;
    case msrNoteHeadKind::kNoteHeadRectangle:
      result = "kNoteHeadRectangle";
      break;
    case msrNoteHeadKind::kNoteHeadNone:
      result = "kNoteHeadNone";
      break;
    case msrNoteHeadKind::kNoteHeadDo:
      result = "kNoteHeadDo";
      break;
    case msrNoteHeadKind::kNoteHeadRe:
      result = "kNoteHeadRe";
      break;
    case msrNoteHeadKind::kNoteHeadMi:
      result = "kNoteHeadMi";
      break;
    case msrNoteHeadKind::kNoteHeadFa:
      result = "kNoteHeadFa";
      break;
    case msrNoteHeadKind::kNoteHeadFaUp:
      result = "kNoteHeadFaUp";
      break;
    case msrNoteHeadKind::kNoteHeadSo:
      result = "kNoteHeadSo";
      break;
    case msrNoteHeadKind::kNoteHeadLa:
      result = "kNoteHeadLa";
      break;
    case msrNoteHeadKind::kNoteHeadTi:
      result = "kNoteHeadTi";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrNoteHeadKind& elt)
{
  os << msrNoteHeadKindAsString (elt);
  return os;
}

string msrNoteHeadFilledKindAsString (
  msrNoteHeadFilledKind msrNoteHeadFilledKind)
{
  string result;

  switch (msrNoteHeadFilledKind) {
    case msrNoteHeadFilledKind::kNoteHeadFilledYes:
      result = "kNoteHeadFilledYes";
      break;
    case msrNoteHeadFilledKind::kNoteHeadFilledNo:
      result = "kNoteHeadFilledNo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrNoteHeadFilledKind& elt)
{
  os << msrNoteHeadFilledKindAsString (elt);
  return os;
}

string msrNoteHeadParenthesesKindAsString (
  msrNoteHeadParenthesesKind msrNoteHeadParenthesesKind)
{
  string result;

  switch (msrNoteHeadParenthesesKind) {
    case msrNoteHeadParenthesesKind::kNoteHeadParenthesesYes:
      result = "kNoteHeadParenthesesYes";
      break;
    case msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo:
      result = "kNoteHeadParenthesesNo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrNoteHeadParenthesesKind& elt)
{
  os << msrNoteHeadParenthesesKindAsString (elt);
  return os;
}

string msrNoteIsACueNoteKindAsString (
  msrNoteIsACueNoteKind msrNoteIsACueNoteKind)
{
  string result;

  switch (msrNoteIsACueNoteKind) {
    case msrNoteIsACueNoteKind::kNoteIsACueNoteYes:
      result = "kNoteIsACueNoteYes";
      break;
    case msrNoteIsACueNoteKind::kNoteIsACueNoteNo:
      result = "kNoteIsACueNoteNo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrNoteIsACueNoteKind& elt)
{
  os << msrNoteIsACueNoteKindAsString (elt);
  return os;
}

msrDiatonicPitchKind msrNote::noteDiatonicPitchKind (
  int inputLineNumber) const
{
  return
    diatonicPitchKindFromQuarterTonesPitchKind (
      inputLineNumber,
      fNoteQuarterTonesPitchKind);
}

Bool msrNote::noteIsAPitchedRest () const
{
  return
    fetchNoteIsARest () && fNoteDisplayOctaveKind != msrOctaveKind::kOctave_NO_;
}

void msrNote::setNoteStem (S_msrStem stem)
{
  // register note stem
  fNoteStem = stem;

  // mark note as stemless if relevant
  if (stem->getStemKind () == msrStemKind::kStemNeutral)
    fNoteIsStemless = true;
}

void msrNote::setNoteBelongsToAChord ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Setting note " <<
      asShortString () <<
      " to belong to a chord"
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fNoteBelongsToAChord = true;
}

void msrNote::determineTupletMemberSoundingFromDisplayWholeNotes (
  int actualNotes, int normalNotes)
{
  /* JMI
  Type indicates the graphic note type, Valid values (from shortest to longest) are 1024th, 512th, 256th, 128th, 64th, 32nd, 16th, eighth, quarter, half, whole, breve, long, and maxima. The size attribute indicates full, cue, or large size, with full the default for regular notes and cue the default for cue and grace notes.

  Quarter note in a triplet, sounds 2/3 of a quarter:
      <note>
        <pitch>
          <step>B</step>
          <octave>4</octave>
        </pitch>
        <duration>20</duration>
        <voice>1</voice>
        <type>quarter</type>
        <time-modification>
          <actual-notes>3</actual-notes>
          <normal-notes>2</normal-notes>
        </time-modification>
        <notations>
          <tuplet number="1" type="start" />
        </notations>
      </note>
  */

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Determining tuplet sounding from display whole notes" <<
      " for note '" <<
      asShortString () <<
      ", tuplet factor is '" <<
      actualNotes << '/' << normalNotes <<
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  // the display whole notes are known, compute the sounding ones
  doSetMeasureElementSoundingWholeNotes (
    fNoteDisplayWholeNotes
      *
    normalNotes
      /
    actualNotes,
    "determineTupletMemberSoundingFromDisplayWholeNotes()");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "The result is: '" <<
      asShortString () <<
      endl;
  }
#endif
}

void msrNote::appendBeamToNote (S_msrBeam beam)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeams ()) {
    gLogStream <<
      "Adding beam " <<
      beam->asShortString () <<
      " to note " <<
      asString () <<
      endl;
  }
#endif

  // check the order of the beams begins, continues and ends
if (false) { // JMI, note not yet append to anything....
  S_msrVoice
    voice = fetchNoteUpLinkToVoice ();

  voice->checkBeamNumber (beam, this);
}

  // now append the beam
  fNoteBeams.push_back (beam);
}

void msrNote::appendArticulationToNote (S_msrArticulation art)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceArticulations ()) {
    gLogStream <<
      "Adding articulation " <<
      art->asShortString () <<
      " to note " <<
      asString () <<
      endl;
  }
#endif

  fNoteArticulations.push_back (art);
}

void msrNote::appendSpannerToNote (S_msrSpanner spanner)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSpanners ()) {
    gLogStream <<
      "Appending spanner '" <<
      spanner->getSpannerKind () <<
      "' to note " <<
      asString () <<
      endl;
  }
#endif

  // register note has having a wavy line start
  switch (spanner->getSpannerKind ()) {
    case msrSpannerKind::kSpannerDashes:
      break;

    case msrSpannerKind::kSpannerWavyLine:
      switch (spanner->getSpannerTypeKind ()) {
        case msrSpannerTypeKind::kSpannerTypeStart:
          fNoteWavyLineSpannerStart = spanner;
          break;
        case msrSpannerTypeKind::kSpannerTypeStop:
          fNoteWavyLineSpannerStop = spanner;
          break;
        case msrSpannerTypeKind::kSpannerTypeContinue:
          break;
        case msrSpannerTypeKind::kSpannerType_NO_:
          // JMI ???
          break;
      } // switch
      break;
  } // switch

  // append spanner to note spanners
  fNoteSpanners.push_back (spanner);
}

void msrNote::appendTechnicalToNote (S_msrTechnical technical)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
    gLogStream <<
      "Adding technical " <<
      technical->asString () <<
      " to note " <<
      asString () <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  // append the technical to the note technicals list
  fNoteTechnicals.push_back (technical);
}

void msrNote::appendTechnicalWithIntegerToNote (
  S_msrTechnicalWithInteger technicalWithInteger)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
    gLogStream <<
      "Adding technical with integer " <<
      technicalWithInteger->asString () <<
      " to note " <<
      asString () <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  // append the technical with integer to the note technicals with integers list
  fNoteTechnicalWithIntegers.push_back (
    technicalWithInteger);
}

void msrNote::appendTechnicalWithFloatToNote (
  S_msrTechnicalWithFloat technicalWithFloat)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
    gLogStream <<
      "Adding technical with float " <<
      technicalWithFloat->asString () <<
      " to note " <<
      asString () <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  // append the technical with float to the note technicals with floats list
  fNoteTechnicalWithFloats.push_back (
    technicalWithFloat);
}

void msrNote::appendTechnicalWithStringToNote (
  S_msrTechnicalWithString technicalWithString)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTechnicals ()) {
    gLogStream <<
      "Adding technical with string'" <<
      technicalWithString->asString () <<
      " to note " <<
      asString () <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  // append the technical with string to the note technicals with strings list
  fNoteTechnicalWithStrings.push_back (
    technicalWithString);
}

void msrNote::appendOrnamentToNote (S_msrOrnament ornament)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceOrnaments ()) {
    gLogStream <<
      "Adding ornament '" <<
      ornament->asString () <<
      " to note " <<
      asString () <<
      endl;
  }
#endif

  // append the ornament to the note ornaments list
  fNoteOrnaments.push_back (ornament);

  switch (ornament->getOrnamentKind ()) {
    case msrOrnamentKind::kOrnamentTrill:
      fNoteTrillOrnament = ornament;
      break;

/* JMI
    case msrOrnamentKind::kOrnamentDashes:
      fNoteDashesOrnament = ornament;
      break;
*/

    case msrOrnamentKind::kOrnamentDelayedTurn:
      fNoteDelayedTurnOrnament = ornament;
      break;

    case msrOrnamentKind::kOrnamentDelayedInvertedTurn:
      fNoteDelayedInvertedTurnOrnament = ornament;
      break;

    default:
      {}
  } // switch

  // set ornament's note upLink
  ornament->
    setOrnamentUpLinkToNote (this);
}

void msrNote::appendGlissandoToNote (S_msrGlissando glissando)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGlissandos ()) {
    gLogStream <<
      "Adding glissando " <<
      glissando->asShortString () <<
      " to note " <<
      asString () <<
      endl;
  }
#endif

  // append the glissando to the note glissandos list
  fNoteGlissandos.push_back (glissando);
}

void msrNote::appendSlideToNote (S_msrSlide slide)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSlides ()) {
    gLogStream <<
      "Adding slide " <<
      slide->asShortString () <<
      " to note " <<
      asString () <<
      endl;
  }
#endif

  // append the slide to the note glissandos list
  fNoteSlides.push_back (slide);
}

void msrNote::setNoteGraceNotesGroupBefore (
  S_msrGraceNotesGroup graceNotesGroupBefore)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Attaching grace notes group '" <<
      graceNotesGroupBefore->asString () <<
      "' before note '" <<
      asString () <<
      "', line " << graceNotesGroupBefore->getInputLineNumber () <<
      endl;
  }
#endif

  // register the before grace notes group in the note
  fNoteGraceNotesGroupBefore = graceNotesGroupBefore;

  // setup the grace notes group's note upLink
  graceNotesGroupBefore->
    setGraceNotesGroupUpLinkToNote (
      this);
}

void msrNote::setNoteGraceNotesGroupAfter (
  S_msrGraceNotesGroup graceNotesGroupAfter)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Attaching grace notes group '" <<
      graceNotesGroupAfter->asString () <<
      "' after note '" <<
      asString () <<
      "', line " << graceNotesGroupAfter->getInputLineNumber () <<
      endl;
  }
#endif

  // register the after grace notes group in the note
  fNoteGraceNotesGroupAfter = graceNotesGroupAfter;

  // setup the grace notes group's note upLink
  graceNotesGroupAfter->
    setGraceNotesGroupUpLinkToNote (
      this);
}

/* JMI
void msrNote::setNoteAfterGraceNotesGroup (S_msrGraceNotesGroup afterGraceNotesGroup)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Attaching afterGraceNotesGroup '" << afterGraceNotesGroup->asString () <<
      "' to note '" << asShortString () <<
      "', line " << afterGraceNotesGroup->getInputLineNumber () <<
      endl;
  }
#endif

  // register the grace notes in the note
  fNoteAfterGraceNotesGroup = afterGraceNotesGroup;
}
*/

void msrNote::setNoteSingleTremolo (S_msrSingleTremolo trem)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTremolos ()) {
    gLogStream <<
      "Adding singleTremolo " <<
      trem->asString () <<
      " to note " <<
      asString () <<
      "', line " << trem->getInputLineNumber () <<
      endl;
  }
#endif

  // register the singleTremolo in the note
  fNoteSingleTremolo = trem;
}

void msrNote::appendDynamicToNote (S_msrDynamic dynamic)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDynamics ()) {
    gLogStream <<
      "Attaching dynamic " <<
      dynamic->asString () <<
      " to note " <<
      asString () <<
      "', line " << dynamic->getInputLineNumber () <<
      endl;
  }
#endif

  fNoteDynamics.push_back (dynamic);
}
void msrNote::appendOtherDynamicToNote (S_msrOtherDynamic otherDynamic)
{
  fNoteOtherDynamics.push_back (otherDynamic);
}

void msrNote::appendWordsToNote (S_msrWords words)
{
  fNoteWords.push_back (words);
}

void msrNote::appendSlurToNote (S_msrSlur slur)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSlurs ()) {
    gLogStream <<
      "Adding slur '" << slur <<
      "' to note '" << asString () << "'" <<
      endl;
  }
#endif

  fNoteSlurs.push_back (slur);
}

void msrNote::appendLigatureToNote (S_msrLigature ligature)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLigatures ()) {
    gLogStream <<
      "Appending ligature " << ligature << " to note " << asString () <<
       endl;
  }
#endif

  if (fNoteLigatures.size ()) {
    if (
      fNoteLigatures.back ()->getLigatureKind () == msrLigatureKind::kLigatureStart
        &&
      ligature->getLigatureKind () == msrLigatureKind::kLigatureStop
        &&
      fNoteLigatures.back ()->getLigatureNumber () == ligature->getLigatureNumber ()
      ) {
      // it may happen that a given note has a 'ligature start'
      // and a 'ligature stop' in sequence, ignore both

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceLigatures ()) {
        stringstream s;

        s <<
          "a 'ligature start' is immediately followed by a 'ligature stop'" <<
          endl <<
          "with the same number, ignoring both of them at line " <<
          ligature->getInputLineNumber ();

        msrWarning (
          gGlobalServiceRunData->getInputSourceName (),
          ligature->getInputLineNumber (),
          s.str ());
      }
#endif

      // remove 'ligature start'
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceLigatures ()) {
        gLogStream <<
          "Removing last ligature (start) for note '" <<
          asShortString () <<
              endl;
      }
#endif

      fNoteLigatures.pop_back ();

      // don't register 'ligature stop'
    }

    else
      fNoteLigatures.push_back (ligature);
  }

  else
    fNoteLigatures.push_back (ligature);
}

void msrNote::appendPedalToNote (S_msrPedal pedal)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePedals ()) {
    gLogStream <<
      "Appending pedal " << pedal << " to note " << asString () <<
       endl;
  }
#endif

  if (fNotePedals.size ()) {
    if (
      fNotePedals.back ()->getPedalTypeKind () == msrPedalTypeKind::kPedalTypeStart
        &&
      pedal->getPedalTypeKind () == msrPedalTypeKind::kPedalTypeStop
    ) {
      // it may happen that a given note has a 'pedal start'
      // and a 'pedal stop' in sequence, ignore both            // JMI ???

#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTracePedals ()) {
        stringstream s;

        s <<
          "a 'pedal start' is immediately followed by a 'pedal stop'" <<
          endl <<
          "with the same number, ignoring both of them at line " <<
          pedal->getInputLineNumber ();

        msrWarning (
          gGlobalServiceRunData->getInputSourceName (),
          pedal->getInputLineNumber (),
          s.str ());
      }
#endif

      // rmeove 'pedal start'
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTracePedals ()) {
        gLogStream <<
          "Removing last pedal (start) for note '" <<
          asShortString () <<
          endl;
      }
#endif
      fNotePedals.pop_back ();

      // don't register 'pedal stop'
    }

    else
      fNotePedals.push_back (pedal);
  }

  else
    fNotePedals.push_back (pedal);
}

void msrNote::appendSlashToNote (S_msrSlash slash)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSlashes ()) {
    gLogStream <<
      "Appending slash '" <<
      slash->asString () <<
      " to note " <<
      asShortString () <<
      endl;
  }
#endif

  fNoteSlashes.push_back (slash);
}

void msrNote::appendCrescDecrescToNote (
  S_msrCrescDecresc crescDecresc)
{
  fNoteCrescDecrescs.push_back (crescDecresc);
}

void msrNote::appendWedgeToNote (S_msrWedge wedge)
{
  fNoteWedges.push_back (wedge);
}

void msrNote::appendSegnoToNote (S_msrSegno segno)
{
  fNoteSegnos.push_back (segno);
}

void msrNote::appendDalSegnoToNote (S_msrDalSegno dalSegno)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDalSegnos ()) {
    gLogStream <<
      "Appending dal segno " <<
      dalSegno->asShortString () <<
      " to note " <<
      asShortString () <<
      endl;
  }
#endif

  fNoteDalSegnos.push_back (dalSegno);
}

void msrNote::appendCodaToNote (S_msrCoda coda)
{
  fNoteCodas.push_back (coda);
}

void msrNote::appendEyeGlassesToNote (S_msrEyeGlasses eyeGlasses)
{
  fNoteEyeGlasses.push_back (eyeGlasses);
}

void msrNote::appendDampToNote (S_msrDamp damp)
{
  fNoteDamps.push_back (damp);
}

void msrNote::appendDampAllToNote (S_msrDampAll dampAll)
{
  fNoteDampAlls.push_back (dampAll);
}

void msrNote::appendScordaturaToNote (S_msrScordatura scordatura)
{
  fNoteScordaturas.push_back (scordatura);
}

// this 'override' NOT NEEXDED??? JMI v0.9.66
// void msrNote::setMeasureElementVoicePosition (
//   Rational&     voicePosition,
//   const string& context)
// {
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
//     gLogStream <<
//       "Assigning note position in voice of " <<
//       asString () <<
//       " to '" << voicePosition <<
//       "' in measure '" <<
//       fetchMeasureElementMeasureNumber () <<
//       "', context: \"" <<
//       context <<
//       "\"" <<
//       endl;
//   }
// #endif
//
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     voicePosition != msrMoment::K_NO_POSITION,
//     "voicePosition == msrMoment::K_NO_POSITION");
//
//   // set measure element position in voice
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
//     gLogStream <<
//       "Setting note position in voice of " <<
//       asString () <<
//       " to '" << voicePosition <<
//       "' in measure '" <<
//       fetchMeasureElementMeasureNumber () <<
//       "', context: \"" <<
//       context <<
//       "\"" <<
//       endl;
//   }
// #endif
//
//   fMeasureElementVoicePosition = voicePosition;
//
//   // account for it in voicePosition
//   voicePosition +=
//     fMeasureElementSoundingWholeNotes;
//
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
//     gLogStream <<
//       "Position in voice becomes " <<
//       voicePosition <<
//       "', context: \"" <<
//       context <<
//       "\"" <<
//       endl;
//   }
// #endif
// }

bool msrNote::compareNotesByIncreasingMeasurePosition (
  const SMARTP<msrNote>& first,
  const SMARTP<msrNote>& second)
{
  return
    bool (
      first->getMeasureElementMeasurePosition ()
        <
      second->getMeasureElementMeasurePosition ()
    );
}

S_msrDynamic msrNote::removeFirstDynamics () // JMI
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDynamics ()) {
    gLogStream <<
      "Removing first dynamic from note '" <<
      asShortString () <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fNoteDynamics.size () > 0,
    "fNoteDynamics is empty");

  S_msrDynamic dyn = fNoteDynamics.front ();
  fNoteDynamics.pop_front ();

  return dyn;
}

S_msrWedge msrNote::removeFirstWedge () // JMI
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWedges ()) {
    gLogStream <<
      "Removing first wedge from note '" <<
      asShortString () <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fNoteDynamics.size () > 0,
    "fNoteDynamics is empty");

  S_msrWedge wedge = fNoteWedges.front ();
  fNoteWedges.pop_front ();
  return wedge;
}

void msrNote::appendSyllableToNote (S_msrSyllable syllable)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending syllable " <<
      syllable->asString () <<
      " to note " <<
      asString () <<
      endl;
  }
#endif

  fNoteSyllables.push_back (syllable);
}

void msrNote::appendHarmonyToNoteHarmoniesList (S_msrHarmony harmony)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Appending harmony " <<
      harmony->asString () <<
      " to the harmonies list of " <<
      asString () <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  // set the harmony's whole notes duration
  // change the harmony whole notes if it belongs to a tuplet ??? utf8.xml JMI v0.9.66
  harmony->
    setMeasureElementSoundingWholeNotes (
      fMeasureElementSoundingWholeNotes,
      "appendHarmonyToNoteHarmoniesList()");

  fNoteHarmoniesList.push_back (harmony);
}

void msrNote::appendFiguredBassToNoteFiguredBassesList (
  S_msrFiguredBass figuredBass)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Append figured bass " <<
      figuredBass->asString () <<
      " to the figured bass elements list of " <<
      asString () <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  // update the figured bass whole notes if it belongs to a tuplet ??? utf8.xml

  fNoteFiguredBassesList.push_back (figuredBass);

  // register this note as the figuredBass upLink
  figuredBass->
    setFiguredBassUpLinkToNote (this);
}

void msrNote::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrNote::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrNote::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrNote::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrNote::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrNote::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrNote::browseData (basevisitor* v)
{
  // browse the grace notes group before if any
  if (fNoteGraceNotesGroupBefore) {
    // fetch the score
    S_msrScore
      score =
        fetchUpLinkToNoteToScore ();

    if (score) {
      Bool
        inhibitGraceNotesGroupsBeforeBrowsing =
          score->
            getInhibitGraceNotesGroupsBeforeBrowsing ();

      if (inhibitGraceNotesGroupsBeforeBrowsing) {
#ifdef TRACING_IS_ENABLED
        if (
          gGlobalMsrOahGroup->getTraceMsrVisitors ()
            ||
          gGlobalTracingOahGroup->getTraceNotes ()
            ||
          gGlobalTracingOahGroup->getTraceGraceNotes ()
        ) {
          gLogStream <<
            "% ==> visiting grace notes groups before is inhibited" <<
            endl;
        }
#endif
      }
      else {
        // browse the grace notes group before
        msrBrowser<msrGraceNotesGroup> browser (v);
        browser.browse (*fNoteGraceNotesGroupBefore);
      }
    }
  }

  if (fNoteOctaveShift) {
    // browse the octave shift
    msrBrowser<msrOctaveShift> browser (v);
    browser.browse (*fNoteOctaveShift);
  }

  if (fNoteStem) {
    // browse the stem
    msrBrowser<msrStem> browser (v);
    browser.browse (*fNoteStem);
  }

  // browse the beams if any
  if (fNoteBeams.size ()) {
    ++gIndenter;
    list<S_msrBeam>::const_iterator i;
    for (i=fNoteBeams.begin (); i!=fNoteBeams.end (); ++i) {
      // browse the beam
      msrBrowser<msrBeam> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the articulations if any
  if (fNoteArticulations.size ()) {
    ++gIndenter;
    list<S_msrArticulation>::const_iterator i;
    for (i=fNoteArticulations.begin (); i!=fNoteArticulations.end (); ++i) {
      // browse the articulation
      msrBrowser<msrArticulation> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the spanners if any
  if (fNoteSpanners.size ()) {
    ++gIndenter;
    list<S_msrSpanner>::const_iterator i;
    for (i=fNoteSpanners.begin (); i!=fNoteSpanners.end (); ++i) {
      // browse the spanner
      msrBrowser<msrSpanner> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the technicals if any
  if (fNoteTechnicals.size ()) {
    ++gIndenter;
    list<S_msrTechnical>::const_iterator i;
    for (
      i=fNoteTechnicals.begin ();
      i!=fNoteTechnicals.end ();
      ++i
  ) {
      // browse the technical
      msrBrowser<msrTechnical> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the technicals with integer if any
  if (fNoteTechnicalWithIntegers.size ()) {
    ++gIndenter;
    list<S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i=fNoteTechnicalWithIntegers.begin ();
      i!=fNoteTechnicalWithIntegers.end ();
      ++i
  ) {
      // browse the technical
      msrBrowser<msrTechnicalWithInteger> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the technicals with float if any
  if (fNoteTechnicalWithFloats.size ()) {
    ++gIndenter;
    list<S_msrTechnicalWithFloat>::const_iterator i;
    for (
      i=fNoteTechnicalWithFloats.begin ();
      i!=fNoteTechnicalWithFloats.end ();
      ++i
  ) {
      // browse the technical
      msrBrowser<msrTechnicalWithFloat> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the technicals with string if any
  if (fNoteTechnicalWithStrings.size ()) {
    ++gIndenter;
    list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=fNoteTechnicalWithStrings.begin ();
      i!=fNoteTechnicalWithStrings.end ();
      ++i
  ) {
      // browse the technical
      msrBrowser<msrTechnicalWithString> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the ornaments if any
  if (fNoteOrnaments.size ()) {
    ++gIndenter;
    list<S_msrOrnament>::const_iterator i;
    for (i=fNoteOrnaments.begin (); i!=fNoteOrnaments.end (); ++i) {
      // browse the ornament
      msrBrowser<msrOrnament> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the glissandos if any
  if (fNoteGlissandos.size ()) {
    ++gIndenter;
    list<S_msrGlissando>::const_iterator i;
    for (i=fNoteGlissandos.begin (); i!=fNoteGlissandos.end (); ++i) {
      // browse the glissando
      msrBrowser<msrGlissando> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the slides if any
  if (fNoteSlides.size ()) {
    ++gIndenter;
    list<S_msrSlide>::const_iterator i;
    for (i=fNoteSlides.begin (); i!=fNoteSlides.end (); ++i) {
      // browse the glissando
      msrBrowser<msrSlide> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the single tremolo if any
  if (fNoteSingleTremolo) {
    // browse the singleTremolo
    msrBrowser<msrSingleTremolo> browser (v);
    browser.browse (*fNoteSingleTremolo);
  }

  if (fNoteTie) {
    // browse the tie
    msrBrowser<msrTie> browser (v);
    browser.browse (*fNoteTie);
  }

  // browse the dynamics if any
  if (fNoteDynamics.size ()) {
    ++gIndenter;
    list<S_msrDynamic>::const_iterator i;
    for (i=fNoteDynamics.begin (); i!=fNoteDynamics.end (); ++i) {
      // browse the dynamics
      msrBrowser<msrDynamic> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the other dynamics if any
  if (fNoteOtherDynamics.size ()) {
    ++gIndenter;
    list<S_msrOtherDynamic>::const_iterator i;
    for (i=fNoteOtherDynamics.begin (); i!=fNoteOtherDynamics.end (); ++i) {
      // browse the other dynamics
      msrBrowser<msrOtherDynamic> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the words if any
  if (fNoteWords.size ()) {
    ++gIndenter;
    list<S_msrWords>::const_iterator i;
    for (i=fNoteWords.begin (); i!=fNoteWords.end (); ++i) {
      // browse the words
      msrBrowser<msrWords> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the slurs if any
  if (fNoteSlurs.size ()) {
    ++gIndenter;
    list<S_msrSlur>::const_iterator i;
    for (i=fNoteSlurs.begin (); i!=fNoteSlurs.end (); ++i) {
      // browse the slur
      msrBrowser<msrSlur> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the ligatures if any
  if (fNoteLigatures.size ()) {
    ++gIndenter;
    list<S_msrLigature>::const_iterator i;
    for (i=fNoteLigatures.begin (); i!=fNoteLigatures.end (); ++i) {
      // browse the ligature
      msrBrowser<msrLigature> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the pedals if any
  if (fNotePedals.size ()) {
    ++gIndenter;
    list<S_msrPedal>::const_iterator i;
    for (i=fNotePedals.begin (); i!=fNotePedals.end (); ++i) {
      // browse the pedal
      msrBrowser<msrPedal> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the crescDecresc if any
  if (fNoteCrescDecrescs.size ()) {
    ++gIndenter;
    for (S_msrCrescDecresc crescDecresc : fNoteCrescDecrescs) {
      // browse the crescDecresc
      msrBrowser<msrCrescDecresc> browser (v);
      browser.browse (*crescDecresc);
    } // for
    --gIndenter;
  }

  // browse the wedges if any
  if (fNoteWedges.size ()) {
    ++gIndenter;
    for (S_msrWedge wedge : fNoteWedges) {
      // browse the crescDecresc
      msrBrowser<msrWedge> browser (v);
      browser.browse (*wedge);
    } // for
    --gIndenter;
  }

  // browse the slashes if any
  if (fNoteSlashes.size ()) {
    ++gIndenter;
    for (S_msrSlash slash : fNoteSlashes) {
      // browse the slash
      msrBrowser<msrSlash> browser (v);
      browser.browse (*slash);
    } // for
    --gIndenter;
  }

  // browse the segnos if any
  if (fNoteSegnos.size ()) {
    ++gIndenter;
    list<S_msrSegno>::const_iterator i;
    for (i=fNoteSegnos.begin (); i!=fNoteSegnos.end (); ++i) {
      // browse the segno
      msrBrowser<msrSegno> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the dal segnos if any
  if (fNoteDalSegnos.size ()) {
    ++gIndenter;
    list<S_msrDalSegno>::const_iterator i;
    for (i=fNoteDalSegnos.begin (); i!=fNoteDalSegnos.end (); ++i) {
      // browse the dal segno
      msrBrowser<msrDalSegno> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the codas if any
  if (fNoteCodas.size ()) {
    ++gIndenter;
    list<S_msrCoda>::const_iterator i;
    for (i=fNoteCodas.begin (); i!=fNoteCodas.end (); ++i) {
      // browse the coda
      msrBrowser<msrCoda> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the eyeglasses if any
  if (fNoteEyeGlasses.size ()) {
    ++gIndenter;
    list<S_msrEyeGlasses>::const_iterator i;
    for (i=fNoteEyeGlasses.begin (); i!=fNoteEyeGlasses.end (); ++i) {
      // browse the eyeglasses
      msrBrowser<msrEyeGlasses> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the damps if any
  if (fNoteDamps.size ()) {
    ++gIndenter;
    list<S_msrDamp>::const_iterator i;
    for (i=fNoteDamps.begin (); i!=fNoteDamps.end (); ++i) {
      // browse the damp
      msrBrowser<msrDamp> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the damp alls if any
  if (fNoteDampAlls.size ()) {
    ++gIndenter;
    list<S_msrDampAll>::const_iterator i;
    for (i=fNoteDampAlls.begin (); i!=fNoteDampAlls.end (); ++i) {
      // browse the damp all
      msrBrowser<msrDampAll> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the scordaturas if any
  if (fNoteScordaturas.size ()) {
    ++gIndenter;
    list<S_msrScordatura>::const_iterator i;
    for (i=fNoteScordaturas.begin (); i!=fNoteScordaturas.end (); ++i) {
      // browse the scordatura
      msrBrowser<msrScordatura> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the harmonies if any
  if (fNoteHarmoniesList.size ()) {
    list<S_msrHarmony>::const_iterator i;
    for (
      i=fNoteHarmoniesList.begin (); i!=fNoteHarmoniesList.end (); ++i
    ) {
      // browse the harmony
      msrBrowser<msrHarmony> browser (v);
      browser.browse (*(*i));
    } // for
  }

  // browse the figured bass elements if any
  if (fNoteFiguredBassesList.size ()) {
    list<S_msrFiguredBass>::const_iterator i;
    for (
      i=fNoteFiguredBassesList.begin (); i!=fNoteFiguredBassesList.end (); ++i
    ) {
      // browse the figured bass
      msrBrowser<msrFiguredBass> browser (v);
      browser.browse (*(*i));
    } // for
  }

  // browse the syllables if any
  if (fNoteSyllables.size ()) {
    ++gIndenter;
    list<S_msrSyllable>::const_iterator i;
    for (i=fNoteSyllables.begin (); i!=fNoteSyllables.end (); ++i) {
      // browse the syllable
      msrBrowser<msrSyllable> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the grace notes group after if any
  if (fNoteGraceNotesGroupAfter) {
    // fetch the score
    S_msrScore
      score =
        fMeasureElementUpLinkToMeasure->
          fetchMeasureUpLinkToScore ();

    if (score) {
      Bool
        inhibitGraceNotesGroupsAfterBrowsing =
          score->getInhibitGraceNotesGroupsAfterBrowsing ();

      if (inhibitGraceNotesGroupsAfterBrowsing) {
#ifdef TRACING_IS_ENABLED
        if (
          gGlobalMsrOahGroup->getTraceMsrVisitors ()
            ||
          gGlobalTracingOahGroup->getTraceNotes ()
            ||
          gGlobalTracingOahGroup->getTraceGraceNotes ()
        ) {
          gLogStream <<
            "% ==> visiting grace notes groups after is inhibited" <<
            endl;
        }
#endif
      }
      else {
        // browse the grace notes group after
        msrBrowser<msrGraceNotesGroup> browser (v);
        browser.browse (*fNoteGraceNotesGroupAfter);
      }
    }
  }
}

string msrNote::notePitchAsString () const
{
  stringstream s;

  switch (fNoteKind) {
    case msrNoteKind::kNote_NO_:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteInDoubleTremolo:
    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
    case msrNoteKind::kNoteRegularInChord:
    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      s <<
        msrQuarterTonesPitchKindAsStringInLanguage (
          fNoteQuarterTonesPitchKind,
          gGlobalMsrOahGroup->
            getMsrQuarterTonesPitchesLanguageKind ());
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      s << "unpitched ";
      break;
  } // switch

  return s.str ();
}

string msrNote::noteDisplayPitchKindAsString () const
{
  stringstream s;

  s <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fNoteQuarterTonesDisplayPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ());

  return s.str ();
}

string msrNote::noteGraphicDurationAsMsrString () const
{
  string
    result =
      msrDurationKindAsString (
        fNoteGraphicDurationKind);

  return result;
}

string msrNote::noteGraphicDurationAsMusicXMLString () const
{
  string
    result =
      msrDurationKindAsMusicXMLType (
        fNoteGraphicDurationKind);

  return result;
}

string msrNote::tupletNoteGraphicDurationAsMsrString ( // JMI
  int actualNotes, int normalNotes) const
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fMeasureElementSoundingWholeNotes
        *
      Rational (actualNotes, normalNotes));
}

string msrNote::noteDiatonicPitchKindAsString (
  int inputLineNumber) const
{
  return
    msrDiatonicPitchKindAsStringInLanguage (
      gGlobalMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
      noteDiatonicPitchKind (
        fInputLineNumber));
}

string msrNote::asShortStringWithRawWholeNotes () const
{
  stringstream s;

  s << '[';

  switch (fNoteKind) {
    case msrNoteKind::kNote_NO_:
      s <<
        "***noNote***";
      break;

    case msrNoteKind::kNoteRestInMeasure:
      s <<
        "kNoteRestInMeasure: " <<
        msrOctaveKindAsString (fNoteOctaveKind) <<
        ", fMeasureElementSoundingWholeNotes: " <<
        fMeasureElementSoundingWholeNotes <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      s <<
        "kNoteSkipInMeasure: " <<
        ", fMeasureElementSoundingWholeNotes: " <<
        fMeasureElementSoundingWholeNotes <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      s <<
        "kNoteUnpitchedInMeasure: " <<
        notePitchAsString () <<
        ", fMeasureElementSoundingWholeNotes: " <<
        fMeasureElementSoundingWholeNotes <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      s <<
        "kNoteRegularInMeasure" <<
        notePitchAsString () <<
        msrOctaveKindAsString (fNoteOctaveKind) <<
        ", fMeasureElementSoundingWholeNotes: " <<
        fMeasureElementSoundingWholeNotes <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      s <<
        "kNoteInDoubleTremolo" <<
        ", " <<
        notePitchAsString () <<
        msrOctaveKindAsString (fNoteOctaveKind) <<
        ", fMeasureElementSoundingWholeNotes: " <<
        fMeasureElementSoundingWholeNotes <<
        ", fNoteDisplayWholeNotes, " <<
        fNoteDisplayWholeNotes;
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      s <<
        "kNoteRegularInGraceNotesGroup" <<
        ", " <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        ", " <<
        msrOctaveKindAsString (fNoteOctaveKind);

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      s <<
        "kNoteSkipInGraceNotesGroup" <<
        ":" <<
        ", fMeasureElementSoundingWholeNotes: " <<
        fMeasureElementSoundingWholeNotes <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      s <<
        "kNoteInChordInGraceNotesGroup" <<
        ", " <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        ", " <<
        msrOctaveKindAsString (fNoteOctaveKind);

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << "."; // JMI
      } // for
      break;

    case msrNoteKind::kNoteRegularInChord:
      s <<
        "kNoteRegularInChord" <<
        ", " <<
        notePitchAsString () <<
        msrOctaveKindAsString (fNoteOctaveKind) <<
        ", fMeasureElementSoundingWholeNotes: " <<
        fMeasureElementSoundingWholeNotes <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInTuplet:
      s <<
        "kNoteRegularInTuplet" <<
        ", " <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        ", " <<
        msrOctaveKindAsString (fNoteOctaveKind) <<
        ", fMeasureElementSoundingWholeNotes: " <<
        fMeasureElementSoundingWholeNotes <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;

    case msrNoteKind::kNoteRestInTuplet:
      s <<
        "kNoteRestInTuplet" <<
        ", " <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        ", fMeasureElementSoundingWholeNotes: " <<
        fMeasureElementSoundingWholeNotes <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      s <<
        "kNoteInTupletInGraceNotesGroup" <<
        ", " <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString ();

      if (! fetchNoteIsARest ()) {
        s <<
          msrOctaveKindAsString (fNoteOctaveKind);
      }

      s <<
        ", fMeasureElementSoundingWholeNotes: " <<
        fMeasureElementSoundingWholeNotes <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for

      s <<
        ", fNoteTupletFactor " << fNoteTupletFactor;
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      s <<
        "kNoteUnpitchedInTuplet " <<
        noteGraphicDurationAsMsrString () <<
        ", fMeasureElementSoundingWholeNotes: " <<
        fMeasureElementSoundingWholeNotes <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      s <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;
  } // switch

  s <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

string msrNote::asShortString () const
{
  stringstream s;

  s << '[';

  switch (fNoteKind) {
    case msrNoteKind::kNote_NO_:
      s <<
        "kNote_NO_" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteRestInMeasure:
      s <<
        "kNoteRestInMeasure" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      s <<
        "kNoteSkipInMeasure: " <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      s <<
        "kNoteUnpitchedInMeasure: " <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      {
        s <<
          "kNoteRegularInMeasure" <<
          ", " <<
          notePitchAsString () <<
          noteSoundingWholeNotesAsMsrString ();

        for (int i = 0; i < fNoteDotsNumber; ++i) {
          s << ".";
        } // for

        s <<
          ", " <<
          msrOctaveKindAsString (fNoteOctaveKind);

        S_msrVoice
          voice =
            fetchNoteUpLinkToVoice ();

        s <<
          ", voice: ";
        if (voice) {
          s <<
            voice->getVoiceNumber ();
        }
        else {
          s << "[NONE]";
        }

        S_msrStaff
          staff;

        if (voice) {
          staff =
            voice->
              getVoiceUpLinkToStaff ();
        }

        s <<
          ", staff: ";
        if (staff) {
          s <<
            staff->getStaffNumber ();
        }
        else {
          s << "[NONE]";
        }
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      s <<
        "kNoteInDoubleTremolo " <<
        ", " <<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        ", " <<
        msrOctaveKindAsString (fNoteOctaveKind);

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      s <<
        "kNoteRegularInGraceNotesGroup" <<
        ", " <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        ", " <<
        msrOctaveKindAsString (fNoteOctaveKind);

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      s <<
        "kNoteSkipInGraceNotesGroup: " <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

   case msrNoteKind::kNoteInChordInGraceNotesGroup:
      s <<
        "kNoteInChordInGraceNotesGroup " <<
        ", " <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        ", " <<
        msrOctaveKindAsString (fNoteOctaveKind);

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << "."; // JMI
      } // for
      break;

    case msrNoteKind::kNoteRegularInChord:
      s <<
        "kNoteRegularInChord " <<
        ", " <<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        ", " <<
        msrOctaveKindAsString (fNoteOctaveKind);

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInTuplet:
      s <<
        "kNoteRegularInTuplet" <<
        ", " <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        ", fMeasureElementSoundingWholeNotes: " <<
        fMeasureElementSoundingWholeNotes <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes <<
        /* JMI KAKA
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectUpLinkToTuplet->getTupletActualNotes (),
            fNoteDirectUpLinkToTuplet->getTupletNormalNotes ());
            */
        ", " <<
        msrOctaveKindAsString (fNoteOctaveKind);

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteRestInTuplet:
      s <<
        "kNoteRestInTuplet" <<
        ", " <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        ", fMeasureElementSoundingWholeNotes: " <<
        fMeasureElementSoundingWholeNotes <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;
        /* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectUpLinkToTuplet->getTupletActualNotes (),
            fNoteDirectUpLinkToTuplet->getTupletNormalNotes ());
            */

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      s <<
        "kNoteInTupletInGraceNotesGroup" <<
        ", " <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        ", fMeasureElementSoundingWholeNotes: " <<
        fMeasureElementSoundingWholeNotes <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;
        /* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectUpLinkToTuplet->getTupletActualNotes (),
            fNoteDirectUpLinkToTuplet->getTupletNormalNotes ());
            */

      if (! fetchNoteIsARest ()) {
        s <<
          ", " <<
          msrOctaveKindAsString (fNoteOctaveKind);
      }

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      s <<
        "kNoteUnpitchedInTuplet" <<
        ", fMeasureElementSoundingWholeNotes: " <<
        fMeasureElementSoundingWholeNotes <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;
        /* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectUpLinkToTuplet->getTupletActualNotes (),
            fNoteDirectUpLinkToTuplet->getTupletNormalNotes ());
            */

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;
  } // switch

  s <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

string msrNote::asMinimalString () const
{
  stringstream s;

  s << '[';

  switch (fNoteKind) {
    case msrNoteKind::kNote_NO_:
      s <<
        "kNote_NO_" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteRestInMeasure:
      s <<
        "kNoteRestInMeasure" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      s <<
        "kNoteSkipInMeasure: " <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      s <<
        "kNoteUnpitchedInMeasure: " <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      {
        s <<
          "kNoteRegularInMeasure" <<
          ", " <<
          notePitchAsString () <<
          noteSoundingWholeNotesAsMsrString ();

        for (int i = 0; i < fNoteDotsNumber; ++i) {
          s << ".";
        } // for

        s <<
          ", " <<
          msrOctaveKindAsString (fNoteOctaveKind);
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      s <<
        "kNoteInDoubleTremolo " <<
        ", " <<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        ", " <<
        msrOctaveKindAsString (fNoteOctaveKind);

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      s <<
        "kNoteRegularInGraceNotesGroup" <<
        ", " <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        ", " <<
        msrOctaveKindAsString (fNoteOctaveKind);

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      s <<
        "kNoteSkipInGraceNotesGroup: " <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

   case msrNoteKind::kNoteInChordInGraceNotesGroup:
      s <<
        "kNoteInChordInGraceNotesGroup " <<
        ", " <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        ", " <<
        msrOctaveKindAsString (fNoteOctaveKind);

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << "."; // JMI
      } // for
      break;

    case msrNoteKind::kNoteRegularInChord:
      s <<
        "kNoteRegularInChord " <<
        ", " <<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        ", " <<
        msrOctaveKindAsString (fNoteOctaveKind);

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInTuplet:
      s <<
        "kNoteRegularInTuplet" <<
        ", " <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        ", fMeasureElementSoundingWholeNotes: " <<
        fMeasureElementSoundingWholeNotes <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes <<
        ", " <<
        msrOctaveKindAsString (fNoteOctaveKind);

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteRestInTuplet:
      s <<
        "kNoteRestInTuplet" <<
        ", " <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        ", fMeasureElementSoundingWholeNotes: " <<
        fMeasureElementSoundingWholeNotes <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      s <<
        "kNoteInTupletInGraceNotesGroup" <<
        ", " <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        ", fMeasureElementSoundingWholeNotes: " <<
        fMeasureElementSoundingWholeNotes <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      if (! fetchNoteIsARest ()) {
        s <<
          ", " <<
          msrOctaveKindAsString (fNoteOctaveKind);
      }

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      s <<
        "kNoteUnpitchedInTuplet" <<
        ", fMeasureElementSoundingWholeNotes: " <<
        fMeasureElementSoundingWholeNotes <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;
  } // switch

  s <<
    ", line " << fInputLineNumber <<
    ']';

  return ""; // JMI KAKA

  return s.str ();
}

string msrNote::noteComplementsAsString () const
{
  stringstream s;

  S_msrVoice
    voice =
      fetchNoteUpLinkToVoice ();

  S_msrStaff
    staff;

  if (voice) {
    staff =
      voice->
        getVoiceUpLinkToStaff ();
  }

  s <<
    ", staffNumber: ";
  if (staff) {
    s <<
      staff->getStaffNumber ();
  }
  else {
    s << "[NONE]";
  }

  s <<
    ", voiceNumber: ";
  if (voice) {
    s <<
      voice->getVoiceNumber ();
  }
  else {
    s << "[NONE]";
  }

  s <<
    ", measureElementMeasureNumber: ";
  if (fetchMeasureElementMeasureNumber () == K_NO_MEASURE_NUMBER) {
    s << "*unknown*";
  }
  else {
    s << fetchMeasureElementMeasureNumber ();
  }

  return s.str ();
}

string msrNote::soundingNoteEssentialsAsString () const
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fNoteDotsNumber < 10,
    "fNoteDotsNumber " + to_string (fNoteDotsNumber) + ">= 10");

  stringstream s;

  s <<
    "[sounding " <<
    notePitchAsString () <<
    noteSoundingWholeNotesAsMsrString ();

  for (int i = 0; i < fNoteDotsNumber; ++i) {
    s << ".";
  } // for

  s <<
    ' ' <<
    msrOctaveKindAsString (fNoteOctaveKind) <<
//     ",++++ " << // JMI KAKA
    noteComplementsAsString () <<
    ']';

  return s.str ();
}

string msrNote::soundingNoteEssentialsAsStringForMeasuresSlices () const // JMI USELESS??? see above method
{
  stringstream s;

  s <<
    "[sounding " <<
    notePitchAsString () <<
    noteSoundingWholeNotesAsMsrString ();

  for (int i = 0; i < fNoteDotsNumber; ++i) {
    s << ".";
  } // for

  s <<
    ' ' <<
    msrOctaveKindAsString (fNoteOctaveKind) <<
    ']';

  return s.str ();
}

string msrNote::asShortStringForTimeView () const
{
  stringstream s;

  s <<
    "@:" <<
    fMeasureElementMeasurePosition <<
    ' ' <<
    asShortStringForMeasuresSlices ();

  return s.str ();
}

string msrNote::nonSoundingNoteEssentialsAsString () const
{
  stringstream s;

  s <<
    '[' <<
    notePitchAsString () <<
    ' ' << // JMI
    noteDisplayWholeNotesAsMsrString ();

  for (int i = 0; i < fNoteDotsNumber; ++i) {
    s << ".";
  } // for

  s <<
    ", " <<
    msrOctaveKindAsString (fNoteOctaveKind) <<
    ",---- " << // JMI KAKA
    noteComplementsAsString () <<
    ']';

  return ""; // JMI KAKA

  return s.str ();
}

string msrNote::nonSoundingNoteEssentialsAsStringForMeasuresSlices () const
{
  stringstream s;

  s <<
//     notePitchAsString () << JMI
//     ' ' <<
    noteDisplayWholeNotesAsMsrString ();

  for (int i = 0; i < fNoteDotsNumber; ++i) {
    s << ".";
  } // for

//   s << JMI
//     " (" <<
//     msrOctaveKindAsString (fNoteOctaveKind) <<
//     ")";

  return s.str ();
}

string msrNote::asString () const
{
  stringstream s;

  s <<
    "[Note" <<
    ", fNoteKind: ";

  switch (fNoteKind) {
    case msrNoteKind::kNote_NO_:
      s <<
        "*noNote*";
      break;

    case msrNoteKind::kNoteRestInMeasure:
      s <<
        "kNoteRestInMeasure, ";

      if (fNoteOccupiesAFullMeasure) {
        s <<
          "R" <<
          /* JMI
          multipleFullBarRestsWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes);
            */
          nonSoundingNoteEssentialsAsString ();
      }
      else {
        s <<
          "r" <<
          nonSoundingNoteEssentialsAsString ();
      }

      s <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      s <<
        "kNoteSkipInMeasure: " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      s <<
        "kNoteUnpitchedInMeasure: " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      s <<
        "kNoteRegularInMeasure " <<
        soundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      s <<
        "kNoteInDoubleTremolo " <<
        soundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      s <<
        "kNoteRegularInGraceNotesGroup " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      s <<
        "kNoteSkipInGraceNotesGroup " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      s <<
        "kNoteInChordInGraceNotesGroup " <<
        nonSoundingNoteEssentialsAsString () <<
        ", fNoteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteRegularInChord:
      s <<
        "kNoteRegularInChord " <<
        soundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteRegularInTuplet:
      s <<
        "kNoteRegularInTuplet " <<
        soundingNoteEssentialsAsString ();
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectUpLinkToTuplet->getTupletActualNotes (),
            fNoteDirectUpLinkToTuplet->getTupletNormalNotes ())
            */
      break;

    case msrNoteKind::kNoteRestInTuplet:
      s <<
        "kNoteRestInTuplet " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      s <<
        "kNoteInTupletInGraceNotesGroup " <<
        nonSoundingNoteEssentialsAsString ();
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectUpLinkToTuplet->getTupletActualNotes (),
            fNoteDirectUpLinkToTuplet->getTupletNormalNotes ())
            */

      s <<
        ", fNoteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      s <<
        "kNoteUnpitchedInTuplet " <<
        noteSoundingWholeNotesAsMsrString ();
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectUpLinkToTuplet->getTupletActualNotes (),
            fNoteDirectUpLinkToTuplet->getTupletNormalNotes ())
            */
      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;
  } // switch
  s << ", ";


/* JMI
  s << left <<
    ", measurePosition: ";
    / * JMI
  if (fMeasureElementMeasurePosition == msrMoment::K_NO_POSITION_MEASURE_NUMBER) {
    s << "unknown (" << fMeasureElementMeasurePosition << ")";
  }
  else {
    s << fMeasureElementMeasurePosition;
  }
  * /
  s << fMeasureElementMeasurePosition;

  s <<
    ", voicePosition: " <<
    fMeasureElementVoicePosition;
*/

  if (fNoteOccupiesAFullMeasure) {
    s <<
      ", full measure";
  }

  if (fNoteTrillOrnament) {
    s <<
      ", has a trill ornament";
  }

  if (fNoteDashesOrnament) {
    s <<
      ", has a dashes ornament";
  }

  if (fNoteWavyLineSpannerStart) {
    s <<
      ", has a wavy line spanner start";
  }

  if (fNoteWavyLineSpannerStop) {
    s <<
      ", has a wavy line spanner stop";
  }

  if (fNoteIsFollowedByGraceNotesGroup) {
    s <<
      ", followed by grace notes group";
  }

  if (fNoteTie) {
    s <<
      ", " << msrTieKindAsString (fNoteTie->getTieKind ());
  }

  s <<
    ", " << msrNoteIsACueNoteKindAsString (fNoteIsACueNoteKind);

  s <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

string msrNote::asShortStringForMeasuresSlices () const
{
  stringstream s;

  s << '[';

  switch (fNoteKind) {
    case msrNoteKind::kNote_NO_:
      s <<
        "*noNote*";
      break;

    case msrNoteKind::kNoteRestInMeasure:
//       s <<
//         "kNoteRestInMeasure, "; JMI

      if (fNoteOccupiesAFullMeasure) {
        s <<
          "R" <<
          /* JMI
          multipleFullBarRestsWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes);
            */
          nonSoundingNoteEssentialsAsStringForMeasuresSlices ();
      }
      else {
        s <<
          "r" <<
          nonSoundingNoteEssentialsAsStringForMeasuresSlices ();
      }

//       s <<
//         nonSoundingNoteEssentialsAsStringForMeasuresSlices (); JMI KAKA
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      s <<
        "s" <<
        nonSoundingNoteEssentialsAsStringForMeasuresSlices ();
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      s <<
        "unpitched " <<
        nonSoundingNoteEssentialsAsStringForMeasuresSlices ();
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      s <<
// JMI        "kNoteRegularInMeasure " <<
        soundingNoteEssentialsAsStringForMeasuresSlices ();
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      s <<
        "kNoteInDoubleTremolo " <<
        soundingNoteEssentialsAsStringForMeasuresSlices ();
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      s <<
        "kNoteRegularInGraceNotesGroup " <<
        nonSoundingNoteEssentialsAsStringForMeasuresSlices ();
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      s <<
        "kNoteSkipInGraceNotesGroup " <<
        nonSoundingNoteEssentialsAsStringForMeasuresSlices ();
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      s <<
        "kNoteInChordInGraceNotesGroup " <<
        soundingNoteEssentialsAsStringForMeasuresSlices () <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteRegularInChord:
      s <<
// JMI        "kNoteRegularInChord " <<
        soundingNoteEssentialsAsStringForMeasuresSlices ();
      break;

    case msrNoteKind::kNoteRegularInTuplet:
      s <<
        "kNoteRegularInTuplet " <<
        soundingNoteEssentialsAsStringForMeasuresSlices ();
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectUpLinkToTuplet->getTupletActualNotes (),
            fNoteDirectUpLinkToTuplet->getTupletNormalNotes ())
            */
      break;

    case msrNoteKind::kNoteRestInTuplet:
      s <<
        "kNoteRestInTuplet " <<
        soundingNoteEssentialsAsStringForMeasuresSlices ();
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      s <<
        "kNoteInTupletInGraceNotesGroup " <<
        soundingNoteEssentialsAsStringForMeasuresSlices ();
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectUpLinkToTuplet->getTupletActualNotes (),
            fNoteDirectUpLinkToTuplet->getTupletNormalNotes ())
            */

      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      s <<
        "kNoteUnpitchedInTuplet " <<
        noteSoundingWholeNotesAsMsrString ();
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectUpLinkToTuplet->getTupletActualNotes (),
            fNoteDirectUpLinkToTuplet->getTupletNormalNotes ())
            */
      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;
  } // switch

  s <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrNote::printNoteEssentials (ostream& os) const
{
  os <<
    "fNoteKind: ";

  switch (fNoteKind) {
    case msrNoteKind::kNote_NO_:
      os <<
        "*noNote*";
      break;

    case msrNoteKind::kNoteRestInMeasure:
      os <<
        "kNoteRestInMeasure: ";

      if (fNoteOccupiesAFullMeasure) {
        os <<
          "R" <<
          /* JMI
          multipleFullBarRestsWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes);
            */
          nonSoundingNoteEssentialsAsString ();
      }
      else {
        os <<
          "r" <<
          nonSoundingNoteEssentialsAsString ();
      }
      os << endl;

      os <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      os <<
        "kNoteSkipInMeasure: " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      os <<
        "kNoteUnpitchedInMeasure: " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      os <<
        "kNoteRegularInMeasure: " <<
        soundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      os <<
        "kNoteInDoubleTremolo: " <<
        soundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      os <<
        "kNoteRegularInGraceNotesGroup " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      os <<
        "kNoteSkipInGraceNotesGroup: " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      os <<
        "kNoteInChordInGraceNotesGroup: " <<
        nonSoundingNoteEssentialsAsString () <<
        ", fNoteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteRegularInChord:
      os <<
        "kNoteRegularInChord: " <<
        soundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteRegularInTuplet:
      os <<
        "kNoteRegularInTuplet: " <<
        soundingNoteEssentialsAsString ();
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectUpLinkToTuplet->getTupletActualNotes (),
            fNoteDirectUpLinkToTuplet->getTupletNormalNotes ())
            */
      break;

    case msrNoteKind::kNoteRestInTuplet:
      os <<
        "kNoteRestInTuplet: " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      os <<
        "kNoteInTupletInGraceNotesGroup " <<
        nonSoundingNoteEssentialsAsString ();
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectUpLinkToTuplet->getTupletActualNotes (),
            fNoteDirectUpLinkToTuplet->getTupletNormalNotes ())
            */

      os <<
        ", fNoteTupletFactor: " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      os <<
        "kNoteUnpitchedInTuplet: " <<
        noteSoundingWholeNotesAsMsrString ();
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectUpLinkToTuplet->getTupletActualNotes (),
            fNoteDirectUpLinkToTuplet->getTupletNormalNotes ())
            */
      os <<
        ", noteTupletFactor: " << fNoteTupletFactor.asString ();
      break;
  } // switch

  os <<
    ", line " << fInputLineNumber <<
    endl;
}

void msrNote::print (ostream& os) const
{
  os <<
    "[Note ";

  // print the note essentials
  printNoteEssentials (os);

  ++gIndenter;

/* JMI
  os << left <<
    ", measurePosition: ";
    / * JMI
  if (fMeasureElementMeasurePosition == msrMoment::K_NO_POSITION_MEASURE_NUMBER) {
    os << "unknown (" << fMeasureElementMeasurePosition << ")";
  }
  else {
    os << fMeasureElementMeasurePosition;
  }
  * /
  os << fMeasureElementMeasurePosition;

  os <<
    ", voicePosition: " <<
    fMeasureElementVoicePosition;
*/

  const int fieldWidth = 44;

  os << left <<
    setw (fieldWidth) <<
    "fNoteIsACueNoteKind" << " : " <<
    msrNoteIsACueNoteKindAsString (fNoteIsACueNoteKind) <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteOctaveShift" << " : ";
  if (fNoteOctaveShift) {
    os << endl;
    ++gIndenter;

    os <<
      fNoteOctaveShift;

    --gIndenter;
  }
  else {
    os << "[NONE]" << endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "fNotePrintObjectKind" << " : " <<
    fNotePrintObjectKind <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteHeadKind" << " : " <<
    fNoteHeadKind <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteHeadFilledKind" << " : " <<
    fNoteHeadFilledKind <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteHeadParenthesesKind" << " : " <<
    msrNoteHeadParenthesesKindAsString (fNoteHeadParenthesesKind) <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteAccidentalKind" << " : " <<
    fNoteAccidentalKind <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteEditorialAccidentalKind" << " : " <<
    fNoteEditorialAccidentalKind <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteCautionaryAccidentalKind" << " : " <<
    fNoteCautionaryAccidentalKind <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteBelongsToAChord" << " : " <<
    fNoteBelongsToAChord <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteBelongsToATuplet" << " : " <<
    fNoteBelongsToATuplet <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteOccupiesAFullMeasure" << " : " <<
    fNoteOccupiesAFullMeasure <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteBelongsToAMultipleFullBarRests" << " : " <<
    fNoteBelongsToAMultipleFullBarRests <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteMultipleFullBarRestsSequenceNumber" << " : " <<
    fNoteMultipleFullBarRestsSequenceNumber <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteIsStemless" << " : " <<
    fNoteIsStemless <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteIsAChordsFirstMemberNote" << " : " <<
    fNoteIsAChordsFirstMemberNote <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteIsFirstNoteInADoubleTremolo" << " : " <<
    fNoteIsFirstNoteInADoubleTremolo <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteIsSecondNoteInADoubleTremolo" << " : " <<
    fNoteIsSecondNoteInADoubleTremolo <<
    endl;

  os <<
    setw (fieldWidth) <<
    "fNoteTrillOrnament" << " : ";
  if (fNoteTrillOrnament) {
    os << endl;
    ++gIndenter;

    os <<
      fNoteTrillOrnament;

    --gIndenter;
  }
  else {
    os << "[NONE]" << endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "fNoteIsFollowedByGraceNotesGroup" << " : " <<
    fNoteIsFollowedByGraceNotesGroup <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteColorAlphaRGB" << " : " <<
    fNoteColorAlphaRGB <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteColorAlphaRGBHasBenSet" << " : " <<
    fNoteColorAlphaRGBHasBenSet <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteSoloNoteOrRestInVoiceKind" << " : " <<
    fNoteSoloNoteOrRestInVoiceKind <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteSoloNoteOrRestInStaffKind" << " : " <<
    fNoteSoloNoteOrRestInStaffKind <<
    endl;

  os <<
    setw (fieldWidth) <<
    "fNoteIsACueNoteKind" << " : " <<
    msrNoteIsACueNoteKindAsString (fNoteIsACueNoteKind) <<
    endl;

//  ++gIndenter;

  // print measure number
  os << left <<
    setw (fieldWidth) <<
    "measureElementMeasureNumber" << " : ";
  if (fetchMeasureElementMeasureNumber () == K_NO_MEASURE_NUMBER) {
    os <<
      "unknown";
  }
  else if (fetchMeasureElementMeasureNumber ().size ()) {
    os <<
      fetchMeasureElementMeasureNumber ();
  }
  else {
    stringstream s;

    s <<
      "fetchMeasureElementMeasureNumber () is empty in note " <<
      this->asString () <<
      "'";

// JMI     msrInternalError (
    msrInternalWarning (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      s.str ());
  }
  os << endl;

  // print position in measure
  os << left <<
    setw (fieldWidth) <<
    "fMeasureElementMeasurePosition" << " : " <<
    fMeasureElementMeasurePosition <<
    endl;

  // print position from beginning of voice
//   os << left <<
//     setw (fieldWidth) <<
//     "fMeasureElementVoicePosition" << " : " <<
//     fMeasureElementVoicePosition <<
//     endl <<
//     setw (fieldWidth) <<
//     "fMeasureElementVoiceMoment" << " : " <<
//     endl;
//   ++gIndenter;
//   os <<
//     fMeasureElementVoiceMoment;
//   --gIndenter;

  // print note measure uplink
  os <<
    setw (fieldWidth) <<
    "fMeasureElementUpLinkToMeasure" << " : ";

  if (fMeasureElementUpLinkToMeasure) {
    os << endl;

    ++gIndenter;

    os <<
      fMeasureElementUpLinkToMeasure->asShortString () <<
      endl;

    --gIndenter;
  }
  else {
    os <<
      "[NONE]";
  }
  os << endl;

  // print note chord uplink
  os <<
    setw (fieldWidth) <<
    "fNoteDirectUpLinkToChord" << " : ";

  if (fNoteDirectUpLinkToChord) {
    os <<
      fNoteDirectUpLinkToChord->asShortString ();
  }
  else {
    os <<
      " : " << "[NONE]";
  }
  os << endl;

  // print note grace notes group uplink
  os <<
    setw (fieldWidth) <<
    "fNoteDirectUpLinkToGraceNotesGroup" << " :";

  if (fNoteDirectUpLinkToGraceNotesGroup) {
    os << endl;
    ++gIndenter;
    os <<
      fNoteDirectUpLinkToGraceNotesGroup->asShortString ();
    --gIndenter;
  }
  else {
    os <<
      ' ' << "[NONE]";
  }
  os << endl;

  // print note tuplet uplink
  os <<
    setw (fieldWidth) <<
    "fNoteDirectUpLinkToTuplet" << " : ";

  if (fNoteDirectUpLinkToTuplet) {
    os <<
      fNoteDirectUpLinkToTuplet->asShortString ();
  }
  else {
    os <<
      " : " << "[NONE]";
  }
  os << endl;

  // print sounding and displayed whole notes
  switch (fNoteKind) {
    case msrNoteKind::kNote_NO_:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInMeasure:
    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteInDoubleTremolo:
    case msrNoteKind::kNoteRegularInChord:
      os << left <<
        setw (fieldWidth) <<
        "fMeasureElementSoundingWholeNotes" << " : " <<
        fMeasureElementSoundingWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << " : " <<
        fNoteDisplayWholeNotes <<
        endl;
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      os <<
        setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << " : " <<
        fNoteDisplayWholeNotes <<
        endl;
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      os <<
        setw (fieldWidth) <<
        "fMeasureElementSoundingWholeNotes" << " : " <<
        fMeasureElementSoundingWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << " : " <<
        fNoteDisplayWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "tupletSoundingWholeNotes" << " : ";

        if (fNoteDirectUpLinkToTuplet) {
          os <<
            wholeNotesAsMsrString (
              fInputLineNumber,
              getNoteDirectUpLinkToTuplet ()->
                getMeasureElementSoundingWholeNotes ());
        }
        else {
          os <<
            "*** unknown yet ***";
        }
        os << endl;

      os <<
        setw (fieldWidth) <<
        "fNoteTupletFactor" << " : " <<
        endl;

      ++gIndenter;
        os <<
          fNoteTupletFactor;
      --gIndenter;
        break;
  } // switch

  // print note full measure length
  // may be unknown if there is no time signature
  Rational
    measureFullLength =
      fMeasureElementUpLinkToMeasure
        ?
          fMeasureElementUpLinkToMeasure->
            getFullMeasureWholeNotesDuration ()
        : Rational (0, 1); // JMI KAKA

  os << left <<
    setw (fieldWidth) <<
    "measureFullLength" << " : ";
  if (measureFullLength.getNumerator () == 0) {
    os <<
      "unknown, there's no time signature";
  }
  else {
    os <<
      measureFullLength;
  }
  os << endl;

  // chord member?
  os << left <<
    setw (fieldWidth) <<
    "fNoteBelongsToAChord" << " : " <<
    fNoteBelongsToAChord <<
    endl;

  // tuplet member?
  os << left <<
    setw (fieldWidth) <<
    "fNoteBelongsToATuplet" << " : " <<
    fNoteBelongsToATuplet <<
    endl;

  // note occupied a full measure?
  os << left <<
    setw (fieldWidth) <<
    "fNoteOccupiesAFullMeasure" << " : " <<
    fNoteOccupiesAFullMeasure <<
    endl;

  // multiple rest member?
  os << left <<
    setw (fieldWidth) <<
    "fNoteBelongsToAMultipleFullBarRests" << " : " <<
    fNoteBelongsToAMultipleFullBarRests <<
    endl <<
    setw (fieldWidth) <<
    "fNoteMultipleFullBarRestsSequenceNumber" << " : " <<
    fNoteMultipleFullBarRestsSequenceNumber <<
    endl;

  // print the grace notes group before if any
  if (fNoteGraceNotesGroupBefore) {
    os <<
      setw (fieldWidth) <<
      "fNoteGraceNotesGroupBefore";
    if (fNoteGraceNotesGroupBefore) {
      os << endl;

      ++gIndenter;

      os << fNoteGraceNotesGroupBefore;

      --gIndenter;
    }
    else {
      os << " : " <<
        "[NONE]" <<
        endl;
    }
  }

  // note head
  os << left <<
    setw (fieldWidth) <<
    "fNoteHeadKind" << " : " <<
    fNoteHeadKind <<
    endl <<
    setw (fieldWidth) <<
    "fNoteHeadFilledKind" << " : " <<
    fNoteHeadFilledKind <<
    endl <<
    setw (fieldWidth) <<
    "fNoteHeadParenthesesKind" << " : " <<
    fNoteHeadParenthesesKind <<
    endl;

  // accidentals
  os << left <<
    setw (fieldWidth) <<
    "fNoteAccidentalKind" << " : " <<
    msrAccidentalKindAsString (
      fNoteAccidentalKind) <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fNoteEditorialAccidentalKind" << " : " <<
    fNoteEditorialAccidentalKind <<
    endl <<
    setw (fieldWidth) <<
    "fNoteCautionaryAccidentalKind" << " : " <<
    fNoteCautionaryAccidentalKind <<
    endl;

  // cue note???
  os << left <<
    setw (fieldWidth) <<
    "fNoteIsACueNoteKind" << " : " <<
    fNoteIsACueNoteKind <<
    endl;

  // short cuts for efficiency
  os << left <<
    setw (fieldWidth) <<
    "noteIsAGraceNote" << " : " <<
    getNoteIsAGraceNote () <<
    endl;

  // note redundant information (for speed)

  stringstream s;

// JMI  if (fNoteIsStemless) {
    os <<
      setw (fieldWidth) <<
      "fNoteIsStemless" <<
      " : " <<
      fNoteIsStemless <<
      endl;
// JMI  }

 // JMI if (fNoteIsAChordsFirstMemberNote) {
    os <<
      setw (fieldWidth) <<
      "fNoteIsAChordsFirstMemberNote" <<
      " : " <<
      fNoteIsAChordsFirstMemberNote <<
      endl;
 // JMI }

 // JMI if (fNoteIsFirstNoteInADoubleTremolo) {
    os <<
      setw (fieldWidth) <<
      "fNoteIsFirstNoteInADoubleTremolo" <<
      " : " <<
      fNoteIsFirstNoteInADoubleTremolo <<
      endl;
// JMI  }
 // JMI if (fNoteIsSecondNoteInADoubleTremolo) {
    os <<
      setw (fieldWidth) <<
      "fNoteIsSecondNoteInADoubleTremolo" <<
      " : " <<
      fNoteIsSecondNoteInADoubleTremolo <<
      endl;
// JMI  }

  os <<
    setw (fieldWidth) <<
    "fNoteDashesOrnament" << " : ";
  if (fNoteDashesOrnament) {
    os << endl;
    ++gIndenter;

    os <<
      fNoteDashesOrnament;

    --gIndenter;
  }
  else {
    os << "[NONE]" << endl;
  }

  os <<
    setw (fieldWidth) <<
    "fNoteDelayedTurnOrnament" << " : ";
  if (fNoteDelayedTurnOrnament) {
    os << endl;
    ++gIndenter;

    os <<
      fNoteDelayedTurnOrnament;

    --gIndenter;
  }
  else {
    os << "[NONE]" << endl;
  }

  os <<
    setw (fieldWidth) <<
    "fNoteDelayedInvertedTurnOrnament" << " : ";
  if (fNoteDelayedInvertedTurnOrnament) {
    os << endl;
    ++gIndenter;

    os <<
      fNoteDelayedInvertedTurnOrnament;

    --gIndenter;
  }
  else {
    os << "[NONE]" << endl;
  }

  os <<
    setw (fieldWidth) <<
    "fNoteWavyLineSpannerStart" << " : ";
  if (fNoteWavyLineSpannerStart) {
    os << endl;
    ++gIndenter;

    os <<
      fNoteWavyLineSpannerStart;

    --gIndenter;
  }
  else {
    os << "[NONE]" << endl;
  }

  os <<
    setw (fieldWidth) <<
    "fNoteWavyLineSpannerStop" << " : ";
  if (fNoteWavyLineSpannerStop) {
    os << endl;
    ++gIndenter;

    os <<
      fNoteWavyLineSpannerStop;

    --gIndenter;
  }
  else {
    os << "[NONE]" << endl;
  }

  if (fNoteIsFollowedByGraceNotesGroup) {
    os <<
      setw (fieldWidth) <<
      "fNoteIsFollowedByGraceNotesGroup" <<
      " : " <<
      fNoteIsFollowedByGraceNotesGroup <<
      endl;
  }

  // print whole notes durations as MSR strings
  switch (fNoteKind) {
    case msrNoteKind::kNote_NO_:
      break;

    case msrNoteKind::kNoteRestInMeasure:
      os << left <<
        setw (fieldWidth) <<
        "noteSoundingWholeNotesAsMsrString" << " : \"" <<
        noteSoundingWholeNotesAsMsrString () <<
        "\"" <<
        endl;
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      os << left <<
        setw (fieldWidth) <<
        "noteSoundingWholeNotesAsMsrString" << " : \"" <<
        noteSoundingWholeNotesAsMsrString () <<
        "\"" <<
        endl;
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      os << left <<
        setw (fieldWidth) <<
        "noteSoundingWholeNotesAsMsrString" << " : \"" <<
        noteSoundingWholeNotesAsMsrString () <<
        "\"" <<
        endl <<
        setw (fieldWidth) <<
        "noteDisplayWholeNotesAsMsrString" << " : \"" <<
        noteDisplayWholeNotesAsMsrString () <<
        "\"" <<
        endl <<
        setw (fieldWidth) <<
        "noteGraphicDurationAsMsrString" << " : \"" <<
        noteGraphicDurationAsMsrString () <<
        "\"" <<
        setw (fieldWidth) <<
        "noteGraphicDurationAsMusicXMLString" << " : \"" <<
        noteGraphicDurationAsMsrString () <<
        "\"" <<
        endl;
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      os << left <<
        setw (fieldWidth) <<
        "noteSoundingWholeNotesAsMsrString" << " : \"" <<
        noteSoundingWholeNotesAsMsrString () <<
        "\"" <<
        endl <<
        setw (fieldWidth) <<
        "noteDisplayWholeNotesAsMsrString" << " : \"" <<
        noteDisplayWholeNotesAsMsrString () <<
        "\"" <<
        endl <<
        setw (fieldWidth) <<
        "noteGraphicDurationAsMsrString" << " : \"" <<
        noteGraphicDurationAsMsrString () <<
        "\"" <<
        endl;
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      // JMI
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      os << left <<
        setw (fieldWidth) <<
        "noteGraphicDurationAsMsrString" << " : \"" <<
        noteGraphicDurationAsMsrString () <<
        "\"" <<
        endl;
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      os << left <<
        setw (fieldWidth) <<
        "fMeasureElementSoundingWholeNotes" << " : " <<
        fMeasureElementSoundingWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << " : " <<
        fNoteDisplayWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "noteGraphicDurationAsMsrString" << " : \"" <<
        noteGraphicDurationAsMsrString () <<
        "\"" <<
        endl;
      break;

    case msrNoteKind::kNoteRegularInChord:
      // JMI
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      /* JMI KAKA
      os << left <<
        setw (fieldWidth) <<
        "fNoteTupletNoteGraphicDurationAsMsrString" << " : \"" <<
        fNoteTupletNoteGraphicDurationAsMsrString <<
        "\"" <<
        endl <<
        setw (fieldWidth) <<
        "noteTupletNoteSoundingWholeNotesAsMsrString" << " : ";
          */

      if (fNoteDirectUpLinkToTuplet) {
        os <<
          "\"" <<
          wholeNotesAsMsrString (
            fInputLineNumber,
            getNoteDirectUpLinkToTuplet ()->
              getMeasureElementSoundingWholeNotes ()) <<
          "\"";
      }
      else {
        os <<
          "*** unknown yet ***";
      }
      os << endl;

      os <<
        setw (fieldWidth) <<
        "fPositionInTuplet" << " : " <<
        fPositionInTuplet <<
        endl;

      os <<
        setw (fieldWidth) <<
        "noteGraphicDurationAsMsrString" << " : \"" <<
        noteGraphicDurationAsMsrString () <<
        "\"" <<
        endl;
      break;
  } // switch

  // print the octave shift if any
  os << left <<
    setw (fieldWidth) <<
    "fNoteOctaveShift" << " : ";
  if (fNoteOctaveShift) {
    os << endl;
    ++gIndenter;
    os <<
      fNoteOctaveShift;
    --gIndenter;
  }
  else {
    os << "[NONE]" << endl;
  }

  // print the stem if any
  os << left <<
    setw (fieldWidth) <<
    "fNoteStem" << " : ";
  if (fNoteStem) {
    os << endl;
    ++gIndenter;

    os <<
      fNoteStem;

    --gIndenter;
  }
  else {
    os << "[NONE]" << endl;
  }

  // print the note color
  os <<
    setw (fieldWidth) <<
    "fNoteColorAlphaRGB" << " : " <<
    fNoteColorAlphaRGB.asString () <<
    endl <<
    setw (fieldWidth) <<
    "fNoteColorAlphaRGBHasBenSet" << " : " <<
    fNoteColorAlphaRGBHasBenSet <<
    endl;

  // solo note or rest?
  os <<
    setw (fieldWidth) <<
    "fNoteSoloNoteOrRestInVoiceKind" << " : " <<
    fNoteSoloNoteOrRestInVoiceKind <<
    endl <<
    setw (fieldWidth) <<
    "fNoteSoloNoteOrRestInStaffKind" << " : " <<
    fNoteSoloNoteOrRestInStaffKind <<
    endl;

  // print the tie if any
  os <<
    setw (fieldWidth) <<
    "fNoteTie" << " : ";
  if (fNoteTie) {
    os << endl;
    ++gIndenter;

    os <<
      fNoteTie;

    --gIndenter;
  }
  else {
    os << "[NONE]" << endl;
  }

  // print the beams if any
  os <<
    setw (fieldWidth) <<
    "fNoteBeams";
  if (fNoteBeams.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrBeam>::const_iterator
      iBegin = fNoteBeams.begin (),
      iEnd   = fNoteBeams.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the articulations if any
  os <<
    setw (fieldWidth) <<
    "fNoteArticulations";
  if (fNoteArticulations.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrArticulation>::const_iterator
      iBegin = fNoteArticulations.begin (),
      iEnd   = fNoteArticulations.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the spanners if any
  os <<
    setw (fieldWidth) <<
    "fNoteSpanners";
  if (fNoteSpanners.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrSpanner>::const_iterator
      iBegin = fNoteSpanners.begin (),
      iEnd   = fNoteSpanners.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the technicals if any
  os <<
    setw (fieldWidth) <<
    "fNoteTechnicals";
  if (fNoteTechnicals.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrTechnical>::const_iterator
      iBegin = fNoteTechnicals.begin (),
      iEnd   = fNoteTechnicals.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the technicals with integer if any
  os <<
    setw (fieldWidth) <<
    "fNoteTechnicalWithIntegers";
  if (fNoteTechnicalWithIntegers.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrTechnicalWithInteger>::const_iterator
      iBegin = fNoteTechnicalWithIntegers.begin (),
      iEnd   = fNoteTechnicalWithIntegers.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the technicals with float if any
  os <<
    setw (fieldWidth) <<
    "fNoteTechnicalWithFloats";
  if (fNoteTechnicalWithFloats.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrTechnicalWithFloat>::const_iterator
      iBegin = fNoteTechnicalWithFloats.begin (),
      iEnd   = fNoteTechnicalWithFloats.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the technicals with string if any
  os <<
    setw (fieldWidth) <<
    "fNoteTechnicalWithStrings";
  if (fNoteTechnicalWithStrings.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrTechnicalWithString>::const_iterator
      iBegin = fNoteTechnicalWithStrings.begin (),
      iEnd   = fNoteTechnicalWithStrings.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the ornaments if any
  os <<
    setw (fieldWidth) <<
    "fNoteOrnaments";
  if (fNoteOrnaments.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrOrnament>::const_iterator
      iBegin = fNoteOrnaments.begin (),
      iEnd   = fNoteOrnaments.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the glissandos if any
  os <<
    setw (fieldWidth) <<
    "fNoteGlissandos";
  if (fNoteGlissandos.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrGlissando>::const_iterator
      iBegin = fNoteGlissandos.begin (),
      iEnd   = fNoteGlissandos.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the slides if any
  os <<
    setw (fieldWidth) <<
    "fNoteSlides";
  if (fNoteSlides.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrSlide>::const_iterator
      iBegin = fNoteSlides.begin (),
      iEnd   = fNoteSlides.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the singleTremolo if any
  os <<
    setw (fieldWidth) <<
    "fNoteSingleTremolo";
  if (fNoteSingleTremolo) {
    os << endl;

    ++gIndenter;

    os << fNoteSingleTremolo;

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the dynamics if any
  os <<
    setw (fieldWidth) <<
    "fNoteDynamics";
  if (fNoteDynamics.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrDynamic>::const_iterator
      iBegin = fNoteDynamics.begin (),
      iEnd   = fNoteDynamics.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the other dynamics if any
  os <<
    setw (fieldWidth) <<
    "fNoteOtherDynamics";
  if (fNoteOtherDynamics.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrOtherDynamic>::const_iterator
      iBegin = fNoteOtherDynamics.begin (),
      iEnd   = fNoteOtherDynamics.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the words if any
  os <<
    setw (fieldWidth) <<
    "fNoteWords";
  if (fNoteWords.size ()) {
    os << endl;
    ++gIndenter;

    list<S_msrWords>::const_iterator
      iBegin = fNoteWords.begin (),
      iEnd   = fNoteWords.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the slurs if any
  os <<
    setw (fieldWidth) <<
    "fNoteSlurs";
  if (fNoteSlurs.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrSlur>::const_iterator
      iBegin = fNoteSlurs.begin (),
      iEnd   = fNoteSlurs.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the ligatures if any
  os <<
    setw (fieldWidth) <<
    "fNoteLigatures";
  if (fNoteLigatures.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrLigature>::const_iterator
      iBegin = fNoteLigatures.begin (),
      iEnd   = fNoteLigatures.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the pedals if any
  os <<
    setw (fieldWidth) <<
    "fNotePedals";
  if (fNotePedals.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrPedal>::const_iterator
      iBegin = fNotePedals.begin (),
      iEnd   = fNotePedals.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the slashes if any
  os <<
    setw (fieldWidth) <<
    "fNoteSlashes";
  if (fNoteSlashes.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrSlash>::const_iterator
      iBegin = fNoteSlashes.begin (),
      iEnd   = fNoteSlashes.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the crescDecresc if any
  os <<
    setw (fieldWidth) <<
    "fNoteCrescDecrescs";
  if (fNoteCrescDecrescs.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrCrescDecresc>::const_iterator
      iBegin = fNoteCrescDecrescs.begin (),
      iEnd   = fNoteCrescDecrescs.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the wedges if any
  os <<
    setw (fieldWidth) <<
    "fNoteWedges";
  if (fNoteWedges.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrWedge>::const_iterator
      iBegin = fNoteWedges.begin (),
      iEnd   = fNoteWedges.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the segnos if any
  os <<
    setw (fieldWidth) <<
    "fNoteSegnos";
  if (fNoteSegnos.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrSegno>::const_iterator
      iBegin = fNoteSegnos.begin (),
      iEnd   = fNoteSegnos.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the dal segnos if any
  os <<
    setw (fieldWidth) <<
    "fNoteDalSegnos";
  if (fNoteDalSegnos.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrDalSegno>::const_iterator
      iBegin = fNoteDalSegnos.begin (),
      iEnd   = fNoteDalSegnos.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the codas if any
  os <<
    setw (fieldWidth) <<
    "fNoteCodas";
  if (fNoteCodas.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrCoda>::const_iterator
      iBegin = fNoteCodas.begin (),
      iEnd   = fNoteCodas.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the eyeglasses if any
  os <<
    setw (fieldWidth) <<
    "fNoteEyeGlasses";
  if (fNoteEyeGlasses.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrEyeGlasses>::const_iterator
      iBegin = fNoteEyeGlasses.begin (),
      iEnd   = fNoteEyeGlasses.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the damps if any
  os <<
    setw (fieldWidth) <<
    "fNoteDamps";
  if (fNoteDamps.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrDamp>::const_iterator
      iBegin = fNoteDamps.begin (),
      iEnd   = fNoteDamps.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the dampAlls if any
  os <<
    setw (fieldWidth) <<
    "fNoteDampAlls";
  if (fNoteDampAlls.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrDampAll>::const_iterator
      iBegin = fNoteDampAlls.begin (),
      iEnd   = fNoteDampAlls.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the scordaturas if any
  os <<
    setw (fieldWidth) <<
    "fNoteScordaturas";
  if (fNoteScordaturas.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrScordatura>::const_iterator
      iBegin = fNoteScordaturas.begin (),
      iEnd   = fNoteScordaturas.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the harmonies associated to this note if any
  os <<
    setw (fieldWidth) <<
    "fNoteHarmoniesList";
  if (fNoteHarmoniesList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrHarmony>::const_iterator
      iBegin = fNoteHarmoniesList.begin (),
      iEnd   = fNoteHarmoniesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrHarmony
        harmony = (*i);

      os << harmony;

      if (++i == iEnd) break;
      // no endl here
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the figured bass elements associated to this note if any
  os <<
    setw (fieldWidth) <<
    "fNoteFiguredBassesList";
  if (fNoteFiguredBassesList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrFiguredBass>::const_iterator
      iBegin = fNoteFiguredBassesList.begin (),
      iEnd   = fNoteFiguredBassesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrFiguredBass
        figuredBass = (*i);

      os << figuredBass;

      if (++i == iEnd) break;
      // no endl here
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the syllables associated to this note if any
  os <<
    setw (fieldWidth) <<
    "fNoteSyllables";
  if (fNoteSyllables.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrSyllable>::const_iterator
      iBegin = fNoteSyllables.begin (),
      iEnd   = fNoteSyllables.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrSyllable
        syllable = (*i);

      os << syllable;

/* JMI KAKA
      os <<
        msrSyllableKindAsString (syllable->getSyllableKind ()) <<
          syllable->getSyllableExtendKind () <<
        " : ";

      msrSyllable::writeTextsList (
        syllable->getSyllableTextsList (),
        os);

      os <<
        ", stanza " <<
        syllable->
          getSyllableUpLinkToStanza ()->
            getStanzaNumber () <<
        ", line " << syllable->getInputLineNumber () <<
        ", syllableUpLinkToNote: " <<
        syllable->
          getSyllableUpLinkToNote ()->
            asShortString ();
*/

      if (++i == iEnd) break;
      // no endl here
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  // print the after grace group notes after if any
  os <<
    setw (fieldWidth) <<
    "fNoteGraceNotesGroupAfter";
  if (fNoteGraceNotesGroupAfter) {
    os << endl;

    ++gIndenter;

    os << fNoteGraceNotesGroupAfter;

    --gIndenter;
  }
  else {
    os << " : " <<
      "[NONE]" <<
      endl;
  }

  --gIndenter;

  os << ']' << endl;
}

void msrNote::printShort (ostream& os) const
{
  os <<
    "[Note " ;

  // print the note essentials
  printNoteEssentials (os);

  ++gIndenter;

  const int fieldWidth = 44;

  // print position in measure
  os << left <<
    setw (fieldWidth) <<
    "fMeasureElementMeasurePosition" << " : " <<
    fMeasureElementMeasurePosition <<
    endl;

  // print position from beginning of voice
//   os << left <<
//     setw (fieldWidth) <<
//     "fMeasureElementVoicePosition" << " : " <<
//     fMeasureElementVoicePosition <<
//     endl;

  // print sounding and displayed whole notes
  switch (fNoteKind) {
    case msrNoteKind::kNote_NO_:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteUnpitchedInMeasure:
    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteInDoubleTremolo:
    case msrNoteKind::kNoteRegularInChord:
      os << left <<
        setw (fieldWidth) <<
        "fMeasureElementSoundingWholeNotes" << " : " <<
        fMeasureElementSoundingWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << " : " <<
        fNoteDisplayWholeNotes <<
        endl;
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      os <<
        setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << " : " <<
        fNoteDisplayWholeNotes <<
        endl;
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      os << left <<
        setw (fieldWidth) <<
        "fMeasureElementSoundingWholeNotes" << " : " <<
        fMeasureElementSoundingWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << " : " <<
        fNoteDisplayWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "noteGraphicDurationAsMsrString" << " : \"" <<
        noteGraphicDurationAsMsrString () <<
        "\"" <<
        endl;
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      os <<
        setw (fieldWidth) <<
        "fMeasureElementSoundingWholeNotes" << " : " <<
        fMeasureElementSoundingWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << " : " <<
        fNoteDisplayWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "tupletSoundingWholeNotes" << " : ";

        if (fNoteDirectUpLinkToTuplet) {
          os <<
            wholeNotesAsMsrString (
              fInputLineNumber,
              getNoteDirectUpLinkToTuplet ()->
                getMeasureElementSoundingWholeNotes ());
        }
        else {
          os <<
            "*** unknown yet ***";
        }
        os << endl;

      os <<
        setw (fieldWidth) <<
        "fNoteTupletFactor" << " : " <<
        endl;

      ++gIndenter;
        os <<
          fNoteTupletFactor;
      --gIndenter;
      break;
    } // switch

  // print the grace notes group before if any
  if (fNoteGraceNotesGroupBefore) {
    os <<
      setw (fieldWidth) <<
      "fNoteGraceNotesGroupBefore";
      os << endl;

    ++gIndenter;

    fNoteGraceNotesGroupBefore->printShort (os);

    --gIndenter;
  }

  // print the beams if any
  if (fNoteBeams.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteBeams";
      os << endl;

    ++gIndenter;

    list<S_msrBeam>::const_iterator
      iBegin = fNoteBeams.begin (),
      iEnd   = fNoteBeams.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the articulations if any
  if (fNoteArticulations.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteArticulations";
      os << endl;

    ++gIndenter;

    list<S_msrArticulation>::const_iterator
      iBegin = fNoteArticulations.begin (),
      iEnd   = fNoteArticulations.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the spanners if any
  if (fNoteSpanners.size ()) {
      os <<
        setw (fieldWidth) <<
        "fNoteSpanners";
        os << endl;

      ++gIndenter;

      list<S_msrSpanner>::const_iterator
        iBegin = fNoteSpanners.begin (),
        iEnd   = fNoteSpanners.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
  }

  // print the technicals if any
  if (fNoteTechnicals.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteTechnicals";
      os << endl;

    ++gIndenter;

    list<S_msrTechnical>::const_iterator
      iBegin = fNoteTechnicals.begin (),
      iEnd   = fNoteTechnicals.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the technicals with integer if any
  if (fNoteTechnicalWithIntegers.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteTechnicalWithIntegers";
      os << endl;

    ++gIndenter;

    list<S_msrTechnicalWithInteger>::const_iterator
      iBegin = fNoteTechnicalWithIntegers.begin (),
      iEnd   = fNoteTechnicalWithIntegers.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the technicals with float if any
  if (fNoteTechnicalWithFloats.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteTechnicalWithFloats";
      os << endl;

    ++gIndenter;

    list<S_msrTechnicalWithFloat>::const_iterator
      iBegin = fNoteTechnicalWithFloats.begin (),
      iEnd   = fNoteTechnicalWithFloats.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the technicals with string if any
  if (fNoteTechnicalWithStrings.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteTechnicalWithStrings";
      os << endl;

    ++gIndenter;

    list<S_msrTechnicalWithString>::const_iterator
      iBegin = fNoteTechnicalWithStrings.begin (),
      iEnd   = fNoteTechnicalWithStrings.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the ornaments if any
  if (fNoteOrnaments.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteOrnaments";
      os << endl;

    ++gIndenter;

    list<S_msrOrnament>::const_iterator
      iBegin = fNoteOrnaments.begin (),
      iEnd   = fNoteOrnaments.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the glissandos if any
  if (fNoteGlissandos.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteGlissandos";
      os << endl;

    ++gIndenter;

    list<S_msrGlissando>::const_iterator
      iBegin = fNoteGlissandos.begin (),
      iEnd   = fNoteGlissandos.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the slides if any
  if (fNoteSlides.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteSlides";
      os << endl;

    ++gIndenter;

    list<S_msrSlide>::const_iterator
      iBegin = fNoteSlides.begin (),
      iEnd   = fNoteSlides.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the singleTremolo if any
  if (fNoteSingleTremolo) {
    os <<
      setw (fieldWidth) <<
      "fNoteSingleTremolo";
      os << endl;

    ++gIndenter;

    os << fNoteSingleTremolo;

    --gIndenter;
  }

  // print the dynamics if any
  if (fNoteDynamics.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteDynamics";
      os << endl;

    ++gIndenter;

    list<S_msrDynamic>::const_iterator
      iBegin = fNoteDynamics.begin (),
      iEnd   = fNoteDynamics.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the other dynamics if any
  if (fNoteOtherDynamics.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteOtherDynamics";
      os << endl;

    ++gIndenter;

    list<S_msrOtherDynamic>::const_iterator
      iBegin = fNoteOtherDynamics.begin (),
      iEnd   = fNoteOtherDynamics.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the words if any
  if (fNoteWords.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteWords";
      os << endl;

    ++gIndenter;

    list<S_msrWords>::const_iterator
      iBegin = fNoteWords.begin (),
      iEnd   = fNoteWords.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the slurs if any
  if (fNoteSlurs.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteSlurs";
      os << endl;

    ++gIndenter;

    list<S_msrSlur>::const_iterator
      iBegin = fNoteSlurs.begin (),
      iEnd   = fNoteSlurs.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the ligatures if any
  if (fNoteLigatures.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteLigatures";
      os << endl;

    ++gIndenter;

    list<S_msrLigature>::const_iterator
      iBegin = fNoteLigatures.begin (),
      iEnd   = fNoteLigatures.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the pedals if any
  if (fNotePedals.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNotePedals";
      os << endl;

    ++gIndenter;

    list<S_msrPedal>::const_iterator
      iBegin = fNotePedals.begin (),
      iEnd   = fNotePedals.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the slashes if any
  if (fNoteSlashes.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteSlashes";
      os << endl;

    ++gIndenter;

    list<S_msrSlash>::const_iterator
      iBegin = fNoteSlashes.begin (),
      iEnd   = fNoteSlashes.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the crescDecresc if any
  if (fNoteCrescDecrescs.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteCrescDecrescs";
      os << endl;

    ++gIndenter;

    list<S_msrCrescDecresc>::const_iterator
      iBegin = fNoteCrescDecrescs.begin (),
      iEnd   = fNoteCrescDecrescs.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the wedges if any
  if (fNoteWedges.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteWedges";
      os << endl;

    ++gIndenter;

    list<S_msrWedge>::const_iterator
      iBegin = fNoteWedges.begin (),
      iEnd   = fNoteWedges.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the segnos if any
  if (fNoteSegnos.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteSegnos";
      os << endl;

    ++gIndenter;

    list<S_msrSegno>::const_iterator
      iBegin = fNoteSegnos.begin (),
      iEnd   = fNoteSegnos.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the dal segnos if any
  if (fNoteDalSegnos.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteDalSegnos";
      os << endl;

    ++gIndenter;

    list<S_msrDalSegno>::const_iterator
      iBegin = fNoteDalSegnos.begin (),
      iEnd   = fNoteDalSegnos.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the codas if any
  if (fNoteCodas.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteCodas";
      os << endl;

    ++gIndenter;

    list<S_msrCoda>::const_iterator
      iBegin = fNoteCodas.begin (),
      iEnd   = fNoteCodas.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the eyeglasses if any
  if (fNoteEyeGlasses.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteEyeGlasses";
      os << endl;

    ++gIndenter;

    list<S_msrEyeGlasses>::const_iterator
      iBegin = fNoteEyeGlasses.begin (),
      iEnd   = fNoteEyeGlasses.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the damps if any
  if (fNoteDamps.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteDamps";
      os << endl;

    ++gIndenter;

    list<S_msrDamp>::const_iterator
      iBegin = fNoteDamps.begin (),
      iEnd   = fNoteDamps.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the dampAlls if any
  if (fNoteDampAlls.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteDampAlls";
      os << endl;

    ++gIndenter;

    list<S_msrDampAll>::const_iterator
      iBegin = fNoteDampAlls.begin (),
      iEnd   = fNoteDampAlls.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the scordaturas if any
  if (fNoteScordaturas.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteScordaturas";
      os << endl;

    ++gIndenter;

    list<S_msrScordatura>::const_iterator
      iBegin = fNoteScordaturas.begin (),
      iEnd   = fNoteScordaturas.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here;
    } // for

    --gIndenter;
  }

  // print the harmonies associated to this note if any
  if (fNoteHarmoniesList.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteHarmoniesList";
      os << endl;

    ++gIndenter;

    list<S_msrHarmony>::const_iterator
      iBegin = fNoteHarmoniesList.begin (),
      iEnd   = fNoteHarmoniesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrHarmony
        harmony = (*i);

      os << harmony;

      if (++i == iEnd) break;
      // no endl here
    } // for

    --gIndenter;
  }

  // print the figured bass elements associated to this note if any
  if (fNoteFiguredBassesList.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteFiguredBassesList";
      os << endl;

    ++gIndenter;

    list<S_msrFiguredBass>::const_iterator
      iBegin = fNoteFiguredBassesList.begin (),
      iEnd   = fNoteFiguredBassesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrFiguredBass
        figuredBass = (*i);

      os << figuredBass;

      if (++i == iEnd) break;
      // no endl here
    } // for

    --gIndenter;
  }

  // print the syllables associated to this note if any
  if (fNoteSyllables.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteSyllables";
      os << endl;

    ++gIndenter;

    list<S_msrSyllable>::const_iterator
      iBegin = fNoteSyllables.begin (),
      iEnd   = fNoteSyllables.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrSyllable
        syllable = (*i);

      os << syllable;

      if (++i == iEnd) break;
      // no endl here
    } // for

    --gIndenter;
  }

  // print the after grace group notes after if any
  if (fNoteGraceNotesGroupAfter) {
    os <<
      setw (fieldWidth) <<
      "fNoteGraceNotesGroupAfter" <<
      endl;

    ++gIndenter;

    fNoteGraceNotesGroupAfter->printShort (os);

    --gIndenter;
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrNote& elt)
{
  if (elt) {
    os << elt->asString ();
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}


}

/* JMI
// tuplet upLink
S_msrTuplet msrNote::fetchNoteUpLinkToTuplet () const
{
  S_msrTuplet result;

  switch (fNoteKind) {
    case msrNoteKind::kNote_NO_:
      break;

    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteUnpitchedInMeasure:
      result = fNoteDirectUpLinkToTuplet;
      break;

    case msrNoteKind::kNoteRegularInChord:
      if (fNoteDirectUpLinkToChord) {
        result =
          fNoteDirectUpLinkToChord->
            fetchChordUpLinkToTuplet ();
      }
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      if (fNoteDirectUpLinkToTuplet) {
        result =
          fNoteDirectUpLinkToTuplet->
            fetchTupletUpLinkToTuplet ();
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    / * JMi
      if (fNoteDirectUpLinkToGraceNotesGroup) {
        result =
          fNoteDirectUpLinkToGraceNotesGroup->
            getGraceNotesGroupUpLinkToVoice ();
              / * JMI
              getGraceNotesGroupUpLinkToNote ()->
              fetchNoteUpLinkToVoice ();
              * /
      }
    * /
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      if (fNoteDirectUpLinkToChord) {
        result =
          fNoteDirectUpLinkToChord->
            getChordDirectUpLinkToGraceNotesGroup ()->
              fetchGraceNoteGroupUpLinkToTuplet ();
      }
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      / * cannot happen ??? JMI
      if (fNoteDirectUpLinkToTuplet) {
        result =
          fNoteDirectUpLinkToTuplet->
            getTupletDirectUpLinkToTuplet ()->
              fetchUpLinkToTupletToVoice ();
      }
      * /
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      break;
  } // switch

  return result;
}
*/

