/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "visitor.h"

#include "mfAssert.h"
#include "mfIndentedTextOutput.h"
#include "mfStringsHandling.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif

#include "msrBrowsers.h"
#include "msrMeasureConstants.h"
#include "msrMusicXMLPrintLayouts.h"

#include "msrOah.h"


namespace MusicFormats
{

//______________________________________________________________________________
S_msrMusicXMLPrintLayout msrMusicXMLPrintLayout::create (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
{
  msrMusicXMLPrintLayout* o =
    new msrMusicXMLPrintLayout (
      inputLineNumber,
      upLinkToMeasure);
  assert (o != nullptr);
  return o;
}

S_msrMusicXMLPrintLayout msrMusicXMLPrintLayout::create (
  int                 inputLineNumber)
{
  return
    msrMusicXMLPrintLayout::create (
      inputLineNumber,
      gGlobalNullMeasureSmartPointer); // set later in setMeasureElementUpLinkToMeasure()
}

msrMusicXMLPrintLayout::msrMusicXMLPrintLayout (
  int                 inputLineNumber,
  const S_msrMeasure& upLinkToMeasure)
    : msrMeasureElement (
        inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceMusicXMLPrintLayouts ()) {
    gLogStream <<
      "Creating a print layout " <<
      std::endl;
  }
#endif

  fStaffSpacing = -1;

  fNewSystem = false;
  fNewPage   = false;

  fBlankPage = -1;

  fPageNumber = 0;
}

msrMusicXMLPrintLayout::~msrMusicXMLPrintLayout ()
{}

void msrMusicXMLPrintLayout::setMeasureElementUpLinkToMeasure (
  const S_msrMeasure& measure)
{
  setMusicXMLPrintLayoutUpLinkToMeasure (measure);
}

void msrMusicXMLPrintLayout::getMeasureElementUpLinkToMeasure (
  S_msrMeasure& upLinkToMeasure) const
{
  upLinkToMeasure = getMusicXMLPrintLayoutUpLinkToMeasure ();
}

void msrMusicXMLPrintLayout::setMusicXMLPrintLayoutUpLinkToMeasure (
  const S_msrMeasure& measure)
{
  // sanity check
  mfAssert (
    __FILE__, __LINE__,
    measure != nullptr,
    "measure is null");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTracingOahGroup->getTraceWholeNotes ()) {
    ++gIndenter;

    gLogStream <<
      "==> Setting the uplink to measure of print layout " <<
      asString () <<
      " to measure " << measure->asString () <<
      "' in measure '" <<
      measure->asString () <<
      std::endl;

    --gIndenter;
  }
#endif

  fMusicXMLPrintLayoutUpLinkToMeasure = measure;
}

S_msrMeasure msrMusicXMLPrintLayout::getMusicXMLPrintLayoutUpLinkToMeasure () const
{
  return fMusicXMLPrintLayoutUpLinkToMeasure;
}

void msrMusicXMLPrintLayout::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMusicXMLPrintLayout::acceptIn ()" <<
      std::endl;
  }

  if (visitor<S_msrMusicXMLPrintLayout>*
    p =
      dynamic_cast<visitor<S_msrMusicXMLPrintLayout>*> (v)) {
        S_msrMusicXMLPrintLayout elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMusicXMLPrintLayout::visitStart ()" <<
            std::endl;
        }
        p->visitStart (elem);
  }
}

void msrMusicXMLPrintLayout::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMusicXMLPrintLayout::acceptOut ()" <<
      std::endl;
  }

  if (visitor<S_msrMusicXMLPrintLayout>*
    p =
      dynamic_cast<visitor<S_msrMusicXMLPrintLayout>*> (v)) {
        S_msrMusicXMLPrintLayout elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMusicXMLPrintLayout::visitEnd ()" <<
            std::endl;
        }
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
    std::list<S_msrStaffLayout>::const_iterator i = fStaffLayoutsList.begin ();
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
  std::stringstream s;

  s <<
    "[MusicXMLPrintLayout" <<
    ", line " << fInputLineNumber <<
    ']';

  return s.str ();
}

void msrMusicXMLPrintLayout::print (std::ostream& os) const
{
  os <<
    "[MusicXMLPrintLayout" <<
    ", line " << fInputLineNumber <<
    std::endl;

  ++gIndenter;

  const int fieldWidth = 18;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPageLayout" << " : ";
    if (fPageLayout) {
      os << fPageLayout;
    }
    else {
      os << "[NONE]" << std::endl;
    }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fSystemLayout" << " : ";
    if (fSystemLayout) {
      os << fSystemLayout;
    }
    else {
      os << "[NONE]" << std::endl;
    }

  size_t
    staffLayoutsListSize =
      fStaffLayoutsList.size ();

  os <<
    std::setw (fieldWidth) <<
    "staffLayoutsList" << " : " <<
    mfSingularOrPlural (
      staffLayoutsListSize, "element", "elements") <<
    std::endl;

  if (staffLayoutsListSize) {
    ++gIndenter;

    std::list<S_msrStaffLayout>::const_iterator
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
    "systemDividers" << " : ";
    if (fSystemDividers) {
      os << fSystemDividers;
    }
    else {
      os << "[NONE]" << std::endl;
    }

  os << std::left <<
    std::setw (fieldWidth) <<
    "fStaffSpacing" << " : " << fStaffSpacing <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fNewSystem" << " : " << fNewSystem <<
    std::endl;
  os << std::left <<
    std::setw (fieldWidth) <<
    "fNewPage" << " : " << fNewPage <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fBlankPage" << " : " << fBlankPage <<
    std::endl;

  os << std::left <<
    std::setw (fieldWidth) <<
    "fPageNumber" << " : " << fPageNumber <<
    std::endl;

  --gIndenter;

  os << ']' << std::endl;
}

void msrMusicXMLPrintLayout::printShort (std::ostream& os) const
{
  os <<
    "[MusicXMLPrintLayout" <<
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
    os << "[NONE]" << std::endl;
  }

  return os;
}


}