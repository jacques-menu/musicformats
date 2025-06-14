/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "msrWae.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrMeasureConstants.h"
#include "msrTuplets.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrTuplet msrTuplet::create (
  const mfInputLineNumber& inputLineNumber,
  const mfTupletNumber&    tupletNumber,
  const msrTupletFactor&   tupletFactor,
  msrTupletBracketKind     tupletBracketKind,
  msrTupletLineShapeKind   tupletLineShapeKind,
  msrTupletTypeKind        tupletTypeKind,
  msrTupletShowNumberKind  tupletShowNumberKind,
  msrTupletShowTypeKind    tupletShowTypeKind,
  msrPlacementKind         tupletBracketPlacementKind)
{
  msrTuplet* obj =
    new msrTuplet (
      inputLineNumber,
      tupletNumber,
      tupletFactor,
      tupletBracketKind,
      tupletLineShapeKind,
      tupletTypeKind,
      tupletShowNumberKind,
      tupletShowTypeKind,
      tupletBracketPlacementKind);
  assert (obj != nullptr);
  return obj;
}

msrTuplet::msrTuplet (
  const mfInputLineNumber& inputLineNumber,
  const mfTupletNumber&    tupletNumber,
  const msrTupletFactor&   tupletFactor,
  msrTupletBracketKind     tupletBracketKind,
  msrTupletLineShapeKind   tupletLineShapeKind,
  msrTupletTypeKind        tupletTypeKind,
  msrTupletShowNumberKind  tupletShowNumberKind,
  msrTupletShowTypeKind    tupletShowTypeKind,
  msrPlacementKind         tupletBracketPlacementKind)
    : msrTupletElement (
        inputLineNumber)
{
  fTupletKind = msrTupletInKind::kTupletIn_UNKNOWN_;

  fTupletNumber = tupletNumber;
  fTupletFactor = tupletFactor;

  fTupletBracketKind = tupletBracketKind;
  fTupletLineShapeKind = tupletLineShapeKind;
  fTupletTypeKind = tupletTypeKind;
  fTupletShowNumberKind = tupletShowNumberKind;
  fTupletShowTypeKind = tupletShowTypeKind;

  fTupletBracketPlacementKind = tupletBracketPlacementKind;

  setMeasureElementSoundingWholeNotes (
    K_WHOLE_NOTES_ZERO,
    "msrTuplet::msrTuplet()");

  fTupletDisplayWholeNotes = K_WHOLE_NOTES_ZERO;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating tuplet " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

msrTuplet::~msrTuplet ()
{}

S_msrTuplet msrTuplet::createTupletNewbornClone ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of tuplet " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  const S_msrTuplet&
    newbornClone =
      msrTuplet::create (
        fInputLineNumber,
        fTupletNumber,
        fTupletFactor,
        fTupletBracketKind,
        fTupletLineShapeKind,
        fTupletTypeKind,
        fTupletShowNumberKind,
        fTupletShowTypeKind,
        fTupletBracketPlacementKind);

/* JMI ???
  newbornClone->fMeasureElementSoundingWholeNotes =
    fMeasureElementSoundingWholeNotes;

  newbornClone->fTupletDisplayWholeNotes =
    fTupletDisplayWholeNotes;

  newbornClone->fMeasureElementUpLinkToMeasure->getMeasureNumber () =
    fMeasureElementUpLinkToMeasure->getMeasureNumber ();

  newbornClone->fMeasureElementPositionInMeasure =
    fMeasureElementPositionInMeasure;
*/

  return newbornClone;
}

//______________________________________________________________________________
// set and get

// tuplet kind
void msrTuplet::setTupletKind (
    msrTupletInKind tupletKind)
{
  fTupletKind = tupletKind;
}

//______________________________________________________________________________
void msrTuplet::appendNoteToTuplet (const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "Appending note " <<
      note->asShortString () <<
      // the information is missing to display it the normal way JMI 0.9.70
      " to tuplet " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  ++gIndenter;

  // set or check member notes sounding whole notes
  mfWholeNotes
    noteSoundingWholeNotes =
      note->getMeasureElementSoundingWholeNotes ();

  // append note to elements list
  fTupletElementsList.push_back (note);

  // set note's position in tuplet
  note->setPositionInTuplet (
    fTupletElementsList.size ());

  // register note's uplink to tuplet
  note->
    setNoteShortcutUpLinkToTuplet (this);

//   // register note's uplink to measure // JMI 0.9.70 ???
//   note->
//     setMeasureElementUpLinkToMeasure (
//       fMeasureElementUpLinkToMeasure);

  // mark note as belonging to a tuplet
  note->setNoteBelongsToATuplet ();

  // account for the note duration
  fMeasureElementSoundingWholeNotes +=
    note->getMeasureElementSoundingWholeNotes ();

  // account for note duration in tuplet displaly duration
  fTupletDisplayWholeNotes += // JMI 0.9.70
    note->getNoteDisplayWholeNotes ();

  // register note's tuplet factor
  note->
    setNoteTupletFactor (fTupletFactor);

  --gIndenter;
}

void msrTuplet::appendChordToTuplet (const S_msrChord& chord)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "appendChordToTuplet(): Appending chord " <<
      chord <<
      " to tuplet " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    chord != nullptr,
    "chord is NULL");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set the chord kind
  chord->setChordInKind (msrChordInKind::kChordInTuplet);

  // append chord to elements list
  fTupletElementsList.push_back (chord);

  // set chord's position in tuplet
  chord->setPositionInTuplet (
    fTupletElementsList.size ());

  // account for the chord duration in containing tuplet if relevant
  fMeasureElementSoundingWholeNotes +=
    chord->getMeasureElementSoundingWholeNotes ();

  // account for the chord duration in containing tuplet if relevant
  switch (chord->getChordInKind ()) {
    case msrChordInKind::kChordInTuplet:
      if (chord->getChordShortcutUpLinkToContainingTuplet ()) {
        chord->getChordShortcutUpLinkToContainingTuplet ()->
          incrementMeasureElementSoundingWholeNotesBy (
            chord->getMeasureElementSoundingWholeNotes (),
            "appendChordToTuplet()");
      }
      break;
    default:
      ;
  } // switch

// gLog << "appendChordToTuplet(): fMeasureElementSoundingWholeNotes: " << fMeasureElementSoundingWholeNotes << std::endl;

  fTupletDisplayWholeNotes += // JMI USELESS ??? 0.9.72
    chord->getChordDisplayWholeNotes ();

/* too early JMI
  // populate chord's measure number
  chord->setChordMeasureNumber (
    fMeasureElementUpLinkToMeasure->getMeasureNumber ());
*/
}

void msrTuplet::appendTupletToTuplet (const S_msrTuplet& tuplet)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending tuplet " <<
      tuplet->asString () <<
      " to tuplet " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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

  // account for the tuplet duration
  fMeasureElementSoundingWholeNotes +=
    tuplet->getMeasureElementSoundingWholeNotes ();

  fTupletDisplayWholeNotes += // JMI
    tuplet->getTupletDisplayWholeNotes ();
}

void msrTuplet::appendTupletToTupletClone (const S_msrTuplet& tuplet)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Appending tuplet " <<
      tuplet->asString () <<
      " to tuplet clone " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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

  // account for tuplet duration // JMI 0.9.70 ???
  fMeasureElementSoundingWholeNotes +=
    tuplet->getMeasureElementSoundingWholeNotes ();

  fTupletDisplayWholeNotes +=
    tuplet->getTupletDisplayWholeNotes ();
}

// void msrTuplet::removeLastElementFromTuplet (
//   const mfInputLineNumber& inputLineNumber,
//   const S_msrElement& element)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceVoices ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Removing element '" <<
//       element->asShortString () <<
//       "' from voice \"" << fVoiceName << "\"";
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   ++gIndenter;
//
//   fVoiceLastSegment->
//     removeElementFromSegment (
//       inputLineNumber,
//       element);
//
//   --gIndenter;
// }

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
      __FILE__, mfInputLineNumber (__LINE__),
      "cannot access the first note of an empty tuplet");
  }

  return result;
}

// S_msrNote msrTuplet::removeFirstNoteFromTuplet (
//   const mfInputLineNumber& inputLineNumber)
// {
//   S_msrNote result;
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceTuplets ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Removing first note from tuplet " <<
//       asString ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   if (fTupletElementsList.size ()) {
//     S_msrTupletElement
//       firstTupletElement =
//         fTupletElementsList.front ();
//
//     if (
//       S_msrNote note = dynamic_cast<msrNote*>(&(*firstTupletElement))
//     ) {
//       fTupletElementsList.pop_front ();
//       result = note;
//     }
//
//     else {
//       if (true) { // JMI ???
//         this->print (gLog);
//       }
//
//       msrInternalError (
//         gServiceRunData->getInputSourceName (),
//         fInputLineNumber,
//         __FILE__, mfInputLineNumber (__LINE__),
//         "removeFirstNoteFromTuplet () expects a note as the first tuplet element");
//     }
//
// /* JMI
//     for (
//       std::list <S_msrElement>::iterator i = fTupletElementsList.begin ();
//       i != fTupletElementsList.end ();
//       ++i
//     ) {
//       if ((*i) == note) {
//         // found note, erase it
//         i = fTupletElementsList.erase (i);
//
//         // account for note duration
//         fMeasureElementSoundingWholeNotes -=
//           note->getMeasureElementSoundingWholeNotes ();
//         fMeasureElementSoundingWholeNotes.rationalis ();
//
//         fTupletDisplayWholeNotes -= // JMI
//           note->getNoteDisplayWholeNotes ();
//
//         // don't update measure number nor measure position: // JMI
//         // they have not been set yet
//
//         // return from function
//         return;
//       }
//     } // for
//
//     std::stringstream ss;
//
//     ss <<
//       "cannot remove note " <<
//       note <<
//       " from tuplet " << asString () <<
//       " in voice \"" <<
//       fMeasureElementUpLinkToMeasure->
//         fetchMeasureUpLinkToVoice ()->
//           getVoiceName () <<
//       "\"," <<
//       " since this note has not been found in fTupletElementsList";
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   */
//   }
//
//   else {
//     std::stringstream ss;
//
//     ss <<
//       "cannot remove the first note of an empty tuplet " <<
//       " in voice \"" <<
//       fMeasureElementUpLinkToMeasure->
//         fetchMeasureUpLinkToVoice ()->
//           getVoiceName () <<
//       "\"";
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
//   return result;
// }

// S_msrNote msrTuplet::removeLastNoteFromTuplet (
//   const mfInputLineNumber& inputLineNumber)
// {
//   S_msrNote result;
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceTuplets ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Removing last note from tuplet " <<
//       asString ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   if (fTupletElementsList.size ()) {
//     S_msrTupletElement
//       lastTupletElement =
//         fTupletElementsList.back ();
//
//     if (
//       S_msrNote note = dynamic_cast<msrNote*>(&(*lastTupletElement))
//     ) {
//       // remove note from tuplet elements list
//       fTupletElementsList.pop_back ();
//
// /*
//       // decrement the tuplet sounding whole notes accordingly ??? JMI BAD???
//       fMeasureElementSoundingWholeNotes +=
//         note->getMeasureElementSoundingWholeNotes ();
// */
//
//       result = note;
//     }
//
//     else {
//       if (true) { // JMI
//         this->print (gLog);
//       }
//
//       msrInternalError (
//         gServiceRunData->getInputSourceName (),
//         fInputLineNumber,
//         __FILE__, mfInputLineNumber (__LINE__),
//         "removeLastNoteFromTuplet () expects a note as the last tuplet element");
//     }
//   }
//
//   else {
//     std::stringstream ss;
//
//     ss <<
//       "cannot remove the last note of an empty tuplet " <<
//       " in voice \"" <<
//       fMeasureElementUpLinkToMeasure->
//         fetchMeasureUpLinkToVoice ()->
//           getVoiceName () <<
//       "\"";
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceTuplets ()) {
//     std::stringstream ss;
//
//     ss <<
//       "This last note from tuplet " <<
//       asString () <<
//       " turns out to be " <<
//       result->asShortString ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   return result;
// }

// S_msrTupletElement msrTuplet::removeLastElementFromTuplet (
//   const mfInputLineNumber& inputLineNumber)
// {
//   S_msrTupletElement result;
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceTuplets ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Removing last element from tuplet " <<
//       asString ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   if (fTupletElementsList.size ()) {
//     result =
//       fTupletElementsList.back ();
//
// //     if (
// //       S_msrNote note = dynamic_cast<msrNote*>(&(*lastTupletElement))
// //     ) {
// //       // remove note from tuplet elements list
// //       fTupletElementsList.pop_back ();
// //
// // /*
// //       // decrement the tuplet sounding whole notes accordingly ??? JMI BAD???
// //       fMeasureElementSoundingWholeNotes +=
// //         note->getMeasureElementSoundingWholeNotes ();
// // */
// //
// //       result = note;
// //     }
// //
// //     else {
// //       if (true) { // JMI
// //         this->print (gLog);
// //       }
// //
// //       msrInternalError (
// //         gServiceRunData->getInputSourceName (),
// //         fInputLineNumber,
// //         __FILE__, mfInputLineNumber (__LINE__),
// //         "removeLastNoteFromTuplet () expects a note as the last tuplet element");
// //     }
//   }
//
//   else {
//     std::stringstream ss;
//
//     ss <<
//       "cannot remove the last element of an empty tuplet " <<
//       " in voice \"" <<
//       fMeasureElementUpLinkToMeasure->
//         fetchMeasureUpLinkToVoice ()->
//           getVoiceName () <<
//       "\"";
//
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       inputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceTuplets ()) {
//     std::stringstream ss;
//
//     ss <<
//       "This last element from tuplet " <<
//       asString () <<
//       " turns out to be " <<
//       result->asShortString ();
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   return result;
// }

void msrTuplet::setMeasureElementPositionInMeasure (
  const S_msrMeasure&        measure,
  const mfPositionInMeasure& positionInMeasure,
  const std::string&         context)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePositionInMeasures ()) {
    S_msrMeasure
      upLinkToMeasure =
        getMeasureElementUpLinkToMeasure ();

    std::stringstream ss;

    ss <<
      "Setting the measure position of " <<
      asString () <<
      " to " <<
      positionInMeasure.asString () <<
      " (was '" <<
      fMeasureElementPositionInMeasure.asString () <<
      "') in measure " <<
      measure->asShortString () <<
      " (measureElementMeasureNumber: " <<
      upLinkToMeasure->getMeasureNumber () <<
      "), context: \"" <<
      context <<
      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // handle the chord itself
  msrMeasureElement::setMeasureElementPositionInMeasure (
    measure,
    positionInMeasure,
    context);

  // handle its members
  setTupletMembersPositionInMeasures (
    measure,
    positionInMeasure);
}

void msrTuplet::setTupletMembersPositionInMeasures (
  const S_msrMeasure&        measure,
  const mfPositionInMeasure& positionInMeasure)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePositionInMeasures ()) {
    std::stringstream ss;

    ss <<
      "Setting tuplet members measure position of " << asString () <<
      " to " <<
      positionInMeasure.asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  std::string context = // JMI 0.9.66
    "setTupletMembersPositionInMeasures()";

#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, mfInputLineNumber (__LINE__),
    positionInMeasure != K_POSITION_IN_MEASURE_UNKNOWN_,
    "positionInMeasure == K_POSITION_IN_MEASURE_UNKNOWN_");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

  // set tuplet's measure position
  fMeasureElementPositionInMeasure = positionInMeasure;

  // current position
  mfPositionInMeasure currentPosition = positionInMeasure;

  // compute measure position for the tuplets elements
  for (
    std::list <S_msrTupletElement>::const_iterator i = fTupletElementsList.begin ();
    i != fTupletElementsList.end ();
    ++i
  ) {
    // set tuplet element measure position

    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*(*i)))
    ) {
      // note
      note->
        setMeasureElementUpLinkToMeasure ( // JMI 0.9.70 ???
          measure);

      note->
        setMeasureElementPositionInMeasure (
          measure,
          currentPosition,
          "msrTuplet::setTupletMembersPositionInMeasures()");

      currentPosition +=
        note->
          getMeasureElementSoundingWholeNotes ();
    }

    else if (
      S_msrChord chord = dynamic_cast<msrChord*>(&(*(*i)))
    ) {
      // chord
      chord->
        setChordMembersPositionInMeasure (
          measure,
          currentPosition);

      currentPosition +=
        chord->
          getMeasureElementSoundingWholeNotes ();
    }

    else if (
      S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*(*i)))
    ) {
      // nested tuplet
//       currentPosition =
//         tuplet->
//           setTupletPositionInMeasure ( // a function JMI ??? 0.9.66
//             measure,
//             currentPosition);

      currentPosition +=
        tuplet->
          getMeasureElementSoundingWholeNotes ();
    }

    else {
      msrInternalError (
        gServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, mfInputLineNumber (__LINE__),
        "tuplet member should be a note, a chord or another tuplet");
    }
  } // for
}

void msrTuplet::unapplySoundingFactorToTupletMembers (
  const msrTupletFactor& containingTupletFactor)
  /*
  int containingTupletActualNotes, JMI 0.9.66
  int containingTupletNormalNotes)
  */
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTuplets ()) {
    std::stringstream ss;

    ss <<
      "unapplySoundingFactorToTupletMembers()" <<
      ", fTupletFactor: " << fTupletFactor.asFractionString () <<
      ", containingTupletFactor: " << containingTupletFactor.asFractionString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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

void msrTuplet::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTuplet::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
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

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
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

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrTuplet::browseData (basevisitor* v)
{
  for (
    std::list <S_msrTupletElement>::const_iterator i = fTupletElementsList.begin ();
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
    ", fMeasureElementSoundingWholeNotes: " <<
    fMeasureElementSoundingWholeNotes <<
    ", fTupletNumber: " << fTupletNumber <<
    ", fTupletFactor: " << fTupletFactor.asFractionString () <<
    ", fTupletKind: " << fTupletKind <<
    ", line " << fInputLineNumber;

  ss <<
    ", getMeasureNumber: ";
  if (fMeasureElementUpLinkToMeasure) {
    ss <<
      fMeasureElementUpLinkToMeasure->getMeasureNumber ();
  }
  else {
    ss << "[UNKNOWN_MEASURE_NUMBER]";
  }

  ss <<
    ", fMeasureElementPositionInMeasure: " <<
    fMeasureElementPositionInMeasure.asString ();

  ss << ", fTupletElementsList: [";

  if (fTupletElementsList.size ()) {
    std::list <S_msrTupletElement>::const_iterator
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
          __FILE__, mfInputLineNumber (__LINE__),
          "tuplet member should be a note, a chord or another tuplet");
      }

      if (++i == iEnd) break;
      ss << ", ";

    } // for
  }

  ss <<
    ']' <<
    ']';

  return ss.str ();
}

std::string msrTuplet::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[Tuplet" <<
    ", fMeasureElementSoundingWholeNotes: " <<
    fMeasureElementSoundingWholeNotes <<
    ", fTupletNumber: " << fTupletNumber <<
    ", fTupletFactor: " << fTupletFactor.asFractionString () <<
    ", fTupletKind: " << fTupletKind <<
    ", fTupletBracketPlacementKind: " << fTupletBracketPlacementKind <<
    ", line " << fInputLineNumber;

  ss <<
    ", getMeasureNumber: ";
  if (fMeasureElementUpLinkToMeasure) {
    ss <<
      fMeasureElementUpLinkToMeasure->getMeasureNumber ();
  }
  else {
    ss << "[UNKNOWN_MEASURE_NUMBER]";
  }

  ss <<
    ", fMeasureElementPositionInMeasure: " <<
    fMeasureElementPositionInMeasure.asString ();

  ss << '[';

  if (fTupletElementsList.size ()) {
    std::list <S_msrTupletElement>::const_iterator
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
          chord->asShortString ();
      }

      else if (
        S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*(*i)))
        ) {
        ss <<
          tuplet->asShortString ();
      }

      else {
        msrInternalError (
          gServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, mfInputLineNumber (__LINE__),
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
    ", fMeasureElementSoundingWholeNotes: " <<
    fMeasureElementSoundingWholeNotes <<
    ", fTupletNumber: " << fTupletNumber <<
    ", fTupletFactor: " << fTupletFactor.asFractionString () <<
    ", fTupletKind: " << fTupletKind <<
    ", fTupletBracketPlacementKind: " << fTupletBracketPlacementKind <<
    ", " <<
    mfSingularOrPlural (
      fTupletElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 36;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureElementPositionInMeasure" << ": " <<
    fMeasureElementPositionInMeasure.asString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletDisplayWholeNotes" << ": " <<
    fTupletDisplayWholeNotes <<
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

//     std::setw (fieldWidth) <<
//     "fTupletMeasureNumber" << ": " <<
//     fMeasureElementUpLinkToMeasure->getMeasureNumber () <<
//     std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureElementVoicePosition" << ": " <<
    fMeasureElementVoicePosition <<
    std::endl << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureElementUpLinkToMeasure" << ": ";
  if (fMeasureElementUpLinkToMeasure) {
    os <<
      fMeasureElementUpLinkToMeasure->asShortString ();
  }
  else {
    os <<
      "fMeasureElementUpLinkToMeasure: NULL"; // JMI 0.9.71
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletUpLinkToContainingTuplet" << ": ";
  if (fTupletUpLinkToContainingTuplet) {
    os <<
      fTupletUpLinkToContainingTuplet->asShortString ();
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
    "fTupletElementsList" << ": ";
  if (fTupletElementsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrTupletElement>::const_iterator
      iBegin = fTupletElementsList.begin (),
      iEnd   = fTupletElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrTupletElement tupletElement = (*i);

      tupletElement->printFull (os);

      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os << "[EMPTY]" << std::endl;
  }

  os << ']' << std::endl;

  --gIndenter;
}

void msrTuplet::print (std::ostream& os) const
{
  os <<
    "[Tuplet" <<
    ", fMeasureElementSoundingWholeNotes: " <<
    fMeasureElementSoundingWholeNotes <<
    ", fTupletNumber: " << fTupletNumber <<
    ", fTupletFactor: " << fTupletFactor.asFractionString () <<
    ", " <<
    mfSingularOrPlural (
      fTupletElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 36;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletKind" << ": " <<
    fTupletKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletBracketPlacementKind" << ": " <<
    fTupletBracketPlacementKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletBracketKind" << ": " <<
    fTupletBracketKind <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fMeasureElementPositionInMeasure" << ": " <<
    fMeasureElementPositionInMeasure.asString () <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTupletDisplayWholeNotes" << ": " <<
    fTupletDisplayWholeNotes <<
    std::endl;

//   os << std::left <<
//     std::setw (fieldWidth) <<
//     "fTupletMeasureNumber" << ": ";
//     if (fMeasureElementUpLinkToMeasure) {
//       os <<
//         fMeasureElementUpLinkToMeasure->getMeasureNumber ();
//     }
//     else {
//       os << "[UNKNOWN_MEASURE_NUMBER]";
//     }
//   os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fMeasureElementUpLinkToMeasure" << ": ";
  if (fMeasureElementUpLinkToMeasure) {
    os <<
      fMeasureElementUpLinkToMeasure->asShortString ();
  }
  else {
    os <<
      "fMeasureElementUpLinkToMeasure: NULL"; // JMI 0.9.71
  }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletUpLinkToContainingTuplet" << ": ";
  if (fTupletUpLinkToContainingTuplet) {
    os <<
      fTupletUpLinkToContainingTuplet->asShortString ();
  }
  else {
    os << "[NULL]";
  }
  os << std::endl;

  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTupletElementsList" << ": " <<
    mfSingularOrPlural (
      fTupletElementsList.size (),
      "element", "elements");

  if (fTupletElementsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrTupletElement>::const_iterator
      iBegin = fTupletElementsList.begin (),
      iEnd   = fTupletElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrTupletElement tupletElement = (*i);

     tupletElement->print (os);

      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
//   else {
//     os << "[EMPTY]" << std::endl;
//   }

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


// S_msrMeasure msrTuplet::fetchMeasureElementUpLinkToMeasure () const
// {
//   S_msrMeasure result;
//
//   switch (fTupletKind) {
//     case msrTupletInKind::kTupletIn_UNKNOWN_:
//       break;
//
//     case msrTupletInKind::kTupletInMeasure:
//       result = fMeasureElementUpLinkToMeasure;
//       break;
//
//     case msrTupletInKind::kTupletInTuplet:
//       if (fTupletUpLinkToContainingTuplet) {
//         result =
//           fTupletUpLinkToContainingTuplet->
//             fetchMeasureElementUpLinkToMeasure ();
//       }
//       break;
//   } // switch
//
//   return result;
// }

/* JMI ???
void msrTuplet::finalizeTuplet (
  const mfInputLineNumber& inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTupletsBasics ()) {
    std::stringstream ss;

    ss <<
      "Finalizing tuplet " <<
      asString () <<
      ", line " << inputLineNumber;

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/ * JMI 0.9.66
  // we can now set the measure position for all the tuplet members
  setTupletMembersPositionInMeasures (
    fMeasureElementPositionInMeasure);
  * /
}
*/

