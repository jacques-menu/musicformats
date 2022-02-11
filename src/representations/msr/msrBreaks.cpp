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

#include "visitor.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrBreaks.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
string userChosenLineBreakKindAsString (
  msrUserChosenLineBreakKind userChosenLineBreakKind)
{
  string result;

  switch (userChosenLineBreakKind) {
    case msrUserChosenLineBreakKind::kUserChosenLineBreakYes:
      result = "kUserChosenLineBreakYes";
      break;
    case msrUserChosenLineBreakKind::kUserChosenLineBreakNo:
      result = "kUserChosenLineBreakNo";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_msrLineBreak msrLineBreak::create (
  int           inputLineNumber,
  const string& nextBarNumber,
  msrUserChosenLineBreakKind
                userChosenLineBreakKind)
{
  msrLineBreak* o =
    new msrLineBreak (
      inputLineNumber,
      nextBarNumber,
      userChosenLineBreakKind);
  assert (o != nullptr);
  return o;
}

msrLineBreak::msrLineBreak (
  int           inputLineNumber,
  const string& nextBarNumber,
  msrUserChosenLineBreakKind
                userChosenLineBreakKind)
    : msrMeasureElement (inputLineNumber)
{
  fNextBarNumber = nextBarNumber;

  fUserChosenLineBreakKind = userChosenLineBreakKind;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceLineBreaks ()) {
    gLogStream <<
      "Constructing a break before measure " << fNextBarNumber <<
      ", fUserChosenLineBreakKind: " <<
      userChosenLineBreakKindAsString (
        fUserChosenLineBreakKind) <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
}

msrLineBreak::~msrLineBreak ()
{}

void msrLineBreak::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrLineBreak::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrLineBreak>*
    p =
      dynamic_cast<visitor<S_msrLineBreak>*> (v)) {
        S_msrLineBreak elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrLineBreak::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrLineBreak::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrLineBreak::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrLineBreak>*
    p =
      dynamic_cast<visitor<S_msrLineBreak>*> (v)) {
        S_msrLineBreak elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrLineBreak::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrLineBreak::browseData (basevisitor* v)
{}

string msrLineBreak::asString () const
{
  stringstream s;

  s <<
    "LineBreak" <<
    ", nextBarNumber = \"" << fNextBarNumber << "\"" <<
    ", fUserChosenLineBreakKind: " <<
    userChosenLineBreakKindAsString (
      fUserChosenLineBreakKind) <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrLineBreak::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_msrLineBreak& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
string userChosenPageBreakKindAsString (
  msrUserChosenPageBreakKind userChosenPageBreakKind)
{
  string result;

  switch (userChosenPageBreakKind) {
    case msrUserChosenPageBreakKind::kUserChosenPageBreakYes:
      result = "kUserChosenPageBreakYes";
      break;
    case msrUserChosenPageBreakKind::kUserChosenPageBreakNo:
      result = "kUserChosenPageBreakNo";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
S_msrPageBreak msrPageBreak::create (
  int   inputLineNumber,
  msrUserChosenPageBreakKind
        userChosenPageBreakKind)
{
  msrPageBreak* o =
    new msrPageBreak (
      inputLineNumber,
      userChosenPageBreakKind);
  assert (o != nullptr);
  return o;
}

msrPageBreak::msrPageBreak (
  int   inputLineNumber,
  msrUserChosenPageBreakKind
        userChosenPageBreakKind)
    : msrMeasureElement (inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTracePageBreaks ()) {
    gLogStream <<
      "Constructing a page break" <<
      ", fUserChosenPageBreakKind: " <<
      userChosenPageBreakKindAsString (
        fUserChosenPageBreakKind) <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fUserChosenPageBreakKind = userChosenPageBreakKind;
}

msrPageBreak::~msrPageBreak ()
{}

void msrPageBreak::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPageBreak::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrPageBreak>*
    p =
      dynamic_cast<visitor<S_msrPageBreak>*> (v)) {
        S_msrPageBreak elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrPageBreak::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPageBreak::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrPageBreak::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrPageBreak>*
    p =
      dynamic_cast<visitor<S_msrPageBreak>*> (v)) {
        S_msrPageBreak elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrPageBreak::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPageBreak::browseData (basevisitor* v)
{}

string msrPageBreak::asString () const
{
  stringstream s;

  s <<
    "PageBreak" <<
    ", fUserChosenPageBreakKind: " <<
    userChosenPageBreakKindAsString (
      fUserChosenPageBreakKind) <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrPageBreak::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_msrPageBreak& elt)
{
  elt->print (os);
  return os;
}


}
