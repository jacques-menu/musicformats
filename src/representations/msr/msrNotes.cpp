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

#include "msrPitchesNames.h"

#include "msrNotes.h"

#include "oahOah.h"

#include "msrOah.h"

#include "oahEarlyOptions.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrNote msrNote::create (
  int                        inputLineNumber,
  const string&              noteMeasureNumber,

  msrNoteKind                noteKind,

  msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,
  msrOctaveKind              noteOctaveKind,

  const rational&            noteSoundingWholeNotes,
  const rational&            noteDisplayWholeNotes,

  int                        noteDotsNumber,

  msrDurationKind            noteGraphicDurationKind,

  msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
  msrOctaveKind              noteDisplayOctaveKind,

  msrNoteIsACueNoteKind      noteIsACueNoteKind,

  msrPrintObjectKind         notePrintObjectKind,

  msrNoteHeadKind            noteHeadKind,
  msrNoteHeadFilledKind      noteHeadFilledKind,
  msrNoteHeadParenthesesKind noteHeadParenthesesKind)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
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

      noteIsACueNoteKind,

      notePrintObjectKind,

      noteHeadKind,
      noteHeadFilledKind,
      noteHeadParenthesesKind);
  assert (o != nullptr);

  return o;
}

msrNote::msrNote (
  int                        inputLineNumber,
  const string&              noteMeasureNumber,

  msrNoteKind                noteKind,

  msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,
  msrOctaveKind              noteOctaveKind,

  const rational&            noteSoundingWholeNotes,
  const rational&            noteDisplayWholeNotes,

  int                        noteDotsNumber,

  msrDurationKind            noteGraphicDurationKind,

  msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
  msrOctaveKind              noteDisplayOctaveKind,

  msrNoteIsACueNoteKind      noteIsACueNoteKind,

  msrPrintObjectKind         notePrintObjectKind,

  msrNoteHeadKind            noteHeadKind,
  msrNoteHeadFilledKind      noteHeadFilledKind,
  msrNoteHeadParenthesesKind noteHeadParenthesesKind)
  : msrTupletElement (inputLineNumber),
    fNoteAlphaRGBColor ("", "")
{
  fMeasureElementMeasureNumber = noteMeasureNumber;

  // basic note description
  fNoteKind = noteKind;

  fNoteQuarterTonesPitchKind  = noteQuarterTonesPitchKind;
  fNoteOctaveKind = noteOctaveKind;

  fMeasureElementSoundingWholeNotes = noteSoundingWholeNotes;
  fNoteDisplayWholeNotes  = noteDisplayWholeNotes;

  fNoteDotsNumber = noteDotsNumber;

  fNoteGraphicDurationKind = noteGraphicDurationKind;

  fNoteTupletFactor = rational (1, 1);

  fNoteQuarterTonesDisplayPitchKind = noteQuarterTonesDisplayPitchKind;
  fNoteDisplayOctaveKind            = noteDisplayOctaveKind;

  fNoteIsACueNoteKind   = noteIsACueNoteKind;

  fNotePrintObjectKind = notePrintObjectKind;

  fNoteHeadKind            = noteHeadKind;
  fNoteHeadFilledKind      = noteHeadFilledKind;
  fNoteHeadParenthesesKind = noteHeadParenthesesKind;

  // do other initializations
  initializeNote ();
}

void msrNote::initializeNote ()
{
  // rests handling
  // ------------------------------------------------------

  if (fetchNoteIsARest () && fNoteDisplayOctaveKind != msrOctaveKind::k_NoOctave) {
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

  fNoteBelongsToAChord = false;
  fNoteBelongsToATuplet = false;
  fNoteOccupiesAFullMeasure = false;

  fNoteBelongsToAFullBarRests = false;
  fNoteFullBarRestsSequenceNumber = -1;

  fNoteAlphaRGBColorHasBenSet = false;

  // note lyrics
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotesDetails ()) {
    gLogStream <<
      endl <<
      "Initializing a note" <<
      ", kind: ";
    if (fNoteKind == msrNoteKind::k_NoNote)
      gLogStream <<
        "not yet known";
    else
      gLogStream <<
        noteKindAsString (fNoteKind);
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
        "fNoteGraphicDurationKind" << " = ";
      if (fNoteGraphicDurationKind != msrDurationKind::k_NoDuration) {
        gLogStream <<
          msrDurationKindAsString (
            fNoteGraphicDurationKind);
      }
      else {
        gLogStream << "none";
      }

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
        msrOctaveKindAsString (fNoteOctaveKind) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteDisplayOctaveKind" << " = " <<
        msrOctaveKindAsString (fNoteDisplayOctaveKind) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteIsACueNoteKind" << " = " <<
         noteIsACueNoteKindAsString (fNoteIsACueNoteKind) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNotePrintObjectKind" << " = " <<
         msrPrintObjectKindAsString (fNotePrintObjectKind) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteHeadKind" << " = " <<
         noteHeadKindAsString (fNoteHeadKind) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteHeadFilledKind" << " = " <<
         noteHeadFilledKindAsString (fNoteHeadFilledKind) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteHeadParenthesesKind" << " = " <<
         noteHeadParenthesesKindAsString (fNoteHeadParenthesesKind) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteAccidentalKind" << " = " <<
        msrAccidentalKindAsString (
          fNoteAccidentalKind) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteEditorialAccidentalKind" << " = " <<
        msrEditorialAccidentalKindAsString (
          fNoteEditorialAccidentalKind) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteCautionaryAccidentalKind" << " = " <<
        msrCautionaryAccidentalKindAsString (
          fNoteCautionaryAccidentalKind) <<
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

  // note measure information
  // ------------------------------------------------------

  fNoteOccupiesAFullMeasure = false;

  // solo note or rest?
  // ------------------------------------------------------

  fNoteSoloNoteOrRestInVoiceKind =
    msrSoloNoteOrRestInVoiceKind::kSoloNoteOrRestInVoiceNo;

  fNoteSoloNoteOrRestInStaffKind =
    msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffNo;

  // note redundant information (for speed)
  // ------------------------------------------------------

  fNoteIsStemless = false;

  fNoteIsAChordsFirstMemberNote = false;

  fNoteIsFirstNoteInADoubleTremolo  = false;
  fNoteIsSecondNoteInADoubleTremolo = false;

  fNoteIsFollowedByGraceNotesGroup = false;
}

msrNote::~msrNote ()
{}

//________________________________________________________________________
S_msrMeasure msrNote::fetchNoteMeasureUpLink () const
{
  S_msrMeasure result;

  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
      break;

    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteUnpitchedInMeasure:
      result = fNoteDirectMeasureUpLink;
      break;

    case msrNoteKind::kNoteRegularInChord:
      if (fNoteDirectChordUpLink) {
        result =
          fNoteDirectChordUpLink->
            getChordDirectMeasureUpLink ();
      }
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      if (fNoteDirectTupletUpLink) {
        result =
          fNoteDirectTupletUpLink->
            getTupletDirectMeasureUpLink ();
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      // JMI
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    /* JMi
      if (fNoteDirectGraceNotesGroupUpLink) {
        result =
          fNoteDirectGraceNotesGroupUpLink->
            getGraceNotesGroupVoiceUpLink ();
              / * JMI
              getGraceNotesGroupNoteUpLink ()->
              fetchNoteVoiceUpLink ();
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
S_msrGraceNotesGroup msrNote::fetchNoteGraceNotesGroupUpLink () const
{
  S_msrGraceNotesGroup result;

  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
      break;

    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteRegularInChord:
      result = fNoteDirectGraceNotesGroupUpLink;
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      if (fNoteDirectTupletUpLink) {
      /* JMI
        result =
          fNoteDirectTupletUpLink->
            getTupletDirectMeasureUpLink ()->
              fetchMeasureVoiceUpLink ();
      */
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    /* JMi
      if (fNoteDirectGraceNotesGroupUpLink) {
        result =
          fNoteDirectGraceNotesGroupUpLink->
            getGraceNotesGroupVoiceUpLink ();
              / * JMI
              getGraceNotesGroupNoteUpLink ()->
              fetchNoteVoiceUpLink ();
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

S_msrVoice msrNote::fetchNoteVoiceUpLink () const
{
  S_msrVoice result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceNotes ()) {
    gLogStream <<
      "--> fetchNoteVoiceUpLink(): " <<
      asMinimalString () <<
      endl;
  }
#endif

  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
      break;

    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteRegularInChord:
      if (fNoteDirectMeasureUpLink) {
        result =
          fNoteDirectMeasureUpLink->
            fetchMeasureVoiceUpLink ();
      }
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      if (fNoteDirectTupletUpLink) {
        S_msrMeasure
          tupletDirectMeasureUpLink =
            fNoteDirectTupletUpLink->
              getTupletDirectMeasureUpLink ();

        if (tupletDirectMeasureUpLink) {
          result =
            tupletDirectMeasureUpLink->
              fetchMeasureVoiceUpLink ();
        }
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    /* JMi
      if (fNoteDirectGraceNotesGroupUpLink) {
        result =
          fNoteDirectGraceNotesGroupUpLink->
            getGraceNotesGroupVoiceUpLink ();
              / * JMI
              getGraceNotesGroupNoteUpLink ()->
              fetchNoteVoiceUpLink ();
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

S_msrStaff msrNote::fetchNoteStaffUpLink () const
{
  S_msrStaff result;

  if (fNoteDirectMeasureUpLink) {
    result =
      fNoteDirectMeasureUpLink->
        fetchMeasureStaffUpLink ();
  }

  return result;
}

S_msrPart msrNote::fetchNotePartUpLink () const
{
  S_msrPart result;

  if (fNoteDirectMeasureUpLink) {
    result =
      fNoteDirectMeasureUpLink->
        fetchMeasurePartUpLink ();
  }

  return result;
}

S_msrPartGroup msrNote::fetchNotePartGroupUpLink () const
{
  S_msrPartGroup result;

  if (fNoteDirectMeasureUpLink) {
    result =
      fNoteDirectMeasureUpLink->
        fetchMeasurePartGroupUpLink ();
  }

  return result;
}

S_msrScore msrNote::fetchNoteScoreUpLink () const
{
  S_msrScore result;

  if (fNoteDirectMeasureUpLink) {
    result =
      fNoteDirectMeasureUpLink->
        fetchMeasureScoreUpLink ();
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
      noteKindAsString (noteKind) <<
      '\'' <<
      endl;
  }
#endif

  fNoteKind = noteKind;
}

void msrNote::setNoteAlphaRGBColor (
  msrAlphaRGBColor& noteAlphaRGBColor)
{
  fNoteAlphaRGBColor          = noteAlphaRGBColor;
  fNoteAlphaRGBColorHasBenSet = true;
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
        fMeasureElementMeasureNumber,

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

  // note print kind
  // ------------------------------------------------------

  newbornClone->fNotePrintObjectKind =
    fNotePrintObjectKind;

  // note head
  // ------------------------------------------------------

  newbornClone->fNoteHeadKind =
    fNoteHeadKind;
  newbornClone->fNoteHeadFilledKind =
    fNoteHeadFilledKind;
  newbornClone->fNoteHeadParenthesesKind =
    fNoteHeadParenthesesKind;

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
    fNoteBelongsToAFullBarRests =
      fNoteBelongsToAFullBarRests;

  newbornClone->
    fNoteFullBarRestsSequenceNumber =
      fNoteFullBarRestsSequenceNumber;

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

  newbornClone->fNoteAlphaRGBColor =
    fNoteAlphaRGBColor;
  newbornClone->fNoteAlphaRGBColorHasBenSet =
    fNoteAlphaRGBColorHasBenSet;

  // solo note or rest?
  // ------------------------------------------------------

  newbornClone->fNoteSoloNoteOrRestInVoiceKind =
    fNoteSoloNoteOrRestInVoiceKind;
  newbornClone->fNoteSoloNoteOrRestInStaffKind =
    fNoteSoloNoteOrRestInStaffKind;

  // upLinks
  // ------------------------------------------------------

  /* JMI
    S_msrTuplet           fNoteDirectTupletUpLink;

    S_msrMeasure          fNoteDirectMeasureUpLink;
*/

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
        fMeasureElementMeasureNumber,

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

  // note print kind
  // ------------------------------------------------------

  noteDeepClone->fNotePrintObjectKind =
    fNotePrintObjectKind;

  // note head
  // ------------------------------------------------------

  noteDeepClone->fNoteHeadKind =
    fNoteHeadKind;
  noteDeepClone->fNoteHeadFilledKind =
    fNoteHeadFilledKind;
  noteDeepClone->fNoteHeadParenthesesKind =
    fNoteHeadParenthesesKind;

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
    fNoteBelongsToAFullBarRests =
      fNoteBelongsToAFullBarRests;

  noteDeepClone->
    fNoteFullBarRestsSequenceNumber =
      fNoteFullBarRestsSequenceNumber;

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

  noteDeepClone->fNoteFiguredBassElementsList = // JMI ???
    fNoteFiguredBassElementsList;

  // note measure information
  // ------------------------------------------------------

  noteDeepClone->
    fMeasureElementMeasureNumber =
      fMeasureElementMeasureNumber;
  noteDeepClone->
    fMeasureElementPositionInMeasure =
      fMeasureElementPositionInMeasure;
  noteDeepClone->
    fMeasureElementPositionInVoice =
      fMeasureElementPositionInVoice;

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

  noteDeepClone->fNoteAlphaRGBColor =
    fNoteAlphaRGBColor;
  noteDeepClone->fNoteAlphaRGBColorHasBenSet =
    fNoteAlphaRGBColorHasBenSet;

  // solo note or rest?
  // ------------------------------------------------------

  noteDeepClone->fNoteSoloNoteOrRestInVoiceKind =
    fNoteSoloNoteOrRestInVoiceKind;
  noteDeepClone->fNoteSoloNoteOrRestInStaffKind =
    fNoteSoloNoteOrRestInStaffKind;

  // upLinks
  // ------------------------------------------------------

  /* JMI

    S_msrTuplet           fNoteDirectChordUpLink;

    S_msrGraceNotesGroup  fNoteGraceNoteGroupUpLink;

    S_msrTuplet           fNoteDirectTupletUpLink;

    S_msrMeasure          fNoteDirectMeasureUpLink;
*/

  return noteDeepClone;
}

S_msrNote msrNote::createRestNote (
  int             inputLineNumber,
  const string&   noteMeasureNumber,
  const rational& soundingWholeNotes,
  const rational& displayWholeNotes,
  int             dotsNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      noteMeasureNumber,

      msrNoteKind::kNoteRestInMeasure, // noteKind

      msrQuarterTonesPitchKind::k_NoQuarterTonesPitch,
      msrOctaveKind::k_NoOctave, // noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrDurationKind::k_NoDuration, // noteGraphicDuration

      msrQuarterTonesPitchKind::k_NoQuarterTonesPitch, // noteDisplayQuarterTonesPitch
      msrOctaveKind::k_NoOctave, // noteDisplayOctave,

      msrNote::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
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
  const rational& soundingWholeNotes,
  const rational& displayWholeNotes,
  int             dotsNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      noteMeasureNumber,

      msrNoteKind::kNoteSkipInMeasure, // noteKind

      msrQuarterTonesPitchKind::k_NoQuarterTonesPitch,
      msrOctaveKind::k_NoOctave, // noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrDurationKind::k_NoDuration, // noteGraphicDuration

      msrQuarterTonesPitchKind::k_NoQuarterTonesPitch, // noteDisplayQuarterTonesPitch
      msrOctaveKind::k_NoOctave, // noteDisplayOctave,

      msrNote::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
  assert (o != nullptr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSkipNotes () || gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
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
  const rational& soundingWholeNotes,
  const rational& displayWholeNotes,
  int             dotsNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      noteMeasureNumber,

      msrNoteKind::kNoteSkipInGraceNotesGroup, // noteKind

      msrQuarterTonesPitchKind::k_NoQuarterTonesPitch,
      msrOctaveKind::k_NoOctave, // noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrDurationKind::k_NoDuration, // noteGraphicDuration

      msrQuarterTonesPitchKind::k_NoQuarterTonesPitch, // noteDisplayQuarterTonesPitch
      msrOctaveKind::k_NoOctave, // noteDisplayOctave,

      msrNote::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
  assert (o != nullptr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSkipNotes () || gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Creating skip note " <<
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
  const rational& soundingWholeNotes,
  const rational& displayWholeNotes,
  int             dotsNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      noteMeasureNumber,

      msrNoteKind::kNoteRestInMeasure, // noteKind

      msrQuarterTonesPitchKind::kQTP_Rest,
      noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrDurationKind::k_NoDuration, // noteGraphicDuration

      msrQuarterTonesPitchKind::kQTP_Rest,  // noteQuarterTonesDisplayPitchKind
      msrOctaveKind::k_NoOctave,  // noteDisplayOctaveKind

      msrNote::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
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
  const rational& soundingWholeNotes,
  const rational& displayWholeNotes,
  int             dotsNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      noteMeasureNumber,

      msrNoteKind::kNoteSkipInMeasure, // noteKind

      msrQuarterTonesPitchKind::kQTP_Skip,
      noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrDurationKind::k_NoDuration, // noteGraphicDuration JMI ???

      msrQuarterTonesPitchKind::kQTP_Skip,  // noteQuarterTonesDisplayPitchKind
      noteOctave,

      msrNote::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
  assert (o != nullptr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSkipNotes ()) {
    gLogStream <<
      "Creating skip note " <<
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
  const rational&          soundingWholeNotes,
  const rational&          displayWholeNotes,
  int                      dotsNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
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

      msrNote::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
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
  rational
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
  rational
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
  rational
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
      K_NO_MEASURE_NUMBER, // JMI ???

      msrNoteKind::kNoteRegularInMeasure, // noteKind

      quarterTonesPitchKindFromSemiTonesPitchKind (
        semiTonesPitchAndOctave->
          getSemiTonesPitchKind ()),
      semiTonesPitchAndOctave->
        getOctaveKind (),

      rational (0, 1), // soundingWholeNotes,
      rational (0, 1), // displayWholeNotes,

      0, // dotsNumber,

      msrDurationKind::k_NoDuration, // noteGraphicDuration

      msrQuarterTonesPitchKind::k_NoQuarterTonesPitch, // noteDisplayQuarterTonesPitch
      msrOctaveKind::k_NoOctave, // noteDisplayOctave,

      msrNote::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectNo, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
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

//________________________________________________________________________
void msrNote::setNotePositionInMeasure (
  const rational& positionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting note position in measure of " << asString () <<
      " to '" <<
      positionInMeasure <<
      '\'' <<
      endl;
  }
#endif

//  positionInMeasure.rationalise (); // TEMP ? JMI

  string context =
    "setNotePositionInMeasure()";

  setMeasureElementPositionInMeasure (
    positionInMeasure,
    context);

  if (false) { // JMI
  // compute note's position in voice
  rational
     positionInVoice =
      fNoteDirectMeasureUpLink->getMeasurePositionInVoice ()
        +
      positionInMeasure;
  positionInVoice.rationalise ();

  // set note's position in voice
  setMeasureElementPositionInVoice (
    positionInVoice,
    context);

  // update current position in voice
  S_msrVoice
    voice =
      fNoteDirectMeasureUpLink->
        fetchMeasureVoiceUpLink ();

  voice->
    incrementCurrentPositionInVoice (
      fMeasureElementSoundingWholeNotes);
}

  // are there harmonies attached to this note?
  if (fNoteHarmoniesList.size ()) {
    list<S_msrHarmony>::const_iterator i;
    for (
      i=fNoteHarmoniesList.begin (); i!=fNoteHarmoniesList.end (); ++i
    ) {
      // set the harmony position in measure, taking it's offset into account
      (*i)->
        setHarmonyPositionInMeasure (
          positionInMeasure);
    } // for
  }

  // are there figured bass elements attached to this note?
  if (fNoteFiguredBassElementsList.size ()) {
    list<S_msrFiguredBassElement>::const_iterator i;
    for (
      i=fNoteFiguredBassElementsList.begin (); i!=fNoteFiguredBassElementsList.end (); ++i
    ) {
      // set the figured bass position in measure
      (*i)->
        setFiguredBassPositionInMeasure (
          positionInMeasure);
    } // for
  }

  // are there dal segnos attached to this note?
  if (fNoteDalSegnos.size ()) {
    list<S_msrDalSegno>::const_iterator i;
    for (
      i=fNoteDalSegnos.begin (); i!=fNoteDalSegnos.end (); ++i
    ) {
      // set the dal segno position in measure
      (*i)->
        setDalSegnoPositionInMeasure (
          positionInMeasure);
    } // for
  }
}

void msrNote::setNoteOccupiesAFullMeasure ()
{
  fNoteOccupiesAFullMeasure = true;
}

string msrNote::noteSoundingWholeNotesAsMsrString () const
{
  string result;

  if (fMeasureElementSoundingWholeNotes.getNumerator () == 0)
    result = " (no sounding whole notes)";
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
    result = " (no display whole notes)";
  }
  else {
    result =
      wholeNotesAsMsrString (
        fInputLineNumber,
        fNoteDisplayWholeNotes);
  }

  return result;
}

string msrNote::noteHeadKindAsString (
  msrNoteHeadKind noteHeadKind)
{
  string result;

  switch (noteHeadKind) {
    case msrNote::kNoteHeadSlash:
      result = "kNoteHeadSlash";
      break;
    case msrNote::kNoteHeadTriangle:
      result = "kNoteHeadTriangle";
      break;
    case msrNote::kNoteHeadDiamond:
      result = "kNoteHeadDiamond";
      break;
    case msrNote::kNoteHeadSquare:
      result = "kNoteHeadSquare";
      break;
    case msrNote::kNoteHeadCross:
      result = "kNoteHeadCross";
      break;
    case msrNote::kNoteHeadX:
      result = "kNoteHeadX";
      break;
    case msrNote::kNoteHeadCircleX:
      result = "kNoteHeadCircleX";
      break;
    case msrNote::kNoteHeadInvertedTriangle:
      result = "kNoteHeadInvertedTriangle";
      break;
    case msrNote::kNoteHeadArrowDown:
      result = "kNoteHeadArrowDown";
      break;
    case msrNote::kNoteHeadArrowUp:
      result = "kNoteHeadArrowUp";
      break;
    case msrNote::kNoteHeadSlashed:
      result = "kNoteHeadSlashed";
      break;
    case msrNote::kNoteHeadBackSlashed:
      result = "kNoteHeadBackSlashed";
      break;
    case msrNote::kNoteHeadNormal:
      result = "kNoteHeadNormal";
      break;
    case msrNote::kNoteHeadCluster:
      result = "kNoteHeadCluster";
      break;
    case msrNote::kNoteHeadCircleDot:
      result = "kNoteHeadCircleDot";
      break;
    case msrNote::kNoteHeadLeftTriangle:
      result = "kNoteHeadLeftTriangle";
      break;
    case msrNote::kNoteHeadRectangle:
      result = "kNoteHeadRectangle";
      break;
    case msrNote::kNoteHeadNone:
      result = "kNoteHeadNone";
      break;
    case msrNote::kNoteHeadDo:
      result = "kNoteHeadDo";
      break;
    case msrNote::kNoteHeadRe:
      result = "kNoteHeadRe";
      break;
    case msrNote::kNoteHeadMi:
      result = "kNoteHeadMi";
      break;
    case msrNote::kNoteHeadFa:
      result = "kNoteHeadFa";
      break;
    case msrNote::kNoteHeadFaUp:
      result = "kNoteHeadFaUp";
      break;
    case msrNote::kNoteHeadSo:
      result = "kNoteHeadSo";
      break;
    case msrNote::kNoteHeadLa:
      result = "kNoteHeadLa";
      break;
    case msrNote::kNoteHeadTi:
      result = "kNoteHeadTi";
      break;
  } // switch

  return result;
}

string msrNote::noteHeadFilledKindAsString (
  msrNoteHeadFilledKind noteHeadFilledKind)
{
  string result;

  switch (noteHeadFilledKind) {
    case msrNote::kNoteHeadFilledYes:
      result = "kNoteHeadFilledYes";
      break;
    case msrNote::kNoteHeadFilledNo:
      result = "kNoteHeadFilledNo";
      break;
  } // switch

  return result;
}

string msrNote::noteHeadParenthesesKindAsString (
  msrNoteHeadParenthesesKind noteHeadParenthesesKind)
{
  string result;

  switch (noteHeadParenthesesKind) {
    case msrNote::kNoteHeadParenthesesYes:
      result = "kNoteHeadParenthesesYes";
      break;
    case msrNote::kNoteHeadParenthesesNo:
      result = "kNoteHeadParenthesesNo";
      break;
  } // switch

  return result;
}

string msrNote::noteIsACueNoteKindAsString (
  msrNoteIsACueNoteKind noteIsACueNoteKind)
{
  string result;

  switch (noteIsACueNoteKind) {
    case msrNote::kNoteIsACueNoteYes:
      result = "kNoteIsACueNoteYes";
      break;
    case msrNote::kNoteIsACueNoteNo:
      result = "kNoteIsACueNoteNo";
      break;
  } // switch

  return result;
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
    fetchNoteIsARest () && fNoteDisplayOctaveKind != msrOctaveKind::k_NoOctave;
}

void msrNote::setNoteStem (S_msrStem stem)
{
  // register note stem
  fNoteStem = stem;

  // mark note as stemless if relevant
  if (stem->getStemKind () == msrStem::kStemNeutral)
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
      actualNotes << "/" << normalNotes <<
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  // the display whole notes are known, compute the sounding ones
  fMeasureElementSoundingWholeNotes =
    fNoteDisplayWholeNotes
      *
    normalNotes
      /
    actualNotes;
  fMeasureElementSoundingWholeNotes.rationalise ();

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
    voice = fetchNoteVoiceUpLink ();

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
      spanner->spannerKindAsString () <<
      "' to note " <<
      asString () <<
      endl;
  }
#endif

  // register note has having a wavy line start
  switch (spanner->getSpannerKind ()) {
    case msrSpanner::kSpannerDashes:
      break;

    case msrSpanner::kSpannerWavyLine:
      switch (spanner->getSpannerTypeKind ()) {
        case msrSpannerTypeKind::kSpannerTypeStart:
          fNoteWavyLineSpannerStart = spanner;
          break;
        case msrSpannerTypeKind::kSpannerTypeStop:
          fNoteWavyLineSpannerStop = spanner;
          break;
        case msrSpannerTypeKind::kSpannerTypeContinue:
          break;
        case msrSpannerTypeKind::k_NoSpannerType:
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
    case msrOrnament::kOrnamentTrill:
      fNoteTrillOrnament = ornament;
      break;

/* JMI
    case msrOrnament::kOrnamentDashes:
      fNoteDashesOrnament = ornament;
      break;
*/

    case msrOrnament::kOrnamentDelayedTurn:
      fNoteDelayedTurnOrnament = ornament;
      break;

    case msrOrnament::kOrnamentDelayedInvertedTurn:
      fNoteDelayedInvertedTurnOrnament = ornament;
      break;

    default:
      {}
  } // switch

  // set ornament's note upLink
  ornament->
    setOrnamentNoteUpLink (this);
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
    setGraceNotesGroupNoteUpLink (
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
    setGraceNotesGroupNoteUpLink (
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
      fNoteLigatures.back ()->getLigatureKind () == msrLigature::kLigatureStart
        &&
      ligature->getLigatureKind () == msrLigature::kLigatureStop
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
      fNotePedals.back ()->getPedalTypeKind () == msrPedal::kPedalStart
        &&
      pedal->getPedalTypeKind () == msrPedal::kPedalStop
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

void msrNote::assignMeasureElementPositionInVoice (
  rational&     positionInVoice,
  const string& context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Assigning note position in voice of " <<
      asString () <<
      " to '" << positionInVoice <<
      "' in measure '" <<
      fMeasureElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    positionInVoice != msrMoment::K_NO_POSITION,
    "positionInVoice == msrMoment::K_NO_POSITION");

  // set measure element position in voice
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting note position in voice of " <<
      asString () <<
      " to '" << positionInVoice <<
      "' in measure '" <<
      fMeasureElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  fMeasureElementPositionInVoice = positionInVoice;

  // account for it in positionInVoice
  positionInVoice +=
    fMeasureElementSoundingWholeNotes;
  positionInVoice.rationalise ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Position in voice becomes " <<
      positionInVoice <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif
}

bool msrNote::compareNotesByIncreasingPositionInMeasure (
  const SMARTP<msrNote>& first,
  const SMARTP<msrNote>& second)
{
  return
    first->getMeasureElementPositionInMeasure ()
      <
    second->getMeasureElementPositionInMeasure ();
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

  // update the harmony whole notes if it belongs to a tuplet ??? utf8.xml JMI

  fNoteHarmoniesList.push_back (harmony);

  // register this note as the harmony note upLink
  harmony->
    setHarmonyNoteUpLink (this);
}

void msrNote::appendFiguredBassElementToNoteFiguredBassElementsList (
  S_msrFiguredBassElement figuredBassElement)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceFiguredBass ()) {
    gLogStream <<
      "Append figured bass " <<
      figuredBassElement->asString () <<
      " to the figured bass elements list of " <<
      asString () <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  // update the figured bass whole notes if it belongs to a tuplet ??? utf8.xml

  fNoteFiguredBassElementsList.push_back (figuredBassElement);

  // register this note as the figuredBassElement upLink
  figuredBassElement->
    setFiguredBassNoteUpLink (this);
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
        fetchNoteScoreUpLink ();

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
  if (fNoteFiguredBassElementsList.size ()) {
    list<S_msrFiguredBassElement>::const_iterator i;
    for (
      i=fNoteFiguredBassElementsList.begin (); i!=fNoteFiguredBassElementsList.end (); ++i
    ) {
      // browse the figured bass
      msrBrowser<msrFiguredBassElement> browser (v);
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
        fNoteDirectMeasureUpLink->
          fetchMeasureScoreUpLink ();

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
    case msrNoteKind::k_NoNote:
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
      rational (actualNotes, normalNotes));
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

  s << "[";

  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
      s <<
        "***noNote***";
      break;

    case msrNoteKind::kNoteRestInMeasure:
      s <<
        "kNoteRestInMeasure" <<
        " [" <<
        msrOctaveKindAsString (fNoteOctaveKind) <<
        ", sounding: " <<
        fMeasureElementSoundingWholeNotes <<
        ", displayed: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      s <<
        "kNoteSkipInMeasure: " <<
        ", sounding: " <<
        fMeasureElementSoundingWholeNotes <<
        ", displayed: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      s <<
        "kNoteUnpitchedInMeasure: " <<
        notePitchAsString () <<
        ", sounding: " <<
        fMeasureElementSoundingWholeNotes <<
        ", displayed: " <<
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
        ", sounding: " <<
        fMeasureElementSoundingWholeNotes <<
        ", displayed: " <<
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
        ", sounding: " <<
        fMeasureElementSoundingWholeNotes <<
        ", sounding, " <<
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
        ", sounding: " <<
        fMeasureElementSoundingWholeNotes <<
        ", displayed: " <<
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
        ", sounding: " <<
        fMeasureElementSoundingWholeNotes <<
        ", displayed: " <<
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
        ", sounding: " <<
        fMeasureElementSoundingWholeNotes <<
        ", displayed: " <<
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
        ", sounding: " <<
        fMeasureElementSoundingWholeNotes <<
        ", displayed: " <<
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
        ", sounding: " <<
        fMeasureElementSoundingWholeNotes <<
        ", displayed: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      s <<
        "kNoteUnpitchedInTuplet " <<
        noteGraphicDurationAsMsrString () <<
        ", sounding: " <<
        fMeasureElementSoundingWholeNotes <<
        ", displayed: " <<
        fNoteDisplayWholeNotes;

      s <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;
  } // switch

  s <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

string msrNote::asShortString () const
{
  stringstream s;

  s << "[";

  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
      s <<
        "k_NoNote" <<
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
            fetchNoteVoiceUpLink ();

        s <<
          ", voice: ";
        if (voice) {
          s <<
            voice->getVoiceNumber ();
        }
        else {
          s << "*none*";
        }

        S_msrStaff
          staff;

        if (voice) {
          staff =
            voice->
              getVoiceStaffUpLink ();
        }

        s <<
          ", staff: ";
        if (staff) {
          s <<
            staff->getStaffNumber ();
        }
        else {
          s << "*none*";
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
        ", sounding: " <<
        fMeasureElementSoundingWholeNotes <<
        ", displayed: " <<
        fNoteDisplayWholeNotes <<
        /* JMI
        notePartUpLink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectTupletUpLink->getTupletActualNotes (),
            fNoteDirectTupletUpLink->getTupletNormalNotes ());
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
        ", sounding: " <<
        fMeasureElementSoundingWholeNotes <<
        ", displayed: " <<
        fNoteDisplayWholeNotes;
        /* JMI
        notePartUpLink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectTupletUpLink->getTupletActualNotes (),
            fNoteDirectTupletUpLink->getTupletNormalNotes ());
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
        ", sounding: " <<
        fMeasureElementSoundingWholeNotes <<
        ", displayed: " <<
        fNoteDisplayWholeNotes;
        /* JMI
        notePartUpLink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectTupletUpLink->getTupletActualNotes (),
            fNoteDirectTupletUpLink->getTupletNormalNotes ());
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
        ", sounding: " <<
        fMeasureElementSoundingWholeNotes <<
        ", displayed: " <<
        fNoteDisplayWholeNotes;
        /* JMI
        notePartUpLink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectTupletUpLink->getTupletActualNotes (),
            fNoteDirectTupletUpLink->getTupletNormalNotes ());
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
    "]";

  return s.str ();
}

string msrNote::asMinimalString () const
{
  stringstream s;

  s << "[";

  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
      s <<
        "k_NoNote" <<
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
        ", sounding: " <<
        fMeasureElementSoundingWholeNotes <<
        ", displayed: " <<
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
        ", sounding: " <<
        fMeasureElementSoundingWholeNotes <<
        ", displayed: " <<
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
        ", sounding: " <<
        fMeasureElementSoundingWholeNotes <<
        ", displayed: " <<
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
        ", sounding: " <<
        fMeasureElementSoundingWholeNotes <<
        ", displayed: " <<
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
    "]";

  return s.str ();
}

string msrNote::noteComplementsAsString () const
{
  stringstream s;

  S_msrVoice
    voice =
      fetchNoteVoiceUpLink ();

  s <<
    ", voice: ";
  if (voice) {
    s <<
      voice->getVoiceNumber ();
  }
  else {
    s << "*none*";
  }

  S_msrStaff
    staff;

  if (voice) {
    staff =
      voice->
        getVoiceStaffUpLink ();
  }

  s <<
    ", staff: ";
  if (staff) {
    s <<
      staff->getStaffNumber ();
  }
  else {
    s << "*none*";
  }

  s <<
    ", measureNumber: ";
  if (fMeasureElementMeasureNumber == K_NO_MEASURE_NUMBER) {
    s << "*unknown*";
  }
  else {
    s << fMeasureElementMeasureNumber;
  }

  s <<
    ", measureElementPositionInMeasure " <<
    fMeasureElementPositionInMeasure;
    /* JMI
  if (fMeasureElementPositionInMeasure == msrMoment::K_NO_POSITION_MEASURE_NUMBER) {
    s << "unknown (" << fMeasureElementPositionInMeasure << ")";
  }
  else {
    s << fMeasureElementPositionInMeasure;
  }
*/

  return s.str ();
}

string msrNote::soundingNoteEssentialsAsString () const
{
  stringstream s;

  s <<
    notePitchAsString () <<
    noteSoundingWholeNotesAsMsrString ();

  for (int i = 0; i < fNoteDotsNumber; ++i) {
    s << ".";
  } // for

  s <<
    " (" <<
    msrOctaveKindAsString (fNoteOctaveKind) <<
    ")";

  s <<
    noteComplementsAsString ();

  return s.str ();
}

string msrNote::soundingNoteEssentialsAsStringForMeasuresSlices () const
{
  stringstream s;

  s <<
    notePitchAsString () <<
    noteSoundingWholeNotesAsMsrString ();

  for (int i = 0; i < fNoteDotsNumber; ++i) {
    s << ".";
  } // for

  s <<
    ' ' <<
    msrOctaveKindAsString (fNoteOctaveKind);

  return s.str ();
}

string msrNote::asShortStringForTimeView () const
{
  stringstream s;

  s <<
    "@:" <<
    fMeasureElementPositionInMeasure <<
    ' ' <<
    asShortStringForMeasuresSlices ();

  return s.str ();
}

string msrNote::nonSoundingNoteEssentialsAsString () const
{
  stringstream s;

  s <<
    notePitchAsString () <<
    ' ' << // JMI
    noteDisplayWholeNotesAsMsrString ();

  for (int i = 0; i < fNoteDotsNumber; ++i) {
    s << ".";
  } // for

  s <<
    " (" <<
    msrOctaveKindAsString (fNoteOctaveKind) <<
    ")";

  s <<
    noteComplementsAsString ();

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
    "[=== ";

  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
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
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
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
        notePartUpLink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectTupletUpLink->getTupletActualNotes (),
            fNoteDirectTupletUpLink->getTupletNormalNotes ())
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
        notePartUpLink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectTupletUpLink->getTupletActualNotes (),
            fNoteDirectTupletUpLink->getTupletNormalNotes ())
            */

      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      s <<
        "kNoteUnpitchedInTuplet " <<
        noteSoundingWholeNotesAsMsrString ();
/* JMI
        notePartUpLink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectTupletUpLink->getTupletActualNotes (),
            fNoteDirectTupletUpLink->getTupletNormalNotes ())
            */
      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;
  } // switch
  s << ' ';


/* JMI
  s << left <<
    ", positionInMeasure: ";
    / * JMI
  if (fMeasureElementPositionInMeasure == msrMoment::K_NO_POSITION_MEASURE_NUMBER) {
    s << "unknown (" << fMeasureElementPositionInMeasure << ")";
  }
  else {
    s << fMeasureElementPositionInMeasure;
  }
  * /
  s << fMeasureElementPositionInMeasure;

  s <<
    ", positionInVoice: " <<
    fMeasureElementPositionInVoice;
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
    ", " << noteIsACueNoteKindAsString (fNoteIsACueNoteKind);

  s <<
    ", line " << fInputLineNumber <<
    " ===]";

  return s.str ();
}

string msrNote::asShortStringForMeasuresSlices () const
{
  stringstream s;

  s << "[";

  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
      s <<
        "*noNote*";
      break;

    case msrNoteKind::kNoteRestInMeasure:
//       s <<
//         "kNoteRestInMeasure, ";

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
//         nonSoundingNoteEssentialsAsStringForMeasuresSlices ();
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
        notePartUpLink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectTupletUpLink->getTupletActualNotes (),
            fNoteDirectTupletUpLink->getTupletNormalNotes ())
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
        notePartUpLink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectTupletUpLink->getTupletActualNotes (),
            fNoteDirectTupletUpLink->getTupletNormalNotes ())
            */

      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      s <<
        "kNoteUnpitchedInTuplet " <<
        noteSoundingWholeNotesAsMsrString ();
/* JMI
        notePartUpLink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectTupletUpLink->getTupletActualNotes (),
            fNoteDirectTupletUpLink->getTupletNormalNotes ())
            */
      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;
  } // switch

  s <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrNote::printNoteEssentials (ostream& os) const //JMI superflous
{}

void msrNote::print (ostream& os) const
{
  // print the note itself
  os <<
    asShortString () <<
    endl;

  ++gIndenter;

  const int fieldWidth = 35;

  // print measure number
  os << left <<
    setw (fieldWidth) <<
    "fMeasureElementMeasureNumber" << " : ";
  if (fMeasureElementMeasureNumber == K_NO_MEASURE_NUMBER) {
    os <<
      "unknown";
  }
  else if (fMeasureElementMeasureNumber.size ()) {
    os <<
      fMeasureElementMeasureNumber;
  }
  else {
    stringstream s;

    s <<
      "fMeasureElementMeasureNumber is empty in note " <<
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
    "measureElementPositionInMeasure" << " : ";
    /* JMI
  if (fMeasureElementPositionInMeasure == msrMoment::K_NO_POSITION_MEASURE_NUMBER) {
    os << "unknown (" << fMeasureElementPositionInMeasure << ")";
  }
  else {
    os << fMeasureElementPositionInMeasure;
  }
  */
  os <<
    fMeasureElementPositionInMeasure <<
    endl;

  // print position in voice
  os << left <<
    setw (fieldWidth) << "measureElementPositionInVoice" << " : " <<
    fMeasureElementPositionInVoice <<
    endl <<
    setw (fieldWidth) << "measureElementMomentInVoice" << " : " <<
    endl;
  ++gIndenter;
  os <<
    fMeasureElementMomentInVoice;
  --gIndenter;

  // print note measure uplink
  os <<
    setw (fieldWidth) <<
    "fNoteDirectMeasureUpLink" << " : ";

  if (fNoteDirectMeasureUpLink) {
    os << endl;

    ++gIndenter;

    os <<
      fNoteDirectMeasureUpLink->asShortString () <<
      endl;

    --gIndenter;
  }
  else {
    os <<
      "none";
  }
  os << endl;

  // print note chord uplink
  os <<
    setw (fieldWidth) <<
    "fNoteDirectChordUpLink" << " : ";

  if (fNoteDirectChordUpLink) {
    os <<
      fNoteDirectChordUpLink->asShortString ();
  }
  else {
    os <<
      " : " << "none";
  }
  os << endl;

  // print note grace notes group uplink
  os <<
    setw (fieldWidth) <<
    "fNoteDirectGraceNotesGroupUpLink" << " :";

  if (fNoteDirectGraceNotesGroupUpLink) {
    os << endl;
    ++gIndenter;
    os <<
      fNoteDirectGraceNotesGroupUpLink->asShortString ();
    --gIndenter;
  }
  else {
    os <<
      ' ' << "none";
  }
  os << endl;

  // print note tuplet uplink
  os <<
    setw (fieldWidth) <<
    "fNoteDirectTupletUpLink" << " : ";

  if (fNoteDirectTupletUpLink) {
    os <<
      fNoteDirectTupletUpLink->asShortString ();
  }
  else {
    os <<
      " : " << "none";
  }
  os << endl;

  // print sounding and displayed whole notes
  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
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

        if (fNoteDirectTupletUpLink) {
          os <<
            wholeNotesAsMsrString (
              fInputLineNumber,
              getNoteDirectTupletUpLink ()->
                getTupletSoundingWholeNotes ());
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
  rational
    measureFullLength =
      fNoteDirectMeasureUpLink
        ?
          fNoteDirectMeasureUpLink->
            getFullMeasureWholeNotesDuration ()
        : rational (0, 1); // JMI

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
    "fNoteBelongsToAFullBarRests" << " : " <<
    fNoteBelongsToAFullBarRests <<
    endl <<
    setw (fieldWidth) <<
    "fNoteFullBarRestsSequenceNumber" << " : " <<
    fNoteFullBarRestsSequenceNumber <<
    endl;

  // note print kind
  os << left <<
    setw (fieldWidth) <<
   "fNotePrintObjectKind" << " : " <<
    msrPrintObjectKindAsString (
      fNotePrintObjectKind) <<
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
        "none" <<
        endl;
    }
  }

  // note head
  os << left <<
    setw (fieldWidth) <<
    "fNoteHeadKind" << " : " <<
    noteHeadKindAsString (
      fNoteHeadKind) <<
    endl <<
    setw (fieldWidth) <<
    "fNoteHeadFilledKind" << " : " <<
    noteHeadFilledKindAsString (
      fNoteHeadFilledKind) <<
    endl <<
    setw (fieldWidth) <<
    "fNoteHeadParenthesesKind" << " : " <<
    noteHeadParenthesesKindAsString (
      fNoteHeadParenthesesKind) <<
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
    msrEditorialAccidentalKindAsString (
      fNoteEditorialAccidentalKind) <<
    endl <<
    setw (fieldWidth) <<
    "fNoteCautionaryAccidentalKind" << " : " <<
    msrCautionaryAccidentalKindAsString (
      fNoteCautionaryAccidentalKind) <<
    endl;

  // cue note???
  os << left <<
    setw (fieldWidth) <<
    "fNoteIsACueNoteKind" << " : " <<
    noteIsACueNoteKindAsString (fNoteIsACueNoteKind) <<
    endl;

  // short cuts for efficiency
  os << left <<
    setw (fieldWidth) <<
    "getNoteIsAGraceNote" << " : " <<
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

  if (fNoteTrillOrnament) {
    os <<
      setw (fieldWidth) <<
      "fNoteTrillOrnament" <<
      " : " <<
      fNoteTrillOrnament-> asString () <<
      endl;
  }

  if (fNoteDashesOrnament) {
    os <<
      setw (fieldWidth) <<
      "fNoteDashesOrnament" <<
      " : " <<
      fNoteDashesOrnament-> asString () <<
      endl;
  }

  if (fNoteDelayedTurnOrnament) {
    os <<
      setw (fieldWidth) <<
      "fNoteDelayedTurnOrnament" <<
      " : " <<
      fNoteDelayedTurnOrnament-> asString () <<
      endl;
  }
  if (fNoteDelayedInvertedTurnOrnament) {
    os <<
      setw (fieldWidth) <<
      "fNoteDelayedInvertedTurnOrnament" <<
      " : " <<
      fNoteDelayedInvertedTurnOrnament-> asString () <<
      endl;
  }

  if (fNoteWavyLineSpannerStart) {
    os <<
      setw (fieldWidth) <<
      "fNoteWavyLineSpannerStart" <<
      " : " <<
      fNoteWavyLineSpannerStart->asShortString () <<
      endl;
  }
  if (fNoteWavyLineSpannerStop) {
    os <<
      setw (fieldWidth) <<
      "fNoteWavyLineSpannerStop" <<
      " : " <<
      fNoteWavyLineSpannerStop->asShortString () <<
      endl;
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
    case msrNoteKind::k_NoNote:
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
      /* JMI
      os << left <<
        setw (fieldWidth) <<
        "fNoteTupletNoteGraphicDurationAsMsrString" << " : \"" <<
        fNoteTupletNoteGraphicDurationAsMsrString <<
        "\"" <<
        endl <<
        setw (fieldWidth) <<
        "noteTupletNoteSoundingWholeNotesAsMsrString" << " : ";
          */

      if (fNoteDirectTupletUpLink) {
        os <<
          "\"" <<
          wholeNotesAsMsrString (
            fInputLineNumber,
            getNoteDirectTupletUpLink ()->
              getTupletSoundingWholeNotes ()) <<
          "\"";
      }
      else {
        os <<
          "*** unknown yet ***";
      }
      os << endl;

      os <<
        setw (fieldWidth) <<
        "positionInTuplet" << " : " <<
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
  if (fNoteOctaveShift) {
    os <<
      setw (fieldWidth) <<
      "fNoteOctaveShift";
    if (fNoteOctaveShift) {
      os << endl;

      ++gIndenter;

      os <<
        fNoteOctaveShift;

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the stem if any
  if (fNoteStem) {
    os <<
      setw (fieldWidth) <<
      "fNoteStem";
    if (fNoteStem) {
      os << endl;

      ++gIndenter;

      os <<
        fNoteStem;

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the note color
  os <<
    setw (fieldWidth) <<
    "fNoteAlphaRGBColor" << " : " <<
    fNoteAlphaRGBColor.asString () <<
    endl <<
    setw (fieldWidth) <<
    "fNoteAlphaRGBColorHasBenSet" << " : " <<
    fNoteAlphaRGBColorHasBenSet <<
    endl;

  // solo note or rest?
  os <<
    setw (fieldWidth) <<
    "fNoteSoloNoteOrRestInVoiceKind" << " : " <<
    soloNoteOrRestInVoiceKindAsString (
      fNoteSoloNoteOrRestInVoiceKind) <<
    endl <<
    setw (fieldWidth) <<
    "fNoteSoloNoteOrRestInStaffKind" << " : " <<
    soloNoteOrRestInStaffKindAsString (
      fNoteSoloNoteOrRestInStaffKind) <<
    endl;

  // print the tie if any
  if (fNoteTie) {
    os <<
      setw (fieldWidth) <<
      "fNoteTie";
    if (fNoteTie) {
      os << endl;

      ++gIndenter;

      os <<
        fNoteTie;

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
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
      "none" <<
      endl;
  }

  // print the figured bass elements associated to this note if any
  os <<
    setw (fieldWidth) <<
    "fNoteFiguredBassElementsList";
  if (fNoteFiguredBassElementsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrFiguredBassElement>::const_iterator
      iBegin = fNoteFiguredBassElementsList.begin (),
      iEnd   = fNoteFiguredBassElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrFiguredBassElement
        figuredBassElement = (*i);

      os << figuredBassElement;

      if (++i == iEnd) break;
      // no endl here
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "none" <<
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

/* JMI
      os <<
        syllable->syllableKindAsString () <<
          syllable->syllableExtendKindAsString () <<
        " : ";

      msrSyllable::writeTextsList (
        syllable->getSyllableTextsList (),
        os);

      os <<
        ", stanza " <<
        syllable->
          getSyllableStanzaUpLink ()->
            getStanzaNumber () <<
        ", line " << syllable->getInputLineNumber () <<
        ", noteUpLink: " <<
        syllable->
          getSyllableNoteUpLink ()->
            asShortString ();
*/

      if (++i == iEnd) break;
      // no endl here
    } // for

    --gIndenter;
  }
  else {
    os << " : " <<
      "none" <<
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
      "none" <<
      endl;
  }

  --gIndenter;
}

void msrNote::printShort (ostream& os) const
{
  // print the note itself
  os << asString () << endl;

  ++gIndenter;

  const int fieldWidth = 34;

  // print position in measure
  os << left <<
    setw (fieldWidth) <<
    "measureElementPositionInMeasure" << " : " <<
    endl;

  // print position in voice
  os << left <<
    setw (fieldWidth) << "measureElementPositionInVoice" << " : " <<
    fMeasureElementPositionInVoice <<
    endl;

  // print sounding and displayed whole notes
  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
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

        if (fNoteDirectTupletUpLink) {
          os <<
            wholeNotesAsMsrString (
              fInputLineNumber,
              getNoteDirectTupletUpLink ()->
                getTupletSoundingWholeNotes ());
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
  if (fNoteFiguredBassElementsList.size ()) {
    os <<
      setw (fieldWidth) <<
      "fNoteFiguredBassElementsList";
      os << endl;

    ++gIndenter;

    list<S_msrFiguredBassElement>::const_iterator
      iBegin = fNoteFiguredBassElementsList.begin (),
      iEnd   = fNoteFiguredBassElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrFiguredBassElement
        figuredBassElement = (*i);

      os << figuredBassElement;

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
}

ostream& operator<< (ostream& os, const S_msrNote& elt)
{
  elt->print (os);
  return os;
}


}

/* JMI
// tuplet upLink
S_msrTuplet msrNote::fetchNoteTupletUpLink () const
{
  S_msrTuplet result;

  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
      break;

    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteUnpitchedInMeasure:
      result = fNoteDirectTupletUpLink;
      break;

    case msrNoteKind::kNoteRegularInChord:
      if (fNoteDirectChordUpLink) {
        result =
          fNoteDirectChordUpLink->
            fetchChordTupletUpLink ();
      }
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      if (fNoteDirectTupletUpLink) {
        result =
          fNoteDirectTupletUpLink->
            fetchTupletTupletUpLink ();
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    / * JMi
      if (fNoteDirectGraceNotesGroupUpLink) {
        result =
          fNoteDirectGraceNotesGroupUpLink->
            getGraceNotesGroupVoiceUpLink ();
              / * JMI
              getGraceNotesGroupNoteUpLink ()->
              fetchNoteVoiceUpLink ();
              * /
      }
    * /
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      if (fNoteDirectChordUpLink) {
        result =
          fNoteDirectChordUpLink->
            getChordDirectGraceNotesGroupUpLink ()->
              fetchGraceNoteGroupTupletUpLink ();
      }
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      / * cannot happen ??? JMI
      if (fNoteDirectTupletUpLink) {
        result =
          fNoteDirectTupletUpLink->
            getTupletDirectTupletUpLink ()->
              fetchTupletVoiceUpLink ();
      }
      * /
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      break;
  } // switch

  return result;
}
*/

