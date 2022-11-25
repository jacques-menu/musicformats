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

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"

// #include "msrMeasureConstants.h"

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


namespace MusicFormats
{

// chords
//______________________________________________________________________________

std::string msrChordInKindAsString (
  msrChordInKind chordInKind)
{
  std::string result;

  switch (chordInKind) {
    case msrChordInKind::kChordIn_NO_:
      result = "kChordIn_NO_";
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
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const Rational&     chordSoundingWholeNotes,
  const Rational&     chordDisplayWholeNotes,
  msrDurationKind     chordGraphicDurationKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Creating a chord" <<
      ", chordSoundingWholeNotes = " << chordSoundingWholeNotes <<
      ", chordDisplayWholeNotes = " << chordDisplayWholeNotes <<
      ", chordGraphicDuration = " <<
      msrDurationKindAsString (chordGraphicDurationKind) <<
      std::endl;
 }
#endif

  msrChord* o =
    new msrChord (
      inputLineNumber,
      upLinkToMeasure,
      chordSoundingWholeNotes, chordDisplayWholeNotes,
      chordGraphicDurationKind);
  assert (o != nullptr);
  return o;
}

S_msrChord msrChord::create (
  int                 inputLineNumber,
  const Rational&     chordSoundingWholeNotes,
  const Rational&     chordDisplayWholeNotes,
  msrDurationKind     chordGraphicDurationKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Creating a chord" <<
      ", chordSoundingWholeNotes = " << chordSoundingWholeNotes <<
      ", chordDisplayWholeNotes = " << chordDisplayWholeNotes <<
      ", chordGraphicDuration = " <<
      msrDurationKindAsString (chordGraphicDurationKind) <<
      std::endl;
 }
#endif

  return
    msrChord::create (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer, // set later in setChordUpLinkToMeasure()
      chordSoundingWholeNotes, chordDisplayWholeNotes,
      chordGraphicDurationKind);
}

msrChord::msrChord (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const Rational&     chordSoundingWholeNotes,
  const Rational&     chordDisplayWholeNotes,
  msrDurationKind     chordGraphicDurationKind)
    : msrTupletElement (
        inputLineNumber)
{
  fChordKind = msrChordInKind::kChordIn_NO_;

  fChordUpLinkToMeasure = upLinkToMeasure;

  doSetMeasureElementSoundingWholeNotes (
    chordSoundingWholeNotes,
    "msrChord::msrChord()");

  fChordDisplayWholeNotes  = chordDisplayWholeNotes;

  fChordGraphicDurationKind = chordGraphicDurationKind;

  fChordIsFirstChordInADoubleTremolo  = false;
  fChordIsSecondChordInADoubleTremolo = false;
}

msrChord::~msrChord ()
{}

S_msrChord msrChord::createChordNewbornClone (
  const S_msrPart& containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Creating a newborn clone of chord '" <<
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

  S_msrChord
    newbornClone =
      msrChord::create (
        fInputLineNumber,
	      gGlobalNullMeasureSmartPointer, // set later in setChordUpLinkToMeasure()
        fMeasureElementSoundingWholeNotes,
        fChordDisplayWholeNotes,
        fChordGraphicDurationKind);

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
//   const Rational&    measurePosition,
//   const std::string&      context)
// {
// #ifdef TRACING_IS_ENABLED
//   if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
//     gLogStream <<
//       "Setting chord's position in measure of " <<
//       asString () <<
//       " to " <<
//       measurePosition <<
//       " (was " <<
//       fMeasureElementMeasurePosition <<
//       ") in measure " <<
//       measure->asShortString () <<
//       " (measureElementMeasureNumber: " <<
//       fChordUpLinkToMeasure->getMeasureNumber () <<
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
//     measurePosition != msrMoment::K_NO_POSITION,
//     "measurePosition == msrMoment::K_NO_POSITION");
//
//   // set chord's position in measure
//   fMeasureElementMeasurePosition = measurePosition;
// }

// uplink to measure
// S_msrMeasure msrChord::fetchMeasureElementUpLinkToMeasure () const
// {
//   S_msrMeasure result;
//
//   switch (fChordKind) {
//     case msrChordInKind::kChordIn_NO_:
//       break;
//
//     case msrChordInKind::kChordInMeasure:
//       result = fChordUpLinkToMeasure;
//       break;
//
//     case msrChordInKind::kChordInTuplet:
//       if (fChordDirectUpLinkToTuplet) {
//         result =
//           fChordDirectUpLinkToTuplet->
//             fChordUpLinkToMeasure;
//       }
//       break;
//
//     case msrChordInKind::kChordInGraceNotesGroup:
//       if (fChordDirectUpLinkToGraceNotesGroup) {
//         result =
//           fChordDirectUpLinkToGraceNotesGroup->
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
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of chord " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

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
    case msrChordInKind::kChordIn_NO_:
      break;

    case msrChordInKind::kChordInMeasure:
      break;

    case msrChordInKind::kChordInTuplet:
      result = fChordDirectUpLinkToTuplet;
      break;

    case msrChordInKind::kChordInGraceNotesGroup:
      if (fChordDirectUpLinkToGraceNotesGroup) {
        result =
          fChordDirectUpLinkToGraceNotesGroup->
            getGraceNotesGroupUpLinkToNote ()->
              getNoteDirectUpLinkToTuplet ();
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
    case msrChordInKind::kChordIn_NO_:
      break;
    case msrChordInKind::kChordInMeasure:
      break;
    case msrChordInKind::kChordInTuplet:
      break;
    case msrChordInKind::kChordInGraceNotesGroup:
      result = fChordDirectUpLinkToGraceNotesGroup; // JMI
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
   const Rational& wholeNotes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Setting chord displayed whole notes to '" <<
      wholeNotes <<
      "' for chord '" <<
      asString () <<
      "'" <<
      std::endl;
  }
#endif

  fChordDisplayWholeNotes = wholeNotes;
}

void msrChord::setChordGraceNotesGroupLinkBefore (
  int     inputLineNumber,
  const S_msrChordGraceNotesGroupLink&
          chordChordGraceNotesGroupLinkBefore)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Setting chord grace notes groups before in " <<
      asString () <<
      " to '" <<
      chordChordGraceNotesGroupLinkBefore->asShortString () <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

  fChordGraceNotesGroupLinkBefore =
    chordChordGraceNotesGroupLinkBefore;
}

void msrChord::setChordGraceNotesGroupLinkAfter (
  int     inputLineNumber,
  const S_msrChordGraceNotesGroupLink&
          chordChordGraceNotesGroupLinkAfter)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Setting chord grace notes groups after in " <<
      asString () <<
      " to '" <<
      chordChordGraceNotesGroupLinkAfter->asShortString () <<
      "', line " << inputLineNumber <<
      std::endl;
  }
#endif

  fChordGraceNotesGroupLinkAfter =
    chordChordGraceNotesGroupLinkAfter;
}

void msrChord::setChordMembersMeasurePosition (
  const S_msrMeasure& measure,
  const Rational&     measurePosition)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Setting chord members positions in measure of " << asString () <<
      " to '" <<
      measurePosition <<
      "'" <<
      std::endl;
  }
#endif

  std::string context =
    "setChordMembersMeasurePosition()";

//   setChordMeasurePosition (
//     measure,
//     measurePosition,
//     context);

  if (false) { // JMI CAFE
  // compute chord's position in voice
  Rational
    voicePosition =
      fChordUpLinkToMeasure->
        getMeasureVoicePosition ()
        +
      measurePosition;

  // set chord's position in voice
  setMeasureElementVoicePosition (
    voicePosition,
    context);

  // update current position in voice
  S_msrVoice
    voice =
      measure->
        fetchMeasureUpLinkToVoice ();

  voice->
    incrementCurrentVoicePosition (
      fChordNotesVector [0]->
        getMeasureElementSoundingWholeNotes ());
}

  // set the chord's elements' position in measure
  if (fChordNotesVector.size ()) {
    for (S_msrNote note : fChordNotesVector) {
      // set note's uplink to measure
      note->
        setNoteUpLinkToMeasure (
          measure);

      // set note's position in measure
//       note->
//         setMeasureElementMeasurePosition (
//           measure,
//           measurePosition, // they all share the same one
//           "chord member");

//    JMI   set note's position in voice v0.9.66
//       note->
//         setMeasureElementVoicePosition (
//           voicePosition,
//           context); // they all share the same one
    } // for
  }

  // are there dal segnos attached to this chord?
  if (fChordDalSegnos.size ()) {
    for (S_msrDalSegno dalSegno : fChordDalSegnos) {
      // set the dal segno position in measure
//       dalSegno->
//         setDalSegnoMeasurePosition (
//           measure,
//           measurePosition,
//           "msrChord::setChordMembersMeasurePosition()");
    } // for
  }
}

void msrChord::addFirstNoteToChord (
  const S_msrNote&  note,
  const S_msrVoice& voice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Adding first note '" <<
      note->asString () <<
      "' to chord '" <<
      asString () <<
      "'" <<
      std::endl;
  }
#endif

  // append note to chord notes
  fChordNotesVector.push_back (note);

  // register note's uplink to chord
  note->
    setNoteDirectUpLinkToChord (this);

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Adding another note '" <<
      note->asShortString () <<
      "' to chord '" <<
      asString () <<
      "'" <<
      std::endl;
  }
#endif

//   gLogStream << "++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
//   print (gLogStream); // JMI v0.9.66
//   gLogStream << "++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;

  fChordNotesVector.push_back (note);

  // register note's uplink to chord
  note->
    setNoteDirectUpLinkToChord (this);

  // mark note as belonging to a chord
  note->setNoteBelongsToAChord ();

  // append the note to the measure's notes flat std::list
  if (false) // JMI
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
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      "cannot access the first note of an empty chord");
  }

  return result;
}

void msrChord::appendDalSegnoToChord (
  const S_msrDalSegno& dalSegno)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDalSegnos ()) {
    gLogStream <<
      "Appending dal segno " <<
      dalSegno->asShortString () <<
      " to chord " <<
      asShortString () <<
      std::endl;
  }
#endif

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceArticulations ()) {
    gLogStream <<
      "Appending articulation '" <<
      art->getArticulationKind () <<
      "' to chord" <<
      std::endl;
  }
#endif

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSpanners ()) {
    gLogStream <<
      "Appending spanner '" <<
      span->getSpannerKind () <<
      "' to chord '" <<
      asString () <<
      "'" <<
      std::endl;
  }
#endif

  fChordSpanners.push_back (span);
}

void msrChord::setChordSingleTremolo (
  const S_msrSingleTremolo& trem)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTremolos ()) {
    gLogStream <<
      "Adding singleTremolo '" <<
      trem->asString () <<
      "' to chord" <<
      std::endl;
  }
#endif

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Appending technical '" <<
      tech->asString () <<
      "' to chord" <<
      std::endl;
  }
#endif

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Appending technical with integer '" <<
      tech->asString () <<
      "' to chord" <<
      std::endl;
  }
#endif

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Appending technical with integer '" <<
      tech->asString () <<
      "' to chord" <<
      std::endl;
  }
#endif

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Appending technical with std::string '" <<
      tech->asString () <<
      "' to chord" <<
      std::endl;
  }
#endif

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Appending ornament '" <<
      msrOrnamentKindAsString (orn->getOrnamentKind ()) <<
      "' to chord" <<
      std::endl;
  }
#endif

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Appending glissando '" <<
      gliss->asString () <<
      "' to chord" <<
      std::endl;
  }
#endif

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Appending slide '" <<
      slide->asString () <<
      "' to chord" <<
      std::endl;
  }
#endif

  fChordSlides.push_back (slide);
}

void msrChord::appendChordSlurLinkToChord (
  const S_msrChordSlurLink& chordSlurLink)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSlurs ()) {
    gLogStream <<
      "Adding slur link '" << chordSlurLink->asString() <<
      "' to chord '" << asString () << "'" <<
      std::endl;
  }
#endif

  fChordSlurLinks.push_back (chordSlurLink);
}

void msrChord::appendStemToChord (
  const S_msrStem& stem)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStems ()) {
    gLogStream <<
      "Adding stem '" << stem->asString() <<
      "' to chord '" << asString () << "'" <<
      std::endl;
  }
#endif

  // sanity check
  for (
    std::list<S_msrStem>::const_iterator i = fChordStems.begin ();
    i != fChordStems.end ();
    ++i
  ) {
    // is stem's stem kind consistent with the other ones' for this chord?
    if (stem->getStemKind () != (*i)->getStemKind ()) {
      std::stringstream s;

      s <<
        "stem " << stem->asString () <<
        " and " << (*i)->asString () <<
        " don't have the same kind, but are in one and the same chord";

//      msrInternalError ( // not internal actually JMI ???
      msrInternalWarning ( // not internal actually JMI ???
        gGlobalServiceRunData->getInputSourceName (),
        fInputLineNumber,
  //      __FILE__, __LINE__,
        s.str ());
    }
  } // for

  fChordStems.push_back (stem);
}

/* JMI
void msrChord::appendBeamToChord (const S_msrBeam& beam)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeams ()) {
    gLogStream <<
      "Appending beam '" <<
      beam->asString () <<
      "' to chord" <<
      std::endl;
  }
#endif

  fChordBeams.push_back (beam);
}
*/

void msrChord::appendChordBeamLinkToChord (
  const S_msrChordBeamLink& chordBeamLink)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeams ()) {
    gLogStream <<
      "Adding beam link '" << chordBeamLink->asString() <<
      "' to chord '" << asString () << "'" <<
      std::endl;
  }
#endif

  fChordBeamLinks.push_back (chordBeamLink);
}

void msrChord::finalizeChord (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Finalizing chord '" <<
      asString () <<
      "', line " << inputLineNumber <<
      std::endl <<
      "fMeasureElementMeasurePosition = " <<
      std::endl <<
      fMeasureElementMeasurePosition <<
      std::endl;
  }
#endif

  // we can now set the position in measures for all the chord members JMI v0.9.66
//   setChordMembersMeasurePosition (
//     fChordUpLinkToMeasure,
//     fMeasureElementMeasurePosition);
}

void msrChord::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrChord::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrChord::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrChord::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrChord::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrChord::visitEnd ()" <<
            std::endl;
        }
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
          std::endl;
      }
#endif

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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTuplets ()) {
    gLogStream <<
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
#endif
*/

std::string msrChord::asStringwithRawDivisions () const
{
  std::stringstream s;

  s << "<";

  if (fChordNotesVector.size ()) {
    std::vector<S_msrNote>::const_iterator
      iBegin = fChordNotesVector.begin (),
      iEnd   = fChordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote
        note = (*i);

      s <<
      /* JMI
        note->notePitchAsString () <<
        note->noteSoundingWholeNotesAsMsrString () <<
        '[' << note->getNoteOctave () << ']'
        */

        note->asShortStringWithRawWholeNotes ();

      if (++i == iEnd) break;
      s << ' ';
    } // for
  }

  s << ">";

  return s.str ();
}

std::string msrChord::asString () const
{
  std::stringstream s;

  s <<
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

      s <<
        note->notePitchAsString () <<
        ", whole notes: " <<
        " sounding " <<
        note->getMeasureElementSoundingWholeNotes () <<
        ", displayed " <<
        note->getNoteDisplayWholeNotes () <<
        ", octave: " <<
        msrOctaveKindAsString (note->getNoteOctaveKind ()) <<
        ']';

      if (++i == iEnd) break;
      s << ' ';
    } // for
  }

  s << ">]";

  return s.str ();
}

std::string msrChord::asShortString () const
{
  std::stringstream s;

  s <<
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

      s <<
        note->notePitchAsString () <<
        " snd: " <<
        note->getMeasureElementSoundingWholeNotes () <<
        ", disp: " <<
        note->getNoteDisplayWholeNotes () <<
        ", " <<
        msrOctaveKindAsString (note->getNoteOctaveKind ());

      if (++i == iEnd) break;
      s << ", ";
    } // for
  }

  s << ">";

  return s.str ();
}

void msrChord::print (std::ostream& os) const
{
  Rational
    chordMeasureFullLength =
      fChordUpLinkToMeasure
        ? fChordUpLinkToMeasure->
            getFullMeasureWholeNotesDuration ()
        : Rational (0, 1); // JMI

  os <<
    "[Chord" <<
    ", chordKind: " << fChordKind <<
    ", " <<
    mfSingularOrPlural (
      fChordNotesVector.size (), "note", "notes") <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 44;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureElementSoundingWholeNotes" << " : " << fMeasureElementSoundingWholeNotes <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fChordDisplayWholeNotes" << " : " << fChordDisplayWholeNotes <<
    std::endl <<
    std::setw (fieldWidth) <<
    "measureElementMeasureNumber" << " : " <<
    fChordUpLinkToMeasure->getMeasureNumber () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMeasureElementMeasurePosition" << " : " << fMeasureElementMeasurePosition <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     "fMeasureElementVoicePosition" << " : " << fMeasureElementVoicePosition <<
//     std::endl <<
    std::setw (fieldWidth) <<
    "chordMeasureFullLength" << " : " << chordMeasureFullLength <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fChordUpLinkToMeasure" << " : ";
  if (fChordDirectUpLinkToTuplet) {
    os <<
      fChordUpLinkToMeasure->asShortString ();
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fChordDirectUpLinkToTuplet" << " : ";
  if (fChordDirectUpLinkToTuplet) {
    os <<
      fChordDirectUpLinkToTuplet->asShortString ();
  }
  else {
    os << "[NONE]";
  }
  os << std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fPositionInTuplet" << " : " <<
    fPositionInTuplet <<
    std::endl;

  // print simplified position in measure if relevant
// JMI  if (fChordUpLinkToMeasure) {
    // the chord uplink to measure may not have been set yet
    Rational
      chordPositionBis =
        fMeasureElementMeasurePosition;

    if (
      chordPositionBis.getNumerator ()
        !=
      fMeasureElementMeasurePosition.getNumerator ()
    ) {
      // print rationalised Rational view
      os << std::left <<
        std::setw (fieldWidth) <<
        "chordPositionBis" << " : " << chordPositionBis <<
        std::endl;
    }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fChordIsFirstChordInADoubleTremolo" << " : " <<
    fChordIsFirstChordInADoubleTremolo <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fChordIsSecondChordInADoubleTremolo" << " : " <<
    fChordIsSecondChordInADoubleTremolo <<
    std::endl << std::endl;

  // print the chord grace notes groups links if any
  if (fChordGraceNotesGroupLinkBefore || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordGraceNotesGroupLinkBefore";

    if (fChordGraceNotesGroupLinkBefore) {
      os << std::endl;
      os << gTab << fChordGraceNotesGroupLinkBefore;
    }
    else {
      os << " : " << "[NONE]" << std::endl; // JMI TEST
    }
  }

  // print the articulations if any
  size_t chordArticulationsSize = fChordArticulations.size ();

  if (chordArticulationsSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

  // print the spanners if any
  size_t chordSpannersSize = fChordSpanners.size ();

  if (chordSpannersSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

  // print the technicals if any
  size_t chordTechnicalsSize = fChordTechnicals.size ();

  if (chordTechnicalsSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

  // print the ornaments if any
  size_t chordOrnamentsSize = fChordOrnaments.size ();

  if (chordOrnamentsSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

  // print the glissandos if any
  size_t chordGlissandosSize = fChordGlissandos.size ();

  if (chordGlissandosSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

  // print the slides if any
  size_t chordSlidesSize = fChordSlides.size ();

  if (chordSlidesSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

  // print the dynamics if any
  size_t chordDynamicsSize = fChordDynamics.size ();

  if (chordDynamicsSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

  // print the other dynamics if any
  size_t chordOtherDynamicsSize = fChordOtherDynamics.size ();

  if (chordOtherDynamicsSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

  // print the stems if any
  size_t chordStemsSize = fChordStems.size ();

  if (chordStemsSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

/* JMI
  // print the beams if any
  size_t chordBeamsSize = fChordBeams.size ();

  if (chordBeamsSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }
*/
#ifdef TRACING_IS_ENABLED
  // print the beam links if any
  size_t chordBeamLinksSize = fChordBeamLinks.size ();

  if (gGlobalTracingOahGroup->getTraceBeams () || chordBeamLinksSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }
#endif

  // print the words if any
  size_t chordWordsSize = fChordWords.size ();

  if (chordWordsSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

  // print the ties if any
  size_t chordTiesSize = fChordTies.size ();

  if (chordTiesSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

  // print the ligatures if any
  size_t chordLigaturesSize = fChordLigatures.size ();

  if (chordLigaturesSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

  // print the slashes if any
  size_t chordSlashesSize = fChordSlashes.size ();

  if (chordSlashesSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

  // print the cresc/decresc if any
  size_t chordCrescDecrescsSize = fChordCrescDecrescs.size ();

  if (chordCrescDecrescsSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

  // print the wedges if any
  size_t chordWedgesSize = fChordWedges.size ();

  if (chordWedgesSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

  // print the segnos if any
  size_t chordSegnosSize = fChordSegnos.size ();

  if (chordSegnosSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

  // print the dal segnos if any
  size_t chordDalSegnosSize = fChordDalSegnos.size ();

  if (chordDalSegnosSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

  // print the codas if any
  size_t chordCodasSize = fChordCodas.size ();

  if (chordCodasSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

  // print the octave shift if any
  if (fChordOctaveShift || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

  // print the harmonies associated to this chord if any
  size_t chordHarmoniesListSize = fChordHarmoniesList.size ();

  if (chordHarmoniesListSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
      os << " : " <<
        "[NONE]" <<
        std::endl;
    }
  }

  // print the figured bass if any
  if (fChordFiguredBass || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordFiguredBass" << " : " <<
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

  if (chordNotesVectorSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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

#ifdef TRACING_IS_ENABLED
  // print the slur links if any
  size_t chordSlurLinksSize = fChordSlurLinks.size ();

  if (gGlobalTracingOahGroup->getTraceSlurs () || chordSlurLinksSize || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }
#endif

/*
  // print the chord grace notes groups if any
  if (fChordGraceNotesGroupBefore || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
      os << " : " << "[NONE]" << std::endl; // JMI TEST
    }
//    os << std::endl;
  }

  if (fChordGraceNotesGroupAfter || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
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
      os << " : " << "[NONE]";
    }
    os << std::endl;
  }
*/

  if (fChordGraceNotesGroupLinkAfter || gGlobalMsrOahGroup->getDisplayMsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fChordGraceNotesGroupLinkAfter";

    if (fChordGraceNotesGroupLinkAfter) {
      os << std::endl;
      os << gTab << fChordGraceNotesGroupLinkAfter;
    }
    else {
      os << " : " << "[NONE]";
    }
    os << std::endl;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrChord::printShort (std::ostream& os) const
{
  Rational
    chordMeasureFullLength =
      fChordUpLinkToMeasure
        ? fChordUpLinkToMeasure->
            getFullMeasureWholeNotesDuration ()
        : Rational (0, 1); // JMI

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
 // JMI   "chordSoundingWholeNotes" << " : " << fChordSoundingWholeNotes <<
    "fMeasureElementSoundingWholeNotes" << " : " << fMeasureElementSoundingWholeNotes <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fChordDisplayWholeNotes" << " : " << fChordDisplayWholeNotes <<
    std::endl <<
    std::setw (fieldWidth) <<
    "measureElementMeasureNumber" << " : " <<
    fChordUpLinkToMeasure->getMeasureNumber () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMeasureElementMeasurePosition" << " : " << fMeasureElementMeasurePosition <<
    std::endl <<
//     std::setw (fieldWidth) <<
//     "fMeasureElementVoicePosition" << " : " << fMeasureElementVoicePosition <<
//     std::endl <<
    std::setw (fieldWidth) <<
    "chordMeasureFullLength" << " : " << chordMeasureFullLength <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fChordDirectUpLinkToTuplet" << " : " <<
    fChordDirectUpLinkToTuplet <<
    std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fPositionInTuplet" << " : " <<
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
      os << " : " << "[NONE]";
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
        " : " << "[NONE]" <<
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
        " : " << "[NONE]" <<
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
        " : " << "[NONE]" <<
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
        " : " << "[NONE]" <<
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
        " : " << "[NONE]" <<
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
        " : " << "[NONE]" <<
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
        " : " << "[NONE]" <<
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
        " : " << "[NONE]" <<
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
        " : " << "[NONE]" <<
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }
*/

#ifdef TRACING_IS_ENABLED
  // print the beam links if any
  size_t chordBeamLinksSize = fChordBeamLinks.size ();

  if (gGlobalTracingOahGroup->getTraceBeams () || chordBeamLinksSize) {
    os <<
      std::setw (fieldWidth) <<
      "fChordBeamLinks";
    if (chordBeamLinksSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_msrChordBeamLink>::const_iterator i;
      for (i=fChordBeamLinks.begin (); i!=fChordBeamLinks.end (); ++i) {
        (*i)->printShort(os);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "[NONE]" <<
      std::endl;
    }
  }
#endif

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
        " : " << "[NONE]" <<
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }

#ifdef TRACING_IS_ENABLED
  // print the slur links if any
  size_t chordSlurLinksSize = fChordSlurLinks.size ();

  if (gGlobalTracingOahGroup->getTraceSlurs () || chordSlurLinksSize) {
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
        " : " << "[NONE]" <<
      std::endl;
    }
  }
#endif

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
        " : " << "[NONE]" <<
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
        " : " << "[NONE]" <<
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
        " : " << "[NONE]" <<
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
        " : " << "[NONE]" <<
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
        " : " << "[NONE]" <<
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
        " : " << "[NONE]" <<
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
        " : " << "[NONE]" <<
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
        " : " << "[NONE]" <<
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
      os << " : " <<
        "[NONE]" <<
        std::endl;
    }
  }

  // print the figured bass if any
  if (fChordFiguredBass) {
    os <<
      std::setw (fieldWidth) <<
      "fChordFiguredBass" << " : " <<
      std::endl;

    ++gIndenter;

    if (fChordFiguredBass) {
      os <<
        fChordFiguredBass->asString () <<
        std::endl;
    }
    else {
      os <<
        "[NONE]";
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
        (*i)->printShort (os);
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
      fChordGraceNotesGroupBefore->printShort (os);
      --gIndenter;
    }
    else {
      os << " : " << "[NONE]" << std::endl; // JMI TEST
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
      fChordGraceNotesGroupAfter->printShort (os);
      --gIndenter;
    }
    else {
      os << " : " << "[NONE]";
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
      os << " : " << "[NONE]";
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
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrChordBeamLink msrChordBeamLink::create (
  int        inputLineNumber,
  const S_msrBeam&  originalBeam,
  const S_msrChord& upLinkToChord)
{
  msrChordBeamLink* o =
    new msrChordBeamLink (
      inputLineNumber,
      originalBeam,
      upLinkToChord);
  assert (o != nullptr);
  return o;
}

msrChordBeamLink::msrChordBeamLink (
  int        inputLineNumber,
  const S_msrBeam&  originalBeam,
  const S_msrChord& upLinkToChord)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalBeam != nullptr,
    "originalBeam is null");

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToChord != nullptr,
    "upLinkToChord is null");

  fUpLinkToChord = upLinkToChord;

  fOriginalBeam = originalBeam;
}

msrChordBeamLink::~msrChordBeamLink ()
{}

S_msrChordBeamLink msrChordBeamLink::createBeamNewbornClone ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Creating a newborn clone of grace notes group link '" <<
      asShortString () <<
      "'" <<
      std::endl;
  }
#endif

/* JMI
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");
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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrChordBeamLink::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrChordBeamLink>*
    p =
      dynamic_cast<visitor<S_msrChordBeamLink>*> (v)) {
        S_msrChordBeamLink elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrChordBeamLink::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrChordBeamLink::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrChordBeamLink::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrChordBeamLink>*
    p =
      dynamic_cast<visitor<S_msrChordBeamLink>*> (v)) {
        S_msrChordBeamLink elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrChordBeamLink::visitEnd ()" <<
            std::endl;
        }
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
  std::stringstream s;

  s <<
    "[ChordBeamLink" <<
    ", originalBeam \"" <<
    fOriginalBeam->asShortString () <<
    ", upLinkToChord \"" <<
    fUpLinkToChord->asShortString () <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

std::string msrChordBeamLink::asString () const
{
  std::stringstream s;

  s <<
    "[ChordBeamLink" <<
    ", originalBeam \"" <<
    fOriginalBeam->asString () <<
    ", upLinkToChord \"" <<
    fUpLinkToChord->asString () <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
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
    "originalBeam:" <<
    std::endl;

  ++gIndenter;
  os <<
    fOriginalBeam; // <<
    fUpLinkToChord->asString ();
  --gIndenter;

  --gIndenter;

  os << ']' << std::endl;
}

void msrChordBeamLink::printShort (std::ostream& os) const
{
  os <<
    "[ChordBeamLink" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os <<
    std::setw (fieldWidth) <<
    "originalBeam:" <<
    std::endl;

  ++gIndenter;
  fOriginalBeam->printShort (os);
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
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrChordSlurLink msrChordSlurLink::create (
  int        inputLineNumber,
  const S_msrSlur&  originalSlur,
  const S_msrChord& upLinkToChord)
{
  msrChordSlurLink* o =
    new msrChordSlurLink (
      inputLineNumber,
      originalSlur,
      upLinkToChord);
  assert (o != nullptr);
  return o;
}

msrChordSlurLink::msrChordSlurLink (
  int        inputLineNumber,
  const S_msrSlur&  originalSlur,
  const S_msrChord& upLinkToChord)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalSlur != nullptr,
    "originalSlur is null");

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToChord != nullptr,
    "upLinkToChord is null");

  fUpLinkToChord = upLinkToChord;

  fOriginalSlur = originalSlur;
}

msrChordSlurLink::~msrChordSlurLink ()
{}

S_msrChordSlurLink msrChordSlurLink::createSlurNewbornClone ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Creating a newborn clone of grace notes group link '" <<
      asShortString () <<
      "'" <<
      std::endl;
  }
#endif

/* JMI
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");
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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrChordSlurLink::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrChordSlurLink>*
    p =
      dynamic_cast<visitor<S_msrChordSlurLink>*> (v)) {
        S_msrChordSlurLink elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrChordSlurLink::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrChordSlurLink::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrChordSlurLink::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrChordSlurLink>*
    p =
      dynamic_cast<visitor<S_msrChordSlurLink>*> (v)) {
        S_msrChordSlurLink elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrChordSlurLink::visitEnd ()" <<
            std::endl;
        }
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
  std::stringstream s;

  s <<
    "[ChordSlurLink" <<
    ", originalSlur \"" <<
    fOriginalSlur->asShortString () <<
    ", upLinkToChord \"" <<
    fUpLinkToChord->asShortString () <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

std::string msrChordSlurLink::asString () const
{
  std::stringstream s;

  s <<
    "[ChordSlurLink" <<
    ", originalSlur \"" <<
    fOriginalSlur->asString () <<
    ", upLinkToChord \"" <<
    fUpLinkToChord->asString () <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
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

void msrChordSlurLink::printShort (std::ostream& os) const
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
  fOriginalSlur->printShort (os);
  os <<
    fUpLinkToChord->asShortString () <<
    std::endl;
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
    os << "[NONE]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrChordGraceNotesGroupLink msrChordGraceNotesGroupLink::create (
  int                  inputLineNumber,
  const S_msrGraceNotesGroup& originalGraceNotesGroup,
  const S_msrChord&           upLinkToChord)
{
  msrChordGraceNotesGroupLink* o =
    new msrChordGraceNotesGroupLink (
      inputLineNumber,
      originalGraceNotesGroup,
      upLinkToChord);
  assert (o != nullptr);
  return o;
}

msrChordGraceNotesGroupLink::msrChordGraceNotesGroupLink (
  int                  inputLineNumber,
  const S_msrGraceNotesGroup& originalGraceNotesGroup,
  const S_msrChord&           upLinkToChord)
    : msrElement (inputLineNumber)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    originalGraceNotesGroup != nullptr,
    "originalGraceNotesGroup is null");

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    upLinkToChord != nullptr,
    "upLinkToChord is null");

  fUpLinkToChord = upLinkToChord;

  fOriginalGraceNotesGroup = originalGraceNotesGroup;
}

msrChordGraceNotesGroupLink::~msrChordGraceNotesGroupLink ()
{}

S_msrChordGraceNotesGroupLink msrChordGraceNotesGroupLink::createChordGraceNotesGroupLinkNewbornClone ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Creating a newborn clone of grace notes group link '" <<
      asShortString () <<
      "'" <<
      std::endl;
  }
#endif

/* JMI
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");
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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrChordGraceNotesGroupLink::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrChordGraceNotesGroupLink>*
    p =
      dynamic_cast<visitor<S_msrChordGraceNotesGroupLink>*> (v)) {
        S_msrChordGraceNotesGroupLink elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrChordGraceNotesGroupLink::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrChordGraceNotesGroupLink::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrChordGraceNotesGroupLink::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrChordGraceNotesGroupLink>*
    p =
      dynamic_cast<visitor<S_msrChordGraceNotesGroupLink>*> (v)) {
        S_msrChordGraceNotesGroupLink elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrChordGraceNotesGroupLink::visitEnd ()" <<
            std::endl;
        }
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
  std::stringstream s;

  s <<
    "[chordGraceNotesGroupLink" <<
    ", fOriginalGraceNotesGroup: " <<
    fOriginalGraceNotesGroup->asShortString () <<
    ", upLinkToChord \"" <<
    fUpLinkToChord->asShortString () <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

std::string msrChordGraceNotesGroupLink::asString () const
{
  std::stringstream s;

  s <<
    "[chordGraceNotesGroupLink" <<
    ", originalGraceNotesGroup \"" <<
    fOriginalGraceNotesGroup->asString () <<
    ", upLinkToChord \"" <<
    fUpLinkToChord->asString () <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
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
    "originalGraceNotesGroup" <<
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

void msrChordGraceNotesGroupLink::printShort (std::ostream& os) const
{
  os <<
    "[ChordGraceNotesGroupLink" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os <<
    std::setw (fieldWidth) <<
    "originalGraceNotesGroup" <<
    std::endl;

  ++gIndenter;
  fOriginalGraceNotesGroup->printShort (os);
  os <<
    fUpLinkToChord->asShortString () <<
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
    os << "[NONE]" << std::endl;
  }

  return os;
}


}
