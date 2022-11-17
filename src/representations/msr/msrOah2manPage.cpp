/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <cmath>
#include <string>

#include "msrOah2manPage.h"

#include "oahEnableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "tracingOah.h"
#endif


#include "oahOah.h"


namespace MusicFormats
{

//________________________________________________________________________
msrOah2manPage::msrOah2manPage (
  const S_oahHandler& handler,
  std::ostream&           manPageOutputStream)
    : oah2manPage (
        handler,
        manPageOutputStream)
{}

msrOah2manPage::~msrOah2manPage ()
{}

//________________________________________________________________________
void msrOah2manPage::visitStart (S_msrRenamePartAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> Start visiting msrRenamePartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;
  }
#endif
/*
  fManPageOutputStream <<
    ".HP" <<
    std::endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    std::endl <<

    ".IP" <<
    std::endl <<
    elt->getDescription () <<
    std::endl;
    */
}

void msrOah2manPage::visitEnd (S_msrRenamePartAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> End visiting msrRenamePartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msrOah2manPage::visitStart (S_msrIgnorePartAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> Start visiting msrIgnorePartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;
  }
#endif
/*
  fManPageOutputStream <<
    ".HP" <<
    std::endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    std::endl <<

    ".IP" <<
    std::endl <<
    elt->getDescription () <<
    std::endl;
    */
}

void msrOah2manPage::visitEnd (S_msrIgnorePartAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> End visiting msrIgnorePartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msrOah2manPage::visitStart (S_msrKeepPartAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> Start visiting msrKeepPartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;
  }
#endif
/*
  fManPageOutputStream <<
    ".HP" <<
    std::endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    std::endl <<

    ".IP" <<
    std::endl <<
    elt->getDescription () <<
    std::endl;
    */
}

void msrOah2manPage::visitEnd (S_msrKeepPartAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> End visiting msrKeepPartAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void msrOah2manPage::visitStart (S_msrPitchesLanguageAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> Start visiting msrPitchesLanguageAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;
  }
#endif
/*
  fManPageOutputStream <<
    ".HP" <<
    std::endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), "-", "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), "-", "\\-") <<
    "\\fR, " <<
    std::endl <<

    ".IP" <<
    std::endl <<
    elt->getDescription () <<
    std::endl;
    */
}

void msrOah2manPage::visitEnd (S_msrPitchesLanguageAtom& elt)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> End visiting msrPitchesLanguageAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;
  }
#endif
}


} // namespace
