/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "mfStringsHandling.h"

#include "lpsrPartGroups.h"

#include "oahOah.h"

#include "lpsrOah.h"

#include "msrBrowsers.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_lpsrPartGroupBlock lpsrPartGroupBlock::create (
  const S_msrPartGroup& partGroup)
{
  lpsrPartGroupBlock* o = new lpsrPartGroupBlock (
    partGroup);
  assert (o != nullptr);
  return o;
}

lpsrPartGroupBlock::lpsrPartGroupBlock (
  const S_msrPartGroup& partGroup)
    : lpsrElement (0) // JMI
{
  fPartGroup = partGroup;
}

lpsrPartGroupBlock::~lpsrPartGroupBlock ()
{}

void lpsrPartGroupBlock::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> lpsrPartGroupBlock::acceptIn ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrPartGroupBlock>*
    p =
      dynamic_cast<visitor<S_lpsrPartGroupBlock>*> (v)) {
        S_lpsrPartGroupBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrPartGroupBlock::visitStart ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void lpsrPartGroupBlock::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> lpsrPartGroupBlock::acceptOut ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_lpsrPartGroupBlock>*
    p =
      dynamic_cast<visitor<S_lpsrPartGroupBlock>*> (v)) {
        S_lpsrPartGroupBlock elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching lpsrPartGroupBlock::visitEnd ()" <<
            std::endl;

          gWaeHandler->waeTrace (
            __FILE__, __LINE__,
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void lpsrPartGroupBlock::browseData (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% ==> lpsrPartGroupBlock::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  for (
    std::list<S_msrElement>::const_iterator i = fPartGroupBlockElements.begin ();
    i != fPartGroupBlockElements.end ();
    ++i
  ) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

#ifdef MF_TRACE_IS_ENABLED
  if (gGlobalLpsrOahGroup->getTraceLpsrVisitors ()) {
		std::stringstream ss;

    ss <<
      "% <== lpsrPartGroupBlock::browseData ()" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

void lpsrPartGroupBlock::print (std::ostream& os) const
{
  os <<
    std::endl <<
    "PartGroupBlock for partGroup \"" <<
    fPartGroup->getPartGroupCombinedName () <<
    "\", " <<
    msrPartGroupSymbolKindAsString (
      fPartGroup->getPartGroupSymbolKind ()) <<
    ", " <<
    mfSingularOrPlural (
      fPartGroupBlockElements.size (), "element", "elements") <<
    std::endl << std::endl;

  ++gIndenter;

  if (fPartGroupBlockElements.size ()) {
    std::list<S_msrElement>::const_iterator
      iBegin = fPartGroupBlockElements.begin (),
      iEnd   = fPartGroupBlockElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << std::endl;
    } // for
  }

  --gIndenter;
}

std::ostream& operator << (std::ostream& os, const S_lpsrPartGroupBlock& scr)
{
  scr->print (os);
  return os;
}


}
