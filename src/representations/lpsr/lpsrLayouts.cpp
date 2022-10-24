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

#include "lpsrLayouts.h"

#include "lpsrScheme.h"

#include "oahOah.h"

#include "lpsrOah.h"


using namespace std;

namespace MusicFormats
{

//______________________________________________________________________________
S_lpsrLayout lpsrLayout::create (
  int inputLineNumber)
{
  lpsrLayout* o =
    new lpsrLayout (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

lpsrLayout::lpsrLayout (
  int inputLineNumber)
    : lpsrElement (inputLineNumber)
{
  fLayoutGlobalStaffSize = 20; // LilyPond default // JMI
}

lpsrLayout::~lpsrLayout ()
{}

void lpsrLayout::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrLayout::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrLayout>*
    p =
      dynamic_cast<visitor<S_lpsrLayout>*> (v)) {
        S_lpsrLayout elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrLayout::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrLayout::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrLayout::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrLayout>*
    p =
      dynamic_cast<visitor<S_lpsrLayout>*> (v)) {
        S_lpsrLayout elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrLayout::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrLayout::browseData (basevisitor* v)
{}

void lpsrLayout::print (ostream& os) const
{
  os <<
    "Layout" <<
    endl;

  ++gIndenter;

  const int fieldWidth = 9;

  os << left <<
    setw (fieldWidth) <<
   "layoutGlobalStaffSize" << " : " << fLayoutGlobalStaffSize <<
    endl;

  --gIndenter;
}

ostream& operator << (ostream& os, const S_lpsrLayout& lay)
{
  lay->print (os);
  return os;
}


}
