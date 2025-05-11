/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <regex>

#include "mfPreprocessorSettings.h"

#include "visitor.h"

#include "mfAssert.h"

#include "mfConstants.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "oahWae.h"
#include "msrWae.h"

#include "msrMeasureConstants.h"

#include "msrArticulations.h"
#include "msrBrowsers.h"
#include "msrGlissandos.h"
#include "msrPitchesNames.h"
#include "msrSlides.h"
#include "msrTechnicals.h"
#include "msrTuplets.h"

#include "oahOah.h"
#include "oahEarlyOptions.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrNote msrNote::create (
  int                        inputLineNumber,
  const S_msrMeasure&        upLinkToMeasure,

//   const std::string&         noteMeasureNumber, // JMI 0.9.66

  msrNoteKind                noteKind,

  msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,
  msrOctaveKind              noteOctaveKind,

  const mfWholeNotes&        noteSoundingWholeNotes,
  const mfWholeNotes&        noteDisplayWholeNotes,

  int                        noteDotsNumber,

  mfDurationKind             noteGraphicNotesDurationKind,

  msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
  msrOctaveKind              noteDisplayOctaveKind,

  msrNoteIsACueNoteKind      msrNoteIsACueNoteKind,

  msrPrintObjectKind         notePrintObjectKind,

  msrNoteHeadKind            noteHeadKind,
  msrNoteHeadFilledKind      msrNoteHeadFilledKind,
  msrNoteHeadParenthesesKind msrNoteHeadParenthesesKind)
{
  msrNote * obj =
    new msrNote (
      inputLineNumber,
      upLinkToMeasure,

//       noteMeasureNumber, // JMI 0.9.66

      noteKind,

      noteQuarterTonesPitchKind,
      noteOctaveKind,

      noteSoundingWholeNotes,
      noteDisplayWholeNotes,

      noteDotsNumber,

      noteGraphicNotesDurationKind,

      noteQuarterTonesDisplayPitchKind,
      noteDisplayOctaveKind,

      msrNoteIsACueNoteKind,

      notePrintObjectKind,

      noteHeadKind,
      msrNoteHeadFilledKind,
      msrNoteHeadParenthesesKind);
  assert (obj != nullptr);
  return obj;
}

msrNote::msrNote (
  int                        inputLineNumber,
  const S_msrMeasure&        upLinkToMeasure,

//   const std::string&         noteMeasureNumber, // JMI 0.9.66

  msrNoteKind                noteKind,

  msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,
  msrOctaveKind              noteOctaveKind,

  const mfWholeNotes&        noteSoundingWholeNotes,
  const mfWholeNotes&        noteDisplayWholeNotes,

  int                        noteDotsNumber,

  mfDurationKind             noteGraphicNotesDurationKind,

  msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
  msrOctaveKind              noteDisplayOctaveKind,

  msrNoteIsACueNoteKind      noteIsACueNoteKind,

  msrPrintObjectKind         notePrintObjectKind,

  msrNoteHeadKind            noteHeadKind,
  msrNoteHeadFilledKind      msrNoteHeadFilledKind,
  msrNoteHeadParenthesesKind msrNoteHeadParenthesesKind)
    : msrTupletElement (
        inputLineNumber),
      fNoteColorAlphaRGB ("", "")
{
  fMeasureElementUpLinkToMeasure = upLinkToMeasure;

//   fMeasureElementMeasureNumber = noteMeasureNumber; // JMI ??? 0.9.66

// 	if (noteKind == msrNoteKind::kNoteSkipInMeasure) abort (); // JMI 0.9.72

  // basic note description
  fNoteKind = noteKind;

  fNoteQuarterTonesPitchKind = noteQuarterTonesPitchKind;
  fNoteOctaveKind = noteOctaveKind;

  fNoteDotsNumber = noteDotsNumber;

  fNoteGraphicNotesDurationKind = noteGraphicNotesDurationKind;

  fNoteTupletFactor = mfRational (1, 1);

  fNoteQuarterTonesDisplayPitchKind = noteQuarterTonesDisplayPitchKind;
  fNoteDisplayOctaveKind = noteDisplayOctaveKind;

  fNoteIsACueNoteKind = noteIsACueNoteKind;

  fNotePrintObjectKind = notePrintObjectKind;

  fNoteHeadKind = noteHeadKind;
  fNoteHeadFilledKind = msrNoteHeadFilledKind;
  fNoteHeadParenthesesKind = msrNoteHeadParenthesesKind;

  fNoteDisplayWholeNotes = noteDisplayWholeNotes;

  // only now, to be able to display the note as a string
  setMeasureElementSoundingWholeNotes (
    noteSoundingWholeNotes,
    "msrNote::msrNote()");

  // do other initializations
  initializeNote ();
}

void msrNote::initializeNote ()
{
  // rests handling
  // ------------------------------------------------------

  if (fetchNoteIsARest () && fNoteDisplayOctaveKind != msrOctaveKind::kOctave_UNKNOWN_) {
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

  fNoteMultipleMeasureRestsSequenceNumber = -1;

//   fNoteColorAlphaRGBHasBenSet = false; JMI 0.9.64

  // note lyrics
  // ------------------------------------------------------

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesDetails ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "Initializing a note" <<
      ", kind: ";
		ss <<
			fNoteKind <<
      ", line " << fInputLineNumber << ":" <<
      std::endl;

    ++gIndenter;

    constexpr int fieldWidth = 30;

    ss <<
      std::left <<
        std::setw (fieldWidth) <<
        "fNoteQuarterTonesPitchKind" << ": " <<
        msrQuarterTonesPitchKindAsStringInLanguage (
          fNoteQuarterTonesPitchKind,
          gMsrOahGroup->
            getMsrQuarterTonesPitchesLanguageKind ()) <<
        std::endl;

    ss <<
      std::left <<
        std::setw (fieldWidth) <<
        "fMeasureElementSoundingWholeNotes" << ": " <<
        fMeasureElementSoundingWholeNotes.asFractionString () <<
        std::endl <<
      std::left <<
        std::setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << ": " <<
        fNoteDisplayWholeNotes.asFractionString () <<
        std::endl;

    ss <<
      std::left <<
        std::setw (fieldWidth) <<
        "fNoteOctaveKind" << ": " <<
        fNoteOctaveKind <<
        std::endl <<
      std::left <<
        std::setw (fieldWidth) <<
        "fNoteDisplayOctaveKind" << ": " <<
        fNoteDisplayOctaveKind <<
        std::endl;

    ss <<
      std::left <<
        std::setw (fieldWidth) <<
        "fNoteDotsNumber" << ": " <<
        fNoteDotsNumber <<
        std::endl <<

      std::left <<
        std::setw (fieldWidth) <<
        "fNoteGraphicNotesDurationKind" << ": " <<
        fNoteGraphicNotesDurationKind <<
        std::endl;

    ss <<
      std::left <<
        std::setw (fieldWidth) <<
        "fNoteTupletFactor" << ": " <<
        fNoteTupletFactor <<
        std::endl;

    ss <<
      std::left <<
        std::setw (fieldWidth) <<
        "fNoteIsACueNoteKind" << ": " <<
        fNoteIsACueNoteKind <<
        std::endl <<

      std::left <<
        std::setw (fieldWidth) <<
        "fNotePrintObjectKind" << ": " <<
         fNotePrintObjectKind <<
        std::endl <<

      std::left <<
        std::setw (fieldWidth) <<
        "fNoteHeadKind" << ": " <<
         fNoteHeadKind <<
        std::endl <<
      std::left <<
        std::setw (fieldWidth) <<
        "fNoteHeadFilledKind" << ": " <<
         fNoteHeadFilledKind <<
        std::endl <<
      std::left <<
        std::setw (fieldWidth) <<
        "fNoteHeadParenthesesKind" << ": " <<
         fNoteHeadParenthesesKind <<
        std::endl <<

      std::left <<
        std::setw (fieldWidth) <<
        "fNoteAccidentalKind" << ": " <<
        fNoteAccidentalKind <<
        std::endl <<
      std::left <<
        std::setw (fieldWidth) <<
        "fNoteEditorialAccidentalKind" << ": " <<
        fNoteEditorialAccidentalKind <<
        std::endl <<
      std::left <<
        std::setw (fieldWidth) <<
        "fNoteCautionaryAccidentalKind" << ": " <<
        fNoteCautionaryAccidentalKind <<
        std::endl <<

      std::left <<
        std::setw (fieldWidth) <<
        "fNoteBelongsToAChord" << ": " <<
         fNoteBelongsToAChord <<
        std::endl <<

      std::left <<
        std::setw (fieldWidth) <<
        "fNoteBelongsToATuplet" << ": " <<
         fNoteBelongsToATuplet <<
        std::endl <<

      std::left <<
        std::setw (fieldWidth) <<
        "fNoteOccupiesAFullMeasure" << ": " <<
         fNoteOccupiesAFullMeasure <<
        std::endl;

		ss << std::endl;

    --gIndenter;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // solo note or rest?
  // ------------------------------------------------------

  fNoteSoloNoteOrRestInVoiceKind =
    msrSoloNoteOrRestInVoiceKind::kSoloNoteOrRestInVoiceNo;

  fNoteSoloNoteOrRestInStaffKind =
    msrSoloNoteOrRestInStaffKind::kSoloNoteOrRestInStaffNo;
}

msrNote::~msrNote ()
{}

// uplink to tuplet
void msrNote::setNoteShortcutUpLinkToTuplet (
	const S_msrTuplet& tuplet)
{
	fNoteShortcutUpLinkToTuplet = tuplet;
}

S_msrTuplet msrNote::getNoteShortcutUpLinkToTuplet () const
{
	return fNoteShortcutUpLinkToTuplet;
}

// uplink to grace notes group
S_msrGraceNotesGroup msrNote::fetchNoteUpLinkToGraceNotesGroup () const
{
  S_msrGraceNotesGroup result;

  switch (fNoteKind) {
    case msrNoteKind::kNote_UNKNOWN_:
      break;

    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteRegularInChord:
      result = fNoteShortcutUpLinkToGraceNotesGroup;
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      if (fNoteShortcutUpLinkToTuplet) {
      /* JMI
        result =
          fNoteShortcutUpLinkToTuplet->
            fMeasureElementUpLinkToMeasure ()->
              fetchMeasureUpLinkToVoice ();
      */
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteRestInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    /* JMi
      if (fNoteShortcutUpLinkToGraceNotesGroup) {
        result =
          fNoteShortcutUpLinkToGraceNotesGroup->
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "--> fetchNoteUpLinkToVoice() for note: " <<
//       asMinimalString ();
      asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  switch (fNoteKind) {
    case msrNoteKind::kNote_UNKNOWN_:
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
      if (fNoteShortcutUpLinkToTuplet) {
				S_msrMeasure
					tupletShortcutUpLinkToMeasure =
						fNoteShortcutUpLinkToTuplet->
							getMeasureElementUpLinkToMeasure ();

        if (tupletShortcutUpLinkToMeasure) {
          result =
            tupletShortcutUpLinkToMeasure->
              fetchMeasureUpLinkToVoice ();
        }
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteRestInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    /* JMi
      if (fNoteShortcutUpLinkToGraceNotesGroup) {
        result =
          fNoteShortcutUpLinkToGraceNotesGroup->
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

S_msrScore msrNote::fetchNoteUpLinkToScore () const
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Setting the kind of note " <<
      asString () <<
      " to " <<
      noteKind;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNoteKind = noteKind;
}

void msrNote::setNoteColorAlphaRGB (
  msrColorAlphaRGB& noteColorAlphaRGB)
{
  fNoteColorAlphaRGB = noteColorAlphaRGB;
  fNoteColorAlphaRGBHasBenSet = true;
}

S_msrNote msrNote::createNoteNewbornClone (
  const S_msrPart& containingPart)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of note " <<
      asString () <<
      " in part " <<
      containingPart->
        fetchPartNameForTrace ();

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
    "containingPart is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrNote
    newbornClone =
      msrNote::create (
        fInputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()

//         fMeasureElementUpLinkToMeasure->getMeasureNumber (), // JMI 0.9.66

        fNoteKind,

        fNoteQuarterTonesPitchKind,
        fNoteOctaveKind,

        fMeasureElementSoundingWholeNotes,
        fNoteDisplayWholeNotes,

        fNoteDotsNumber,

        fNoteGraphicNotesDurationKind,

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
    fNoteBelongsToAMultipleMeasureRests =
      fNoteBelongsToAMultipleMeasureRests;

  newbornClone->
    fNoteMultipleMeasureRestsSequenceNumber =
      fNoteMultipleMeasureRestsSequenceNumber;

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

  // ties
  // ------------------------------------------------------

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
  const S_msrVoice& containingVoice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a deep clone of note " <<
      asString () <<
      /* JMI
      " in part " <<
      containingPart->
        fetchPartNameForTrace () <<
        */
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED
*/

  S_msrNote
    deepClone =
      msrNote::create (
        fInputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()

//         fMeasureElementUpLinkToMeasure->getMeasureNumber (), // 0.9.66

        fNoteKind,

        fNoteQuarterTonesPitchKind,
        fNoteOctaveKind,

        fMeasureElementSoundingWholeNotes,
        fNoteDisplayWholeNotes,

        fNoteDotsNumber,

        fNoteGraphicNotesDurationKind,

        fNoteQuarterTonesDisplayPitchKind,
        fNoteDisplayOctaveKind,

        fNoteIsACueNoteKind,

        fNotePrintObjectKind,

        fNoteHeadKind,
        fNoteHeadFilledKind,
        fNoteHeadParenthesesKind);

  // basic note description
  // ------------------------------------------------------

  deepClone->fNoteOctaveShift = // JMI
    fNoteOctaveShift;

  // accidentals
  // ------------------------------------------------------

  deepClone->fNoteAccidentalKind =
    fNoteAccidentalKind;

  deepClone->fNoteEditorialAccidentalKind =
    fNoteEditorialAccidentalKind;

  deepClone->fNoteCautionaryAccidentalKind =
    fNoteCautionaryAccidentalKind;

  // chord member?
  // ------------------------------------------------------

  deepClone->
    fNoteBelongsToAChord =
      fNoteBelongsToAChord;

  // tuplet member?
  // ------------------------------------------------------

  deepClone->
    fNoteBelongsToATuplet =
      fNoteBelongsToATuplet;

  // note measure information
  // ------------------------------------------------------

  deepClone->
    fNoteOccupiesAFullMeasure =
      fNoteOccupiesAFullMeasure;

  // multiple rest member?
  // ------------------------------------------------------

  deepClone->
    fNoteBelongsToAMultipleMeasureRests =
      fNoteBelongsToAMultipleMeasureRests;

  deepClone->
    fNoteMultipleMeasureRestsSequenceNumber =
      fNoteMultipleMeasureRestsSequenceNumber;

  // note lyrics
  // ------------------------------------------------------

/* JMI
  {
    std::list <S_msrSyllable>::const_iterator i;
    for (i = fNoteSyllablesList.begin (); i != fNoteSyllablesList.end (); ++i) {
      // share this data
      deepClone->
        fNoteSyllablesList.push_back ((*i));
    } // for
  }
*/

  // stem
  // ------------------------------------------------------

  deepClone->fNoteStem = // JMI
    // share this data
    fNoteStem;

  // beams
  // ------------------------------------------------------

  {
    std::list <S_msrBeam>::const_iterator i;
    for (i = fNoteBeamsList.begin (); i != fNoteBeamsList.end (); ++i) {
      // share this data
      deepClone->
        fNoteBeamsList.push_back ((*i));
    } // for
  }

  // articulations
  // ------------------------------------------------------

  for (
    std::list <S_msrArticulation>::const_iterator i =
      fNoteArticulationsList.begin ();
      i != fNoteArticulationsList.end ();
      ++i
  ) {
    // share this data
    deepClone->
      fNoteArticulationsList.push_back ((*i));
  } // for

  // spanners
  // ------------------------------------------------------

  for (
    std::list <S_msrSpanner>::const_iterator i =
      fNoteSpannersList.begin ();
      i != fNoteSpannersList.end ();
      ++i
  ) {
    // share this data
    deepClone->
      fNoteSpannersList.push_back ((*i));
  } // for

  // technicals
  // ------------------------------------------------------

  {
    std::list <S_msrTechnical>::const_iterator i;
    for (
      i = fNoteTechnicalsList.begin ();
      i != fNoteTechnicalsList.end ();
      ++i
  ) {
      // share this data
      deepClone->
        fNoteTechnicalsList.push_back ((*i));
    } // for
  }

  {
    std::list <S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i = fNoteTechnicalWithIntegersList.begin ();
      i != fNoteTechnicalWithIntegersList.end ();
      ++i
  ) {
      // share this data
      deepClone->
        fNoteTechnicalWithIntegersList.push_back ((*i));
    } // for
  }

  {
    std::list <S_msrTechnicalWithString>::const_iterator i;
    for (
      i = fNoteTechnicalWithStringsList.begin ();
      i != fNoteTechnicalWithStringsList.end ();
      ++i
  ) {
      // share this data
      deepClone->
        fNoteTechnicalWithStringsList.push_back ((*i));
    } // for
  }

  // ornaments
  // ------------------------------------------------------

  {
    std::list <S_msrOrnament>::const_iterator i;
    for (i = fNoteOrnamentsList.begin (); i != fNoteOrnamentsList.end (); ++i) {
      // share this data
      deepClone->
        fNoteOrnamentsList.push_back ((*i));
    } // for
  }

  // glissandos
  // ------------------------------------------------------

  {
    std::list <S_msrGlissando>::const_iterator i;
    for (i = fNoteGlissandosList.begin (); i != fNoteGlissandosList.end (); ++i) {
      // share this data
      deepClone->
        fNoteGlissandosList.push_back ((*i));
    } // for
  }

  // slides
  // ------------------------------------------------------

  {
    std::list <S_msrSlide>::const_iterator i;
    for (i = fNoteSlidesList.begin (); i != fNoteSlidesList.end (); ++i) {
      // share this data
      deepClone->
        fNoteSlidesList.push_back ((*i));
    } // for
  }

  // grace notes
  // ------------------------------------------------------

  deepClone->fGraceNotesGroupBeforeNote =
    fGraceNotesGroupBeforeNote;
  deepClone->fGraceNotesGroupAfterNote =
    fGraceNotesGroupAfterNote;

/* JMI
  // after grace notes
  // ------------------------------------------------------

  deepClone->fNoteAfterGraceNotesGroup =
    fNoteAfterGraceNotesGroup;
*/

  // single tremolo
  // ------------------------------------------------------

  deepClone->fNoteSingleTremolo =
    fNoteSingleTremolo;

  // ties
  // ------------------------------------------------------

  deepClone->fNoteTiesList = // JMI
    // share this data
    fNoteTiesList;

  // dynamics
  // ------------------------------------------------------

  {
    std::list <S_msrDynamic>::const_iterator i;
    for (i = fNoteDynamicsList.begin (); i != fNoteDynamicsList.end (); ++i) {
      // share this data
      deepClone->
        fNoteDynamicsList.push_back ((*i));
    } // for
  }

  {
    std::list <S_msrOtherDynamic>::const_iterator i;
    for (i = fNoteOtherDynamicsList.begin (); i != fNoteOtherDynamicsList.end (); ++i) {
      // share this data
      deepClone->
        fNoteOtherDynamicsList.push_back ((*i));
    } // for
  }

  // slashes
  // ------------------------------------------------------

  {
    std::list <S_msrSlash>::const_iterator i;
    for (i = fNoteSlashesList.begin (); i != fNoteSlashesList.end (); ++i) {
      // share this data
      deepClone->
        fNoteSlashesList.push_back ((*i));
    } // for
  }

  // cresc/decresc
  // ------------------------------------------------------

  {
    for (S_msrCrescDecresc crescDecresc : fNoteCrescDecrescsList) {
      // share this data
      deepClone->
        fNoteCrescDecrescsList.push_back (crescDecresc);
    } // for
  }

  // wedges
  // ------------------------------------------------------

  {
    std::list <S_msrWedge>::const_iterator i;
    for (i = fNoteWedgesList.begin (); i != fNoteWedgesList.end (); ++i) {
      // share this data
      deepClone->
        fNoteWedgesList.push_back ((*i));
    } // for
  }

  // eyeglasses
  // ------------------------------------------------------

  {
    std::list <S_msrEyeGlasses>::const_iterator i;
    for (i = fNoteEyeGlassesList.begin (); i != fNoteEyeGlassesList.end (); ++i) {
      // share this data
      deepClone->
        fNoteEyeGlassesList.push_back ((*i));
    } // for
  }

  // damps
  // ------------------------------------------------------

  {
    std::list <S_msrDamp>::const_iterator i;
    for (i = fNoteDampsList.begin (); i != fNoteDampsList.end (); ++i) {
      // share this data
      deepClone->
        fNoteDampsList.push_back ((*i));
    } // for
  }

  // damp alls
  // ------------------------------------------------------

  {
    std::list <S_msrDampAll>::const_iterator i;
    for (i = fNoteDampAllsList.begin (); i != fNoteDampAllsList.end (); ++i) {
      // share this data
      deepClone->
        fNoteDampAllsList.push_back ((*i));
    } // for
  }

  // scordaturas
  // ------------------------------------------------------

  {
    std::list <S_msrScordatura>::const_iterator i;
    for (i = fNoteScordaturasList.begin (); i != fNoteScordaturasList.end (); ++i) {
      // share this data
      deepClone->
        fNoteScordaturasList.push_back ((*i));
    } // for
  }

  // words
  // ------------------------------------------------------

  {
    std::list <S_msrWords>::const_iterator i;
    for (i = fNoteWordsList.begin (); i != fNoteWordsList.end (); ++i) {
      // share this data
      deepClone->
        fNoteWordsList.push_back ((*i));
    } // for
  }

  // slurs
  // ------------------------------------------------------

  {
    for (S_msrSlur slur : fNoteSlursList) {
      // share this data
      deepClone->
        fNoteSlursList.push_back (slur);
    } // for
  }

  // ligatures
  // ------------------------------------------------------

  {
    std::list <S_msrLigature>::const_iterator i;
    for (i = fNoteLigaturesList.begin (); i != fNoteLigaturesList.end (); ++i) {
      // share this data
      deepClone->
        fNoteLigaturesList.push_back ((*i));
    } // for
  }

  // pedals
  // ------------------------------------------------------

  {
    std::list <S_msrPedal>::const_iterator i;
    for (i = fNotePedalsList.begin (); i != fNotePedalsList.end (); ++i) {
      // share this data
      deepClone->
        fNotePedalsList.push_back ((*i));
    } // for
  }

  // harmonies
  // ------------------------------------------------------

  deepClone->fNoteHarmoniesList = // JMI ???
    fNoteHarmoniesList;

  // figured bass elements
  // ------------------------------------------------------

  deepClone->fNoteFiguredBassesList = // JMI ???
    fNoteFiguredBassesList;

  // note measure information
  // ------------------------------------------------------

  deepClone->
    fMeasureElementUpLinkToMeasure->getMeasureNumber () =
      fMeasureElementUpLinkToMeasure->getMeasureNumber ();
  deepClone->
    fMeasureElementPositionInMeasure =
      fMeasureElementPositionInMeasure;
  deepClone->
    fMeasureElementVoicePosition =
      fMeasureElementVoicePosition;

  // note redundant information (for speed)
  // ------------------------------------------------------

  deepClone->fNoteIsStemless =
    fNoteIsStemless;

  deepClone->fNoteIsAChordsFirstMemberNote =
    fNoteIsAChordsFirstMemberNote;

  deepClone->fNoteIsFirstNoteInADoubleTremolo =
    fNoteIsFirstNoteInADoubleTremolo;
  deepClone->fNoteIsSecondNoteInADoubleTremolo =
    fNoteIsSecondNoteInADoubleTremolo;

  deepClone->fNoteTrillOrnament =
    fNoteTrillOrnament;
  deepClone->fNoteIsFollowedByGraceNotesGroup =
    fNoteIsFollowedByGraceNotesGroup;

  deepClone->fNoteDashesOrnament =
    fNoteDashesOrnament;

  deepClone->fNoteWavyLineSpannerStart =
    fNoteWavyLineSpannerStart;
  deepClone->fNoteWavyLineSpannerStop =
    fNoteWavyLineSpannerStop;

  deepClone->fNoteDelayedTurnOrnament =
    fNoteDelayedTurnOrnament;
  deepClone->fNoteDelayedInvertedTurnOrnament =
    fNoteDelayedInvertedTurnOrnament;

  // note color
  // ------------------------------------------------------

  deepClone->fNoteColorAlphaRGB =
    fNoteColorAlphaRGB;
  deepClone->fNoteColorAlphaRGBHasBenSet =
    fNoteColorAlphaRGBHasBenSet;

  // solo note or rest?
  // ------------------------------------------------------

  deepClone->fNoteSoloNoteOrRestInVoiceKind =
    fNoteSoloNoteOrRestInVoiceKind;
  deepClone->fNoteSoloNoteOrRestInStaffKind =
    fNoteSoloNoteOrRestInStaffKind;

  return deepClone;
}

S_msrNote msrNote::createRestNote (
	int                 inputLineNumber,
	const std::string&  noteMeasureNumber,
	const mfWholeNotes& soundingWholeNotes,
	const mfWholeNotes& displayWholeNotes,
	int                 dotsNumber)
{
  msrNote * obj =
    new msrNote (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()

//       noteMeasureNumber, // JMI 0.9.66

      msrNoteKind::kNoteRestInMeasure, // noteKind

      msrQuarterTonesPitchKind::kQTP_Rest,
      msrOctaveKind::kOctave_UNKNOWN_, // noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      mfDurationKind::kDuration_UNKNOWN_, // noteGraphicNotesDuration

      msrQuarterTonesPitchKind::kQTP_UNKNOWN_, // noteDisplayQuarterTonesPitch
      msrOctaveKind::kOctave_UNKNOWN_, // noteDisplayOctave,

      msrNoteIsACueNoteKind::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // default value

      msrNoteHeadKind::kNoteHeadNormal, // JMI
      msrNoteHeadFilledKind::kNoteHeadFilledYes, // JMI
      msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo); // JMI
  assert (obj != nullptr);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRestNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating rest note " <<
      obj->asShortString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return obj;
}

S_msrNote msrNote::createSkipNote (
	int                 inputLineNumber,
	const std::string&  noteMeasureNumber,
	const mfWholeNotes& soundingWholeNotes,
	const mfWholeNotes& displayWholeNotes,
	int                 dotsNumber)
{
  msrNote * obj =
    new msrNote (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()

      // noteMeasureNumber, // JMI 0.9.66

      msrNoteKind::kNoteSkipInMeasure, // noteKind

      msrQuarterTonesPitchKind::kQTP_Skip,
      msrOctaveKind::kOctave_UNKNOWN_, // noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      mfDurationKind::kDuration_UNKNOWN_, // noteGraphicNotesDuration

      msrQuarterTonesPitchKind::kQTP_UNKNOWN_, // noteDisplayQuarterTonesPitch
      msrOctaveKind::kOctave_UNKNOWN_, // noteDisplayOctave,

      msrNoteIsACueNoteKind::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // default value

      msrNoteHeadKind::kNoteHeadNormal, // JMI
      msrNoteHeadFilledKind::kNoteHeadFilledYes, // JMI
      msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo); // JMI
  assert (obj != nullptr);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSkipNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating skip note " <<
      obj->asString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return obj;
}

S_msrNote msrNote::createGraceSkipNote (
	int                 inputLineNumber,
	const std::string&  noteMeasureNumber,
	const mfWholeNotes& soundingWholeNotes,
	const mfWholeNotes& displayWholeNotes,
	int                 dotsNumber)
{
  msrNote * obj =
    new msrNote (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure() JMI 0.9.67

//       noteMeasureNumber, // JMI 0.9.66

      msrNoteKind::kNoteSkipInGraceNotesGroup,

      msrQuarterTonesPitchKind::kQTP_Skip,
      msrOctaveKind::kOctave_UNKNOWN_,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      mfDurationKind::kDuration_UNKNOWN_, // noteGraphicNotesDuration

      msrQuarterTonesPitchKind::kQTP_UNKNOWN_, // noteDisplayQuarterTonesPitch
      msrOctaveKind::kOctave_UNKNOWN_, // noteDisplayOctave,

      msrNoteIsACueNoteKind::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // default value

      msrNoteHeadKind::kNoteHeadNormal, // JMI
      msrNoteHeadFilledKind::kNoteHeadFilledYes, // JMI
      msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo); // JMI
  assert (obj != nullptr);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSkipNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating grace skip note " <<
      obj->asString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return obj;
}

//________________________________________________________________________
S_msrNote msrNote::createRestNoteWithOctave (
	int                 inputLineNumber,
	const std::string&  noteMeasureNumber,
	msrOctaveKind       noteOctave,
	const mfWholeNotes& soundingWholeNotes,
	const mfWholeNotes& displayWholeNotes,
	int                 dotsNumber)
{
  msrNote * obj =
    new msrNote (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()

//       noteMeasureNumber, // JMI 0.9.66

      msrNoteKind::kNoteRestInMeasure, // noteKind

      msrQuarterTonesPitchKind::kQTP_Rest,
      noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      mfDurationKind::kDuration_UNKNOWN_, // noteGraphicNotesDuration

      msrQuarterTonesPitchKind::kQTP_Rest,  // noteQuarterTonesDisplayPitchKind
      msrOctaveKind::kOctave_UNKNOWN_,  // noteDisplayOctaveKind

      msrNoteIsACueNoteKind::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // default value

      msrNoteHeadKind::kNoteHeadNormal, // JMI
      msrNoteHeadFilledKind::kNoteHeadFilledYes, // JMI
      msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo); // JMI
  assert (obj != nullptr);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceRestNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating rest note " <<
      obj->asShortString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return obj;
}

//________________________________________________________________________
S_msrNote msrNote::createSkipNoteWithOctave (
	int                 inputLineNumber,
	const std::string&  noteMeasureNumber,
	msrOctaveKind       noteOctave,
	const mfWholeNotes& soundingWholeNotes,
	const mfWholeNotes& displayWholeNotes,
	int                 dotsNumber)
{
  msrNote * obj =
    new msrNote (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()

//       noteMeasureNumber, // JMI 0.9.66

      msrNoteKind::kNoteSkipInMeasure, // noteKind

      msrQuarterTonesPitchKind::kQTP_Skip,
      noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      mfDurationKind::kDuration_UNKNOWN_, // noteGraphicNotesDuration JMI ???

      msrQuarterTonesPitchKind::kQTP_Skip,  // noteQuarterTonesDisplayPitchKind
      noteOctave,

      msrNoteIsACueNoteKind::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // default value

      msrNoteHeadKind::kNoteHeadNormal, // JMI
      msrNoteHeadFilledKind::kNoteHeadFilledYes, // JMI
      msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo); // JMI
  assert (obj != nullptr);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSkipNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating skip note with octave " <<
      obj->asShortString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return obj;
}

//________________________________________________________________________
S_msrNote msrNote::createRegularNote (
  int                      inputLineNumber,
  const std::string&       noteMeasureNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  msrOctaveKind            noteOctaveKind,
  const mfWholeNotes&      soundingWholeNotes,
  const mfWholeNotes&      displayWholeNotes,
  int                      dotsNumber)
{
  msrNote * obj =
    new msrNote (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()

//       noteMeasureNumber, // JMI 0.9.66

      msrNoteKind::kNoteRegularInMeasure, // noteKind

      quarterTonesPitchKind,
      noteOctaveKind,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      wholeNotesAsNotesDurationKind (displayWholeNotes), // JMI caution for tuplet members...

      quarterTonesPitchKind,
      noteOctaveKind, // JMI ???

      msrNoteIsACueNoteKind::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // default value

      msrNoteHeadKind::kNoteHeadNormal, // JMI
      msrNoteHeadFilledKind::kNoteHeadFilledYes, // JMI
      msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo); // JMI
  assert (obj != nullptr);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating regular note " <<
      obj->asShortString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return obj;
}

//________________________________________________________________________
S_msrNote msrNote::createRestFromString (
  int                inputLineNumber,
  const std::string& restString,
  const std::string& restMeasureNumber)
{
  // handling restString à la LilyPond, such as "r4.."

  S_msrNote result;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating rest from string \"" <<
      restString <<
      "\", restMeasureNumber: '" <<
      restMeasureNumber <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string regularExpression (
    "[[:space:]]*"
    "r" //
    "[[:space:]]*"
    "([[:digit:]]+)" // restNotesDuration
    "[[:space:]]*"
    "(\\.*)"         // restDots
    "[[:space:]]*"
    );

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "regularExpression: " <<
      regularExpression;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (restString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for rest string \"" << restString <<
      "\" with std::regex \"" << regularExpression <<
      "\":" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      ss <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    ss << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  // handling restString à la LilyPond, such as "r4.."

  if (smSize != 3) {
    std::stringstream ss;

    ss <<
      "restString \"" << restString <<
      "\" is ill-formed";

    oahError (ss.str ());
  }

  std::string
    restNotesDuration = sm [1],
    restDots = sm [2];

  size_t dotsNumber = restDots.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "restNotesDuration: \"" <<
      restNotesDuration <<
      "\"" <<
      std::endl <<

      "restDots: \"" <<
      restDots <<
      "\"" <<
      std::endl <<
      "dotsNumber: " <<
      dotsNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // compute the restNotesDurationKind from restNotesDuration
  mfDurationKind
    restNotesDurationKind =
      mfDurationKindFromString (
        inputLineNumber,
        restNotesDuration);

  // compute the duration whole notes from restNotesDurationKind
  mfWholeNotes
     notesDurationKindFromMslpString =
       mfDurationKindAsWholeNotes (
         restNotesDurationKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "notesDurationKindFromMslpString: " <<
      notesDurationKindFromMslpString;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  result =
    msrNote::createRestNote (
      inputLineNumber,
      restMeasureNumber,
      notesDurationKindFromMslpString, // soundingWholeNotes
      notesDurationKindFromMslpString, // displayWholeNotes
      dotsNumber);

  return result;
}

//________________________________________________________________________
S_msrNote msrNote::createSkipFromString (
  int                inputLineNumber,
  const std::string& skipString,
  const std::string& skipMeasureNumber)
{
  // handling skipString à la LilyPond, such as "s4.."

  S_msrNote result;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating skip from string \"" <<
      skipString <<
      "\", skipMeasureNumber: '" <<
      skipMeasureNumber <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string regularExpression (
    "[[:space:]]*"
    "s" //
    "[[:space:]]*"
    "([[:digit:]]+)" // skipNotesDuration
    "[[:space:]]*"
    "(\\.*)"         // skipDots
    "[[:space:]]*"
    );

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "regularExpression: " <<
      regularExpression;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (skipString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for skip string \"" << skipString <<
      "\" with std::regex \"" << regularExpression <<
      "\":" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      ss <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    ss << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  // handling skipString à la LilyPond, such as "s4.."

  if (smSize != 3) {
    std::stringstream ss;

    ss <<
      "skipString \"" << skipString <<
      "\" is ill-formed";

    oahError (ss.str ());
  }

  std::string
    skipNotesDuration = sm [1],
    skipDots = sm [2];

  size_t dotsNumber = skipDots.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "skipNotesDuration: \"" <<
      skipNotesDuration <<
      "\"" <<
      std::endl <<

      "skipDots: \"" <<
      skipDots <<
      "\"" <<
      std::endl <<
      "dotsNumber: " <<
      dotsNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // compute the skipNotesDurationKind from skipNotesDuration
  mfDurationKind
    skipNotesDurationKind =
      mfDurationKindFromString (
        inputLineNumber,
        skipNotesDuration);

  // compute the duration whole notes from skipNotesDurationKind
  mfWholeNotes
     notesDurationKindFromMslpString =
       mfDurationKindAsWholeNotes (
         skipNotesDurationKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "notesDurationKindFromMslpString: " <<
      notesDurationKindFromMslpString;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  result =
//    msrNote::createSkipNote (
    msrNote::createRestNote (
      inputLineNumber,
      skipMeasureNumber,
      notesDurationKindFromMslpString, // soundingWholeNotes
      notesDurationKindFromMslpString, // displayWholeNotes
      dotsNumber);

  return result;
}

//________________________________________________________________________
S_msrNote msrNote::createNoteFromString (
  int                inputLineNumber,
  const std::string& noteString,
  const std::string& noteMeasureNumber)
{
  // handling noteString à la LilyPond, such as "bes,4.."

  S_msrNote result;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating note from string \"" <<
      noteString <<
      "\", noteMeasureNumber: '" <<
      noteMeasureNumber <<
      "', line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // noteString contains note specification
  // decipher it to extract its components

  std::string regularExpression (
    "[[:space:]]*"
    "([[:lower:]]+)" // notePitch
    "[[:space:]]*"
    "([,|']*)"       // noteOctaveIndication
    "[[:space:]]*"
    "([[:digit:]]+)" // noteNotesDuration
    "[[:space:]]*"
    "(\\.*)"         // noteDots
    "[[:space:]]*"
    );

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "regularExpression: " <<
      regularExpression;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::regex  e (regularExpression);
  std::smatch sm;

  regex_match (noteString, sm, e);

  size_t smSize = sm.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "There are " << smSize << " matches" <<
      " for note string \"" << noteString <<
      "\" with std::regex \"" << regularExpression <<
      "\":" <<
      std::endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      ss <<
        i << ": " << "\"" << sm [i] << "\"" <<
        std::endl;
    } // for
    ss << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  // handling noteString à la LilyPond, such as "bes,4.."

  if (smSize != 5) {
    std::stringstream ss;

    ss <<
      "noteString \"" << noteString <<
      "\" is ill-formed";

    oahError (ss.str ());
  }

  std::string
    notePitch = sm [1],
    noteOctaveIndication = sm [2],
    noteNotesDuration = sm [3],
    noteDots = sm [4];

  size_t dotsNumber = noteDots.size ();

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "notePitch: \"" <<
      notePitch <<
      "\"" <<
      std::endl <<

      "noteOctaveIndication: \"" <<
      noteOctaveIndication <<
      "\"" <<
      std::endl <<

      "noteNotesDuration: \"" <<
      noteNotesDuration <<
      "\"" <<
      std::endl <<

      "noteDots: \"" <<
      noteDots <<
      "\"" <<
      std::endl <<
      "dotsNumber: " <<
      dotsNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // fetch the quarternotes pitches kind
  msrQuarterTonesPitchKind
    quarterTonesPitchKind =
      quarterTonesPitchKindFromString (
        gMsrOahGroup->
          getMsrQuarterTonesPitchesLanguageKind (),
        notePitch);

  // compute the octave from noteOctaveIndication
  msrOctaveKind
    octaveKind =
      msrOctaveKindFromCommasOrQuotes (
        inputLineNumber,
        noteOctaveIndication);

  // compute the noteNotesDurationKind from noteNotesDuration
  mfDurationKind
    noteNotesDurationKind =
      mfDurationKindFromString (
        inputLineNumber,
        noteNotesDuration);

  // compute the duration whole notes from noteNotesDurationKind
  mfWholeNotes
     notesDurationKindFromMslpString =
       mfDurationKindAsWholeNotes (
         noteNotesDurationKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "quarterTonesPitchKind: " <<
      quarterTonesPitchKind <<
      std::endl <<
      "octaveKind: " <<
      octaveKind <<
      std::endl <<
      "notesDurationKindFromMslpString: " <<
      notesDurationKindFromMslpString;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  result =
    msrNote::createRegularNote (
      inputLineNumber,
      noteMeasureNumber,
      quarterTonesPitchKind, // msrQuarterTonesPitchKind::kQTP_D_Natural
      octaveKind, // msrOctaveKind::kOctave3
      notesDurationKindFromMslpString, // soundingWholeNotes
      notesDurationKindFromMslpString, // displayWholeNotes
      dotsNumber);

  return result;
}

//________________________________________________________________________
S_msrNote msrNote::createNoteFromSemiTonesPitchAndOctave (
  int                                 inputLineNumber,
  const S_msrSemiTonesPitchAndOctave& semiTonesPitchAndOctave)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    semiTonesPitchAndOctave != nullptr,
    "semiTonesPitchAndOctave is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  msrNote * obj =
    new msrNote (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()

//       K_MEASURE_NUMBER_UNKNOWN_, // JMI ???  // 0.9.66

      msrNoteKind::kNoteRegularInMeasure, // noteKind

      quarterTonesPitchKindFromSemiTonesPitchKind (
        semiTonesPitchAndOctave->
          getSemiTonesPitchKind ()),
      semiTonesPitchAndOctave->
        getOctaveKind (),

      K_WHOLE_NOTES_ZERO, // soundingWholeNotes,
      K_WHOLE_NOTES_ZERO, // displayWholeNotes,

      0, // dotsNumber,

      mfDurationKind::kDuration_UNKNOWN_, // noteGraphicNotesDuration

      msrQuarterTonesPitchKind::kQTP_UNKNOWN_, // noteDisplayQuarterTonesPitch
      msrOctaveKind::kOctave_UNKNOWN_, // noteDisplayOctave,

      msrNoteIsACueNoteKind::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // default value

      msrNoteHeadKind::kNoteHeadNormal, // JMI
      msrNoteHeadFilledKind::kNoteHeadFilledYes, // JMI
      msrNoteHeadParenthesesKind::kNoteHeadParenthesesNo); // JMI
  assert (obj != nullptr);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesOctaveEntry ()) {
    std::stringstream ss;

    ss <<
      "Creating note " <<
      obj->asShortString () <<
      " from semitones pitch and octave " <<
      semiTonesPitchAndOctave->asString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return obj;
}

//________________________________________________________________________
// void msrNote::setMeasureElementPositionInMeasure (
//   const S_msrMeasure& measure,
//   const mfWholeNotes&     positionInMeasure,
//   const std::string&  context)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTracePositionInMeasures ()) {
//     S_msrMeasure
//       upLinkToMeasure =
//         getMeasureElementUpLinkToMeasure ();
//
//     std::stringstream ss;
//
//     ss <<
//       "Setting the measure position of " <<
//       asString () <<
//       " to " <<
//       positionInMeasure.asString () <<
//       " (was '" <<
//       fMeasureElementPositionInMeasure.asString () <<
//       "') in measure " <<
//       measure->asShortString () <<
//       " (measureElementMeasureNumber: " <<
//       upLinkToMeasure->getMeasureNumber () <<
//       "), context: \"" <<
//       context <<
//       "\"";
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     positionInMeasure != K_POSITION_IN_MEASURE_UNKNOWN_,
//     "positionInMeasure == K_POSITION_IN_MEASURE_UNKNOWN_");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   // handle the chord itself
//   msrMeasureElement::setMeasureElementPositionInMeasure (
//     measure,
//     positionInMeasure,
//     context);
//
//   setNoteAttachedElementsPositionInMeasure (
//     measure,
//     positionInMeasure);
// }

void msrNote::setNoteAttachedElementsPositionInMeasure (
  const S_msrMeasure& measure,
  const mfWholeNotes&     positionInMeasure)
{
// // JMI 0.9.66
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "setNoteAttachedElementsPositionInMeasure(), fNoteHarmoniesList.size (): " <<
      fNoteHarmoniesList.size ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

// JMI 0.9.66
//   if (fNoteHarmoniesList.size ()) {
//     std::list <S_msrHarmony>::const_iterator i;
//     for (S_msrHarmony harmony : fNoteHarmoniesList) {
//       // set the harmony measure position, taking it's offset into account
//       harmony->
//         setMeasureElementPositionInMeasure (
//           measure,
//           positionInMeasure,
//           "msrNote::setMeasureElementPositionInMeasure()");
//     } // for
//   }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "fNoteFiguredBassesList.size (): " <<
      fNoteFiguredBassesList.size ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // are there figured bass elements attached to this note?
//   if (fNoteFiguredBassesList.size ()) {
//     std::list <S_msrFiguredBass>::const_iterator i;
//     for (S_msrFiguredBass figuredBass : fNoteFiguredBassesList) {
//       // set the figured bass element measure position
//       figuredBass->
//         setMeasureElementPositionInMeasure (
//           measure,
//           positionInMeasure,
//           "msrNote::setMeasureElementPositionInMeasure()");
//     } // for
//   }

//   // are there dal segnos attached to this note?
//   if (fNoteDalSegnosList.size ()) {
//     for (S_msrDalSegno dalSegno : fNoteDalSegnosList) {
//       // set the dal segno measure position
//       dalSegno->
//         setDalSegnoPositionInMeasure (
//           measure,
//           positionInMeasure,
//           "msrNote::setMeasureElementPositionInMeasure()");
//     } // for
//   }
}

void msrNote::setNoteOccupiesAFullMeasure ()
{
  fNoteOccupiesAFullMeasure = true;
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
    fetchNoteIsARest ()
    	&&
    fNoteDisplayOctaveKind != msrOctaveKind::kOctave_UNKNOWN_;
}

void msrNote::setNoteStem (const S_msrStem& stem)
{
  // register note stem
  fNoteStem = stem;

  // mark note as stemless if relevant
  switch (stem->getStemKind ()) {
    case msrStemKind::kStemKind_NONE:
      break;
    case msrStemKind::kStemKindNeutral:
	    fNoteIsStemless = true;
      break;
    case msrStemKind::kStemKindUp:
      break;
    case msrStemKind::kStemKindDown:
      break;
    case msrStemKind::kStemKindDouble:
      break;
  } // switch
}

void msrNote::setNoteBelongsToAChord ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Setting note " <<
      asShortString () <<
      " to belong to a chord"
      ", line " << fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNoteBelongsToAChord = true;
}

void msrNote::setNoteBelongsToATuplet ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Setting note " <<
      asShortString () <<
      " to belong to a tuplet"
      ", line " << fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNoteBelongsToATuplet = true;
}

void msrNote::determineTupletMemberSoundingFromDisplayWholeNotes (
  int actualNotes,
  int normalNotes)
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

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Determining tuplet sounding from display whole notes" <<
      " for note '" <<
      asShortString () <<
      ", tuplet factor is '" <<
      actualNotes << '/' << normalNotes <<
      "', line " << fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // the display whole notes are known, compute the sounding ones
  setMeasureElementSoundingWholeNotes (
    fNoteDisplayWholeNotes
      *
    normalNotes
      /
    actualNotes,
    "determineTupletMemberSoundingFromDisplayWholeNotes()");

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "The result is: '" <<
      asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrNote::appendBeamToNote (
  const S_msrBeam& beam)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBeams ()) {
    std::stringstream ss;

    ss <<
      "Adding beam " <<
      beam->asShortString () <<
      " to note " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // check the order of the beams begins, continues and ends
if (false) { // JMI, note not yet append to anything....
  S_msrVoice
    voice = fetchNoteUpLinkToVoice ();

  voice->checkBeamNumber (beam, this);
}

  // now append the beam
  fNoteBeamsList.push_back (beam);
}

void msrNote::appendArticulationToNote (
  const S_msrArticulation& art)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceArticulations ()) {
    std::stringstream ss;

    ss <<
      "Adding articulation " <<
      art->asShortString () <<
      " to note " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNoteArticulationsList.push_back (art);
}

void msrNote::appendSpannerToNote (
  const S_msrSpanner& spanner)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSpanners ()) {
    std::stringstream ss;

    ss <<
      "Appending spanner " <<
      spanner->asString () <<
      " to note " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
        case msrSpannerTypeKind::kSpannerType_UNKNOWN_:
          // JMI ???
          break;
      } // switch
      break;
  } // switch

  // append spanner to note spanners
  fNoteSpannersList.push_back (spanner);
}

void msrNote::appendTechnicalToNote (
  const S_msrTechnical& technical)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTechnicals ()) {
    std::stringstream ss;

    ss <<
      "Adding technical " <<
      technical->asString () <<
      " to note " <<
      asString () <<
      ", line " << fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the technical to the note technicals list
  fNoteTechnicalsList.push_back (technical);
}

void msrNote::appendTechnicalWithIntegerToNote (
  const S_msrTechnicalWithInteger& technicalWithInteger)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTechnicals ()) {
    std::stringstream ss;

    ss <<
      "Adding technical with integer " <<
      technicalWithInteger->asString () <<
      " to note " <<
      asString () <<
      ", line " << fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the technical with integer to the note technicals with integers list
  fNoteTechnicalWithIntegersList.push_back (
    technicalWithInteger);
}

void msrNote::appendTechnicalWithFloatToNote (
  const S_msrTechnicalWithFloat& technicalWithFloat)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTechnicals ()) {
    std::stringstream ss;

    ss <<
      "Adding technical with float " <<
      technicalWithFloat->asString () <<
      " to note " <<
      asString () <<
      ", line " << fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the technical with float to the note technicals with floats list
  fNoteTechnicalWithFloatsList.push_back (
    technicalWithFloat);
}

void msrNote::appendTechnicalWithStringToNote (
  const S_msrTechnicalWithString& technicalWithString)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTechnicals ()) {
    std::stringstream ss;

    ss <<
      "Adding technical with string'" <<
      technicalWithString->asString () <<
      " to note " <<
      asString () <<
      ", line " << fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the technical with string to the note technicals with strings list
  fNoteTechnicalWithStringsList.push_back (
    technicalWithString);
}

void msrNote::appendOrnamentToNote (
  const S_msrOrnament& ornament)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceOrnaments ()) {
    std::stringstream ss;

    ss <<
      "Adding ornament '" <<
      ornament->asString () <<
      " to note " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the ornament to the note ornaments list
  fNoteOrnamentsList.push_back (ornament);

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

void msrNote::appendGlissandoToNote (
  const S_msrGlissando& glissando)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGlissandos ()) {
    std::stringstream ss;

    ss <<
      "Adding glissando " <<
      glissando->asShortString () <<
      " to note " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the glissando to the note glissandos list
  fNoteGlissandosList.push_back (glissando);
}

void msrNote::appendSlideToNote (
  const S_msrSlide& slide)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSlides ()) {
    std::stringstream ss;

    ss <<
      "Adding slide " <<
      slide->asShortString () <<
      " to note " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the slide to the note glissandos list
  fNoteSlidesList.push_back (slide);
}

void msrNote::appendTieToNote (
	const S_msrTie& tie)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTies ()) {
    std::stringstream ss;

    ss <<
      "Adding tie " <<
      tie->asShortString () <<
      " to note " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the slide to the note ties list
  fNoteTiesList.push_back (tie);
}

void msrNote::setGraceNotesGroupBeforeNote (
  const S_msrGraceNotesGroup& graceNotesGroupBefore)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "Attaching grace notes group " <<
      graceNotesGroupBefore->asString () <<
      " before note " <<
      asString () <<
      ", line " << graceNotesGroupBefore->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the before grace notes group in the note
  fGraceNotesGroupBeforeNote = graceNotesGroupBefore;

  // setup the grace notes group's note upLink
  graceNotesGroupBefore->
    setGraceNotesGroupUpLinkToNote (
      this);
}

void msrNote::setGraceNotesGroupAfterNote (
  const S_msrGraceNotesGroup& graceNotesGroupAfter)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotesBasics ()) {
    std::stringstream ss;

    ss <<
      "Attaching grace notes group " <<
      graceNotesGroupAfter->asString () <<
      " after note " <<
      asString () <<
      ", line " << graceNotesGroupAfter->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the after grace notes group in the note
  fGraceNotesGroupAfterNote = graceNotesGroupAfter;

  // setup the grace notes group's note upLink
  graceNotesGroupAfter->
    setGraceNotesGroupUpLinkToNote (
      this);
}

/* JMI
void msrNote::setNoteAfterGraceNotesGroup (S_msrGraceNotesGroup afterGraceNotesGroup)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Attaching afterGraceNotesGroup '" << afterGraceNotesGroup->asString () <<
      " to note " << asShortString () <<
      ", line " << afterGraceNotesGroup->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the grace notes in the note
  fNoteAfterGraceNotesGroup = afterGraceNotesGroup;
}
*/

void msrNote::setNoteSingleTremolo (
  const S_msrSingleTremolo& trem)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTremolos ()) {
    std::stringstream ss;

    ss <<
      "Adding singleTremolo " <<
      trem->asString () <<
      " to note " <<
      asString () <<
      ", line " << trem->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the singleTremolo in the note
  fNoteSingleTremolo = trem;
}

void msrNote::appendDynamicToNote (
  const S_msrDynamic& dynamic)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDynamics ()) {
    std::stringstream ss;

    ss <<
      "Attaching dynamic " <<
      dynamic->asString () <<
      " to note " <<
      asString () <<
      ", line " << dynamic->getInputLineNumber ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNoteDynamicsList.push_back (dynamic);
}
void msrNote::appendOtherDynamicToNote (
  const S_msrOtherDynamic& otherDynamic)
{
  fNoteOtherDynamicsList.push_back (otherDynamic);
}

void msrNote::appendWordsToNote (
  const S_msrWords& words)
{
  fNoteWordsList.push_back (words);
}

void msrNote::appendSlurToNote (
  const S_msrSlur& slur)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSlurs ()) {
    std::stringstream ss;

    ss <<
      "Adding slur " << slur->asString () <<
      " to note " << asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNoteSlursList.push_back (slur);

  // make sure slur stops appear before slur starts
  fNoteSlursList.sort (
  	msrSlur::compareSlursStopsBeforeStarts);
}

void msrNote::appendLigatureToNote (
  const S_msrLigature& ligature)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLigatures ()) {
    std::stringstream ss;

    ss <<
      "Appending ligature " << ligature << " to note " << asString () <<
       std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fNoteLigaturesList.size ()) {
    if (
      fNoteLigaturesList.back ()->getLigatureKind () == msrLigatureKind::kLigatureStart
        &&
      ligature->getLigatureKind () == msrLigatureKind::kLigatureStop
        &&
      fNoteLigaturesList.back ()->getLigatureNumber () == ligature->getLigatureNumber ()
      ) {
      // it may happen that a given note has a 'ligature start'
      // and a 'ligature stop' in sequence, ignore both

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceLigatures ()) {
        std::stringstream ss;

        ss <<
          "a 'ligature start' is immediately followed by a 'ligature stop'" <<
          std::endl <<
          "with the same number, ignoring both of them at line " <<
          ligature->getInputLineNumber ();

        msrWarning (
          gServiceRunData->getInputSourceName (),
          ligature->getInputLineNumber (),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // remove 'ligature start'
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceLigatures ()) {
        std::stringstream ss;

        ss <<
          "Removing last ligature (start) for note '" <<
          asShortString ();

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fNoteLigaturesList.pop_back ();

      // don't register 'ligature stop'
    }

    else
      fNoteLigaturesList.push_back (ligature);
  }

  else
    fNoteLigaturesList.push_back (ligature);
}

void msrNote::appendPedalToNote (
  const S_msrPedal& pedal)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePedals ()) {
    std::stringstream ss;

    ss <<
      "Appending pedal " << pedal << " to note " << asString () <<
       std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   if (fNotePedalsList.size ()) { // JMI 0.9.71
//     if (
//       fNotePedalsList.back ()->getPedalTypeKind () == msrPedalTypeKind::kPedalTypeStart
//         &&
//       pedal->getPedalTypeKind () == msrPedalTypeKind::kPedalTypeStop
//     ) {
//       // it may happen that a given note has a 'pedal start'
//       // and a 'pedal stop' in sequence, ignore both            // JMI ???
//
// #ifdef MF_TRACE_IS_ENABLED
//       if (gTraceOahGroup->getTracePedals ()) {
//         std::stringstream ss;
//
//         ss <<
//           "a 'pedal start' is immediately followed by a 'pedal stop'" <<
//           std::endl <<
//           "with the same number, ignoring both of them at line " <<
//           pedal->getInputLineNumber ();
//
//         msrWarning (
//           gServiceRunData->getInputSourceName (),
//           pedal->getInputLineNumber (),
//           ss.str ());
//       }
// #endif // MF_TRACE_IS_ENABLED
//
//       // rmeove 'pedal start'
// #ifdef MF_TRACE_IS_ENABLED
//       if (gTraceOahGroup->getTracePedals ()) {
//         std::stringstream ss;
//
//         ss <<
//           "Removing last pedal (start) for note '" <<
//           asShortString ();
//
//         gWaeHandler->waeTrace (
//           __FILE__, __LINE__,
//           ss.str ());
//       }
// #endif // MF_TRACE_IS_ENABLED
//       fNotePedalsList.pop_back ();
//
//       // don't register 'pedal stop'
//     }
//
//     else
//       fNotePedalsList.push_back (pedal);
//   }
//
//   else
//     fNotePedalsList.push_back (pedal);

	fNotePedalsList.push_back (pedal);
}

void msrNote::appendSlashToNote (
  const S_msrSlash& slash)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSlashes ()) {
    std::stringstream ss;

    ss <<
      "Appending slash '" <<
      slash->asString () <<
      " to note " <<
      asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNoteSlashesList.push_back (slash);
}

void msrNote::appendCrescDecrescToNote (
  const S_msrCrescDecresc& crescDecresc)
{
  fNoteCrescDecrescsList.push_back (crescDecresc);
}

void msrNote::appendWedgeToNote (
  const S_msrWedge& wedge)
{
  fNoteWedgesList.push_back (wedge);
}

void msrNote::appendSegnoToNote (
  const S_msrSegno& segno)
{
  fNoteSegnosList.push_back (segno);
}

void msrNote::appendDalSegnoToNote (
  const S_msrDalSegno& dalSegno)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDalSegnos ()) {
    std::stringstream ss;

    ss <<
      "Appending dal segno " <<
      dalSegno->asShortString () <<
      " to note " <<
      asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNoteDalSegnosList.push_back (dalSegno);
}

void msrNote::appendCodaToNote (
  const S_msrCoda& coda)
{
  fNoteCodasList.push_back (coda);
}

void msrNote::appendEyeGlassesToNote (
  const S_msrEyeGlasses& eyeGlasses)
{
  fNoteEyeGlassesList.push_back (eyeGlasses);
}

void msrNote::appendDampToNote (
  const S_msrDamp&  damp)
{
  fNoteDampsList.push_back (damp);
}

void msrNote::appendDampAllToNote (
  const S_msrDampAll& dampAll)
{
  fNoteDampAllsList.push_back (dampAll);
}

void msrNote::appendScordaturaToNote (
  const S_msrScordatura& scordatura)
{
  fNoteScordaturasList.push_back (scordatura);
}

// this 'override' NOT NEEXDED??? JMI 0.9.66
// void msrNote::setMeasureElementVoicePosition (
//   mfWholeNotes&     voicePosition,
//   const std::string& context)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTracePositionInMeasures ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Assigning note voice position of " <<
//       asString () <<
//       " to " << voicePosition <<
//       " in measure '" <<
//       fMeasureElementUpLinkToMeasure->getMeasureNumber () <<
//       ", context: \"" <<
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
//     voicePosition != K_POSITION_IN_MEASURE_UNKNOWN_,
//     "voicePosition == K_POSITION_IN_MEASURE_UNKNOWN_");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   // set measure element voice position
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTracePositionInMeasures ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Setting note voice position of " <<
//       asString () <<
//       " to " << voicePosition <<
//       " in measure '" <<
//       fMeasureElementUpLinkToMeasure->getMeasureNumber () <<
//       ", context: \"" <<
//       context <<
//       "\"" <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fMeasureElementVoicePosition = voicePosition;
//
//   // account for it in voicePosition
//   voicePosition +=
//     fMeasureElementSoundingWholeNotes;
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTracePositionInMeasures ()) {
//      std::stringstream ss;
//
//     ss <<
//       "Position in voice becomes " <<
//       voicePosition <<
//       ", context: \"" <<
//       context <<
//       "\"" <<
//       std::endl;

//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str (),
//   }
// #endif // MF_TRACE_IS_ENABLED
// }

bool msrNote::compareNotesByIncreasingPositionInMeasure (
  const SMARTP<msrNote>& first,
  const SMARTP<msrNote>& second)
{
  return
    bool (
      first->fMeasureElementPositionInMeasure
        <
      second->fMeasureElementPositionInMeasure
    );
}

int msrNote::fetchNoteSlurStartsNumber () const
{
	int result = 0;

	for (S_msrSlur slur : fNoteSlursList) {
		switch (slur->getSlurTypeKind ()) {
			case msrSlurTypeKind::kSlurType_UNKNOWN_:
				break;

			case msrSlurTypeKind::kSlurTypeRegularStart:
				++result;
				break;
			case msrSlurTypeKind::kSlurTypeRegularContinue:
				break;
			case msrSlurTypeKind::kSlurTypeRegularStop:
				break;

			case msrSlurTypeKind::kSlurTypePhrasingStart:
				break;
			case msrSlurTypeKind::kSlurTypePhrasingContinue:
				break;
			case msrSlurTypeKind::kSlurTypePhrasingStop:
				break;
		} // switch
	} // for

	return result;
}

int msrNote::fetchNoteSlurStopsNumber () const
{
	int result = 0;

	for (S_msrSlur slur : fNoteSlursList) {
		switch (slur->getSlurTypeKind ()) {
			case msrSlurTypeKind::kSlurType_UNKNOWN_:
				break;

			case msrSlurTypeKind::kSlurTypeRegularStart:
				break;
			case msrSlurTypeKind::kSlurTypeRegularContinue:
				break;
			case msrSlurTypeKind::kSlurTypeRegularStop:
				++result;
				break;

			case msrSlurTypeKind::kSlurTypePhrasingStart:
				break;
			case msrSlurTypeKind::kSlurTypePhrasingContinue:
				break;
			case msrSlurTypeKind::kSlurTypePhrasingStop:
				break;
		} // switch
	} // for

	return result;
}

// S_msrDynamic msrNote::removeFirstDynamics () // JMI
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceDynamics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Removing first dynamic from note '" <<
//       asShortString ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     fNoteDynamicsList.size () > 0,
//     "fNoteDynamicsList is empty");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   S_msrDynamic dyn = fNoteDynamicsList.front ();
//   fNoteDynamicsList.pop_front ();
//
//   return dyn;
// }

// S_msrWedge msrNote::removeFirstWedge () // JMI
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceWedges ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Removing first wedge from note '" <<
//       asShortString ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     fNoteDynamicsList.size () > 0,
//     "fNoteDynamicsList is empty");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   S_msrWedge wedge = fNoteWedgesList.front ();
//
//   fNoteWedgesList.pop_front ();
//
//   return wedge;
// }

void msrNote::appendSyllableToNote (
  const S_msrSyllable& syllable)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceLyrics ()) {
    std::stringstream ss;

    ss <<
      "Appending syllable " <<
      syllable->asString () <<
      " to note " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNoteSyllablesList.push_back (syllable);

  // set syllable upLink to note
  syllable->
		setSyllableUpLinkToNote (this);
}

void msrNote::appendHarmonyToNote (
  const S_msrHarmony& harmony)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "Appending harmony " <<
      harmony->asString () <<
      " to the harmonies list of " <<
      asString () <<
      ", line " << fInputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set the harmony's whole notes duration
  // change the harmony whole notes if it belongs to a tuplet ??? utf8.xml JMI 0.9.66
  // JMI 0.9.67 NOT if there are several harmonies with offsets on the same note -- HARMFUL!
//   harmony->
//     setMeasureElementSoundingWholeNotes (
//       fMeasureElementSoundingWholeNotes,
//       "appendHarmonyToNote()");

  fNoteHarmoniesList.push_back (harmony);

//   // register this note as the harmony upLink // JMI 0.9.66 JMI 0.9.66 JMI 0.9.66 MERDUM
//   harmony->
//     setHarmonyUpLinkToNote (this);
}

// void msrNote::appendFiguredBassToNote (
//   const S_msrFiguredBass& figuredBass)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceFiguredBasses ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Append figured bass " <<
//       figuredBass->asString () <<
//       " to the figured bass list of " <<
//       asString () <<
//       ", line " << fInputLineNumber <<
//       std::endl;
//
//       gWaeHandler->waeTrace (
//         __FILE__, __LINE__,
//         ss.str (),
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // set the figured bass's whole notes duration
//   // change the figured bass whole notes if it belongs to a tuplet ??? utf8.xml JMI 0.9.66
//   // JMI 0.9.67 NOT if there are several figured basses with offsets on the same note -- HARMFUL!
//   figuredBass->
//     setMeasureElementSoundingWholeNotes (
//       fMeasureElementSoundingWholeNotes,
//       "appendFiguredBassToNote()");
//
//   fNoteFiguredBassesList.push_back (figuredBass);
//
// //   // register this note as the figured bass upLink // JMI 0.9.66 JMI 0.9.66 JMI 0.9.66 MERDUM
// //   figuredBass->
// //     setFiguredBassUpLinkToNote (this);
// }

void msrNote::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrNote::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrNote::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrNote::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrNote::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrNote::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrNote::browseData (basevisitor* v)
{
  // browse the grace notes group before note if any
  if (fGraceNotesGroupBeforeNote) {
    // fetch the score
    S_msrScore
      score =
        fetchNoteUpLinkToScore ();

    if (score) {
      Bool
        inhibitGraceNotesGroupsBeforeBrowsing =
          score->
            getInhibitGraceNotesGroupsBeforeBrowsing ();

      if (inhibitGraceNotesGroupsBeforeBrowsing) {
#ifdef MF_TRACE_IS_ENABLED
        if (
          gMsrOahGroup->getTraceMsrVisitors ()
            ||
          gTraceOahGroup->getTraceNotes ()
            ||
          gTraceOahGroup->getTraceGraceNotes ()
        ) {
          std::stringstream ss;

          ss <<
	          "% ==> visiting grace notes groups before notes is inhibited";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
      }
      else {
        // browse the grace notes group before note
        msrBrowser<msrGraceNotesGroup> browser (v);
        browser.browse (*fGraceNotesGroupBeforeNote);
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
  if (fNoteBeamsList.size ()) {
    ++gIndenter;
//     std::list <S_msrBeam>::const_iterator i;
//     for (i = fNoteBeamsList.begin (); i != fNoteBeamsList.end (); ++i) {
//       // browse the beam
//       msrBrowser<msrBeam> browser (v);
//       browser.browse (*(*i));
//     } // for
    std::list <S_msrBeam>::const_iterator i;
    for (S_msrBeam beam : fNoteBeamsList) {
      // browse the beam
      msrBrowser<msrBeam> browser (v);
      browser.browse (*beam);
    } // for
    --gIndenter;
  }

  // browse the articulations if any
  if (fNoteArticulationsList.size ()) {
    ++gIndenter;
    std::list <S_msrArticulation>::const_iterator i;
    for (i = fNoteArticulationsList.begin (); i != fNoteArticulationsList.end (); ++i) {
      // browse the articulation
      msrBrowser<msrArticulation> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the spanners if any
  if (fNoteSpannersList.size ()) {
    ++gIndenter;
    std::list <S_msrSpanner>::const_iterator i;
    for (i = fNoteSpannersList.begin (); i != fNoteSpannersList.end (); ++i) {
      // browse the spanner
      msrBrowser<msrSpanner> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the technicals if any
  if (fNoteTechnicalsList.size ()) {
    ++gIndenter;
    std::list <S_msrTechnical>::const_iterator i;
    for (
      i = fNoteTechnicalsList.begin ();
      i != fNoteTechnicalsList.end ();
      ++i
  ) {
      // browse the technical
      msrBrowser<msrTechnical> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the technicals with integer if any
  if (fNoteTechnicalWithIntegersList.size ()) {
    ++gIndenter;
    std::list <S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i = fNoteTechnicalWithIntegersList.begin ();
      i != fNoteTechnicalWithIntegersList.end ();
      ++i
  ) {
      // browse the technical
      msrBrowser<msrTechnicalWithInteger> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the technicals with float if any
  if (fNoteTechnicalWithFloatsList.size ()) {
    ++gIndenter;
    std::list <S_msrTechnicalWithFloat>::const_iterator i;
    for (
      i = fNoteTechnicalWithFloatsList.begin ();
      i != fNoteTechnicalWithFloatsList.end ();
      ++i
  ) {
      // browse the technical
      msrBrowser<msrTechnicalWithFloat> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the technicals with string if any
  if (fNoteTechnicalWithStringsList.size ()) {
    ++gIndenter;
    std::list <S_msrTechnicalWithString>::const_iterator i;
    for (
      i = fNoteTechnicalWithStringsList.begin ();
      i != fNoteTechnicalWithStringsList.end ();
      ++i
  ) {
      // browse the technical
      msrBrowser<msrTechnicalWithString> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the ornaments if any
  if (fNoteOrnamentsList.size ()) {
    ++gIndenter;
    std::list <S_msrOrnament>::const_iterator i;
    for (i = fNoteOrnamentsList.begin (); i != fNoteOrnamentsList.end (); ++i) {
      // browse the ornament
      msrBrowser<msrOrnament> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the glissandos if any
  if (fNoteGlissandosList.size ()) {
    ++gIndenter;
    std::list <S_msrGlissando>::const_iterator i;
    for (i = fNoteGlissandosList.begin (); i != fNoteGlissandosList.end (); ++i) {
      // browse the glissando
      msrBrowser<msrGlissando> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the slides if any
  if (fNoteSlidesList.size ()) {
    ++gIndenter;
    std::list <S_msrSlide>::const_iterator i;
    for (i = fNoteSlidesList.begin (); i != fNoteSlidesList.end (); ++i) {
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

  // browse the ties if any
  if (fNoteTiesList.size ()) {
    ++gIndenter;
    for (S_msrTie tie : fNoteTiesList) {
      // browse the tie
      msrBrowser<msrTie> browser (v);
      browser.browse (*tie);
    } // for
    --gIndenter;
  }

  // browse the dynamics if any
  if (fNoteDynamicsList.size ()) {
    ++gIndenter;
    std::list <S_msrDynamic>::const_iterator i;
    for (i = fNoteDynamicsList.begin (); i != fNoteDynamicsList.end (); ++i) {
      // browse the dynamics
      msrBrowser<msrDynamic> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the other dynamics if any
  if (fNoteOtherDynamicsList.size ()) {
    ++gIndenter;
    std::list <S_msrOtherDynamic>::const_iterator i;
    for (i = fNoteOtherDynamicsList.begin (); i != fNoteOtherDynamicsList.end (); ++i) {
      // browse the other dynamics
      msrBrowser<msrOtherDynamic> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the words if any
  if (fNoteWordsList.size ()) {
    ++gIndenter;
    std::list <S_msrWords>::const_iterator i;
    for (i = fNoteWordsList.begin (); i != fNoteWordsList.end (); ++i) {
      // browse the words
      msrBrowser<msrWords> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the slurs if any
  if (fNoteSlursList.size ()) {
    ++gIndenter;
    for (S_msrSlur slur : fNoteSlursList) {
      // browse the slur
      msrBrowser<msrSlur> browser (v);
      browser.browse (*slur);
    } // for
    --gIndenter;
  }

  // browse the ligatures if any
  if (fNoteLigaturesList.size ()) {
    ++gIndenter;
    std::list <S_msrLigature>::const_iterator i;
    for (i = fNoteLigaturesList.begin (); i != fNoteLigaturesList.end (); ++i) {
      // browse the ligature
      msrBrowser<msrLigature> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the pedals if any
  if (fNotePedalsList.size ()) {
    ++gIndenter;
    std::list <S_msrPedal>::const_iterator i;
    for (i = fNotePedalsList.begin (); i != fNotePedalsList.end (); ++i) {
      // browse the pedal
      msrBrowser<msrPedal> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the crescDecresc if any
  if (fNoteCrescDecrescsList.size ()) {
    ++gIndenter;
    for (S_msrCrescDecresc crescDecresc : fNoteCrescDecrescsList) {
      // browse the crescDecresc
      msrBrowser<msrCrescDecresc> browser (v);
      browser.browse (*crescDecresc);
    } // for
    --gIndenter;
  }

  // browse the wedges if any
  if (fNoteWedgesList.size ()) {
    ++gIndenter;
    for (S_msrWedge wedge : fNoteWedgesList) {
      // browse the crescDecresc
      msrBrowser<msrWedge> browser (v);
      browser.browse (*wedge);
    } // for
    --gIndenter;
  }

  // browse the slashes if any
  if (fNoteSlashesList.size ()) {
    ++gIndenter;
    for (S_msrSlash slash : fNoteSlashesList) {
      // browse the slash
      msrBrowser<msrSlash> browser (v);
      browser.browse (*slash);
    } // for
    --gIndenter;
  }

  // browse the segnos if any
  if (fNoteSegnosList.size ()) {
    ++gIndenter;
    std::list <S_msrSegno>::const_iterator i;
    for (i = fNoteSegnosList.begin (); i != fNoteSegnosList.end (); ++i) {
      // browse the segno
      msrBrowser<msrSegno> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the dal segnos if any
  if (fNoteDalSegnosList.size ()) {
    ++gIndenter;
    std::list <S_msrDalSegno>::const_iterator i;
    for (i = fNoteDalSegnosList.begin (); i != fNoteDalSegnosList.end (); ++i) {
      // browse the dal segno
      msrBrowser<msrDalSegno> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the codas if any
  if (fNoteCodasList.size ()) {
    ++gIndenter;
    std::list <S_msrCoda>::const_iterator i;
    for (i = fNoteCodasList.begin (); i != fNoteCodasList.end (); ++i) {
      // browse the coda
      msrBrowser<msrCoda> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the eyeglasses if any
  if (fNoteEyeGlassesList.size ()) {
    ++gIndenter;
    std::list <S_msrEyeGlasses>::const_iterator i;
    for (i = fNoteEyeGlassesList.begin (); i != fNoteEyeGlassesList.end (); ++i) {
      // browse the eyeglasses
      msrBrowser<msrEyeGlasses> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the damps if any
  if (fNoteDampsList.size ()) {
    ++gIndenter;
    std::list <S_msrDamp>::const_iterator i;
    for (i = fNoteDampsList.begin (); i != fNoteDampsList.end (); ++i) {
      // browse the damp
      msrBrowser<msrDamp> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the damp alls if any
  if (fNoteDampAllsList.size ()) {
    ++gIndenter;
    std::list <S_msrDampAll>::const_iterator i;
    for (i = fNoteDampAllsList.begin (); i != fNoteDampAllsList.end (); ++i) {
      // browse the damp all
      msrBrowser<msrDampAll> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the scordaturas if any
  if (fNoteScordaturasList.size ()) {
    ++gIndenter;
    std::list <S_msrScordatura>::const_iterator i;
    for (i = fNoteScordaturasList.begin (); i != fNoteScordaturasList.end (); ++i) {
      // browse the scordatura
      msrBrowser<msrScordatura> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the harmonies if any
  if (fNoteHarmoniesList.size ()) {
    ++gIndenter;
    for (S_msrHarmony harmony : fNoteHarmoniesList) {
      // browse the harmony
      msrBrowser<msrHarmony> browser (v);
      browser.browse (*harmony);
    } // for
    --gIndenter;
  }

  // browse the figured basses if any
  if (fNoteFiguredBassesList.size ()) {
    ++gIndenter;
    for (S_msrFiguredBass figuredBass : fNoteFiguredBassesList) {
      // browse the figured bass
      msrBrowser<msrFiguredBass> browser (v);
      browser.browse (*figuredBass);
    } // for
    --gIndenter;
  }

  // browse the syllables if any // NO JMI ??? 0.9.73 this is done elsewhere for xml2ly
  if (fNoteSyllablesList.size ()) {
    ++gIndenter;
    for (S_msrSyllable syllable : fNoteSyllablesList) {
      // browse the syllable
      msrBrowser<msrSyllable> browser (v);
      browser.browse (*syllable);
    } // for
    --gIndenter;
  }

  // browse the grace notes group after note if any
  if (fGraceNotesGroupAfterNote) {
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
#ifdef MF_TRACE_IS_ENABLED
        if (
          gMsrOahGroup->getTraceMsrVisitors ()
            ||
          gTraceOahGroup->getTraceNotes ()
            ||
          gTraceOahGroup->getTraceGraceNotes ()
        ) {
          std::stringstream ss;

          ss <<
            "% ==> visiting grace notes groups after notes is inhibited";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
      }
      else {
        // browse the grace notes group after note
        msrBrowser<msrGraceNotesGroup> browser (v);
        browser.browse (*fGraceNotesGroupAfterNote);
      }
    }
  }
}

std::string msrNote::noteDiatonicPitchKindAsString (
  int inputLineNumber) const
{
  return
    msrDiatonicPitchKindAsStringInLanguage (
      gMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
      noteDiatonicPitchKind (
        fInputLineNumber));
}

std::string msrNote::notePitchAsString () const
{
  std::stringstream ss;

	// ss << "======> notePitchAsString()" << std::endl;

  switch (fNoteKind) {
    case msrNoteKind::kNote_UNKNOWN_:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteInDoubleTremolo:
    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteRestInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
    case msrNoteKind::kNoteRegularInChord:
    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      ss <<
        msrQuarterTonesPitchKindAsStringInLanguage (
          fNoteQuarterTonesPitchKind,
          gMsrOahGroup->
            getMsrQuarterTonesPitchesLanguageKind ());
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      ss << "kNoteUnpitchedInMeasure";
      break;
    case msrNoteKind::kNoteUnpitchedInTuplet:
      ss << "kNoteUnpitchedInTuplet";
      break;
  } // switch

  return ss.str ();
}

std::string msrNote::noteDisplayPitchKindAsString () const
{
  std::stringstream ss;

// 	// ss << "======> noteDisplayPitchKindAsString()" << std::endl;

  ss <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fNoteQuarterTonesDisplayPitchKind,
      gMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ());

  return ss.str ();
}

std::string msrNote::noteSoundingWholeNotesPitchAndOctaveAsString () const
{
  std::string result;

  if (fMeasureElementSoundingWholeNotes.getNumerator () == 0)
    result = "[0 WHOLE NOTES]";
  else
    result =
      wholeNotesPitchAndOctaveAsString (
        fInputLineNumber,
        fMeasureElementSoundingWholeNotes);

  return result;
}

std::string msrNote::noteDisplayWholeNotesPitchAndOctaveAsString () const
{
  std::string result;

  if (fNoteDisplayWholeNotes.getNumerator () == 0) {
    result = " *** NO DISPLAY WHOLE NOTES ***";
  }
  else {
    result =
      wholeNotesPitchAndOctaveAsString (
        fInputLineNumber,
        fNoteDisplayWholeNotes);
  }

  return result;
}

std::string msrNote::noteGraphicNotesDurationAsMusicXMLString () const
{
  std::string
    result =
      mfDurationKindAsMusicXMLType (
        fNoteGraphicNotesDurationKind);

  return result;
}

// std::string msrNote::tupletNoteGraphicNotesDurationpitchAndOctaveAsString ( // JMI 0.9.66
//   int actualNotes, int normalNotes) const
// {
//   return
//     wholeNotesPitchAndOctaveAsString (
//       fInputLineNumber,
//       fMeasureElementSoundingWholeNotes
//         *
//       mfRational (actualNotes, normalNotes));
// }

std::string msrNote::noteCoreAsString () const
{
  std::stringstream ss;

	// two superflous variables??? JMI 0.9.70
	Bool doIncludeSoundingWholeNotes (true);
	Bool doIncludeDisplayGraceNotesWholeNotes (false);

	Bool doIncludeTupletFactor (false);

  switch (fNoteKind) {
    case msrNoteKind::kNote_UNKNOWN_:
      ss <<
      	"kNote_UNKNOWN_"; // JMI 0.9.70
      break;

    case msrNoteKind::kNoteRestInMeasure:
			break;
    case msrNoteKind::kNoteSkipInMeasure:
			break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
			break;

    case msrNoteKind::kNoteRegularInMeasure:
			break;

    case msrNoteKind::kNoteInDoubleTremolo:
			break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteRestInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
    	doIncludeSoundingWholeNotes = false;
    	doIncludeDisplayGraceNotesWholeNotes = true;
      break;

    case msrNoteKind::kNoteRegularInChord:
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    	doIncludeTupletFactor = true;
      break;

    case msrNoteKind::kNoteRestInTuplet:
    	doIncludeTupletFactor = true;
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    	doIncludeSoundingWholeNotes = false;
    	doIncludeDisplayGraceNotesWholeNotes = true;
    	doIncludeTupletFactor = true;
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
    	doIncludeTupletFactor = true;
      break;
  } // switch

	ss <<
		"fNoteKind: " << fNoteKind << ", ";

	if (doIncludeSoundingWholeNotes) {
		ss <<
			msrQuarterTonesPitchKindAsStringInLanguage (
				fNoteQuarterTonesPitchKind,
				gMsrOahGroup->
					getMsrQuarterTonesPitchesLanguageKind ()) <<

			noteSoundingWholeNotesPitchAndOctaveAsString () <<

			'-' <<
			fNoteOctaveKind;
	}

	if (doIncludeDisplayGraceNotesWholeNotes) {
		ss <<
			msrQuarterTonesPitchKindAsStringInLanguage (
				fNoteQuarterTonesPitchKind,
				gMsrOahGroup->
					getMsrQuarterTonesPitchesLanguageKind ()) <<

			noteDisplayWholeNotesPitchAndOctaveAsString () <<

			'-' <<
			fNoteOctaveKind;
  }

	if (doIncludeTupletFactor) {
		ss <<
//       ss <<
// //       	noteCoreAsString () << // JMI 0.9.70
//         ", fNoteDisplayWholeNotes: " <<
//         fNoteDisplayWholeNotes;
//
//       ss <<
//         ", noteTupletFactor " <<
//         fNoteTupletFactor;

// 			noteCoreAsString () <<
			", fNoteGraphicNotesDurationKind: " <<
			fNoteGraphicNotesDurationKind <<
			", fNoteDisplayWholeNotes: " <<
			fNoteDisplayWholeNotes.asFractionString () <<
			", fNoteTupletFactor " <<
			fNoteTupletFactor.asString ();
	}

  return ss.str ();
}

std::string msrNote::noteCoreAndInputLineNumbersAsString () const
{
  std::stringstream ss;

  ss <<
		noteCoreAsString () <<
    ", " <<
    mfInputLineNumberAsString (
    	fInputLineNumber);

  return ss.str ();
}

// std::string msrNote::asMinimalString () const
// {
//   std::stringstream ss;
//
//   ss <<
//   	"[Note" <<
//     ", " <<
//     mfInputLineNumberAsString (
//     	fInputLineNumber) <<
//     ", ";
//
//   switch (fNoteKind) {
//     case msrNoteKind::kNote_UNKNOWN_:
//       ss <<
//       	noteCoreAsString ();
//       break;
//
//     case msrNoteKind::kNoteRestInMeasure:
//       ss <<
//       	noteCoreAsString ();
//       break;
//
//     case msrNoteKind::kNoteSkipInMeasure:
//       ss <<
//       	noteCoreAsString ();
//       break;
//
//     case msrNoteKind::kNoteUnpitchedInMeasure:
//       ss <<
//       	noteCoreAsString ();
//       break;
//
//     case msrNoteKind::kNoteRegularInMeasure:
// 			ss <<
//       	noteCoreAsString ();
//       break;
//
//     case msrNoteKind::kNoteInDoubleTremolo:
//       ss <<
//       	noteCoreAsString ();
//       break;
//
//     case msrNoteKind::kNoteRegularInGraceNotesGroup:
//       ss <<
//       	noteCoreAsString ();
//       break;
//
//     case msrNoteKind::kNoteRestInGraceNotesGroup:
//       ss <<
//       	noteCoreAsString ();
//       break;
//
//     case msrNoteKind::kNoteSkipInGraceNotesGroup:
//       ss <<
//       	noteCoreAsString ();
//       break;
//
//    case msrNoteKind::kNoteInChordInGraceNotesGroup:
//       ss <<
//       	noteCoreAsString () <<
//         ", fNoteGraphicNotesDurationKind: " <<
//         fNoteGraphicNotesDurationKind ;
//       break;
//
//     case msrNoteKind::kNoteRegularInChord:
//       ss <<
//       	noteCoreAsString ();
//       break;
//
//     case msrNoteKind::kNoteRegularInTuplet:
//       ss <<
//       	noteCoreAsString () <<
//         ", fNoteGraphicNotesDurationKind: " <<
//         fNoteGraphicNotesDurationKind <<
//         ", fNoteDisplayWholeNotes: " <<
//         fNoteDisplayWholeNotes.asFractionString () <<
//         ", noteTupletFactor " << fNoteTupletFactor.asString ();
//       break;
//
//     case msrNoteKind::kNoteRestInTuplet:
//       ss <<
//       	noteCoreAsString () <<
//         ", fNoteGraphicNotesDurationKind: " <<
//         fNoteGraphicNotesDurationKind <<
//         ", fNoteDisplayWholeNotes: " <<
//         fNoteDisplayWholeNotes.asFractionString () <<
//         ", noteTupletFactor " << fNoteTupletFactor.asString ();
//       break;
//
//     case msrNoteKind::kNoteInTupletInGraceNotesGroup:
//       ss <<
//       	noteCoreAsString () <<
//         ", fNoteGraphicNotesDurationKind: " <<
//         fNoteGraphicNotesDurationKind <<
//         ", fNoteDisplayWholeNotes: " <<
//         fNoteDisplayWholeNotes.asFractionString () <<
//         ", noteTupletFactor " << fNoteTupletFactor.asString ();
//       break;
//
//     case msrNoteKind::kNoteUnpitchedInTuplet:
//       ss <<
//       	noteCoreAsString () <<
//         ", fNoteDisplayWholeNotes: " <<
//         fNoteDisplayWholeNotes;
//
//       ss <<
//         ", noteTupletFactor " << fNoteTupletFactor.asString ();
//       break;
//   } // switch
//
//   ss <<
//     ", line " << fInputLineNumber <<
//     ']';
//
//   return ""; // JMI 0.9.67
//
//   return ss.str ();
// }
//
std::string msrNote::noteComplementsAsString () const
{
  std::stringstream ss;

// 	S_msrVoice
// 		noteVoiceUpLink =
// 			fetchNoteUpLinkToVoice ();
//
// 	S_msrStaff
// 		noteStaffUpLink;
//
//   if (noteVoiceUpLink) {
//     noteStaffUpLink =
//       noteVoiceUpLink->
//         getVoiceUpLinkToStaff ();
//   }
//
//   ss <<
//     "noteStaffUpLink: ";
//   if (noteStaffUpLink) {
//     ss <<
//       noteStaffUpLink->getStaffPathLikeName ();
//   }
//   else {
//     ss << "[NULL]";
//   }
//
//   ss <<
//     ", noteVoiceUpLink: ";
//   if (noteVoiceUpLink) {
//     ss <<
//       noteVoiceUpLink->getVoiceName ();
//   }
//   else {
//     ss << "[NULL]";
//   }

  ss <<
    ", measureElementMeasureNumber: ";
  if (fMeasureElementUpLinkToMeasure) {
    std::string
      measureNumber =
        fMeasureElementUpLinkToMeasure->
          getMeasureNumber ();

    if (measureNumber == K_MEASURE_NUMBER_UNKNOWN_) {
      ss << "[UNKNOWN_MEASURE_NUMBER]";
    }
    else {
      ss << measureNumber;
    }
  }
  else {
    ss << "[NULL]";
  }

  return ss.str ();
}

std::string msrNote::noteCoreAndComplementAsString () const
{
  std::stringstream ss;

  ss <<
    noteCoreAsString () <<
    ", " <<
    noteComplementsAsString ();

  return ss.str ();
}

std::string msrNote::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[Note" <<
    ", fNoteKind: " << fNoteKind <<
    ", ";

  switch (fNoteKind) {
    case msrNoteKind::kNote_UNKNOWN_:
      ss <<
      	noteCoreAsString ();
      break;

    case msrNoteKind::kNoteRestInMeasure:
      ss <<
      	noteCoreAsString ();

      if (fNoteOccupiesAFullMeasure) {
        ss <<
          'R' <<
          /* JMI
          multipleMeasureRestsWholeNotesPitchAndOctaveAsString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes);
            */
          noteCoreAndComplementAsString ();
      }
      else {
        ss <<
      		noteCoreAsString () <<
          'r' << " %{ FOO !fNoteOccupiesAFullMeasure %} " << // JMI 0.9.70
          noteCoreAndComplementAsString ();
      }
			ss << ", ";

//       ss <<
//         noteCoreAndComplementAsString ();
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      ss <<
        noteCoreAndComplementAsString ();
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      ss <<
        noteCoreAndComplementAsString ();
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      ss <<
        noteCoreAndComplementAsString ();
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      ss <<
        noteCoreAndComplementAsString ();
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      ss <<
        noteCoreAndComplementAsString ();
      break;

    case msrNoteKind::kNoteRestInGraceNotesGroup:
      ss <<
        noteCoreAndComplementAsString ();
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      ss <<
        noteCoreAndComplementAsString ();
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      ss <<
        noteCoreAndComplementAsString () <<
        ", fNoteTupletFactor: " << fNoteTupletFactor.asShortString ();
      break;

    case msrNoteKind::kNoteRegularInChord:
      ss <<
        noteCoreAndComplementAsString ();
      break;

    case msrNoteKind::kNoteRegularInTuplet:
      ss <<
        noteCoreAndComplementAsString ();
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesPitchAndOctaveAsString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ())
            */
      break;

    case msrNoteKind::kNoteRestInTuplet:
      ss <<
        noteCoreAndComplementAsString ();
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      ss <<
        noteCoreAndComplementAsString () <<
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesPitchAndOctaveAsString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ())
            */
        ", fNoteTupletFactor: " << fNoteTupletFactor.asShortString ();
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      ss <<
        noteCoreAndComplementAsString () <<
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesPitchAndOctaveAsString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ())
            */
        ", fNoteTupletFactor: " << fNoteTupletFactor.asShortString ();
      break;
  } // switch

/* JMI
  ss << std::left <<
    ", positionInMeasure: ";
    / * JMI
  if (fMeasureElementPositionInMeasure == K_POSITION_IN_MEASURE_UNKNOWN__MEASURE_NUMBER) {
    ss << "unknown (" << fMeasureElementPositionInMeasure.asString () << ")";
  }
  else {
    ss << fMeasureElementPositionInMeasure;
  }
  * /
  ss << fMeasureElementPositionInMeasure;

  ss <<
    ", voicePosition: " <<
    fMeasureElementVoicePosition;
*/

  if (fNoteOccupiesAFullMeasure) {
    ss <<
      ", fNoteOccupiesAFullMeasure";
  }

  if (fNoteTrillOrnament) {
    ss <<
      ", fNoteTrillOrnament";
  }

  if (fNoteDashesOrnament) {
    ss <<
      ", fNoteDashesOrnament";
  }

  if (fNoteWavyLineSpannerStart) {
    ss <<
      ", fNoteWavyLineSpannerStart";
  }

  if (fNoteWavyLineSpannerStop) {
    ss <<
      ", fNoteWavyLineSpannerStop";
  }

  if (fNoteIsFollowedByGraceNotesGroup) {
    ss <<
      ", fNoteIsFollowedByGraceNotesGroup";
  }

  if (fNoteTiesList.size ()) {
		ss <<
			", fNoteTiesList:";

		++gIndenter;

// 		for (S_msrTie noteTie : fNoteTiesList) {
// 			ss <<
// 				", " << noteTie->getTieKind ();
// 		} // for

		for (
			std::list <S_msrTie>::const_iterator i = fNoteTiesList.begin ();
			i != fNoteTiesList.end ();
			++i
		) {
			S_msrTie noteTie = (*i);

			ss <<
				noteTie->getTieKind ();

			if (i != std::prev (fNoteTiesList.end ())) {
				ss <<
					", ";
			}
		} // for

		--gIndenter;
  }

  ss <<
    ", " << fNoteIsACueNoteKind;

  ss <<
    ", " <<
    mfInputLineNumberAsString (
    	fInputLineNumber) <<
    ']';

  return ss.str ();
}

std::string msrNote::asHeaderLessString () const
{
  std::stringstream ss;

	ss <<
		"fNoteKind: ";

  switch (fNoteKind) {
    case msrNoteKind::kNote_UNKNOWN_:
			ss <<
      	noteCoreAsString ();
      break;

    case msrNoteKind::kNoteRestInMeasure:
      ss <<
      	noteCoreAsString ();
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      ss <<
      	noteCoreAsString ();
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      ss <<
      	noteCoreAsString ();
      break;

    case msrNoteKind::kNoteRegularInMeasure:
			ss <<
				noteCoreAsString ();
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      ss <<
      	noteCoreAsString ();
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      ss <<
      	noteCoreAsString ();
      break;

    case msrNoteKind::kNoteRestInGraceNotesGroup:
      ss <<
      	noteCoreAsString ();
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      ss <<
      	noteCoreAsString ();
      break;

   case msrNoteKind::kNoteInChordInGraceNotesGroup:
      ss <<
      	noteCoreAsString () <<
        ", fNoteGraphicNotesDurationKind: " <<
        fNoteGraphicNotesDurationKind;
      break;

    case msrNoteKind::kNoteRegularInChord:
      ss <<
      	noteCoreAsString ();
      break;

    case msrNoteKind::kNoteRegularInTuplet:
      ss <<
      	noteCoreAsString () <<
        ", fNoteGraphicNotesDurationKind: " <<
        fNoteGraphicNotesDurationKind <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes.asFractionString () <<
        /* JMI 0.9.67
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesPitchAndOctaveAsString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ());
            */
        ", fNoteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteRestInTuplet:
      ss <<
      	noteCoreAsString () <<
        ", fNoteGraphicNotesDurationKind: " <<
        fNoteGraphicNotesDurationKind <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes.asFractionString () <<
        /* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesPitchAndOctaveAsString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ());
            */
        ", fNoteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      ss <<
      	noteCoreAsString () <<
        ", fNoteGraphicNotesDurationKind: " <<
        fNoteGraphicNotesDurationKind <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes.asFractionString () <<
        /* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesPitchAndOctaveAsString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ());
            */
        ", fNoteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      ss <<
      	noteCoreAsString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes.asFractionString () <<
        /* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesPitchAndOctaveAsString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ());
            */
        ", fNoteTupletFactor " << fNoteTupletFactor.asString ();
      break;
  } // switch

  ss <<
    ", " <<
    mfInputLineNumberAsString (
    	fInputLineNumber);

  return ss.str ();
}

std::string msrNote::asString () const
{
  std::stringstream ss;

  ss <<
    "[Note, " <<
    asHeaderLessString () <<
    ']';

  return ss.str ();
}

void msrNote::print (std::ostream& os) const
{
// 	os << "======> msrNote::print ()" << std::endl;

  os <<
    "[Note, " <<
//     ", " <<
//     mfInputLineNumberAsString (
//     	fInputLineNumber) <<
//     ", " <<
//     noteForPrintAsString () <<
    asHeaderLessString () <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 34;

  // print measure position and sounding whole notes
  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureElementSoundingWholeNotes" << ": " <<
    fMeasureElementSoundingWholeNotes.asFractionString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteDisplayWholeNotes" << ": " <<
    fNoteDisplayWholeNotes.asFractionString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureElementPositionInMeasure" << ": " <<
    fMeasureElementPositionInMeasure.asString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteOccupiesAFullMeasure" << ": " <<
    fNoteOccupiesAFullMeasure <<
    std::endl <<

    std::setw (fieldWidth) <<
		"fNoteIsACueNoteKind" << ": " <<
		fNoteIsACueNoteKind <<
    std::endl;

  // print position from beginning of voice
//   os << std::left <<
//     std::setw (fieldWidth) <<
//     "fMeasureElementVoicePosition" << ": " <<
//     fMeasureElementVoicePosition <<
//     std::endl;

  // print sounding and displayed whole notes
  switch (fNoteKind) {
    case msrNoteKind::kNote_UNKNOWN_:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteUnpitchedInMeasure:
    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteInDoubleTremolo:
    case msrNoteKind::kNoteRegularInChord:
      os << std::left <<
        std::setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << ": " <<
        fNoteDisplayWholeNotes.asFractionString () <<
        std::endl;
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteRestInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      os <<
        std::setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << ": " <<
        fNoteDisplayWholeNotes.asFractionString () <<
        std::endl;
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      os << std::left <<
        std::setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << ": " <<
        fNoteDisplayWholeNotes.asFractionString () <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fNoteGraphicNotesDurationKind" << ": " <<
        fNoteGraphicNotesDurationKind <<
        std::endl;
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      os <<
        std::setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << ": " <<
        fNoteDisplayWholeNotes.asFractionString () <<
        std::endl <<
        std::setw (fieldWidth) <<
        "tupletSoundingWholeNotes" << ": ";

        if (fNoteShortcutUpLinkToTuplet) {
          os <<
            wholeNotesPitchAndOctaveAsString (
              fInputLineNumber,
              getNoteShortcutUpLinkToTuplet ()->
                getMeasureElementSoundingWholeNotes ());
        }
        else {
          os <<
            "*** unknown yet ***";
        }
        os << std::endl;

      os <<
        std::setw (fieldWidth) <<
        "fNoteTupletFactor" << ": RAH" <<
        std::endl;

      ++gIndenter;
        os <<
          fNoteTupletFactor;
      --gIndenter;
      break;
    } // switch

  // print the octave shift if any
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteOctaveShift" << ": ";
  if (fNoteOctaveShift) {
    os << std::endl;
    ++gIndenter;
    os <<
      fNoteOctaveShift;
    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

	// is the note a grace note?
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteIsAGraceNote" << ": " <<
    fNoteIsAGraceNote <<
    std::endl;

	// is the note a cue note?
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteIsACueNote" << ": " <<
    fNoteIsACueNote <<
    std::endl;

	// does the note belong to a chord?
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteBelongsToAChord" << ": " <<
    fNoteBelongsToAChord <<
    std::endl;

	// does the note belong to a tuplet?
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteBelongsToATuplet" << ": " <<
    fNoteBelongsToATuplet <<
    std::endl;

	// print the uplinks
	S_msrVoice
		noteVoiceUpLink =
			fetchNoteUpLinkToVoice ();

	S_msrStaff
		noteStaffUpLink;

	if (noteVoiceUpLink) {
		noteStaffUpLink =
			noteVoiceUpLink->
				getVoiceUpLinkToStaff ();
	}

  os << std::left <<
    std::setw (fieldWidth) <<
    "noteStaffUpLink" << ": ";
	if (noteStaffUpLink) {
		os <<
			noteStaffUpLink->getStaffPathLikeName ();
	}
	else {
		os << "[NULL]";
	}
	os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "noteVoiceUpLink" << ": ";
	if (noteVoiceUpLink) {
		os <<
			noteVoiceUpLink->getVoiceName ();
	}
	else {
		os << "[NULL]";
	}
	os << std::endl;

  // print the stem if any
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteStem" << ": ";
  if (fNoteStem) {
    os << std::endl;
    ++gIndenter;

    os <<
      fNoteStem;

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  // print the grace notes group before note if any
  if (fGraceNotesGroupBeforeNote) {
		os << std::left <<
    	std::endl <<
      std::setw (fieldWidth) <<
      "fGraceNotesGroupBeforeNote" << ": " <<
      std::endl;

    ++gIndenter;

    os <<
    	fGraceNotesGroupBeforeNote;

    --gIndenter;
  }

  // print the ties if any
  if (fNoteTiesList.size ()) {
		os << std::left <<
			std::setw (fieldWidth) <<
			"fNoteTiesList:" << ": " <<
			std::endl;

		++gIndenter;

		for (S_msrTie noteTie : fNoteTiesList) {
			noteTie->print (os);
		} // for

		--gIndenter;
  }

  // print the beams if any
  if (fNoteBeamsList.size ()) {
		os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteBeamsList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrBeam>::const_iterator
      iBegin = fNoteBeamsList.begin (),
      iEnd   = fNoteBeamsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the articulations if any
  if (fNoteArticulationsList.size ()) {
		os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteArticulationsList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrArticulation>::const_iterator
      iBegin = fNoteArticulationsList.begin (),
      iEnd   = fNoteArticulationsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the spanners if any
  if (fNoteSpannersList.size ()) {
		os << std::left <<
			std::setw (fieldWidth) <<
			"fNoteSpannersList" << ": " <<
			std::endl;

		++gIndenter;

		std::list <S_msrSpanner>::const_iterator
			iBegin = fNoteSpannersList.begin (),
			iEnd   = fNoteSpannersList.end (),
			i      = iBegin;
		for ( ; ; ) {
			(*i)->print (os);
			if (++i == iEnd) break;
			// no std::endl here;
		} // for

		--gIndenter;
  }

  // print the technicals if any
  if (fNoteTechnicalsList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteTechnicalsList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrTechnical>::const_iterator
      iBegin = fNoteTechnicalsList.begin (),
      iEnd   = fNoteTechnicalsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the technicals with integer if any
  if (fNoteTechnicalWithIntegersList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteTechnicalWithIntegersList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrTechnicalWithInteger>::const_iterator
      iBegin = fNoteTechnicalWithIntegersList.begin (),
      iEnd   = fNoteTechnicalWithIntegersList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the technicals with float if any
  if (fNoteTechnicalWithFloatsList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteTechnicalWithFloatsList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrTechnicalWithFloat>::const_iterator
      iBegin = fNoteTechnicalWithFloatsList.begin (),
      iEnd   = fNoteTechnicalWithFloatsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the technicals with string if any
  if (fNoteTechnicalWithStringsList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteTechnicalWithStringsList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrTechnicalWithString>::const_iterator
      iBegin = fNoteTechnicalWithStringsList.begin (),
      iEnd   = fNoteTechnicalWithStringsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the ornaments if any
  if (fNoteOrnamentsList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteOrnamentsList:" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrOrnament>::const_iterator
      iBegin = fNoteOrnamentsList.begin (),
      iEnd   = fNoteOrnamentsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the glissandos if any
  if (fNoteGlissandosList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteGlissandosList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrGlissando>::const_iterator
      iBegin = fNoteGlissandosList.begin (),
      iEnd   = fNoteGlissandosList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the slides if any
  if (fNoteSlidesList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteSlidesList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrSlide>::const_iterator
      iBegin = fNoteSlidesList.begin (),
      iEnd   = fNoteSlidesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the singleTremolo if any
  if (fNoteSingleTremolo) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteSingleTremolo" << ": " <<
      std::endl;

    ++gIndenter;

    os << fNoteSingleTremolo;

    --gIndenter;
  }

  // print the dynamics if any
  if (fNoteDynamicsList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteDynamicsList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrDynamic>::const_iterator
      iBegin = fNoteDynamicsList.begin (),
      iEnd   = fNoteDynamicsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the other dynamics if any
  if (fNoteOtherDynamicsList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteOtherDynamicsList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrOtherDynamic>::const_iterator
      iBegin = fNoteOtherDynamicsList.begin (),
      iEnd   = fNoteOtherDynamicsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the words if any
  if (fNoteWordsList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteWordsList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrWords>::const_iterator
      iBegin = fNoteWordsList.begin (),
      iEnd   = fNoteWordsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the slurs if any
  if (fNoteSlursList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteSlursList" << ": " <<
      std::endl;

    ++gIndenter;

    for (S_msrSlur slur : fNoteSlursList) {
      slur->print (os);
    } // for

    --gIndenter;
  }

  // print the ligatures if any
  if (fNoteLigaturesList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteLigaturesList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrLigature>::const_iterator
      iBegin = fNoteLigaturesList.begin (),
      iEnd   = fNoteLigaturesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the pedals if any
  if (fNotePedalsList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNotePedalsList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrPedal>::const_iterator
      iBegin = fNotePedalsList.begin (),
      iEnd   = fNotePedalsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the slashes if any
  if (fNoteSlashesList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteSlashesList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrSlash>::const_iterator
      iBegin = fNoteSlashesList.begin (),
      iEnd   = fNoteSlashesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the crescDecresc if any
  if (fNoteCrescDecrescsList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteCrescDecrescsList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrCrescDecresc>::const_iterator
      iBegin = fNoteCrescDecrescsList.begin (),
      iEnd   = fNoteCrescDecrescsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the wedges if any
  if (fNoteWedgesList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteWedgesList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrWedge>::const_iterator
      iBegin = fNoteWedgesList.begin (),
      iEnd   = fNoteWedgesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the segnos if any
  if (fNoteSegnosList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteSegnosList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrSegno>::const_iterator
      iBegin = fNoteSegnosList.begin (),
      iEnd   = fNoteSegnosList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the dal segnos if any
  if (fNoteDalSegnosList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteDalSegnosList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrDalSegno>::const_iterator
      iBegin = fNoteDalSegnosList.begin (),
      iEnd   = fNoteDalSegnosList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the codas if any
  if (fNoteCodasList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteCodasList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrCoda>::const_iterator
      iBegin = fNoteCodasList.begin (),
      iEnd   = fNoteCodasList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the eyeglasses if any
  if (fNoteEyeGlassesList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteEyeGlassesList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrEyeGlasses>::const_iterator
      iBegin = fNoteEyeGlassesList.begin (),
      iEnd   = fNoteEyeGlassesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the damps if any
  if (fNoteDampsList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteDampsList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrDamp>::const_iterator
      iBegin = fNoteDampsList.begin (),
      iEnd   = fNoteDampsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the dampAlls if any
  if (fNoteDampAllsList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteDampAllsList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrDampAll>::const_iterator
      iBegin = fNoteDampAllsList.begin (),
      iEnd   = fNoteDampAllsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the scordaturas if any
  if (fNoteScordaturasList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteScordaturasList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrScordatura>::const_iterator
      iBegin = fNoteScordaturasList.begin (),
      iEnd   = fNoteScordaturasList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->print (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

  // print the harmonies associated to this note if any
  if (fNoteHarmoniesList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteHarmoniesList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrHarmony>::const_iterator
      iBegin = fNoteHarmoniesList.begin (),
      iEnd   = fNoteHarmoniesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrHarmony
        harmony = (*i);

      os << harmony;

      if (++i == iEnd) break;
      // no std::endl here
    } // for

    --gIndenter;
  }

  // print the figured bass elements associated to this note if any
  if (fNoteFiguredBassesList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteFiguredBassesList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrFiguredBass>::const_iterator
      iBegin = fNoteFiguredBassesList.begin (),
      iEnd   = fNoteFiguredBassesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrFiguredBass
        figuredBass = (*i);

      os << figuredBass;

      if (++i == iEnd) break;
      // no std::endl here
    } // for

    --gIndenter;
  }

  // print the syllables associated to this note if any
  if (fNoteSyllablesList.size ()) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteSyllablesList" << ": " <<
      std::endl;

    ++gIndenter;

    std::list <S_msrSyllable>::const_iterator
      iBegin = fNoteSyllablesList.begin (),
      iEnd   = fNoteSyllablesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrSyllable
        syllable = (*i);

      os << syllable;

      if (++i == iEnd) break;
      // no std::endl here
    } // for

    --gIndenter;
  }

  // print the grace group notes after note if any
  if (fGraceNotesGroupAfterNote) {
    os <<
    	std::endl <<
      std::setw (fieldWidth) <<
      "fGraceNotesGroupAfterNote" << ": " <<
      std::endl;

    ++gIndenter;

    fGraceNotesGroupAfterNote->print (os);

    --gIndenter;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrNote::printFull (std::ostream& os) const
{
  os <<
    "[Note, " <<
//     ", " <<
//     mfInputLineNumberAsString (
//     	fInputLineNumber) <<
//     ", " <<
//     noteForPrintAsString () <<
    asHeaderLessString () <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 44;

  // print measure position and sounding whole notes
  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureElementSoundingWholeNotes" << ": " <<
    fMeasureElementSoundingWholeNotes.asFractionString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteDisplayWholeNotes" << ": " <<
    fNoteDisplayWholeNotes.asFractionString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureElementPositionInMeasure" << ": " <<
    fMeasureElementPositionInMeasure.asString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fNoteOccupiesAFullMeasure" << ": " <<
    fNoteOccupiesAFullMeasure <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteIsACueNoteKind" << ": " <<
    fNoteIsACueNoteKind <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteOctaveShift" << ": ";
  if (fNoteOctaveShift) {
    os << std::endl;
    ++gIndenter;

    os <<
      fNoteOctaveShift;

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNotePrintObjectKind" << ": " <<
    fNotePrintObjectKind <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteHeadKind" << ": " <<
    fNoteHeadKind <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteHeadFilledKind" << ": " <<
    fNoteHeadFilledKind <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteHeadParenthesesKind" << ": " <<
    fNoteHeadParenthesesKind <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteAccidentalKind" << ": " <<
    fNoteAccidentalKind <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteEditorialAccidentalKind" << ": " <<
    fNoteEditorialAccidentalKind <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteCautionaryAccidentalKind" << ": " <<
    fNoteCautionaryAccidentalKind <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteBelongsToAChord" << ": " <<
    fNoteBelongsToAChord <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteBelongsToATuplet" << ": " <<
    fNoteBelongsToATuplet <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteBelongsToAMultipleMeasureRests" << ": " <<
    fNoteBelongsToAMultipleMeasureRests <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteMultipleMeasureRestsSequenceNumber" << ": " <<
    fNoteMultipleMeasureRestsSequenceNumber <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteIsStemless" << ": " <<
    fNoteIsStemless <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteIsAChordsFirstMemberNote" << ": " <<
    fNoteIsAChordsFirstMemberNote <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteIsFirstNoteInADoubleTremolo" << ": " <<
    fNoteIsFirstNoteInADoubleTremolo <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteIsSecondNoteInADoubleTremolo" << ": " <<
    fNoteIsSecondNoteInADoubleTremolo <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteTrillOrnament" << ": ";
  if (fNoteTrillOrnament) {
    os << std::endl;
    ++gIndenter;
		fNoteTrillOrnament->printFull (os);
    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

	// print the uplinks
	S_msrVoice
		noteVoiceUpLink =
			fetchNoteUpLinkToVoice ();

	S_msrStaff
		noteStaffUpLink;

	if (noteVoiceUpLink) {
		noteStaffUpLink =
			noteVoiceUpLink->
				getVoiceUpLinkToStaff ();
	}

  os << std::left <<
    std::setw (fieldWidth) <<
    "noteStaffUpLink" << ": ";
	if (noteStaffUpLink) {
		os <<
			noteStaffUpLink->getStaffPathLikeName ();
	}
	else {
		os << "[NULL]";
	}
	os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "noteVoiceUpLink" << ": ";
	if (noteVoiceUpLink) {
		os <<
			noteVoiceUpLink->getVoiceName ();
	}
	else {
		os << "[NULL]";
	}
	os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteIsFollowedByGraceNotesGroup" << ": " <<
    fNoteIsFollowedByGraceNotesGroup <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteColorAlphaRGB" << ": " <<
    fNoteColorAlphaRGB <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteColorAlphaRGBHasBenSet" << ": " <<
    fNoteColorAlphaRGBHasBenSet <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteSoloNoteOrRestInVoiceKind" << ": " <<
    fNoteSoloNoteOrRestInVoiceKind <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteSoloNoteOrRestInStaffKind" << ": " <<
    fNoteSoloNoteOrRestInStaffKind <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fNoteIsACueNoteKind" << ": " <<
    fNoteIsACueNoteKind <<
    std::endl;

//  ++gIndenter;

  // print measure number
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteUpLinkToMeasureMeasureNumber" << ": ";
  if (fMeasureElementUpLinkToMeasure) {
    std::string
      measureNumber =
        fMeasureElementUpLinkToMeasure->
          getMeasureNumber ();

    if (measureNumber == K_MEASURE_NUMBER_UNKNOWN_) {
      os << "[UNKNOWN_MEASURE_NUMBER]";
    }
    else {
      os << measureNumber;
    }
  }
  else {
    os << "[NULL]";
  }


//   if (fMeasureElementUpLinkToMeasure->getMeasureNumber () == K_MEASURE_NUMBER_UNKNOWN_) {
//     os <<
//       "[UNKNOWN_MEASURE_NUMBER]";
//   }
//   else if (fMeasureElementUpLinkToMeasure->getMeasureNumber ().size ()) {
//     os <<
//       fMeasureElementUpLinkToMeasure->getMeasureNumber ();
//   }
//   else {
//     std::stringstream ss;
//
//     ss <<
//       "fMeasureElementUpLinkToMeasure->getMeasureNumber () is empty in note " <<
//       this->asString ();
//
// // JMI     msrInternalError (
//     msrInternalWarning (
//       gServiceRunData->getInputSourceName (),
//       fInputLineNumber,
//       ss.str ());
//   }
  os << std::endl;

  // print position from beginning of voice
//   os << std::left <<
//     std::setw (fieldWidth) <<
//     "fMeasureElementVoicePosition" << ": " <<
//     fMeasureElementVoicePosition <<
//     std::endl <<
//     std::setw (fieldWidth) <<
//     "fVoiceMoment" << ": " <<
//     std::endl;
//   ++gIndenter;
//   os <<
//     fVoiceMoment;
//   --gIndenter;

  // print note uplink to measure
  os <<
    std::setw (fieldWidth) <<
    "fMeasureElementUpLinkToMeasure" << ": ";

  if (fMeasureElementUpLinkToMeasure) {
    os << std::endl;

    ++gIndenter;

    os <<
      fMeasureElementUpLinkToMeasure->asShortString () <<
      std::endl;

    --gIndenter;
  }
  else {
    os <<
      "[NULL]";
  }
  os << std::endl;

  // print note uplink to chord
  os <<
    std::setw (fieldWidth) <<
    "fNoteShortcutUpLinkToChord" << ": ";
  if (fNoteShortcutUpLinkToChord) {
    os <<
      fNoteShortcutUpLinkToChord->asShortString ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  // print note uplink to grace notes group
  os <<
    std::setw (fieldWidth) <<
    "fNoteShortcutUpLinkToGraceNotesGroup" << ": ";
  if (fNoteShortcutUpLinkToGraceNotesGroup) {
    os << std::endl;
    ++gIndenter;
    os <<
      fNoteShortcutUpLinkToGraceNotesGroup->asShortString (); // avoid loop
    --gIndenter;
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  // print note uplink to tuplet
  os <<
    std::setw (fieldWidth) <<
    "fNoteShortcutUpLinkToTuplet" << ": ";

  if (fNoteShortcutUpLinkToTuplet) {
    os <<
      fNoteShortcutUpLinkToTuplet->asShortString ();
  }
  else {
    os <<
      ": " << "[NULL]";
  }
  os << std::endl;

  // print sounding and displayed whole notes
  switch (fNoteKind) {
    case msrNoteKind::kNote_UNKNOWN_:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInMeasure:
    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteInDoubleTremolo:
    case msrNoteKind::kNoteRegularInChord:
      os << std::left <<
        std::setw (fieldWidth) <<
        "fMeasureElementSoundingWholeNotes" << ": " <<
        fMeasureElementSoundingWholeNotes.asFractionString () <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << ": " <<
        fNoteDisplayWholeNotes.asFractionString () <<
        std::endl;
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteRestInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      os <<
        std::setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << ": " <<
        fNoteDisplayWholeNotes.asFractionString () <<
        std::endl;
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      os <<
        std::setw (fieldWidth) <<
        "fMeasureElementSoundingWholeNotes" << ": " <<
        fMeasureElementSoundingWholeNotes.asFractionString () <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << ": " <<
        fNoteDisplayWholeNotes.asFractionString () <<
        std::endl <<
        std::setw (fieldWidth) <<
        "tupletSoundingWholeNotes" << ": ";

        if (fNoteShortcutUpLinkToTuplet) {
          os <<
            wholeNotesPitchAndOctaveAsString (
              fInputLineNumber,
              getNoteShortcutUpLinkToTuplet ()->
                getMeasureElementSoundingWholeNotes ());
        }
        else {
          os <<
            "*** unknown yet ***";
        }
        os << std::endl;

      os <<
        std::setw (fieldWidth) <<
        "fNoteTupletFactor" << ": RUH" <<
        std::endl;

      ++gIndenter;
        os <<
          fNoteTupletFactor;
      --gIndenter;
        break;
  } // switch

  // print note full measure length
  // may be unknown if there is no time signature
  mfWholeNotes
    measureFullLength =
      fMeasureElementUpLinkToMeasure
        ?
          fMeasureElementUpLinkToMeasure->
            getFullMeasureWholeNotesDuration ()
        : K_WHOLE_NOTES_ZERO; // JMI 0.9.67

  os << std::left <<
    std::setw (fieldWidth) <<
    "measureFullLength" << ": ";
  if (measureFullLength.getNumerator () == 0) {
    os <<
      "unknown, there's no time signature";
  }
  else {
    os <<
      measureFullLength;
  }
  os << std::endl;

	// grace note?
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteIsAGraceNote" << ": " <<
    fNoteIsAGraceNote <<
    std::endl;

	// cue note?
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteIsACueNote" << ": " <<
    fNoteIsACueNote <<
    std::endl;

  // chord member?
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteBelongsToAChord" << ": " <<
    fNoteBelongsToAChord <<
    std::endl;

  // tuplet member?
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteBelongsToATuplet" << ": " <<
    fNoteBelongsToATuplet <<
    std::endl;

  // multiple rest member?
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteBelongsToAMultipleMeasureRests" << ": " <<
    fNoteBelongsToAMultipleMeasureRests <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteMultipleMeasureRestsSequenceNumber" << ": " <<
    fNoteMultipleMeasureRestsSequenceNumber <<
    std::endl;

  // print the grace notes group before note if any
  if (fGraceNotesGroupBeforeNote) {
    os <<
    	std::endl <<
      std::setw (fieldWidth) <<
      "fGraceNotesGroupBeforeNote";
    if (fGraceNotesGroupBeforeNote) {
      os << std::endl;

      ++gIndenter;

      fGraceNotesGroupBeforeNote->printFull (os);

      --gIndenter;
    }
    else {
      os << ": " <<
        "[NULL]" <<
        std::endl;
    }
  }

  // note head
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteHeadKind" << ": " <<
    fNoteHeadKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteHeadFilledKind" << ": " <<
    fNoteHeadFilledKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteHeadParenthesesKind" << ": " <<
    fNoteHeadParenthesesKind <<
    std::endl;

  // accidentals
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteAccidentalKind" << ": " <<
    fNoteAccidentalKind <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteEditorialAccidentalKind" << ": " <<
    fNoteEditorialAccidentalKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteCautionaryAccidentalKind" << ": " <<
    fNoteCautionaryAccidentalKind <<
    std::endl;

  // grace note?
  os << std::left <<
    std::setw (fieldWidth) <<
    "getNoteIsAGraceNote" << ": " <<
    getNoteIsAGraceNote () <<
    std::endl;

  // cue note?
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteIsACueNoteKind" << ": " <<
    fNoteIsACueNoteKind <<
    std::endl;

  // note redundant information (for speed)

  std::stringstream ss;

// JMI  if (fNoteIsStemless) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteIsStemless" <<
      ": " <<
      fNoteIsStemless <<
      std::endl;
// JMI  }

 // JMI if (fNoteIsAChordsFirstMemberNote) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteIsAChordsFirstMemberNote" <<
      ": " <<
      fNoteIsAChordsFirstMemberNote <<
      std::endl;
 // JMI }

 // JMI if (fNoteIsFirstNoteInADoubleTremolo) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteIsFirstNoteInADoubleTremolo" <<
      ": " <<
      fNoteIsFirstNoteInADoubleTremolo <<
      std::endl;
// JMI  }
 // JMI if (fNoteIsSecondNoteInADoubleTremolo) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteIsSecondNoteInADoubleTremolo" <<
      ": " <<
      fNoteIsSecondNoteInADoubleTremolo <<
      std::endl;
// JMI  }

  os <<
    std::setw (fieldWidth) <<
    "fNoteDashesOrnament" << ": ";
  if (fNoteDashesOrnament) {
    os << std::endl;
    ++gIndenter;

		fNoteDashesOrnament->printFull (os);

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  os <<
    std::setw (fieldWidth) <<
    "fNoteDelayedTurnOrnament" << ": ";
  if (fNoteDelayedTurnOrnament) {
    os << std::endl;
    ++gIndenter;

		fNoteDelayedTurnOrnament->printFull (os);

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  os <<
    std::setw (fieldWidth) <<
    "fNoteDelayedInvertedTurnOrnament" << ": ";
  if (fNoteDelayedInvertedTurnOrnament) {
    os << std::endl;
    ++gIndenter;

		fNoteDelayedInvertedTurnOrnament->printFull (os);

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  os <<
    std::setw (fieldWidth) <<
    "fNoteWavyLineSpannerStart" << ": ";
  if (fNoteWavyLineSpannerStart) {
    os << std::endl;
    ++gIndenter;

		fNoteWavyLineSpannerStart->printFull (os);

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  os <<
    std::setw (fieldWidth) <<
    "fNoteWavyLineSpannerStop" << ": ";
  if (fNoteWavyLineSpannerStop) {
    os << std::endl;
    ++gIndenter;

		fNoteWavyLineSpannerStop->printFull (os);

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  if (fNoteIsFollowedByGraceNotesGroup) {
    os << std::left <<
      std::setw (fieldWidth) <<
      "fNoteIsFollowedByGraceNotesGroup" <<
      ": " <<
      fNoteIsFollowedByGraceNotesGroup <<
      std::endl;
  }

  // print whole notes durations as MSR strings
  switch (fNoteKind) {
    case msrNoteKind::kNote_UNKNOWN_:
      break;

    case msrNoteKind::kNoteRestInMeasure:
      os << std::left <<
        std::setw (fieldWidth) <<
        "noteSoundingWholeNotesPitchAndOctaveAsString" << ": \"" <<
        noteSoundingWholeNotesPitchAndOctaveAsString () <<
        "\"" <<
        std::endl;
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      os << std::left <<
        std::setw (fieldWidth) <<
        "noteSoundingWholeNotesPitchAndOctaveAsString" << ": \"" <<
        noteSoundingWholeNotesPitchAndOctaveAsString () <<
        "\"" <<
        std::endl;
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      os << std::left <<
        std::setw (fieldWidth) <<
        "noteSoundingWholeNotesPitchAndOctaveAsString" << ": \"" <<
        noteSoundingWholeNotesPitchAndOctaveAsString () <<
        "\"" <<
        std::endl <<
        std::setw (fieldWidth) <<
        "noteDisplayWholeNotesPitchAndOctaveAsString" << ": \"" <<
        noteDisplayWholeNotesPitchAndOctaveAsString () <<
        "\"" <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fNoteGraphicNotesDurationKind" << ": " <<
        fNoteGraphicNotesDurationKind <<
        std::endl <<
        std::setw (fieldWidth) <<
        "noteGraphicNotesDurationAsMusicXMLString" << ": \"" <<
        fNoteGraphicNotesDurationKind <<
        "\"" <<
        std::endl;
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      os << std::left <<
        std::setw (fieldWidth) <<
        "noteSoundingWholeNotesPitchAndOctaveAsString" << ": \"" <<
        noteSoundingWholeNotesPitchAndOctaveAsString () <<
        "\"" <<
        std::endl <<
        std::setw (fieldWidth) <<
        "noteDisplayWholeNotesPitchAndOctaveAsString" << ": \"" <<
        noteDisplayWholeNotesPitchAndOctaveAsString () <<
        "\"" <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fNoteGraphicNotesDurationKind" << ": " <<
        fNoteGraphicNotesDurationKind <<
        std::endl;
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      // JMI
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteRestInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      os << std::left <<
        std::setw (fieldWidth) <<
        "fNoteGraphicNotesDurationKind" << ": " <<
        fNoteGraphicNotesDurationKind <<
        std::endl;
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      os << std::left <<
        std::setw (fieldWidth) <<
        "fMeasureElementSoundingWholeNotes" << ": " <<
        fMeasureElementSoundingWholeNotes.asFractionString () <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << ": " <<
        fNoteDisplayWholeNotes.asFractionString () <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fNoteGraphicNotesDurationKind" << ": " <<
        fNoteGraphicNotesDurationKind <<
        std::endl;
      break;

    case msrNoteKind::kNoteRegularInChord:
      // JMI
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      /* JMI 0.9.67
      os << std::left <<
        std::setw (fieldWidth) <<
        "fNoteTupletNoteGraphicNotesDurationpitchAndOctaveAsString" << ": \"" <<
        fNoteTupletNoteGraphicNotesDurationpitchAndOctaveAsString <<
        "\"" <<
        std::endl <<
        std::setw (fieldWidth) <<
        "noteTupletnoteSoundingWholeNotesPitchAndOctaveAsString" << ": ";
          */

      if (fNoteShortcutUpLinkToTuplet) {
        os <<
          "\"" <<
          wholeNotesPitchAndOctaveAsString (
            fInputLineNumber,
            getNoteShortcutUpLinkToTuplet ()->
              getMeasureElementSoundingWholeNotes ()) <<
          "\"";
      }
      else {
        os <<
          "*** unknown yet ***";
      }
      os << std::endl;

      os <<
        std::setw (fieldWidth) <<
        "fPositionInTuplet" << ": " <<
        fPositionInTuplet <<
        std::endl;

      os <<
        std::setw (fieldWidth) <<
        "fNoteGraphicNotesDurationKind" << ": " <<
        fNoteGraphicNotesDurationKind <<
        std::endl;
      break;
  } // switch

  // print the octave shift if any
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteOctaveShift" << ": ";
  if (fNoteOctaveShift) {
    os << std::endl;
    ++gIndenter;
		fNoteOctaveShift->printFull (os);
    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  // print the stem if any
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteStem" << ": ";
  if (fNoteStem) {
    os << std::endl;
    ++gIndenter;

		fNoteStem->printFull (os);

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  // print the note color
  os <<
    std::setw (fieldWidth) <<
    "fNoteColorAlphaRGB" << ": " <<
    fNoteColorAlphaRGB.asString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteColorAlphaRGBHasBenSet" << ": " <<
    fNoteColorAlphaRGBHasBenSet <<
    std::endl;

  // solo note or rest?
  os <<
    std::setw (fieldWidth) <<
    "fNoteSoloNoteOrRestInVoiceKind" << ": " <<
    fNoteSoloNoteOrRestInVoiceKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteSoloNoteOrRestInStaffKind" << ": " <<
    fNoteSoloNoteOrRestInStaffKind <<
    std::endl;

  // print the ties if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteTiesList" << ": ";
  if (fNoteTiesList.size ()) {
    os << std::endl;

    ++gIndenter;

    for (S_msrTie noteTie : fNoteTiesList) {
			noteTie->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  // print the beams if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteBeamsList";
  if (fNoteBeamsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrBeam>::const_iterator
      iBegin = fNoteBeamsList.begin (),
      iEnd   = fNoteBeamsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the articulations if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteArticulationsList";
  if (fNoteArticulationsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrArticulation>::const_iterator
      iBegin = fNoteArticulationsList.begin (),
      iEnd   = fNoteArticulationsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the spanners if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteSpannersList";
  if (fNoteSpannersList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrSpanner>::const_iterator
      iBegin = fNoteSpannersList.begin (),
      iEnd   = fNoteSpannersList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the technicals if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteTechnicalsList";
  if (fNoteTechnicalsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrTechnical>::const_iterator
      iBegin = fNoteTechnicalsList.begin (),
      iEnd   = fNoteTechnicalsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the technicals with integer if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteTechnicalWithIntegersList";
  if (fNoteTechnicalWithIntegersList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrTechnicalWithInteger>::const_iterator
      iBegin = fNoteTechnicalWithIntegersList.begin (),
      iEnd   = fNoteTechnicalWithIntegersList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the technicals with float if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteTechnicalWithFloatsList";
  if (fNoteTechnicalWithFloatsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrTechnicalWithFloat>::const_iterator
      iBegin = fNoteTechnicalWithFloatsList.begin (),
      iEnd   = fNoteTechnicalWithFloatsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the technicals with string if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteTechnicalWithStringsList";
  if (fNoteTechnicalWithStringsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrTechnicalWithString>::const_iterator
      iBegin = fNoteTechnicalWithStringsList.begin (),
      iEnd   = fNoteTechnicalWithStringsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the ornaments if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteOrnamentsList";
  if (fNoteOrnamentsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrOrnament>::const_iterator
      iBegin = fNoteOrnamentsList.begin (),
      iEnd   = fNoteOrnamentsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the glissandos if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteGlissandosList";
  if (fNoteGlissandosList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrGlissando>::const_iterator
      iBegin = fNoteGlissandosList.begin (),
      iEnd   = fNoteGlissandosList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the slides if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteSlidesList";
  if (fNoteSlidesList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrSlide>::const_iterator
      iBegin = fNoteSlidesList.begin (),
      iEnd   = fNoteSlidesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the singleTremolo if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteSingleTremolo";
  if (fNoteSingleTremolo) {
    os << std::endl;

    ++gIndenter;

    fNoteSingleTremolo->printFull (os);

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NULL]" <<
      std::endl;
  }

  // print the dynamics if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteDynamicsList";
  if (fNoteDynamicsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrDynamic>::const_iterator
      iBegin = fNoteDynamicsList.begin (),
      iEnd   = fNoteDynamicsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the other dynamics if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteOtherDynamicsList";
  if (fNoteOtherDynamicsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrOtherDynamic>::const_iterator
      iBegin = fNoteOtherDynamicsList.begin (),
      iEnd   = fNoteOtherDynamicsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the words if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteWordsList";
  if (fNoteWordsList.size ()) {
    os << std::endl;
    ++gIndenter;

    std::list <S_msrWords>::const_iterator
      iBegin = fNoteWordsList.begin (),
      iEnd   = fNoteWordsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the slurs if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteSlursList";
  if (fNoteSlursList.size ()) {
    os << std::endl;

    ++gIndenter;

    for (S_msrSlur slur : fNoteSlursList) {
      slur->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the ligatures if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteLigaturesList";
  if (fNoteLigaturesList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrLigature>::const_iterator
      iBegin = fNoteLigaturesList.begin (),
      iEnd   = fNoteLigaturesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the pedals if any
  os <<
    std::setw (fieldWidth) <<
    "fNotePedalsList";
  if (fNotePedalsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrPedal>::const_iterator
      iBegin = fNotePedalsList.begin (),
      iEnd   = fNotePedalsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the slashes if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteSlashesList";
  if (fNoteSlashesList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrSlash>::const_iterator
      iBegin = fNoteSlashesList.begin (),
      iEnd   = fNoteSlashesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the crescDecresc if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteCrescDecrescsList";
  if (fNoteCrescDecrescsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrCrescDecresc>::const_iterator
      iBegin = fNoteCrescDecrescsList.begin (),
      iEnd   = fNoteCrescDecrescsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the wedges if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteWedgesList";
  if (fNoteWedgesList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrWedge>::const_iterator
      iBegin = fNoteWedgesList.begin (),
      iEnd   = fNoteWedgesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the segnos if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteSegnosList";
  if (fNoteSegnosList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrSegno>::const_iterator
      iBegin = fNoteSegnosList.begin (),
      iEnd   = fNoteSegnosList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the dal segnos if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteDalSegnosList";
  if (fNoteDalSegnosList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrDalSegno>::const_iterator
      iBegin = fNoteDalSegnosList.begin (),
      iEnd   = fNoteDalSegnosList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the codas if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteCodasList";
  if (fNoteCodasList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrCoda>::const_iterator
      iBegin = fNoteCodasList.begin (),
      iEnd   = fNoteCodasList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the eyeglasses if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteEyeGlassesList";
  if (fNoteEyeGlassesList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrEyeGlasses>::const_iterator
      iBegin = fNoteEyeGlassesList.begin (),
      iEnd   = fNoteEyeGlassesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the damps if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteDampsList";
  if (fNoteDampsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrDamp>::const_iterator
      iBegin = fNoteDampsList.begin (),
      iEnd   = fNoteDampsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the dampAlls if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteDampAllsList";
  if (fNoteDampAllsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrDampAll>::const_iterator
      iBegin = fNoteDampAllsList.begin (),
      iEnd   = fNoteDampAllsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the scordaturas if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteScordaturasList";
  if (fNoteScordaturasList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrScordatura>::const_iterator
      iBegin = fNoteScordaturasList.begin (),
      iEnd   = fNoteScordaturasList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the harmonies associated to this note if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteHarmoniesList";
  if (fNoteHarmoniesList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrHarmony>::const_iterator
      iBegin = fNoteHarmoniesList.begin (),
      iEnd   = fNoteHarmoniesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrHarmony
        harmony = (*i);

      os << harmony;

      if (++i == iEnd) break;
      // no std::endl here
    } // for

    --gIndenter;
  }
  else {
    os << ": " << "[EMPTY]" << std::endl;
  }

  // print the figured bass elements associated to this note if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteFiguredBassesList";
  if (fNoteFiguredBassesList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrFiguredBass>::const_iterator
      iBegin = fNoteFiguredBassesList.begin (),
      iEnd   = fNoteFiguredBassesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrFiguredBass
        figuredBass = (*i);

      os << figuredBass;

      if (++i == iEnd) break;
      // no std::endl here
    } // for

    --gIndenter;
  }
  else {
    os << ": " << "[EMPTY]" << std::endl;
  }

  // print the syllables associated to this note if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteSyllablesList";
  if (fNoteSyllablesList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrSyllable>::const_iterator
      iBegin = fNoteSyllablesList.begin (),
      iEnd   = fNoteSyllablesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrSyllable
        syllable = (*i);

      os << syllable;

/* JMI 0.9.67
      os <<
        syllable->getSyllableKind () <<
          syllable->getSyllableExtendKind () <<
        ": ";

      syllableElementsListAsString (
        syllable->getSyllableElementsList (),
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
      // no std::endl here
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the grace notes group after note if any
  if (fGraceNotesGroupAfterNote) {
    os <<
    	std::endl <<
      std::setw (fieldWidth) <<
      "fGraceNotesGroupAfterNote" <<
      std::endl;

    ++gIndenter;

    fGraceNotesGroupAfterNote->printFull (os);

    --gIndenter;
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::string msrNote::asShortStringWithRawWholeNotes () const
{
  std::stringstream ss;

  ss <<
    "[Note, " <<
    fNoteKind <<
    ' ' <<
    mfInputLineNumberAsString (
    	fInputLineNumber) <<
    ", ";

  switch (fNoteKind) {
    case msrNoteKind::kNote_UNKNOWN_:
      ss <<
      	noteCoreAsString ();
      break;

    case msrNoteKind::kNoteRestInMeasure:
      ss <<
      	noteCoreAsString () <<
        ", fNoteOccupiesAFullMeasure: " <<
        fNoteOccupiesAFullMeasure;
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      ss <<
      	noteCoreAsString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      ss <<
      	noteCoreAsString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      ss <<
        noteCoreAsString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      ss <<
      	noteCoreAsString () <<
        ", fNoteDisplayWholeNotes, " <<
        fNoteDisplayWholeNotes;
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      ss <<
      	noteCoreAsString () <<
        ", fNoteGraphicNotesDurationKind: " <<
        fNoteGraphicNotesDurationKind;
      break;

    case msrNoteKind::kNoteRestInGraceNotesGroup:
      ss <<
      	noteCoreAsString () <<
        ", fNoteGraphicNotesDurationKind: " <<
        fNoteGraphicNotesDurationKind;
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      ss <<
      	noteCoreAsString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      ss <<
      	noteCoreAsString () <<
        ", fNoteGraphicNotesDurationKind: " <<
        fNoteGraphicNotesDurationKind;
      break;

    case msrNoteKind::kNoteRegularInChord:
      ss <<
      	noteCoreAsString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;
      break;

    case msrNoteKind::kNoteRegularInTuplet:
      ss <<
      	noteCoreAsString () <<
        ", fNoteGraphicNotesDurationKind: " <<
        fNoteGraphicNotesDurationKind <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes.asFractionString () <<
        ", fNoteTupletFactor " << fNoteTupletFactor.asShortString ();
      break;

    case msrNoteKind::kNoteRestInTuplet:
      ss <<
      	noteCoreAsString () <<
        ", fNoteGraphicNotesDurationKind: " <<
        fNoteGraphicNotesDurationKind <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes.asFractionString () <<
        ", fNoteTupletFactor " << fNoteTupletFactor.asShortString ();
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      ss <<
      	noteCoreAsString () <<
        ", fNoteGraphicNotesDurationKind: " <<
        fNoteGraphicNotesDurationKind <<
        ", fNoteTupletFactor " << fNoteTupletFactor.asShortString ();
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      ss <<
      	noteCoreAsString () <<
        ", fNoteGraphicNotesDurationKind: " <<
        fNoteGraphicNotesDurationKind <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes.asFractionString () <<
        ", fNoteTupletFactor " << fNoteTupletFactor.asShortString ();
      break;
  } // switch

  ss <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

std::string msrNote::asStringForVoicesFlatView () const
{
  std::stringstream ss;

  ss <<
    "@" <<
    fMeasureElementPositionInMeasure.asFractionString () <<
    ": " <<
    noteCoreAndInputLineNumbersAsString ();

  return ss.str ();
}

std::string msrNote::asStringForMeasuresSlices () const
{
  std::stringstream ss;

  ss <<
    "@:" <<
    fMeasureElementPositionInMeasure.asFractionString () <<
    ' ' <<
    noteCoreAndComplementAsString ();

  return ss.str ();
}

std::ostream& operator << (std::ostream& os, const S_msrNote& elt)
{
  if (elt) {
    os << elt->asString ();
  }
  else {
    os << "[NULL]" << std::endl;
  }

  return os;
}


}

/* JMI
// uplink to tuplet
S_msrTuplet msrNote::fetchNoteUpLinkToTuplet () const
{
  const S_msrTuplet& result;

  switch (fNoteKind) {
    case msrNoteKind::kNote_UNKNOWN_:
      break;

    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteUnpitchedInMeasure:
      result = fNoteShortcutUpLinkToTuplet;
      break;

    case msrNoteKind::kNoteRegularInChord:
      if (fNoteShortcutUpLinkToChord) {
        result =
          fNoteShortcutUpLinkToChord->
            fetchChordUpLinkToTuplet (); JMI 0.9.71
      }
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      if (fNoteShortcutUpLinkToTuplet) {
        result =
          fNoteShortcutUpLinkToTuplet->
            fetchTupletUpLinkToTuplet (); JMI 0.9.71
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteRestInGraceNotesGroup:
    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    / * JMi
      if (fNoteShortcutUpLinkToGraceNotesGroup) {
        result =
          fNoteShortcutUpLinkToGraceNotesGroup->
            getGraceNotesGroupUpLinkToVoice ();
              / * JMI
              getGraceNotesGroupUpLinkToNote ()->
              fetchNoteUpLinkToVoice ();
              * /
      }
    * /
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      if (fNoteShortcutUpLinkToChord) {
        result =
          fNoteShortcutUpLinkToChord->
            getChordUpLinkToContainingGraceNotesGroup ()->
              fetchGraceNoteGroupUpLinkToTuplet ();
      }
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      / * cannot happen ??? JMI
      if (fNoteShortcutUpLinkToTuplet) {
        result =
          fNoteShortcutUpLinkToTuplet->
            getTupletUpLinkToContainingTuplet ()->
              fetchUpLinkToTupletToVoice ();
      }
      * /
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      break;
  } // switch

  return result;
}


// std::string msrNote::asStringForMeasuresSlices () const
// {
//   std::stringstream ss;
//
//   ss <<
//   	"[Note, " <<
//     ", " <<
//     mfInputLineNumberAsString (
//     	fInputLineNumber) <<
//     ", ";
//
//   switch (fNoteKind) {
//     case msrNoteKind::kNote_UNKNOWN_:
//       ss <<
// 				fNoteKind;
//       break;
//
//     case msrNoteKind::kNoteRestInMeasure:
//       ss <<
//         noteCoreAsString () <<
//         ", ";
//
//       if (fNoteOccupiesAFullMeasure) {
//         ss <<
//           'R' <<
//           / * JMI
//           multipleMeasureRestsWholeNotesPitchAndOctaveAsString (
//             fInputLineNumber,
//             fMeasureElementSoundingWholeNotes);
//             */
//           noteCoreAsString ();
//       }
//       else {
//         ss <<
//           'r' << " FII " <<
//           noteCoreAsString ();
//       }
//
// //       ss <<
// //         noteCoreAsString (); JMI 0.9.67
//       break;
//
//     case msrNoteKind::kNoteSkipInMeasure:
//       ss <<
//         's' <<
//         noteCoreAsString ();
//       break;
//
//     case msrNoteKind::kNoteUnpitchedInMeasure:
//       ss <<
//         noteCoreAsString ();
//       break;
//
//     case msrNoteKind::kNoteRegularInMeasure:
//       ss <<
//         noteCoreAsString ();
//       break;
//
//     case msrNoteKind::kNoteInDoubleTremolo:
//       ss <<
// 				noteCoreAsString () <<
//       break;
//
//     case msrNoteKind::kNoteRegularInGraceNotesGroup:
//       ss <<
//         noteCoreAsString ();
//       break;
//
//     case msrNoteKind::kNoteRestInGraceNotesGroup:
//       ss <<
//         noteCoreAsString ();
//       break;
//
//     case msrNoteKind::kNoteSkipInGraceNotesGroup:
//       ss <<
//         noteCoreAsString ();
//       break;
//
//     case msrNoteKind::kNoteInChordInGraceNotesGroup:
//       ss <<
//         noteCoreAsString ();
//         ", noteTupletFactor " << fNoteTupletFactor.asString ();
//       break;
//
//     case msrNoteKind::kNoteRegularInChord:
//       ss <<
//         noteCoreAsString ();
//       break;
//
//     case msrNoteKind::kNoteRegularInTuplet:
//       ss <<
//         "kNoteRegularInTuplet, " <<
//         soundingNoteEssentialsAsStringForMeasuresSlices ();
// /* JMI
//         noteUpLinkToPart ()->
//           tupletSoundingWholeNotesPitchAndOctaveAsString (
//             fInputLineNumber,
//             fMeasureElementSoundingWholeNotes,
//             fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
//             fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ())
//             */
//       break;
//
//     case msrNoteKind::kNoteRestInTuplet:
//       ss <<
//         noteCoreAsString ();
//       break;
//
//     case msrNoteKind::kNoteInTupletInGraceNotesGroup:
//       ss <<
//         noteCoreAsString () <<
// /* JMI
//         noteUpLinkToPart ()->
//           tupletSoundingWholeNotesPitchAndOctaveAsString (
//             fInputLineNumber,
//             fMeasureElementSoundingWholeNotes,
//             fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
//             fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ())
//             */
//         ", noteTupletFactor: " << fNoteTupletFactor.asString ();
//       break;
//
//     case msrNoteKind::kNoteUnpitchedInTuplet:
//       ss <<
//         "kNoteUnpitchedInTuplet, " <<
//         noteSoundingWholeNotesPitchAndOctaveAsString ();
// /* JMI
//         noteUpLinkToPart ()->
//           tupletSoundingWholeNotesPitchAndOctaveAsString (
//             fInputLineNumber,
//             fMeasureElementSoundingWholeNotes,
//             fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
//             fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ())
//             */
//       ss <<
//         ", noteTupletFactor: " << fNoteTupletFactor.asString ();
//       break;
//   } // switch
//
//   ss <<
//     ", line " << fInputLineNumber <<
//     ']';
//
//   return ss.str ();
// }
//
// std::string msrNote::noteForPrintAsString () const
// {
//   std::stringstream ss;
//
// //   ss <<
// //     "fNoteKind: "; // JMI 0.9.67
//
//   switch (fNoteKind) {
//     case msrNoteKind::kNote_UNKNOWN_:
//       ss <<
//         fNoteKind;
//       break;
//
//     case msrNoteKind::kNoteRestInMeasure:
//       ss <<
//         fNoteKind <<
//         ", ";
//
//       if (fNoteOccupiesAFullMeasure) {
//         ss <<
//           'R' <<
//           asShortStringWithRawWholeNotes ();
//       }
//       else {
//         ss <<
//           'r' << " FEE " <<
//           asShortStringWithRawWholeNotes ();
//       }
//       ss << std::endl;
// 			break;
//
//     case msrNoteKind::kNoteSkipInMeasure:
//       ss <<
//         noteCoreAndComplementAsString ();
//       break;
//
//     case msrNoteKind::kNoteUnpitchedInMeasure:
//       ss <<
//         noteCoreAndComplementAsString ();
//       break;
//
//     case msrNoteKind::kNoteRegularInMeasure:
//       ss <<
//         noteCoreAndComplementAsString ();
// 				 ", " <<
// 				 fNoteOctaveKind;
//       break;
//
//     case msrNoteKind::kNoteInDoubleTremolo:
//       ss <<
//         noteCoreAndComplementAsString ();
//       break;
//
//     case msrNoteKind::kNoteRegularInGraceNotesGroup:
//       ss <<
//         noteCoreAndComplementAsString ();
//       break;
//
//     case msrNoteKind::kNoteRestInGraceNotesGroup:
//       ss <<
//         noteCoreAndComplementAsString ();
//       break;
//
//     case msrNoteKind::kNoteSkipInGraceNotesGroup:
//       ss <<
//         noteCoreAndComplementAsString ();
//       break;
//
//     case msrNoteKind::kNoteInChordInGraceNotesGroup:
//       ss <<
//         noteCoreAndComplementAsString ()
//         ", fNoteTupletFactor " << fNoteTupletFactor.asString ();
//       break;
//
//     case msrNoteKind::kNoteRegularInChord:
//       ss <<
//         noteCoreAndComplementAsString ();
//       break;
//
//     case msrNoteKind::kNoteRegularInTuplet:
//       ss <<
//         noteCoreAndComplementAsString ();
// /* JMI
//         noteUpLinkToPart ()->
//           tupletSoundingWholeNotesPitchAndOctaveAsString (
//             fInputLineNumber,
//             fMeasureElementSoundingWholeNotes,
//             fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
//             fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ())
//             */
//       break;
//
//     case msrNoteKind::kNoteRestInTuplet:
//       ss <<
//         noteCoreAndComplementAsString ();
//       break;
//
//     case msrNoteKind::kNoteInTupletInGraceNotesGroup:
//       ss <<
//         noteCoreAndComplementAsString ();
// /* JMI
//         noteUpLinkToPart ()->
//           tupletSoundingWholeNotesPitchAndOctaveAsString (
//             fInputLineNumber,
//             fMeasureElementSoundingWholeNotes,
//             fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
//             fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ())
//             */
//
//       ss <<
//         ", fNoteTupletFactor: " << fNoteTupletFactor.asString ();
//       break;
//
//     case msrNoteKind::kNoteUnpitchedInTuplet:
//       ss <<
//         fNoteKind <<
//         noteSoundingWholeNotesPitchAndOctaveAsString ();
// /* JMI
//         noteUpLinkToPart ()->
//           tupletSoundingWholeNotesPitchAndOctaveAsString (
//             fInputLineNumber,
//             fMeasureElementSoundingWholeNotes,
//             fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
//             fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ())
//             */
//       ss <<
//         ", noteTupletFactor: " << fNoteTupletFactor.asString ();
//       break;
//   } // switch
//
//   return ss.str ();
// }

// */

