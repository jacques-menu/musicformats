/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <cmath>
#include <string>

#include "mfPreprocessorSettings.h"

#include "msrOah2manPage.h"

#include "oahOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
msrOah2manPage::msrOah2manPage (
  const S_oahHandler& handler,
  std::ostream&       manPageOutputStream)
    : oah2manPage (
        handler,
        manPageOutputStream)
{}

msrOah2manPage::~msrOah2manPage ()
{}

//________________________________________________________________________
void msrOah2manPage::visitStart (S_msrRenamePartAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting msrRenamePartAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
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
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting msrRenamePartAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msrOah2manPage::visitStart (S_msrIgnorePartAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting msrIgnorePartAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
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
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting msrIgnorePartAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msrOah2manPage::visitStart (S_msrKeepPartAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting msrKeepPartAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
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
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting msrKeepPartAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void msrOah2manPage::visitStart (S_msrPitchesLanguageAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting msrPitchesLanguageAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
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
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting msrPitchesLanguageAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}


} // namespace
