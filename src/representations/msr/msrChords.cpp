/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

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

#include "msrArticulations.h"
#include "msrGlissandos.h"
#include "msrMeasureConstants.h"
#include "msrMusicXMLBasicTypes.h"
#include "msrSlides.h"
#include "msrTechnicals.h"
#include "msrTuplets.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrWae.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

// chords
//______________________________________________________________________________

std::string msrChordInKindAsString (
  msrChordInKind chordInKind)
{
  std::string result;

  switch (chordInKind) {
    case msrChordInKind::kChordIn_UNKNOWN_:
      result = "kChordIn_UNKNOWN_";
      break;
    case msrChordInKind::kChordInMeasure:
      result = "kChordInMeasure";
      break;
    case msrChordInKind::kChordInTuplet:
      result = "kChordInTuplet";
      break;
    case msrChordInKind::kChordInGraceNotesGroup:
      result = "kChordInGraceNotesGroup";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrChordInKind& elt)
{
  os << msrChordInKindAsString (elt);
  return os;
}

//______________________________________________________________________________
S_msrChord msrChord::create (int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a chord" <<
      ", inputLineNumber  " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  msrChord* obj =
    new msrChord (
      inputLineNumber);
  assert (obj != nullptr);
  return obj;
}

// S_msrChord msrChord::create (
//   int                  inputLineNumber,
//   const S_msrMeasure&  upLinkToMeasure)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceChordsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Creating a chord" <<
//       ", inputLineNumber  " << inputLineNumber;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   msrChord* obj =
//     new msrChord (
//       inputLineNumber,
//       upLinkToMeasure);
//   assert (obj != nullptr);
//   return obj;
// }

S_msrChord msrChord::create (
  int                  inputLineNumber,
  const S_msrMeasure&  upLinkToMeasure,
//   const msrWholeNotes& chordSoundingWholeNotes,
  const msrWholeNotes& chordDisplayWholeNotes,
  msrNotesDurationKind chordGraphicNotesDurationKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a chord" <<
//       ", chordSoundingWholeNotes: " <<
//       chordSoundingWholeNotes.asFractionString () <<
      ", chordDisplayWholeNotes: " <<
      chordDisplayWholeNotes.asFractionString () <<
      ", chordGraphicNotesDuration: " <<
      msrNotesDurationKindAsString (chordGraphicNotesDurationKind);

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  msrChord* obj =
    new msrChord (
      inputLineNumber,
      upLinkToMeasure,
//       chordSoundingWholeNotes,
      chordDisplayWholeNotes,
      chordGraphicNotesDurationKind);
  assert (obj != nullptr);
  return obj;
}

// S_msrChord msrChord::create (
//   int                  inputLineNumber,
//   const msrWholeNotes& chordSoundingWholeNotes,
//   const msrWholeNotes& chordDisplayWholeNotes,
//   msrNotesDurationKind chordGraphicNotesDurationKind)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceChordsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Creating a chord" <<
//       ", chordSoundingWholeNotes: " << chordSoundingWholeNotes.asFractionString () <<
//       ", chordDisplayWholeNotes: " << chordDisplayWholeNotes.asFractionString () <<
//       ", chordGraphicNotesDuration: " <<
//       msrNotesDurationKindAsString (chordGraphicNotesDurationKind);
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   return
//     msrChord::create (
//       inputLineNumber,
//       gNullMeasure, // set later in setChordUpLinkToMeasure()
//       chordSoundingWholeNotes,
//       chordDisplayWholeNotes,
//       chordGraphicNotesDurationKind);
// }

msrChord::msrChord (int inputLineNumber)
    : msrTupletElement (
        inputLineNumber)
{
  fChordKind = msrChordInKind::kChordIn_UNKNOWN_;

  // fChordUpLinkToMeasure = upLinkToMeasure;
//   fMeasureElementUpLinkToMeasure = upLinkToMeasure;

  setMeasureElementSoundingWholeNotes (
    msrWholeNotes (0, 1),
    "msrChord::msrChord()");

  fChordDisplayWholeNotes = msrWholeNotes (0, 1);

  fChordGraphicNotesDurationKind =
    msrNotesDurationKind::kNotesDuration_UNKNOWN_;

  fChordIsFirstChordInADoubleTremolo  = false;
  fChordIsSecondChordInADoubleTremolo = false;
}

// msrChord::msrChord (
//   int                  inputLineNumber,
//   const S_msrMeasure&  upLinkToMeasure)
//     : msrTupletElement (
//         inputLineNumber)
// {
//   fChordKind = msrChordInKind::kChordIn_UNKNOWN_;
//
//   // fChordUpLinkToMeasure = upLinkToMeasure;
//   fMeasureElementUpLinkToMeasure = upLinkToMeasure;
//
//   setMeasureElementSoundingWholeNotes (
//     msrWholeNotes (0, 1),
//     "msrChord::msrChord()");
//
//   fChordDisplayWholeNotes  = msrWholeNotes (0, 1);
//
//   fChordGraphicNotesDurationKind =
//     msrNotesDurationKind::kNotesDuration_UNKNOWN_;
//
//   fChordIsFirstChordInADoubleTremolo  = false;
//   fChordIsSecondChordInADoubleTremolo = false;
// }

msrChord::msrChord (
  int                  inputLineNumber,
  const S_msrMeasure&  upLinkToMeasure,
//   const msrWholeNotes& chordSoundingWholeNotes,
  const msrWholeNotes& chordDisplayWholeNotes,
  msrNotesDurationKind chordGraphicNotesDurationKind)
    : msrTupletElement (
        inputLineNumber)
{
  fChordKind = msrChordInKind::kChordIn_UNKNOWN_;

  // fChordUpLinkToMeasure = upLinkToMeasure;
  fMeasureElementUpLinkToMeasure = upLinkToMeasure;

//   setMeasureElementSoundingWholeNotes (
//     chordSoundingWholeNotes,
//     "msrChord::msrChord()");

  fChordDisplayWholeNotes = chordDisplayWholeNotes;

  fChordGraphicNotesDurationKind = chordGraphicNotesDurationKind;

  fChordIsFirstChordInADoubleTremolo  = false;
  fChordIsSecondChordInADoubleTremolo = false;
}

msrChord::~msrChord ()
{}

S_msrChord msrChord::createChordNewbornClone (
  const S_msrPart& containingPart)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChordsBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of chord '" <<
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

  S_msrChord
    newbornClone =
      msrChord::create (
        fInputStartLineNumber,
        gNullMeasure, // set later in setChordUpLinkToMeasure()
//         fMeasureElementSoundingWholeNotes,
        fChordDisplayWholeNotes,
        fChordGraphicNotesDurationKind);

/*
  newbornClone->
    fChordMeasurePosition =
      fChordMeasurePosition;

  newbornClone->
    fChordIsFirstChordInADoubleTremolo =
      fChordIsFirstChordInADoubleTremolo;

  newbornClone->
    fChordIsSecondChordInADoubleTremolo =
      fChordIsSecondChordInADoubleTremolo;
*/

  return newbornClone;
}

// void msrChord::setChordMeasurePosition (
//   const S_msrMeasure measure,
//   const msrWholeNotes&    measurePosition,
//   const std::string&      context)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasurePositions ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Setting chord's measure position of " <<
//       asString () <<
//       " to " <<
//       measurePosition.asString () <<
//       " (was " <<
//       fMeasureElementMeasurePosition.asString () <<
//       ") in measure " <<
//       measure->asShortString () <<
//       " (measureElementMeasureNumber: " <<
//       fMeasureElementUpLinkToMeasure->getMeasureNumber () <<
//       "), context: \"" <<
//       context <<
//       "\"" <<
//       std::endl;
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     measurePosition != K_MEASURE_POSITION_UNKNOWN_,
//     "measurePosition == K_MEASURE_POSITION_UNKNOWN_");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
//   // set chord's measure position
//   fMeasureElementMeasurePosition = measurePosition;
// }

// uplink to measure
// S_msrMeasure msrChord::fetchMeasureElementUpLinkToMeasure () const
// {
//   S_msrMeasure result;
//
//   switch (fChordKind) {
//     case msrChordInKind::kChordIn_UNKNOWN_:
//       break;
//
//     case msrChordInKind::kChordInMeasure:
//       result = fMeasureElementUpLinkToMeasure;
//       break;
//
//     case msrChordInKind::kChordInTuplet:
//       if (fChordShortcutUpLinkToContainingTuplet) {
//         result =
//           fChordShortcutUpLinkToContainingTuplet->
//             fMeasureElementUpLinkToMeasure;
//       }
//       break;
//
//     case msrChordInKind::kChordInGraceNotesGroup:
//       if (fChordUpLinkToContainingGraceNotesGroup) {
//         result =
//           fChordUpLinkToContainingGraceNotesGroup->
//             getGraceNotesGroupUpLinkToNote ()->
//               fMeasureElementUpLinkToMeasure;
//       }
//       break;
//   } // switch
//
//   return result;
// }

// void  msrChord::setMeasureElementUpLinkToMeasure (
//   const S_msrMeasure& measure)
// {
//   setChordUpLinkToMeasure (measure);
// }
//
// S_msrMeasure msrChord::getMeasureElementUpLinkToMeasure () const
// {
//   return getChordUpLinkToMeasure ();
// }

// void msrChord::setChordUpLinkToMeasure (
//   const S_msrMeasure& measure)
// {
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, __LINE__,
//     measure != nullptr,
//     "measure is null");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceChords ()) {
//     ++gIndenter;
//
//     std::stringstream ss;
//
//     ss <<
//       "Setting the uplink to measure of chord " <<
//       asString () <<
//       " to measure " << measure->asString () <<
//       "' in measure '" <<
//       measure->asString () <<
//       std::endl;
//
//     --gIndenter;
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fChordUpLinkToMeasure = measure;
// }
//
// S_msrMeasure msrChord::getChordUpLinkToMeasure () const
// {
//   return fChordUpLinkToMeasure;
// }
//
// // uplink to tuplet
// S_msrTuplet msrChord::fetchChordUpLinkToTuplet () const
// {
//   const S_msrTuplet& result;
//
//   switch (fChordKind) {
//     case msrChordInKind::kChordIn_UNKNOWN_:
//       break;
//
//     case msrChordInKind::kChordInMeasure:
//       break;
//
//     case msrChordInKind::kChordInTuplet:
//       result = fChordShortcutUpLinkToContainingTuplet;
//       break;
//
//     case msrChordInKind::kChordInGraceNotesGroup:
//       if (fChordUpLinkToContainingGraceNotesGroup) {
//         result =
//           fChordUpLinkToContainingGraceNotesGroup->
//             getGraceNotesGroupUpLinkToNote ()->
//               getNoteShortcutUpLinkToTuplet ();
//       }
//       break;
//   } // switch
//
//   return result;
// }

// // uplink to grace notes group
// S_msrGraceNotesGroup msrChord::fetchChordUpLinkToGraceNotesGroup () const
// {
//   S_msrGraceNotesGroup result;
//
//   switch (fChordKind) {
//     case msrChordInKind::kChordIn_UNKNOWN_:
//       break;
//     case msrChordInKind::kChordInMeasure:
//       break;
//     case msrChordInKind::kChordInTuplet:
//       break;
//     case msrChordInKind::kChordInGraceNotesGroup:
//       result = fChordUpLinkToContainingGraceNotesGroup; // JMI v0.9.71
//       break;
//   } // switch
//
//   return result;
// }

// uplink to tuplet
void msrChord::setChordShortcutUpLinkToContainingTuplet (
  const S_msrTuplet& tuplet)
{
  fChordShortcutUpLinkToContainingTuplet = tuplet;
}

S_msrTuplet msrChord::getChordShortcutUpLinkToContainingTuplet () const
{
  return fChordShortcutUpLinkToContainingTuplet;
}

// // uplink to grace notes group
// void msrChord::setChordUpLinkToContainingGraceNotesGroup (
//   const S_msrGraceNotesGroup& graceNotesGroup)
// {
//   fChordUpLinkToContainingGraceNotesGroup =
//     graceNotesGroup;
// }

// score upLink
S_msrScore msrChord::fetchChordUpLinkToScore () const
{
  S_msrScore result;

  S_msrMeasure
    measure =
//       fChordUpLinkToMeasure;
      fMeasureElementUpLinkToMeasure;

  if (measure) {
    result =
      measure ->
        fetchMeasureUpLinkToScore ();
  }

  return result;
}

// void msrChord::setChordSoundingWholeNotes (
//    const msrWholeNotes& wholeNotes)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceChords ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Setting chord sounding whole notes to '" <<
//       wholeNotes.asString () <<
//       "' for chord '" <<
//       asString () <<
//       "'";
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fChordSoundingWholeNotes = wholeNotes;
// }

void msrChord::setChordDisplayWholeNotes (
   const msrWholeNotes& wholeNotes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Setting chord displayed whole notes to '" <<
      wholeNotes.asString () <<
      "' for chord '" <<
      asString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordDisplayWholeNotes = wholeNotes;
}

void msrChord::setChordGraceNotesGroupLinkBefore (
  int     inputLineNumber,
  const S_msrChordGraceNotesGroupLink&
          chordChordGraceNotesGroupLinkBefore)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Setting chord grace notes groups before in " <<
      asString () <<
      " to " <<
      chordChordGraceNotesGroupLinkBefore->asShortString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordGraceNotesGroupLinkBefore =
    chordChordGraceNotesGroupLinkBefore;
}

void msrChord::setChordGraceNotesGroupLinkAfter (
  int     inputLineNumber,
  const S_msrChordGraceNotesGroupLink&
          chordChordGraceNotesGroupLinkAfter)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Setting chord grace notes groups after in " <<
      asString () <<
      " to " <<
      chordChordGraceNotesGroupLinkAfter->asShortString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordGraceNotesGroupLinkAfter =
    chordChordGraceNotesGroupLinkAfter;
}

// void msrChord::setMeasureElementMeasurePosition (
//   const S_msrMeasure& measure,
//   const msrWholeNotes&     measurePosition,
//   const std::string&  context)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceMeasurePositions ()) {
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
//       measurePosition.asString () <<
//       " (was '" <<
//       fMeasureElementMeasurePosition.asString () <<
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
//   // handle the chord itself
//   msrMeasureElement::setMeasureElementMeasurePosition (
//     measure,
//     measurePosition,
//     context);
//
//   // handle its members
//   setChordMembersMeasurePosition (
//     measure,
//     measurePosition);
// }

void msrChord::setChordMembersMeasurePosition (
  const S_msrMeasure&  measure,
  const msrWholeNotes& measurePosition)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasurePositions ()) {
    std::stringstream ss;

    ss <<
      "Setting chord members measure positions of " << asString () <<
      " to " <<
      measurePosition.asString () <<
      ", fChordNotesVector.size(): " << fChordNotesVector.size ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string context =
    "setChordMembersMeasurePosition()";

  // set the chord's elements' measure position
  for (S_msrNote note : fChordNotesVector) {
    // set note's uplink to measure
    note->
//       setNoteUpLinkToMeasure (
      setMeasureElementUpLinkToMeasure (
        measure);

    // set note's measure position // JMI v0.9.66
    note->
      setMeasureElementMeasurePosition (
        measure,
        measurePosition, // they all share the same one
        "setChordMembersMeasurePosition()");

//    // set note's voice position JMI v0.9.66
//       note->
//         setMeasureElementVoicePosition (
//           voicePosition,
//           context); // they all share the same one
  } // for

//   // are there dal segnos attached to this chord?
//   if (fChordDalSegnosList.size ()) {
//     for (S_msrDalSegno dalSegno : fChordDalSegnosList) {
//       // set the dal segno measure position
// //       dalSegno->
// //         setDalSegnoMeasurePosition (
// //           measure,
// //           measurePosition,
// //           "msrChord::setChordMembersMeasurePosition()"); // JMI v0.9.66
//     } // for
}

void msrChord::setupNoteAsFirstInChord (
  const S_msrNote&  note)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Setting up note " <<
      note->asString () <<
      " as first note in chord " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // mark note as being the first one in the chord
  note->
    setNoteIsAChordsFirstMemberNote ();

  // has this note an msrStem attached to it? // JMI ??? v0.9.72
  S_msrStem
    noteStem =
      note->getNoteStem ();

  if (noteStem) {
    appendStemToChord (noteStem);
  }
}

void msrChord::addNoteToChord (
  const S_msrNote&  note,
  const S_msrVoice& voice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Adding note " <<
      note->asShortString () <<
      " to chord " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   gLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
//   print (gLog); // JMI v0.9.66 ???
//   gLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;

  // append note to chord notes vector
  fChordNotesVector.push_back (note);

  // register note's uplink to chord
  note->
    setNoteShortcutUpLinkToChord (this);

  // mark note as belonging to a chord
  note->setNoteBelongsToAChord ();

  // is this note the shortest one in this voice? // JMI ??? v0.9.72
  voice->
    registerShortestNoteInVoiceIfRelevant (
      note);

  // append the note to the measure's notes flat list
//   if (false) // JMI v0.9.70
//   fMeasureElementUpLinkToMeasure->
//     appendNoteToMeasureNotesFlatList (note);

/* JMI
  // register note as the last appended one into this voice
  voice->
    registerNoteAsVoiceLastAppendedNote (note);
*/
}

S_msrNote msrChord::fetchChordFirstNonGraceNote () const
{
  S_msrNote result;

  if (fChordNotesVector.size ()) {
    result = fChordNotesVector.front ();
  }

  else {
    msrInternalError (
      gServiceRunData->getInputSourceName (),
      fInputStartLineNumber,
      __FILE__, __LINE__,
      "cannot access the first note of an empty chord");
  }

  return result;
}

void msrChord::appendDalSegnoToChord (
  const S_msrDalSegno& dalSegno)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceDalSegnos ()) {
    std::stringstream ss;

    ss <<
      "Appending dal segno " <<
      dalSegno->asShortString () <<
      " to chord " <<
      asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordDalSegnosList.push_back (dalSegno);
}

void msrChord::appendArticulationToChord (
  const S_msrArticulation& art)
{
  msrArticulationKind
    articulationKind =
      art->
        getArticulationKind ();

  // don't append the same articulation several times
  for (S_msrArticulation articulation : fChordArticulationsList) {
    if (articulation->getArticulationKind () == articulationKind)
      return;
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceArticulations ()) {
    std::stringstream ss;

    ss <<
      "Appending articulation '" <<
      art->getArticulationKind () <<
      "' to chord";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordArticulationsList.push_back (art);
}

void msrChord::appendSpannerToChord (
  const S_msrSpanner& span)
{
  msrSpannerKind
    spannerKind =
      span->
        getSpannerKind ();

  // don't append the same spanner several times
  for (S_msrSpanner spanner : fChordSpannersList) {
    if (spanner->getSpannerKind () == spannerKind)
      return;
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSpanners ()) {
    std::stringstream ss;

    ss <<
      "Appending spanner " <<
      span->asString () <<
      " to chord " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordSpannersList.push_back (span);
}

void msrChord::setChordSingleTremolo (
  const S_msrSingleTremolo& trem)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTremolos ()) {
    std::stringstream ss;

    ss <<
      "Adding singleTremolo '" <<
      trem->asString () <<
      "' to chord";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordSingleTremolo = trem;
}

void msrChord::appendTechnicalToChord (
  const S_msrTechnical& tech)
{
  msrTechnicalKind
    technicalKind =
      tech->
        getTechnicalKind ();

  // don't append the same technical several times
  for (
    std::list <S_msrTechnical>::const_iterator i =
      fChordTechnicalsList.begin ();
    i != fChordTechnicalsList.end ();
    ++i
  ) {
      if ((*i)->getTechnicalKind () == technicalKind)
        return;
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Appending technical '" <<
      tech->asString () <<
      "' to chord";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordTechnicalsList.push_back (tech);
}

void msrChord::appendTechnicalWithIntegerToChord (
  const S_msrTechnicalWithInteger& tech)
{
  msrTechnicalWithIntegerKind
    technicalWithIntegerKind =
      tech->
        getTechnicalWithIntegerKind ();

  // don't append the same technical several times
  for (
    std::list <S_msrTechnicalWithInteger>::const_iterator i =
      fChordTechnicalWithIntegersList.begin ();
    i != fChordTechnicalWithIntegersList.end ();
    ++i
  ) {
      if ((*i)->getTechnicalWithIntegerKind () == technicalWithIntegerKind)
        return;
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Appending technical with integer '" <<
      tech->asString () <<
      "' to chord";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordTechnicalWithIntegersList.push_back (tech);
}

void msrChord::appendTechnicalWithFloatToChord (
  const S_msrTechnicalWithFloat& tech)
{
  msrTechnicalWithFloatKind
    technicalWithFloatKind =
      tech->
        getTechnicalWithFloatKind ();

  // don't append the same technical several times
  for (
    std::list <S_msrTechnicalWithFloat>::const_iterator i =
      fChordTechnicalWithFloatsList.begin ();
    i != fChordTechnicalWithFloatsList.end ();
    ++i
  ) {
      if ((*i)->getTechnicalWithFloatKind () == technicalWithFloatKind)
        return;
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Appending technical with integer '" <<
      tech->asString () <<
      "' to chord";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordTechnicalWithFloatsList.push_back (tech);
}

void msrChord::appendTechnicalWithStringToChord (
  const S_msrTechnicalWithString& tech)
{
  msrTechnicalWithStringKind
    technicalWithStringKind =
      tech->
        getTechnicalWithStringKind ();

  // don't append the same technical several times
  for (
    std::list <S_msrTechnicalWithString>::const_iterator i =
      fChordTechnicalWithStringsList.begin ();
    i != fChordTechnicalWithStringsList.end ();
    ++i
  ) {
      if ((*i)->getTechnicalWithStringKind () == technicalWithStringKind)
        return;
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Appending technical with string '" <<
      tech->asString () <<
      "' to chord";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordTechnicalWithStringsList.push_back (tech);
}

void msrChord::appendOrnamentToChord (
  const S_msrOrnament& orn)
{
  msrOrnamentKind
    ornamentKind =
      orn->
        getOrnamentKind ();

  // don't append the same ornament several times
  for (S_msrOrnament ornament : fChordOrnamentsList) {
      if (ornament->getOrnamentKind () == ornamentKind)
        return;
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Appending ornament '" <<
      msrOrnamentKindAsString (orn->getOrnamentKind ()) <<
      "' to chord";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordOrnamentsList.push_back (orn);
}

void msrChord::appendGlissandoToChord (
  const S_msrGlissando& gliss)
{
  msrGlissandoTypeKind
    glissandoTypeKind =
      gliss->
        getGlissandoTypeKind ();

  // don't append the same slissando several times
  for (S_msrGlissando glissando : fChordGlissandosList) {
    if (glissando->getGlissandoTypeKind () == glissandoTypeKind)
      return;
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Appending glissando '" <<
      gliss->asString () <<
      "' to chord";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordGlissandosList.push_back (gliss);
}

void msrChord::appendSlideToChord (
  const S_msrSlide& slide)
{
  msrSlideTypeKind
    slideTypeKind =
      slide->
        getSlideTypeKind ();

  // don't append the same slide several times
  for (
    std::list <S_msrSlide>::const_iterator i = fChordSlidesList.begin ();
    i != fChordSlidesList.end ();
    ++i
  ) {
      if ((*i)->getSlideTypeKind () == slideTypeKind)
        return;
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Appending slide '" <<
      slide->asString () <<
      "' to chord";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordSlidesList.push_back (slide);
}

void msrChord::appendChordSlurLinkToChord (
  const S_msrChordSlurLink& chordSlurLink)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceSlurs ()) {
    std::stringstream ss;

    ss <<
      "Adding slur link '" << chordSlurLink->asString() <<
      "' to chord '" << asString () << "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordSlurLinksList.push_back (chordSlurLink);
}

void msrChord::appendStemToChord (
  const S_msrStem& stem)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceStems ()) {
    std::stringstream ss;

    ss <<
      "Adding stem '" << stem->asString() <<
      "' to chord '" << asString () << "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  for (
    std::list <S_msrStem>::const_iterator i = fChordStemsList.begin ();
    i != fChordStemsList.end ();
    ++i
  ) {
    // is stem's stem kind consistent with the other ones' for this chord?
    if (stem->getStemKind () != (*i)->getStemKind ()) {
      std::stringstream ss;

      ss <<
        "stem " << stem->asString () <<
        " and " << (*i)->asString () <<
        " don't have the same kind, but are in one and the same chord";

//      msrInternalError ( // not internal actually JMI ???
      msrInternalWarning ( // not internal actually JMI ???
        gServiceRunData->getInputSourceName (),
        fInputStartLineNumber,
  //      __FILE__, __LINE__,
        ss.str ());
    }
  } // for
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fChordStemsList.push_back (stem);
}

void msrChord::appendBeamToChord (const S_msrBeam& beam)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBeams ()) {
    std::stringstream ss;

    ss <<
      "Appending beam '" <<
      beam->asString () <<
      "' to chord";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordBeamsList.push_back (beam);
}

void msrChord::appendChordBeamLinkToChord (
  const S_msrChordBeamLink& chordBeamLink)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBeams ()) {
    std::stringstream ss;

    ss <<
      "Adding beam link " << chordBeamLink->asString() <<
      " to chord " << asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordBeamLinksList.push_back (chordBeamLink);
}

// void msrChord::finalizeChord (
//   int inputLineNumber)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceChordsBasics ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Finalizing chord '" <<
//       asString () <<
//       "', line " << inputLineNumber <<
//       std::endl <<
//       "fMeasureElementMeasurePosition: " <<
//       std::endl <<
//       fMeasureElementMeasurePosition.asString ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, __LINE__,
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   // we can now set the measure positions for all the chord members JMI v0.9.66
//   if (false) // JMI v0.9.67 MERDUM
//   setChordMembersMeasurePosition (
//     fMeasureElementUpLinkToMeasure,
//     fMeasureElementMeasurePosition);
// }

void msrChord::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrChord::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrChord::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrChord::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrChord::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrChord::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrChord::browseData (basevisitor* v)
{
  // should this chord be browsed?
  // fetch the score
  S_msrScore
    score =
      fetchChordUpLinkToScore ();

  if (score) {
    Bool
      inhibitChordsInGraceNotesGroupsBrowsing =
        score->
          getInhibitChordsInGraceNotesGroupsBrowsing ();

    if (inhibitChordsInGraceNotesGroupsBrowsing) {
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
          "% ==> visiting grace notes groups before chords is inhibited";

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      return;
    }
  }

  if (fChordGraceNotesGroupLinkBefore) {
    // browse the grace notes group before note
    msrBrowser<msrChordGraceNotesGroupLink> browser (v);
    browser.browse (*fChordGraceNotesGroupLinkBefore);
  }

  for (
    std::vector <S_msrNote>::const_iterator i = fChordNotesVector.begin ();
    i != fChordNotesVector.end ();
    ++i
  ) {
    // browse chord note
    msrBrowser<msrNote> browser (v);
    browser.browse (*(*i));
  } // for


  for (
    std::list <S_msrStem>::const_iterator i = fChordStemsList.begin ();
    i != fChordStemsList.end ();
    ++i
  ) {
    // browse chord stems
    msrBrowser<msrStem> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list <S_msrBeam>::const_iterator i = fChordBeamsList.begin ();
    i != fChordBeamsList.end ();
    ++i
  ) {
    // browse chord beams
    msrBrowser<msrBeam> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list <S_msrChordBeamLink>::const_iterator i = fChordBeamLinksList.begin ();
    i != fChordBeamLinksList.end ();
    ++i
  ) {
    // browse the beam link
    msrBrowser<msrChordBeamLink> browser (v);
    browser.browse (*(*i));
  } // for


  for (
    std::list <S_msrArticulation>::const_iterator i = fChordArticulationsList.begin ();
    i != fChordArticulationsList.end ();
    ++i
  ) {
    // browse the articulation
    msrBrowser<msrArticulation> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list <S_msrSpanner>::const_iterator i = fChordSpannersList.begin ();
    i != fChordSpannersList.end ();
    ++i
  ) {
    // browse the spanner
    msrBrowser<msrSpanner> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list <S_msrTechnical>::const_iterator i = fChordTechnicalsList.begin ();
    i != fChordTechnicalsList.end ();
    ++i
  ) {
    // browse the technical
    msrBrowser<msrTechnical> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list <S_msrOrnament>::const_iterator i = fChordOrnamentsList.begin ();
    i != fChordOrnamentsList.end ();
    ++i
  ) {
    // browse the ornament
    msrBrowser<msrOrnament> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list <S_msrGlissando>::const_iterator i = fChordGlissandosList.begin ();
    i != fChordGlissandosList.end ();
    ++i
  ) {
    // browse the glissando
    msrBrowser<msrGlissando> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list <S_msrSlide>::const_iterator i = fChordSlidesList.begin ();
    i != fChordSlidesList.end ();
    ++i
  ) {
    // browse the slide
    msrBrowser<msrSlide> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list <S_msrDynamic>::const_iterator i = fChordDynamicsList.begin ();
    i != fChordDynamicsList.end ();
    ++i
  ) {
    // browse the dynamic
    msrBrowser<msrDynamic> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list <S_msrOtherDynamic>::const_iterator i = fChordOtherDynamicsList.begin ();
    i != fChordOtherDynamicsList.end ();
    ++i
  ) {
    // browse the other dynamic
    msrBrowser<msrOtherDynamic> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list <S_msrWords>::const_iterator i = fChordWordsList.begin ();
    i != fChordWordsList.end ();
    ++i
  ) {
    // browse the words
    msrBrowser<msrWords> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list <S_msrTie>::const_iterator i = fChordTiesList.begin ();
    i != fChordTiesList.end ();
    ++i
  ) {
    // browse the tie
    msrBrowser<msrTie> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list <S_msrLigature>::const_iterator i = fChordLigaturesList.begin ();
    i != fChordLigaturesList.end ();
    ++i
  ) {
    // browse the ligature
    msrBrowser<msrLigature> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list <S_msrSlash>::const_iterator i = fChordSlashesList.begin ();
    i != fChordSlashesList.end ();
    ++i
  ) {
    // browse the slash
    msrBrowser<msrSlash> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list <S_msrCrescDecresc>::const_iterator i = fChordCrescDecrescsList.begin ();
    i != fChordCrescDecrescsList.end ();
    ++i
  ) {
    // browse the cresc/decresc
    msrBrowser<msrCrescDecresc> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list <S_msrWedge>::const_iterator i = fChordWedgesList.begin ();
    i != fChordWedgesList.end ();
    ++i
  ) {
    // browse the wedge
    msrBrowser<msrWedge> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list <S_msrSegno>::const_iterator i = fChordSegnosList.begin ();
    i != fChordSegnosList.end ();
    ++i
  ) {
    // browse the segno
    msrBrowser<msrSegno> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list <S_msrDalSegno>::const_iterator i = fChordDalSegnosList.begin ();
    i != fChordDalSegnosList.end ();
    ++i
  ) {
    // browse the dal segno
    msrBrowser<msrDalSegno> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list <S_msrCoda>::const_iterator i = fChordCodasList.begin ();
    i != fChordCodasList.end ();
    ++i
  ) {
    // browse the coda
    msrBrowser<msrCoda> browser (v);
    browser.browse (*(*i));
  } // for

  if (fChordOctaveShift) {
    // browse the octave shift
    msrBrowser<msrOctaveShift> browser (v);
    browser.browse (*fChordOctaveShift);
  }

  // browse the harmonies if any
  if (fChordHarmoniesList.size ()) {
    std::list <S_msrHarmony>::const_iterator i;
    for (i = fChordHarmoniesList.begin (); i != fChordHarmoniesList.end (); ++i) {
      // browse the harmony
      msrBrowser<msrHarmony> browser (v);
      browser.browse (*(*i));
    } // for
  }

  if (fChordFiguredBassesList) {
    // browse the figured bass
    msrBrowser<msrFiguredBass> browser (v);
    browser.browse (*fChordFiguredBassesList);
  }

  for (
    std::list <S_msrChordSlurLink>::const_iterator i = fChordSlurLinksList.begin ();
    i != fChordSlurLinksList.end ();
    ++i
  ) {
    // browse the slur link
    msrBrowser<msrChordSlurLink> browser (v);
    browser.browse (*(*i));
  } // for

/*
  if (fChordGraceNotesGroupBefore) {
    // browse the grace notes group before note
    msrBrowser<msrGraceNotesGroup> browser (v);
    browser.browse (*fChordGraceNotesGroupBefore);
  }
  if (fChordGraceNotesGroupAfter) {
    // browse the grace notes group after note
    msrBrowser<msrGraceNotesGroup> browser (v);
    browser.browse (*fChordGraceNotesGroupAfter);
  }
*/

  if (fChordGraceNotesGroupLinkAfter) {
    // browse the grace notes group after note
    msrBrowser<msrChordGraceNotesGroupLink> browser (v);
    browser.browse (*fChordGraceNotesGroupLinkAfter);
  }
}

/* JMI
void msrChord::applyTupletMemberDisplayFactorToChordMembers (
  int actualNotes, int normalNotes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Applying tuplet member sounding factor '" <<
      actualNotes << '/' << normalNotes <<
      "' to the members of chord '" << asStringwithRawDivisions () <<
      "', line " << fInputStartLineNumber <<
      std::endl;

  for (
    std::vector <S_msrNote>::const_iterator i = fChordNotesVector.begin ();
    i != fChordNotesVector.end ();
    ++i) {
    (*i)->
      applyTupletMemberSoundingFactorToNote (
        actualNotes, normalNotes);
  } // for
}
#endif // MF_TRACE_IS_ENABLED
*/

std::string msrChord::asStringwithRawDivisions () const // SUPERFLOUS??? JMI v0.9.70
{
  std::stringstream ss;

  ss << "<";

  if (fChordNotesVector.size ()) {
    std::vector <S_msrNote>::const_iterator
      iBegin = fChordNotesVector.begin (),
      iEnd   = fChordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote
        note = (*i);

      ss <<
      /* JMI
        note->notePitchAsString () <<
        note->noteSoundingWholeNotesPitchAndOctaveAsString () <<
        '[' << note->getNoteOctave () << ']'
        */

        note->asShortStringWithRawWholeNotes ();

      if (++i == iEnd) break;
      ss << ' ';
    } // for
  }

  ss << ">";

  return ss.str ();
}

std::string msrChord::asString () const
{
  std::stringstream ss;

  ss <<
    "[Chord asString ()" << // JMI v0.9.71
    ", fChordKind: " << fChordKind <<
    ", line " << fInputStartLineNumber <<
    " <";

  if (fChordNotesVector.size ()) {
    std::vector <S_msrNote>::const_iterator
      iBegin = fChordNotesVector.begin (),
      iEnd   = fChordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote note = (*i);

      ss <<
        note->notePitchAsString () <<
        " sounding: " <<
        note->getMeasureElementSoundingWholeNotes ().asFractionString () <<
        ", display: " <<
        note->getNoteDisplayWholeNotes ().asFractionString () <<
        ", octave: " <<
        note->getNoteOctaveKind () <<
        ']';

      if (++i == iEnd) break;
      ss << ' ';
    } // for
  }

  ss << ">]";

  return ss.str ();
}

std::string msrChord::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[Chord asShortString ()" << // JMI v0.9.71
    ", " << fChordKind <<
    ", line " << fInputStartLineNumber <<
    ", <";

  if (fChordNotesVector.size ()) {
    std::vector <S_msrNote>::const_iterator
      iBegin = fChordNotesVector.begin (),
      iEnd   = fChordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote
        note = (*i);

      ss <<
        note->notePitchAsString () <<
        " sounding: " <<
        note->getMeasureElementSoundingWholeNotes ().asFractionString () <<
        ", display: " <<
        note->getNoteDisplayWholeNotes ().asFractionString () <<
        ", octave: " <<
        note->getNoteOctaveKind ();

      if (++i == iEnd) break;
      ss << ", ";
    } // for
  }

  ss << "> ]";

  return ss.str ();
}

void msrChord::print (std::ostream& os) const
{
  msrWholeNotes
    chordMeasureFullLength =
      fMeasureElementUpLinkToMeasure
        ? fMeasureElementUpLinkToMeasure->
            getFullMeasureWholeNotesDuration ()
        : msrWholeNotes (0, 1); // JMI

  os <<
    "[Chord" <<
    ", line " << fInputStartLineNumber <<
    ", " <<
    mfSingularOrPlural (
      fChordNotesVector.size (), "note", "notes") <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 44;

  os << std::left <<
    std::setw (fieldWidth) <<
 // JMI   "chordSoundingWholeNotes" << ": " << fChordSoundingWholeNotes << v0.9.72
//     "fMeasureElementSoundingWholeNotes" << ": " <<
//     fMeasureElementSoundingWholeNotes <<
    "fMeasureElementSoundingWholeNotes" << ": " <<
    fMeasureElementSoundingWholeNotes.asFractionString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fChordDisplayWholeNotes" << ": " <<
    fChordDisplayWholeNotes <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "chordUpLinkToMeasureMeasureNumber" << ": ";
    if (fMeasureElementUpLinkToMeasure) {
      os <<
        fMeasureElementUpLinkToMeasure->getMeasureNumber ();
    }
    else {
      os << "[UNKNOWN_MEASURE_NUMBER]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureElementMeasurePosition" << ": " << fMeasureElementMeasurePosition.asString () <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     "fMeasureElementVoicePosition" << ": " << fMeasureElementVoicePosition <<
//     std::endl <<
    std::setw (fieldWidth) <<
    "chordMeasureFullLength" << ": " << chordMeasureFullLength <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fChordShortcutUpLinkToContainingTuplet" << ": " <<
    fChordShortcutUpLinkToContainingTuplet <<
    std::endl;
/*
  os <<
    std::setw (fieldWidth) <<
    "fChordShortcutUpLinkToContainingTuplet" << ": ";
  if (fChordShortcutUpLinkToContainingTuplet) {
    os << fChordShortcutUpLinkToContainingTuplet->asString (); // JMI v0.9.71
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;
*/

  os <<
    std::setw (fieldWidth) <<
    "fPositionInTuplet" << ": " <<
    fPositionInTuplet <<
    std::endl;

  // print the chord grace notes group link before if any
  os <<
    std::setw (fieldWidth) <<
    "fChordGraceNotesGroupLinkBefore";
  if (fChordGraceNotesGroupLinkBefore) {
    os << std::endl;
    gIndenter++;
    os <<
      fChordGraceNotesGroupLinkBefore;
    gIndenter--;
  }
  else {
    os << ": " << "[NONE]" << std::endl;
  }

/*
  // print the articulations if any
  size_t chordArticulationsListSize = fChordArticulationsList.size ();

  if (chordArticulationsListSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordArticulationsList";
    if (chordArticulationsListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_msrArticulation>::const_iterator i;
      for (i = fChordArticulationsList.begin (); i != fChordArticulationsList.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }

  // print the spanners if any
  size_t chordSpannersListSize = fChordSpannersList.size ();

  if (chordSpannersListSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordSpannersList";
    if (chordSpannersListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_msrSpanner>::const_iterator i;
      for (i = fChordSpannersList.begin (); i != fChordSpannersList.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }

  // print the technicals if any
  size_t chordTechnicalsListSize = fChordTechnicalsList.size ();

  if (chordTechnicalsListSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordTechnicalsList";
    if (chordTechnicalsListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_msrTechnical>::const_iterator i;
      for (i = fChordTechnicalsList.begin (); i != fChordTechnicalsList.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }

  // print the ornaments if any
  size_t chordOrnamentsListSize = fChordOrnamentsList.size ();

  if (chordOrnamentsListSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordOrnamentsList";
    if (chordOrnamentsListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_msrOrnament>::const_iterator i;
      for (i = fChordOrnamentsList.begin (); i != fChordOrnamentsList.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }

  // print the glissandos if any
  size_t chordGlissandosListSize = fChordGlissandosList.size ();

  if (chordGlissandosListSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordGlissandosList";
    if (chordGlissandosListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_msrGlissando>::const_iterator i;
      for (i = fChordGlissandosList.begin (); i != fChordGlissandosList.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }

  // print the slides if any
  size_t chordSlidesListSize = fChordSlidesList.size ();

  if (chordSlidesListSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordSlidesList";
    if (chordSlidesListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_msrSlide>::const_iterator i;
      for (i = fChordSlidesList.begin (); i != fChordSlidesList.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }

  // print the dynamics if any
  size_t chordDynamicsListSize = fChordDynamicsList.size ();

  if (chordDynamicsListSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordDynamicsList";
    if (chordDynamicsListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_msrDynamic>::const_iterator i;
      for (i = fChordDynamicsList.begin (); i != fChordDynamicsList.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }

  // print the other dynamics if any
  size_t chordOtherDynamicsListSize = fChordOtherDynamicsList.size ();

  if (chordOtherDynamicsListSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordOtherDynamicsList";
    if (chordOtherDynamicsListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_msrOtherDynamic>::const_iterator i;
      for (i = fChordOtherDynamicsList.begin (); i != fChordOtherDynamicsList.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }
*/

  // print the stems if any
  size_t chordStemsListSize = fChordStemsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordStemsList";
  if (chordStemsListSize) {
    os << std::endl;
    ++gIndenter;

    std::list <S_msrStem>::const_iterator i;
    for (i = fChordStemsList.begin (); i != fChordStemsList.end (); ++i) {
      os << (*i);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

  // print the beams if any
  size_t chordBeamsListSize = fChordBeamsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordBeamsList";
  if (chordBeamsListSize) {
    os << std::endl;
    ++gIndenter;

    std::list <S_msrBeam>::const_iterator i;
    for (i = fChordBeamsList.begin (); i != fChordBeamsList.end (); ++i) {
      os << (*i);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

#ifdef MF_TRACE_IS_ENABLED
  // print the beam links if any
  size_t chordBeamLinksListSize = fChordBeamLinksList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordBeamLinksList";
//   if (gTraceOahGroup->getTraceBeams () || chordBeamLinksListSize) {
    if (chordBeamLinksListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_msrChordBeamLink>::const_iterator i;
      for (i = fChordBeamLinksList.begin (); i != fChordBeamLinksList.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
//   }
#endif // MF_TRACE_IS_ENABLED

  // print the words if any
  size_t chordWordsListSize = fChordWordsList.size ();

  if (chordWordsListSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordWordsList";
    if (chordWordsListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_msrWords>::const_iterator i;
      for (i = fChordWordsList.begin (); i != fChordWordsList.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }

  // print the ties if any
  size_t chordTiesListSize = fChordTiesList.size ();

  if (chordTiesListSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordTiesList";
    if (chordTiesListSize) {
      os << std::endl;
      ++gIndenter;

      for (S_msrTie tie : fChordTiesList) {
        tie->print (os);
        } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }

#ifdef MF_TRACE_IS_ENABLED
  // print the slur links if any
  size_t chordSlurLinksListSize = fChordSlurLinksList.size ();

  if (gTraceOahGroup->getTraceSlurs () || chordSlurLinksListSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordSlurLinksList";
    if (chordSlurLinksListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_msrChordSlurLink>::const_iterator i;
      for (i = fChordSlurLinksList.begin (); i != fChordSlurLinksList.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // print the ligatures if any
  size_t chordLigaturesListSize = fChordLigaturesList.size ();

  if (chordLigaturesListSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordLigaturesList";
    if (chordLigaturesListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_msrLigature>::const_iterator i;
      for (i = fChordLigaturesList.begin (); i != fChordLigaturesList.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }

/*
  // print the slashes if any
  size_t chordSlashesListSize = fChordSlashesList.size ();

  if (chordSlashesListSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordSlashesList";
    if (chordSlashesListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_msrSlash>::const_iterator i;
      for (i = fChordSlashesList.begin (); i != fChordSlashesList.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }

  // print the cresc/descrec if any
  size_t chordCrescDescrecsSize = fChordCrescDescrecs.size ();

  if (chordCrescDescrecsSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordCrescDescrecs";
    if (chordCrescDescrecsSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_msrCrescDescrec>::const_iterator i;
      for (i = fChordCrescDescrecs.begin (); i != fChordCrescDescrecs.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }

  // print the wedges if any
  size_t chordWedgesListSize = fChordWedgesList.size ();

  if (chordWedgesListSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordWedgesList";
    if (chordWedgesListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_msrWedge>::const_iterator i;
      for (i = fChordWedgesList.begin (); i != fChordWedgesList.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }

  // print the segnos if any
  size_t chordSegnosListSize = fChordSegnosList.size ();

  if (chordSegnosListSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordSegnosList";
    if (chordSegnosListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_msrSegno>::const_iterator i;
      for (i = fChordSegnosList.begin (); i != fChordSegnosList.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }

  // print the dal segnos if any
  size_t chordDalSegnosListSize = fChordDalSegnosList.size ();

  if (chordDalSegnosListSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordDalSegnosList";
    if (chordDalSegnosListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_msrDalSegno>::const_iterator i;
      for (i = fChordDalSegnosList.begin (); i != fChordDalSegnosList.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }

  // print the codas if any
  size_t chordCodasListSize = fChordCodasList.size ();

  if (chordCodasListSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordCodasList";
    if (chordCodasListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_msrCoda>::const_iterator i;
      for (i = fChordCodasList.begin (); i != fChordCodasList.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }

  // print the octave shift if any
  if (fChordOctaveShift) {
    os <<
      std::setw (fieldWidth) <<
      "fChordOctaveShift";
    if (fChordOctaveShift) {
      os << std::endl;
      ++gIndenter;
        os << fChordOctaveShift;
      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }
*/

  // print the harmonies associated to this chord if any
  size_t chordHarmoniesListSize = fChordHarmoniesList.size ();

  if (chordHarmoniesListSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordHarmoniesList";
    if (chordHarmoniesListSize) {
      os << std::endl;
      ++gIndenter;

      std::list <S_msrHarmony>::const_iterator
        iBegin = fChordHarmoniesList.begin (),
        iEnd   = fChordHarmoniesList.end (),
        i      = iBegin;
      for ( ; ; ) {
        S_msrHarmony
          harmony = (*i);

        os << harmony->asString ();

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
  }

  // print the figured bass if any
  if (fChordFiguredBassesList) {
    os <<
      std::setw (fieldWidth) <<
      "fChordFiguredBassesList" << ": " <<
      std::endl;

    ++gIndenter;

    if (fChordFiguredBassesList) {
      os <<
        fChordFiguredBassesList->asString () <<
        std::endl;
    }
    else {
      os <<
        "[NULL]";
    }

    --gIndenter;
  }
  os << std::endl;

  // print the chord notes if any
  size_t chordNotesVectorSize = fChordNotesVector.size ();

  if (chordNotesVectorSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordNotesVector";
    if (chordNotesVectorSize) {
      os << std::endl;
      ++gIndenter;

      std::vector <S_msrNote>::const_iterator
        iBegin = fChordNotesVector.begin (),
        iEnd   = fChordNotesVector.end (),
        i      = iBegin;

      for ( ; ; ) {
        S_msrNote note = (*i);

        note->print (os);
        if (++i == iEnd) break;
        os << std::endl;
      } // for

      --gIndenter;
    }
    else {
      os << ":" << "[EMPTY]" <<
      std::endl;
    }

    os << std::endl;
  }

/*
  // print the chord grace notes groups if any
  if (fChordGraceNotesGroupBefore) {
    os <<
      std::setw (fieldWidth) <<
      "fChordGraceNotesGroupBefore";

    if (fChordGraceNotesGroupBefore) {
      os << std::endl;
      ++gIndenter;
      os << fChordGraceNotesGroupBefore;
      --gIndenter;
    }
    else {
      os << ": " << "[NULL]" << std::endl; // JMI TEST
    }
//    os << std::endl;
  }

  if (fChordGraceNotesGroupAfter) {
    os <<
      std::setw (fieldWidth) <<
      "fChordGraceNotesGroupAfter";

    if (fChordGraceNotesGroupAfter) {
      os << std::endl;
      ++gIndenter;
      os << fChordGraceNotesGroupAfter;
      --gIndenter;
    }
    else {
      os << ": " << "[NULL]";
    }
    os << std::endl;
  }
*/

  // print the chord grace notes group link after if any
  os <<
    std::setw (fieldWidth) <<
    "fChordGraceNotesGroupLinkAfter";
  if (fChordGraceNotesGroupLinkAfter) {
    os << std::endl;
    gIndenter++;
    os <<
      fChordGraceNotesGroupLinkAfter;
    gIndenter--;
  }
  else {
    os << ": " << "[NONE]" << std::endl;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrChord::printFull (std::ostream& os) const
{
  msrWholeNotes
    chordMeasureFullLength =
      fMeasureElementUpLinkToMeasure
        ? fMeasureElementUpLinkToMeasure->
            getFullMeasureWholeNotesDuration ()
        : msrWholeNotes (0, 1); // JMI

  os <<
    "[Chord FULL" <<
    ", fChordKind: " << fChordKind <<
    ", " <<
    mfSingularOrPlural (
      fChordNotesVector.size (), "note", "notes") <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 44;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureElementMeasurePosition" << ": " <<
    fMeasureElementMeasurePosition.asString () <<
    std::setw (fieldWidth) <<
    "fMeasureElementSoundingWholeNotes" << ": " <<
    fMeasureElementSoundingWholeNotes <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fChordDisplayWholeNotes" << ": " << fChordDisplayWholeNotes <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMeasureElementUpLinkToMeasure()" << ": " <<
    fMeasureElementUpLinkToMeasure->getMeasureNumber () <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     "fMeasureElementVoicePosition" << ": " << fMeasureElementVoicePosition <<
//     std::endl <<
    std::setw (fieldWidth) <<
    "chordMeasureFullLength" << ": " << chordMeasureFullLength <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fMeasureElementUpLinkToMeasure" << ": ";
  if (fChordShortcutUpLinkToContainingTuplet) {
    os <<
      fMeasureElementUpLinkToMeasure->asShortString ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fChordShortcutUpLinkToContainingTuplet" << ": ";
  if (fChordShortcutUpLinkToContainingTuplet) {
    os <<
      fChordShortcutUpLinkToContainingTuplet->asShortString ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fPositionInTuplet" << ": " <<
    fPositionInTuplet <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fChordIsFirstChordInADoubleTremolo" << ": " <<
    fChordIsFirstChordInADoubleTremolo <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fChordIsSecondChordInADoubleTremolo" << ": " <<
    fChordIsSecondChordInADoubleTremolo <<
    std::endl << std::endl;

  // print the chord grace notes groups links if any
  os <<
    std::setw (fieldWidth) <<
    "fChordGraceNotesGroupLinkBefore";

  if (fChordGraceNotesGroupLinkBefore) {
    os << std::endl;
    gIndenter++;
    fChordGraceNotesGroupLinkBefore->printFull (os);
    gIndenter--;
  }
  else {
    os << ": " << "[NULL]" << std::endl; // JMI TEST
  }

  // print the articulations if any
  size_t chordArticulationsListSize = fChordArticulationsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordArticulationsList";
  if (chordArticulationsListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrArticulation articulation : fChordArticulationsList) {
      articulation->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

  // print the spanners if any
  size_t chordSpannersListSize = fChordSpannersList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordSpannersList";
  if (chordSpannersListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrSpanner spanner : fChordSpannersList) {
      spanner->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

  // print the technicals if any
  size_t chordTechnicalsListSize = fChordTechnicalsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordTechnicalsList";
  if (chordTechnicalsListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrTechnical technical : fChordTechnicalsList) {
      technical->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

  // print the ornaments if any
  size_t chordOrnamentsListSize = fChordOrnamentsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordOrnamentsList";
  if (chordOrnamentsListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrOrnament ornament : fChordOrnamentsList) {
      ornament->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

  // print the glissandos if any
  size_t chordGlissandosListSize = fChordGlissandosList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordGlissandosList";
  if (chordGlissandosListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrGlissando glissando : fChordGlissandosList) {
      glissando->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

  // print the slides if any
  size_t chordSlidesListSize = fChordSlidesList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordSlidesList";
  if (chordSlidesListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrSlide slide : fChordSlidesList) {
      slide->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

  // print the dynamics if any
  size_t chordDynamicsListSize = fChordDynamicsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordDynamicsList";
  if (chordDynamicsListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrDynamic dynamic : fChordDynamicsList) {
      dynamic->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

  // print the other dynamics if any
  size_t chordOtherDynamicsListSize = fChordOtherDynamicsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordOtherDynamicsList";
  if (chordOtherDynamicsListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrOtherDynamic otherDynamic : fChordOtherDynamicsList) {
      otherDynamic->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

  // print the stems if any
  size_t chordStemsListSize = fChordStemsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordStemsList";
  if (chordStemsListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrStem stem : fChordStemsList) {
      stem->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

//   // print the beams if any
  size_t chordBeamsListSize = fChordBeamsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordBeamsList";
  if (chordBeamsListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrBeam beam : fChordBeamsList) {
      beam->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

// #ifdef MF_TRACE_IS_ENABLED
  // print the beam links if any
  size_t chordBeamLinksListSize = fChordBeamLinksList.size ();

  os <<
    std::setw (fieldWidth) <<
    "===> fChordBeamLinksList ===>";
  if (chordBeamLinksListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrChordBeamLink chordBeamLink : fChordBeamLinksList) {
      chordBeamLink->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }
// #endif // MF_TRACE_IS_ENABLED

  // print the words if any
  size_t chordWordsListSize = fChordWordsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordBeamLinksList";
  if (chordWordsListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrWords words : fChordWordsList) {
      words->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

  // print the ties if any
  size_t chordTiesListSize = fChordTiesList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordTiesList";
  if (chordTiesListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrTie tie : fChordTiesList) {
      tie->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

  // print the ligatures if any
  size_t chordLigaturesListSize = fChordLigaturesList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordLigaturesList";
  if (chordLigaturesListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrLigature ligature : fChordLigaturesList) {
      ligature->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

  // print the slashes if any
  size_t chordSlashesListSize = fChordSlashesList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordSlashesList";
  if (chordSlashesListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrSlash slash : fChordSlashesList) {
      slash->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

  // print the cresc/decresc if any
  size_t chordCrescDecrescsListSize = fChordCrescDecrescsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordCrescDecrescsList";
  if (chordCrescDecrescsListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrCrescDecresc crescDecresc : fChordCrescDecrescsList) {
      crescDecresc->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

  // print the wedges if any
  size_t chordWedgesListSize = fChordWedgesList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordWedgesList";
  if (chordWedgesListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrWedge wedge : fChordWedgesList) {
      wedge->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

  // print the segnos if any
  size_t chordSegnosListSize = fChordSegnosList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordSegnosList";
  if (chordSegnosListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrSegno segno : fChordSegnosList) {
      segno->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

  // print the dal segnos if any
  size_t chordDalSegnosListSize = fChordDalSegnosList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordDalSegnosList";
  if (chordDalSegnosListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrDalSegno dalSegno : fChordDalSegnosList) {
      dalSegno->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

  // print the codas if any
  size_t chordCodasListSize = fChordCodasList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordCodasList";
  if (chordCodasListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrCoda coda : fChordCodasList) {
      coda->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }

  // print the octave shift if any
  os <<
    std::setw (fieldWidth) <<
    "fChordOctaveShift";
  if (fChordOctaveShift) {
    os << std::endl;
    ++gIndenter;
      fChordOctaveShift->printFull (os);
    --gIndenter;
  }
  else {
    os <<
      ": " << "[NULL]" <<
    std::endl;
  }

  // print the harmonies associated to this chord if any
  size_t chordHarmoniesListSize = fChordHarmoniesList.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordHarmoniesList";
  if (chordHarmoniesListSize) {
    os << std::endl;
    ++gIndenter;

    for (S_msrHarmony harmony : fChordHarmoniesList) {
      harmony->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os << ": " <<
      "[EMPTY]" <<
      std::endl;
  }

  // print the figured bass if any
  os <<
    std::setw (fieldWidth) <<
    "fChordFiguredBassesList" << ": " <<
    std::endl;

  ++gIndenter;

  if (fChordFiguredBassesList) {
    fChordFiguredBassesList->printFull (os);
  }
  else {
    os << "[EMPTY]";
  }

  --gIndenter;
  os << std::endl;

  // print the chord notes if any
  size_t chordNotesVectorSize = fChordNotesVector.size ();

  os <<
    std::setw (fieldWidth) <<
    "fChordNotesVector";
  if (chordNotesVectorSize) {
    os << std::endl;
    ++gIndenter;

    std::vector <S_msrNote>::const_iterator
      iBegin = fChordNotesVector.begin (),
      iEnd   = fChordNotesVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrNote note = (*i);

      note->printFull (os);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os << ":" << "[EMPTY]" <<
    std::endl;
  }

  os << std::endl;

#ifdef MF_TRACE_IS_ENABLED
  // print the slur links if any
  size_t chordSlurLinksListSize = fChordSlurLinksList.size ();

  os <<
    std::setw (fieldWidth) <<
    "===> fChordSlurLinksList ===>";
  if (chordSlurLinksListSize) {
    os << std::endl;
    ++gIndenter;

    std::list <S_msrChordSlurLink>::const_iterator i;
    for (i = fChordSlurLinksList.begin (); i != fChordSlurLinksList.end (); ++i) {
      (*i)->printFull (os);
    } // for

    --gIndenter;
  }
  else {
    os <<
      ": " << "[EMPTY]" <<
    std::endl;
  }
#endif // MF_TRACE_IS_ENABLED

//   // print the chord grace notes groups if any
//   if (fChordGraceNotesGroupBefore || gMsrOahGroup->getDisplayMsrFull ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fChordGraceNotesGroupBefore";
//
//     if (fChordGraceNotesGroupBefore) {
//       os << std::endl;
//       ++gIndenter;
//       os <<
//         fChordGraceNotesGroupBefore->asString () <<
//         std::endl;
//       --gIndenter;
//     }
//     else {
//       os << ": " << "[NONE]" << std::endl; // JMI TEST
//     }
// //    os << std::endl;
//   }

//   if (fChordGraceNotesGroupAfter || gMsrOahGroup->getDisplayMsrFull ()) {
//     os <<
//       std::setw (fieldWidth) <<
//       "fChordGraceNotesGroupAfter";
//
//     if (fChordGraceNotesGroupAfter) {
//       os << std::endl;
//       gIndenter++;
//       fChordGraceNotesGroupAfter->printFull (os);
//       gIndenter--;
//     }
//     else {
//       os << ": " << "[NONE]";
//     }
//     os << std::endl;
//   }

  os <<
    std::setw (fieldWidth) <<
    "fChordGraceNotesGroupLinkAfter";

  if (fChordGraceNotesGroupLinkAfter) {
    os << std::endl;
    gIndenter++;
    fChordGraceNotesGroupLinkAfter->printFull (os);
    gIndenter--;
  }
  else {
    os << ": " << "[NULL]";
  }
  os << std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrChord& elt)
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
S_msrChordBeamLink msrChordBeamLink::create (
  int               inputLineNumber,
  const S_msrBeam&  originalBeam,
  const S_msrChord& upLinkToChord)
{
  msrChordBeamLink* obj =
    new msrChordBeamLink (
      inputLineNumber,
      originalBeam,
      upLinkToChord);
  assert (obj != nullptr);
  return obj;
}

msrChordBeamLink::msrChordBeamLink (
  int               inputLineNumber,
  const S_msrBeam&  originalBeam,
  const S_msrChord& upLinkToChord)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalBeam != nullptr,
    "originalBeam is null");

  mfAssert (
    __FILE__, __LINE__,
    upLinkToChord != nullptr,
    "upLinkToChord is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fUpLinkToChord = upLinkToChord;

  fOriginalBeam = originalBeam;
}

msrChordBeamLink::~msrChordBeamLink ()
{}

S_msrChordBeamLink msrChordBeamLink::createBeamNewbornClone ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of grace notes group link '" <<
      asShortString () <<
      "'";

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
    "containingVoice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED
*/

  S_msrChordBeamLink
    newbornClone =
      msrChordBeamLink::create (
        fInputStartLineNumber,
        fOriginalBeam,
        fUpLinkToChord);

  return newbornClone;
}

void msrChordBeamLink::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrChordBeamLink::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrChordBeamLink>*
    p =
      dynamic_cast<visitor<S_msrChordBeamLink>*> (v)) {
        S_msrChordBeamLink elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrChordBeamLink::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrChordBeamLink::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrChordBeamLink::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrChordBeamLink>*
    p =
      dynamic_cast<visitor<S_msrChordBeamLink>*> (v)) {
        S_msrChordBeamLink elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrChordBeamLink::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrChordBeamLink::browseData (basevisitor* v)
{
  // browse the original grace notes group
  msrBrowser<msrBeam> browser (v);
  browser.browse (*fOriginalBeam);
}

std::string msrChordBeamLink::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[ChordBeamLink" <<
    ", fOriginalBeam: " <<
    fOriginalBeam->asShortString () <<
    ", fUpLinkToChord:" <<
    fUpLinkToChord->asShortString () <<
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

std::string msrChordBeamLink::asString () const
{
  std::stringstream ss;

  ss <<
    "[ChordBeamLink" <<
    ", fOriginalBeam: " <<
    fOriginalBeam->asString () <<
    ", fUpLinkToChord: " <<
    fUpLinkToChord->asString () <<
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

void msrChordBeamLink::printFull (std::ostream& os) const
{
  os <<
    "[ChordBeamLink" <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os <<
    std::setw (fieldWidth) <<
    "fOriginalBeam:" <<
    std::endl;

  ++gIndenter;
  fOriginalBeam->printFull(os);
  os <<
    fUpLinkToChord->asString () <<
    std::endl;
  --gIndenter;

  --gIndenter;

  os << ']' << std::endl;
}

void msrChordBeamLink::print (std::ostream& os) const
{
  os <<
    "[ChordBeamLink" <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os <<
    std::setw (fieldWidth) <<
    "fOriginalBeam:" <<
    std::endl;

  ++gIndenter;
  fOriginalBeam->print (os);
  os <<
    fUpLinkToChord->asShortString () <<
    std::endl;
  --gIndenter;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrChordBeamLink& elt)
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
S_msrChordSlurLink msrChordSlurLink::create (
  int               inputLineNumber,
  const S_msrSlur&  originalSlur,
  const S_msrChord& upLinkToChord)
{
  msrChordSlurLink* obj =
    new msrChordSlurLink (
      inputLineNumber,
      originalSlur,
      upLinkToChord);
  assert (obj != nullptr);
  return obj;
}

msrChordSlurLink::msrChordSlurLink (
  int               inputLineNumber,
  const S_msrSlur&  originalSlur,
  const S_msrChord& upLinkToChord)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalSlur != nullptr,
    "originalSlur is null");

  mfAssert (
    __FILE__, __LINE__,
    upLinkToChord != nullptr,
    "upLinkToChord is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fUpLinkToChord = upLinkToChord;

  fOriginalSlur = originalSlur;
}

msrChordSlurLink::~msrChordSlurLink ()
{}

S_msrChordSlurLink msrChordSlurLink::createSlurNewbornClone ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of grace notes group link '" <<
      asShortString () <<
      "'";

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
    "containingVoice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED
*/

  S_msrChordSlurLink
    newbornClone =
      msrChordSlurLink::create (
        fInputStartLineNumber,
        fOriginalSlur,
        fUpLinkToChord);

  return newbornClone;
}

void msrChordSlurLink::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrChordSlurLink::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrChordSlurLink>*
    p =
      dynamic_cast<visitor<S_msrChordSlurLink>*> (v)) {
        S_msrChordSlurLink elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrChordSlurLink::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrChordSlurLink::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrChordSlurLink::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrChordSlurLink>*
    p =
      dynamic_cast<visitor<S_msrChordSlurLink>*> (v)) {
        S_msrChordSlurLink elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrChordSlurLink::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrChordSlurLink::browseData (basevisitor* v)
{
  // browse the original grace notes group
  msrBrowser<msrSlur> browser (v);
  browser.browse (*fOriginalSlur);
}

std::string msrChordSlurLink::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[ChordSlurLink" <<
    ", fOriginalSlur: " <<
    fOriginalSlur->asShortString () <<
    ", fUpLinkToChord: " <<
    fUpLinkToChord->asShortString () <<
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

std::string msrChordSlurLink::asString () const
{
  std::stringstream ss;

  ss <<
    "[ChordSlurLink" <<
    ", fOriginalSlur: " <<
    fOriginalSlur->asString () <<
    ", fUpLinkToChord: " <<
    fUpLinkToChord <<
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

void msrChordSlurLink::print (std::ostream& os) const
{
  os <<
    "[ChordSlurLink" <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os <<
    std::setw (fieldWidth) <<
    "fOriginalSlur:" <<
    std::endl;

  ++gIndenter;
  os <<
    fOriginalSlur <<
    fUpLinkToChord->asString ();
  --gIndenter;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrChordSlurLink& elt)
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
S_msrChordGraceNotesGroupLink msrChordGraceNotesGroupLink::create (
  int                         inputLineNumber,
  const S_msrGraceNotesGroup& originalGraceNotesGroup,
  const S_msrChord&           upLinkToChord)
{
  msrChordGraceNotesGroupLink* obj =
    new msrChordGraceNotesGroupLink (
      inputLineNumber,
      originalGraceNotesGroup,
      upLinkToChord);
  assert (obj != nullptr);
  return obj;
}

msrChordGraceNotesGroupLink::msrChordGraceNotesGroupLink (
  int                  inputLineNumber,
  const S_msrGraceNotesGroup& originalGraceNotesGroup,
  const S_msrChord&           upLinkToChord)
    : msrElement (inputLineNumber)
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalGraceNotesGroup != nullptr,
    "originalGraceNotesGroup is null");

  mfAssert (
    __FILE__, __LINE__,
    upLinkToChord != nullptr,
    "upLinkToChord is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fUpLinkToChord = upLinkToChord;

  fOriginalGraceNotesGroup = originalGraceNotesGroup;
}

msrChordGraceNotesGroupLink::~msrChordGraceNotesGroupLink ()
{}

S_msrChordGraceNotesGroupLink msrChordGraceNotesGroupLink::createChordGraceNotesGroupLinkNewbornClone ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of grace notes group link '" <<
      asShortString () <<
      "'";

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
    "containingVoice is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED
*/

  const S_msrChordGraceNotesGroupLink&
    newbornClone =
      msrChordGraceNotesGroupLink::create (
        fInputStartLineNumber,
        fOriginalGraceNotesGroup,
        fUpLinkToChord);

  return newbornClone;
}

void msrChordGraceNotesGroupLink::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrChordGraceNotesGroupLink::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrChordGraceNotesGroupLink>*
    p =
      dynamic_cast<visitor<S_msrChordGraceNotesGroupLink>*> (v)) {
        S_msrChordGraceNotesGroupLink elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrChordGraceNotesGroupLink::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrChordGraceNotesGroupLink::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrChordGraceNotesGroupLink::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrChordGraceNotesGroupLink>*
    p =
      dynamic_cast<visitor<S_msrChordGraceNotesGroupLink>*> (v)) {
        S_msrChordGraceNotesGroupLink elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrChordGraceNotesGroupLink::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrChordGraceNotesGroupLink::browseData (basevisitor* v)
{
  std::list <S_msrMeasureElement>::const_iterator i;

  // browse the original grace notes group
  msrBrowser<msrGraceNotesGroup> browser (v);
  browser.browse (*fOriginalGraceNotesGroup);
}

std::string msrChordGraceNotesGroupLink::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[chordGraceNotesGroupLink" <<
    ", fOriginalGraceNotesGroup: " <<
    fOriginalGraceNotesGroup->asShortString () <<
    ", fUpLinkToChord \"" <<
    fUpLinkToChord->asShortString () <<
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

std::string msrChordGraceNotesGroupLink::asString () const
{
  std::stringstream ss;

  ss <<
    "[chordGraceNotesGroupLink" <<
    ", fOriginalGraceNotesGroup " <<
    fOriginalGraceNotesGroup->asString () <<
    ", fUpLinkToChord \"" <<
    fUpLinkToChord <<
    ", line " << fInputStartLineNumber <<
    ']';

  return ss.str ();
}

void msrChordGraceNotesGroupLink::print (std::ostream& os) const
{
  os <<
    "[ChordGraceNotesGroupLink FULL" <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os <<
    std::setw (fieldWidth) <<
    "fOriginalGraceNotesGroup" <<
    std::endl;

  ++gIndenter;
  os <<
    fOriginalGraceNotesGroup <<
    std::endl;
  --gIndenter;

  --gIndenter;

  os << ']' << std::endl;
}

void msrChordGraceNotesGroupLink::printFull (std::ostream& os) const
{
  os <<
    "[ChordGraceNotesGroupLink" <<
    ", line " << fInputStartLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os <<
    std::setw (fieldWidth) <<
    "fOriginalGraceNotesGroup" <<
    std::endl;

  ++gIndenter;
  fOriginalGraceNotesGroup->printFull (os);
  --gIndenter;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrChordGraceNotesGroupLink& elt)
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
