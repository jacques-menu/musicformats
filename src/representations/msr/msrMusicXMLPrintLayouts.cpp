/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2025

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "mfPreprocessorSettings.h"

#include "mfAssert.h"
#include "mfIndentedTextOutput.h"
#include "mfStringsHandling.h"

#include "msrBrowsers.h"
#include "msrMeasureConstants.h"
#include "msrMusicXMLPrintLayouts.h"

#include "msrOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrMusicXMLPrintLayout msrMusicXMLPrintLayout::create (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
{
  msrMusicXMLPrintLayout* obj =
    new msrMusicXMLPrintLayout (
      inputLineNumber,
      upLinkToMeasure);
  assert (obj != nullptr);
  return obj;
}

S_msrMusicXMLPrintLayout msrMusicXMLPrintLayout::create (
  const mfInputLineNumber& inputLineNumber)
{
  return
    msrMusicXMLPrintLayout::create (
      inputLineNumber,
      gNullMeasure); // set later in setMeasureElementUpLinkToMeasure()
}

msrMusicXMLPrintLayout::msrMusicXMLPrintLayout (
  const mfInputLineNumber& inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gTraceOahGroup->getTraceMusicXMLPrintLayouts ()) {
    std::stringstream ss;

    ss <<
      "Creating a print layout ";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fStaffSpacing = -1;

  fNewSystem = false;
  fNewPage = false;

  fBlankPage = -1;

  fPageNumber = 0;
}

msrMusicXMLPrintLayout::~msrMusicXMLPrintLayout ()
{}

void msrMusicXMLPrintLayout::acceptIn (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMusicXMLPrintLayout::acceptIn ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrMusicXMLPrintLayout>*
    p =
      dynamic_cast<visitor<S_msrMusicXMLPrintLayout>*> (v)) {
        S_msrMusicXMLPrintLayout elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMusicXMLPrintLayout::visitStart ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitStart (elem);
  }
}

void msrMusicXMLPrintLayout::acceptOut (basevisitor* v)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gMsrOahGroup->getTraceMsrVisitors ()) {
    std::stringstream ss;

    ss <<
      "% ==> msrMusicXMLPrintLayout::acceptOut ()";

    gWaeHandler->waeTrace (
      __FILE__, mfInputLineNumber (__LINE__),
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  if (visitor<S_msrMusicXMLPrintLayout>*
    p =
      dynamic_cast<visitor<S_msrMusicXMLPrintLayout>*> (v)) {
        S_msrMusicXMLPrintLayout elem = this;

#ifdef MF_TRACE_IS_ENABLED
        if (gMsrOahGroup->getTraceMsrVisitors ()) {
          std::stringstream ss;

          ss <<
            "% ==> Launching msrMusicXMLPrintLayout::visitEnd ()";

          gWaeHandler->waeTrace (
            __FILE__, mfInputLineNumber (__LINE__),
            ss.str ());
        }
#endif // MF_TRACE_IS_ENABLED
        p->visitEnd (elem);
  }
}

void msrMusicXMLPrintLayout::browseData (basevisitor* v)
{
  if (fPageLayout) {
    // browse the page layout
    msrBrowser<msrPageLayout> browser (v);
    browser.browse (*fPageLayout);
  }

  if (fSystemLayout) {
    // browse the system layout
    msrBrowser<msrSystemLayout> browser (v);
    browser.browse (*fSystemLayout);
  }

  for (
    std::list <S_msrStaffLayout>::const_iterator i = fStaffLayoutsList.begin ();
    i != fStaffLayoutsList.end ();
    ++i
  ) {
    // browse staff layout
    msrBrowser<msrStaffLayout> browser (v);
    browser.browse (*(*i));
  } // for

  if (fSystemDividers) {
    // browse the system dividers
    msrBrowser<msrSystemDividers> browser (v);
    browser.browse (*fSystemDividers);
  }
}

std::string msrMusicXMLPrintLayout::asString () const
{
  std::stringstream ss;

  ss <<
    "[MusicXMLPrintLayout" <<
    ", line " << fInputLineNumber <<
    ']';

  return ss.str ();
}

void msrMusicXMLPrintLayout::printFull (std::ostream& os) const
{
  os <<
    "[MusicXMLPrintLayout" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  constexpr int fieldWidth = 18;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPageLayout" << ": ";
    if (fPageLayout) {
      os << fPageLayout;
    }
    else {
      os << "[NULL]" << std::endl;
    }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fSystemLayout" << ": ";
    if (fSystemLayout) {
      os << fSystemLayout;
    }
    else {
      os << "[NULL]" << std::endl;
    }

  size_t
    staffLayoutsListSize =
      fStaffLayoutsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "staffLayoutsList" << ": " <<
    mfSingularOrPlural (
      staffLayoutsListSize, "element", "elements") <<
    std::endl;

  if (staffLayoutsListSize) {
    ++gIndenter;

    std::list <S_msrStaffLayout>::const_iterator
      iBegin = fStaffLayoutsList.begin (),
      iEnd   = fStaffLayoutsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << std::endl;
    } // for

    --gIndenter;
  }

  os << std::left <<
    std::setw (fieldWidth) <<
    "systemDividers" << ": ";
    if (fSystemDividers) {
      os << fSystemDividers;
    }
    else {
      os << "[NULL]" << std::endl;
    }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fStaffSpacing" << ": " << fStaffSpacing <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNewSystem" << ": " << fNewSystem <<
    std::endl;
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNewPage" << ": " << fNewPage <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fBlankPage" << ": " << fBlankPage <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPageNumber" << ": " << fPageNumber <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

void msrMusicXMLPrintLayout::print (std::ostream& os) const
{
  os <<
    "[MusicXMLPrintLayout" << // JMI 0.9.66
    ", line " << fInputLineNumber <<
    ']' <<
    std::endl;
}

std::ostream& operator << (std::ostream& os, const S_msrMusicXMLPrintLayout& elt)
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
