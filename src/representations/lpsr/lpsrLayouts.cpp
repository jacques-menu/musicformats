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

#include "lpsrLayouts.h"

#include "lpsrScheme.h"

#include "oahOah.h"

#include "lpsrOah.h"


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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrLayout::acceptIn ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrLayout>*
    p =
      dynamic_cast<visitor<S_lpsrLayout>*> (v)) {
        S_lpsrLayout elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrLayout::visitStart ()" <<
            std::endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrLayout::acceptOut (basevisitor* v)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
    gLogStream <<
      "% ==> lpsrLayout::acceptOut ()" <<
      std::endl;
  }
#endif

  if (visitor<S_lpsrLayout>*
    p =
      dynamic_cast<visitor<S_lpsrLayout>*> (v)) {
        S_lpsrLayout elem = this;

#ifdef OAH_TRACING_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          gLogStream <<
            "% ==> Launching lpsrLayout::visitEnd ()" <<
            std::endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrLayout::browseData (basevisitor* v)
{}

void lpsrLayout::print (std::ostream& os) const
{
  os <<
    "Layout" <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 9;

  os << std::left <<
    std::setw (fieldWidth) <<
   "layoutGlobalStaffSize" << ": " << fLayoutGlobalStaffSize <<
    std::endl;

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrLayout& lay)
{
  lay->print (os);
  return os;
}


}
