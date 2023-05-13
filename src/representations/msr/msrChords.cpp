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

#include "msrArticulations.h"
#include "msrGlissandos.h"
#include "msrMeasureConstants.h"
#include "msrMusicXMLBasicTypes.h"
#include "msrSlides.h"
#include "msrTechnicals.h"

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
S_msrChord msrChord::create (
  int                  inputLineNumber,
  const S_msrMeasure&  upLinkToMeasure,
  const msrWholeNotes& chordSoundingWholeNotes,
  const msrWholeNotes& chordDisplayWholeNotes,
  msrNotesDurationKind chordGraphicNotesDurationKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Creating a chord" <<
      ", chordSoundingWholeNotes: " << chordSoundingWholeNotes.asString () <<
      ", chordDisplayWholeNotes: " << chordDisplayWholeNotes.asString () <<
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
      chordSoundingWholeNotes, chordDisplayWholeNotes,
      chordGraphicNotesDurationKind);
  assert (obj != nullptr);
  return obj;
}

S_msrChord msrChord::create (
  int                  inputLineNumber,
  const msrWholeNotes& chordSoundingWholeNotes,
  const msrWholeNotes& chordDisplayWholeNotes,
  msrNotesDurationKind chordGraphicNotesDurationKind)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Creating a chord" <<
      ", chordSoundingWholeNotes: " << chordSoundingWholeNotes.asString () <<
      ", chordDisplayWholeNotes: " << chordDisplayWholeNotes.asString () <<
      ", chordGraphicNotesDuration: " <<
      msrNotesDurationKindAsString (chordGraphicNotesDurationKind);

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return
    msrChord::create (
      inputLineNumber,
      gNullMeasure, // set later in setChordUpLinkToMeasure()
      chordSoundingWholeNotes, chordDisplayWholeNotes,
      chordGraphicNotesDurationKind);
}

msrChord::msrChord (
  int                  inputLineNumber,
  const S_msrMeasure&  upLinkToMeasure,
  const msrWholeNotes& chordSoundingWholeNotes,
  const msrWholeNotes& chordDisplayWholeNotes,
  msrNotesDurationKind chordGraphicNotesDurationKind)
    : msrTupletElement (
        inputLineNumber)
{
  fChordKind = msrChordInKind::kChordIn_UNKNOWN_;

  fChordUpLinkToMeasure = upLinkToMeasure;

  doSetSoundingWholeNotes (
    chordSoundingWholeNotes,
    "msrChord::msrChord()");

  fChordDisplayWholeNotes  = chordDisplayWholeNotes;

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
  if (gTraceOahGroup->getTraceChords ()) {
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
        fInputLineNumber,
        gNullMeasure, // set later in setChordUpLinkToMeasure()
        fSoundingWholeNotes,
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
//       fMeasurePosition.asString () <<
//       ") in measure " <<
//       measure->asShortString () <<
//       " (measureElementMeasureNumber: " <<
//       fChordUpLinkToMeasure->getMeasureNumber () <<
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
//   fMeasurePosition = measurePosition;
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
//       result = fChordUpLinkToMeasure;
//       break;
//
//     case msrChordInKind::kChordInTuplet:
//       if (fChordShortcutUpLinkToTuplet) {
//         result =
//           fChordShortcutUpLinkToTuplet->
//             fChordUpLinkToMeasure;
//       }
//       break;
//
//     case msrChordInKind::kChordInGraceNotesGroup:
//       if (fChordShortcutUpLinkToGraceNotesGroup) {
//         result =
//           fChordShortcutUpLinkToGraceNotesGroup->
//             getGraceNotesGroupUpLinkToNote ()->
//               fChordUpLinkToMeasure;
//       }
//       break;
//   } // switch
//
//   return result;
// }

void  msrChord::setMeasureElementUpLinkToMeasure (
  const S_msrMeasure& measure)
{
  setChordUpLinkToMeasure (measure);
}

void msrChord::getMeasureElementUpLinkToMeasure (
  S_msrMeasure& upLinkToMeasure) const
{
  upLinkToMeasure = getChordUpLinkToMeasure ();
}

void msrChord::setChordUpLinkToMeasure (
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
  if (gTraceOahGroup->getTraceChords ()) {
    ++gIndenter;

    std::stringstream ss;

    ss <<
      "Setting the uplink to measure of chord " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordUpLinkToMeasure = measure;
}

S_msrMeasure msrChord::getChordUpLinkToMeasure () const
{
  return fChordUpLinkToMeasure;
}

// uplink to tuplet
S_msrTuplet msrChord::fetchChordUpLinkToTuplet () const
{
  S_msrTuplet result;

  switch (fChordKind) {
    case msrChordInKind::kChordIn_UNKNOWN_:
      break;

    case msrChordInKind::kChordInMeasure:
      break;

    case msrChordInKind::kChordInTuplet:
      result = fChordShortcutUpLinkToTuplet;
      break;

    case msrChordInKind::kChordInGraceNotesGroup:
      if (fChordShortcutUpLinkToGraceNotesGroup) {
        result =
          fChordShortcutUpLinkToGraceNotesGroup->
            getGraceNotesGroupUpLinkToNote ()->
              getNoteShortcutUpLinkToTuplet ();
      }
      break;
  } // switch

  return result;
}

// uplink to grace notes group
S_msrGraceNotesGroup msrChord::fetchChordUpLinkToGraceNotesGroup () const
{
  S_msrGraceNotesGroup result;

  switch (fChordKind) {
    case msrChordInKind::kChordIn_UNKNOWN_:
      break;
    case msrChordInKind::kChordInMeasure:
      break;
    case msrChordInKind::kChordInTuplet:
      break;
    case msrChordInKind::kChordInGraceNotesGroup:
      result = fChordShortcutUpLinkToGraceNotesGroup; // JMI
      break;
  } // switch

  return result;
}

// score upLink
S_msrScore msrChord::fetchChordUpLinkToScore () const
{
  S_msrScore result;

  S_msrMeasure
    measure =
      fChordUpLinkToMeasure;

  if (measure) {
    result =
      measure ->
        fetchMeasureUpLinkToScore ();
  }

  return result;
}

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

void msrChord::setMeasurePosition (
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

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // handle the chord itself
  msrMeasureElement::setMeasurePosition (
    measure,
    measurePosition,
    context);

  // handle its members
  setChordMembersMeasurePosition (
    measure,
    measurePosition);
}

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
      setNoteUpLinkToMeasure (
        measure);

    // set note's measure position // JMI v0.9.66
    note->
      setMeasurePosition (
        measure,
        measurePosition, // they all share the same one
        "setChordMembersMeasurePosition()");

//    // set note's voice position JMI v0.9.66
//       note->
//         setVoicePosition (
//           voicePosition,
//           context); // they all share the same one
  } // for

//   // are there dal segnos attached to this chord?
//   if (fChordDalSegnos.size ()) {
//     for (S_msrDalSegno dalSegno : fChordDalSegnos) {
//       // set the dal segno measure position
// //       dalSegno->
// //         setDalSegnoMeasurePosition (
// //           measure,
// //           measurePosition,
// //           "msrChord::setChordMembersMeasurePosition()"); // JMI v0.9.66
//     } // for
}

void msrChord::addFirstNoteToChord (
  const S_msrNote&  note,
  const S_msrVoice& voice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Adding first note '" <<
      note->asString () <<
      "' to chord '" <<
      asString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // append note to chord notes vector
  fChordNotesVector.push_back (note);

  // register note's uplink to chord
  note->
    setNoteShortcutUpLinkToChord (this);

/* JMI too early v0.9.66
  // register note's uplink to measure
  note->
    setNoteUpLinkToMeasure (
      fChordUpLinkToMeasure);
*/

  // mark note as belonging to a chord
  note->setNoteBelongsToAChord ();

  // mark note as being the first one in the chord
  note->setNoteIsAChordsFirstMemberNote ();

  // is this note the shortest one in this voice?
  voice->
    registerShortestNoteInVoiceIfRelevant (
      note);

  // has this note an msrStem attached to it?
  S_msrStem
    noteStem =
      note->getNoteStem ();

  if (noteStem) {
    appendStemToChord (noteStem);
  }

  // register note as the last appended one into this voice
  /* JMI
  voice->
    registerNoteAsVoiceLastAppendedNote (note);
    */
}

void msrChord::addAnotherNoteToChord (
  const S_msrNote&  note,
  const S_msrVoice& voice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Adding another note '" <<
      note->asShortString () <<
      "' to chord '" <<
      asString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

//   gLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
//   print (gLog); // JMI v0.9.66
//   gLog << "++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;

  // append note to chord notes vector
  fChordNotesVector.push_back (note);

  // register note's uplink to chord
  note->
    setNoteShortcutUpLinkToChord (this);

  // mark note as belonging to a chord
  note->setNoteBelongsToAChord ();

  // append the note to the measure's notes flat list
  if (false) // JMI v0.9.66
  fChordUpLinkToMeasure->
    appendNoteToMeasureNotesFlatList (note);

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
      fInputLineNumber,
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

  fChordDalSegnos.push_back (dalSegno);
}

void msrChord::appendArticulationToChord (
  const S_msrArticulation& art)
{
  msrArticulationKind
    articulationKind =
      art->
        getArticulationKind ();

  // don't append the same articulation several times
  for (const S_msrArticulation& articulation : fChordArticulations) {
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

  fChordArticulations.push_back (art);
}

void msrChord::appendSpannerToChord (
  const S_msrSpanner& span)
{
  msrSpannerKind
    spannerKind =
      span->
        getSpannerKind ();

  // don't append the same spanner several times
  for (S_msrSpanner spanner : fChordSpanners) {
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

  fChordSpanners.push_back (span);
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
    std::list<S_msrTechnical>::const_iterator i =
      fChordTechnicals.begin ();
    i!=fChordTechnicals.end ();
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

  fChordTechnicals.push_back (tech);
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
    std::list<S_msrTechnicalWithInteger>::const_iterator i =
      fChordTechnicalWithIntegers.begin ();
    i!=fChordTechnicalWithIntegers.end ();
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

  fChordTechnicalWithIntegers.push_back (tech);
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
    std::list<S_msrTechnicalWithFloat>::const_iterator i =
      fChordTechnicalWithFloats.begin ();
    i!=fChordTechnicalWithFloats.end ();
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

  fChordTechnicalWithFloats.push_back (tech);
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
    std::list<S_msrTechnicalWithString>::const_iterator i =
      fChordTechnicalWithStrings.begin ();
    i!=fChordTechnicalWithStrings.end ();
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

  fChordTechnicalWithStrings.push_back (tech);
}

void msrChord::appendOrnamentToChord (
  const S_msrOrnament& orn)
{
  msrOrnamentKind
    ornamentKind =
      orn->
        getOrnamentKind ();

  // don't append the same ornament several times
  for (S_msrOrnament ornament : fChordOrnaments) {
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

  fChordOrnaments.push_back (orn);
}

void msrChord::appendGlissandoToChord (
  const S_msrGlissando& gliss)
{
  msrGlissandoTypeKind
    glissandoTypeKind =
      gliss->
        getGlissandoTypeKind ();

  // don't append the same slissando several times
  for (S_msrGlissando glissando : fChordGlissandos) {
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

  fChordGlissandos.push_back (gliss);
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
    std::list<S_msrSlide>::const_iterator i = fChordSlides.begin ();
    i!=fChordSlides.end ();
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

  fChordSlides.push_back (slide);
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

  fChordSlurLinks.push_back (chordSlurLink);
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
    std::list<S_msrStem>::const_iterator i = fChordStems.begin ();
    i != fChordStems.end ();
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
        fInputLineNumber,
  //      __FILE__, __LINE__,
        ss.str ());
    }
  } // for
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  fChordStems.push_back (stem);
}

/* JMI
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

  fChordBeams.push_back (beam);
}
*/

void msrChord::appendChordBeamLinkToChord (
  const S_msrChordBeamLink& chordBeamLink)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceBeams ()) {
    std::stringstream ss;

    ss <<
      "Adding beam link '" << chordBeamLink->asString() <<
      "' to chord '" << asString () << "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fChordBeamLinks.push_back (chordBeamLink);
}

void msrChord::finalizeChord (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceChords ()) {
    std::stringstream ss;

    ss <<
      "Finalizing chord '" <<
      asString () <<
      "', line " << inputLineNumber <<
      std::endl <<
      "fMeasurePosition: " <<
      std::endl <<
      fMeasurePosition.asString ();

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // we can now set the measure positions for all the chord members JMI v0.9.66
  if (false) // JMI v0.9.67 MERDUM
  setChordMembersMeasurePosition (
    fChordUpLinkToMeasure,
    fMeasurePosition);
}

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
          "% ==> visiting grace notes groups 'before' is inhibited" <<
          std::endl;

        gWaeHandler->waeTrace (
          __FILE__, __LINE__,
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      return;
    }
  }

  if (fChordGraceNotesGroupLinkBefore) {
    // browse the grace notes group before
    msrBrowser<msrChordGraceNotesGroupLink> browser (v);
    browser.browse (*fChordGraceNotesGroupLinkBefore);
  }

  for (
    std::vector<S_msrNote>::const_iterator i = fChordNotesVector.begin ();
    i != fChordNotesVector.end ();
    ++i
  ) {
    // browse chord note
    msrBrowser<msrNote> browser (v);
    browser.browse (*(*i));
  } // for


  for (
    std::list<S_msrStem>::const_iterator i = fChordStems.begin ();
    i != fChordStems.end ();
    ++i
  ) {
    // browse chord stems
    msrBrowser<msrStem> browser (v);
    browser.browse (*(*i));
  } // for

/* JMI
  for (
    std::list<S_msrBeam>::const_iterator i = fChordBeams.begin ();
    i != fChordBeams.end ();
    ++i
  ) {
    // browse chord beams
    msrBrowser<msrBeam> browser (v);
    browser.browse (*(*i));
  } // for
  */
  for (
    std::list<S_msrChordBeamLink>::const_iterator i = fChordBeamLinks.begin ();
    i != fChordBeamLinks.end ();
    ++i
  ) {
    // browse the beam link
    msrBrowser<msrChordBeamLink> browser (v);
    browser.browse (*(*i));
  } // for


  for (
    std::list<S_msrArticulation>::const_iterator i = fChordArticulations.begin ();
    i != fChordArticulations.end ();
    ++i
  ) {
    // browse the articulation
    msrBrowser<msrArticulation> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list<S_msrSpanner>::const_iterator i = fChordSpanners.begin ();
    i != fChordSpanners.end ();
    ++i
  ) {
    // browse the spanner
    msrBrowser<msrSpanner> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list<S_msrTechnical>::const_iterator i = fChordTechnicals.begin ();
    i != fChordTechnicals.end ();
    ++i
  ) {
    // browse the technical
    msrBrowser<msrTechnical> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list<S_msrOrnament>::const_iterator i = fChordOrnaments.begin ();
    i != fChordOrnaments.end ();
    ++i
  ) {
    // browse the ornament
    msrBrowser<msrOrnament> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list<S_msrGlissando>::const_iterator i = fChordGlissandos.begin ();
    i != fChordGlissandos.end ();
    ++i
  ) {
    // browse the glissando
    msrBrowser<msrGlissando> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list<S_msrSlide>::const_iterator i = fChordSlides.begin ();
    i != fChordSlides.end ();
    ++i
  ) {
    // browse the slide
    msrBrowser<msrSlide> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list<S_msrDynamic>::const_iterator i = fChordDynamics.begin ();
    i != fChordDynamics.end ();
    ++i
  ) {
    // browse the dynamic
    msrBrowser<msrDynamic> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list<S_msrOtherDynamic>::const_iterator i = fChordOtherDynamics.begin ();
    i != fChordOtherDynamics.end ();
    ++i
  ) {
    // browse the other dynamic
    msrBrowser<msrOtherDynamic> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list<S_msrWords>::const_iterator i = fChordWords.begin ();
    i != fChordWords.end ();
    ++i
  ) {
    // browse the words
    msrBrowser<msrWords> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list<S_msrTie>::const_iterator i = fChordTies.begin ();
    i != fChordTies.end ();
    ++i
  ) {
    // browse the tie
    msrBrowser<msrTie> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list<S_msrLigature>::const_iterator i = fChordLigatures.begin ();
    i != fChordLigatures.end ();
    ++i
  ) {
    // browse the ligature
    msrBrowser<msrLigature> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list<S_msrSlash>::const_iterator i = fChordSlashes.begin ();
    i != fChordSlashes.end ();
    ++i
  ) {
    // browse the slash
    msrBrowser<msrSlash> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list<S_msrCrescDecresc>::const_iterator i = fChordCrescDecrescs.begin ();
    i != fChordCrescDecrescs.end ();
    ++i
  ) {
    // browse the cresc/decresc
    msrBrowser<msrCrescDecresc> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list<S_msrWedge>::const_iterator i = fChordWedges.begin ();
    i != fChordWedges.end ();
    ++i
  ) {
    // browse the wedge
    msrBrowser<msrWedge> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list<S_msrSegno>::const_iterator i = fChordSegnos.begin ();
    i != fChordSegnos.end ();
    ++i
  ) {
    // browse the segno
    msrBrowser<msrSegno> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list<S_msrDalSegno>::const_iterator i = fChordDalSegnos.begin ();
    i != fChordDalSegnos.end ();
    ++i
  ) {
    // browse the dal segno
    msrBrowser<msrDalSegno> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    std::list<S_msrCoda>::const_iterator i = fChordCodas.begin ();
    i != fChordCodas.end ();
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
    std::list<S_msrHarmony>::const_iterator i;
    for (i=fChordHarmoniesList.begin (); i!=fChordHarmoniesList.end (); ++i) {
      // browse the harmony
      msrBrowser<msrHarmony> browser (v);
      browser.browse (*(*i));
    } // for
  }

  if (fChordFiguredBass) {
    // browse the figured bass
    msrBrowser<msrFiguredBass> browser (v);
    browser.browse (*fChordFiguredBass);
  }

  for (
    std::list<S_msrChordSlurLink>::const_iterator i = fChordSlurLinks.begin ();
    i != fChordSlurLinks.end ();
    ++i
  ) {
    // browse the slur link
    msrBrowser<msrChordSlurLink> browser (v);
    browser.browse (*(*i));
  } // for

/*
  if (fChordGraceNotesGroupBefore) {
    // browse the grace notes group before
    msrBrowser<msrGraceNotesGroup> browser (v);
    browser.browse (*fChordGraceNotesGroupBefore);
  }
  if (fChordGraceNotesGroupAfter) {
    // browse the grace notes group after
    msrBrowser<msrGraceNotesGroup> browser (v);
    browser.browse (*fChordGraceNotesGroupAfter);
  }
*/

  if (fChordGraceNotesGroupLinkAfter) {
    // browse the grace notes group after
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
      "', line " << fInputLineNumber <<
      std::endl;

  for (
    std::vector<S_msrNote>::const_iterator i = fChordNotesVector.begin ();
    i != fChordNotesVector.end ();
    ++i) {
    (*i)->
      applyTupletMemberSoundingFactorToNote (
        actualNotes, normalNotes);
  } // for
}
#endif // MF_TRACE_IS_ENABLED
*/

std::string msrChord::asStringwithRawDivisions () const
{
  std::stringstream ss;

  ss << "<";

  if (fChordNotesVector.size ()) {
    std::vector<S_msrNote>::const_iterator
      iBegin = fChordNotesVector.begin (),
      iEnd   = fChordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote
        note = (*i);

      ss <<
      /* JMI
        note->notePitchAsString () <<
        note->noteSoundingWholeNotesAsMsrString () <<
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
    "[Chord" <<
    ", chordKind: " << fChordKind <<
    ", line " << fInputLineNumber <<
    " <";

  if (fChordNotesVector.size ()) {
    std::vector<S_msrNote>::const_iterator
      iBegin = fChordNotesVector.begin (),
      iEnd   = fChordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote note = (*i);

      ss <<
        note->notePitchAsString () <<
        ", whole notes: " <<
        " sounding " <<
        note->getSoundingWholeNotes () <<
        ", displayed " <<
        note->getNoteDisplayWholeNotes () <<
        ", octave: " <<
        msrOctaveKindAsString (note->getNoteOctaveKind ()) <<
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
    "[Chord" <<
    ", " << fChordKind <<
    ", line " << fInputLineNumber <<
    ", <";

  if (fChordNotesVector.size ()) {
    std::vector<S_msrNote>::const_iterator
      iBegin = fChordNotesVector.begin (),
      iEnd   = fChordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote
        note = (*i);

      ss <<
        note->notePitchAsString () <<
        " snd: " <<
        note->getSoundingWholeNotes () <<
        ", disp: " <<
        note->getNoteDisplayWholeNotes () <<
        ", " <<
        msrOctaveKindAsString (note->getNoteOctaveKind ());

      if (++i == iEnd) break;
      ss << ", ";
    } // for
  }

  ss << ">";

  return ss.str ();
}

void msrChord::printFull (std::ostream& os) const
{
  msrWholeNotes
    chordMeasureFullLength =
      fChordUpLinkToMeasure
        ? fChordUpLinkToMeasure->
            getFullMeasureWholeNotesDuration ()
        : msrWholeNotes (0, 1); // JMI

  os <<
    "[Chord" <<
    ", fChordKind: " << fChordKind <<
    ", " <<
    mfSingularOrPlural (
      fChordNotesVector.size (), "note", "notes") <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 44;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasurePosition" << ": " << fMeasurePosition.asString () <<
    std::setw (fieldWidth) <<
    "fSoundingWholeNotes" << ": " << fSoundingWholeNotes.asString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fChordDisplayWholeNotes" << ": " << fChordDisplayWholeNotes.asString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "getMeasureNumber()" << ": " <<
    fChordUpLinkToMeasure->getMeasureNumber () <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     "fVoicePosition" << ": " << fVoicePosition <<
//     std::endl <<
    std::setw (fieldWidth) <<
    "chordMeasureFullLength" << ": " << chordMeasureFullLength <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fChordUpLinkToMeasure" << ": ";
  if (fChordShortcutUpLinkToTuplet) {
    os <<
      fChordUpLinkToMeasure->asShortString ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fChordShortcutUpLinkToTuplet" << ": ";
  if (fChordShortcutUpLinkToTuplet) {
    os <<
      fChordShortcutUpLinkToTuplet->asShortString ();
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
  if (fChordGraceNotesGroupLinkBefore || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordGraceNotesGroupLinkBefore";

    if (fChordGraceNotesGroupLinkBefore) {
      os << std::endl;
      os << gTab << fChordGraceNotesGroupLinkBefore;
    }
    else {
      os << ": " << "[NULL]" << std::endl; // JMI TEST
    }
  }

  // print the articulations if any
  size_t chordArticulationsSize = fChordArticulations.size ();

  if (chordArticulationsSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordArticulations";
    if (chordArticulationsSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrArticulation>::const_iterator i;
      for (i=fChordArticulations.begin (); i!=fChordArticulations.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the spanners if any
  size_t chordSpannersSize = fChordSpanners.size ();

  if (chordSpannersSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordSpanners";
    if (chordSpannersSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrSpanner>::const_iterator i;
      for (i=fChordSpanners.begin (); i!=fChordSpanners.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the technicals if any
  size_t chordTechnicalsSize = fChordTechnicals.size ();

  if (chordTechnicalsSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordTechnicals";
    if (chordTechnicalsSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrTechnical>::const_iterator i;
      for (i=fChordTechnicals.begin (); i!=fChordTechnicals.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the ornaments if any
  size_t chordOrnamentsSize = fChordOrnaments.size ();

  if (chordOrnamentsSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordOrnaments";
    if (chordOrnamentsSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrOrnament>::const_iterator i;
      for (i=fChordOrnaments.begin (); i!=fChordOrnaments.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the glissandos if any
  size_t chordGlissandosSize = fChordGlissandos.size ();

  if (chordGlissandosSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordGlissandos";
    if (chordGlissandosSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrGlissando>::const_iterator i;
      for (i=fChordGlissandos.begin (); i!=fChordGlissandos.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the slides if any
  size_t chordSlidesSize = fChordSlides.size ();

  if (chordSlidesSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordSlides";
    if (chordSlidesSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrSlide>::const_iterator i;
      for (i=fChordSlides.begin (); i!=fChordSlides.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the dynamics if any
  size_t chordDynamicsSize = fChordDynamics.size ();

  if (chordDynamicsSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordDynamics";
    if (chordDynamicsSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrDynamic>::const_iterator i;
      for (i=fChordDynamics.begin (); i!=fChordDynamics.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the other dynamics if any
  size_t chordOtherDynamicsSize = fChordOtherDynamics.size ();

  if (chordOtherDynamicsSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordOtherDynamics";
    if (chordOtherDynamicsSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrOtherDynamic>::const_iterator i;
      for (i=fChordOtherDynamics.begin (); i!=fChordOtherDynamics.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the stems if any
  size_t chordStemsSize = fChordStems.size ();

  if (chordStemsSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordStems";
    if (chordStemsSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrStem>::const_iterator i;
      for (i=fChordStems.begin (); i!=fChordStems.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

/* JMI
  // print the beams if any
  size_t chordBeamsSize = fChordBeams.size ();

  if (chordBeamsSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordBeams";
    if (chordBeamsSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrBeam>::const_iterator i;
      for (i=fChordBeams.begin (); i!=fChordBeams.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }
*/
#ifdef MF_TRACE_IS_ENABLED
  // print the beam links if any
  size_t chordBeamLinksSize = fChordBeamLinks.size ();

  if (
    gTraceOahGroup->getTraceBeams ()
      ||
    chordBeamLinksSize
      ||
    gMsrOahGroup->getDisplayMsrFull ()
  ) {
    os <<
      std::setw (fieldWidth) <<
      "===> fChordBeamLinks ===>";
    if (chordBeamLinksSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrChordBeamLink>::const_iterator i;
      for (i=fChordBeamLinks.begin (); i!=fChordBeamLinks.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // print the words if any
  size_t chordWordsSize = fChordWords.size ();

  if (chordWordsSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordBeamLinks";
    if (chordWordsSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrWords>::const_iterator i;
      for (i=fChordWords.begin (); i!=fChordWords.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the ties if any
  size_t chordTiesSize = fChordTies.size ();

  if (chordTiesSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordTies";
    if (chordTiesSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrTie>::const_iterator i;
      for (i=fChordTies.begin (); i!=fChordTies.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the ligatures if any
  size_t chordLigaturesSize = fChordLigatures.size ();

  if (chordLigaturesSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordLigatures";
    if (chordLigaturesSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrLigature>::const_iterator i;
      for (i=fChordLigatures.begin (); i!=fChordLigatures.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the slashes if any
  size_t chordSlashesSize = fChordSlashes.size ();

  if (chordSlashesSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordSlashes";
    if (chordSlashesSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrSlash>::const_iterator i;
      for (i=fChordSlashes.begin (); i!=fChordSlashes.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the cresc/decresc if any
  size_t chordCrescDecrescsSize = fChordCrescDecrescs.size ();

  if (chordCrescDecrescsSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordCrescDecrescs";
    if (chordCrescDecrescsSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrCrescDecresc>::const_iterator i;
      for (i=fChordCrescDecrescs.begin (); i!=fChordCrescDecrescs.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the wedges if any
  size_t chordWedgesSize = fChordWedges.size ();

  if (chordWedgesSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordWedges";
    if (chordWedgesSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrWedge>::const_iterator i;
      for (i=fChordWedges.begin (); i!=fChordWedges.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the segnos if any
  size_t chordSegnosSize = fChordSegnos.size ();

  if (chordSegnosSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordSegnos";
    if (chordSegnosSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrSegno>::const_iterator i;
      for (i=fChordSegnos.begin (); i!=fChordSegnos.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the dal segnos if any
  size_t chordDalSegnosSize = fChordDalSegnos.size ();

  if (chordDalSegnosSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordDalSegnos";
    if (chordDalSegnosSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrDalSegno>::const_iterator i;
      for (i=fChordDalSegnos.begin (); i!=fChordDalSegnos.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the codas if any
  size_t chordCodasSize = fChordCodas.size ();

  if (chordCodasSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordCodas";
    if (chordCodasSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrCoda>::const_iterator i;
      for (i=fChordCodas.begin (); i!=fChordCodas.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the octave shift if any
  if (fChordOctaveShift || gMsrOahGroup->getDisplayMsrFull ()) {
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
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the harmonies associated to this chord if any
  size_t chordHarmoniesListSize = fChordHarmoniesList.size ();

  if (chordHarmoniesListSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordHarmoniesList";
    if (chordHarmoniesListSize) {
      os << std::endl;
      ++gIndenter;

      for (S_msrHarmony harmony : fChordHarmoniesList) {
        os << harmony->asString ();
      } // for

      --gIndenter;
    }
    else {
      os << ": " <<
        "[NONE]" <<
        std::endl;
    }
  }

  // print the figured bass if any
  if (fChordFiguredBass || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordFiguredBass" << ": " <<
      std::endl;

    ++gIndenter;

    if (fChordFiguredBass) {
      os <<
        fChordFiguredBass->asString () <<
        std::endl;
    }
    else {
      os << "[NONE]";
    }

    --gIndenter;
  }
  os << std::endl;

  // print the chord notes if any
  size_t chordNotesVectorSize = fChordNotesVector.size ();

  if (chordNotesVectorSize || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordNotesVector";
    if (chordNotesVectorSize) {
      os << std::endl;
      ++gIndenter;

      std::vector<S_msrNote>::const_iterator
        iBegin = fChordNotesVector.begin (),
        iEnd   = fChordNotesVector.end (),
        i      = iBegin;

      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        os << std::endl;
      } // for

      --gIndenter;
    }
    else {
      os << ":" << "[NONE]" <<
      std::endl;
    }

    os << std::endl;
  }

#ifdef MF_TRACE_IS_ENABLED
  // print the slur links if any
  size_t chordSlurLinksSize = fChordSlurLinks.size ();

  if (
    gTraceOahGroup->getTraceSlurs ()
      ||
    chordSlurLinksSize
      ||
    gMsrOahGroup->getDisplayMsrFull ()
  ) {
    os <<
      std::setw (fieldWidth) <<
      "===> fChordSlurLinks ===>";
    if (chordSlurLinksSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrChordSlurLink>::const_iterator i;
      for (i=fChordSlurLinks.begin (); i!=fChordSlurLinks.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }
#endif // MF_TRACE_IS_ENABLED

/*
  // print the chord grace notes groups if any
  if (fChordGraceNotesGroupBefore || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordGraceNotesGroupBefore";

    if (fChordGraceNotesGroupBefore) {
      os << std::endl;
      ++gIndenter;
      os <<
        fChordGraceNotesGroupBefore->asString () <<
        std::endl;
      --gIndenter;
    }
    else {
      os << ": " << "[NONE]" << std::endl; // JMI TEST
    }
//    os << std::endl;
  }

  if (fChordGraceNotesGroupAfter || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordGraceNotesGroupAfter";

    if (fChordGraceNotesGroupAfter) {
      os << std::endl;
      ++gIndenter;
      os <<
        fChordGraceNotesGroupAfter->asString ();
      --gIndenter;
    }
    else {
      os << ": " << "[NONE]";
    }
    os << std::endl;
  }
*/

  if (fChordGraceNotesGroupLinkAfter || gMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordGraceNotesGroupLinkAfter";

    if (fChordGraceNotesGroupLinkAfter) {
      os << std::endl;
      os << gTab << fChordGraceNotesGroupLinkAfter;
    }
    else {
      os << ": " << "[NONE]";
    }
    os << std::endl;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrChord::print (std::ostream& os) const
{
  msrWholeNotes
    chordMeasureFullLength =
      fChordUpLinkToMeasure
        ? fChordUpLinkToMeasure->
            getFullMeasureWholeNotesDuration ()
        : msrWholeNotes (0, 1); // JMI

  os <<
    "[Chord" <<
    ", line " << fInputLineNumber <<
    ", " <<
    mfSingularOrPlural (
      fChordNotesVector.size (), "note", "notes") <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 44;

  os << std::left <<
    std::setw (fieldWidth) <<
 // JMI   "chordSoundingWholeNotes" << ": " << fChordSoundingWholeNotes.asString () <<
    "fSoundingWholeNotes" << ": " << fSoundingWholeNotes.asString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fChordDisplayWholeNotes" << ": " << fChordDisplayWholeNotes.asString () <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "measureElementMeasureNumber" << ": ";
    if (fChordUpLinkToMeasure) {
      os <<
        fChordUpLinkToMeasure->getMeasureNumber ();
    }
    else {
      os << "[UNKNOWN_MEASURE_NUMBER]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasurePosition" << ": " << fMeasurePosition.asString () <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     "fVoicePosition" << ": " << fVoicePosition <<
//     std::endl <<
    std::setw (fieldWidth) <<
    "chordMeasureFullLength" << ": " << chordMeasureFullLength <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fChordShortcutUpLinkToTuplet" << ": " <<
    fChordShortcutUpLinkToTuplet <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fPositionInTuplet" << ": " <<
    fPositionInTuplet <<
    std::endl;

  // print the chord grace notes group link before if any
  if (fChordGraceNotesGroupLinkBefore) {
    os <<
      std::setw (fieldWidth) <<
      "===> fChordGraceNotesGroupLinkBefore ===>";

    if (fChordGraceNotesGroupLinkBefore) {
      os << std::endl;
      os << gTab << fChordGraceNotesGroupLinkBefore->asShortString ();
    }
    else {
      os << ": " << "[NONE]";
    }
    os << std::endl;
  }

/*
  // print the articulations if any
  size_t chordArticulationsSize = fChordArticulations.size ();

  if (chordArticulationsSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordArticulations";
    if (chordArticulationsSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrArticulation>::const_iterator i;
      for (i=fChordArticulations.begin (); i!=fChordArticulations.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the spanners if any
  size_t chordSpannersSize = fChordSpanners.size ();

  if (chordSpannersSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordSpanners";
    if (chordSpannersSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrSpanner>::const_iterator i;
      for (i=fChordSpanners.begin (); i!=fChordSpanners.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the technicals if any
  size_t chordTechnicalsSize = fChordTechnicals.size ();

  if (chordTechnicalsSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordTechnicals";
    if (chordTechnicalsSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrTechnical>::const_iterator i;
      for (i=fChordTechnicals.begin (); i!=fChordTechnicals.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the ornaments if any
  size_t chordOrnamentsSize = fChordOrnaments.size ();

  if (chordOrnamentsSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordOrnaments";
    if (chordOrnamentsSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrOrnament>::const_iterator i;
      for (i=fChordOrnaments.begin (); i!=fChordOrnaments.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the glissandos if any
  size_t chordGlissandosSize = fChordGlissandos.size ();

  if (chordGlissandosSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordGlissandos";
    if (chordGlissandosSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrGlissando>::const_iterator i;
      for (i=fChordGlissandos.begin (); i!=fChordGlissandos.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the slides if any
  size_t chordSlidesSize = fChordSlides.size ();

  if (chordSlidesSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordSlides";
    if (chordSlidesSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrSlide>::const_iterator i;
      for (i=fChordSlides.begin (); i!=fChordSlides.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the dynamics if any
  size_t chordDynamicsSize = fChordDynamics.size ();

  if (chordDynamicsSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordDynamics";
    if (chordDynamicsSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrDynamic>::const_iterator i;
      for (i=fChordDynamics.begin (); i!=fChordDynamics.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the other dynamics if any
  size_t chordOtherDynamicsSize = fChordOtherDynamics.size ();

  if (chordOtherDynamicsSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordOtherDynamics";
    if (chordOtherDynamicsSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrOtherDynamic>::const_iterator i;
      for (i=fChordOtherDynamics.begin (); i!=fChordOtherDynamics.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the stems if any
  size_t chordStemsSize = fChordStems.size ();

  if (chordStemsSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordStems";
    if (chordStemsSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrStem>::const_iterator i;
      for (i=fChordStems.begin (); i!=fChordStems.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }
*/

/*
  // print the beams if any
  size_t chordBeamsSize = fChordBeams.size ();

  if (chordBeamsSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordBeams";
    if (chordBeamsSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrBeam>::const_iterator i;
      for (i=fChordBeams.begin (); i!=fChordBeams.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }
*/

#ifdef MF_TRACE_IS_ENABLED
  // print the beam links if any
  size_t chordBeamLinksSize = fChordBeamLinks.size ();

  if (gTraceOahGroup->getTraceBeams () || chordBeamLinksSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordBeamLinks";
    if (chordBeamLinksSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrChordBeamLink>::const_iterator i;
      for (i=fChordBeamLinks.begin (); i!=fChordBeamLinks.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // print the words if any
  size_t chordWordsSize = fChordWords.size ();

  if (chordWordsSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordWords";
    if (chordWordsSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrWords>::const_iterator i;
      for (i=fChordWords.begin (); i!=fChordWords.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the ties if any
  size_t chordTiesSize = fChordTies.size ();

  if (chordTiesSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordTies";
    if (chordTiesSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrTie>::const_iterator i;
      for (i=fChordTies.begin (); i!=fChordTies.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

#ifdef MF_TRACE_IS_ENABLED
  // print the slur links if any
  size_t chordSlurLinksSize = fChordSlurLinks.size ();

  if (gTraceOahGroup->getTraceSlurs () || chordSlurLinksSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordSlurLinks";
    if (chordSlurLinksSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrChordSlurLink>::const_iterator i;
      for (i=fChordSlurLinks.begin (); i!=fChordSlurLinks.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }
#endif // MF_TRACE_IS_ENABLED

  // print the ligatures if any
  size_t chordLigaturesSize = fChordLigatures.size ();

  if (chordLigaturesSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordLigatures";
    if (chordLigaturesSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrLigature>::const_iterator i;
      for (i=fChordLigatures.begin (); i!=fChordLigatures.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

/*
  // print the slashes if any
  size_t chordSlashesSize = fChordSlashes.size ();

  if (chordSlashesSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordSlashes";
    if (chordSlashesSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrSlash>::const_iterator i;
      for (i=fChordSlashes.begin (); i!=fChordSlashes.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
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

      std::list<S_msrCrescDescrec>::const_iterator i;
      for (i=fChordCrescDescrecs.begin (); i!=fChordCrescDescrecs.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the wedges if any
  size_t chordWedgesSize = fChordWedges.size ();

  if (chordWedgesSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordWedges";
    if (chordWedgesSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrWedge>::const_iterator i;
      for (i=fChordWedges.begin (); i!=fChordWedges.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the segnos if any
  size_t chordSegnosSize = fChordSegnos.size ();

  if (chordSegnosSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordSegnos";
    if (chordSegnosSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrSegno>::const_iterator i;
      for (i=fChordSegnos.begin (); i!=fChordSegnos.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the dal segnos if any
  size_t chordDalSegnosSize = fChordDalSegnos.size ();

  if (chordDalSegnosSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordDalSegnos";
    if (chordDalSegnosSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrDalSegno>::const_iterator i;
      for (i=fChordDalSegnos.begin (); i!=fChordDalSegnos.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
      std::endl;
    }
  }

  // print the codas if any
  size_t chordCodasSize = fChordCodas.size ();

  if (chordCodasSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordCodas";
    if (chordCodasSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrCoda>::const_iterator i;
      for (i=fChordCodas.begin (); i!=fChordCodas.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        ": " << "[NONE]" <<
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
        ": " << "[NONE]" <<
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

      std::list<S_msrHarmony>::const_iterator
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
        "[NONE]" <<
        std::endl;
    }
  }

  // print the figured bass if any
  if (fChordFiguredBass) {
    os <<
      std::setw (fieldWidth) <<
      "fChordFiguredBass" << ": " <<
      std::endl;

    ++gIndenter;

    if (fChordFiguredBass) {
      os <<
        fChordFiguredBass->asString () <<
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

      std::vector<S_msrNote>::const_iterator
        iBegin = fChordNotesVector.begin (),
        iEnd   = fChordNotesVector.end (),
        i      = iBegin;

      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        os << std::endl;
      } // for

      --gIndenter;
    }
    else {
      os << ":" << "[NONE]" <<
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
  if (fChordGraceNotesGroupLinkAfter) {
    os <<
      std::setw (fieldWidth) <<
      "===> fChordGraceNotesGroupLinkAfter ===>";

    if (fChordGraceNotesGroupLinkAfter) {
      os << std::endl;
      os << gTab << fChordGraceNotesGroupLinkAfter->asShortString ();
    }
    else {
      os << ": " << "[NULL]";
    }
    os << std::endl;
  }

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
        fInputLineNumber,
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
    ", fOriginalBeam \"" <<
    fOriginalBeam->asShortString () <<
    ", fUpLinkToChord \"" <<
    fUpLinkToChord->asShortString () <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

std::string msrChordBeamLink::asString () const
{
  std::stringstream ss;

  ss <<
    "[ChordBeamLink" <<
    ", fOriginalBeam \"" <<
    fOriginalBeam->asString () <<
    ", fUpLinkToChord \"" <<
    fUpLinkToChord->asString () <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrChordBeamLink::printFull (std::ostream& os) const
{
  os <<
    "[ChordBeamLink" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os <<
    std::setw (fieldWidth) <<
    "fOriginalBeam:" <<
    std::endl;

  ++gIndenter;
  os <<
    fOriginalBeam; // <<
    fUpLinkToChord->asString ();
  --gIndenter;

  --gIndenter;

  os << ']' << std::endl;
}

void msrChordBeamLink::print (std::ostream& os) const
{
  os <<
    "[ChordBeamLink" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os <<
    std::setw (fieldWidth) <<
    "originalBfOriginalBeameam:" <<
    std::endl;

  ++gIndenter;
  os << fOriginalBeam;
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
        fInputLineNumber,
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
    ", originalSlur \"" <<
    fOriginalSlur->asShortString () <<
    ", upLinkToChord \"" <<
    fUpLinkToChord->asShortString () <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

std::string msrChordSlurLink::asString () const
{
  std::stringstream ss;

  ss <<
    "[ChordSlurLink" <<
    ", originalSlur \"" <<
    fOriginalSlur->asString () <<
    ", upLinkToChord \"" <<
    fUpLinkToChord->asString () <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrChordSlurLink::print (std::ostream& os) const
{
  os <<
    "[ChordSlurLink" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os <<
    std::setw (fieldWidth) <<
    "originalSlur:" <<
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
        fInputLineNumber,
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
  std::list<S_msrMeasureElement>::const_iterator i;

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
    ", upLinkToChord \"" <<
    fUpLinkToChord->asShortString () <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

std::string msrChordGraceNotesGroupLink::asString () const
{
  std::stringstream ss;

  ss <<
    "[chordGraceNotesGroupLink" <<
    ", fOriginalGraceNotesGroup \"" <<
    fOriginalGraceNotesGroup->asString () <<
    ", fUpLinkToChord \"" <<
    fUpLinkToChord->asString () <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrChordGraceNotesGroupLink::print (std::ostream& os) const
{
  os <<
    "[ChordGraceNotesGroupLink" <<
    ", line " << fInputLineNumber <<
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
    fUpLinkToChord->asString () <<
    std::endl;
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
