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

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msrWae.h"

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif

#include "msrGraceNotesGroups.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrGraceNotesGroup msrGraceNotesGroup::create (
  int                    inputLineNumber,
  msrGraceNotesGroupKind graceNotesGroupKind,
  Bool                   graceNotesGroupIsSlashed,
  Bool                   graceNotesGroupIsBeamed,
  const std::string&     graceNotesGroupMeasureNumber)
{
  msrGraceNotesGroup* o =
    new msrGraceNotesGroup (
      inputLineNumber,
      graceNotesGroupKind,
      graceNotesGroupIsSlashed,
      graceNotesGroupIsBeamed,
      graceNotesGroupMeasureNumber);
  assert (o != nullptr);
  return o;
}

msrGraceNotesGroup::msrGraceNotesGroup (
  int                    inputLineNumber,
  msrGraceNotesGroupKind graceNotesGroupKind,
  Bool                   graceNotesGroupIsSlashed,
  Bool                   graceNotesGroupIsBeamed,
  const std::string&     graceNotesGroupMeasureNumber)
    : msrElement (inputLineNumber)
{

  fGraceNotesGroupKind = graceNotesGroupKind;

  fGraceNotesGroupIsTied = false;

  fGraceNotesGroupIsSlashed = graceNotesGroupIsSlashed;
  fGraceNotesGroupIsBeamed = graceNotesGroupIsBeamed;

  fGraceNotesGroupMeasureNumber = graceNotesGroupMeasureNumber;

  // grace notes are followed by notes
  // unless they are last in a measure
  fGraceNotesGroupIsFollowedByNotes = true;
}

msrGraceNotesGroup::~msrGraceNotesGroup ()
{}

S_msrGraceNotesGroup msrGraceNotesGroup::createGraceNotesGroupNewbornClone ()
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Creating a newborn clone of grace notes group '" <<
      asShortString () <<
      "'" <<
      std::endl;
  }
#endif

  S_msrGraceNotesGroup
    newbornClone =
      msrGraceNotesGroup::create (
        fInputLineNumber,
        fGraceNotesGroupKind,
        fGraceNotesGroupIsSlashed,
        fGraceNotesGroupIsBeamed,
        fGraceNotesGroupMeasureNumber);

  newbornClone->fGraceNotesGroupIsTied =
    fGraceNotesGroupIsTied;

  newbornClone->fGraceNotesGroupIsFollowedByNotes =
    fGraceNotesGroupIsFollowedByNotes;

  return newbornClone;
}

S_msrTuplet msrGraceNotesGroup::fetchGraceNoteGroupUpLinkToTuplet () const
{
  S_msrTuplet result;

  // JMI ???

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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Creating a skip clone of grace notes group '" <<
      asShortString () <<
      "'" <<
      std::endl;
  }
#endif

  // create the grace notes group
  S_msrGraceNotesGroup
    clone =
      msrGraceNotesGroup::create (
        fInputLineNumber,
        fGraceNotesGroupKind,
        fGraceNotesGroupIsSlashed,
        fGraceNotesGroupIsBeamed,
        fGraceNotesGroupMeasureNumber);

  clone->fGraceNotesGroupIsTied =
    fGraceNotesGroupIsTied;

  clone->fGraceNotesGroupIsFollowedByNotes =
    fGraceNotesGroupIsFollowedByNotes;

  // populating the clone with skips
  for (
    std::list<S_msrMeasureElement>::const_iterator i=fGraceNotesGroupElementsList.begin ();
    i!=fGraceNotesGroupElementsList.end ();
    ++i
  ) {
    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*(*i)))
    ) {
      // create grace skip note with same duration as note
#ifdef OAH_TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
        gLogStream <<
          "Creating a skip grace note" <<
          ", soundingWholeNotes: " << note->getMeasureElementSoundingWholeNotes () <<
          ", displayWholeNotes: " << note->getNoteDisplayWholeNotes () <<
          std::endl;
      }
#endif

      S_msrNote
        skip =
          msrNote::createGraceSkipNote (
            note->getInputLineNumber (),
            note->getNoteUpLinkToMeasure ()->getMeasureNumber (),
            note->getMeasureElementSoundingWholeNotes (), // 0/1 JMI v0.9.66
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
            chordFirstNote->getNoteUpLinkToMeasure ()->getMeasureNumber (),
            chordFirstNote->getMeasureElementSoundingWholeNotes (), // 0/1 JMI v0.9.66
            chordFirstNote->getNoteDisplayWholeNotes (),
            chordFirstNote->getNoteDotsNumber ());

      // append it to the grace notes
      clone->
        appendNoteToGraceNotesGroup (skip);
    }

    else {
      msrInternalError (
        gGlobalServiceRunData->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "grace notes element should be a note or a chord");
    }
  } // for

  return clone;
}

void msrGraceNotesGroup::appendNoteToGraceNotesGroup (const S_msrNote& note)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Appending note " <<
      note->asShortString () <<
      " to grace notes group" <<
      asShortString () <<
//      " in voice \"" <<
//      fGraceNotesGroupUpLinkToVoice->getVoiceName () <<
//      "\"" <<
      std::endl;
  }
#endif

  fGraceNotesGroupElementsList.push_back (note);

  // register note's grace notes groups upLink
  note->
    setNoteShortcutUpLinkToGraceNotesGroup (this);

  // is this grace note tied?
  if (note->getNoteTie ()) {
    fGraceNotesGroupIsTied = true;
  }
}

void msrGraceNotesGroup::appendChordToGraceNotesGroup (const S_msrChord& chord)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Appending chord " <<
      chord->asShortString () <<
      " to grace notes group '" <<
      asShortString () <<
//      "' in voice \"" <<
//      fGraceNotesGroupUpLinkToVoice->getVoiceName () <<
//      "\"" <<
      std::endl;
  }
#endif

  // set the chord kind
  chord->setChordKind (msrChordInKind::kChordInGraceNotesGroup);

  fGraceNotesGroupElementsList.push_back (chord);
}

S_msrNote msrGraceNotesGroup::removeLastNoteFromGraceNotesGroup (
  int inputLineNumber)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Removing last note from grace notes group " <<
      asShortString () <<
//      " in voice \"" <<
//      fGraceNotesGroupUpLinkToVoice->getVoiceName () <<
//      "\"" <<
      std::endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fGraceNotesGroupElementsList.size () != 0,
    "fGraceNotesGroupElementsList.size () == 0");

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Removing last note from grace notes '" <<
      asString () <<
      "'" <<
      std::endl;
  }
#endif

  S_msrNote result;

  if (
    S_msrNote note = dynamic_cast<msrNote*>(&(*fGraceNotesGroupElementsList.back ()))
    ) {
    result = note;
  }

  else {
    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      "removeLastNoteFromGraceNotesGroup (): grace notes group element should be a note");
  }

#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "This last note from grace notes '" <<
      asString () <<
      "' turns out to be '" <<
      result->asShortString () <<
      "'" <<
      std::endl;
  }
#endif

  fGraceNotesGroupElementsList.pop_back ();

  return result;
}

void msrGraceNotesGroup::setGraceNotesGroupElementsMeasurePositions (
  const S_msrMeasure& measure,
  const Rational&     measurePosition)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Setting grace notes group elements' measure positions of " << asString () <<
      " to '" <<
      measurePosition <<
      "'" <<
      std::endl;
  }
#endif

/* JMI
  std::string context =
    "setChordMembersMeasurePosition()";

  setMeasureElementMeasurePosition (
    measurePosition,
    context);

  if (false) { // JMI
  // compute chord's voice position
  Rational
     voicePosition =
      fMeasureElementUpLinkToMeasure->getMeasureVoicePosition ()
        +
      measurePosition;

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
      fChordNotesVector [0]->getMeasureElementSoundingWholeNotes ());
}
*/

  // set the grace notes group's elements' measure position
  if (fGraceNotesGroupElementsList.size ()) {
  /* JMI
  if (false) { // JMI
    // compute chord's voice position
    Rational
       voicePosition =
        fGraceNotesGroupUpLinkToNote->
          getMeasureElementUpLinkToMeasure ()->
            getMeasureVoicePosition ()
          +
        measurePosition;
}
*/

    std::string context =
      "setGraceNotesGroupElementsMeasurePositions()";

    for (S_msrMeasureElement measureElement : fGraceNotesGroupElementsList) {
      // set measure element's measure position
      measureElement->
        setMeasureElementMeasurePosition (
          measure,
          measurePosition,
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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrGraceNotesGroup::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrGraceNotesGroup>*
    p =
      dynamic_cast<visitor<S_msrGraceNotesGroup>*> (v)) {
        S_msrGraceNotesGroup elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrGraceNotesGroup::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrGraceNotesGroup::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrGraceNotesGroup::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrGraceNotesGroup>*
    p =
      dynamic_cast<visitor<S_msrGraceNotesGroup>*> (v)) {
        S_msrGraceNotesGroup elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrGraceNotesGroup::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrGraceNotesGroup::browseData (basevisitor* v)
{
  std::list<S_msrMeasureElement>::const_iterator i;

  for (
    i=fGraceNotesGroupElementsList.begin ();
    i!=fGraceNotesGroupElementsList.end ();
    ++i
  ) {
    // browse the element (note or chord)
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for
}

std::string msrGraceNotesGroup::asShortString () const
{
  std::stringstream s;

  s <<
    "[GraceNotesGroup" <<
    ", graceNotesGroupKind: " <<
    msrGraceNotesGroupKindAsString (fGraceNotesGroupKind) <<
    ", graceNotesGroupMeasureNumber: \"" << fGraceNotesGroupMeasureNumber <<
    "\", ";

  if (fGraceNotesGroupElementsList.size ()) {
    std::list<S_msrMeasureElement>::const_iterator
      iBegin = fGraceNotesGroupElementsList.begin (),
      iEnd   = fGraceNotesGroupElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i)->asShortString ();
      if (++i == iEnd) break;
      s << ", ";
    } // for
  }

  s <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

std::string msrGraceNotesGroup::asString () const
{
  std::stringstream s;

  s <<
    "[GraceNotesGroup" <<
    ", graceNotesGroupKind: " <<
    msrGraceNotesGroupKindAsString (fGraceNotesGroupKind) <<
    ", graceNotesGroupMeasureNumber: \"" << fGraceNotesGroupMeasureNumber <<
    "\", line " << fInputLineNumber <<
    ", ";

  if (fGraceNotesGroupElementsList.size ()) {
    std::list<S_msrMeasureElement>::const_iterator
      iBegin = fGraceNotesGroupElementsList.begin (),
      iEnd   = fGraceNotesGroupElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i)->asString ();
      if (++i == iEnd) break;
      s << ", ";
    } // for
  }

  s << ']';

  return s.str ();
}

void msrGraceNotesGroup::printFull (std::ostream& os) const
{
  os <<
    "[GraceNotesGroup" <<
    ", graceNotesGroupKind: " <<
    msrGraceNotesGroupKindAsString (fGraceNotesGroupKind) <<
    ", line " << fInputLineNumber <<
    ", " <<
    mfSingularOrPlural (
      fGraceNotesGroupElementsList.size (), "element", "elements") <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os << std::left <<
    std::setw (fieldWidth) <<
    "upLinkToGraceNotesGroupToNote" << ": ";
    if (fGraceNotesGroupUpLinkToNote) {
      os <<
        fGraceNotesGroupUpLinkToNote->asShortString ();
    }
    else {
      os <<
        "[NONE]";
    }
  os << std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fGraceNotesGroupIsSlashed" << ": " <<
    fGraceNotesGroupIsSlashed <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fGraceNotesGroupIsTied" << ": " <<
    fGraceNotesGroupIsTied <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fGraceNotesGroupIsBeamed" << ": " <<
    fGraceNotesGroupIsBeamed <<
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
    "graceNotesGroupElementsList:";
  if (fGraceNotesGroupElementsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrMeasureElement>::const_iterator
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
       "[NONE]" <<
      std::endl;
  }

  --gIndenter;

  os << ']' << std::endl;
}

void msrGraceNotesGroup::print (std::ostream& os) const
{
  os <<
    "[GraceNotesGroup" <<
    ", graceNotesGroupKind: " <<
    msrGraceNotesGroupKindAsString (fGraceNotesGroupKind) <<
    ", line " << fInputLineNumber <<
    ", " <<
    mfSingularOrPlural (
      fGraceNotesGroupElementsList.size (), "element", "elements") <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os <<
    std::setw (fieldWidth) <<
    "upLinkToGraceNotesGroupToNote" << ": ";
    if (fGraceNotesGroupUpLinkToNote) {
      os <<
        fGraceNotesGroupUpLinkToNote->asShortString ();
    }
    else {
      os <<
        "[NONE]";
    }
  os << std::endl;

  os <<
    std::setw (fieldWidth) <<
    "fGraceNotesGroupIsSlashed" << ": " <<
    fGraceNotesGroupIsSlashed <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fGraceNotesGroupIsTied" << ": " <<
    fGraceNotesGroupIsTied <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fGraceNotesGroupIsBeamed" << ": " <<
    fGraceNotesGroupIsBeamed <<
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
    "graceNotesGroupElementsList";
  if (fGraceNotesGroupElementsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrMeasureElement>::const_iterator
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

  os << ']' << std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_msrGraceNotesGroup& elt)
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
