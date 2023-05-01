/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "msrWae.h"

#include "mfStaticSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrMeasureConstants.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrTuplet msrTuplet::create (
  int                     inputLineNumber,
  const S_msrMeasure&     upLinkToMeasure,
  const std::string&      tupletMeasureNumber,
  int                     tupletNumber,
  msrTupletBracketKind    tupletBracketKind,
  msrTupletLineShapeKind  tupletLineShapeKind,
  msrTupletShowNumberKind tupletShowNumberKind,
  msrTupletShowTypeKind   tupletShowTypeKind,
  const msrTupletFactor&  tupletFactor,
  const msrWholeNotes&    memberNotesSoundingWholeNotes,
  const msrWholeNotes&    memberNotesDisplayWholeNotes)
{
  msrTuplet* obj =
    new msrTuplet (
      inputLineNumber,
      upLinkToMeasure,
      tupletMeasureNumber,
      tupletNumber,
      tupletBracketKind,
      tupletLineShapeKind,
      tupletShowNumberKind,
      tupletShowTypeKind,
      tupletFactor,
      memberNotesSoundingWholeNotes,
      memberNotesDisplayWholeNotes);
  assert (obj != nullptr);
  return obj;
}

S_msrTuplet msrTuplet::create (
  int                     inputLineNumber,
  const std::string&      tupletMeasureNumber,
  int                     tupletNumber,
  msrTupletBracketKind    tupletBracketKind,
  msrTupletLineShapeKind  tupletLineShapeKind,
  msrTupletShowNumberKind tupletShowNumberKind,
  msrTupletShowTypeKind   tupletShowTypeKind,
  const msrTupletFactor&  tupletFactor,
  const msrWholeNotes&    memberNotesSoundingWholeNotes,
  const msrWholeNotes&    memberNotesDisplayWholeNotes)
{
  return
    msrTuplet::create (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      tupletMeasureNumber,
      tupletNumber,
      tupletBracketKind,
      tupletLineShapeKind,
      tupletShowNumberKind,
      tupletShowTypeKind,
      tupletFactor,
      memberNotesSoundingWholeNotes,
      memberNotesDisplayWholeNotes);
}

msrTuplet::msrTuplet (
  int                     inputLineNumber,
  const S_msrMeasure&     upLinkToMeasure,
  const std::string&      tupletMeasureNumber,
  int                     tupletNumber,
  msrTupletBracketKind    tupletBracketKind,
  msrTupletLineShapeKind  tupletLineShapeKind,
  msrTupletShowNumberKind tupletShowNumberKind,
  msrTupletShowTypeKind   tupletShowTypeKind,
  const msrTupletFactor&  tupletFactor,
  const msrWholeNotes&    memberNotesSoundingWholeNotes,
  const msrWholeNotes&    memberNotesDisplayWholeNotes)
    : msrTupletElement (
        inputLineNumber)
{
  fTupletUpLinkToMeasure = upLinkToMeasure;

  fTupletKind = msrTupletInKind::kTupletIn_UNKNOWN_;

  fTupletNumber = tupletNumber;

  fTupletBracketKind    = tupletBracketKind;
  fTupletLineShapeKind  = tupletLineShapeKind;
  fTupletShowNumberKind = tupletShowNumberKind;
  fTupletShowTypeKind   = tupletShowTypeKind;

  fTupletFactor = tupletFactor;

  fMemberNotesSoundingWholeNotes = memberNotesSoundingWholeNotes;
  fMemberNotesDisplayWholeNotes  = memberNotesDisplayWholeNotes;

  doSetSoundingWholeNotes (
    msrWholeNotes (0, 1),
    "msrTuplet::msrTuplet()");

  fTupletDisplayWholeNotes = msrWholeNotes (0, 1);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Creating tuplet " <<
      asString ();

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrTuplet::~msrTuplet ()
{}

S_msrTuplet msrTuplet::createTupletNewbornClone ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of tuplet " <<
      asString ();

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrTuplet
    newbornClone =
      msrTuplet::create (
        fInputLineNumber,
        gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
        fTupletUpLinkToMeasure->getMeasureNumber (),
        fTupletNumber,
        fTupletBracketKind,
        fTupletLineShapeKind,
        fTupletShowNumberKind,
        fTupletShowTypeKind,
        fTupletFactor,
        fMemberNotesSoundingWholeNotes,
        fMemberNotesDisplayWholeNotes);

/* JMI ???
  newbornClone->fSoundingWholeNotes =
    fSoundingWholeNotes;

  newbornClone->fTupletDisplayWholeNotes =
    fTupletDisplayWholeNotes;

  newbornClone->fTupletUpLinkToMeasure->getMeasureNumber () =
    fTupletUpLinkToMeasure->getMeasureNumber ();

  newbornClone->fMeasurePosition =
    fMeasurePosition;
*/

  return newbornClone;
}

// S_msrMeasure msrTuplet::fetchMeasureElementUpLinkToMeasure () const
// {
//   S_msrMeasure result;
//
//   switch (fTupletKind) {
//     case msrTupletInKind::kTupletIn_UNKNOWN_:
//       break;
//
//     case msrTupletInKind::kTupletInMeasure:
//       result = fTupletUpLinkToMeasure;
//       break;
//
//     case msrTupletInKind::kTupletInTuplet:
//       if (fTupletShortcutUpLinkToTuplet) {
//         result =
//           fTupletShortcutUpLinkToTuplet->
//             fetchMeasureElementUpLinkToMeasure ();
//       }
//       break;
//   } // switch
//
//   return result;
// }

// S_msrTuplet msrTuplet::fetchTupletUpLinkToTuplet () const
// {
//   S_msrTuplet result;
//
//   switch (fTupletKind) {
//     case msrTupletInKind::kTupletIn_UNKNOWN_:
//       break;
//
//     case msrTupletInKind::kTupletInMeasure:
//       break;
//
//     case msrTupletInKind::kTupletInTuplet:
//       result = fTupletShortcutUpLinkToTuplet;
//       break;
//   } // switch
//
//   return result;
// }

void msrTuplet::setMeasureElementUpLinkToMeasure (
  const S_msrMeasure& measure)
{
  setTupletUpLinkToMeasure (measure);
}

void msrTuplet::getMeasureElementUpLinkToMeasure (
  S_msrMeasure& upLinkToMeasure) const
{
  upLinkToMeasure = getTupletUpLinkToMeasure ();
}

void msrTuplet::setTupletUpLinkToMeasure (
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
  if (gTraceOahGroup->getTraceTuplets ()) {
    ++gIndenter;

    std::stringstream ss;

    ss <<
      "Setting the uplink to measure of tuplet " <<
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

  fTupletUpLinkToMeasure = measure;
}

S_msrMeasure msrTuplet::getTupletUpLinkToMeasure () const
{
  return fTupletUpLinkToMeasure;
}

//______________________________________________________________________________
void msrTuplet::appendNoteToTuplet (
  const S_msrNote&  note,
  const S_msrVoice& voice)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Appending note " <<
      note->asShortString () <<
      // the information is missing to display it the normal way
      " to tuplet " <<
      asString ();

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // append note to elements list
  fTupletElementsList.push_back (note);

  // set note's position in tuplet
  note->setPositionInTuplet (
    fTupletElementsList.size ());

  // register note's uplink to tuplet
  note->
    setNoteShortcutUpLinkToTuplet (this);

//   // register note's uplink to measure // JMI v0.9.66 ???
//   note->
//     setMeasureElementUpLinkToMeasure (
//       fTupletUpLinkToMeasure);

  // account for note duration in tuplet sounding duration
  fSoundingWholeNotes +=
    note->getSoundingWholeNotes ();

  // account for note duration in tuplet displaly duration
  fTupletDisplayWholeNotes += // JMI
    note->getNoteDisplayWholeNotes ();

  // register note's tuplet factor
  note->
    setNoteTupletFactor (fTupletFactor);

  // is this note the shortest one in this voice?
  voice->
    registerShortestNoteInVoiceIfRelevant (note);

  // fetch voice last measure
//   S_msrMeasure
//     voiceLastMeasure =
//       voice->fetchVoiceLastMeasure (
//         inputLineNumber);
//
//   // account for the duration of note in voice last measure
//   voiceLastMeasure->
//     accountForTupletMemberNoteNotesDurationInMeasure (
//       note);

  --gIndenter;
}

void msrTuplet::appendChordToTuplet (const S_msrChord& chord)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Appending chord " <<
      chord->asString () <<
      " to tuplet " <<
      asString ();

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set the chord kind
  chord->setChordKind (msrChordInKind::kChordInTuplet);

  // append chord to elements list
  fTupletElementsList.push_back (chord);

  // set chord's position in tuplet
  chord->setPositionInTuplet (
    fTupletElementsList.size ());

  // DO NOT account for the chord duration,
  // since its first note has been accounted for already
  /* JMI
  fSoundingWholeNotes +=
    chord->getSoundingWholeNotes ();
*/

  fTupletDisplayWholeNotes += // JMI
    chord->getChordDisplayWholeNotes ();

/* too early JMI
  // populate chord's measure number
  chord->setChordMeasureNumber (
    fTupletUpLinkToMeasure->getMeasureNumber ());
*/
}

void msrTuplet::appendTupletToTuplet (const S_msrTuplet& tuplet)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Appending tuplet " <<
      tuplet->asString () <<
      " to tuplet " <<
      asString ();

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  // unapply containing tuplet factor,
  // i.e 3/2 inside 5/4 becomes 15/8 in MusicXML...
  tuplet->
    unapplySoundingFactorToTupletMembers (
      this->getTupletNormalNotes (),
      this->getTupletNormalNotes ();
  */

  // set the tuplet kind
  tuplet->setTupletKind (msrTupletInKind::kTupletInTuplet);

  // append tuplet to elements list
  fTupletElementsList.push_back (tuplet);

  // set tuplet's position in tuplet
  tuplet->setPositionInTuplet (
    fTupletElementsList.size ());

  // account for tuplet duration
  fSoundingWholeNotes +=
    tuplet->getSoundingWholeNotes ();

  fTupletDisplayWholeNotes += // JMI
    tuplet->getTupletDisplayWholeNotes ();
}

void msrTuplet::appendTupletToTupletClone (const S_msrTuplet& tuplet)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Appending tuplet " <<
      tuplet->asString () <<
      " to tuplet clone " <<
      asString ();

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // dont' unapply containing tuplet factor,
  // this has been done when building the MSR from MusicXML

  // append tuplet to elements list
  fTupletElementsList.push_back (tuplet);

  // set tuplet's position in tuplet
  tuplet->setPositionInTuplet (
    fTupletElementsList.size ());

  // account for tuplet duration
  fSoundingWholeNotes +=
    tuplet->getSoundingWholeNotes ();

  fTupletDisplayWholeNotes +=
    tuplet->getTupletDisplayWholeNotes ();
}

S_msrNote msrTuplet::fetchTupletFirstNonGraceNote () const
{
  S_msrNote result;

  if (fTupletElementsList.size ()) {
    S_msrElement
      firstTupletElement =
        fTupletElementsList.front ();

    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*firstTupletElement))
      ) {
      // first element is a note, we have it
      result = note;
    }

    else if (
      S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*firstTupletElement))
      ) {
      // first element is another tuplet, recurse
      result = tuplet->fetchTupletFirstNonGraceNote ();
    }
  }

  else {
    msrInternalError (
      gServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      "cannot access the first note of an empty tuplet");
  }

  return result;
}

S_msrNote msrTuplet::removeFirstNoteFromTuplet (
  int inputLineNumber)
{
  S_msrNote result;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Removing first note from tuplet " <<
      asString ();

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fTupletElementsList.size ()) {
    S_msrElement
      firstTupletElement =
        fTupletElementsList.front ();

    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*firstTupletElement))
    ) {
      fTupletElementsList.pop_front ();
      result = note;
    }

    else {
      if (true) { // JMI ???
        this->print (gLog);
      }

      msrInternalError (
        gServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "removeFirstNoteFromTuplet () expects a note as the first tuplet element");
    }

/* JMI
    for (
      std::list<S_msrElement>::iterator i=fTupletElementsList.begin ();
      i!=fTupletElementsList.end ();
      ++i
    ) {
      if ((*i) == note) {
        // found note, erase it
        i = fTupletElementsList.erase (i);

        // account for note duration
        fSoundingWholeNotes -=
          note->getSoundingWholeNotes ();
        fSoundingWholeNotes.rationalis ();

        fTupletDisplayWholeNotes -= // JMI
          note->getNoteDisplayWholeNotes ();

        // don't update measure number nor measure position: // JMI
        // they have not been set yet

        // return from function
        return;
      }
    } // for

    std::stringstream ss;

    ss <<
      "cannot remove note " <<
      note <<
      " from tuplet " << asString () <<
      " in voice \"" <<
      fTupletUpLinkToMeasure->
        fetchMeasureUpLinkToVoice ()->
          getVoiceName () <<
      "\"," <<
      " since this note has not been found in fTupletElementsList";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  */
  }

  else {
    std::stringstream ss;

    ss <<
      "cannot remove the first note of an empty tuplet " <<
      " in voice \"" <<
      fTupletUpLinkToMeasure->
        fetchMeasureUpLinkToVoice ()->
          getVoiceName () <<
      "\"";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  return result;
}

S_msrNote msrTuplet::removeLastNoteFromTuplet (
  int inputLineNumber)
{
  S_msrNote result;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Removing last note from tuplet " <<
      asString ();

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (fTupletElementsList.size ()) {
    S_msrElement
      lastTupletElement =
        fTupletElementsList.back ();

    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*lastTupletElement))
    ) {
      // remove note from tuplet elements list
      fTupletElementsList.pop_back ();

/*
      // decrement the tuplet sounding whole notes accordingly ??? JMI BAD???
      fSoundingWholeNotes +=
        note->getSoundingWholeNotes ();
*/

      result = note;
    }

    else {
      if (true) { // JMI
        this->print (gLog);
      }

      msrInternalError (
        gServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "removeLastNoteFromTuplet () expects a note as the last tuplet element");
    }
  }

  else {
    std::stringstream ss;

    ss <<
      "cannot remove the last note of an empty tuplet " <<
      " in voice \"" <<
      fTupletUpLinkToMeasure->
        fetchMeasureUpLinkToVoice ()->
          getVoiceName () <<
      "\"";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "This last note from tuplet " <<
      asString () <<
      " turns out to be " <<
      result->asShortString ();

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  return result;
}

void msrTuplet::setMeasurePosition (
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

  // handle the chord itself
  msrMeasureElement::setMeasurePosition (
    measure,
    measurePosition,
    context);

  // handle its members
  setTupletMembersMeasurePositions (
    measure,
    measurePosition);
}

void msrTuplet::setTupletMembersMeasurePositions (
  const S_msrMeasure& measure,
  const msrWholeNotes&     measurePosition)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMeasurePositions ()) {
    std::stringstream ss;

    ss <<
      "Setting tuplet members measure position of " << asString () <<
      " to " <<
      measurePosition.asString ();

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string context = // JMI v0.9.66
    "setTupletMembersMeasurePositions()";

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measurePosition != K_MEASURE_POSITION_UNKNOWN_,
    "measurePosition == K_MEASURE_POSITION_UNKNOWN_");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set tuplet's measure position
  fMeasurePosition = measurePosition;

  // current position
  msrWholeNotes currentPosition = measurePosition;

  // compute measure position for the tuplets elements
  for (
    std::list<S_msrTupletElement>::const_iterator i = fTupletElementsList.begin ();
    i != fTupletElementsList.end ();
    ++i
  ) {
    // set tuplet element measure position

    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*(*i)))
    ) {
      // note
      note->
        setMeasureElementUpLinkToMeasure (
          measure);

      note->
        setMeasurePosition (
          measure,
          currentPosition,
          "msrTuplet::setTupletMembersMeasurePositions()");

      currentPosition +=
        note->
          getSoundingWholeNotes ();
    }

    else if (
      S_msrChord chord = dynamic_cast<msrChord*>(&(*(*i)))
    ) {
      // chord
      chord->
        setChordMembersMeasurePosition (
          measure,
          currentPosition);

      currentPosition +=
        chord->
          getSoundingWholeNotes ();
    }

    else if (
      S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*(*i)))
    ) {
      // nested tuplet
//       currentPosition =
//         tuplet->
//           setTupletMeasurePosition ( // a function JMI ??? v0.9.66
//             measure,
//             currentPosition);

      currentPosition +=
        tuplet->
          getSoundingWholeNotes ();
    }

    else {
      msrInternalError (
        gServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "tuplet member should be a note, a chord or another tuplet");
    }
  } // for
}

void msrTuplet::unapplySoundingFactorToTupletMembers (
  const msrTupletFactor& containingTupletFactor)
  /*
  int containingTupletActualNotes, JMI v0.9.66
  int containingTupletNormalNotes)
  */
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "unapplySoundingFactorToTupletMembers ()" <<
      std::endl;

    ++gIndenter;

    gLog <<
      "% fTupletFactor: " << fTupletFactor.asString () <<
      std::endl <<
      "% containingTupletFactor: " << containingTupletFactor.asString () <<
      std::endl;

    --gIndenter;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fTupletFactor.setTupletActualNotes (
    fTupletFactor.getTupletActualNotes ()
      /
    containingTupletFactor.getTupletActualNotes ());
  fTupletFactor.setTupletNormalNotes (
    fTupletFactor.getTupletNormalNotes ()
      /
    containingTupletFactor.getTupletNormalNotes ());

/* JMI
  fTupletFactor.getTupletNormalNotes () /=
    containingTupletFactor.getTupletNormalNotes ();
  fTupletFactor.fTupletNormalNotes /=
    containingTupletFactor.fTupletNormalNotes;
  */
}

/* JMI ???
void msrTuplet::finalizeTuplet (
  int inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Finalizing tuplet " <<
      asString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/ * JMI v0.9.66
  // we can now set the measure position for all the tuplet members
  setTupletMembersMeasurePositions (
    fMeasurePosition);
  * /
}
*/

void msrTuplet::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTuplet::acceptIn ()";

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTuplet::visitStart ()";

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrTuplet::acceptOut (basevisitor* v)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
      std::stringstream ss;

      ss <<
      "% ==> msrTuplet::acceptOut ()";

    gWaeHandler->waeTraceWithLocationDetails (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTuplet::visitEnd ()";

          gWaeHandler->waeTraceWithLocationDetails (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrTuplet::browseData (basevisitor* v)
{
  for (
    std::list<S_msrTupletElement>::const_iterator i = fTupletElementsList.begin ();
    i != fTupletElementsList.end ();
    ++i
  ) {
    // browse tuplet element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for
}

std::string msrTuplet::asString () const
{
  std::stringstream ss;

  ss <<
    "[Tuplet" <<
    ", tupletKind: " << fTupletKind <<
    fTupletFactor.asString () <<
    ' ' << fSoundingWholeNotes.asString () << " tupletSoundingWholeNotes" <<
    ", measure ' " <<
    ", line " << fInputLineNumber;

  ss <<
    ", getMeasureNumber: ";
  if (fTupletUpLinkToMeasure) {
    ss <<
      fTupletUpLinkToMeasure->getMeasureNumber ();
  }
  else {
    ss << "[UNKNOWN_MEASURE_NUMBER]";
  }

  ss <<
    ", fMeasurePosition: " <<
    fMeasurePosition.asString ();

  ss << '[';

  if (fTupletElementsList.size ()) {
    std::list<S_msrTupletElement>::const_iterator
      iBegin = fTupletElementsList.begin (),
      iEnd   = fTupletElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {

      if (
        S_msrNote note = dynamic_cast<msrNote*>(&(*(*i)))
        ) {
        ss <<
          note->asShortString ();
      }

      else if (
        S_msrChord chord = dynamic_cast<msrChord*>(&(*(*i)))
        ) {
        ss <<
          chord->asString ();
      }

      else if (
        S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*(*i)))
        ) {
        ss <<
          tuplet->asString ();
      }

      else {
        msrInternalError (
          gServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          "tuplet member should be a note, a chord or another tuplet");
      }

      if (++i == iEnd) break;
      ss << ' ';

    } // for
  }

  ss << ']' << ']';

  return ss.str ();
}

void msrTuplet::printFull (std::ostream& os) const
{
  os <<
    "[Tuplet" <<
    ", fTupletKind: " << fTupletKind <<
    ", " <<
    mfSingularOrPlural (
      fTupletElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 36;

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
    "fMemberNotesSoundingWholeNotes" << ": " <<
    fMemberNotesSoundingWholeNotes.asString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMemberNotesDisplayWholeNotes" << ": " <<
    fMemberNotesDisplayWholeNotes.asString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletDisplayWholeNotes" << ": " <<
    fTupletDisplayWholeNotes.asString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletFactor" << ": " <<
    fTupletFactor <<

    std::setw (fieldWidth) <<
    "fTupletNumber" << ": " <<
    fTupletNumber <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletBracketKind" << ": " <<
    msrTupletBracketKindAsString (
      fTupletBracketKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletLineShapeKind" << ": " <<
    msrTupletLineShapeKindAsString (
      fTupletLineShapeKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletShowNumberKind" << ": " <<
    msrTupletShowNumberKindAsString (
      fTupletShowNumberKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletShowTypeKind" << ": " <<
    msrTupletShowTypeKindAsString (
      fTupletShowTypeKind) <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletMeasureNumber" << ": " <<
    fTupletUpLinkToMeasure->getMeasureNumber () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fVoicePosition" << ": " <<
    fVoicePosition <<
    std::endl << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletElementsList" << ": ";
  if (fTupletElementsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrTupletElement>::const_iterator
      iBegin = fTupletElementsList.begin (),
      iEnd   = fTupletElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrTupletElement tupletElement = (*i);

      os << tupletElement;

      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os << "[EMPTY]" << std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletUpLinkToMeasure" << ": ";
  if (fTupletUpLinkToMeasure) {
    os <<
      fTupletUpLinkToMeasure->asShortString ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletShortcutUpLinkToTuplet" << ": ";
  if (fTupletShortcutUpLinkToTuplet) {
    os <<
      fTupletShortcutUpLinkToTuplet->asShortString ();
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

  os << ']' << std::endl;

  --gIndenter;
}

void msrTuplet::print (std::ostream& os) const
{
  os <<
    "[Tuplet" <<
    ", fTupletKind: " << fTupletKind <<
    ", " <<
    mfSingularOrPlural (
      fTupletElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 36;

  os << std::left <<
    std::setw (fieldWidth) <<
    std::setw (fieldWidth) <<
    "fMeasurePosition" << ": " <<
    fMeasurePosition.asString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fSoundingWholeNotes" << ": " <<
    fSoundingWholeNotes.asString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletDisplayWholeNotes" << ": " <<
    fTupletDisplayWholeNotes.asString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMemberNotesSoundingWholeNotes" << ": " <<
    fMemberNotesSoundingWholeNotes.asString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fMemberNotesDisplayWholeNotes" << ": " <<
    fMemberNotesDisplayWholeNotes.asString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletFactor" << ": " <<
    fTupletFactor <<

    std::setw (fieldWidth) <<
    "fTupletNumber" << ": " <<
    fTupletNumber <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletMeasureNumber" << ": ";
    if (fTupletUpLinkToMeasure) {
      os <<
        fTupletUpLinkToMeasure->getMeasureNumber ();
    }
    else {
      os << "[UNKNOWN_MEASURE_NUMBER]";
    }
  os << std::endl;

  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletElementsList" << ": ";
  if (fTupletElementsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrTupletElement>::const_iterator
      iBegin = fTupletElementsList.begin (),
      iEnd   = fTupletElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrTupletElement tupletElement = (*i);

      os << tupletElement;

      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os << "[EMPTY]" << std::endl;
  }

  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletUpLinkToMeasure" << ": ";
  if (fTupletUpLinkToMeasure) {
    os <<
      fTupletUpLinkToMeasure->asShortString ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletShortcutUpLinkToTuplet" << ": ";
  if (fTupletShortcutUpLinkToTuplet) {
    os <<
      fTupletShortcutUpLinkToTuplet->asShortString ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrTuplet& elt)
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
