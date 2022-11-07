/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // setw, setprecision, ...

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"

#include "mfServiceRunData.h"

#include "mfStringsHandling.h"

#include "msrWae.h"

#include "msrTempos.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_msrTempoNote msrTempoNote::create (
  int             inputLineNumber,
  const Rational& tempoNoteWholeNotes,
  Bool            tempoNoteBelongsToATuplet)
{
  msrTempoNote * o =
    new msrTempoNote (
      inputLineNumber,
      tempoNoteWholeNotes,
      tempoNoteBelongsToATuplet);
  assert (o != nullptr);

  return o;
}

msrTempoNote::msrTempoNote (
  int             inputLineNumber,
  const Rational& tempoNoteWholeNotes,
  Bool            tempoNoteBelongsToATuplet)
    : msrElement (inputLineNumber)
{
  fTempoNoteWholeNotes = tempoNoteWholeNotes;

  fTempoNoteBelongsToATuplet = tempoNoteBelongsToATuplet;
}

msrTempoNote::~msrTempoNote ()
{}

void msrTempoNote::appendBeamToTempoNote (S_msrBeam beam)
{
  fTempoNoteBeams.push_back (beam);
}

void msrTempoNote::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTempoNote::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrTempoNote>*
    p =
      dynamic_cast<visitor<S_msrTempoNote>*> (v)) {
        S_msrTempoNote elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTempoNote::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTempoNote::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTempoNote::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTempoNote>*
    p =
      dynamic_cast<visitor<S_msrTempoNote>*> (v)) {
        S_msrTempoNote elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTempoNote::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTempoNote::browseData (basevisitor* v)
{
  for (
    list<S_msrBeam>::const_iterator i = fTempoNoteBeams.begin ();
    i != fTempoNoteBeams.end ();
    ++i ) {
    // browse tempoTuplet element
    msrBrowser<msrBeam> browser (v);
    browser.browse (*(*i));
  } // for
}

string msrTempoNote::asString () const
{
  stringstream s;

  s <<
    "Tempo note" <<
    ", tempoNoteWholeNotes = " << fTempoNoteWholeNotes <<
    ", tempoNoteBelongsToATuplet = " <<
    fTempoNoteBelongsToATuplet;

  return s.str ();
}

void msrTempoNote::print (ostream& os) const
{
  os <<
    "[TempoNote" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 26;

  os << left <<
    setw (fieldWidth) <<
    "tempoNoteWholeNotes" << " : " << fTempoNoteWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "tempoNoteBelongsToATuplet" << " : " <<
    fTempoNoteBelongsToATuplet <<
    endl <<
    setw (fieldWidth) <<
    "tempoNoteBeams";

    if (fTempoNoteBeams.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrBeam>::const_iterator
        iBegin = fTempoNoteBeams.begin (),
        iEnd   = fTempoNoteBeams.end (),
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

ostream& operator << (ostream& os, const S_msrTempoNote& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrTempoTuplet msrTempoTuplet::create (
  int                          inputLineNumber,
  int                          tempoTupletNumber,
  msrTempoTupletBracketKind    tempoTupletBracketKind,
  msrTempoTupletShowNumberKind tempoTupletShowNumberKind,
  msrTupletFactor              tempoTupletFactor,
  const Rational&              memberNotesDisplayWholeNotes)
{
  msrTempoTuplet* o =
    new msrTempoTuplet (
      inputLineNumber,
      tempoTupletNumber,
      tempoTupletBracketKind,
      tempoTupletShowNumberKind,
      tempoTupletFactor,
      memberNotesDisplayWholeNotes);
  assert (o != nullptr);
  return o;
}

msrTempoTuplet::msrTempoTuplet (
  int                          inputLineNumber,
  int                          tempoTupletNumber,
  msrTempoTupletBracketKind    tempoTupletBracketKind,
  msrTempoTupletShowNumberKind tempoTupletShowNumberKind,
  msrTupletFactor              tempoTupletFactor,
  const Rational&              memberNotesDisplayWholeNotes)
    : msrElement (inputLineNumber)
{
  fTempoTupletNumber = tempoTupletNumber;

  fTempoTupletBracketKind    = tempoTupletBracketKind;
  fTempoTupletShowNumberKind = tempoTupletShowNumberKind;

  fTempoTupletFactor = tempoTupletFactor;

  fMemberNotesDisplayWholeNotes  = memberNotesDisplayWholeNotes;

  fTempoTupletDisplayWholeNotes  = Rational (0, 1);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTempos ()){
    gLogStream <<
      "Creating tempo tuplet '" <<
      this->asString () <<
      "'" <<
      endl;
  }
#endif
}

msrTempoTuplet::~msrTempoTuplet ()
{}

string msrTempoTuplet::msrTempoTupletTypeKindAsString (
  msrTempoTupletTypeKind tempoTupletTypeKind)
{
  string result;

  switch (tempoTupletTypeKind) {
    case msrTempoTupletTypeKind::kTempoTupletTypeNone:
      result = "tempoTupletTypeNone";
      break;
    case msrTempoTupletTypeKind::kTempoTupletTypeStart:
      result = "tempoTupletTypeStart";
      break;
    case msrTempoTupletTypeKind::kTempoTupletTypeStop:
      result = "tempoTupletTypeStop";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrTempoTupletTypeKind& elt)
{
  os << msrTempoTupletTypeKindAsString (elt);
  return os;
}

string msrTempoTuplet::msrTempoTupletBracketKindAsString (
  msrTempoTupletBracketKind tempoTupletBracketKind)
{
  string result;

  switch (tempoTupletBracketKind) {
    case msrTempoTupletBracketKind::kTempoTupletBracketYes:
      result = "tempoTupletBracketYes";
      break;
    case msrTempoTupletBracketKind::kTempoTupletBracketNo:
      result = "tempoTupletBracketNo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrTempoTupletBracketKind& elt)
{
  os << msrTempoTupletBracketKindAsString (elt);
  return os;
}

string msrTempoTupletShowNumberKindAsString (
  msrTempoTupletShowNumberKind tempoTupletShowNumberKind)
{
  string result;

  switch (tempoTupletShowNumberKind) {
    case msrTempoTupletShowNumberKind::kTempoTupletShowNumberActual:
      result = "tempoTupletShowNumberActual";
      break;
    case msrTempoTupletShowNumberKind::kTempoTupletShowNumberBoth:
      result = "tempoTupletShowNumberBoth";
      break;
    case msrTempoTupletShowNumberKind::kTempoTupletShowNumberNone:
      result = "tempoTupletShowNumberNone";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrTempoTupletShowNumberKind& elt)
{
  os << msrTempoTupletShowNumberKindAsString (elt);
  return os;
}

void msrTempoTuplet::addTempoNoteToTempoTuplet (S_msrTempoNote tempoNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTempos ()) {
    gLogStream <<
      "Adding tempoNote '" <<
      tempoNote->asShortString () <<
      // the information is missing to display it the normal way JMI ???
      "' to tempoTuplet '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fTempoTupletElements.push_back (tempoNote);

/* JMI
  // register note's tempoTuplet upLink
  note->
    setNoteTempoUpLinkToTuplet (this);

  fTupletDisplayWholeNotes += // JMI
    note->getNoteDisplayWholeNotes ();

    */
}

/*
void msrTempoTuplet::addTempoTupletToTempoTuplet (S_msrTempoTuplet tempoTuplet)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTempos ()) {
    gLogStream <<
      "Adding tempoTuplet '" <<
      tempoTuplet->asString () <<
      "' to tempoTuplet '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // register tempoTuplet in elements list
  fTempoTupletElements.push_back (tempoTuplet);

  // account for tempoTuplet duration
  fTempoTupletDisplayWholeNotes += // JMI
    tempoTuplet->getTempoTupletDisplayWholeNotes ();

    / *
  fTempoTupletDisplayWholeNotes += // JMI
    tempoTuplet->getTempoTupletDisplayWholeNotes ();
    * /

  // don't populate tempoTuplet's measure number nor position in measure,
  // this will be done in setTupletMembersMeasureNumber ()
  / * JMI
  tempoTuplet->setTempoTupletMeasureNumber (
    fTempoTupletMeasureNumber);

  // populate tempoTuplet's position in measure
  tempoTuplet->setTempoTupletMeasurePosition (
    fTempoTupletMeasurePosition);
    * /
}

void msrTempoTuplet::removeFirstNoteFromTempoTuplet (
  int            inputLineNumber,
  S_msrTempoNote tempoNote)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTempos ()) {
    gLogStream <<
      "Removing first tempoNote '" <<
      tempoNote->asShortString () <<
      "' from tempoTuplet '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  if (fTempoTupletElements.size ()) {
    S_msrElement
      firstTempoTupletElement =
        fTempoTupletElements.front ();

    for (
      list<S_msrElement>::iterator i=fTempoTupletElements.begin ();
      i!=fTempoTupletElements.end ();
      ++i) {
      if ((*i) == tempoNote) {
        // found note, erase it
        i = fTempoTupletElements.erase (i);

        // account for note duration
        fTempoTupletDisplayWholeNotes -= // JMI
          tempoNote->getTempoNoteWholeNotes ();

        // don't update measure number nor position in measure: // JMI
        // they have not been set yet

        // return from function
        return;
      }
    } // for

    stringstream s;

    s <<
      "cannot remove tempoNote " <<
      tempoNote <<
      " from tempoTuplet " << asString () <<
      " since this note has not been found in fTempoTupletElements";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  else {
    stringstream s;

    s <<
      "cannot remove tempoNote " <<
      tempoNote <<
      " from empty tempoTuplet " <<
      " since this note cannot be found in empty fTempoTupletElements";

    msrInternalError (
      gGlobalServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
}
*/

/* JMI
void msrTempoTuplet::applyDisplayFactorToTempoTupletMembers ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTempos ()) {
    gLogStream <<
      "% ==> applyDisplayFactorToTempoTupletMembers ()" <<
      endl;

    ++gIndenter;

    gLogStream <<
      "% fTempoTupletFactor = " <<
      fTempoTupletFactor <<
      endl << endl;

    --gIndenter;
  }
#endif
}

void msrTempoTuplet::unapplySoundingFactorToTempoTupletMembers (
  int containingTempoTupletActualNotes,
  int containingTempoTupletNormalNotes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTempos ()) {
    gLogStream <<
      "unapplySoundingFactorToTempoTupletMembers ()" <<
      endl;

    ++gIndenter;

    gLogStream <<
      "% fTempoTupletFactor = " <<
      fTempoTupletFactor <<
      endl <<
      "% containingTempoTupletActualNotes = " <<
      containingTempoTupletActualNotes <<
      ", containingTempoTupletNormalNotes = " <<
      containingTempoTupletNormalNotes <<
      endl << endl;

    --gIndenter;
  }
#endif

  fTempoTupletFactor /= // JMI ???
    containingTempoTupletActualNotes;
}
*/

void msrTempoTuplet::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTempoTuplet::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrTempoTuplet>*
    p =
      dynamic_cast<visitor<S_msrTempoTuplet>*> (v)) {
        S_msrTempoTuplet elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTempoTuplet::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTempoTuplet::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTempoTuplet::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTempoTuplet>*
    p =
      dynamic_cast<visitor<S_msrTempoTuplet>*> (v)) {
        S_msrTempoTuplet elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTempoTuplet::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTempoTuplet::browseData (basevisitor* v)
{
  for (
    list<S_msrElement>::const_iterator i = fTempoTupletElements.begin ();
    i != fTempoTupletElements.end ();
    ++i ) {
    // browse tempoTuplet element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for
}

string msrTempoTuplet::asString () const
{
  stringstream s;

  s <<
    "TempoTuplet " <<
    fTempoTupletFactor <<
    ' ' << fTempoTupletDisplayWholeNotes << " display whole notes" <<
    ":";

  s << "[[";

  if (fTempoTupletElements.size ()) {
    list<S_msrElement>::const_iterator
      iBegin = fTempoTupletElements.begin (),
      iEnd   = fTempoTupletElements.end (),
      i      = iBegin;
    for ( ; ; ) {

      if (
        S_msrTempoNote note = dynamic_cast<msrTempoNote*>(&(*(*i)))
        ) {
        s <<
          note->asShortString ();
      }

      else if (
        S_msrTempoTuplet tempoTuplet = dynamic_cast<msrTempoTuplet*>(&(*(*i)))
        ) {
        s <<
          tempoTuplet->asString ();
      }

      else {
        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          "tempoTuplet member should be a note, a chord or another tempoTuplet");
      }

      if (++i == iEnd) break;
      s << ' ';

    } // for
  }

  s << "]]";

  return s.str ();
}

/* JMI
string msrTempoTuplet::asString () const
{
  stringstream s;

  s <<
    "TempoTuplet " <<
    fTempoTupletFactor <<
    ' ' << fTempoTupletSoundingWholeNotes << " sound whole notes" <<
    " measure '" <<
    fTempoTupletMeasureNumber <<
    "':";

  if (fTempoTupletMeasurePosition.getNumerator () < 0) {
    s << "?";
  }
  else {
    s << fTempoTupletMeasurePosition;
  }

  s << "[[";

  if (fTempoTupletElements.size ()) {
    list<S_msrElement>::const_iterator
      iBegin = fTempoTupletElements.begin (),
      iEnd   = fTempoTupletElements.end (),
      i      = iBegin;
    for ( ; ; ) {

      if (
        S_msrTempoNote note = dynamic_cast<msrTempoNote*>(&(*(*i)))
        ) {
        s <<
          note->asShortString ();
      }

      else if (
        S_msrChord chord = dynamic_cast<msrChord*>(&(*(*i)))
        ) {
        s <<
          chord->asString ();
      }

      else if (
        S_msrTempoTuplet tempoTuplet = dynamic_cast<msrTempoTuplet*>(&(*(*i)))
        ) {
        s <<
          tempoTuplet->asString ();
      }

      else {
        msrInternalError (
          gGlobalServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          "tempoTuplet member should be a note, a chord or another tempoTuplet");
      }

      if (++i == iEnd) break;
      s << ' ';
    } // for
  }

  s << "]]";

  return s.str ();
}
*/

void msrTempoTuplet::print (ostream& os) const
{
  os <<
    "[TempoTuplet " <<
    fTempoTupletFactor <<
    ", " <<
    mfSingularOrPlural (
      fTempoTupletElements.size (), "element", "elements") <<
    ", display whole notes: " <<
    fTempoTupletDisplayWholeNotes <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 30;

  os << left <<
    setw (fieldWidth) <<
    "TempoTupletBracketKind" << " : " <<
    msrTempoTupletBracketKindAsString (
      fTempoTupletBracketKind) <<
    endl <<
    setw (fieldWidth) <<
    "TempoTupletShowNumberKind" << " : " <<
    fTempoTupletShowNumberKind <<
    endl <<
    setw (fieldWidth) <<
    "MemberNotesDisplayWholeNotes" << " : " <<
    fMemberNotesDisplayWholeNotes <<
    endl << endl;

/* JMI ???
  os << left <<
    setw (fieldWidth) <<
    "(position in measure" << " : ";
  if (fTempoTupletMeasurePosition.getNumerator () < 0) {
    os << "???)";
  }
  else {
    os << fTempoTupletMeasurePosition << ")";
  }
  os << endl;
    */

  os <<
    "fTempoTupletElements";

  if (fTempoTupletElements.size ()) {
    ++gIndenter;

    list<S_msrElement>::const_iterator
      iBegin = fTempoTupletElements.begin (),
      iEnd   = fTempoTupletElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here
    } // for

    --gIndenter;
    }
  else {
    os << left <<
      " : " << "[EMPTY]" <<
      endl;
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrTempoTuplet& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrTempoNotesRelationshipElements msrTempoNotesRelationshipElements::create (
  int      inputLineNumber,
  msrTempoNotesRelationshipElementsKind
           tempoNotesRelationshipElementsKind)
{
  msrTempoNotesRelationshipElements * o =
    new msrTempoNotesRelationshipElements (
      inputLineNumber,
      tempoNotesRelationshipElementsKind);
  assert (o != nullptr);

  return o;
}

msrTempoNotesRelationshipElements::msrTempoNotesRelationshipElements (
  int      inputLineNumber,
  msrTempoNotesRelationshipElementsKind
           tempoNotesRelationshipElementsKind)
    : msrElement (inputLineNumber)
{
  fTempoNotesRelationshipElementsKind = tempoNotesRelationshipElementsKind;
}

msrTempoNotesRelationshipElements::~msrTempoNotesRelationshipElements ()
{}

void msrTempoNotesRelationshipElements::addElementToTempoNotesRelationshipElements (
  S_msrElement element)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceTempos ()){
    gLogStream <<
      "Adding element '" <<
      element->asString () <<
      "' to tempo relationship" <<
      endl;
  }
#endif

  fTempoNotesRelationshipElementsList.push_back (element);
}

void msrTempoNotesRelationshipElements::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTempoNotesRelationshipElements::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrTempoNotesRelationshipElements>*
    p =
      dynamic_cast<visitor<S_msrTempoNotesRelationshipElements>*> (v)) {
        S_msrTempoNotesRelationshipElements elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTempoNotesRelationshipElements::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTempoNotesRelationshipElements::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTempoNotesRelationshipElements::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTempoNotesRelationshipElements>*
    p =
      dynamic_cast<visitor<S_msrTempoNotesRelationshipElements>*> (v)) {
        S_msrTempoNotesRelationshipElements elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTempoNotesRelationshipElements::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTempoNotesRelationshipElements::browseData (basevisitor* v)
{
  // browse the elements list
  if (fTempoNotesRelationshipElementsList.size ()) {
    for (
      list<S_msrElement>::const_iterator i =
        fTempoNotesRelationshipElementsList.begin ();
      i != fTempoNotesRelationshipElementsList.end ();
      ++i
  ) {
      // browse the element
      msrBrowser<msrElement> browser (v);
      browser.browse (*(*i));
    } // for
  }
}

string msrTempoNotesRelationshipElements::asString () const
{
  stringstream s;

  s <<
    "TempoNotesRelationshipElements" <<
    ", fTempoNotesRelationshipElementsKind = " << fTempoNotesRelationshipElementsKind;

  return s.str ();
}

string msrTempoNotesRelationshipElementsKindAsString (
  msrTempoNotesRelationshipElementsKind tempoNotesRelationshipElementsKind)
{
  string result;

  switch (tempoNotesRelationshipElementsKind) {
    case msrTempoNotesRelationshipElementsKind::kTempoNotesRelationshipElementsLeft:
      result = "tempoNotesRelationshipElementsLeft";
      break;
    case msrTempoNotesRelationshipElementsKind::kTempoNotesRelationshipElementsRight:
      result = "tempoNotesRelationshipElementsRight";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrTempoNotesRelationshipElementsKind& elt)
{
  os << msrTempoNotesRelationshipElementsKindAsString (elt);
  return os;
}

void msrTempoNotesRelationshipElements::print (ostream& os) const
{
  os <<
    "[TempoNotesRelationshipElements" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  os <<
    "msrTempoNotesRelationshipElementsKindAsString : " <<
    msrTempoNotesRelationshipElementsKindAsString (
      fTempoNotesRelationshipElementsKind) <<
    endl;

  const int fieldWidth = 26;

  os << left <<
    setw (fieldWidth) <<
    "tempoNotesRelationshipElementsList";

    if (fTempoNotesRelationshipElementsList.size ()) {
      ++gIndenter;

      os << endl;

      list<S_msrElement>::const_iterator
        iBegin = fTempoNotesRelationshipElementsList.begin (),
        iEnd   = fTempoNotesRelationshipElementsList.end (),
        i      = iBegin;

      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // os << endl;
      } // for

      --gIndenter;
  }
  else {
    os <<
      " : " << "[NONE]" <<
      endl;
  }

  --gIndenter;

  os << ']' << endl;
}

ostream& operator << (ostream& os, const S_msrTempoNotesRelationshipElements& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "[NONE]" << endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrTempo msrTempo::createTempoWordsOnly (
  int               inputLineNumber,
  S_msrMeasure      upLinkToMeasure,
  S_msrWords        tempoWords,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
{
  msrTempo* o =
    new msrTempo (
      inputLineNumber,
      upLinkToMeasure,
      tempoWords,
      tempoParenthesizedKind,
      tempoPlacementKind);
  assert (o != nullptr);

  return o;
}

S_msrTempo msrTempo::createTempoPerMinute (
  int               inputLineNumber,
  S_msrMeasure      upLinkToMeasure,
  msrDottedDuration tempoBeatUnit,
  string            tempoPerMinute,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
{
  msrTempo* o =
    new msrTempo (
      inputLineNumber,
      upLinkToMeasure,
      tempoBeatUnit,
      tempoPerMinute,
      tempoParenthesizedKind,
      tempoPlacementKind);
  assert (o != nullptr);
  return o;
}

S_msrTempo msrTempo::createTempoBeatUnitEquivalent (
  int               inputLineNumber,
  S_msrMeasure      upLinkToMeasure,
  msrDottedDuration tempoBeatUnit,
  msrDottedDuration tempoEquivalentBeatUnit,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
{
  msrTempo* o =
    new msrTempo (
      inputLineNumber,
      upLinkToMeasure,
      tempoBeatUnit,
      tempoEquivalentBeatUnit,
      tempoParenthesizedKind,
      tempoPlacementKind);
  assert (o != nullptr);
  return o;
}

S_msrTempo msrTempo::createTempoNotesRelationship (
  int               inputLineNumber,
  S_msrMeasure      upLinkToMeasure,
  S_msrTempoNotesRelationshipElements
                    tempoNotesRelationshipLeftElements,
  msrTempoNotesRelationshipKind
                    tempoNotesRelationshipKind,
  S_msrTempoNotesRelationshipElements
                    tempoNotesRelationshipRightElements,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
{
  msrTempo* o =
    new msrTempo (
      inputLineNumber,
      upLinkToMeasure,
      tempoNotesRelationshipLeftElements,
      tempoNotesRelationshipKind,
      tempoNotesRelationshipRightElements,
      tempoParenthesizedKind,
      tempoPlacementKind);
  assert (o != nullptr);
  return o;
}

msrTempo::msrTempo (
  int               inputLineNumber,
  S_msrMeasure      upLinkToMeasure,
  S_msrWords        tempoWords,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure),
      fTempoBeatUnit (),
      fTempoEquivalentBeatUnit ()
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    tempoWords != nullptr,
    "tempoWords is null");

  fTempoKind = kTempoBeatUnitsWordsOnly;

  fTempoPerMinute = "";

  fTempoNotesRelationshipKind = kTempoNotesRelationshipNone;

  fTempoParenthesizedKind = kTempoParenthesizedNo;

  fTempoPlacementKind = msrPlacementKind::kPlacementAbove;

  fTempoWordsList.push_back (tempoWords);
}

msrTempo::msrTempo (
  int               inputLineNumber,
  S_msrMeasure      upLinkToMeasure,
  msrDottedDuration tempoBeatUnit,
  string            tempoPerMinute,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure),
      fTempoBeatUnit (tempoBeatUnit),
      fTempoEquivalentBeatUnit ()
{
  fTempoKind = kTempoBeatUnitsPerMinute;

  fTempoPerMinute = tempoPerMinute;

  fTempoNotesRelationshipKind = kTempoNotesRelationshipNone;

  fTempoParenthesizedKind = tempoParenthesizedKind;

  fTempoPlacementKind = tempoPlacementKind;
}

msrTempo::msrTempo (
  int               inputLineNumber,
  S_msrMeasure      upLinkToMeasure,
  msrDottedDuration tempoBeatUnit,
  msrDottedDuration tempoEquivalentBeatUnit,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure),
      fTempoBeatUnit (tempoBeatUnit),
      fTempoEquivalentBeatUnit (tempoEquivalentBeatUnit)
{
  fTempoKind = kTempoBeatUnitsEquivalence;

  fTempoPerMinute = "";

  fTempoNotesRelationshipKind = kTempoNotesRelationshipNone;

  fTempoParenthesizedKind = tempoParenthesizedKind;

  fTempoPlacementKind = tempoPlacementKind;
}

msrTempo::msrTempo (
  int               inputLineNumber,
  S_msrMeasure      upLinkToMeasure,
  S_msrTempoNotesRelationshipElements
                    tempoNotesRelationshipLeftElements,
  msrTempoNotesRelationshipKind
                    tempoNotesRelationshipKind,
  S_msrTempoNotesRelationshipElements
                    tempoNotesRelationshipRightElements,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
    : msrMeasureElement (
        inputLineNumber,
        upLinkToMeasure),
      fTempoBeatUnit (),
      fTempoEquivalentBeatUnit ()
{
  fTempoKind = kTempoNotesRelationship;

  fTempoPerMinute = "";

  fTempoNotesRelationshipLeftElements  = tempoNotesRelationshipLeftElements;
  fTempoNotesRelationshipKind          = tempoNotesRelationshipKind;
  fTempoNotesRelationshipRightElements = tempoNotesRelationshipRightElements;

  fTempoParenthesizedKind = tempoParenthesizedKind;

  fTempoPlacementKind = tempoPlacementKind;
}

msrTempo::~msrTempo ()
{}

void msrTempo::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTempo::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrTempo>*
    p =
      dynamic_cast<visitor<S_msrTempo>*> (v)) {
        S_msrTempo elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTempo::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTempo::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTempo::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTempo>*
    p =
      dynamic_cast<visitor<S_msrTempo>*> (v)) {
        S_msrTempo elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTempo::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTempo::browseData (basevisitor* v)
{
  switch (fTempoKind) {
    case msrTempoKBeatUnitsKind::k_NoTempoBeatUnits:
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsWordsOnly:
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsPerMinute:
      break;

    case msrTempoKBeatUnitsKind::kTempoBeatUnitsEquivalence:
      break;

    case msrTempoKBeatUnitsKind::kTempoNotesRelationship:
      {
        // browse the left elements
        if (fTempoNotesRelationshipLeftElements) {
          msrBrowser<msrTempoNotesRelationshipElements> browser (v);
          browser.browse (*fTempoNotesRelationshipLeftElements);
        }

        // browse the right elements
        if (fTempoNotesRelationshipRightElements) {
          msrBrowser<msrTempoNotesRelationshipElements> browser (v);
          browser.browse (*fTempoNotesRelationshipRightElements);
        }
      }
      break;
  } // switch
}

string msrTempoKBeatUnitsKindAsString (
  msrTempoKBeatUnitsKind tempoKind)
{
  string result;

  switch (tempoKind) {
    case msrTempoKBeatUnitsKind::k_NoTempoBeatUnits:
      result = "k_NoTempoBeatUnits???";
      break;
    case msrTempoKBeatUnitsKind::kTempoBeatUnitsWordsOnly:
      result = "kTempoBeatUnitsWordsOnly";
      break;
    case msrTempoKBeatUnitsKind::kTempoBeatUnitsPerMinute:
      result = "kTempoBeatUnitsPerMinute";
      break;
    case msrTempoKBeatUnitsKind::kTempoBeatUnitsEquivalence:
      result = "kTempoBeatUnitsEquivalence";
      break;
    case msrTempoKBeatUnitsKind::kTempoNotesRelationship:
      result = "kTempoNotesRelationship";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrTempoKBeatUnitsKind& elt)
{
  os << msrTempoKBeatUnitsKindAsString (elt);
  return os;
}

string msrTempoParenthesizedKindAsString (
  msrTempoParenthesizedKind tempoParenthesizedKind)
{
  string result;

  switch (tempoParenthesizedKind) {
    case msrTempoParenthesizedKind::kTempoParenthesizedYes:
      result = "kTempoParenthesizedYes";
      break;
    case msrTempoParenthesizedKind::kTempoParenthesizedNo:
      result = "kTempoParenthesizedNo";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrTempoParenthesizedKind& elt)
{
  os << msrTempoParenthesizedKindAsString (elt);
  return os;
}

string msrTempoNotesRelationshipKindAsString (
  msrTempoNotesRelationshipKind tempoNotesRelationshipKind)
{
  string result;

  switch (tempoNotesRelationshipKind) {
    case msrTempoNotesRelationshipKind::kTempoNotesRelationshipEquals:
      result = "kTempoNotesRelationshipEquals";
      break;
    case msrTempoNotesRelationshipKind::kTempoNotesRelationshipNone:
      result = "kTempoNotesRelationshipNone";
      break;
  } // switch

  return result;
}

ostream& operator << (ostream& os, const msrTempoNotesRelationshipKind& elt)
{
  os << msrTempoNotesRelationshipKindAsString (elt);
  return os;
}

string msrTempo::tempoWordsListAsString (const string& separator) const
{
  string result;

  list<S_msrWords>::const_iterator
    iBegin = fTempoWordsList.begin (),
    iEnd   = fTempoWordsList.end (),
    i      = iBegin;
  for ( ; ; ) {
    result += (*i)->getWordsContents ();
    if (++i == iEnd) break;
    result += separator;
  } // for

  return result;
}

string msrTempo::asString () const
{
  stringstream s;

  s <<
    "[Tempo" <<
    ", fTempoKind: " << fTempoKind <<
    ", fTempoWordsList: ";

  if (fTempoWordsList.size ()) {
    list<S_msrWords>::const_iterator
      iBegin = fTempoWordsList.begin (),
      iEnd   = fTempoWordsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << ", ";
    } // for
  }
  else {
    s << "\"\"";
  }

  s <<
    ", fTempoBeatUnit: " << fTempoBeatUnit.asString () <<
    ", fTempoPerMinute: " << fTempoPerMinute <<
    ", fTempoParenthesizedKind: " << fTempoParenthesizedKind <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

string msrTempo::asShortStringForMeasuresSlices () const
{
  stringstream s;

  s <<
    '[';
//     ", tempoKind: " << msrTempoKBeatUnitsKindAsString (fTempoKind) <<
//     ", tempoWordsList: ";
//
//   if (fTempoWordsList.size ()) {
//     list<S_msrWords>::const_iterator
//       iBegin = fTempoWordsList.begin (),
//       iEnd   = fTempoWordsList.end (),
//       i      = iBegin;
//     for ( ; ; ) {
//       s << (*i);
//       if (++i == iEnd) break;
//       s << ", ";
//     } // for
//   }
//   else {
//     s << "\"\"";
//   }

  s <<
    fTempoBeatUnit.asString () <<
    " = " <<
    fTempoPerMinute;
//     ", tempoParenthesizedKind: "  <<
//     msrTempoParenthesizedKindAsString (fTempoParenthesizedKind) <<
//     ", line " << fInputLineNumber;

  s << ']';

  return s.str ();
}

void msrTempo::print (ostream& os) const
{
  os <<
    "[Tempo" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const int fieldWidth = 27;

  os << left <<
    setw (fieldWidth) <<
   "fTempoKind" << " : " << msrTempoKBeatUnitsKindAsString (fTempoKind) <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fTempoWordsList";

  if (fTempoWordsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_msrWords>::const_iterator
      iBegin = fTempoWordsList.begin (),
      iEnd   = fTempoWordsList.end (),
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
      " : " << "[EMPTY]" <<
      endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "fTempoBeatUnit :" <<
    endl;

  ++gIndenter;

  os <<
    fTempoBeatUnit;

  --gIndenter;

  os << left <<
    setw (fieldWidth) <<
    "tempoPerMinute" << " : \"" << fTempoPerMinute << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "fTempoEquivalentBeatUnit"  << " : " <<
    fTempoEquivalentBeatUnit <<
    endl <<

    setw (fieldWidth) <<
    "tempoParenthesizedKind"  << " : " <<
    msrTempoParenthesizedKindAsString (fTempoParenthesizedKind) <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fTempoNotesRelationshipLeftElements";
  if (fTempoNotesRelationshipLeftElements) {
    os <<
      " : " <<
      endl;

    ++gIndenter;

    os <<
      fTempoNotesRelationshipLeftElements;

    --gIndenter;
  }
  else {
    os << " : " << "[NONE]" <<
    endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "fTempoNotesRelationshipKind" << " : " <<
    msrTempoNotesRelationshipKindAsString (
      fTempoNotesRelationshipKind) <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "fTempoNotesRelationshipRightElements";
  if (fTempoNotesRelationshipRightElements) {
    os <<
      ":" <<
      endl;

    ++gIndenter;

    os <<
      fTempoNotesRelationshipRightElements;

    --gIndenter;
  }
  else {
    os << " : " << "[NONE]" <<
    endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "fTempoPlacementKind" << " : " <<
    msrPlacementKindAsString (
      fTempoPlacementKind) <<
    endl;

  --gIndenter;

  os << ']' << endl;
}

void msrTempo::printShort (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator << (ostream& os, const S_msrTempo& elt)
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
