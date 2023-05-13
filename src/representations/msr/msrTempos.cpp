/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfServices.h"
#include "mfStringsHandling.h"

#include "msrMeasureConstants.h"

#include "msrTemposEnumTypes.h"
#include "msrTempos.h"

#include "oahOah.h"

#include "msrOah.h"

#include "msrBrowsers.h"

#include "msrWae.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrTempoNote msrTempoNote::create (
  int             inputLineNumber,
  const msrWholeNotes& tempoNoteWholeNotes,
  Bool            tempoNoteBelongsToATuplet)
{
  msrTempoNote * obj =
    new msrTempoNote (
      inputLineNumber,
      tempoNoteWholeNotes,
      tempoNoteBelongsToATuplet);
  assert (obj != nullptr);
  return obj;
}

msrTempoNote::msrTempoNote (
  int             inputLineNumber,
  const msrWholeNotes& tempoNoteWholeNotes,
  Bool            tempoNoteBelongsToATuplet)
    : msrElement (inputLineNumber)
{
  fTempoNoteWholeNotes = tempoNoteWholeNotes;

  fTempoNoteBelongsToATuplet = tempoNoteBelongsToATuplet;
}

msrTempoNote::~msrTempoNote ()
{}

void msrTempoNote::appendBeamToTempoNote (const S_msrBeam& beam)
{
  fTempoNoteBeams.push_back (beam);
}

void msrTempoNote::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTempoNote::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTempoNote>*
    p =
      dynamic_cast<visitor<S_msrTempoNote>*> (v)) {
        S_msrTempoNote elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTempoNote::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrTempoNote::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTempoNote::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTempoNote>*
    p =
      dynamic_cast<visitor<S_msrTempoNote>*> (v)) {
        S_msrTempoNote elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTempoNote::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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
  std::stringstream ss;

  ss <<
    "Tempo note" <<
    ", tempoNoteWholeNotes: " << fTempoNoteWholeNotes.asString () <<
    ", tempoNoteBelongsToATuplet: " <<
    fTempoNoteBelongsToATuplet;

  return ss.str ();
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
    "tempoNoteWholeNotes" << ": " << fTempoNoteWholeNotes.asString () <<
    std::endl <<
    std::setw (fieldWidth) <<
    "tempoNoteBelongsToATuplet" << ": " <<
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
        ": " <<
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
    os << "[NULL]" << std::endl;
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
  const msrWholeNotes&         memberNotesDisplayWholeNotes)
{
  msrTempoTuplet* obj =
    new msrTempoTuplet (
      inputLineNumber,
      tempoTupletNumber,
      tempoTupletBracketKind,
      tempoTupletShowNumberKind,
      tempoTupletFactor,
      memberNotesDisplayWholeNotes);
  assert (obj != nullptr);
  return obj;
}

msrTempoTuplet::msrTempoTuplet (
  int                          inputLineNumber,
  int                          tempoTupletNumber,
  msrTempoTupletBracketKind    tempoTupletBracketKind,
  msrTempoTupletShowNumberKind tempoTupletShowNumberKind,
  const msrTupletFactor&       tempoTupletFactor,
  const msrWholeNotes&         memberNotesDisplayWholeNotes)
    : msrElement (inputLineNumber)
{
  fTempoTupletNumber = tempoTupletNumber;

  fTempoTupletBracketKind    = tempoTupletBracketKind;
  fTempoTupletShowNumberKind = tempoTupletShowNumberKind;

  fTempoTupletFactor = tempoTupletFactor;

  fMemberNotesDisplayWholeNotes  = memberNotesDisplayWholeNotes;

  fTempoTupletDisplayWholeNotes  = msrWholeNotes (0, 1);

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTempos ()){
    std::stringstream ss;

    ss <<
      "Creating tempo tuplet '" <<
      this->asString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTempos ()) {
    std::stringstream ss;

    ss <<
      "Adding tempoNote '" <<
      tempoNote->asShortString () <<
      // the information is missing to display it the normal way JMI ???
      "' to tempoTuplet '" <<
      asString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTempos ()) {
    std::stringstream ss;

    ss <<
      "Adding tempoTuplet '" <<
      tempoTuplet->asString () <<
      "' to tempoTuplet '" <<
      asString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  // register tempoTuplet in elements list
  fTempoTupletElements.push_back (tempoTuplet);

  // account for tempoTuplet duration
  fTempoTupletDisplayWholeNotes += // JMI
    tempoTuplet->getTempoTupletDisplayWholeNotes ();

    / *
  fTempoTupletDisplayWholeNotes += // JMI
    tempoTuplet->getTempoTupletDisplayWholeNotes ();
    * /

  // don't populate tempoTuplet's measure number nor measure position,
  // this will be done in setTupletMembersMeasureNumber ()
  / * JMI
  tempoTuplet->setTempoTupletMeasureNumber (
    fTempoTupletMeasureNumber);

  // populate tempoTuplet's measure position
  tempoTuplet->setTempoTupletMeasurePosition (
    fTempoTupletMeasurePosition);
    * /
}

void msrTempoTuplet::removeFirstNoteFromTempoTuplet (
  int            inputLineNumber,
  const S_msrTempoNote& tempoNote)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTempos ()) {
    std::stringstream ss;

    ss <<
      "Removing first tempoNote '" <<
      tempoNote->asShortString () <<
      "' from tempoTuplet '" <<
      asString () <<
      "'";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

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

        // don't update measure number nor measure position: // JMI
        // they have not been set yet

        // return from function
        return;
      }
    } // for

    std::stringstream ss;

    ss <<
      "cannot remove tempoNote " <<
      tempoNote <<
      " from tempoTuplet " << asString () <<
      " since this note has not been found in fTempoTupletElements";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }

  else {
    std::stringstream ss;

    ss <<
      "cannot remove tempoNote " <<
      tempoNote <<
      " from empty tempoTuplet " <<
      " since this note cannot be found in empty fTempoTupletElements";

    msrInternalError (
      gServiceRunData->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      ss.str ());
  }
}
*/

/* JMI
void msrTempoTuplet::applyDisplayFactorToTempoTupletMembers ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTempos ()) {
    std::stringstream ss;

    ss <<
      "% ==> applyDisplayFactorToTempoTupletMembers ()" <<
      std::endl;

    ++gIndenter;

    gLog <<
      "% fTempoTupletFactor: " <<
      fTempoTupletFactor <<
      std::endl << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED
}

void msrTempoTuplet::unapplySoundingFactorToTempoTupletMembers (
  int containingTempoTupletActualNotes,
  int containingTempoTupletNormalNotes)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTempos ()) {
    std::stringstream ss;

    ss <<
      "unapplySoundingFactorToTempoTupletMembers ()" <<
      std::endl;

    ++gIndenter;

    gLog <<
      "% fTempoTupletFactor: " <<
      fTempoTupletFactor <<
      std::endl <<
      "% containingTempoTupletActualNotes: " <<
      containingTempoTupletActualNotes <<
      ", containingTempoTupletNormalNotes: " <<
      containingTempoTupletNormalNotes <<
      std::endl << std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  fTempoTupletFactor /= // JMI ???
    containingTempoTupletActualNotes;
}
*/

void msrTempoTuplet::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTempoTuplet::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTempoTuplet>*
    p =
      dynamic_cast<visitor<S_msrTempoTuplet>*> (v)) {
        S_msrTempoTuplet elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTempoTuplet::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrTempoTuplet::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTempoTuplet::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTempoTuplet>*
    p =
      dynamic_cast<visitor<S_msrTempoTuplet>*> (v)) {
        S_msrTempoTuplet elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTempoTuplet::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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
  std::stringstream ss;

  ss <<
    "TempoTuplet " <<
    fTempoTupletFactor <<
    ' ' << fTempoTupletDisplayWholeNotes.asString () << " display whole notes" <<
    ":";

  ss << '[';

  if (fTempoTupletElements.size ()) {
    std::list<S_msrElement>::const_iterator
      iBegin = fTempoTupletElements.begin (),
      iEnd   = fTempoTupletElements.end (),
      i      = iBegin;
    for ( ; ; ) {

      if (
        S_msrTempoNote note = dynamic_cast<msrTempoNote*>(&(*(*i)))
        ) {
        ss <<
          note->asShortString ();
      }

      else if (
        S_msrTempoTuplet tempoTuplet = dynamic_cast<msrTempoTuplet*>(&(*(*i)))
        ) {
        ss <<
          tempoTuplet->asString ();
      }

      else {
        msrInternalError (
          gServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          "tempoTuplet member should be a note, a chord or another tempoTuplet");
      }

      if (++i == iEnd) break;
      ss << ' ';

    } // for
  }

  ss << ']';

  return ss.str ();
}

/* JMI
std::string msrTempoTuplet::asString () const
{
  std::stringstream ss;

  ss <<
    "TempoTuplet " <<
    fTempoTupletFactor <<
    ' ' << fTempoTupletSoundingWholeNotes.asString () << " tempo tuplet sounding whole notes" <<
    " measure '" <<
    fTempoTupletMeasureNumber <<
    "':";

  if (fTempoTupletMeasurePosition.getNumerator () < 0) {
    ss << "?";
  }
  else {
    ss << fTempoTupletMeasurePosition;
  }

  ss << '[';

  if (fTempoTupletElements.size ()) {
    std::list<S_msrElement>::const_iterator
      iBegin = fTempoTupletElements.begin (),
      iEnd   = fTempoTupletElements.end (),
      i      = iBegin;
    for ( ; ; ) {

      if (
        S_msrTempoNote note = dynamic_cast<msrTempoNote*>(&(*(*i)))
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
        S_msrTempoTuplet tempoTuplet = dynamic_cast<msrTempoTuplet*>(&(*(*i)))
        ) {
        ss <<
          tempoTuplet->asString ();
      }

      else {
        msrInternalError (
          gServiceRunData->getInputSourceName (),
          fInputLineNumber,
          __FILE__, __LINE__,
          "tempoTuplet member should be a note, a chord or another tempoTuplet");
      }

      if (++i == iEnd) break;
      ss << ' ';
    } // for
  }

  ss << ']';

  return ss.str ();
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
    fTempoTupletDisplayWholeNotes.asString () <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 30;

  os << std::left <<
    std::setw (fieldWidth) <<
    "TempoTupletBracketKind" << ": " <<
    msrTempoTupletBracketKindAsString (
      fTempoTupletBracketKind) <<
    std::endl <<
    std::setw (fieldWidth) <<
    "TempoTupletShowNumberKind" << ": " <<
    fTempoTupletShowNumberKind <<
    std::endl <<
    std::setw (fieldWidth) <<
    "MemberNotesDisplayWholeNotes" << ": " <<
    fMemberNotesDisplayWholeNotes.asString () <<
    std::endl << std::endl;

/* JMI ???
  os << std::left <<
    std::setw (fieldWidth) <<
    "(measure position" << ": ";
  if (fTempoTupletMeasurePosition.getNumerator () < 0) {
    os << "???)";
  }
  else {
    os << fTempoTupletMeasurePosition.asString () << ")";
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
      ": " << "[EMPTY]" <<
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
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrTempoNotesRelationshipElements msrTempoNotesRelationshipElements::create (
  int      inputLineNumber,
  msrTempoNotesRelationshipElementsKind
           tempoNotesRelationshipElementsKind)
{
  msrTempoNotesRelationshipElements * obj =
    new msrTempoNotesRelationshipElements (
      inputLineNumber,
      tempoNotesRelationshipElementsKind);
  assert (obj != nullptr);
  return obj;
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
  const S_msrElement& element)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTempos ()){
    std::stringstream ss;

    ss <<
      "Adding element '" <<
      element->asString () <<
      "' to tempo relationship";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fTempoNotesRelationshipElementsList.push_back (element);
}

void msrTempoNotesRelationshipElements::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTempoNotesRelationshipElements::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTempoNotesRelationshipElements>*
    p =
      dynamic_cast<visitor<S_msrTempoNotesRelationshipElements>*> (v)) {
        S_msrTempoNotesRelationshipElements elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTempoNotesRelationshipElements::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrTempoNotesRelationshipElements::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTempoNotesRelationshipElements::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTempoNotesRelationshipElements>*
    p =
      dynamic_cast<visitor<S_msrTempoNotesRelationshipElements>*> (v)) {
        S_msrTempoNotesRelationshipElements elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTempoNotesRelationshipElements::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
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
  std::stringstream ss;

  ss <<
    "TempoNotesRelationshipElements" <<
    ", fTempoNotesRelationshipElementsKind: " << fTempoNotesRelationshipElementsKind;

  return ss.str ();
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
      ": " << "[NONE]" <<
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
    os << "[NULL]" << std::endl;
  }

  return os;
}

//______________________________________________________________________________
S_msrTempo msrTempo::createTempoWordsOnly (
  int               inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const S_msrWords& tempoWords,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
{
  msrTempo* obj =
    new msrTempo (
      inputLineNumber,
      upLinkToMeasure,
      tempoWords,
      tempoParenthesizedKind,
      tempoPlacementKind);
  assert (obj != nullptr);
  return obj;
}

S_msrTempo msrTempo::createTempoWordsOnly (
  int               inputLineNumber,
  const S_msrWords& tempoWords,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
{
  return
    msrTempo::createTempoWordsOnly (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      tempoWords,
      tempoParenthesizedKind,
      tempoPlacementKind);
}

S_msrTempo msrTempo::createTempoPerMinute (
  int               inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrDottedNotesDuration tempoBeatUnit,
  std::string       tempoPerMinute,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
{
  msrTempo* obj =
    new msrTempo (
      inputLineNumber,
      upLinkToMeasure,
      tempoBeatUnit,
      tempoPerMinute,
      tempoParenthesizedKind,
      tempoPlacementKind);
  assert (obj != nullptr);
  return obj;
}

S_msrTempo msrTempo::createTempoPerMinute (
  int               inputLineNumber,
  msrDottedNotesDuration tempoBeatUnit,
  std::string       tempoPerMinute,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
{
  return
    msrTempo::createTempoPerMinute (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      tempoBeatUnit,
      tempoPerMinute,
      tempoParenthesizedKind,
      tempoPlacementKind);
}

S_msrTempo msrTempo::createTempoBeatUnitEquivalent (
  int               inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  msrDottedNotesDuration tempoBeatUnit,
  msrDottedNotesDuration tempoEquivalentBeatUnit,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
{
  msrTempo* obj =
    new msrTempo (
      inputLineNumber,
      upLinkToMeasure,
      tempoBeatUnit,
      tempoEquivalentBeatUnit,
      tempoParenthesizedKind,
      tempoPlacementKind);
  assert (obj != nullptr);
  return obj;
}

S_msrTempo msrTempo::createTempoBeatUnitEquivalent (
  int               inputLineNumber,
  msrDottedNotesDuration tempoBeatUnit,
  msrDottedNotesDuration tempoEquivalentBeatUnit,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
{
  return
     msrTempo::createTempoBeatUnitEquivalent (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      tempoBeatUnit,
      tempoEquivalentBeatUnit,
      tempoParenthesizedKind,
      tempoPlacementKind);
}

S_msrTempo msrTempo::createTempoNotesRelationship (
  int               inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const S_msrTempoNotesRelationshipElements&
                    tempoNotesRelationshipLeftElements,
  msrTempoNotesRelationshipKind
                    tempoNotesRelationshipKind,
  const S_msrTempoNotesRelationshipElements&
                    tempoNotesRelationshipRightElements,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
{
  msrTempo* obj =
    new msrTempo (
      inputLineNumber,
      upLinkToMeasure,
      tempoNotesRelationshipLeftElements,
      tempoNotesRelationshipKind,
      tempoNotesRelationshipRightElements,
      tempoParenthesizedKind,
      tempoPlacementKind);
  assert (obj != nullptr);
  return obj;
}

S_msrTempo msrTempo::createTempoNotesRelationship (
  int               inputLineNumber,
  const S_msrTempoNotesRelationshipElements&
                    tempoNotesRelationshipLeftElements,
  msrTempoNotesRelationshipKind
                    tempoNotesRelationshipKind,
  const S_msrTempoNotesRelationshipElements&
                    tempoNotesRelationshipRightElements,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
{
  return
    msrTempo::createTempoNotesRelationship (
      inputLineNumber,
      gNullMeasure, // set later in setMeasureElementUpLinkToMeasure()
      tempoNotesRelationshipLeftElements,
      tempoNotesRelationshipKind,
      tempoNotesRelationshipRightElements,
      tempoParenthesizedKind,
      tempoPlacementKind);
}

msrTempo::msrTempo (
  int               inputLineNumber,
  const S_msrMeasure& upLinkToMeasure,
  const S_msrWords& tempoWords,
  msrTempoParenthesizedKind
                    tempoParenthesizedKind,
  msrPlacementKind  tempoPlacementKind)
    : msrMeasureElement (
        inputLineNumber),
      fTempoBeatUnit (),
      fTempoEquivalentBeatUnit ()
{
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    tempoWords != nullptr,
    "tempoWords is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

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
  msrDottedNotesDuration tempoBeatUnit,
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
  msrDottedNotesDuration tempoBeatUnit,
  msrDottedNotesDuration tempoEquivalentBeatUnit,
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
  const S_msrTempoNotesRelationshipElements&
                    tempoNotesRelationshipLeftElements,
  msrTempoNotesRelationshipKind
                    tempoNotesRelationshipKind,
  const S_msrTempoNotesRelationshipElements&
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
#ifdef MF_SANITY_CHECKS_ARE_ENABLED
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");
#endif // MF_SANITY_CHECKS_ARE_ENABLED

#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceTempos ()) {
    ++gIndenter;

    gLog <<
      "Setting the uplink to measure of tempo " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif // MF_TRACE_IS_ENABLED

  fTempoUpLinkToMeasure = measure;
}

void msrTempo::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTempo::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTempo>*
    p =
      dynamic_cast<visitor<S_msrTempo>*> (v)) {
        S_msrTempo elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTempo::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrTempo::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrTempo::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrTempo>*
    p =
      dynamic_cast<visitor<S_msrTempo>*> (v)) {
        S_msrTempo elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrTempo::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrTempo::browseData (basevisitor* v)
{
  switch (fTempoKind) {
    case msrTempoKBeatUnitsKind::kTempoBeatUnits_UNKNOWN_:
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
  std::stringstream ss;

  ss <<
    "[Tempo" <<
    ", fTempoKind: " << fTempoKind <<
    ", fTempoWordsList: ";

  if (fTempoWordsList.size ()) {
    std::list<S_msrWords>::const_iterator
      iBegin = fTempoWordsList.begin (),
      iEnd   = fTempoWordsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      ss << (*i);
      if (++i == iEnd) break;
      ss << ", ";
    } // for
  }
  else {
    ss << "\"\"";
  }

  ss <<
    ", fTempoBeatUnit: " << fTempoBeatUnit.asString () <<
    ", fTempoPerMinute: " << fTempoPerMinute <<
    ", fTempoParenthesizedKind: " << fTempoParenthesizedKind <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

std::string msrTempo::asShortStringForMeasuresSlices () const
{
  std::stringstream ss;

  ss <<
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
//       ss << (*i);
//       if (++i == iEnd) break;
//       ss << ", ";
//     } // for
//   }
//   else {
//     ss << "\"\"";
//   }

  ss <<
    fTempoBeatUnit.asString () <<
    ": " <<
    fTempoPerMinute;
//     ", tempoParenthesizedKind: "  <<
//     msrTempoParenthesizedKindAsString (fTempoParenthesizedKind) <<
//     ", line " << fInputLineNumber;

  ss << ']';

  return ss.str ();
}

void msrTempo::printFull (std::ostream& os) const
{
  os <<
    "[Tempo" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 27;

  os << std::left <<
    std::setw (fieldWidth) <<
   "fTempoKind" << ": " << msrTempoBeatUnitsKindAsString (fTempoKind) <<
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
      ": " << "[EMPTY]" <<
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
    "tempoPerMinute" << ": \"" << fTempoPerMinute << "\"" <<
    std::endl <<

    std::setw (fieldWidth) <<
    "fTempoEquivalentBeatUnit"  << ": " <<
    fTempoEquivalentBeatUnit <<
    std::endl <<

    std::setw (fieldWidth) <<
    "tempoParenthesizedKind"  << ": " <<
    msrTempoParenthesizedKindAsString (fTempoParenthesizedKind) <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTempoNotesRelationshipLeftElements";
  if (fTempoNotesRelationshipLeftElements) {
    os <<
      ": " <<
      std::endl;

    ++gIndenter;

    os <<
      fTempoNotesRelationshipLeftElements;

    --gIndenter;
  }
  else {
    os << ": " << "[NULL]" <<
    std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTempoNotesRelationshipKind" << ": " <<
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
    os << ": " << "[NULL]" <<
    std::endl;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fTempoPlacementKind" << ": " <<
    msrPlacementKindAsString (
      fTempoPlacementKind) <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

void msrTempo::print (std::ostream& os) const
{
  os << asString () << std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrTempo& elt)
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
