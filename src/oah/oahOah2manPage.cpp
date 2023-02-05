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

#include "oahOah2manPage.h"

#include "mfStaticSettings.h"


#include "oahOah.h"

namespace MusicFormats
{

/* JMI
//________________________________________________________________________
oahOah2manPage::oahOah2manPage (
  const S_oahHandler& handler,
  std::ostream&       manPageOutputStream)
    : oah2manPage (
        handler,
        manPageOutputStream)
{}

oahOah2manPage::~oahOah2manPage ()
{}

//________________________________________________________________________
void oahOah2manPage::visitStart (S_oahOptionalValuesStyleKindAtom& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> Start visiting oahOptionalValuesStyleKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif
/ *
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
    * /
}

void oahOah2manPage::visitEnd (S_oahOptionalValuesStyleKindAtom& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
		std::stringstream ss;

    ss <<
      std::endl <<
      ".\\\" --> End visiting oahOptionalValuesStyleKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;

    gWaeHandler->waeTrace (
      __FILE__, __LINE__,
      ss.str ());
  }
#endif
}
*/

} // namespace
