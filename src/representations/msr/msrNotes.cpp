/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <regex>

#include "mfStaticSettings.h"

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

//   const std::string&         noteMeasureNumber, // JMI v0.9.66

  msrNoteKind                noteKind,

  msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,
  msrOctaveKind              noteOctaveKind,

  const msrWholeNotes&       noteSoundingWholeNotes,
  const msrWholeNotes&       noteDisplayWholeNotes,

  int                        noteDotsNumber,

  msrNotesDurationKind       noteGraphicNotesDurationKind,

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

//       noteMeasureNumber, // JMI v0.9.66

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

//   const std::string&         noteMeasureNumber, // JMI v0.9.66

  msrNoteKind                noteKind,

  msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,
  msrOctaveKind              noteOctaveKind,

  const msrWholeNotes&       noteSoundingWholeNotes,
  const msrWholeNotes&       noteDisplayWholeNotes,

  int                        noteDotsNumber,

  msrNotesDurationKind       noteGraphicNotesDurationKind,

  msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
  msrOctaveKind              noteDisplayOctaveKind,

  msrNoteIsACueNoteKind      msrNoteIsACueNoteKind,

  msrPrintObjectKind         notePrintObjectKind,

  msrNoteHeadKind            noteHeadKind,
  msrNoteHeadFilledKind      msrNoteHeadFilledKind,
  msrNoteHeadParenthesesKind msrNoteHeadParenthesesKind)
    : msrTupletElement (
        inputLineNumber),
      fNoteColorAlphaRGB ("", "")
{
  fNoteUpLinkToMeasure = upLinkToMeasure;

//   fMeasureElementMeasureNumber = noteMeasureNumber; // JMI ??? v0.9.66

  // basic note description
  fNoteKind = noteKind;

  fNoteQuarterTonesPitchKind  = noteQuarterTonesPitchKind;
  fNoteOctaveKind = noteOctaveKind;

  fNoteDotsNumber = noteDotsNumber;

  fNoteGraphicNotesDurationKind = noteGraphicNotesDurationKind;

  fNoteTupletFactor = mfRational (1, 1);

  fNoteQuarterTonesDisplayPitchKind = noteQuarterTonesDisplayPitchKind;
  fNoteDisplayOctaveKind            = noteDisplayOctaveKind;

  fNoteIsACueNoteKind   = msrNoteIsACueNoteKind;

  fNotePrintObjectKind = notePrintObjectKind;

  fNoteHeadKind            = noteHeadKind;
  fNoteHeadFilledKind      = msrNoteHeadFilledKind;
  fNoteHeadParenthesesKind = msrNoteHeadParenthesesKind;

  fNoteDisplayWholeNotes  = noteDisplayWholeNotes;

  // only now, to be able to display the note as a string
  doSetSoundingWholeNotes (
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

  fNoteMultipleFullBarRestsSequenceNumber = -1;

//   fNoteColorAlphaRGBHasBenSet = false; JMI v0.9.64

  // note lyrics
  // ------------------------------------------------------

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotesDetails ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      "Initializing a note" <<
      ", kind: ";
    if (fNoteKind == msrNoteKind::kNote_UNKNOWN_)
      ss <<
        "kNote_UNKNOWN_";
    else
      ss << fNoteKind;
    ss <<
      ", line " << fInputLineNumber << ":" <<
      std::endl;

    ++gIndenter;

    const int fieldWidth = 30;

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
        "fSoundingWholeNotes" << ": " <<
        fSoundingWholeNotes.asString () <<
        std::endl <<
      std::left <<
        std::setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << ": " <<
        fNoteDisplayWholeNotes.asString () <<
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
        fNoteTupletFactor.asString () <<
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

    gWaeHandler->waeTraceWithLocationDetails (
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

void msrNote::setMeasureElementUpLinkToMeasure (
  const S_msrMeasure& measure)
{
  setNoteUpLinkToMeasure (measure);
}

void msrNote::getMeasureElementUpLinkToMeasure (
  S_msrMeasure& upLinkToMeasure) const
{
  upLinkToMeasure = getNoteUpLinkToMeasure ();
}

void msrNote::setNoteUpLinkToMeasure (
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
  if (gTraceOahGroup->getTraceWholeNoteDurations ()) {
    ++gIndenter;

    std::stringstream ss;

    ss <<
      "Setting the uplink to measure of note " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNoteUpLinkToMeasure = measure;
}

S_msrMeasure msrNote::getNoteUpLinkToMeasure () const
{
  return fNoteUpLinkToMeasure;
}

//________________________________________________________________________
// S_msrMeasure msrNote::fetchMeasureElementUpLinkToMeasure () const
// {
//   S_msrMeasure result;
//
//   switch (fNoteKind) {
//     case msrNoteKind::kNote_UNKNOWN_:
//       break;
//
//     case msrNoteKind::kNoteRegularInMeasure:
//     case msrNoteKind::kNoteRestInMeasure:
//     case msrNoteKind::kNoteSkipInMeasure:
//     case msrNoteKind::kNoteUnpitchedInMeasure:
//       result = fNoteUpLinkToMeasure;
//       break;
//
//     case msrNoteKind::kNoteRegularInChord:
//       if (fNoteShortcutUpLinkToChord) {
//         result =
//           fNoteShortcutUpLinkToChord->
//             fMeasureElementUpLinkToMeasure ();
//       }
//       break;
//
//     case msrNoteKind::kNoteRegularInTuplet:
//     case msrNoteKind::kNoteRestInTuplet:
//     case msrNoteKind::kNoteUnpitchedInTuplet:
//       if (fNoteShortcutUpLinkToTuplet) {
//         result =
//           fNoteShortcutUpLinkToTuplet->
//             fMeasureElementUpLinkToMeasure ();
//       }
//       break;
//
//     case msrNoteKind::kNoteInDoubleTremolo:
//       // JMI
//       break;
//
//     case msrNoteKind::kNoteRegularInGraceNotesGroup:
//     case msrNoteKind::kNoteSkipInGraceNotesGroup:
//     /* JMi
//       if (fNoteShortcutUpLinkToGraceNotesGroup) {
//         result =
//           fNoteShortcutUpLinkToGraceNotesGroup->
//             getGraceNotesGroupUpLinkToVoice ();
//               / * JMI
//               getGraceNotesGroupUpLinkToNote ()->
//               fetchNoteUpLinkToVoice ();
//               * /
//       }
//     */
//       break;
//
//     case msrNoteKind::kNoteInChordInGraceNotesGroup:
//       break;
//
//     case msrNoteKind::kNoteInTupletInGraceNotesGroup:
//       break;
//   } // switch
//
//   return result;
// }

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
      asMinimalString ();

    gWaeHandler->waeTraceWithLocationDetails (
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
      if (fNoteUpLinkToMeasure) {
        result =
          fNoteUpLinkToMeasure->
            fetchMeasureUpLinkToVoice ();
      }
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      if (fNoteShortcutUpLinkToTuplet) {
        S_msrMeasure upLinkToMeasure;

        fNoteShortcutUpLinkToTuplet->
          getMeasureElementUpLinkToMeasure (
            upLinkToMeasure);

        S_msrMeasure
          tupletShortcutUpLinkToMeasure =
            upLinkToMeasure;

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

  if (fNoteUpLinkToMeasure) {
    result =
      fNoteUpLinkToMeasure->
        fetchMeasureUpLinkToStaff ();
  }

  return result;
}

S_msrPart msrNote::fetchUpLinkToNoteToPart () const
{
  S_msrPart result;

  if (fNoteUpLinkToMeasure) {
    result =
      fNoteUpLinkToMeasure->
        fetchMeasureUpLinkToPart ();
  }

  return result;
}

S_msrPartGroup msrNote::fetchNoteUpLinkToPartGroup () const
{
  S_msrPartGroup result;

  if (fNoteUpLinkToMeasure) {
    result =
      fNoteUpLinkToMeasure->
        fetchMeasureUpLinkToPartGroup ();
  }

  return result;
}

S_msrScore msrNote::fetchUpLinkToNoteToScore () const
{
  S_msrScore result;

  if (fNoteUpLinkToMeasure) {
    result =
      fNoteUpLinkToMeasure->
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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNoteKind = noteKind;
}

void msrNote::setNoteColorAlphaRGB (
  msrColorAlphaRGB& noteColorAlphaRGB)
{
  fNoteColorAlphaRGB          = noteColorAlphaRGB;
  fNoteColorAlphaRGBHasBenSet = true;
}

S_msrNote msrNote::createNoteNewbornClone (
  const S_msrPart& containingPart)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of note " <<
      asString () <<
      " in part " <<
      containingPart->
        getPartCombinedName ();

    gWaeHandler->waeTraceWithLocationDetails (
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

  S_msrNote
    newbornClone =
      msrNote::create (
        fInputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()

//         fNoteUpLinkToMeasure->getMeasureNumber (), // JMI v0.9.66

        fNoteKind,

        fNoteQuarterTonesPitchKind,
        fNoteOctaveKind,

        fSoundingWholeNotes,
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
  const S_msrVoice& containingVoice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating a deep clone of note " <<
      asString () <<
      /* JMI
      " in part " <<
      containingPart->
        getPartCombinedName () <<
        */
      std::endl;

    gWaeHandler->waeTraceWithLocationDetails (
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
    "containingVoice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED
*/

  S_msrNote
    deepClone =
      msrNote::create (
        fInputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()

//         fNoteUpLinkToMeasure->getMeasureNumber (), // v0.9.66

        fNoteKind,

        fNoteQuarterTonesPitchKind,
        fNoteOctaveKind,

        fSoundingWholeNotes,
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
    fNoteBelongsToAMultipleFullBarRests =
      fNoteBelongsToAMultipleFullBarRests;

  deepClone->
    fNoteMultipleFullBarRestsSequenceNumber =
      fNoteMultipleFullBarRestsSequenceNumber;

  // note lyrics
  // ------------------------------------------------------

/* JMI
  {
    std::list<S_msrSyllable>::const_iterator i;
    for (i=fNoteSyllables.begin (); i!=fNoteSyllables.end (); ++i) {
      // share this data
      deepClone->
        fNoteSyllables.push_back ((*i));
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
    std::list<S_msrBeam>::const_iterator i;
    for (i=fNoteBeams.begin (); i!=fNoteBeams.end (); ++i) {
      // share this data
      deepClone->
        fNoteBeams.push_back ((*i));
    } // for
  }

  // articulations
  // ------------------------------------------------------

  for (
    std::list<S_msrArticulation>::const_iterator i=
      fNoteArticulations.begin ();
      i!=fNoteArticulations.end ();
      ++i
  ) {
    // share this data
    deepClone->
      fNoteArticulations.push_back ((*i));
  } // for

  // spanners
  // ------------------------------------------------------

  for (
    std::list<S_msrSpanner>::const_iterator i=
      fNoteSpanners.begin ();
      i!=fNoteSpanners.end ();
      ++i
  ) {
    // share this data
    deepClone->
      fNoteSpanners.push_back ((*i));
  } // for

  // technicals
  // ------------------------------------------------------

  {
    std::list<S_msrTechnical>::const_iterator i;
    for (
      i=fNoteTechnicals.begin ();
      i!=fNoteTechnicals.end ();
      ++i
  ) {
      // share this data
      deepClone->
        fNoteTechnicals.push_back ((*i));
    } // for
  }

  {
    std::list<S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i=fNoteTechnicalWithIntegers.begin ();
      i!=fNoteTechnicalWithIntegers.end ();
      ++i
  ) {
      // share this data
      deepClone->
        fNoteTechnicalWithIntegers.push_back ((*i));
    } // for
  }

  {
    std::list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=fNoteTechnicalWithStrings.begin ();
      i!=fNoteTechnicalWithStrings.end ();
      ++i
  ) {
      // share this data
      deepClone->
        fNoteTechnicalWithStrings.push_back ((*i));
    } // for
  }

  // ornaments
  // ------------------------------------------------------

  {
    std::list<S_msrOrnament>::const_iterator i;
    for (i=fNoteOrnaments.begin (); i!=fNoteOrnaments.end (); ++i) {
      // share this data
      deepClone->
        fNoteOrnaments.push_back ((*i));
    } // for
  }

  // glissandos
  // ------------------------------------------------------

  {
    std::list<S_msrGlissando>::const_iterator i;
    for (i=fNoteGlissandos.begin (); i!=fNoteGlissandos.end (); ++i) {
      // share this data
      deepClone->
        fNoteGlissandos.push_back ((*i));
    } // for
  }

  // slides
  // ------------------------------------------------------

  {
    std::list<S_msrSlide>::const_iterator i;
    for (i=fNoteSlides.begin (); i!=fNoteSlides.end (); ++i) {
      // share this data
      deepClone->
        fNoteSlides.push_back ((*i));
    } // for
  }

  // grace notes
  // ------------------------------------------------------

  deepClone->fNoteGraceNotesGroupBefore =
    fNoteGraceNotesGroupBefore;
  deepClone->fNoteGraceNotesGroupAfter =
    fNoteGraceNotesGroupAfter;

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

  // tie
  // ------------------------------------------------------

  deepClone->fNoteTie = // JMI
    // share this data
    fNoteTie;

  // dynamics
  // ------------------------------------------------------

  {
    std::list<S_msrDynamic>::const_iterator i;
    for (i=fNoteDynamics.begin (); i!=fNoteDynamics.end (); ++i) {
      // share this data
      deepClone->
        fNoteDynamics.push_back ((*i));
    } // for
  }

  {
    std::list<S_msrOtherDynamic>::const_iterator i;
    for (i=fNoteOtherDynamics.begin (); i!=fNoteOtherDynamics.end (); ++i) {
      // share this data
      deepClone->
        fNoteOtherDynamics.push_back ((*i));
    } // for
  }

  // slashes
  // ------------------------------------------------------

  {
    std::list<S_msrSlash>::const_iterator i;
    for (i=fNoteSlashes.begin (); i!=fNoteSlashes.end (); ++i) {
      // share this data
      deepClone->
        fNoteSlashes.push_back ((*i));
    } // for
  }

  // cresc/decresc
  // ------------------------------------------------------

  {
    for (S_msrCrescDecresc crescDecresc : fNoteCrescDecrescs) {
      // share this data
      deepClone->
        fNoteCrescDecrescs.push_back (crescDecresc);
    } // for
  }

  // wedges
  // ------------------------------------------------------

  {
    std::list<S_msrWedge>::const_iterator i;
    for (i=fNoteWedges.begin (); i!=fNoteWedges.end (); ++i) {
      // share this data
      deepClone->
        fNoteWedges.push_back ((*i));
    } // for
  }

  // eyeglasses
  // ------------------------------------------------------

  {
    std::list<S_msrEyeGlasses>::const_iterator i;
    for (i=fNoteEyeGlasses.begin (); i!=fNoteEyeGlasses.end (); ++i) {
      // share this data
      deepClone->
        fNoteEyeGlasses.push_back ((*i));
    } // for
  }

  // damps
  // ------------------------------------------------------

  {
    std::list<S_msrDamp>::const_iterator i;
    for (i=fNoteDamps.begin (); i!=fNoteDamps.end (); ++i) {
      // share this data
      deepClone->
        fNoteDamps.push_back ((*i));
    } // for
  }

  // damp alls
  // ------------------------------------------------------

  {
    std::list<S_msrDampAll>::const_iterator i;
    for (i=fNoteDampAlls.begin (); i!=fNoteDampAlls.end (); ++i) {
      // share this data
      deepClone->
        fNoteDampAlls.push_back ((*i));
    } // for
  }

  // scordaturas
  // ------------------------------------------------------

  {
    std::list<S_msrScordatura>::const_iterator i;
    for (i=fNoteScordaturas.begin (); i!=fNoteScordaturas.end (); ++i) {
      // share this data
      deepClone->
        fNoteScordaturas.push_back ((*i));
    } // for
  }

  // words
  // ------------------------------------------------------

  {
    std::list<S_msrWords>::const_iterator i;
    for (i=fNoteWords.begin (); i!=fNoteWords.end (); ++i) {
      // share this data
      deepClone->
        fNoteWords.push_back ((*i));
    } // for
  }

  // slurs
  // ------------------------------------------------------

  {
    std::list<S_msrSlur>::const_iterator i;
    for (i=fNoteSlurs.begin (); i!=fNoteSlurs.end (); ++i) {
      // share this data
      deepClone->
        fNoteSlurs.push_back ((*i));
    } // for
  }

  // ligatures
  // ------------------------------------------------------

  {
    std::list<S_msrLigature>::const_iterator i;
    for (i=fNoteLigatures.begin (); i!=fNoteLigatures.end (); ++i) {
      // share this data
      deepClone->
        fNoteLigatures.push_back ((*i));
    } // for
  }

  // pedals
  // ------------------------------------------------------

  {
    std::list<S_msrPedal>::const_iterator i;
    for (i=fNotePedals.begin (); i!=fNotePedals.end (); ++i) {
      // share this data
      deepClone->
        fNotePedals.push_back ((*i));
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
    fNoteUpLinkToMeasure->getMeasureNumber () =
      fNoteUpLinkToMeasure->getMeasureNumber ();
  deepClone->
    fMeasurePosition =
      fMeasurePosition;
  deepClone->
    fVoicePosition =
      fVoicePosition;

  deepClone->
    fNoteOccupiesAFullMeasure =
      fNoteOccupiesAFullMeasure;

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
  int                  inputLineNumber,
  const std::string&   noteMeasureNumber,
  const msrWholeNotes& soundingWholeNotes,
  const msrWholeNotes& displayWholeNotes,
  int                  dotsNumber)
{
  msrNote * obj =
    new msrNote (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()

//       noteMeasureNumber, // JMI v0.9.66

      msrNoteKind::kNoteRestInMeasure, // noteKind

      msrQuarterTonesPitchKind::kQTP_UNKNOWN_,
      msrOctaveKind::kOctave_UNKNOWN_, // noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrNotesDurationKind::kNotesDuration_UNKNOWN_, // noteGraphicNotesDuration

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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return obj;
}

S_msrNote msrNote::createSkipNote (
  int                  inputLineNumber,
  const std::string&   noteMeasureNumber,
  const msrWholeNotes& soundingWholeNotes,
  const msrWholeNotes& displayWholeNotes,
  int                  dotsNumber)
{
  msrNote * obj =
    new msrNote (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()

      // noteMeasureNumber, // JMI v0.9.66

      msrNoteKind::kNoteSkipInMeasure, // noteKind

      msrQuarterTonesPitchKind::kQTP_UNKNOWN_,
      msrOctaveKind::kOctave_UNKNOWN_, // noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrNotesDurationKind::kNotesDuration_UNKNOWN_, // noteGraphicNotesDuration

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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return obj;
}

S_msrNote msrNote::createGraceSkipNote (
  int                  inputLineNumber,
  const std::string&   noteMeasureNumber,
  const msrWholeNotes& soundingWholeNotes,
  const msrWholeNotes& displayWholeNotes,
  int                  dotsNumber)
{
  msrNote * obj =
    new msrNote (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure() JMI v0.9.67

//       noteMeasureNumber, // JMI v0.9.66

      msrNoteKind::kNoteSkipInGraceNotesGroup,

      msrQuarterTonesPitchKind::kQTP_UNKNOWN_,
      msrOctaveKind::kOctave_UNKNOWN_,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrNotesDurationKind::kNotesDuration_UNKNOWN_, // noteGraphicNotesDuration

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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return obj;
}

//________________________________________________________________________
S_msrNote msrNote::createRestNoteWithOctave (
  int                  inputLineNumber,
  const std::string&   noteMeasureNumber,
  msrOctaveKind        noteOctave,
  const msrWholeNotes& soundingWholeNotes,
  const msrWholeNotes& displayWholeNotes,
  int                  dotsNumber)
{
  msrNote * obj =
    new msrNote (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()

//       noteMeasureNumber, // JMI v0.9.66

      msrNoteKind::kNoteRestInMeasure, // noteKind

      msrQuarterTonesPitchKind::kQTP_Rest,
      noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrNotesDurationKind::kNotesDuration_UNKNOWN_, // noteGraphicNotesDuration

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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return obj;
}

//________________________________________________________________________
S_msrNote msrNote::createSkipNoteWithOctave (
  int                  inputLineNumber,
  const std::string&   noteMeasureNumber,
  msrOctaveKind        noteOctave,
  const msrWholeNotes& soundingWholeNotes,
  const msrWholeNotes& displayWholeNotes,
  int                  dotsNumber)
{
  msrNote * obj =
    new msrNote (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()

//       noteMeasureNumber, // JMI v0.9.66

      msrNoteKind::kNoteSkipInMeasure, // noteKind

      msrQuarterTonesPitchKind::kQTP_Skip,
      noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrNotesDurationKind::kNotesDuration_UNKNOWN_, // noteGraphicNotesDuration JMI ???

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

    gWaeHandler->waeTraceWithLocationDetails (
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
  const msrWholeNotes&     soundingWholeNotes,
  const msrWholeNotes&     displayWholeNotes,
  int                      dotsNumber)
{
  msrNote * obj =
    new msrNote (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()

//       noteMeasureNumber, // JMI v0.9.66

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
  if (gTraceOahGroup->getTraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating regular note " <<
      obj->asShortString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTraceWithLocationDetails (
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

    gWaeHandler->waeTraceWithLocationDetails (
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

    gWaeHandler->waeTraceWithLocationDetails (
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
    restDots     = sm [2];

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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // compute the restNotesDurationKind from restNotesDuration
  msrNotesDurationKind
    restNotesDurationKind =
      msrNotesDurationKindFromString (
        inputLineNumber,
        restNotesDuration);

  // compute the duration whole notes from restNotesDurationKind
  msrWholeNotes
     notesNotesDurationKindFromMslpString =
       msrNotesDurationKindAsWholeNotes (
         restNotesDurationKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "notesNotesDurationKindFromMslpString: " <<
      notesNotesDurationKindFromMslpString;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  result =
    msrNote::createRestNote (
      inputLineNumber,
      restMeasureNumber,
      notesNotesDurationKindFromMslpString, // soundingWholeNotes
      notesNotesDurationKindFromMslpString, // displayWholeNotes
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

    gWaeHandler->waeTraceWithLocationDetails (
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

    gWaeHandler->waeTraceWithLocationDetails (
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
    skipDots     = sm [2];

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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // compute the skipNotesDurationKind from skipNotesDuration
  msrNotesDurationKind
    skipNotesDurationKind =
      msrNotesDurationKindFromString (
        inputLineNumber,
        skipNotesDuration);

  // compute the duration whole notes from skipNotesDurationKind
  msrWholeNotes
     notesNotesDurationKindFromMslpString =
       msrNotesDurationKindAsWholeNotes (
         skipNotesDurationKind);

#ifdef MF_TRACE_IS_ENABLED
  if (gEarlyOptions.getTraceEarlyOptions ()) {
    std::stringstream ss;

    ss <<
      "notesNotesDurationKindFromMslpString: " <<
      notesNotesDurationKindFromMslpString;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  result =
//    msrNote::createSkipNote (
    msrNote::createRestNote (
      inputLineNumber,
      skipMeasureNumber,
      notesNotesDurationKindFromMslpString, // soundingWholeNotes
      notesNotesDurationKindFromMslpString, // displayWholeNotes
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

    gWaeHandler->waeTraceWithLocationDetails (
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

    gWaeHandler->waeTraceWithLocationDetails (
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
    notePitch            = sm [1],
    noteOctaveIndication = sm [2],
    noteNotesDuration         = sm [3],
    noteDots             = sm [4];

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

    gWaeHandler->waeTraceWithLocationDetails (
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
  msrNotesDurationKind
    noteNotesDurationKind =
      msrNotesDurationKindFromString (
        inputLineNumber,
        noteNotesDuration);

  // compute the duration whole notes from noteNotesDurationKind
  msrWholeNotes
     notesNotesDurationKindFromMslpString =
       msrNotesDurationKindAsWholeNotes (
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
      "notesNotesDurationKindFromMslpString: " <<
      notesNotesDurationKindFromMslpString;

    gWaeHandler->waeTraceWithLocationDetails (
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
      notesNotesDurationKindFromMslpString, // soundingWholeNotes
      notesNotesDurationKindFromMslpString, // displayWholeNotes
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
    "semiTonesPitchAndOctave is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  msrNote * obj =
    new msrNote (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()

//       K_MEASURE_NUMBER_UNKNOWN_, // JMI ???  // v0.9.66

      msrNoteKind::kNoteRegularInMeasure, // noteKind

      quarterTonesPitchKindFromSemiTonesPitchKind (
        semiTonesPitchAndOctave->
          getSemiTonesPitchKind ()),
      semiTonesPitchAndOctave->
        getOctaveKind (),

      msrWholeNotes (0, 1), // soundingWholeNotes,
      msrWholeNotes (0, 1), // displayWholeNotes,

      0, // dotsNumber,

      msrNotesDurationKind::kNotesDuration_UNKNOWN_, // noteGraphicNotesDuration

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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return obj;
}

//________________________________________________________________________
void msrNote::setMeasurePosition (
  const S_msrMeasure& measure,
  const msrWholeNotes&     measurePosition,
  const std::string&  context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasurePositions ()) {
    S_msrMeasure upLinkToMeasure;

    getMeasureElementUpLinkToMeasure (
      upLinkToMeasure);

    std::stringstream ss;

    ss <<
      "Setting the measure position of " <<
      asString () <<
      " to " <<
      measurePosition.asString () <<
      " (was '" <<
      fMeasurePosition.asString () <<
      "') in measure " <<
      measure->asShortString () <<
      " (measureElementMeasureNumber: " <<
      upLinkToMeasure->getMeasureNumber () <<
      "), context: \"" <<
      context <<
      "\"";

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measurePosition != K_MEASURE_POSITION_UNKNOWN_,
    "measurePosition == K_MEASURE_POSITION_UNKNOWN_");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // handle the chord itself
  msrMeasureElement::setMeasurePosition (
    measure,
    measurePosition,
    context);

  setNoteAttachedElementsMeasurePosition (
    measure,
    measurePosition);
}

void msrNote::setNoteAttachedElementsMeasurePosition (
  const S_msrMeasure& measure,
  const msrWholeNotes&     measurePosition)
{
// // JMI v0.9.66
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceHarmonies ()) {
    std::stringstream ss;

    ss <<
      "setNoteAttachedElementsMeasurePosition(), fNoteHarmoniesList.size (): " <<
      fNoteHarmoniesList.size ();

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

// JMI v0.9.66
//   if (fNoteHarmoniesList.size ()) {
//     std::list<S_msrHarmony>::const_iterator i;
//     for (S_msrHarmony harmony : fNoteHarmoniesList) {
//       // set the harmony measure position, taking it's offset into account
//       harmony->
//         setMeasurePosition (
//           measure,
//           measurePosition,
//           "msrNote::setMeasurePosition()");
//     } // for
//   }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceFiguredBasses ()) {
    std::stringstream ss;

    ss <<
      "fNoteFiguredBassesList.size (): " <<
      fNoteFiguredBassesList.size ();

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // are there figured bass elements attached to this note?
//   if (fNoteFiguredBassesList.size ()) {
//     std::list<S_msrFiguredBass>::const_iterator i;
//     for (S_msrFiguredBass figuredBass : fNoteFiguredBassesList) {
//       // set the figured bass element measure position
//       figuredBass->
//         setMeasurePosition (
//           measure,
//           measurePosition,
//           "msrNote::setMeasurePosition()");
//     } // for
//   }

//   // are there dal segnos attached to this note?
//   if (fNoteDalSegnos.size ()) {
//     for (S_msrDalSegno dalSegno : fNoteDalSegnos) {
//       // set the dal segno measure position
//       dalSegno->
//         setDalSegnoMeasurePosition (
//           measure,
//           measurePosition,
//           "msrNote::setMeasurePosition()");
//     } // for
//   }
}

void msrNote::setNoteOccupiesAFullMeasure ()
{
  fNoteOccupiesAFullMeasure = true;
}

std::string msrNote::noteSoundingWholeNotesAsMsrString () const
{
  std::string result;

  if (fSoundingWholeNotes.getNumerator () == 0)
    result = "[0 WHOLE NOTES]";
  else
    result =
      wholeNotesAsMsrString (
        fInputLineNumber,
        fSoundingWholeNotes);

  return result;
}

std::string msrNote::noteDisplayWholeNotesAsMsrString () const
{
  std::string result;

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
    fetchNoteIsARest () && fNoteDisplayOctaveKind != msrOctaveKind::kOctave_UNKNOWN_;
}

void msrNote::setNoteStem (const S_msrStem& stem)
{
  // register note stem
  fNoteStem = stem;

  // mark note as stemless if relevant
  if (stem->getStemKind () == msrStemKind::kStemNeutral)
    fNoteIsStemless = true;
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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNoteBelongsToAChord = true;
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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // the display whole notes are known, compute the sounding ones
  doSetSoundingWholeNotes (
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

    gWaeHandler->waeTraceWithLocationDetails (
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

    gWaeHandler->waeTraceWithLocationDetails (
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
  fNoteBeams.push_back (beam);
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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNoteArticulations.push_back (art);
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

    gWaeHandler->waeTraceWithLocationDetails (
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
  fNoteSpanners.push_back (spanner);
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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the technical to the note technicals list
  fNoteTechnicals.push_back (technical);
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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the technical with integer to the note technicals with integers list
  fNoteTechnicalWithIntegers.push_back (
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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the technical with float to the note technicals with floats list
  fNoteTechnicalWithFloats.push_back (
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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the technical with string to the note technicals with strings list
  fNoteTechnicalWithStrings.push_back (
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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the glissando to the note glissandos list
  fNoteGlissandos.push_back (glissando);
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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append the slide to the note glissandos list
  fNoteSlides.push_back (slide);
}

void msrNote::setNoteGraceNotesGroupBefore (
  const S_msrGraceNotesGroup& graceNotesGroupBefore)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Attaching grace notes group " <<
      graceNotesGroupBefore->asString () <<
      " before note " <<
      asString () <<
      ", line " << graceNotesGroupBefore->getInputLineNumber ();

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register the before grace notes group in the note
  fNoteGraceNotesGroupBefore = graceNotesGroupBefore;

  // setup the grace notes group's note upLink
  graceNotesGroupBefore->
    setGraceNotesGroupUpLinkToNote (
      this);
}

void msrNote::setNoteGraceNotesGroupAfter (
  const S_msrGraceNotesGroup& graceNotesGroupAfter)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Attaching grace notes group " <<
      graceNotesGroupAfter->asString () <<
      " after note " <<
      asString () <<
      ", line " << graceNotesGroupAfter->getInputLineNumber ();

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Attaching afterGraceNotesGroup '" << afterGraceNotesGroup->asString () <<
      "' to note '" << asShortString () <<
      "', line " << afterGraceNotesGroup->getInputLineNumber ();

    gWaeHandler->waeTraceWithLocationDetails (
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
      "', line " << trem->getInputLineNumber ();

    gWaeHandler->waeTraceWithLocationDetails (
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
      "', line " << dynamic->getInputLineNumber ();

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNoteDynamics.push_back (dynamic);
}
void msrNote::appendOtherDynamicToNote (
  const S_msrOtherDynamic& otherDynamic)
{
  fNoteOtherDynamics.push_back (otherDynamic);
}

void msrNote::appendWordsToNote (
  const S_msrWords& words)
{
  fNoteWords.push_back (words);
}

void msrNote::appendSlurToNote (
  const S_msrSlur& slur)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSlurs ()) {
    std::stringstream ss;

    ss <<
      "Adding slur '" << slur <<
      "' to note '" << asString () << "'";

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNoteSlurs.push_back (slur);
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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
          asShortString () <<
          std::endl;

        gWaeHandler->waeTraceWithLocationDetails (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      fNoteLigatures.pop_back ();

      // don't register 'ligature stop'
    }

    else
      fNoteLigatures.push_back (ligature);
  }

  else
    fNoteLigatures.push_back (ligature);
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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fNotePedals.size ()) {
    if (
      fNotePedals.back ()->getPedalTypeKind () == msrPedalTypeKind::kPedalTypeStart
        &&
      pedal->getPedalTypeKind () == msrPedalTypeKind::kPedalTypeStop
    ) {
      // it may happen that a given note has a 'pedal start'
      // and a 'pedal stop' in sequence, ignore both            // JMI ???

#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTracePedals ()) {
        std::stringstream ss;

        ss <<
          "a 'pedal start' is immediately followed by a 'pedal stop'" <<
          std::endl <<
          "with the same number, ignoring both of them at line " <<
          pedal->getInputLineNumber ();

        msrWarning (
          gServiceRunData->getInputSourceName (),
          pedal->getInputLineNumber (),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      // rmeove 'pedal start'
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTracePedals ()) {
        std::stringstream ss;

        ss <<
          "Removing last pedal (start) for note '" <<
          asShortString () <<
          std::endl;

        gWaeHandler->waeTraceWithLocationDetails (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED
      fNotePedals.pop_back ();

      // don't register 'pedal stop'
    }

    else
      fNotePedals.push_back (pedal);
  }

  else
    fNotePedals.push_back (pedal);
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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNoteSlashes.push_back (slash);
}

void msrNote::appendCrescDecrescToNote (
  const S_msrCrescDecresc& crescDecresc)
{
  fNoteCrescDecrescs.push_back (crescDecresc);
}

void msrNote::appendWedgeToNote (
  const S_msrWedge& wedge)
{
  fNoteWedges.push_back (wedge);
}

void msrNote::appendSegnoToNote (
  const S_msrSegno& segno)
{
  fNoteSegnos.push_back (segno);
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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNoteDalSegnos.push_back (dalSegno);
}

void msrNote::appendCodaToNote (
  const S_msrCoda& coda)
{
  fNoteCodas.push_back (coda);
}

void msrNote::appendEyeGlassesToNote (
  const S_msrEyeGlasses& eyeGlasses)
{
  fNoteEyeGlasses.push_back (eyeGlasses);
}

void msrNote::appendDampToNote (
  const S_msrDamp&  damp)
{
  fNoteDamps.push_back (damp);
}

void msrNote::appendDampAllToNote (
  const S_msrDampAll& dampAll)
{
  fNoteDampAlls.push_back (dampAll);
}

void msrNote::appendScordaturaToNote (
  const S_msrScordatura& scordatura)
{
  fNoteScordaturas.push_back (scordatura);
}

// this 'override' NOT NEEXDED??? JMI v0.9.66
// void msrNote::setVoicePosition (
//   msrWholeNotes&     voicePosition,
//   const std::string& context)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasurePositions ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Assigning note voice position of " <<
//       asString () <<
//       " to " << voicePosition <<
//       " in measure '" <<
//       fNoteUpLinkToMeasure->getMeasureNumber () <<
//       "', context: \"" <<
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
//     voicePosition != K_MEASURE_POSITION_UNKNOWN_,
//     "voicePosition == K_MEASURE_POSITION_UNKNOWN_");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   // set measure element voice position
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasurePositions ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Setting note voice position of " <<
//       asString () <<
//       " to " << voicePosition <<
//       " in measure '" <<
//       fNoteUpLinkToMeasure->getMeasureNumber () <<
//       "', context: \"" <<
//       context <<
//       "\"" <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fVoicePosition = voicePosition;
//
//   // account for it in voicePosition
//   voicePosition +=
//     fSoundingWholeNotes;
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasurePositions ()) {
//      std::stringstream ss;
//
//     ss <<
//       "Position in voice becomes " <<
//       voicePosition <<
//       "', context: \"" <<
//       context <<
//       "\"" <<
//       std::endl;

//       gWaeHandler->waeTraceWithLocationDetails (
//         __FILE__, __LINE__,
//         ss.str (),
//   }
// #endif // MF_TRACE_IS_ENABLED
// }

bool msrNote::compareNotesByIncreasingMeasurePosition (
  const SMARTP<msrNote>& first,
  const SMARTP<msrNote>& second)
{
  return
    bool (
      first->fMeasurePosition
        <
      second->fMeasurePosition
    );
}

S_msrDynamic msrNote::removeFirstDynamics () // JMI
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDynamics ()) {
    std::stringstream ss;

    ss <<
      "Removing first dynamic from note '" <<
      asShortString ();

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fNoteDynamics.size () > 0,
    "fNoteDynamics is empty");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrDynamic dyn = fNoteDynamics.front ();
  fNoteDynamics.pop_front ();

  return dyn;
}

S_msrWedge msrNote::removeFirstWedge () // JMI
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceWedges ()) {
    std::stringstream ss;

    ss <<
      "Removing first wedge from note '" <<
      asShortString ();

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fNoteDynamics.size () > 0,
    "fNoteDynamics is empty");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  S_msrWedge wedge = fNoteWedges.front ();
  fNoteWedges.pop_front ();
  return wedge;
}

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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fNoteSyllables.push_back (syllable);
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

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set the harmony's whole notes duration
  // change the harmony whole notes if it belongs to a tuplet ??? utf8.xml JMI v0.9.66
  // JMI v0.9.67 NOT if there are several harmonies with offsets on the same note -- HARMFUL!
//   harmony->
//     setSoundingWholeNotes (
//       fSoundingWholeNotes,
//       "appendHarmonyToNote()");

  fNoteHarmoniesList.push_back (harmony);

//   // register this note as the harmony upLink // JMI v0.9.66 JMI v0.9.66 JMI v0.9.66 MERDUM
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
//       gWaeHandler->waeTraceWithLocationDetails (
//         __FILE__, __LINE__,
//         ss.str (),
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // set the figured bass's whole notes duration
//   // change the figured bass whole notes if it belongs to a tuplet ??? utf8.xml JMI v0.9.66
//   // JMI v0.9.67 NOT if there are several figured basses with offsets on the same note -- HARMFUL!
//   figuredBass->
//     setSoundingWholeNotes (
//       fSoundingWholeNotes,
//       "appendFiguredBassToNote()");
//
//   fNoteFiguredBassesList.push_back (figuredBass);
//
// //   // register this note as the figured bass upLink // JMI v0.9.66 JMI v0.9.66 JMI v0.9.66 MERDUM
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

    gWaeHandler->waeTraceWithLocationDetails (
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

          gWaeHandler->waeTraceWithLocationDetails (
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

    gWaeHandler->waeTraceWithLocationDetails (
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

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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
            "% ==> visiting grace notes groups 'before' is inhibited";

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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
    std::list<S_msrBeam>::const_iterator i;
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
    std::list<S_msrArticulation>::const_iterator i;
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
    std::list<S_msrSpanner>::const_iterator i;
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
    std::list<S_msrTechnical>::const_iterator i;
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
    std::list<S_msrTechnicalWithInteger>::const_iterator i;
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
    std::list<S_msrTechnicalWithFloat>::const_iterator i;
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
    std::list<S_msrTechnicalWithString>::const_iterator i;
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
    std::list<S_msrOrnament>::const_iterator i;
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
    std::list<S_msrGlissando>::const_iterator i;
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
    std::list<S_msrSlide>::const_iterator i;
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
    std::list<S_msrDynamic>::const_iterator i;
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
    std::list<S_msrOtherDynamic>::const_iterator i;
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
    std::list<S_msrWords>::const_iterator i;
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
    std::list<S_msrSlur>::const_iterator i;
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
    std::list<S_msrLigature>::const_iterator i;
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
    std::list<S_msrPedal>::const_iterator i;
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
    std::list<S_msrSegno>::const_iterator i;
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
    std::list<S_msrDalSegno>::const_iterator i;
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
    std::list<S_msrCoda>::const_iterator i;
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
    std::list<S_msrEyeGlasses>::const_iterator i;
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
    std::list<S_msrDamp>::const_iterator i;
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
    std::list<S_msrDampAll>::const_iterator i;
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
    std::list<S_msrScordatura>::const_iterator i;
    for (i=fNoteScordaturas.begin (); i!=fNoteScordaturas.end (); ++i) {
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

  // browse the syllables if any
  if (fNoteSyllables.size ()) {
    ++gIndenter;
    for (S_msrSyllable syllable : fNoteSyllables) {
      // browse the syllable
      msrBrowser<msrSyllable> browser (v);
      browser.browse (*syllable);
    } // for
    --gIndenter;
  }

  // browse the grace notes group after if any
  if (fNoteGraceNotesGroupAfter) {
    // fetch the score
    S_msrScore
      score =
        fNoteUpLinkToMeasure->
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
            "% ==> visiting grace notes groups 'after' is inhibited";

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
      }
      else {
        // browse the grace notes group after
        msrBrowser<msrGraceNotesGroup> browser (v);
        browser.browse (*fNoteGraceNotesGroupAfter);
      }
    }
  }
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
    case msrNoteKind::kNoteUnpitchedInTuplet:
      ss << "unpitched";
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

std::string msrNote::noteGraphicNotesDurationAsMusicXMLString () const
{
  std::string
    result =
      msrNotesDurationKindAsMusicXMLType (
        fNoteGraphicNotesDurationKind);

  return result;
}

// std::string msrNote::tupletNoteGraphicNotesDurationAsMsrString ( // JMI v0.9.66
//   int actualNotes, int normalNotes) const
// {
//   return
//     wholeNotesAsMsrString (
//       fInputLineNumber,
//       fSoundingWholeNotes
//         *
//       mfRational (actualNotes, normalNotes));
// }

std::string msrNote::noteDiatonicPitchKindAsString (
  int inputLineNumber) const
{
  return
    msrDiatonicPitchKindAsStringInLanguage (
      gMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
      noteDiatonicPitchKind (
        fInputLineNumber));
}

std::string msrNote::asShortStringWithRawWholeNotes () const
{
  std::stringstream ss;

  ss << "[Note, ";

  switch (fNoteKind) {
    case msrNoteKind::kNote_UNKNOWN_:
      ss <<
        "***noNote***";
      break;

    case msrNoteKind::kNoteRestInMeasure:
      ss <<
        "kNoteRestInMeasure: " <<
        fNoteOctaveKind <<
        ", fSoundingWholeNotes: " <<
        fSoundingWholeNotes.asString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      ss <<
        "kNoteSkipInMeasure: " <<
        ", fSoundingWholeNotes: " <<
        fSoundingWholeNotes.asString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      ss <<
        "kNoteUnpitchedInMeasure: " <<
        notePitchAsString () <<
        ", fSoundingWholeNotes: " <<
        fSoundingWholeNotes.asString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInMeasure:
			// ss << "======> asShortStringWithRawWholeNotes()" << std::endl;

      ss <<
        "kNoteRegularInMeasure" <<
        notePitchAsString () <<
        fNoteOctaveKind <<
        ", fSoundingWholeNotes: " <<
        fSoundingWholeNotes.asString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      ss <<
        "kNoteInDoubleTremolo" <<
        ", " <<
        notePitchAsString () <<
        fNoteOctaveKind <<
        ", fSoundingWholeNotes: " <<
        fSoundingWholeNotes.asString () <<
        ", fNoteDisplayWholeNotes, " <<
        fNoteDisplayWholeNotes;
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      ss <<
        "kNoteRegularInGraceNotesGroup" <<
        ", " <<
        notePitchAsString () <<
        fNoteGraphicNotesDurationKind <<
        ", " <<
        fNoteOctaveKind;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      ss <<
        "kNoteSkipInGraceNotesGroup" <<
        ":" <<
        ", fSoundingWholeNotes: " <<
        fSoundingWholeNotes.asString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      ss <<
        "kNoteInChordInGraceNotesGroup" <<
        ", " <<
        notePitchAsString () <<
        fNoteGraphicNotesDurationKind <<
        ", " <<
        fNoteOctaveKind;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << "."; // JMI
      } // for
      break;

    case msrNoteKind::kNoteRegularInChord:
      ss <<
        "kNoteRegularInChord" <<
        ", " <<
        notePitchAsString () <<
        fNoteOctaveKind <<
        ", fSoundingWholeNotes: " <<
        fSoundingWholeNotes.asString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInTuplet:
      ss <<
        "kNoteRegularInTuplet" <<
        ", " <<
        notePitchAsString () <<
        fNoteGraphicNotesDurationKind <<
        ", " <<
        fNoteOctaveKind <<
        ", fSoundingWholeNotes: " <<
        fSoundingWholeNotes.asString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for

      ss <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;

    case msrNoteKind::kNoteRestInTuplet:
      ss <<
        "kNoteRestInTuplet" <<
        ", " <<
        notePitchAsString () <<
        fNoteGraphicNotesDurationKind <<
        ", fSoundingWholeNotes: " <<
        fSoundingWholeNotes.asString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for

      ss <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      ss <<
        "kNoteInTupletInGraceNotesGroup" <<
        ", " <<
        notePitchAsString () <<
        fNoteGraphicNotesDurationKind;

      if (! fetchNoteIsARest ()) {
        ss <<
          fNoteOctaveKind;
      }

      ss <<
        ", fSoundingWholeNotes: " <<
        fSoundingWholeNotes.asString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for

      ss <<
        ", fNoteTupletFactor " << fNoteTupletFactor;
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      ss <<
        "kNoteUnpitchedInTuplet " <<
        fNoteGraphicNotesDurationKind <<
        ", fSoundingWholeNotes: " <<
        fSoundingWholeNotes.asString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes;

      ss <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;
  } // switch

  ss <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

std::string msrNote::asShortString () const
{
  std::stringstream ss;

  ss << "[Note, ";

  switch (fNoteKind) {
    case msrNoteKind::kNote_UNKNOWN_:
      ss <<
        "kNote_UNKNOWN_" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteRestInMeasure:
      ss <<
        "kNoteRestInMeasure" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      ss <<
        "kNoteSkipInMeasure: " <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      ss <<
        "kNoteUnpitchedInMeasure: " <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      {
			// ss << "======> asShortString()" << std::endl;

        ss <<
          "kNoteRegularInMeasure" <<
          ", " <<
          notePitchAsString () <<
          noteSoundingWholeNotesAsMsrString ();

        for (int i = 0; i < fNoteDotsNumber; ++i) {
          ss << ".";
        } // for

        ss <<
          ", " <<
          fNoteOctaveKind;

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

        ss <<
          ", noteStaffUpLink: ";
        if (noteStaffUpLink) {
          ss <<
            noteStaffUpLink->getStaffName ();
        }
        else {
          ss << "[NULL]";
        }

        ss <<
          ", noteVoiceUpLink: ";
        if (noteVoiceUpLink) {
          ss <<
            noteVoiceUpLink->getVoiceName ();
        }
        else {
          ss << "[NULL]";
        }
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      ss <<
        "kNoteInDoubleTremolo " <<
        ", " <<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        ", " <<
        fNoteOctaveKind;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      ss <<
        "kNoteRegularInGraceNotesGroup" <<
        ", " <<
        notePitchAsString () <<
        fNoteGraphicNotesDurationKind <<
        ", " <<
        fNoteOctaveKind;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      ss <<
        "kNoteSkipInGraceNotesGroup: " <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

   case msrNoteKind::kNoteInChordInGraceNotesGroup:
      ss <<
        "kNoteInChordInGraceNotesGroup " <<
        ", " <<
        notePitchAsString () <<
        fNoteGraphicNotesDurationKind <<
        ", " <<
        fNoteOctaveKind;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << "."; // JMI
      } // for
      break;

    case msrNoteKind::kNoteRegularInChord:
      ss <<
        "kNoteRegularInChord " <<
        ", " <<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        ", " <<
        fNoteOctaveKind;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInTuplet:
      ss <<
        "kNoteRegularInTuplet" <<
        ", " <<
        notePitchAsString () <<
        fNoteGraphicNotesDurationKind <<
        ", fSoundingWholeNotes: " <<
        fSoundingWholeNotes.asString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes.asString () <<
        /* JMI v0.9.67
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fSoundingWholeNotes.asString (),
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ());
            */
        ", " <<
        fNoteOctaveKind;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for

      ss <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteRestInTuplet:
      ss <<
        "kNoteRestInTuplet" <<
        ", " <<
        notePitchAsString () <<
        fNoteGraphicNotesDurationKind <<
        ", fSoundingWholeNotes: " <<
        fSoundingWholeNotes.asString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes.asString ();
        /* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fSoundingWholeNotes.asString (),
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ());
            */

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for

      ss <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      ss <<
        "kNoteInTupletInGraceNotesGroup" <<
        ", " <<
        notePitchAsString () <<
        fNoteGraphicNotesDurationKind <<
        ", fSoundingWholeNotes: " <<
        fSoundingWholeNotes.asString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes.asString ();
        /* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fSoundingWholeNotes.asString (),
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ());
            */

      if (! fetchNoteIsARest ()) {
        ss <<
          ", " <<
          fNoteOctaveKind;
      }

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for

      ss <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      ss <<
        "kNoteUnpitchedInTuplet" <<
        ", fSoundingWholeNotes: " <<
        fSoundingWholeNotes.asString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes.asString ();
        /* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fSoundingWholeNotes.asString (),
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ());
            */

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for

      ss <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;
  } // switch

  ss <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

std::string msrNote::asMinimalString () const
{
  std::stringstream ss;

  ss << "[Note, ";

  switch (fNoteKind) {
    case msrNoteKind::kNote_UNKNOWN_:
      ss <<
        "kNote_UNKNOWN_" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteRestInMeasure:
      ss <<
        "kNoteRestInMeasure" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      ss <<
        "kNoteSkipInMeasure: " <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      ss <<
        "kNoteUnpitchedInMeasure: " <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      {
				// ss << "======> asMinimalString()" << std::endl;

        ss <<
          "kNoteRegularInMeasure" <<
          ", " <<
          notePitchAsString () <<
          noteSoundingWholeNotesAsMsrString ();

        for (int i = 0; i < fNoteDotsNumber; ++i) {
          ss << ".";
        } // for

        ss <<
          ", " <<
          fNoteOctaveKind;
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      ss <<
        "kNoteInDoubleTremolo " <<
        ", " <<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        ", " <<
        fNoteOctaveKind;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      ss <<
        "kNoteRegularInGraceNotesGroup" <<
        ", " <<
        notePitchAsString () <<
        fNoteGraphicNotesDurationKind <<
        ", " <<
        fNoteOctaveKind;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      ss <<
        "kNoteSkipInGraceNotesGroup: " <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

   case msrNoteKind::kNoteInChordInGraceNotesGroup:
      ss <<
        "kNoteInChordInGraceNotesGroup " <<
        ", " <<
        notePitchAsString () <<
        fNoteGraphicNotesDurationKind <<
        ", " <<
        fNoteOctaveKind;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << "."; // JMI
      } // for
      break;

    case msrNoteKind::kNoteRegularInChord:
      ss <<
        "kNoteRegularInChord " <<
        ", " <<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        ", " <<
        fNoteOctaveKind;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInTuplet:
      ss <<
        "kNoteRegularInTuplet" <<
        ", " <<
        notePitchAsString () <<
        fNoteGraphicNotesDurationKind <<
        ", fSoundingWholeNotes: " <<
        fSoundingWholeNotes.asString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes.asString () <<
        ", " <<
        fNoteOctaveKind;

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for

      ss <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteRestInTuplet:
      ss <<
        "kNoteRestInTuplet" <<
        ", " <<
        notePitchAsString () <<
        fNoteGraphicNotesDurationKind <<
        ", fSoundingWholeNotes: " <<
        fSoundingWholeNotes.asString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes.asString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for

      ss <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      ss <<
        "kNoteInTupletInGraceNotesGroup" <<
        ", " <<
        notePitchAsString () <<
        fNoteGraphicNotesDurationKind <<
        ", fSoundingWholeNotes: " <<
        fSoundingWholeNotes.asString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes.asString ();

      if (! fetchNoteIsARest ()) {
        ss <<
          ", " <<
          fNoteOctaveKind;
      }

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for

      ss <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      ss <<
        "kNoteUnpitchedInTuplet" <<
        ", fSoundingWholeNotes: " <<
        fSoundingWholeNotes.asString () <<
        ", fNoteDisplayWholeNotes: " <<
        fNoteDisplayWholeNotes.asString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        ss << ".";
      } // for

      ss <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;
  } // switch

  ss <<
    ", line " << fInputLineNumber <<
    ']';

  return ""; // JMI v0.9.67

  return ss.str ();
}

std::string msrNote::noteComplementsAsString () const
{
  std::stringstream ss;

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

  ss <<
    ", noteStaffUpLink: ";
  if (noteStaffUpLink) {
    ss <<
      noteStaffUpLink->getStaffName ();
  }
  else {
    ss << "[NULL]";
  }

  ss <<
    ", noteVoiceUpLink: ";
  if (noteVoiceUpLink) {
    ss <<
      noteVoiceUpLink->getVoiceName ();
  }
  else {
    ss << "[NULL]";
  }

  ss <<
    ", measureElementMeasureNumber: ";
  if (fNoteUpLinkToMeasure) {
    std::string
      measureNumber =
        fNoteUpLinkToMeasure->
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

std::string msrNote::soundingNoteEssentialsAsString () const
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fNoteDotsNumber < 10,
    "fNoteDotsNumber " + std::to_string (fNoteDotsNumber) + ">= 10");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  std::stringstream ss;

// 	ss << "======> msrNote::soundingNoteEssentialsAsString()" << std::endl;
//
// 	print (gLog);

  ss <<
    "sounding: " <<
    notePitchAsString () <<
    noteSoundingWholeNotesAsMsrString ();

  for (int i = 0; i < fNoteDotsNumber; ++i) {
    ss << ".";
  } // for

  ss <<
    '-' <<
    fNoteOctaveKind <<
//     ",++++ " << // JMI v0.9.67 v0.9.66
    noteComplementsAsString ();

// 	ss << "<====== msrNote::soundingNoteEssentialsAsString(), return " << ss.str () << std::endl;

  return ss.str ();
}

std::string msrNote::soundingNoteEssentialsAsStringForMeasuresSlices () const // JMI USELESS??? see above method
{
  std::stringstream ss;

  ss <<
    "[sounding " <<
    notePitchAsString () <<
    noteSoundingWholeNotesAsMsrString ();

  for (int i = 0; i < fNoteDotsNumber; ++i) {
    ss << ".";
  } // for

  ss <<
    ' ' <<
    fNoteOctaveKind <<
    ']';

  return ss.str ();
}

std::string msrNote::asShortStringForTimeView () const
{
  std::stringstream ss;

  ss <<
    "@:" <<
    fMeasurePosition.asString () <<
    ' ' <<
    asShortStringForMeasuresSlices ();

  return ss.str ();
}

std::string msrNote::nonSoundingNoteEssentialsAsString () const
{
  std::stringstream ss;

  ss <<
    '[' <<
    notePitchAsString () <<
    ' ' << // JMI
    noteDisplayWholeNotesAsMsrString ();

  for (int i = 0; i < fNoteDotsNumber; ++i) {
    ss << ".";
  } // for

  ss <<
    ", " <<
    fNoteOctaveKind <<
    ",---- " << // JMI v0.9.67
    noteComplementsAsString () <<
    ']';

  return ss.str ();
}

std::string msrNote::nonSoundingNoteEssentialsAsStringForMeasuresSlices () const
{
  std::stringstream ss;

  ss <<
//     notePitchAsString () << JMI
//     ' ' <<
    noteDisplayWholeNotesAsMsrString ();

  for (int i = 0; i < fNoteDotsNumber; ++i) {
    ss << ".";
  } // for

//   ss << JMI
//     " (" <<
//     fNoteOctaveKind <<
//     ")";

  return ss.str ();
}

std::string msrNote::asString () const
{
  std::stringstream ss;

// 	ss << "======> msrNote::asString()" << std::endl;

  ss <<
    "[Note, ";
//     "fNoteKind: "; // JMI v0.9.67

  switch (fNoteKind) {
    case msrNoteKind::kNote_UNKNOWN_:
      ss <<
        "*noNote*";
      break;

    case msrNoteKind::kNoteRestInMeasure:
      ss <<
        "kNoteRestInMeasure, ";

      if (fNoteOccupiesAFullMeasure) {
        ss <<
          "R" <<
          /* JMI
          multipleFullBarRestsWholeNotesAsMsrString (
            fInputLineNumber,
            fSoundingWholeNotes.asString ());
            */
          nonSoundingNoteEssentialsAsString ();
      }
      else {
        ss <<
          "r" <<
          nonSoundingNoteEssentialsAsString ();
      }

      ss <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      ss <<
        "kNoteSkipInMeasure, " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      ss <<
        "kNoteUnpitchedInMeasure, " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteRegularInMeasure:
// 			ss << "======> msrNote::asString() - " << std::endl;

      ss <<
        "kNoteRegularInMeasure, " <<
        soundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      ss <<
        "kNoteInDoubleTremolo, " <<
        soundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      ss <<
        "kNoteRegularInGraceNotesGroup, " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      ss <<
        "kNoteSkipInGraceNotesGroup, " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      ss <<
        "kNoteInChordInGraceNotesGroup, " <<
        nonSoundingNoteEssentialsAsString () <<
        ", fNoteTupletFactor: " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteRegularInChord:
      ss <<
        "kNoteRegularInChord, " <<
        soundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteRegularInTuplet:
      ss <<
        "kNoteRegularInTuplet, " <<
        soundingNoteEssentialsAsString ();
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fSoundingWholeNotes,
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ())
            */
      break;

    case msrNoteKind::kNoteRestInTuplet:
      ss <<
        "kNoteRestInTuplet, " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      ss <<
        "kNoteInTupletInGraceNotesGroup, " <<
        nonSoundingNoteEssentialsAsString ();
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fSoundingWholeNotes,
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ())
            */

      ss <<
        ", fNoteTupletFactor: " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      ss <<
        "kNoteUnpitchedInTuplet, " <<
        noteSoundingWholeNotesAsMsrString ();
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fSoundingWholeNotes,
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ())
            */
      ss <<
        ", noteTupletFactor: " << fNoteTupletFactor.asString ();
      break;
  } // switch

/* JMI
  ss << std::left <<
    ", measurePosition: ";
    / * JMI
  if (fMeasurePosition == K_MEASURE_POSITION_UNKNOWN__MEASURE_NUMBER) {
    ss << "unknown (" << fMeasurePosition.asString () << ")";
  }
  else {
    ss << fMeasurePosition;
  }
  * /
  ss << fMeasurePosition;

  ss <<
    ", voicePosition: " <<
    fVoicePosition;
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

  if (fNoteTie) {
    ss <<
      ", " << fNoteTie->getTieKind ();
  }

  ss <<
    ", " << fNoteIsACueNoteKind;

  ss <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

std::string msrNote::asShortStringForMeasuresSlices () const
{
  std::stringstream ss;

  ss << "[Note, ";

  switch (fNoteKind) {
    case msrNoteKind::kNote_UNKNOWN_:
      ss <<
        "*noNote*";
      break;

    case msrNoteKind::kNoteRestInMeasure:
      ss <<
        "kNoteRestInMeasure, ";

      if (fNoteOccupiesAFullMeasure) {
        ss <<
          "R" <<
          /* JMI
          multipleFullBarRestsWholeNotesAsMsrString (
            fInputLineNumber,
            fSoundingWholeNotes);
            */
          nonSoundingNoteEssentialsAsStringForMeasuresSlices ();
      }
      else {
        ss <<
          "r" <<
          nonSoundingNoteEssentialsAsStringForMeasuresSlices ();
      }

//       ss <<
//         nonSoundingNoteEssentialsAsStringForMeasuresSlices (); JMI v0.9.67
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      ss <<
        "s" <<
        nonSoundingNoteEssentialsAsStringForMeasuresSlices ();
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      ss <<
        "unpitched, " <<
        nonSoundingNoteEssentialsAsStringForMeasuresSlices ();
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      ss <<
       "kNoteRegularInMeasure, " <<
        soundingNoteEssentialsAsStringForMeasuresSlices ();
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      ss <<
        "kNoteInDoubleTremolo, " <<
        soundingNoteEssentialsAsStringForMeasuresSlices ();
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      ss <<
        "kNoteRegularInGraceNotesGroup, " <<
        nonSoundingNoteEssentialsAsStringForMeasuresSlices ();
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      ss <<
        "kNoteSkipInGraceNotesGroup, " <<
        nonSoundingNoteEssentialsAsStringForMeasuresSlices ();
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      ss <<
        "kNoteInChordInGraceNotesGroup, " <<
        soundingNoteEssentialsAsStringForMeasuresSlices () <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteRegularInChord:
      ss <<
       "kNoteRegularInChord, " <<
        soundingNoteEssentialsAsStringForMeasuresSlices ();
      break;

    case msrNoteKind::kNoteRegularInTuplet:
      ss <<
        "kNoteRegularInTuplet, " <<
        soundingNoteEssentialsAsStringForMeasuresSlices ();
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fSoundingWholeNotes,
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ())
            */
      break;

    case msrNoteKind::kNoteRestInTuplet:
      ss <<
        "kNoteRestInTuplet, " <<
        soundingNoteEssentialsAsStringForMeasuresSlices ();
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      ss <<
        "kNoteInTupletInGraceNotesGroup, " <<
        soundingNoteEssentialsAsStringForMeasuresSlices ();
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fSoundingWholeNotes,
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ())
            */

      ss <<
        ", noteTupletFactor: " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      ss <<
        "kNoteUnpitchedInTuplet, " <<
        noteSoundingWholeNotesAsMsrString ();
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fSoundingWholeNotes,
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ())
            */
      ss <<
        ", noteTupletFactor: " << fNoteTupletFactor.asString ();
      break;
  } // switch

  ss <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

std::string msrNote::noteEssentialsAsSting () const
{
  std::stringstream ss;

//   ss <<
//     "fNoteKind: "; // JMI v0.9.67

  switch (fNoteKind) {
    case msrNoteKind::kNote_UNKNOWN_:
      ss <<
        "[kNote_UNKNOWN_]";
      break;

    case msrNoteKind::kNoteRestInMeasure:
      ss <<
        "kNoteRestInMeasure, ";

      if (fNoteOccupiesAFullMeasure) {
        ss <<
          "R" <<
          asShortStringWithRawWholeNotes ();
      }
      else {
        ss <<
          "r" <<
          asShortStringWithRawWholeNotes ();
      }
      ss << std::endl;
        break;

    case msrNoteKind::kNoteSkipInMeasure:
      ss <<
        "kNoteSkipInMeasure, " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      ss <<
        "kNoteUnpitchedInMeasure, " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteRegularInMeasure:
			// ss << "======> noteEssentialsAsSting()" << std::endl;

      ss <<
        "kNoteRegularInMeasure, " <<
        soundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      ss <<
        "kNoteInDoubleTremolo, " <<
        soundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      ss <<
        "kNoteRegularInGraceNotesGroup, " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      ss <<
        "kNoteSkipInGraceNotesGroup, " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      ss <<
        "kNoteInChordInGraceNotesGroup, " <<
        nonSoundingNoteEssentialsAsString () <<
        ", fNoteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteRegularInChord:
      ss <<
        "kNoteRegularInChord, " <<
        soundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteRegularInTuplet:
      ss <<
        "kNoteRegularInTuplet, " <<
        soundingNoteEssentialsAsString ();
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fSoundingWholeNotes,
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ())
            */
      break;

    case msrNoteKind::kNoteRestInTuplet:
      ss <<
        "kNoteRestInTuplet, " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      ss <<
        "kNoteInTupletInGraceNotesGroup, " <<
        nonSoundingNoteEssentialsAsString ();
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fSoundingWholeNotes,
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ())
            */

      ss <<
        ", fNoteTupletFactor: " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      ss <<
        "kNoteUnpitchedInTuplet, " <<
        noteSoundingWholeNotesAsMsrString ();
/* JMI
        noteUpLinkToPart ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fSoundingWholeNotes,
            fNoteShortcutUpLinkToTuplet->getTupletActualNotes (),
            fNoteShortcutUpLinkToTuplet->getTupletNormalNotes ())
            */
      ss <<
        ", noteTupletFactor: " << fNoteTupletFactor.asString ();
      break;
  } // switch

  return ss.str ();
}

void msrNote::print (std::ostream& os) const
{
// 	os << "======> msrNote::print()" << std::endl;

  os <<
    "[Note " <<
    noteEssentialsAsSting () <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 44;

  // print measure position and sounding whole notes
  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasurePosition" << ": " <<
    fMeasurePosition.asString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSoundingWholeNotes" << ": " <<
    fSoundingWholeNotes.asString () <<
    std::endl;

  // print position from beginning of voice
//   os << std::left <<
//     std::setw (fieldWidth) <<
//     "fVoicePosition" << ": " <<
//     fVoicePosition <<
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
        fNoteDisplayWholeNotes.asString () <<
        std::endl;
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      os <<
        std::setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << ": " <<
        fNoteDisplayWholeNotes.asString () <<
        std::endl;
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      os << std::left <<
        std::setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << ": " <<
        fNoteDisplayWholeNotes.asString () <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fNoteGraphicNotesDurationKind" << ": \"" <<
        fNoteGraphicNotesDurationKind <<
        "\"" <<
        std::endl;
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      os <<
        std::setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << ": " <<
        fNoteDisplayWholeNotes.asString () <<
        std::endl <<
        std::setw (fieldWidth) <<
        "tupletSoundingWholeNotes" << ": ";

        if (fNoteShortcutUpLinkToTuplet) {
          os <<
            wholeNotesAsMsrString (
              fInputLineNumber,
              getNoteShortcutUpLinkToTuplet ()->
                getSoundingWholeNotes ());
        }
        else {
          os <<
            "*** unknown yet ***";
        }
        os << std::endl;

      os <<
        std::setw (fieldWidth) <<
        "fNoteTupletFactor" << ": " <<
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

  // print the grace notes group before if any
  if (fNoteGraceNotesGroupBefore) {
    os <<
      std::setw (fieldWidth) <<
      "fNoteGraceNotesGroupBefore";
      os << std::endl;

    ++gIndenter;

    os << fNoteGraceNotesGroupBefore;

    --gIndenter;
  }

//   // print the beams if any
//   if (fNoteBeams.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteBeams";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrBeam>::const_iterator
//       iBegin = fNoteBeams.begin (),
//       iEnd   = fNoteBeams.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the articulations if any
//   if (fNoteArticulations.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteArticulations";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrArticulation>::const_iterator
//       iBegin = fNoteArticulations.begin (),
//       iEnd   = fNoteArticulations.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }

  // print the spanners if any
  if (fNoteSpanners.size ()) {
		os <<
			std::setw (fieldWidth) <<
			"fNoteSpanners";
			os << std::endl;

		++gIndenter;

		std::list<S_msrSpanner>::const_iterator
			iBegin = fNoteSpanners.begin (),
			iEnd   = fNoteSpanners.end (),
			i      = iBegin;
		for ( ; ; ) {
			os << (*i);
			if (++i == iEnd) break;
			// no std::endl here;
		} // for

		--gIndenter;
  }

//   // print the technicals if any
//   if (fNoteTechnicals.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteTechnicals";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrTechnical>::const_iterator
//       iBegin = fNoteTechnicals.begin (),
//       iEnd   = fNoteTechnicals.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the technicals with integer if any
//   if (fNoteTechnicalWithIntegers.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteTechnicalWithIntegers";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrTechnicalWithInteger>::const_iterator
//       iBegin = fNoteTechnicalWithIntegers.begin (),
//       iEnd   = fNoteTechnicalWithIntegers.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the technicals with float if any
//   if (fNoteTechnicalWithFloats.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteTechnicalWithFloats";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrTechnicalWithFloat>::const_iterator
//       iBegin = fNoteTechnicalWithFloats.begin (),
//       iEnd   = fNoteTechnicalWithFloats.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the technicals with string if any
//   if (fNoteTechnicalWithStrings.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteTechnicalWithStrings";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrTechnicalWithString>::const_iterator
//       iBegin = fNoteTechnicalWithStrings.begin (),
//       iEnd   = fNoteTechnicalWithStrings.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the ornaments if any
//   if (fNoteOrnaments.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteOrnaments";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrOrnament>::const_iterator
//       iBegin = fNoteOrnaments.begin (),
//       iEnd   = fNoteOrnaments.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the glissandos if any
//   if (fNoteGlissandos.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteGlissandos";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrGlissando>::const_iterator
//       iBegin = fNoteGlissandos.begin (),
//       iEnd   = fNoteGlissandos.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the slides if any
//   if (fNoteSlides.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteSlides";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrSlide>::const_iterator
//       iBegin = fNoteSlides.begin (),
//       iEnd   = fNoteSlides.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the singleTremolo if any
//   if (fNoteSingleTremolo) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteSingleTremolo";
//       os << std::endl;
//
//     ++gIndenter;
//
//     os << fNoteSingleTremolo;
//
//     --gIndenter;
//   }
//
//   // print the dynamics if any
//   if (fNoteDynamics.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteDynamics";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrDynamic>::const_iterator
//       iBegin = fNoteDynamics.begin (),
//       iEnd   = fNoteDynamics.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the other dynamics if any
//   if (fNoteOtherDynamics.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteOtherDynamics";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrOtherDynamic>::const_iterator
//       iBegin = fNoteOtherDynamics.begin (),
//       iEnd   = fNoteOtherDynamics.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }

  // print the words if any
  if (fNoteWords.size ()) {
    os <<
      std::setw (fieldWidth) <<
      "fNoteWords";
      os << std::endl;

    ++gIndenter;

    std::list<S_msrWords>::const_iterator
      iBegin = fNoteWords.begin (),
      iEnd   = fNoteWords.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }

//   // print the slurs if any
//   if (fNoteSlurs.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteSlurs";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrSlur>::const_iterator
//       iBegin = fNoteSlurs.begin (),
//       iEnd   = fNoteSlurs.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the ligatures if any
//   if (fNoteLigatures.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteLigatures";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrLigature>::const_iterator
//       iBegin = fNoteLigatures.begin (),
//       iEnd   = fNoteLigatures.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the pedals if any
//   if (fNotePedals.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNotePedals";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrPedal>::const_iterator
//       iBegin = fNotePedals.begin (),
//       iEnd   = fNotePedals.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the slashes if any
//   if (fNoteSlashes.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteSlashes";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrSlash>::const_iterator
//       iBegin = fNoteSlashes.begin (),
//       iEnd   = fNoteSlashes.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the crescDecresc if any
//   if (fNoteCrescDecrescs.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteCrescDecrescs";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrCrescDecresc>::const_iterator
//       iBegin = fNoteCrescDecrescs.begin (),
//       iEnd   = fNoteCrescDecrescs.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the wedges if any
//   if (fNoteWedges.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteWedges";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrWedge>::const_iterator
//       iBegin = fNoteWedges.begin (),
//       iEnd   = fNoteWedges.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the segnos if any
//   if (fNoteSegnos.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteSegnos";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrSegno>::const_iterator
//       iBegin = fNoteSegnos.begin (),
//       iEnd   = fNoteSegnos.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the dal segnos if any
//   if (fNoteDalSegnos.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteDalSegnos";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrDalSegno>::const_iterator
//       iBegin = fNoteDalSegnos.begin (),
//       iEnd   = fNoteDalSegnos.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the codas if any
//   if (fNoteCodas.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteCodas";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrCoda>::const_iterator
//       iBegin = fNoteCodas.begin (),
//       iEnd   = fNoteCodas.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the eyeglasses if any
//   if (fNoteEyeGlasses.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteEyeGlasses";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrEyeGlasses>::const_iterator
//       iBegin = fNoteEyeGlasses.begin (),
//       iEnd   = fNoteEyeGlasses.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the damps if any
//   if (fNoteDamps.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteDamps";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrDamp>::const_iterator
//       iBegin = fNoteDamps.begin (),
//       iEnd   = fNoteDamps.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the dampAlls if any
//   if (fNoteDampAlls.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteDampAlls";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrDampAll>::const_iterator
//       iBegin = fNoteDampAlls.begin (),
//       iEnd   = fNoteDampAlls.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }
//
//   // print the scordaturas if any
//   if (fNoteScordaturas.size ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fNoteScordaturas";
//       os << std::endl;
//
//     ++gIndenter;
//
//     std::list<S_msrScordatura>::const_iterator
//       iBegin = fNoteScordaturas.begin (),
//       iEnd   = fNoteScordaturas.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       os << (*i);
//       if (++i == iEnd) break;
//       // no std::endl here;
//     } // for
//
//     --gIndenter;
//   }

  // print the harmonies associated to this note if any
  if (fNoteHarmoniesList.size ()) {
    os <<
      std::setw (fieldWidth) <<
      "fNoteHarmoniesList";
      os << std::endl;

    ++gIndenter;

    std::list<S_msrHarmony>::const_iterator
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
    os <<
      std::setw (fieldWidth) <<
      "fNoteFiguredBassesList";
      os << std::endl;

    ++gIndenter;

    std::list<S_msrFiguredBass>::const_iterator
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
  if (fNoteSyllables.size ()) {
    os <<
      std::setw (fieldWidth) <<
      "fNoteSyllables";
      os << std::endl;

    ++gIndenter;

    std::list<S_msrSyllable>::const_iterator
      iBegin = fNoteSyllables.begin (),
      iEnd   = fNoteSyllables.end (),
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

  // print the after grace group notes after if any
  if (fNoteGraceNotesGroupAfter) {
    os <<
      std::setw (fieldWidth) <<
      "fNoteGraceNotesGroupAfter" <<
      std::endl;

    ++gIndenter;

    os << fNoteGraceNotesGroupAfter;

    --gIndenter;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrNote::printFull (std::ostream& os) const
{
	os << "======> msrNote::printFull()" << std::endl;

  os <<
    "[Note, " <<
    noteEssentialsAsSting () <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 44;

  // print measure position and sounding whole notes
  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasurePosition" << ": " <<
    fMeasurePosition.asString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSoundingWholeNotes" << ": " <<
    fSoundingWholeNotes.asString () <<
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
    "fNoteBelongsToAMultipleFullBarRests" << ": " <<
    fNoteBelongsToAMultipleFullBarRests <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteMultipleFullBarRestsSequenceNumber" << ": " <<
    fNoteMultipleFullBarRestsSequenceNumber <<
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

  os <<
    std::setw (fieldWidth) <<
    "fNoteTrillOrnament" << ": ";
  if (fNoteTrillOrnament) {
    os << std::endl;
    ++gIndenter;

    os <<
      fNoteTrillOrnament;

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

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
    "measureElementMeasureNumber" << ": ";
  if (fNoteUpLinkToMeasure) {
    std::string
      measureNumber =
        fNoteUpLinkToMeasure->
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


//   if (fNoteUpLinkToMeasure->getMeasureNumber () == K_MEASURE_NUMBER_UNKNOWN_) {
//     os <<
//       "[UNKNOWN_MEASURE_NUMBER]";
//   }
//   else if (fNoteUpLinkToMeasure->getMeasureNumber ().size ()) {
//     os <<
//       fNoteUpLinkToMeasure->getMeasureNumber ();
//   }
//   else {
//     std::stringstream ss;
//
//     ss <<
//       "fNoteUpLinkToMeasure->getMeasureNumber () is empty in note " <<
//       this->asString () <<
//       "'";
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
//     "fVoicePosition" << ": " <<
//     fVoicePosition <<
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
    "fNoteUpLinkToMeasure" << ": ";

  if (fNoteUpLinkToMeasure) {
    os << std::endl;

    ++gIndenter;

    os <<
      fNoteUpLinkToMeasure->asShortString () <<
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
      fNoteShortcutUpLinkToGraceNotesGroup->asShortString ();
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
        "fSoundingWholeNotes" << ": " <<
        fSoundingWholeNotes.asString () <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << ": " <<
        fNoteDisplayWholeNotes.asString () <<
        std::endl;
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      os <<
        std::setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << ": " <<
        fNoteDisplayWholeNotes.asString () <<
        std::endl;
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      os <<
        std::setw (fieldWidth) <<
        "fSoundingWholeNotes" << ": " <<
        fSoundingWholeNotes.asString () <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << ": " <<
        fNoteDisplayWholeNotes.asString () <<
        std::endl <<
        std::setw (fieldWidth) <<
        "tupletSoundingWholeNotes" << ": ";

        if (fNoteShortcutUpLinkToTuplet) {
          os <<
            wholeNotesAsMsrString (
              fInputLineNumber,
              getNoteShortcutUpLinkToTuplet ()->
                getSoundingWholeNotes ());
        }
        else {
          os <<
            "*** unknown yet ***";
        }
        os << std::endl;

      os <<
        std::setw (fieldWidth) <<
        "fNoteTupletFactor" << ": " <<
        std::endl;

      ++gIndenter;
        os <<
          fNoteTupletFactor;
      --gIndenter;
        break;
  } // switch

  // print note full measure length
  // may be unknown if there is no time signature
  msrWholeNotes
    measureFullLength =
      fNoteUpLinkToMeasure
        ?
          fNoteUpLinkToMeasure->
            getFullMeasureCurrentWholeNotesDuration ()
        : msrWholeNotes (0, 1); // JMI v0.9.67

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

  // note occupied a full measure?
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteOccupiesAFullMeasure" << ": " <<
    fNoteOccupiesAFullMeasure <<
    std::endl;

  // multiple rest member?
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteBelongsToAMultipleFullBarRests" << ": " <<
    fNoteBelongsToAMultipleFullBarRests <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fNoteMultipleFullBarRestsSequenceNumber" << ": " <<
    fNoteMultipleFullBarRestsSequenceNumber <<
    std::endl;

  // print the grace notes group before if any
  if (fNoteGraceNotesGroupBefore) {
    os <<
      std::setw (fieldWidth) <<
      "fNoteGraceNotesGroupBefore";
    if (fNoteGraceNotesGroupBefore) {
      os << std::endl;

      ++gIndenter;

      os << fNoteGraceNotesGroupBefore;

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

  // cue note???
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNoteIsACueNoteKind" << ": " <<
    fNoteIsACueNoteKind <<
    std::endl;

  // short cuts for efficiency
  os << std::left <<
    std::setw (fieldWidth) <<
    "noteIsAGraceNote" << ": " <<
    getNoteIsAGraceNote () <<
    std::endl;

  // note redundant information (for speed)

  std::stringstream ss;

// JMI  if (fNoteIsStemless) {
    os <<
      std::setw (fieldWidth) <<
      "fNoteIsStemless" <<
      ": " <<
      fNoteIsStemless <<
      std::endl;
// JMI  }

 // JMI if (fNoteIsAChordsFirstMemberNote) {
    os <<
      std::setw (fieldWidth) <<
      "fNoteIsAChordsFirstMemberNote" <<
      ": " <<
      fNoteIsAChordsFirstMemberNote <<
      std::endl;
 // JMI }

 // JMI if (fNoteIsFirstNoteInADoubleTremolo) {
    os <<
      std::setw (fieldWidth) <<
      "fNoteIsFirstNoteInADoubleTremolo" <<
      ": " <<
      fNoteIsFirstNoteInADoubleTremolo <<
      std::endl;
// JMI  }
 // JMI if (fNoteIsSecondNoteInADoubleTremolo) {
    os <<
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

    os <<
      fNoteDashesOrnament;

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

    os <<
      fNoteDelayedTurnOrnament;

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

    os <<
      fNoteDelayedInvertedTurnOrnament;

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

    os <<
      fNoteWavyLineSpannerStart;

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

    os <<
      fNoteWavyLineSpannerStop;

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  if (fNoteIsFollowedByGraceNotesGroup) {
    os <<
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
        "noteSoundingWholeNotesAsMsrString" << ": \"" <<
        noteSoundingWholeNotesAsMsrString () <<
        "\"" <<
        std::endl;
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      os << std::left <<
        std::setw (fieldWidth) <<
        "noteSoundingWholeNotesAsMsrString" << ": \"" <<
        noteSoundingWholeNotesAsMsrString () <<
        "\"" <<
        std::endl;
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      os << std::left <<
        std::setw (fieldWidth) <<
        "noteSoundingWholeNotesAsMsrString" << ": \"" <<
        noteSoundingWholeNotesAsMsrString () <<
        "\"" <<
        std::endl <<
        std::setw (fieldWidth) <<
        "noteDisplayWholeNotesAsMsrString" << ": \"" <<
        noteDisplayWholeNotesAsMsrString () <<
        "\"" <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fNoteGraphicNotesDurationKind" << ": \"" <<
        fNoteGraphicNotesDurationKind <<
        "\"" <<
        std::setw (fieldWidth) <<
        "noteGraphicNotesDurationAsMusicXMLString" << ": \"" <<
        fNoteGraphicNotesDurationKind <<
        "\"" <<
        std::endl;
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      os << std::left <<
        std::setw (fieldWidth) <<
        "noteSoundingWholeNotesAsMsrString" << ": \"" <<
        noteSoundingWholeNotesAsMsrString () <<
        "\"" <<
        std::endl <<
        std::setw (fieldWidth) <<
        "noteDisplayWholeNotesAsMsrString" << ": \"" <<
        noteDisplayWholeNotesAsMsrString () <<
        "\"" <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fNoteGraphicNotesDurationKind" << ": \"" <<
        fNoteGraphicNotesDurationKind <<
        "\"" <<
        std::endl;
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      // JMI
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      os << std::left <<
        std::setw (fieldWidth) <<
        "fNoteGraphicNotesDurationKind" << ": \"" <<
        fNoteGraphicNotesDurationKind <<
        "\"" <<
        std::endl;
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      os << std::left <<
        std::setw (fieldWidth) <<
        "fSoundingWholeNotes" << ": " <<
        fSoundingWholeNotes.asString () <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << ": " <<
        fNoteDisplayWholeNotes.asString () <<
        std::endl <<
        std::setw (fieldWidth) <<
        "fNoteGraphicNotesDurationKind" << ": \"" <<
        fNoteGraphicNotesDurationKind <<
        "\"" <<
        std::endl;
      break;

    case msrNoteKind::kNoteRegularInChord:
      // JMI
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      /* JMI v0.9.67
      os << std::left <<
        std::setw (fieldWidth) <<
        "fNoteTupletNoteGraphicNotesDurationAsMsrString" << ": \"" <<
        fNoteTupletNoteGraphicNotesDurationAsMsrString <<
        "\"" <<
        std::endl <<
        std::setw (fieldWidth) <<
        "noteTupletNoteSoundingWholeNotesAsMsrString" << ": ";
          */

      if (fNoteShortcutUpLinkToTuplet) {
        os <<
          "\"" <<
          wholeNotesAsMsrString (
            fInputLineNumber,
            getNoteShortcutUpLinkToTuplet ()->
              getSoundingWholeNotes ()) <<
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
        "fNoteGraphicNotesDurationKind" << ": \"" <<
        fNoteGraphicNotesDurationKind <<
        "\"" <<
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
    os <<
      fNoteOctaveShift;
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

    os <<
      fNoteStem;

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

  // print the tie if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteTie" << ": ";
  if (fNoteTie) {
    os << std::endl;
    ++gIndenter;

    os <<
      fNoteTie;

    --gIndenter;
  }
  else {
    os << "[NULL]" << std::endl;
  }

  // print the beams if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteBeams";
  if (fNoteBeams.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrBeam>::const_iterator
      iBegin = fNoteBeams.begin (),
      iEnd   = fNoteBeams.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the articulations if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteArticulations";
  if (fNoteArticulations.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrArticulation>::const_iterator
      iBegin = fNoteArticulations.begin (),
      iEnd   = fNoteArticulations.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the spanners if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteSpanners";
  if (fNoteSpanners.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrSpanner>::const_iterator
      iBegin = fNoteSpanners.begin (),
      iEnd   = fNoteSpanners.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the technicals if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteTechnicals";
  if (fNoteTechnicals.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrTechnical>::const_iterator
      iBegin = fNoteTechnicals.begin (),
      iEnd   = fNoteTechnicals.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the technicals with integer if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteTechnicalWithIntegers";
  if (fNoteTechnicalWithIntegers.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrTechnicalWithInteger>::const_iterator
      iBegin = fNoteTechnicalWithIntegers.begin (),
      iEnd   = fNoteTechnicalWithIntegers.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the technicals with float if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteTechnicalWithFloats";
  if (fNoteTechnicalWithFloats.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrTechnicalWithFloat>::const_iterator
      iBegin = fNoteTechnicalWithFloats.begin (),
      iEnd   = fNoteTechnicalWithFloats.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the technicals with string if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteTechnicalWithStrings";
  if (fNoteTechnicalWithStrings.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrTechnicalWithString>::const_iterator
      iBegin = fNoteTechnicalWithStrings.begin (),
      iEnd   = fNoteTechnicalWithStrings.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the ornaments if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteOrnaments";
  if (fNoteOrnaments.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrOrnament>::const_iterator
      iBegin = fNoteOrnaments.begin (),
      iEnd   = fNoteOrnaments.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the glissandos if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteGlissandos";
  if (fNoteGlissandos.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrGlissando>::const_iterator
      iBegin = fNoteGlissandos.begin (),
      iEnd   = fNoteGlissandos.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the slides if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteSlides";
  if (fNoteSlides.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrSlide>::const_iterator
      iBegin = fNoteSlides.begin (),
      iEnd   = fNoteSlides.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the singleTremolo if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteSingleTremolo";
  if (fNoteSingleTremolo) {
    os << std::endl;

    ++gIndenter;

    os << fNoteSingleTremolo;

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
    "fNoteDynamics";
  if (fNoteDynamics.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrDynamic>::const_iterator
      iBegin = fNoteDynamics.begin (),
      iEnd   = fNoteDynamics.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the other dynamics if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteOtherDynamics";
  if (fNoteOtherDynamics.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrOtherDynamic>::const_iterator
      iBegin = fNoteOtherDynamics.begin (),
      iEnd   = fNoteOtherDynamics.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the words if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteWords";
  if (fNoteWords.size ()) {
    os << std::endl;
    ++gIndenter;

    std::list<S_msrWords>::const_iterator
      iBegin = fNoteWords.begin (),
      iEnd   = fNoteWords.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the slurs if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteSlurs";
  if (fNoteSlurs.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrSlur>::const_iterator
      iBegin = fNoteSlurs.begin (),
      iEnd   = fNoteSlurs.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the ligatures if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteLigatures";
  if (fNoteLigatures.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrLigature>::const_iterator
      iBegin = fNoteLigatures.begin (),
      iEnd   = fNoteLigatures.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the pedals if any
  os <<
    std::setw (fieldWidth) <<
    "fNotePedals";
  if (fNotePedals.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrPedal>::const_iterator
      iBegin = fNotePedals.begin (),
      iEnd   = fNotePedals.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the slashes if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteSlashes";
  if (fNoteSlashes.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrSlash>::const_iterator
      iBegin = fNoteSlashes.begin (),
      iEnd   = fNoteSlashes.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the crescDecresc if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteCrescDecrescs";
  if (fNoteCrescDecrescs.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrCrescDecresc>::const_iterator
      iBegin = fNoteCrescDecrescs.begin (),
      iEnd   = fNoteCrescDecrescs.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the wedges if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteWedges";
  if (fNoteWedges.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrWedge>::const_iterator
      iBegin = fNoteWedges.begin (),
      iEnd   = fNoteWedges.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the segnos if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteSegnos";
  if (fNoteSegnos.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrSegno>::const_iterator
      iBegin = fNoteSegnos.begin (),
      iEnd   = fNoteSegnos.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the dal segnos if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteDalSegnos";
  if (fNoteDalSegnos.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrDalSegno>::const_iterator
      iBegin = fNoteDalSegnos.begin (),
      iEnd   = fNoteDalSegnos.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the codas if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteCodas";
  if (fNoteCodas.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrCoda>::const_iterator
      iBegin = fNoteCodas.begin (),
      iEnd   = fNoteCodas.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the eyeglasses if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteEyeGlasses";
  if (fNoteEyeGlasses.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrEyeGlasses>::const_iterator
      iBegin = fNoteEyeGlasses.begin (),
      iEnd   = fNoteEyeGlasses.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the damps if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteDamps";
  if (fNoteDamps.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrDamp>::const_iterator
      iBegin = fNoteDamps.begin (),
      iEnd   = fNoteDamps.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the dampAlls if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteDampAlls";
  if (fNoteDampAlls.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrDampAll>::const_iterator
      iBegin = fNoteDampAlls.begin (),
      iEnd   = fNoteDampAlls.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the scordaturas if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteScordaturas";
  if (fNoteScordaturas.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrScordatura>::const_iterator
      iBegin = fNoteScordaturas.begin (),
      iEnd   = fNoteScordaturas.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here;
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  // print the harmonies associated to this note if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteHarmoniesList";
  if (fNoteHarmoniesList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrHarmony>::const_iterator
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
    os << ": " << "[NONE]" << std::endl;
  }

  // print the figured bass elements associated to this note if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteFiguredBassesList";
  if (fNoteFiguredBassesList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrFiguredBass>::const_iterator
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
    os << ": " << "[NONE]" << std::endl;
  }

  // print the syllables associated to this note if any
  os <<
    std::setw (fieldWidth) <<
    "fNoteSyllables";
  if (fNoteSyllables.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrSyllable>::const_iterator
      iBegin = fNoteSyllables.begin (),
      iEnd   = fNoteSyllables.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrSyllable
        syllable = (*i);

      os << syllable;

/* JMI v0.9.67
      os <<
        syllable->getSyllableKind () <<
          syllable->getSyllableExtendKind () <<
        ": ";

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
      // no std::endl here
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[NONE]" <<
      std::endl;
  }

  --gIndenter;

  os << ']' << std::endl;
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
  S_msrTuplet result;

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
            fetchChordUpLinkToTuplet ();
      }
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      if (fNoteShortcutUpLinkToTuplet) {
        result =
          fNoteShortcutUpLinkToTuplet->
            fetchTupletUpLinkToTuplet ();
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      break;

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
            getChordShortcutUpLinkToGraceNotesGroup ()->
              fetchGraceNoteGroupUpLinkToTuplet ();
      }
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      / * cannot happen ??? JMI
      if (fNoteShortcutUpLinkToTuplet) {
        result =
          fNoteShortcutUpLinkToTuplet->
            getTupletShortcutUpLinkToTuplet ()->
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

