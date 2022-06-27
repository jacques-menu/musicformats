/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // setw, setprecision, ...

#include "visitor.h"

#include "bsrParallels.h"

#include "oahOah.h"

#include "bsrOah.h"

#include "bsrBrowsers.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_bsrParallel bsrParallel::create (
  int inputLineNumber)
{
  bsrParallel* o =
    new bsrParallel (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

bsrParallel::bsrParallel (
  int inputLineNumber)
    : bsrElement (inputLineNumber)
{}

bsrParallel::~bsrParallel ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceParallels ()) {
    gLogStream <<
      "Creating bsrParallel '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      endl;
  }
#endif
}

void bsrParallel::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrParallel::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrParallel>*
    p =
      dynamic_cast<visitor<S_bsrParallel>*> (v)) {
        S_bsrParallel elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrParallel::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrParallel::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrParallel::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrParallel>*
    p =
      dynamic_cast<visitor<S_bsrParallel>*> (v)) {
        S_bsrParallel elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrParallel::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrParallel::browseData (basevisitor* v)
{
  for (
    list<S_bsrElement>::const_iterator i = fParallelElementsList.begin ();
    i != fParallelElementsList.end ();
    ++i ) {
    // browse the element
    bsrBrowser<bsrElement> browser (v);
    browser.browse (*(*i));
  } // for

}

string bsrParallel::parallelLayoutKindAsString (
  bsrParallelLayoutKind parallelLayoutKind)
{
  string result;

  switch (parallelLayoutKind) {
    case bsrParallel::kParallelLayoutBarOverBarKind:
      result = "parallelLayoutBarOverBarKind";
      break;
    case bsrParallel::kParallelLayoutLineOverLineKind:
      result = "parallelLayoutLineOverLineKind";
      break;
  } // switch

  return result;
}

void bsrParallel::print (ostream& os) const
{
  os <<
    "Parallel" <<
    endl;

  ++gIndenter;

  // print the parallel numbers
  const int fieldWidth = 18;

  os << left <<
    setw (fieldWidth) <<
    "printParallelNumber" << " : " << fPrintParallelNumber <<
    endl <<
    setw (fieldWidth) <<
    "brailleParallelNumber" << " : " << fBrailleParallelNumber <<
    endl;

  // print the parallel elements if any
  size_t parallelElementsListSize = fParallelElementsList.size ();

  if (parallelElementsListSize || gGlobalBsrOahGroup->getDisplayBsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "fParallelElementsList";
    if (parallelElementsListSize) {
      os << endl;
      ++gIndenter;

      list<S_bsrElement>::const_iterator
        iBegin = fParallelElementsList.begin (),
        iEnd   = fParallelElementsList.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        os << endl;
      } // for

      os << endl;

      --gIndenter;
    }
    else {
      os <<
        " : " << "empty" <<
      endl;
    }
  }

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_bsrParallel& elt)
{
  if (elt) {
    elt->print (os);
  }
  else {
    os << "*** NONE ***" << endl;
  }
  
  return os;
}


}
