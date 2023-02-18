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

#include "mfStaticSettings.h"

#include "oahOah.h"

#include "brailleGenerationOah2manPage.h"

#include "waeHandlers.h"


namespace MusicFormats
{

//________________________________________________________________________
brailleGenerationOah2manPage::brailleGenerationOah2manPage (
  const S_oahHandler& handler,
  std::ostream&       manPageOutputStream)
    : oah2manPage (
        handler,
        manPageOutputStream)
{}

brailleGenerationOah2manPage::~brailleGenerationOah2manPage ()
{}

//________________________________________________________________________
void brailleGenerationOah2manPage::visitStart (S_brailleOutputKindAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
		std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting brailleOutputKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

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

void brailleGenerationOah2manPage::visitEnd (S_brailleOutputKindAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
		std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting brailleOutputKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void brailleGenerationOah2manPage::visitStart (S_brailleUTFKindAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
		std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting brailleUTFKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

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

void brailleGenerationOah2manPage::visitEnd (S_brailleUTFKindAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
		std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting brailleUTFKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}

//________________________________________________________________________
void brailleGenerationOah2manPage::visitStart (S_brailleByteOrderingKindAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
		std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting brailleByteOrderingKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

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

void brailleGenerationOah2manPage::visitEnd (S_brailleByteOrderingKindAtom& elt)
{
#ifdef MF_TRACE_IS_ENABLED
  if (gOahOahGroup->getTraceOahVisitors ()) {
		std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting brailleByteOrderingKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif // MF_TRACE_IS_ENABLED
}


} // namespace
