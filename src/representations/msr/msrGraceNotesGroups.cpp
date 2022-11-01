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

#include "msrGraceNotesGroups.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrGraceNotesGroup msrGraceNotesGroup::create (
  int                    inputLineNumber,
  msrGraceNotesGroupKind graceNotesGroupKind,
  Bool                   graceNotesGroupIsSlashed,
  Bool                   graceNotesGroupIsBeamed,
  const string&          graceNotesGroupMeasureNumber)
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
  const string&          graceNotesGroupMeasureNumber)
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Creating a newborn clone of grace notes group '" <<
      asShortString () <<
      "'" <<
      endl;
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

string graceNotesGroupKindAsString (
  msrGraceNotesGroupKind graceNotesGroupKind)
{
  string result;

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

S_msrGraceNotesGroup msrGraceNotesGroup::createSkipGraceNotesGroupClone ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Creating a skip clone of grace notes group '" <<
      asShortString () <<
      "'" <<
      endl;
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
    list<S_msrMeasureElement>::const_iterator i=fGraceNotesGroupElementsList.begin ();
    i!=fGraceNotesGroupElementsList.end ();
    ++i
  ) {
    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*(*i)))
    ) {
      // create grace skip note with same duration as note
#ifdef TRACING_IS_ENABLED
      if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
        gLogStream <<
          "Creating a skip grace note" <<
          ", soundingWholeNotes: " << note->getMeasureElementSoundingWholeNotes () <<
          ", displayWholeNotes: " << note->getNoteDisplayWholeNotes () <<
          endl;
      }
#endif

      S_msrNote
        skip =
          msrNote::createGraceSkipNote (
            note->getInputLineNumber (),
            note->fetchMeasureElementMeasureNumber (),
            note->getMeasureElementSoundingWholeNotes (), // 0/1 JMI
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
            chordFirstNote->fetchMeasureElementMeasureNumber (),
            chordFirstNote->getMeasureElementSoundingWholeNotes (), // 0/1 JMI
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

void msrGraceNotesGroup::appendNoteToGraceNotesGroup (S_msrNote note)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Appending note " <<
      note->asShortString () <<
      " to grace notes group" <<
      asShortString () <<
//      " in voice \"" <<
//      fGraceNotesGroupUpLinkToVoice->getVoiceName () <<
//      "\"" <<
      endl;
  }
#endif

  fGraceNotesGroupElementsList.push_back (note);

  // register note's grace notes groups upLink
  note->
    setNoteDirectUpLinkToGraceNotesGroup (this);

  // is this grace note tied?
  if (note->getNoteTie ()) {
    fGraceNotesGroupIsTied = true;
  }
}

void msrGraceNotesGroup::appendChordToGraceNotesGroup (S_msrChord chord)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Appending chord " <<
      chord->asShortString () <<
      " to grace notes group '" <<
      asShortString () <<
//      "' in voice \"" <<
//      fGraceNotesGroupUpLinkToVoice->getVoiceName () <<
//      "\"" <<
      endl;
  }
#endif

  // set the chord kind
  chord->setChordKind (msrChordInKind::kChordInGraceNotesGroup);

  fGraceNotesGroupElementsList.push_back (chord);
}

S_msrNote msrGraceNotesGroup::removeLastNoteFromGraceNotesGroup (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Removing last note from grace notes group " <<
      asShortString () <<
//      " in voice \"" <<
//      fGraceNotesGroupUpLinkToVoice->getVoiceName () <<
//      "\"" <<
      endl;
  }
#endif

  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    fGraceNotesGroupElementsList.size () != 0,
    "fGraceNotesGroupElementsList.size () == 0");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Removing last note from grace notes '" <<
      asString () <<
      "'" <<
      endl;
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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "This last note from grace notes '" <<
      asString () <<
      "' turns out to be '" <<
      result->asShortString () <<
      "'" <<
      endl;
  }
#endif

  fGraceNotesGroupElementsList.pop_back ();

  return result;
}

void msrGraceNotesGroup::setGraceNotesGroupElementsMeasurePositions (
  S_msrMeasure        measure,
  const Rational&     measurePosition)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMeasurePositions ()) {
    gLogStream <<
      "Setting grace notes group elements' positions in measure of " << asString () <<
      " to '" <<
      measurePosition <<
      "'" <<
      endl;
  }
#endif

/* JMI
  string context =
    "setChordMembersMeasurePosition()";

  setMeasureElementMeasurePosition (
    measurePosition,
    context);

  if (false) { // JMI
  // compute chord's position in voice
  Rational
     voicePosition =
      fMeasureElementUpLinkToMeasure->getMeasureVoicePosition ()
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
      fChordNotesVector [0]->getMeasureElementSoundingWholeNotes ());
}
*/

  // set the grace notes group's elements' position in measure
  if (fGraceNotesGroupElementsList.size ()) {
  /* JMI
  if (false) { // JMI
    // compute chord's position in voice
    Rational
       voicePosition =
        fGraceNotesGroupUpLinkToNote->
          getMeasureElementUpLinkToMeasure ()->
            getMeasureVoicePosition ()
          +
        measurePosition;
}
*/

    string context =
      "setGraceNotesGroupElementsMeasurePositions()";

    for (S_msrMeasureElement measureElement : fGraceNotesGroupElementsList) {
      // set measure element's position in measure
      measureElement->
        setMeasureElementMeasurePosition (
          measure,
          measurePosition,
          context);

/* JMI
      // set measure element's position in measure
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
      endl;
  }

  if (visitor<S_msrGraceNotesGroup>*
    p =
      dynamic_cast<visitor<S_msrGraceNotesGroup>*> (v)) {
        S_msrGraceNotesGroup elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrGraceNotesGroup::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrGraceNotesGroup::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrGraceNotesGroup::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrGraceNotesGroup>*
    p =
      dynamic_cast<visitor<S_msrGraceNotesGroup>*> (v)) {
        S_msrGraceNotesGroup elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrGraceNotesGroup::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrGraceNotesGroup::browseData (basevisitor* v)
{
  list<S_msrMeasureElement>::const_iterator i;

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

string msrGraceNotesGroup::asShortString () const
{
  stringstream s;

  s <<
    "[GraceNotesGroup" <<
    ", graceNotesGroupKind: " <<
    graceNotesGroupKindAsString (fGraceNotesGroupKind) <<
    ", graceNotesGroupMeasureNumber: \"" << fGraceNotesGroupMeasureNumber <<
    "\", ";

  if (fGraceNotesGroupElementsList.size ()) {
    list<S_msrMeasureElement>::const_iterator
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

string msrGraceNotesGroup::asString () const
{
  stringstream s;

  s <<
    "[GraceNotesGroup" <<
    ", graceNotesGroupKind: " <<
    graceNotesGroupKindAsString (fGraceNotesGroupKind) <<
    ", graceNotesGroupMeasureNumber: \"" << fGraceNotesGroupMeasureNumber <<
    "\", line " << fInputLineNumber <<
    ", ";

  if (fGraceNotesGroupElementsList.size ()) {
    list<S_msrMeasureElement>::const_iterator
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

void msrGraceNotesGroup::print (ostream& os) const
{
  os <<
    "[GraceNotesGroup" <<
    ", graceNotesGroupKind: " <<
    graceNotesGroupKindAsString (fGraceNotesGroupKind) <<
    ", line " << fInputLineNumber <<
    ", " <<
    mfSingularOrPlural (
      fGraceNotesGroupElementsList.size (), "element", "elements") <<
    endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os << left <<
    setw (fieldWidth) <<
    "upLinkToGraceNotesGroupToNote" << " : ";
    if (fGraceNotesGroupUpLinkToNote) {
      os <<
        fGraceNotesGroupUpLinkToNote->asShortString ();
    }
    else {
      os <<
        "[NONE]";
    }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "fGraceNotesGroupIsSlashed" << " : " <<
    fGraceNotesGroupIsSlashed <<
    endl <<

    setw (fieldWidth) <<
    "fGraceNotesGroupIsTied" << " : " <<
    fGraceNotesGroupIsTied <<
    endl <<

    setw (fieldWidth) <<
    "fGraceNotesGroupIsBeamed" << " : " <<
    fGraceNotesGroupIsBeamed <<
    endl <<

    setw (fieldWidth) <<
    "fGraceNotesGroupIsFollowedByNotes" << " : " <<
    fGraceNotesGroupIsFollowedByNotes <<
    endl <<

    setw (fieldWidth) <<
    "fGraceNotesGroupMeasureNumber" << " : " <<
    fGraceNotesGroupMeasureNumber <<
    endl;

  os << endl;

  os <<
    setw (fieldWidth) <<
    "graceNotesGroupElementsList:";
  if (fGraceNotesGroupElementsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrMeasureElement>::const_iterator
      iBegin = fGraceNotesGroupElementsList.begin (),
      iEnd   = fGraceNotesGroupElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
       " : " <<
       "[NONE]" <<
      endl;
  }

  --gIndenter;

  os << ']' << endl;
}

void msrGraceNotesGroup::printShort (ostream& os) const
{
  os <<
    "GraceNotesGroup" <<
    ", graceNotesGroupKind: " <<
    graceNotesGroupKindAsString (fGraceNotesGroupKind) <<
    ", line " << fInputLineNumber <<
    ", " <<
    mfSingularOrPlural (
      fGraceNotesGroupElementsList.size (), "element", "elements") <<
    endl;

  ++gIndenter;

  const int fieldWidth = 33;

  os <<
    setw (fieldWidth) <<
    "upLinkToGraceNotesGroupToNote" << " : ";
    if (fGraceNotesGroupUpLinkToNote) {
      os <<
        fGraceNotesGroupUpLinkToNote->asShortString ();
    }
    else {
      os <<
        "[NONE]";
    }
  os << endl;

  os <<
    setw (fieldWidth) <<
    "fGraceNotesGroupIsSlashed" << " : " <<
    fGraceNotesGroupIsSlashed <<
    endl <<

    setw (fieldWidth) <<
    "fGraceNotesGroupIsTied" << " : " <<
    fGraceNotesGroupIsTied <<
    endl <<

    setw (fieldWidth) <<
    "fGraceNotesGroupIsBeamed" << " : " <<
    fGraceNotesGroupIsBeamed <<
    endl;

/*
  os <<
    setw (fieldWidth) <<
    "fGraceNotesGroupIsFollowedByNotes" << " : " <<
    fGraceNotesGroupIsFollowedByNotes <<
    endl <<

    setw (fieldWidth) <<
    "fGraceNotesGroupMeasureNumber" << " : " <<
    fGraceNotesGroupMeasureNumber <<
    endl;
*/

  os <<
    setw (fieldWidth) <<
    "graceNotesGroupElementsList";
  if (fGraceNotesGroupElementsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrMeasureElement>::const_iterator
      iBegin = fGraceNotesGroupElementsList.begin (),
      iEnd   = fGraceNotesGroupElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
       " : " <<
       "[EMPTY]" <<
      endl;
  }

  --gIndenter;
}

ostream& operator << (ostream& os, const S_msrGraceNotesGroup& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}


}
