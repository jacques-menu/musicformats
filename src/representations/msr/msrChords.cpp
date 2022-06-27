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

#include "msrChords.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrChord msrChord::create (
  int             inputLineNumber,
  const rational& chordSoundingWholeNotes,
  const rational& chordDisplayWholeNotes,
  msrDurationKind chordGraphicDurationKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Creating a chord" <<
      ", chordSoundingWholeNotes = " << chordSoundingWholeNotes <<
      ", chordDisplayWholeNotes = " << chordDisplayWholeNotes <<
      ", chordGraphicDuration = " <<
      msrDurationKindAsString (chordGraphicDurationKind) <<
      endl;
 }
#endif

  msrChord* o =
    new msrChord (
      inputLineNumber,
      chordSoundingWholeNotes, chordDisplayWholeNotes,
      chordGraphicDurationKind);
  assert (o != nullptr);

  return o;
}

msrChord::msrChord (
  int             inputLineNumber,
  const rational& chordSoundingWholeNotes,
  const rational& chordDisplayWholeNotes,
  msrDurationKind chordGraphicDurationKind)
    : msrTupletElement (inputLineNumber)
{
  fChordKind = msrChordInKind::k_NoChordIn;

// JMI  fChordSoundingWholeNotes = chordSoundingWholeNotes;
  fMeasureElementSoundingWholeNotes = chordSoundingWholeNotes;

  fChordDisplayWholeNotes  = chordDisplayWholeNotes;

  fChordGraphicDurationKind = chordGraphicDurationKind;

  fChordIsFirstChordInADoubleTremolo  = false;
  fChordIsSecondChordInADoubleTremolo = false;
}

msrChord::~msrChord ()
{}

S_msrChord msrChord::createChordNewbornClone (
  S_msrPart containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Creating a newborn clone of chord '" <<
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

  S_msrChord
    newbornClone =
      msrChord::create (
        fInputLineNumber,
  // JMI      fChordSoundingWholeNotes,
        fMeasureElementSoundingWholeNotes,
        fChordDisplayWholeNotes,
        fChordGraphicDurationKind);

/*
  newbornClone->
    fChordPositionInMeasure =
      fChordPositionInMeasure;

  newbornClone->
    fChordIsFirstChordInADoubleTremolo =
      fChordIsFirstChordInADoubleTremolo;

  newbornClone->
    fChordIsSecondChordInADoubleTremolo =
      fChordIsSecondChordInADoubleTremolo;
*/

  return newbornClone;
}

// measure upLink
S_msrMeasure msrChord::fetchChordMeasureUpLink () const
{
  S_msrMeasure result;

  switch (fChordKind) {
    case msrChordInKind::k_NoChordIn:
      break;

    case msrChordInKind::kChordInMeasure:
      result = fChordDirectMeasureUpLink;
      break;

    case msrChordInKind::kChordInTuplet:
      if (fChordDirectTupletUpLink) {
        result =
          fChordDirectTupletUpLink->
            fetchTupletMeasureUpLink ();
      }
      break;

    case msrChordInKind::kChordInGraceNotesGroup:
      if (fChordDirectGraceNotesGroupUpLink) {
        result =
          fChordDirectGraceNotesGroupUpLink->
            getGraceNotesGroupNoteUpLink ()->
              fetchNoteMeasureUpLink ();
      }
      break;
  } // switch

  return result;
}

// tuplet upLink
S_msrTuplet msrChord::fetchChordTupletUpLink () const
{
  S_msrTuplet result;

  switch (fChordKind) {
    case msrChordInKind::k_NoChordIn:
      break;

    case msrChordInKind::kChordInMeasure:
      break;

    case msrChordInKind::kChordInTuplet:
      result = fChordDirectTupletUpLink;
      break;

    case msrChordInKind::kChordInGraceNotesGroup:
      if (fChordDirectGraceNotesGroupUpLink) {
        result =
          fChordDirectGraceNotesGroupUpLink->
            getGraceNotesGroupNoteUpLink ()->
              getNoteDirectTupletUpLink ();
      }
      break;
  } // switch

  return result;
}

// grace notes group upLink
S_msrGraceNotesGroup msrChord::fetchChordGraceNotesGroupUpLink () const
{
  S_msrGraceNotesGroup result;

  switch (fChordKind) {
    case msrChordInKind::k_NoChordIn:
      break;
    case msrChordInKind::kChordInMeasure:
      break;
    case msrChordInKind::kChordInTuplet:
      break;
    case msrChordInKind::kChordInGraceNotesGroup:
      result = fChordDirectGraceNotesGroupUpLink; // JMI
      break;
  } // switch

  return result;
}

// score upLink
S_msrScore msrChord::fetchChordScoreUpLink () const
{
  S_msrScore result;

  S_msrMeasure
    measure =
      fetchChordMeasureUpLink ();

  if (measure) {
    result =
      measure ->
        fetchMeasureScoreUpLink ();
  }

  return result;
}

void msrChord::setChordSoundingWholeNotes (
   const rational& wholeNotes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Setting chord sounding whole notes to '" <<
      wholeNotes <<
      "' for chord '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

// JMI  fChordSoundingWholeNotes = wholeNotes;
  fMeasureElementSoundingWholeNotes = wholeNotes;
}

void msrChord::setChordDisplayWholeNotes (
   const rational& wholeNotes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Setting chord displayed whole notes to '" <<
      wholeNotes <<
      "' for chord '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fChordDisplayWholeNotes = wholeNotes;
}

void msrChord::setChordGraceNotesGroupLinkBefore (
  int     inputLineNumber,
  S_msrChordGraceNotesGroupLink
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
      endl;
  }
#endif

  fChordGraceNotesGroupLinkBefore =
    chordChordGraceNotesGroupLinkBefore;
}

void msrChord::setChordGraceNotesGroupLinkAfter (
  int     inputLineNumber,
  S_msrChordGraceNotesGroupLink
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
      endl;
  }
#endif

  fChordGraceNotesGroupLinkAfter =
    chordChordGraceNotesGroupLinkAfter;
}

void msrChord::setChordMembersPositionInMeasure (
  S_msrMeasure     measure,
   const rational& positionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting chord members position in measure of " << asString () <<
      " to '" <<
      positionInMeasure <<
      "'" <<
      endl;
  }
#endif

  string context =
    "setChordMembersPositionInMeasure()";

  setMeasureElementPositionInMeasure (
    positionInMeasure,
    context);

  if (false) { // JMI CAFE
  // compute chord's position in voice
  rational
     positionInVoice =
      fChordDirectMeasureUpLink->getMeasurePositionInVoice ()
        +
      positionInMeasure;

  // set chord's position in voice
  setMeasureElementPositionInVoice (
    positionInVoice,
    context);

  // update current position in voice
  S_msrVoice
    voice =
      measure->
        fetchMeasureVoiceUpLink ();

  voice->
    incrementCurrentPositionInVoice (
      fChordNotesVector [0]->getNoteSoundingWholeNotes ());
}

  // set the chord's elements' position in measure
  if (fChordNotesVector.size ()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotesVector.begin (),
      iEnd   = fChordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote
        note = (*i);

      // set note's measure uplink
      note->
        setNoteDirectMeasureUpLink (
          measure);

      // set note's position in measure
      note->
        setNotePositionInMeasure (
          positionInMeasure); // they all share the same one

//    JMI   set note's position in voice
//       note->
//         setMeasureElementPositionInVoice (
//           positionInVoice,
//           context); // they all share the same one

      if (++i == iEnd) break;
    } // for
  }

  // are there dal segnos attached to this chord?
  if (fChordDalSegnos.size ()) {
    list<S_msrDalSegno>::const_iterator i;
    for (
      i=fChordDalSegnos.begin (); i!=fChordDalSegnos.end (); ++i
    ) {
      // set the dal segno position in measure
      (*i)->
        setDalSegnoPositionInMeasure (
          positionInMeasure);
    } // for
  }
}

void msrChord::addFirstNoteToChord (
  S_msrNote  note,
  S_msrVoice voice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Adding first note '" <<
      note->asString () <<
      "' to chord '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // append note to chord notes
  fChordNotesVector.push_back (note);

  // register note's chord upLink
  note->
    setNoteDirectChordUpLink (this);

/* JMI too early
  // register note's measure upLink
  note->
    setNoteDirectMeasureUpLink (fChordDirectMeasureUpLink);
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
  S_msrNote  note,
  S_msrVoice voice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Adding another note '" <<
      note->asShortString () <<
      "' to chord '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fChordNotesVector.push_back (note);

  // register note's chord upLink
  note->
    setNoteDirectChordUpLink (this);

  // mark note as belonging to a chord
  note->setNoteBelongsToAChord ();

  // append the note to the measure's notes flat list
  if (false) // JMI
  fetchChordMeasureUpLink ()->
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

void msrChord::appendDalSegnoToChord (S_msrDalSegno dalSegno)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceDalSegnos ()) {
    gLogStream <<
      "Appending dal segno " <<
      dalSegno->asShortString () <<
      " to chord " <<
      asShortString () <<
      endl;
  }
#endif

  fChordDalSegnos.push_back (dalSegno);
}

void msrChord::appendArticulationToChord (S_msrArticulation art)
{
  msrArticulation::msrArticulationKind
    articulationKind =
      art->
        getArticulationKind ();

  // don't append the same articulation several times
  for (
    list<S_msrArticulation>::const_iterator i = fChordArticulations.begin ();
    i!=fChordArticulations.end ();
    ++i
  ) {
      if ((*i)->getArticulationKind () == articulationKind)
        return;
  } // for

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceArticulations ()) {
    gLogStream <<
      "Appending articulation '" <<
      art->articulationKindAsString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordArticulations.push_back (art);
}

void msrChord::appendSpannerToChord (S_msrSpanner span)
{
  msrSpanner::msrSpannerKind
    spannerKind =
      span->
        getSpannerKind ();

  // don't append the same spanner several times
  for (
    list<S_msrSpanner>::const_iterator i = fChordSpanners.begin ();
    i!=fChordSpanners.end ();
    ++i
  ) {
      if ((*i)->getSpannerKind () == spannerKind)
        return;
  } // for

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSpanners ()) {
    gLogStream <<
      "Appending spanner '" <<
      span->spannerKindAsString () <<
      "' to chord '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fChordSpanners.push_back (span);
}

void msrChord::setChordSingleTremolo (S_msrSingleTremolo trem)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTremolos ()) {
    gLogStream <<
      "Adding singleTremolo '" <<
      trem->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordSingleTremolo = trem;
}

void msrChord::appendTechnicalToChord (S_msrTechnical tech)
{
  msrTechnical::msrTechnicalKind
    technicalKind =
      tech->
        getTechnicalKind ();

  // don't append the same technical several times
  for (
    list<S_msrTechnical>::const_iterator i =
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
      endl;
  }
#endif

  fChordTechnicals.push_back (tech);
}

void msrChord::appendTechnicalWithIntegerToChord (
  S_msrTechnicalWithInteger tech)
{
  msrTechnicalWithInteger::msrTechnicalWithIntegerKind
    technicalWithIntegerKind =
      tech->
        getTechnicalWithIntegerKind ();

  // don't append the same technical several times
  for (
    list<S_msrTechnicalWithInteger>::const_iterator i =
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
      endl;
  }
#endif

  fChordTechnicalWithIntegers.push_back (tech);
}

void msrChord::appendTechnicalWithFloatToChord (
  S_msrTechnicalWithFloat tech)
{
  msrTechnicalWithFloat::msrTechnicalWithFloatKind
    technicalWithFloatKind =
      tech->
        getTechnicalWithFloatKind ();

  // don't append the same technical several times
  for (
    list<S_msrTechnicalWithFloat>::const_iterator i =
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
      endl;
  }
#endif

  fChordTechnicalWithFloats.push_back (tech);
}

void msrChord::appendTechnicalWithStringToChord (
  S_msrTechnicalWithString tech)
{
  msrTechnicalWithString::msrTechnicalWithStringKind
    technicalWithStringKind =
      tech->
        getTechnicalWithStringKind ();

  // don't append the same technical several times
  for (
    list<S_msrTechnicalWithString>::const_iterator i =
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
      "Appending technical with string '" <<
      tech->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordTechnicalWithStrings.push_back (tech);
}

void msrChord::appendOrnamentToChord (S_msrOrnament orn)
{
  msrOrnament::msrOrnamentKind
    ornamentKind =
      orn->
        getOrnamentKind ();

  // don't append the same ornament several times
  for (
    list<S_msrOrnament>::const_iterator i = fChordOrnaments.begin ();
    i!=fChordOrnaments.end ();
    ++i
  ) {
      if ((*i)->getOrnamentKind () == ornamentKind)
        return;
  } // for

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Appending ornament '" <<
      orn->ornamentKindAsString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordOrnaments.push_back (orn);
}

void msrChord::appendGlissandoToChord (S_msrGlissando gliss)
{
  msrGlissando::msrGlissandoTypeKind
    glissandoTypeKind =
      gliss->
        getGlissandoTypeKind ();

  // don't append the same slissando several times
  for (
    list<S_msrGlissando>::const_iterator i = fChordGlissandos.begin ();
    i!=fChordGlissandos.end ();
    ++i
  ) {
      if ((*i)->getGlissandoTypeKind () == glissandoTypeKind)
        return;
  } // for

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceChords ()) {
    gLogStream <<
      "Appending glissando '" <<
      gliss->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordGlissandos.push_back (gliss);
}

void msrChord::appendSlideToChord (S_msrSlide slide)
{
  msrSlide::msrSlideTypeKind
    slideTypeKind =
      slide->
        getSlideTypeKind ();

  // don't append the same slide several times
  for (
    list<S_msrSlide>::const_iterator i = fChordSlides.begin ();
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
      endl;
  }
#endif

  fChordSlides.push_back (slide);
}

void msrChord::appendChordSlurLinkToChord (S_msrChordSlurLink chordSlurLink)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceSlurs ()) {
    gLogStream <<
      "Adding slur link '" << chordSlurLink->asString() <<
      "' to chord '" << asString () << "'" <<
      endl;
  }
#endif

  fChordSlurLinks.push_back (chordSlurLink);
}

void msrChord::appendStemToChord (S_msrStem stem)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceStems ()) {
    gLogStream <<
      "Adding stem '" << stem->asString() <<
      "' to chord '" << asString () << "'" <<
      endl;
  }
#endif

  // sanity check
  for (
    list<S_msrStem>::const_iterator i = fChordStems.begin ();
    i != fChordStems.end ();
    ++i
  ) {
    // is stem's stem kind consistent with the other ones' for this chord?
    if (stem->getStemKind () != (*i)->getStemKind ()) {
      stringstream s;

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
void msrChord::appendBeamToChord (S_msrBeam beam)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeams ()) {
    gLogStream <<
      "Appending beam '" <<
      beam->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordBeams.push_back (beam);
}
*/

void msrChord::appendChordBeamLinkToChord (S_msrChordBeamLink chordBeamLink)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceBeams ()) {
    gLogStream <<
      "Adding beam link '" << chordBeamLink->asString() <<
      "' to chord '" << asString () << "'" <<
      endl;
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
      endl <<
      "fMeasureElementPositionInMeasure = " <<
      endl <<
      fMeasureElementPositionInMeasure <<
      endl;
  }
#endif

  // we can now set the position in measures for all the chord members
  setChordMembersPositionInMeasure (
    fChordDirectMeasureUpLink,
    fMeasureElementPositionInMeasure);
}

void msrChord::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrChord::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrChord::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrChord::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrChord::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrChord::visitEnd ()" <<
            endl;
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
      fetchChordScoreUpLink ();

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
          endl;
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
    vector<S_msrNote>::const_iterator i = fChordNotesVector.begin ();
    i != fChordNotesVector.end ();
    ++i
  ) {
    // browse chord note
    msrBrowser<msrNote> browser (v);
    browser.browse (*(*i));
  } // for


  for (
    list<S_msrStem>::const_iterator i = fChordStems.begin ();
    i != fChordStems.end ();
    ++i
  ) {
    // browse chord stems
    msrBrowser<msrStem> browser (v);
    browser.browse (*(*i));
  } // for

/* JMI
  for (
    list<S_msrBeam>::const_iterator i = fChordBeams.begin ();
    i != fChordBeams.end ();
    ++i
  ) {
    // browse chord beams
    msrBrowser<msrBeam> browser (v);
    browser.browse (*(*i));
  } // for
  */
  for (
    list<S_msrChordBeamLink>::const_iterator i = fChordBeamLinks.begin ();
    i != fChordBeamLinks.end ();
    ++i
  ) {
    // browse the beam link
    msrBrowser<msrChordBeamLink> browser (v);
    browser.browse (*(*i));
  } // for


  for (
    list<S_msrArticulation>::const_iterator i = fChordArticulations.begin ();
    i != fChordArticulations.end ();
    ++i
  ) {
    // browse the articulation
    msrBrowser<msrArticulation> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrSpanner>::const_iterator i = fChordSpanners.begin ();
    i != fChordSpanners.end ();
    ++i
  ) {
    // browse the spanner
    msrBrowser<msrSpanner> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrTechnical>::const_iterator i = fChordTechnicals.begin ();
    i != fChordTechnicals.end ();
    ++i
  ) {
    // browse the technical
    msrBrowser<msrTechnical> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrOrnament>::const_iterator i = fChordOrnaments.begin ();
    i != fChordOrnaments.end ();
    ++i
  ) {
    // browse the ornament
    msrBrowser<msrOrnament> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrGlissando>::const_iterator i = fChordGlissandos.begin ();
    i != fChordGlissandos.end ();
    ++i
  ) {
    // browse the glissando
    msrBrowser<msrGlissando> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrSlide>::const_iterator i = fChordSlides.begin ();
    i != fChordSlides.end ();
    ++i
  ) {
    // browse the slide
    msrBrowser<msrSlide> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrDynamic>::const_iterator i = fChordDynamics.begin ();
    i != fChordDynamics.end ();
    ++i
  ) {
    // browse the dynamic
    msrBrowser<msrDynamic> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrOtherDynamic>::const_iterator i = fChordOtherDynamics.begin ();
    i != fChordOtherDynamics.end ();
    ++i
  ) {
    // browse the other dynamic
    msrBrowser<msrOtherDynamic> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrWords>::const_iterator i = fChordWords.begin ();
    i != fChordWords.end ();
    ++i
  ) {
    // browse the words
    msrBrowser<msrWords> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrTie>::const_iterator i = fChordTies.begin ();
    i != fChordTies.end ();
    ++i
  ) {
    // browse the tie
    msrBrowser<msrTie> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrLigature>::const_iterator i = fChordLigatures.begin ();
    i != fChordLigatures.end ();
    ++i
  ) {
    // browse the ligature
    msrBrowser<msrLigature> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrSlash>::const_iterator i = fChordSlashes.begin ();
    i != fChordSlashes.end ();
    ++i
  ) {
    // browse the slash
    msrBrowser<msrSlash> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrCrescDecresc>::const_iterator i = fChordCrescDecrescs.begin ();
    i != fChordCrescDecrescs.end ();
    ++i
  ) {
    // browse the cresc/decresc
    msrBrowser<msrCrescDecresc> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrWedge>::const_iterator i = fChordWedges.begin ();
    i != fChordWedges.end ();
    ++i
  ) {
    // browse the wedge
    msrBrowser<msrWedge> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrSegno>::const_iterator i = fChordSegnos.begin ();
    i != fChordSegnos.end ();
    ++i
  ) {
    // browse the segno
    msrBrowser<msrSegno> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrDalSegno>::const_iterator i = fChordDalSegnos.begin ();
    i != fChordDalSegnos.end ();
    ++i
  ) {
    // browse the dal segno
    msrBrowser<msrDalSegno> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrCoda>::const_iterator i = fChordCodas.begin ();
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
    list<S_msrHarmony>::const_iterator i;
    for (i=fChordHarmoniesList.begin (); i!=fChordHarmoniesList.end (); ++i) {
      // browse the harmony
      msrBrowser<msrHarmony> browser (v);
      browser.browse (*(*i));
    } // for
  }

  if (fChordFiguredBass) {
    // browse the figured bass
    msrBrowser<msrFiguredBassElement> browser (v);
    browser.browse (*fChordFiguredBass);
  }

  for (
    list<S_msrChordSlurLink>::const_iterator i = fChordSlurLinks.begin ();
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
      actualNotes << "/" << normalNotes <<
      "' to the members of chord '" << asStringwithRawDivisions () <<
      "', line " << fInputLineNumber <<
      endl;

  for (
    vector<S_msrNote>::const_iterator i = fChordNotesVector.begin ();
    i != fChordNotesVector.end ();
    ++i) {
    (*i)->
      applyTupletMemberSoundingFactorToNote (
        actualNotes, normalNotes);
  } // for
}
#endif
*/

string msrChord::asStringwithRawDivisions () const
{
  stringstream s;

  s << "<";

  if (fChordNotesVector.size ()) {
    vector<S_msrNote>::const_iterator
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
        '[' << note->getNoteOctave () << "]"
        */

        note->asShortStringWithRawWholeNotes ();

      if (++i == iEnd) break;
      s << ' ';
    } // for
  }

  s << ">";

  return s.str ();
}

string msrChord::asString () const
{
  stringstream s;

  s <<
    "[Chord" <<
    ", chordKind: " << fChordKind <<
    ", line " << fInputLineNumber <<
    " <";

  if (fChordNotesVector.size ()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotesVector.begin (),
      iEnd   = fChordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote note = (*i);

      s <<
        note->notePitchAsString () <<
        ", whole notes: " <<
        " sounding " <<
        note->getNoteSoundingWholeNotes () <<
        ", displayed " <<
        note->getNoteDisplayWholeNotes () <<
        ", octave: " <<
        msrOctaveKindAsString (note->getNoteOctaveKind ()) <<
        "]";

      if (++i == iEnd) break;
      s << ' ';
    } // for
  }

  s << ">]";

  return s.str ();
}

string msrChord::asShortString () const
{
  stringstream s;

  s <<
    "[Chord" <<
    ", " << fChordKind <<
    ", line " << fInputLineNumber <<
    ", <";

  if (fChordNotesVector.size ()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotesVector.begin (),
      iEnd   = fChordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote
        note = (*i);

      s <<
        note->notePitchAsString () <<
        " snd: " <<
        note->getNoteSoundingWholeNotes () <<
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

void msrChord::print (ostream& os) const
{
  rational
    chordMeasureFullLength =
      fChordDirectMeasureUpLink
        ?
          fChordDirectMeasureUpLink->
            getFullMeasureWholeNotesDuration ()
        : rational (0, 1); // JMI

  os <<
    "[Chord" <<
    ", chordKind: " << fChordKind <<
    ", " <<
    mfSingularOrPlural (
      fChordNotesVector.size (), "note", "notes") <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 35;

  os << left <<
    setw (fieldWidth) <<
 // JMI   "chordSoundingWholeNotes" << " : " << fChordSoundingWholeNotes <<
    "chordSoundingWholeNotes" << " : " << fMeasureElementSoundingWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "fChordDisplayWholeNotes" << " : " << fChordDisplayWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "measureNumber" << " : " << fMeasureElementMeasureNumber <<
    endl <<
    setw (fieldWidth) <<
    "positionInMeasure" << " : " << fMeasureElementPositionInMeasure <<
    endl <<
    setw (fieldWidth) <<
    "positionInVoice" << " : " << fMeasureElementPositionInVoice <<
    endl <<
    setw (fieldWidth) <<
    "chordMeasureFullLength" << " : " << chordMeasureFullLength <<
    endl;

  os <<
    "fChordDirectTupletUpLink" << " : ";
  if (fChordDirectTupletUpLink) {
    os <<
      fChordDirectTupletUpLink->asShortString ();
  }
  else {
    os << "none";
  }
  os << endl;

  os <<
    setw (fieldWidth) <<
    "fPositionInTuplet" << " : " <<
    fPositionInTuplet <<
    endl;

  // print simplified position in measure if relevant
// JMI  if (fChordDirectMeasureUpLink) {
    // the chord measure upLink may not have been set yet
    rational
      chordPositionBis =
        fMeasureElementPositionInMeasure;
    chordPositionBis.rationalise ();

    if (
      chordPositionBis.getNumerator ()
        !=
      fMeasureElementPositionInMeasure.getNumerator ()
    ) {
      // print rationalized rational view
      os << left <<
        setw (fieldWidth) <<
        "chordPositionBis" << " : " << chordPositionBis <<
        endl;
    }

  os << left <<
    setw (fieldWidth) <<
    "fChordIsFirstChordInADoubleTremolo" << " : " <<
    fChordIsFirstChordInADoubleTremolo <<
    endl <<
    setw (fieldWidth) <<
    "fChordIsSecondChordInADoubleTremolo" << " : " <<
    fChordIsSecondChordInADoubleTremolo <<
    endl << endl;

  // print the chord grace notes groups links if any
  if (fChordGraceNotesGroupLinkBefore || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordGraceNotesGroupLinkBefore";

    if (fChordGraceNotesGroupLinkBefore) {
      os << endl;
      os << gTab << fChordGraceNotesGroupLinkBefore;
    }
    else {
      os << " : " << "none" << endl; // JMI TEST
    }
  }

  // print the articulations if any
  size_t chordArticulationsSize = fChordArticulations.size ();

  if (chordArticulationsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordArticulations";
    if (chordArticulationsSize) {
      os << endl;
      ++gIndenter;

      list<S_msrArticulation>::const_iterator i;
      for (i=fChordArticulations.begin (); i!=fChordArticulations.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the spanners if any
  size_t chordSpannersSize = fChordSpanners.size ();

  if (chordSpannersSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordSpanners";
    if (chordSpannersSize) {
      os << endl;
      ++gIndenter;

      list<S_msrSpanner>::const_iterator i;
      for (i=fChordSpanners.begin (); i!=fChordSpanners.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the technicals if any
  size_t chordTechnicalsSize = fChordTechnicals.size ();

  if (chordTechnicalsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordTechnicals";
    if (chordTechnicalsSize) {
      os << endl;
      ++gIndenter;

      list<S_msrTechnical>::const_iterator i;
      for (i=fChordTechnicals.begin (); i!=fChordTechnicals.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the ornaments if any
  size_t chordOrnamentsSize = fChordOrnaments.size ();

  if (chordOrnamentsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordOrnaments";
    if (chordOrnamentsSize) {
      os << endl;
      ++gIndenter;

      list<S_msrOrnament>::const_iterator i;
      for (i=fChordOrnaments.begin (); i!=fChordOrnaments.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the glissandos if any
  size_t chordGlissandosSize = fChordGlissandos.size ();

  if (chordGlissandosSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordGlissandos";
    if (chordGlissandosSize) {
      os << endl;
      ++gIndenter;

      list<S_msrGlissando>::const_iterator i;
      for (i=fChordGlissandos.begin (); i!=fChordGlissandos.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the slides if any
  size_t chordSlidesSize = fChordSlides.size ();

  if (chordSlidesSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordSlides";
    if (chordSlidesSize) {
      os << endl;
      ++gIndenter;

      list<S_msrSlide>::const_iterator i;
      for (i=fChordSlides.begin (); i!=fChordSlides.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the dynamics if any
  size_t chordDynamicsSize = fChordDynamics.size ();

  if (chordDynamicsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordDynamics";
    if (chordDynamicsSize) {
      os << endl;
      ++gIndenter;

      list<S_msrDynamic>::const_iterator i;
      for (i=fChordDynamics.begin (); i!=fChordDynamics.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the other dynamics if any
  size_t chordOtherDynamicsSize = fChordOtherDynamics.size ();

  if (chordOtherDynamicsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordOtherDynamics";
    if (chordOtherDynamicsSize) {
      os << endl;
      ++gIndenter;

      list<S_msrOtherDynamic>::const_iterator i;
      for (i=fChordOtherDynamics.begin (); i!=fChordOtherDynamics.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the stems if any
  size_t chordStemsSize = fChordStems.size ();

  if (chordStemsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordStems";
    if (chordStemsSize) {
      os << endl;
      ++gIndenter;

      list<S_msrStem>::const_iterator i;
      for (i=fChordStems.begin (); i!=fChordStems.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

/* JMI
  // print the beams if any
  size_t chordBeamsSize = fChordBeams.size ();

  if (chordBeamsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordBeams";
    if (chordBeamsSize) {
      os << endl;
      ++gIndenter;

      list<S_msrBeam>::const_iterator i;
      for (i=fChordBeams.begin (); i!=fChordBeams.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }
*/
#ifdef TRACING_IS_ENABLED
  // print the beam links if any
  size_t chordBeamLinksSize = fChordBeamLinks.size ();

  if (gGlobalTracingOahGroup->getTraceBeams () || chordBeamLinksSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "===> fChordBeamLinks ===>";
    if (chordBeamLinksSize) {
      os << endl;
      ++gIndenter;

      list<S_msrChordBeamLink>::const_iterator i;
      for (i=fChordBeamLinks.begin (); i!=fChordBeamLinks.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }
#endif

  // print the words if any
  size_t chordWordsSize = fChordWords.size ();

  if (chordWordsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordBeamLinks";
    if (chordWordsSize) {
      os << endl;
      ++gIndenter;

      list<S_msrWords>::const_iterator i;
      for (i=fChordWords.begin (); i!=fChordWords.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the ties if any
  size_t chordTiesSize = fChordTies.size ();

  if (chordTiesSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordTies";
    if (chordTiesSize) {
      os << endl;
      ++gIndenter;

      list<S_msrTie>::const_iterator i;
      for (i=fChordTies.begin (); i!=fChordTies.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the ligatures if any
  size_t chordLigaturesSize = fChordLigatures.size ();

  if (chordLigaturesSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordLigatures";
    if (chordLigaturesSize) {
      os << endl;
      ++gIndenter;

      list<S_msrLigature>::const_iterator i;
      for (i=fChordLigatures.begin (); i!=fChordLigatures.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the slashes if any
  size_t chordSlashesSize = fChordSlashes.size ();

  if (chordSlashesSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordSlashes";
    if (chordSlashesSize) {
      os << endl;
      ++gIndenter;

      list<S_msrSlash>::const_iterator i;
      for (i=fChordSlashes.begin (); i!=fChordSlashes.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the cresc/decresc if any
  size_t chordCrescDecrescsSize = fChordCrescDecrescs.size ();

  if (chordCrescDecrescsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordCrescDecrescs";
    if (chordCrescDecrescsSize) {
      os << endl;
      ++gIndenter;

      list<S_msrCrescDecresc>::const_iterator i;
      for (i=fChordCrescDecrescs.begin (); i!=fChordCrescDecrescs.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the wedges if any
  size_t chordWedgesSize = fChordWedges.size ();

  if (chordWedgesSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordWedges";
    if (chordWedgesSize) {
      os << endl;
      ++gIndenter;

      list<S_msrWedge>::const_iterator i;
      for (i=fChordWedges.begin (); i!=fChordWedges.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the segnos if any
  size_t chordSegnosSize = fChordSegnos.size ();

  if (chordSegnosSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordSegnos";
    if (chordSegnosSize) {
      os << endl;
      ++gIndenter;

      list<S_msrSegno>::const_iterator i;
      for (i=fChordSegnos.begin (); i!=fChordSegnos.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the dal segnos if any
  size_t chordDalSegnosSize = fChordDalSegnos.size ();

  if (chordDalSegnosSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordDalSegnos";
    if (chordDalSegnosSize) {
      os << endl;
      ++gIndenter;

      list<S_msrDalSegno>::const_iterator i;
      for (i=fChordDalSegnos.begin (); i!=fChordDalSegnos.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the codas if any
  size_t chordCodasSize = fChordCodas.size ();

  if (chordCodasSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordCodas";
    if (chordCodasSize) {
      os << endl;
      ++gIndenter;

      list<S_msrCoda>::const_iterator i;
      for (i=fChordCodas.begin (); i!=fChordCodas.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the octave shift if any
  if (fChordOctaveShift || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordOctaveShift";
    if (fChordOctaveShift) {
      os << endl;
      ++gIndenter;
        os << fChordOctaveShift;
      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the harmonies associated to this chord if any
  size_t chordHarmoniesListSize = fChordHarmoniesList.size ();

  if (chordHarmoniesListSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordHarmoniesList";
    if (chordHarmoniesListSize) {
      os << endl;
      ++gIndenter;

      list<S_msrHarmony>::const_iterator
        iBegin = fChordHarmoniesList.begin (),
        iEnd   = fChordHarmoniesList.end (),
        i      = iBegin;
      for ( ; ; ) {
        S_msrHarmony
          harmony = (*i);

        os << harmony->asString ();

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
  }

  // print the figured bass if any
  if (fChordFiguredBass || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordFiguredBass" << " : " <<
      endl;

    ++gIndenter;

    if (fChordFiguredBass) {
      os <<
        fChordFiguredBass->asString () <<
        endl;
    }
    else {
      os << "none";
    }

    --gIndenter;
  }
  os << endl;

  // print the chord notes if any
  size_t chordNotesVectorSize = fChordNotesVector.size ();

  if (chordNotesVectorSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordNotesVector";
    if (chordNotesVectorSize) {
      os << endl;
      ++gIndenter;

      vector<S_msrNote>::const_iterator
        iBegin = fChordNotesVector.begin (),
        iEnd   = fChordNotesVector.end (),
        i      = iBegin;

      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        os << endl;
      } // for

      --gIndenter;
    }
    else {
      os << ":" << "none" <<
      endl;
    }

    os << endl;
  }

#ifdef TRACING_IS_ENABLED
  // print the slur links if any
  size_t chordSlurLinksSize = fChordSlurLinks.size ();

  if (gGlobalTracingOahGroup->getTraceSlurs () || chordSlurLinksSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "===> fChordSlurLinks ===>";
    if (chordSlurLinksSize) {
      os << endl;
      ++gIndenter;

      list<S_msrChordSlurLink>::const_iterator i;
      for (i=fChordSlurLinks.begin (); i!=fChordSlurLinks.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }
#endif

/*
  // print the chord grace notes groups if any
  if (fChordGraceNotesGroupBefore || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordGraceNotesGroupBefore";

    if (fChordGraceNotesGroupBefore) {
      os << endl;
      ++gIndenter;
      os <<
        fChordGraceNotesGroupBefore->asString () <<
        endl;
      --gIndenter;
    }
    else {
      os << " : " << "none" << endl; // JMI TEST
    }
//    os << endl;
  }

  if (fChordGraceNotesGroupAfter || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordGraceNotesGroupAfter";

    if (fChordGraceNotesGroupAfter) {
      os << endl;
      ++gIndenter;
      os <<
        fChordGraceNotesGroupAfter->asString ();
      --gIndenter;
    }
    else {
      os << " : " << "none";
    }
    os << endl;
  }
*/

  if (fChordGraceNotesGroupLinkAfter || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fChordGraceNotesGroupLinkAfter";

    if (fChordGraceNotesGroupLinkAfter) {
      os << endl;
      os << gTab << fChordGraceNotesGroupLinkAfter;
    }
    else {
      os << " : " << "none";
    }
    os << endl;
  }

  --gIndenter;

  os << ']' << endl;
}

void msrChord::printShort (ostream& os) const
{
  rational
    chordMeasureFullLength =
      fChordDirectMeasureUpLink
        ?
          fChordDirectMeasureUpLink->
            getFullMeasureWholeNotesDuration ()
        : rational (0, 1); // JMI

  os <<
    "[Chord" <<
    ", line " << fInputLineNumber <<
    ", " <<
    mfSingularOrPlural (
      fChordNotesVector.size (), "note", "notes") <<
    endl;

  ++gIndenter;

  const int fieldWidth = 35;

  os << left <<
    setw (fieldWidth) <<
 // JMI   "chordSoundingWholeNotes" << " : " << fChordSoundingWholeNotes <<
    "chordSoundingWholeNotes" << " : " << fMeasureElementSoundingWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "fChordDisplayWholeNotes" << " : " << fChordDisplayWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "measureNumber" << " : " << fMeasureElementMeasureNumber <<
    endl <<
    setw (fieldWidth) <<
    "positionInMeasure" << " : " << fMeasureElementPositionInMeasure <<
    endl <<
    setw (fieldWidth) <<
    "positionInVoice" << " : " << fMeasureElementPositionInVoice <<
    endl <<
    setw (fieldWidth) <<
    "chordMeasureFullLength" << " : " << chordMeasureFullLength <<
    endl;

  os <<
    setw (fieldWidth) <<
    "positionInTuplet" << " : " <<
    fPositionInTuplet <<
    endl;

  // print the chord grace notes group link before if any
  if (fChordGraceNotesGroupLinkBefore) {
    os <<
      setw (fieldWidth) <<
      "===> fChordGraceNotesGroupLinkBefore ===>";

    if (fChordGraceNotesGroupLinkBefore) {
      os << endl;
      os << gTab << fChordGraceNotesGroupLinkBefore->asShortString ();
    }
    else {
      os << " : " << "none";
    }
    os << endl;
  }

/*
  // print the articulations if any
  size_t chordArticulationsSize = fChordArticulations.size ();

  if (chordArticulationsSize) {
    os <<
      setw (fieldWidth) <<
      "fChordArticulations";
    if (chordArticulationsSize) {
      os << endl;
      ++gIndenter;

      list<S_msrArticulation>::const_iterator i;
      for (i=fChordArticulations.begin (); i!=fChordArticulations.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the spanners if any
  size_t chordSpannersSize = fChordSpanners.size ();

  if (chordSpannersSize) {
    os <<
      setw (fieldWidth) <<
      "fChordSpanners";
    if (chordSpannersSize) {
      os << endl;
      ++gIndenter;

      list<S_msrSpanner>::const_iterator i;
      for (i=fChordSpanners.begin (); i!=fChordSpanners.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the technicals if any
  size_t chordTechnicalsSize = fChordTechnicals.size ();

  if (chordTechnicalsSize) {
    os <<
      setw (fieldWidth) <<
      "fChordTechnicals";
    if (chordTechnicalsSize) {
      os << endl;
      ++gIndenter;

      list<S_msrTechnical>::const_iterator i;
      for (i=fChordTechnicals.begin (); i!=fChordTechnicals.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the ornaments if any
  size_t chordOrnamentsSize = fChordOrnaments.size ();

  if (chordOrnamentsSize) {
    os <<
      setw (fieldWidth) <<
      "fChordOrnaments";
    if (chordOrnamentsSize) {
      os << endl;
      ++gIndenter;

      list<S_msrOrnament>::const_iterator i;
      for (i=fChordOrnaments.begin (); i!=fChordOrnaments.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the glissandos if any
  size_t chordGlissandosSize = fChordGlissandos.size ();

  if (chordGlissandosSize) {
    os <<
      setw (fieldWidth) <<
      "fChordGlissandos";
    if (chordGlissandosSize) {
      os << endl;
      ++gIndenter;

      list<S_msrGlissando>::const_iterator i;
      for (i=fChordGlissandos.begin (); i!=fChordGlissandos.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the slides if any
  size_t chordSlidesSize = fChordSlides.size ();

  if (chordSlidesSize) {
    os <<
      setw (fieldWidth) <<
      "fChordSlides";
    if (chordSlidesSize) {
      os << endl;
      ++gIndenter;

      list<S_msrSlide>::const_iterator i;
      for (i=fChordSlides.begin (); i!=fChordSlides.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the dynamics if any
  size_t chordDynamicsSize = fChordDynamics.size ();

  if (chordDynamicsSize) {
    os <<
      setw (fieldWidth) <<
      "fChordDynamics";
    if (chordDynamicsSize) {
      os << endl;
      ++gIndenter;

      list<S_msrDynamic>::const_iterator i;
      for (i=fChordDynamics.begin (); i!=fChordDynamics.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the other dynamics if any
  size_t chordOtherDynamicsSize = fChordOtherDynamics.size ();

  if (chordOtherDynamicsSize) {
    os <<
      setw (fieldWidth) <<
      "fChordOtherDynamics";
    if (chordOtherDynamicsSize) {
      os << endl;
      ++gIndenter;

      list<S_msrOtherDynamic>::const_iterator i;
      for (i=fChordOtherDynamics.begin (); i!=fChordOtherDynamics.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the stems if any
  size_t chordStemsSize = fChordStems.size ();

  if (chordStemsSize) {
    os <<
      setw (fieldWidth) <<
      "fChordStems";
    if (chordStemsSize) {
      os << endl;
      ++gIndenter;

      list<S_msrStem>::const_iterator i;
      for (i=fChordStems.begin (); i!=fChordStems.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }
*/

/*
  // print the beams if any
  size_t chordBeamsSize = fChordBeams.size ();

  if (chordBeamsSize) {
    os <<
      setw (fieldWidth) <<
      "fChordBeams";
    if (chordBeamsSize) {
      os << endl;
      ++gIndenter;

      list<S_msrBeam>::const_iterator i;
      for (i=fChordBeams.begin (); i!=fChordBeams.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }
*/

#ifdef TRACING_IS_ENABLED
  // print the beam links if any
  size_t chordBeamLinksSize = fChordBeamLinks.size ();

  if (gGlobalTracingOahGroup->getTraceBeams () || chordBeamLinksSize) {
    os <<
      setw (fieldWidth) <<
      "fChordBeamLinks";
    if (chordBeamLinksSize) {
      os << endl;
      ++gIndenter;

      list<S_msrChordBeamLink>::const_iterator i;
      for (i=fChordBeamLinks.begin (); i!=fChordBeamLinks.end (); ++i) {
        (*i)->printShort(os);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }
#endif

  // print the words if any
  size_t chordWordsSize = fChordWords.size ();

  if (chordWordsSize) {
    os <<
      setw (fieldWidth) <<
      "fChordWords";
    if (chordWordsSize) {
      os << endl;
      ++gIndenter;

      list<S_msrWords>::const_iterator i;
      for (i=fChordWords.begin (); i!=fChordWords.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the ties if any
  size_t chordTiesSize = fChordTies.size ();

  if (chordTiesSize) {
    os <<
      setw (fieldWidth) <<
      "fChordTies";
    if (chordTiesSize) {
      os << endl;
      ++gIndenter;

      list<S_msrTie>::const_iterator i;
      for (i=fChordTies.begin (); i!=fChordTies.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

#ifdef TRACING_IS_ENABLED
  // print the slur links if any
  size_t chordSlurLinksSize = fChordSlurLinks.size ();

  if (gGlobalTracingOahGroup->getTraceSlurs () || chordSlurLinksSize) {
    os <<
      setw (fieldWidth) <<
      "fChordSlurLinks";
    if (chordSlurLinksSize) {
      os << endl;
      ++gIndenter;

      list<S_msrChordSlurLink>::const_iterator i;
      for (i=fChordSlurLinks.begin (); i!=fChordSlurLinks.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }
#endif

  // print the ligatures if any
  size_t chordLigaturesSize = fChordLigatures.size ();

  if (chordLigaturesSize) {
    os <<
      setw (fieldWidth) <<
      "fChordLigatures";
    if (chordLigaturesSize) {
      os << endl;
      ++gIndenter;

      list<S_msrLigature>::const_iterator i;
      for (i=fChordLigatures.begin (); i!=fChordLigatures.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

/*
  // print the slashes if any
  size_t chordSlashesSize = fChordSlashes.size ();

  if (chordSlashesSize) {
    os <<
      setw (fieldWidth) <<
      "fChordSlashes";
    if (chordSlashesSize) {
      os << endl;
      ++gIndenter;

      list<S_msrSlash>::const_iterator i;
      for (i=fChordSlashes.begin (); i!=fChordSlashes.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the cresc/descrec if any
  size_t chordCrescDescrecsSize = fChordCrescDescrecs.size ();

  if (chordCrescDescrecsSize) {
    os <<
      setw (fieldWidth) <<
      "fChordCrescDescrecs";
    if (chordCrescDescrecsSize) {
      os << endl;
      ++gIndenter;

      list<S_msrCrescDescrec>::const_iterator i;
      for (i=fChordCrescDescrecs.begin (); i!=fChordCrescDescrecs.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the wedges if any
  size_t chordWedgesSize = fChordWedges.size ();

  if (chordWedgesSize) {
    os <<
      setw (fieldWidth) <<
      "fChordWedges";
    if (chordWedgesSize) {
      os << endl;
      ++gIndenter;

      list<S_msrWedge>::const_iterator i;
      for (i=fChordWedges.begin (); i!=fChordWedges.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the segnos if any
  size_t chordSegnosSize = fChordSegnos.size ();

  if (chordSegnosSize) {
    os <<
      setw (fieldWidth) <<
      "fChordSegnos";
    if (chordSegnosSize) {
      os << endl;
      ++gIndenter;

      list<S_msrSegno>::const_iterator i;
      for (i=fChordSegnos.begin (); i!=fChordSegnos.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the dal segnos if any
  size_t chordDalSegnosSize = fChordDalSegnos.size ();

  if (chordDalSegnosSize) {
    os <<
      setw (fieldWidth) <<
      "fChordDalSegnos";
    if (chordDalSegnosSize) {
      os << endl;
      ++gIndenter;

      list<S_msrDalSegno>::const_iterator i;
      for (i=fChordDalSegnos.begin (); i!=fChordDalSegnos.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the codas if any
  size_t chordCodasSize = fChordCodas.size ();

  if (chordCodasSize) {
    os <<
      setw (fieldWidth) <<
      "fChordCodas";
    if (chordCodasSize) {
      os << endl;
      ++gIndenter;

      list<S_msrCoda>::const_iterator i;
      for (i=fChordCodas.begin (); i!=fChordCodas.end (); ++i) {
        os << (*i);
      } // for

      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the octave shift if any
  if (fChordOctaveShift) {
    os <<
      setw (fieldWidth) <<
      "fChordOctaveShift";
    if (fChordOctaveShift) {
      os << endl;
      ++gIndenter;
        os << fChordOctaveShift;
      --gIndenter;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }
*/

  // print the harmonies associated to this chord if any
  size_t chordHarmoniesListSize = fChordHarmoniesList.size ();

  if (chordHarmoniesListSize) {
    os <<
      setw (fieldWidth) <<
      "fChordHarmoniesList";
    if (chordHarmoniesListSize) {
      os << endl;
      ++gIndenter;

      list<S_msrHarmony>::const_iterator
        iBegin = fChordHarmoniesList.begin (),
        iEnd   = fChordHarmoniesList.end (),
        i      = iBegin;
      for ( ; ; ) {
        S_msrHarmony
          harmony = (*i);

        os << harmony->asString ();

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
  }

  // print the figured bass if any
  if (fChordFiguredBass) {
    os <<
      setw (fieldWidth) <<
      "fChordFiguredBass" << " : " <<
      endl;

    ++gIndenter;

    if (fChordFiguredBass) {
      os <<
        fChordFiguredBass->asString () <<
        endl;
    }
    else {
      os <<
        "none";
    }

    --gIndenter;
  }
  os << endl;

  // print the chord notes if any
  size_t chordNotesVectorSize = fChordNotesVector.size ();

  if (chordNotesVectorSize) {
    os <<
      setw (fieldWidth) <<
      "fChordNotesVector";
    if (chordNotesVectorSize) {
      os << endl;
      ++gIndenter;

      vector<S_msrNote>::const_iterator
        iBegin = fChordNotesVector.begin (),
        iEnd   = fChordNotesVector.end (),
        i      = iBegin;

      for ( ; ; ) {
        (*i)->printShort (os);
        if (++i == iEnd) break;
        os << endl;
      } // for

      --gIndenter;
    }
    else {
      os << ":" << "none" <<
      endl;
    }

    os << endl;
  }

/*
  // print the chord grace notes groups if any
  if (fChordGraceNotesGroupBefore) {
    os <<
      setw (fieldWidth) <<
      "fChordGraceNotesGroupBefore";

    if (fChordGraceNotesGroupBefore) {
      os << endl;
      ++gIndenter;
      fChordGraceNotesGroupBefore->printShort (os);
      --gIndenter;
    }
    else {
      os << " : " << "none" << endl; // JMI TEST
    }
//    os << endl;
  }

  if (fChordGraceNotesGroupAfter) {
    os <<
      setw (fieldWidth) <<
      "fChordGraceNotesGroupAfter";

    if (fChordGraceNotesGroupAfter) {
      os << endl;
      ++gIndenter;
      fChordGraceNotesGroupAfter->printShort (os);
      --gIndenter;
    }
    else {
      os << " : " << "none";
    }
    os << endl;
  }
*/

  // print the chord grace notes group link after if any
  if (fChordGraceNotesGroupLinkAfter) {
    os <<
      setw (fieldWidth) <<
      "===> fChordGraceNotesGroupLinkAfter ===>";

    if (fChordGraceNotesGroupLinkAfter) {
      os << endl;
      os << gTab << fChordGraceNotesGroupLinkAfter->asShortString ();
    }
    else {
      os << " : " << "none";
    }
    os << endl;
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrChord& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}

//______________________________________________________________________________
S_msrChordBeamLink msrChordBeamLink::create (
  int        inputLineNumber,
  S_msrBeam  originalBeam,
  S_msrChord chordUpLink)
{
  msrChordBeamLink* o =
    new msrChordBeamLink (
      inputLineNumber,
      originalBeam,
      chordUpLink);
  assert (o != nullptr);

  return o;
}

msrChordBeamLink::msrChordBeamLink (
  int        inputLineNumber,
  S_msrBeam  originalBeam,
  S_msrChord chordUpLink)
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
    chordUpLink != nullptr,
    "chordUpLink is null");

  fChordUpLink = chordUpLink;

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

  S_msrChordBeamLink
    newbornClone =
      msrChordBeamLink::create (
        fInputLineNumber,
        fOriginalBeam,
        fChordUpLink);

  return newbornClone;
}

void msrChordBeamLink::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrChordBeamLink::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrChordBeamLink>*
    p =
      dynamic_cast<visitor<S_msrChordBeamLink>*> (v)) {
        S_msrChordBeamLink elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrChordBeamLink::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrChordBeamLink::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrChordBeamLink::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrChordBeamLink>*
    p =
      dynamic_cast<visitor<S_msrChordBeamLink>*> (v)) {
        S_msrChordBeamLink elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrChordBeamLink::visitEnd ()" <<
            endl;
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

string msrChordBeamLink::asShortString () const
{
  stringstream s;

  s <<
    "[ChordBeamLink" <<
    ", originalBeam \"" <<
    fOriginalBeam->asShortString () <<
    ", chordUpLink \"" <<
    fChordUpLink->asShortString () <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

string msrChordBeamLink::asString () const
{
  stringstream s;

  s <<
    "[ChordBeamLink" <<
    ", originalBeam \"" <<
    fOriginalBeam->asString () <<
    ", chordUpLink \"" <<
    fChordUpLink->asString () <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrChordBeamLink::print (ostream& os) const
{
  os <<
    "[ChordBeamLink" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os <<
    setw (fieldWidth) <<
    "originalBeam:" <<
    endl;

  ++gIndenter;
  os <<
    fOriginalBeam; // <<
    fChordUpLink->asString ();
  --gIndenter;

  --gIndenter;

  os << ']' << endl;
}

void msrChordBeamLink::printShort (ostream& os) const
{
  os <<
    "[ChordBeamLink" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os <<
    setw (fieldWidth) <<
    "originalBeam:" <<
    endl;

  ++gIndenter;
  fOriginalBeam->printShort (os);
  os <<
    fChordUpLink->asShortString () <<
    endl;
  --gIndenter;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrChordBeamLink& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}

//______________________________________________________________________________
S_msrChordSlurLink msrChordSlurLink::create (
  int        inputLineNumber,
  S_msrSlur  originalSlur,
  S_msrChord chordUpLink)
{
  msrChordSlurLink* o =
    new msrChordSlurLink (
      inputLineNumber,
      originalSlur,
      chordUpLink);
  assert (o != nullptr);

  return o;
}

msrChordSlurLink::msrChordSlurLink (
  int        inputLineNumber,
  S_msrSlur  originalSlur,
  S_msrChord chordUpLink)
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
    chordUpLink != nullptr,
    "chordUpLink is null");

  fChordUpLink = chordUpLink;

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

  S_msrChordSlurLink
    newbornClone =
      msrChordSlurLink::create (
        fInputLineNumber,
        fOriginalSlur,
        fChordUpLink);

  return newbornClone;
}

void msrChordSlurLink::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrChordSlurLink::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrChordSlurLink>*
    p =
      dynamic_cast<visitor<S_msrChordSlurLink>*> (v)) {
        S_msrChordSlurLink elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrChordSlurLink::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrChordSlurLink::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrChordSlurLink::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrChordSlurLink>*
    p =
      dynamic_cast<visitor<S_msrChordSlurLink>*> (v)) {
        S_msrChordSlurLink elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrChordSlurLink::visitEnd ()" <<
            endl;
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

string msrChordSlurLink::asShortString () const
{
  stringstream s;

  s <<
    "[ChordSlurLink" <<
    ", originalSlur \"" <<
    fOriginalSlur->asShortString () <<
    ", chordUpLink \"" <<
    fChordUpLink->asShortString () <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

string msrChordSlurLink::asString () const
{
  stringstream s;

  s <<
    "[ChordSlurLink" <<
    ", originalSlur \"" <<
    fOriginalSlur->asString () <<
    ", chordUpLink \"" <<
    fChordUpLink->asString () <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrChordSlurLink::print (ostream& os) const
{
  os <<
    "[ChordSlurLink" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os <<
    setw (fieldWidth) <<
    "originalSlur:" <<
    endl;

  ++gIndenter;
  os <<
    fOriginalSlur <<
    fChordUpLink->asString ();
  --gIndenter;

  --gIndenter;

  os << ']' << endl;
}

void msrChordSlurLink::printShort (ostream& os) const
{
  os <<
    "[ChordSlurLink" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os <<
    setw (fieldWidth) <<
    "originalSlur:" <<
    endl;

  ++gIndenter;
  fOriginalSlur->printShort (os);
  os <<
    fChordUpLink->asShortString () <<
    endl;
  --gIndenter;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrChordSlurLink& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}

//______________________________________________________________________________
S_msrChordGraceNotesGroupLink msrChordGraceNotesGroupLink::create (
  int                  inputLineNumber,
  S_msrGraceNotesGroup originalGraceNotesGroup,
  S_msrChord           chordUpLink)
{
  msrChordGraceNotesGroupLink* o =
    new msrChordGraceNotesGroupLink (
      inputLineNumber,
      originalGraceNotesGroup,
      chordUpLink);
  assert (o != nullptr);

  return o;
}

msrChordGraceNotesGroupLink::msrChordGraceNotesGroupLink (
  int                  inputLineNumber,
  S_msrGraceNotesGroup originalGraceNotesGroup,
  S_msrChord           chordUpLink)
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
    chordUpLink != nullptr,
    "chordUpLink is null");

  fChordUpLink = chordUpLink;

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

  S_msrChordGraceNotesGroupLink
    newbornClone =
      msrChordGraceNotesGroupLink::create (
        fInputLineNumber,
        fOriginalGraceNotesGroup,
        fChordUpLink);

  return newbornClone;
}

void msrChordGraceNotesGroupLink::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrChordGraceNotesGroupLink::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrChordGraceNotesGroupLink>*
    p =
      dynamic_cast<visitor<S_msrChordGraceNotesGroupLink>*> (v)) {
        S_msrChordGraceNotesGroupLink elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrChordGraceNotesGroupLink::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrChordGraceNotesGroupLink::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrChordGraceNotesGroupLink::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrChordGraceNotesGroupLink>*
    p =
      dynamic_cast<visitor<S_msrChordGraceNotesGroupLink>*> (v)) {
        S_msrChordGraceNotesGroupLink elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrChordGraceNotesGroupLink::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrChordGraceNotesGroupLink::browseData (basevisitor* v)
{
  list<S_msrMeasureElement>::const_iterator i;

  // browse the original grace notes group
  msrBrowser<msrGraceNotesGroup> browser (v);
  browser.browse (*fOriginalGraceNotesGroup);
}

string msrChordGraceNotesGroupLink::asShortString () const
{
  stringstream s;

  s <<
    "[chordGraceNotesGroupLink" <<
    ", fOriginalGraceNotesGroup: " <<
    fOriginalGraceNotesGroup->asShortString () <<
    ", chordUpLink \"" <<
    fChordUpLink->asShortString () <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

string msrChordGraceNotesGroupLink::asString () const
{
  stringstream s;

  s <<
    "[chordGraceNotesGroupLink" <<
    ", originalGraceNotesGroup \"" <<
    fOriginalGraceNotesGroup->asString () <<
    ", chordUpLink \"" <<
    fChordUpLink->asString () <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrChordGraceNotesGroupLink::print (ostream& os) const
{
  os <<
    "[ChordGraceNotesGroupLink" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os <<
    setw (fieldWidth) <<
    "originalGraceNotesGroup" <<
    endl;

  ++gIndenter;
  os <<
    fOriginalGraceNotesGroup <<
    fChordUpLink->asString () <<
    endl;
  --gIndenter;

  --gIndenter;

  os << ']' << endl;
}

void msrChordGraceNotesGroupLink::printShort (ostream& os) const
{
  os <<
    "[ChordGraceNotesGroupLink" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os <<
    setw (fieldWidth) <<
    "originalGraceNotesGroup" <<
    endl;

  ++gIndenter;
  fOriginalGraceNotesGroup->printShort (os);
  os <<
    fChordUpLink->asShortString () <<
    endl;
  --gIndenter;

  --gIndenter;

  os << ']' << endl;
}

ostream& operator<< (ostream& os, const S_msrChordGraceNotesGroupLink& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}


}
