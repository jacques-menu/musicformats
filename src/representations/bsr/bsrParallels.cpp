/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...

#include "visitor.h"

#include "bsrParallels.h"

#include "oahOah.h"

#include "bsrOah.h"

#include "bsrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_bsrParallel bsrParallel::create (
  int inputLineNumber)
{
  bsrParallel* obj =
    new bsrParallel (
      inputLineNumber);
  assert (obj != nullptr);
  return obj;
}

bsrParallel::bsrParallel (
  int inputLineNumber)
    : bsrElement (inputLineNumber)
{}

bsrParallel::~bsrParallel ()
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceParallels ()) {
    std::stringstream ss;

    ss <<
      "Creating bsrParallel '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void bsrParallel::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrParallel::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrParallel>*
    p =
      dynamic_cast<visitor<S_bsrParallel>*> (v)) {
        S_bsrParallel elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrParallel::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void bsrParallel::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gBsrOahGroup->getTraceBsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% --> End visiting bsrTranscriptionNotesElement" <<
      "% ==> bsrParallel::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_bsrParallel>*
    p =
      dynamic_cast<visitor<S_bsrParallel>*> (v)) {
        S_bsrParallel elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gBsrOahGroup->getTraceBsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching bsrParallel::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void bsrParallel::browseData (basevisitor* v)
{
  for (S_bsrElement element : fParallelElementsList) {
    // browse the element
    bsrBrowser<bsrElement> browser (v);
    browser.browse (*element);
  } // for

}

std::string bsrParallelLayoutKindAsString (
  bsrParallelLayoutKind parallelLayoutKind)
{
  std::string result;

  switch (parallelLayoutKind) {
    case bsrParallelLayoutKind::kParallelLayoutBarOverBar:
      result = "kParallelLayoutBarOverBar";
      break;
    case bsrParallelLayoutKind::kParallelLayoutLineOverLine:
      result = "kParallelLayoutLineOverLine";
      break;
  } // switch

  return result;
}

std::ostream& operator << (std::ostream& os, const bsrParallelLayoutKind& elt)
{
  os << bsrParallelLayoutKindAsString (elt);
  return os;
}

void bsrParallel::print (std::ostream& os) const
{
  os <<
    "Parallel" <<
    std::endl;

  ++gIndenter;

  // print the parallel numbers
  const int fieldWidth = 18;

  os << std::left <<
    std::setw (fieldWidth) <<
    "printParallelNumber" << ": " << fPrintParallelNumber <<
    std::endl <<
    std::setw (fieldWidth) <<
    "brailleParallelNumber" << ": " << fBrailleParallelNumber <<
    std::endl;

  // print the parallel elements if any
  size_t parallelElementsListSize = fParallelElementsList.size ();

  if (parallelElementsListSize || gBsrOahGroup->getDisplayBsrFull ()) {
    os <<
      std::setw (fieldWidth) <<
      "fParallelElementsList";
    if (parallelElementsListSize) {
      os << std::endl;
      ++gIndenter;

      std::list<S_bsrElement>::const_iterator
        iBegin = fParallelElementsList.begin (),
        iEnd   = fParallelElementsList.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        os << std::endl;
      } // for

      os << std::endl;

      --gIndenter;
    }
    else {
      os <<
        ": " << "[EMPTY]" <<
      std::endl;
    }
  }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_bsrParallel& elt)
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
