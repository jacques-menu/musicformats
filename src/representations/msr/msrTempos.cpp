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
#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "mfAssert.h"
#include "mfServiceRunData.h"
#include "mfStringsHandling.h"

#include "msrMeasureConstants.h"

#include "msrTemposEnumTypes.h"
#include "msrTempos.h"

#include "oahOah.h"
#include "msrOah.h"

#include "msrBrowsers.h"

#include "msrWae.h"


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
      std::endl;
  }

  if (visitor<S_msrTempoNote>*
    p =
      dynamic_cast<visitor<S_msrTempoNote>*> (v)) {
        S_msrTempoNote elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTempoNote::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrTempoNote::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTempoNote::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrTempoNote>*
    p =
      dynamic_cast<visitor<S_msrTempoNote>*> (v)) {
        S_msrTempoNote elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTempoNote::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrTempoNote::browseData (basevisitor* v)
{
  for (
    std::list<S_msrBeam>::const_iterator i = fTempoNoteBeams.begin ();
    i != fTempoNoteBeams.end ();
    ++i ) {
    // browse tempoTuplet element
    msrBrowser<msrBeam> browser (v);
    browser.browse (*(*i));
  } // for
}

std::string msrTempoNote::asString () const
{
  std::stringstream s;

  s <<
    "Tempo note" <<
    ", tempoNoteWholeNotes = " << fTempoNoteWholeNotes <<
    ", tempoNoteBelongsToATuplet = " <<
    fTempoNoteBelongsToATuplet;

  return s.str ();
}

void msrTempoNote::print (std::ostream& os) const
{
  os <<
    "[TempoNote" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 26;

  os << std::left <<
    std::setw (fieldWidth) <<
    "tempoNoteWholeNotes" << " : " << fTempoNoteWholeNotes <<
    std::endl <<
    std::setw (fieldWidth) <<
    "tempoNoteBelongsToATuplet" << " : " <<
    fTempoNoteBelongsToATuplet <<
    std::endl <<
    std::setw (fieldWidth) <<
    "tempoNoteBeams";

    if (fTempoNoteBeams.size ()) {
      os << std::endl;

      ++gIndenter;

      std::list<S_msrBeam>::const_iterator
        iBegin = fTempoNoteBeams.begin (),
        iEnd   = fTempoNoteBeams.end (),
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
        " : " <<
       "[NONE]" <<
        std::endl;
    }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrTempoNote& elt)
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
S_msrTempoTuplet msrTempoTuplet::create (
  int                          inputLineNumber,
  int                          tempoTupletNumber,
  msrTempoTupletBracketKind    tempoTupletBracketKind,
  msrTempoTupletShowNumberKind tempoTupletShowNumberKind,
  const msrTupletFactor&       tempoTupletFactor,
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
  const msrTupletFactor&       tempoTupletFactor,
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
      std::endl;
  }
#endif
}

msrTempoTuplet::~msrTempoTuplet ()
{}

std::string msrTempoTupletTypeKindAsString (
  msrTempoTupletTypeKind tempoTupletTypeKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const msrTempoTupletTypeKind& elt)
{
  os << msrTempoTupletTypeKindAsString (elt);
  return os;
}

std::string msrTempoTupletBracketKindAsString (
  msrTempoTupletBracketKind tempoTupletBracketKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const msrTempoTupletBracketKind& elt)
{
  os << msrTempoTupletBracketKindAsString (elt);
  return os;
}

std::string msrTempoTupletShowNumberKindAsString (
  msrTempoTupletShowNumberKind tempoTupletShowNumberKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const msrTempoTupletShowNumberKind& elt)
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
      std::endl;
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
      std::endl;
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
      std::endl;
  }
#endif

  if (fTempoTupletElements.size ()) {
    S_msrElement
      firstTempoTupletElement =
        fTempoTupletElements.front ();

    for (
      std::list<S_msrElement>::iterator i=fTempoTupletElements.begin ();
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

    std::stringstream s;

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
    std::stringstream s;

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
      std::endl;

    ++gIndenter;

    gLogStream <<
      "% fTempoTupletFactor = " <<
      fTempoTupletFactor <<
      std::endl << std::endl;

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
      std::endl;

    ++gIndenter;

    gLogStream <<
      "% fTempoTupletFactor = " <<
      fTempoTupletFactor <<
      std::endl <<
      "% containingTempoTupletActualNotes = " <<
      containingTempoTupletActualNotes <<
      ", containingTempoTupletNormalNotes = " <<
      containingTempoTupletNormalNotes <<
      std::endl << std::endl;

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
      std::endl;
  }

  if (visitor<S_msrTempoTuplet>*
    p =
      dynamic_cast<visitor<S_msrTempoTuplet>*> (v)) {
        S_msrTempoTuplet elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTempoTuplet::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrTempoTuplet::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTempoTuplet::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrTempoTuplet>*
    p =
      dynamic_cast<visitor<S_msrTempoTuplet>*> (v)) {
        S_msrTempoTuplet elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTempoTuplet::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrTempoTuplet::browseData (basevisitor* v)
{
  for (
    std::list<S_msrElement>::const_iterator i = fTempoTupletElements.begin ();
    i != fTempoTupletElements.end ();
    ++i ) {
    // browse tempoTuplet element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for
}

std::string msrTempoTuplet::asString () const
{
  std::stringstream s;

  s <<
    "TempoTuplet " <<
    fTempoTupletFactor <<
    ' ' << fTempoTupletDisplayWholeNotes << " display whole notes" <<
    ":";

  s << "[[";

  if (fTempoTupletElements.size ()) {
    std::list<S_msrElement>::const_iterator
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
std::string msrTempoTuplet::asString () const
{
  std::stringstream s;

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
    std::list<S_msrElement>::const_iterator
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

void msrTempoTuplet::print (std::ostream& os) const
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
    std::endl;

  ++gIndenter;

  const int fieldWidth = 30;

  os << std::left <<
    std::setw (fieldWidth) <<
    "TempoTupletBracketKind" << " : " <<
    msrTempoTupletBracketKindAsString (
      fTempoTupletBracketKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "TempoTupletShowNumberKind" << " : " <<
    fTempoTupletShowNumberKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "MemberNotesDisplayWholeNotes" << " : " <<
    fMemberNotesDisplayWholeNotes <<
    std::endl << std::endl;

/* JMI ???
  os << std::left <<
    std::setw (fieldWidth) <<
    "(position in measure" << " : ";
  if (fTempoTupletMeasurePosition.getNumerator () < 0) {
    os << "???)";
  }
  else {
    os << fTempoTupletMeasurePosition << ")";
  }
  os << std::endl;
    */

  os <<
    "fTempoTupletElements";

  if (fTempoTupletElements.size ()) {
    ++gIndenter;

    std::list<S_msrElement>::const_iterator
      iBegin = fTempoTupletElements.begin (),
      iEnd   = fTempoTupletElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no std::endl here
    } // for

    --gIndenter;
    }
  else {
    os << std::left <<
      " : " << "[EMPTY]" <<
      std::endl;
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrTempoTuplet& elt)
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
      std::endl;
  }
#endif

  fTempoNotesRelationshipElementsList.push_back (element);
}

void msrTempoNotesRelationshipElements::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTempoNotesRelationshipElements::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrTempoNotesRelationshipElements>*
    p =
      dynamic_cast<visitor<S_msrTempoNotesRelationshipElements>*> (v)) {
        S_msrTempoNotesRelationshipElements elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTempoNotesRelationshipElements::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrTempoNotesRelationshipElements::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTempoNotesRelationshipElements::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrTempoNotesRelationshipElements>*
    p =
      dynamic_cast<visitor<S_msrTempoNotesRelationshipElements>*> (v)) {
        S_msrTempoNotesRelationshipElements elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTempoNotesRelationshipElements::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrTempoNotesRelationshipElements::browseData (basevisitor* v)
{
  // browse the elements list
  if (fTempoNotesRelationshipElementsList.size ()) {
    for (
      std::list<S_msrElement>::const_iterator i =
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

std::string msrTempoNotesRelationshipElements::asString () const
{
  std::stringstream s;

  s <<
    "TempoNotesRelationshipElements" <<
    ", fTempoNotesRelationshipElementsKind = " << fTempoNotesRelationshipElementsKind;

  return s.str ();
}

std::string msrTempoNotesRelationshipElementsKindAsString (
  msrTempoNotesRelationshipElementsKind tempoNotesRelationshipElementsKind)
{
  std::string result;

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

std::ostream& operator << (std::ostream& os, const msrTempoNotesRelationshipElementsKind& elt)
{
  os << msrTempoNotesRelationshipElementsKindAsString (elt);
  return os;
}

void msrTempoNotesRelationshipElements::print (std::ostream& os) const
{
  os <<
    "[TempoNotesRelationshipElements" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  os <<
    "msrTempoNotesRelationshipElementsKindAsString : " <<
    msrTempoNotesRelationshipElementsKindAsString (
      fTempoNotesRelationshipElementsKind) <<
    std::endl;

  const int fieldWidth = 26;

  os << std::left <<
    std::setw (fieldWidth) <<
    "tempoNotesRelationshipElementsList";

    if (fTempoNotesRelationshipElementsList.size ()) {
      ++gIndenter;

      os << std::endl;

      std::list<S_msrElement>::const_iterator
        iBegin = fTempoNotesRelationshipElementsList.begin (),
        iEnd   = fTempoNotesRelationshipElementsList.end (),
        i      = iBegin;

      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // os << std::endl;
      } // for

      --gIndenter;
  }
  else {
    os <<
      " : " << "[NONE]" <<
      std::endl;
  }

  --gIndenter;

  os << ']' << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrTempoNotesRelationshipElements& elt)
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
S_msrTempo msrTempo::createTempoWordsOnly (
  int               inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
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

S_msrTempo msrTempo::createTempoWordsOnly (
  int               inputLineNumber,
  S_msrWords        tempoWords,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
{
  return
    msrTempo::createTempoWordsOnly (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      tempoWords,
      tempoParenthesizedKind,
      tempoPlacementKind);
}

S_msrTempo msrTempo::createTempoPerMinute (
  int               inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrDottedDuration tempoBeatUnit,
  std::string       tempoPerMinute,
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

S_msrTempo msrTempo::createTempoPerMinute (
  int               inputLineNumber,
  msrDottedDuration tempoBeatUnit,
  std::string       tempoPerMinute,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
{
  return
    msrTempo::createTempoPerMinute (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      tempoBeatUnit,
      tempoPerMinute,
      tempoParenthesizedKind,
      tempoPlacementKind);
}

S_msrTempo msrTempo::createTempoBeatUnitEquivalent (
  int               inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
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

S_msrTempo msrTempo::createTempoBeatUnitEquivalent (
  int               inputLineNumber,
  msrDottedDuration tempoBeatUnit,
  msrDottedDuration tempoEquivalentBeatUnit,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
{
  return
     msrTempo::createTempoBeatUnitEquivalent (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      tempoBeatUnit,
      tempoEquivalentBeatUnit,
      tempoParenthesizedKind,
      tempoPlacementKind);
}

S_msrTempo msrTempo::createTempoNotesRelationship (
  int               inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
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

S_msrTempo msrTempo::createTempoNotesRelationship (
  int               inputLineNumber,
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
  return
    msrTempo::createTempoNotesRelationship (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer, // set later in setMeasureElementUpLinkToMeasure()
      tempoNotesRelationshipLeftElements,
      tempoNotesRelationshipKind,
      tempoNotesRelationshipRightElements,
      tempoParenthesizedKind,
      tempoPlacementKind);
}

msrTempo::msrTempo (
  int               inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  S_msrWords        tempoWords,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
    : msrMeasureElement (
        inputLineNumber),
      fTempoBeatUnit (),
      fTempoEquivalentBeatUnit ()
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    tempoWords != nullptr,
    "tempoWords is null");

  fTempoKind = msrTempoKBeatUnitsKind::kTempoBeatUnitsWordsOnly;

  fTempoPerMinute = "";

  fTempoNotesRelationshipKind =
    msrTempoNotesRelationshipKind::kTempoNotesRelationshipNone;

  fTempoParenthesizedKind = msrTempoParenthesizedKind::kTempoParenthesizedNo;

  fTempoPlacementKind = msrPlacementKind::kPlacementAbove;

  fTempoWordsList.push_back (tempoWords);
}

msrTempo::msrTempo (
  int               inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrDottedDuration tempoBeatUnit,
  std::string       tempoPerMinute,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
    : msrMeasureElement (
        inputLineNumber),
      fTempoBeatUnit (tempoBeatUnit),
      fTempoEquivalentBeatUnit ()
{
  fTempoKind = msrTempoKBeatUnitsKind::kTempoBeatUnitsPerMinute;

  fTempoPerMinute = tempoPerMinute;

  fTempoNotesRelationshipKind =
    msrTempoNotesRelationshipKind::kTempoNotesRelationshipNone;

  fTempoParenthesizedKind = tempoParenthesizedKind;

  fTempoPlacementKind = tempoPlacementKind;
}

msrTempo::msrTempo (
  int               inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrDottedDuration tempoBeatUnit,
  msrDottedDuration tempoEquivalentBeatUnit,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
    : msrMeasureElement (
        inputLineNumber),
      fTempoBeatUnit (tempoBeatUnit),
      fTempoEquivalentBeatUnit (tempoEquivalentBeatUnit)
{
  fTempoKind = msrTempoKBeatUnitsKind::kTempoBeatUnitsEquivalence;

  fTempoPerMinute = "";

  fTempoNotesRelationshipKind =
    msrTempoNotesRelationshipKind::kTempoNotesRelationshipNone;

  fTempoParenthesizedKind = tempoParenthesizedKind;

  fTempoPlacementKind = tempoPlacementKind;
}

msrTempo::msrTempo (
  int               inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
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
        inputLineNumber),
      fTempoBeatUnit (),
      fTempoEquivalentBeatUnit ()
{
  fTempoKind = msrTempoKBeatUnitsKind::kTempoBeatUnitsWordsOnly;

  fTempoPerMinute = "";

  fTempoNotesRelationshipLeftElements  = tempoNotesRelationshipLeftElements;
  fTempoNotesRelationshipKind          = tempoNotesRelationshipKind;
  fTempoNotesRelationshipRightElements = tempoNotesRelationshipRightElements;

  fTempoParenthesizedKind = tempoParenthesizedKind;

  fTempoPlacementKind = tempoPlacementKind;
}

msrTempo::~msrTempo ()
{}

void msrTempo::setTempoUpLinkToMeasure (
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
      "==> Setting the uplink to measure of tempo " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fTempoUpLinkToMeasure = measure;
}

void msrTempo::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTempo::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrTempo>*
    p =
      dynamic_cast<visitor<S_msrTempo>*> (v)) {
        S_msrTempo elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTempo::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrTempo::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrTempo::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrTempo>*
    p =
      dynamic_cast<visitor<S_msrTempo>*> (v)) {
        S_msrTempo elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrTempo::visitEnd ()" <<
            std::endl;
        }
        p->visitEnd (elem);
  }
}

void msrTempo::browseData (basevisitor* v)
{
  switch (fTempoKind) {
    case msrTempoKBeatUnitsKind::kTempoBeatUnits_NO_:
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

std::string msrTempo::tempoWordsListAsString (const std::string& separator) const
{
  std::string result;

  std::list<S_msrWords>::const_iterator
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

std::string msrTempo::asString () const
{
  std::stringstream s;

  s <<
    "[Tempo" <<
    ", fTempoKind: " << fTempoKind <<
    ", fTempoWordsList: ";

  if (fTempoWordsList.size ()) {
    std::list<S_msrWords>::const_iterator
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

std::string msrTempo::asShortStringForMeasuresSlices () const
{
  std::stringstream s;

  s <<
    '[';
//     ", tempoKind: " << msrTempoBeatUnitsKindAsString (fTempoKind) <<
//     ", tempoWordsList: ";
//
//   if (fTempoWordsList.size ()) {
//     std::list<S_msrWords>::const_iterator
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

void msrTempo::print (std::ostream& os) const
{
  os <<
    "[Tempo" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 27;

  os << std::left <<
    std::setw (fieldWidth) <<
   "fTempoKind" << " : " << msrTempoBeatUnitsKindAsString (fTempoKind) <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTempoWordsList";

  if (fTempoWordsList.size ()) {
    os << std::endl;

    ++gIndenter;

    std::list<S_msrWords>::const_iterator
      iBegin = fTempoWordsList.begin (),
      iEnd   = fTempoWordsList.end (),
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
      " : " << "[EMPTY]" <<
      std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTempoBeatUnit :" <<
    std::endl;

  ++gIndenter;

  os <<
    fTempoBeatUnit;

  --gIndenter;

  os << std::left <<
    std::setw (fieldWidth) <<
    "tempoPerMinute" << " : \"" << fTempoPerMinute << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTempoEquivalentBeatUnit"  << " : " <<
    fTempoEquivalentBeatUnit <<
    std::endl <<

    std::setw (fieldWidth) <<
    "tempoParenthesizedKind"  << " : " <<
    msrTempoParenthesizedKindAsString (fTempoParenthesizedKind) <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTempoNotesRelationshipLeftElements";
  if (fTempoNotesRelationshipLeftElements) {
    os <<
      " : " <<
      std::endl;

    ++gIndenter;

    os <<
      fTempoNotesRelationshipLeftElements;

    --gIndenter;
  }
  else {
    os << " : " << "[NONE]" <<
    std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTempoNotesRelationshipKind" << " : " <<
    msrTempoNotesRelationshipKindAsString (
      fTempoNotesRelationshipKind) <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTempoNotesRelationshipRightElements";
  if (fTempoNotesRelationshipRightElements) {
    os <<
      ":" <<
      std::endl;

    ++gIndenter;

    os <<
      fTempoNotesRelationshipRightElements;

    --gIndenter;
  }
  else {
    os << " : " << "[NONE]" <<
    std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTempoPlacementKind" << " : " <<
    msrPlacementKindAsString (
      fTempoPlacementKind) <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

void msrTempo::printShort (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrTempo& elt)
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
