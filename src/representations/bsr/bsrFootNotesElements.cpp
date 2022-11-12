/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip> // for 'std::setw()'

#include "visitor.h"

#include "bsrFootNotesElements.h"

#include "oahOah.h"
#include "bsrOah.h"
#include "brailleGenerationOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_bsrFootNotesElement bsrFootNotesElement::create (
  int           inputLineNumber,
  const std::string& footNoteText)
{
  bsrFootNotesElement* o =
    new bsrFootNotesElement (
      inputLineNumber, footNoteText);
  assert (o != nullptr);
  return o;
}

bsrFootNotesElement::bsrFootNotesElement (
  int           inputLineNumber,
  const std::string& footNoteText)
    : bsrElement (inputLineNumber)
{
  fFootNoteText = footNoteText;

  fFootNotesElementCellsList =
    bsrCellsList::create (inputLineNumber);
}

bsrFootNotesElement::~bsrFootNotesElement ()
{}

void bsrFootNotesElement::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrFootNotesElement::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrFootNotesElement>*
    p =
      dynamic_cast<visitor<S_bsrFootNotesElement>*> (v)) {
        S_bsrFootNotesElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrFootNotesElement::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrFootNotesElement::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrFootNotesElement::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_bsrFootNotesElement>*
    p =
      dynamic_cast<visitor<S_bsrFootNotesElement>*> (v)) {
        S_bsrFootNotesElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrFootNotesElement::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrFootNotesElement::browseData (basevisitor* v)
{}

std::string bsrFootNotesElement::asString () const
{
  std::stringstream s;

  s <<
    "FootNotesElement" <<
    // JMI ", fFootNoteText: " <<
    ", \"" << fFootNoteText << "\"" <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrFootNotesElement::print (std::ostream& os) const
{
  os <<
    "FootNotesElement" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  os <<
    "footNoteText " << " : \"" << fFootNoteText << "\"" <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrFootNotesElement& elt)
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
