/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <sstream>
#include <iomanip> // for 'setw()'

#include "visitor.h"

#include "bsrFootNotes.h"

#include "oahOah.h"
#include "bsrOah.h"
#include "brailleGenerationOah.h"

#include "bsrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_bsrFootNotes bsrFootNotes::create (
  int inputLineNumber)
{
  bsrFootNotes* o =
    new bsrFootNotes (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

bsrFootNotes::bsrFootNotes (
  int inputLineNumber)
    : bsrLine (
        inputLineNumber,
        0, // JMI ???
        gGlobalBrailleGenerationOahGroup->getCellsPerLine ())
{}

bsrFootNotes::~bsrFootNotes ()
{}

void bsrFootNotes::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrFootNotes::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrFootNotes>*
    p =
      dynamic_cast<visitor<S_bsrFootNotes>*> (v)) {
        S_bsrFootNotes elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrFootNotes::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrFootNotes::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrFootNotes::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrFootNotes>*
    p =
      dynamic_cast<visitor<S_bsrFootNotes>*> (v)) {
        S_bsrFootNotes elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrFootNotes::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrFootNotes::browseData (basevisitor* v)
{
  for (
    list<S_bsrFootNotesElement>::const_iterator i =
      fFootNotesElementsList.begin ();
    i != fFootNotesElementsList.end ();
    ++i ) {
    // browse the element
    bsrBrowser<bsrFootNotesElement> browser (v);
    browser.browse (*(*i));
  } // for
}

void bsrFootNotes::print (ostream& os) const
{
  os <<
    "FootNotes" <<
    endl;

  ++gIndenter;

  // print the notes if any
  const int fieldWidth = 19;

  unsigned int footNotesElementsListSize = fFootNotesElementsList.size ();

  if (footNotesElementsListSize || gGlobalBsrOahGroup->getDisplayBsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "footNotesElementsList" << " : " <<
      endl;

    if (footNotesElementsListSize) {
      ++gIndenter;

      list<S_bsrFootNotesElement>::const_iterator
        iBegin = fFootNotesElementsList.begin (),
        iEnd   = fFootNotesElementsList.end (),
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
        "empty" <<
      endl;
    }
  }

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_bsrFootNotes& elt)
{
  elt->print (os);
  return os;
}


}
