/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

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

#include "msrWae.h"

#include "msrGraceNotesGroups.h"
#include "msrTuplets.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrGraceNotesGroup msrGraceNotesGroup::create (
  const mfInputLineNumber& inputLineNumber,
  msrGraceNotesGroupKind   graceNotesGroupKind,
  Bool                     graceNotesGroupIsSlashed,
  Bool                     graceNotesGroupIsBeamed,
  Bool                     graceNotesGroupIsTied,
  Bool                     graceNotesGroupIsSlurred,
  const mfMeasureNumber&   graceNotesGroupMeasureNumber)
{
  msrGraceNotesGroup* obj =
    new msrGraceNotesGroup (
      inputLineNumber,
      graceNotesGroupKind,
      graceNotesGroupIsSlashed,
      graceNotesGroupIsBeamed,
      graceNotesGroupIsTied,
      graceNotesGroupIsSlurred,
      graceNotesGroupMeasureNumber);
  assert (obj != nullptr);
  return obj;
}

msrGraceNotesGroup::msrGraceNotesGroup (
  const mfInputLineNumber& inputLineNumber,
  msrGraceNotesGroupKind   graceNotesGroupKind,
  Bool                     graceNotesGroupIsSlashed,
  Bool                     graceNotesGroupIsBeamed,
  Bool                     graceNotesGroupIsTied,
  Bool                     graceNotesGroupIsSlurred,
  const mfMeasureNumber&   graceNotesGroupMeasureNumber)
    : msrElement (inputLineNumber)
{

  fGraceNotesGroupKind = graceNotesGroupKind;

  fGraceNotesGroupIsSlashed = graceNotesGroupIsSlashed;
  fGraceNotesGroupIsBeamed = graceNotesGroupIsBeamed;

  fGraceNotesGroupIsTied = graceNotesGroupIsTied;
  fGraceNotesGroupIsSlurred = graceNotesGroupIsSlurred;

  fGraceNotesGroupMeasureNumber = graceNotesGroupMeasureNumber;

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating a grace notes group: " <<
      asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // grace notes are followed by notes
  // unless they are last in a measure
  fGraceNotesGroupIsFollowedByNotes = true;
}

msrGraceNotesGroup::~msrGraceNotesGroup ()
{}

S_msrGraceNotesGroup msrGraceNotesGroup::createGraceNotesGroupNewbornClone ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating a newborn clone of grace notes group " <<
      asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  S_msrGraceNotesGroup
    newbornClone =
      msrGraceNotesGroup::create (
        fInputLineNumber,
        fGraceNotesGroupKind,
        fGraceNotesGroupIsSlashed,
        fGraceNotesGroupIsBeamed,
        fGraceNotesGroupIsTied,
        fGraceNotesGroupIsSlurred,
        fGraceNotesGroupMeasureNumber);

  newbornClone->fGraceNotesGroupIsTied =
    fGraceNotesGroupIsTied;
  newbornClone->fGraceNotesGroupIsSlurred =
    fGraceNotesGroupIsSlurred;

  newbornClone->fGraceNotesGroupIsFollowedByNotes =
    fGraceNotesGroupIsFollowedByNotes;

  return newbornClone;
}

S_msrTuplet msrGraceNotesGroup::fetchGraceNoteGroupUpLinkToTuplet () const
{
  S_msrTuplet result;

  // JMI 0.9.70 ???

  return result;
}

S_msrPart msrGraceNotesGroup::fetchGraceNotesGroupUpLinkToPart () const
{
  S_msrPart result;

/* JMI BLARK
  if (fGraceNotesGroupUpLinkToVoice) {
    result =
      fGraceNotesGroupUpLinkToVoice->
        fetchVoiceUpLinkToPart ();
  }
*/

  return result;
}

std::string msrGraceNotesGroupKindAsString (
  msrGraceNotesGroupKind graceNotesGroupKind)
{
  std::string result;

  switch (graceNotesGroupKind) {
    case msrGraceNotesGroupKind::kGraceNotesGroupBefore:
      result = "kGraceNotesGroupBefore";
      break;
    case msrGraceNotesGroupKind::kGraceNotesGroupAfter:
      result = "kGraceNotesGroupAfter";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const msrGraceNotesGroupKind& elt)
{
  os << msrGraceNotesGroupKindAsString (elt);
  return os;
}

S_msrGraceNotesGroup msrGraceNotesGroup::createSkipGraceNotesGroupClone ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Creating a skip clone of grace notes group " <<
      asShortString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // create the grace notes group
  S_msrGraceNotesGroup
    clone =
      msrGraceNotesGroup::create (
        fInputLineNumber,
        fGraceNotesGroupKind,
        fGraceNotesGroupIsSlashed,
        fGraceNotesGroupIsBeamed,
        fGraceNotesGroupIsTied,
        fGraceNotesGroupIsSlurred,
        fGraceNotesGroupMeasureNumber);

  clone->fGraceNotesGroupIsTied =
    fGraceNotesGroupIsTied;
  clone->fGraceNotesGroupIsSlurred =
    fGraceNotesGroupIsSlurred;

  clone->fGraceNotesGroupIsFollowedByNotes =
    fGraceNotesGroupIsFollowedByNotes;

  // populating the clone with skips
  for (
    std::list <S_msrMeasureElement>::const_iterator i = fGraceNotesGroupElementsList.begin ();
    i != fGraceNotesGroupElementsList.end ();
    ++i
  ) {
    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*(*i)))
    ) {
      // create grace skip note with same duration as note
#ifdef MF_TRACE_IS_ENABLED
      if (gTraceOahGroup->getTraceGraceNotes ()) {
        std::stringstream ss;

        ss <<
          "Creating a skip grace note" <<
          ", soundingWholeNotes: " << note->getMeasureElementSoundingWholeNotes ().asFractionString () <<
          ", displayWholeNotes: " << note->getNoteDisplayWholeNotes ().asFractionString ();

        gWaeHandler->waeTrace (
          __FILE__, mfInputLineNumber (__LINE__),
          ss.str ());
      }
#endif // MF_TRACE_IS_ENABLED

      S_msrNote
        skip =
          msrNote::createGraceSkipNote (
            note->getInputLineNumber (),
            mfMeasureNumber ("K_MF_MEASURE_NUMBER_UNKNOWN_"), // will be set afterwards 0.9.75
            note->getMeasureElementSoundingWholeNotes (),   // 0/1 JMI 0.9.66
            note->getNoteDisplayWholeNotes (),
            note->getNoteDotsNumber ());

      // append it to the grace notes
      clone->
        appendNoteToGraceNotesGroup (skip);
    }

    else if (
      S_msrChord chord = dynamic_cast<msrChord*>(&(*(*i)))
    ) {
      // fetch the chord's first note
      S_msrNote
        chordFirstNote =
          chord->getChordNotesVector () [0];

      // create grace skip note with same duration as chord
      S_msrNote
        skip =
          msrNote::createGraceSkipNote (
            chordFirstNote->getInputLineNumber (),
            chordFirstNote->
              getMeasureElementUpLinkToMeasure ()->
                getMeasureNumber (),
            chordFirstNote->getMeasureElementSoundingWholeNotes (), // 0/1 JMI 0.9.66
            chordFirstNote->getNoteDisplayWholeNotes (),
            chordFirstNote->getNoteDotsNumber ());

      // append it to the grace notes
      clone->
        appendNoteToGraceNotesGroup (skip);
    }

    else {
      msrInternalError (
        gServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, mfInputLineNumber (__LINE__),
        "grace notes element should be a note or a chord");
    }
  } // for

  return clone;
}

void msrGraceNotesGroup::appendNoteToGraceNotesGroup (const S_msrNote& note)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Appending grace note " <<
      note->asString () <<
      " to grace notes group " <<
      asShortString ();
//      " in voice \"" <<
//      fGraceNotesGroupUpLinkToVoice->getVoiceName () <<
//      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fGraceNotesGroupElementsList.push_back (note);

  // register note as a grace notes group
  note->
    setNoteIsAGraceNote ();

  // register note's grace notes groups upLink
  note->
    setNoteShortcutUpLinkToGraceNotesGroup (this);

  // is this grace note tied?
  if (! note->getNoteTiesList ().empty ()) {
    fGraceNotesGroupIsTied = true;
  }

  // is this grace note slurred?
  if (! note->getNoteSlursList ().empty ()) {
    fGraceNotesGroupIsSlurred = true;
  }

//   if (note->getNoteKind () == msrNoteKind::kNoteSkipInGraceNotesGroup) abort ();
}

void msrGraceNotesGroup::appendChordToGraceNotesGroup (const S_msrChord& chord)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceGraceNotes ()) {
    std::stringstream ss;

    ss <<
      "Appending chord " <<
      chord->asShortString () <<
      " to grace notes group " <<
      asShortString ();
//      " in voice \"" <<
//      fGraceNotesGroupUpLinkToVoice->getVoiceName () <<
//      "\"";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // set the chord kind
  chord->setChordInKind (msrChordInKind::kChordInGraceNotesGroup);

  fGraceNotesGroupElementsList.push_back (chord);
}

// S_msrNote msrGraceNotesGroup::removeLastNoteFromGraceNotesGroup (
//   const mfInputLineNumber& inputLineNumber)
// {
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceGraceNotes ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Removing last note from grace notes group " <<
//       asShortString ();
// //      " in voice \"" <<
// //      fGraceNotesGroupUpLinkToVoice->getVoiceName () <<
// //      "\"";
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
// #ifdef MF_SANITY_CHECKS_ARE_ENABLED
//   // sanity check
//   mfAssert (
//     __FILE__, mfInputLineNumber (__LINE__),
//     ! fGraceNotesGroupElementsList.empty (),
//     "fGraceNotesGroupElementsList.empty ()");
// #endif // MF_SANITY_CHECKS_ARE_ENABLED
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceGraceNotes ()) {
//     std::stringstream ss;
//
//     ss <<
//       "Removing last note from grace notes '" <<
//       asString () <<
//       "'";
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   S_msrNote result;
//
//   if (
//     S_msrNote note = dynamic_cast<msrNote*>(&(*fGraceNotesGroupElementsList.back ()))
//     ) {
//     result = note;
//   }
//
//   else {
//     msrInternalError (
//       gServiceRunData->getInputSourceName (),
//       fInputLineNumber,
//       __FILE__, mfInputLineNumber (__LINE__),
//       "removeLastNoteFromGraceNotesGroup (): grace notes group element should be a note");
//   }
//
// #ifdef MF_TRACE_IS_ENABLED
//   if (gTraceOahGroup->getTraceGraceNotes ()) {
//     std::stringstream ss;
//
//     ss <<
//       "This last note from grace notes '" <<
//       asString () <<
//       "' turns out to be '" <<
//       result->asShortString () <<
//       "'";
//
//     gWaeHandler->waeTrace (
//       __FILE__, mfInputLineNumber (__LINE__),
//       ss.str ());
//   }
// #endif // MF_TRACE_IS_ENABLED
//
//   fGraceNotesGroupElementsList.pop_back ();
//
//   return result;
// }

void msrGraceNotesGroup::setGraceNotesGroupElementsPositionInMeasures (
  const S_msrMeasure&        measure,
  const mfPositionInMeasure& positionInMeasure)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTracePositionInMeasures ()) {
    std::stringstream ss;

    ss <<
      "Setting grace notes group elements' measure positions of " << asString () <<
      " to " <<
      positionInMeasure.asString ();

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

/* JMI
  std::string context =
    "setChordMembersPositionInMeasure()";

  setMeasureElementPositionInMeasure (
    positionInMeasure,
    context);

  if (false) { // JMI
  // compute chord's voice position
  mfWholeNotes
     voicePosition =
      fMeasureElementUpLinkToMeasure->getMeasureVoicePosition ()
        +
      positionInMeasure;

  // set chord's voice position
  setMeasureElementVoicePosition (
    voicePosition,
    context);

  // update current voice position
  S_msrVoice
    voice =
      measure->
        fetchMeasureUpLinkToVoice ();

  voice->
    incrementCurrentVoicePosition (
      fChordNotes.at (0)->getMeasureElementSoundingWholeNotes ());
}
*/

  // set the grace notes group's elements' measure position
  if (! fGraceNotesGroupElementsList.empty ()) {
  /* JMI
  if (false) { // JMI
    // compute chord's voice position
    mfWholeNotes
       voicePosition =
        fGraceNotesGroupUpLinkToNote->
          getMeasureElementUpLinkToMeasure ()->
            getMeasureVoicePosition ()
          +
        positionInMeasure;
}
*/

    std::string context =
      "setGraceNotesGroupElementsPositionInMeasures()";

    for (S_msrMeasureElement measureElement : fGraceNotesGroupElementsList) {
      // set measure element's measure position
      measureElement->
        setMeasureElementPositionInMeasure (
          measure,
          positionInMeasure,
          context);

/* JMI
      // set measure element's measure position
  if (false) { // JMI
      measureElement->
        setMeasureElementVoicePosition (
          voicePosition,
          context);
}
*/
    } // for
  }
}

void msrGraceNotesGroup::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrGraceNotesGroup::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrGraceNotesGroup>*
    p =
      dynamic_cast<visitor<S_msrGraceNotesGroup>*> (v)) {
        S_msrGraceNotesGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrGraceNotesGroup::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrGraceNotesGroup::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrGraceNotesGroup::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrGraceNotesGroup>*
    p =
      dynamic_cast<visitor<S_msrGraceNotesGroup>*> (v)) {
        S_msrGraceNotesGroup elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrGraceNotesGroup::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrGraceNotesGroup::browseData (basevisitor* v)
{
  std::list <S_msrMeasureElement>::const_iterator i;

  for (
    i = fGraceNotesGroupElementsList.begin ();
    i != fGraceNotesGroupElementsList.end ();
    ++i
  ) {
    // browse the element (note or chord)
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for
}

std::string msrGraceNotesGroup::asShortString () const
{
  std::stringstream ss;

  ss <<
    "[GraceNotesGroup" <<
    ", fGraceNotesGroupKind: " <<
    fGraceNotesGroupKind <<
    ", fGraceNotesGroupMeasureNumber: \"" <<
    fGraceNotesGroupMeasureNumber <<
    "\", ";

  if (! fGraceNotesGroupElementsList.empty ()) {
    std::list <S_msrMeasureElement>::const_iterator
      iBegin = fGraceNotesGroupElementsList.begin (),
      iEnd   = fGraceNotesGroupElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i)->asShortString ();
      if (++i == iEnd) break;
      ss << ", ";
    } // for
  }

  ss <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

std::string msrGraceNotesGroup::asString () const
{
  std::stringstream ss;

  ss <<
    "[GraceNotesGroup" <<
    ", fGraceNotesGroupKind: " <<
    fGraceNotesGroupKind <<

    ", fGraceNotesGroupIsSlashed" << ": " <<
    fGraceNotesGroupIsSlashed <<
    ", fGraceNotesGroupIsBeamed" << ": " <<
    fGraceNotesGroupIsBeamed <<

    ", fGraceNotesGroupIsTied" << ": " <<
    fGraceNotesGroupIsTied <<
    ", fGraceNotesGroupIsSlurred" << ": " <<
    fGraceNotesGroupIsSlurred <<

    ", fGraceNotesGroupIsFollowedByNotes" << ": " <<
    fGraceNotesGroupIsFollowedByNotes <<

    ", fGraceNotesGroupMeasureNumber: \"" <<
    fGraceNotesGroupMeasureNumber <<
    "\", line " << fInputLineNumber <<
    ", ";

  if (! fGraceNotesGroupElementsList.empty ()) {
    std::list <S_msrMeasureElement>::const_iterator
      iBegin = fGraceNotesGroupElementsList.begin (),
      iEnd   = fGraceNotesGroupElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i)->asString ();
      if (++i == iEnd) break;
      ss << ", ";
    } // for
  }

  ss <<
    "fGraceNotesGroupElementsList: ";
  if (! fGraceNotesGroupElementsList.empty ()) {
    ss << '[';

    std::list <S_msrMeasureElement>::const_iterator
      iBegin = fGraceNotesGroupElementsList.begin (),
      iEnd   = fGraceNotesGroupElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      ss << (*i);
      if (++i == iEnd) break;
      ss << ", ";
    } // for
    ss << ']';

  }
  else {
    ss << "[EMPTY]";
  }

  ss << ']';

  return ss.str ();
}

void msrGraceNotesGroup::printFull (std::ostream& os) const
{
  os <<
    "[GraceNotesGroup FULL" <<
    ", fGraceNotesGroupKind: " <<
    fGraceNotesGroupKind <<
    ", line " << fInputLineNumber <<
    ", " <<
    mfSingularOrPlural (
      fGraceNotesGroupElementsList.size (), "element", "elements") <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 33;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fGraceNotesGroupUpLinkToNote";
  if (fGraceNotesGroupUpLinkToNote) {
    os << std::endl;
    ++gIndenter;
    os <<
      fGraceNotesGroupUpLinkToNote;
    --gIndenter;
  }
  else {
    os <<
      ": [NULL]" <<
      std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fGraceNotesGroupIsSlashed" << ": " <<
    fGraceNotesGroupIsSlashed <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fGraceNotesGroupIsBeamed" << ": " <<
    fGraceNotesGroupIsBeamed <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fGraceNotesGroupIsTied" << ": " <<
    fGraceNotesGroupIsTied <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fGraceNotesGroupIsSlurred" << ": " <<
    fGraceNotesGroupIsSlurred <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fGraceNotesGroupIsFollowedByNotes" << ": " <<
    fGraceNotesGroupIsFollowedByNotes <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fGraceNotesGroupMeasureNumber" << ": " <<
    fGraceNotesGroupMeasureNumber <<
    std::endl;

  os << std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fGraceNotesGroupElementsList:";
  if (! fGraceNotesGroupElementsList.empty ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrMeasureElement>::const_iterator
      iBegin = fGraceNotesGroupElementsList.begin (),
      iEnd   = fGraceNotesGroupElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      (*i)->printFull (os);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
       ": " <<
       "[EMPTY]" <<
      std::endl;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrGraceNotesGroup::print (std::ostream& os) const
{
  os <<
    "[GraceNotesGroup" <<
    ", fGraceNotesGroupKind: " <<
    fGraceNotesGroupKind <<
    ", " <<
    mfSingularOrPlural (
      fGraceNotesGroupElementsList.size (), "element", "elements") <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 34;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fGraceNotesGroupUpLinkToNote" << ": ";
  if (fGraceNotesGroupUpLinkToNote) {
    os << std::endl;
    ++gIndenter;
    os <<
      fGraceNotesGroupUpLinkToNote <<
      std::endl;
    --gIndenter;
  }
  else {
    os << ": [NULL]";
  }
  os << std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fGraceNotesGroupIsSlashed" << ": " <<
    fGraceNotesGroupIsSlashed <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fGraceNotesGroupIsBeamed" << ": " <<
    fGraceNotesGroupIsBeamed <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fGraceNotesGroupIsTied" << ": " <<
    fGraceNotesGroupIsTied <<
    std::endl <<
    std::setw (fieldWidth) <<
    "fGraceNotesGroupIsSlurred" << ": " <<
    fGraceNotesGroupIsSlurred <<
    std::endl;

/*
  os <<
    std::setw (fieldWidth) <<
    "fGraceNotesGroupIsFollowedByNotes" << ": " <<
    fGraceNotesGroupIsFollowedByNotes <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fGraceNotesGroupMeasureNumber" << ": " <<
    fGraceNotesGroupMeasureNumber <<
    std::endl;
*/

  os <<
    std::setw (fieldWidth) <<
    "fGraceNotesGroupElementsList" <<  ": ";
  if (! fGraceNotesGroupElementsList.empty ()) {
    os << std::endl;

    ++gIndenter;

    std::list <S_msrMeasureElement>::const_iterator
      iBegin = fGraceNotesGroupElementsList.begin (),
      iEnd   = fGraceNotesGroupElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
       ": " <<
       "[EMPTY]" <<
      std::endl;
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrGraceNotesGroup& elt)
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
