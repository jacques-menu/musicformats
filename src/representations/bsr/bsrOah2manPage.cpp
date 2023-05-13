/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include <iomanip>      // std::setw, std::setprecision, ...
#include <cmath>
#include <string>

#include <regex>

#include "mfPreprocessorSettings.h"

#include "bsrOah2manPage.h"

#include "oahOah.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
bsrOah2manPage::bsrOah2manPage (
  const S_oahHandler& handler,
  std::ostream&       manPageOutputStream)
    : oah2manPage (
        handler,
        manPageOutputStream)
{}

bsrOah2manPage::~bsrOah2manPage ()
{}

//________________________________________________________________________
void bsrOah2manPage::visitStart (S_bsrFacSimileKindAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting bsrFacSimileKindAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fManPageOutputStream <<
    ".HP" <<
    std::endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    std::endl <<

    ".IP" <<
    std::endl <<
    elt->getDescription () <<
    std::endl;
}

void bsrOah2manPage::visitEnd (S_bsrFacSimileKindAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting bsrFacSimileKindAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void bsrOah2manPage::visitStart (S_bsrTextsLanguageAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting bsrTextsLanguageAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED

  fManPageOutputStream <<
    ".HP" <<
    std::endl <<
    "\\fB\\-" <<
    regex_replace (elt->getShortName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    "\\fB\\-" <<
    regex_replace (elt->getLongName (), std::regex ("-"), "\\-") <<
    "\\fR, " <<
    std::endl <<

    ".IP" <<
    std::endl <<
    elt->getDescription () <<
    std::endl;
}

void bsrOah2manPage::visitEnd (S_bsrTextsLanguageAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
    std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting bsrTextsLanguageAtom" <<
      " \"" << elt->fetchNames () << "\"";

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}


} // namespace
