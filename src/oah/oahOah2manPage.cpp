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

#include "oahOah2manPage.h"

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif


#include "oahOah.h"

namespace MusicFormats
{

/* JMI
//________________________________________________________________________
oahOah2manPage::oahOah2manPage (
  const S_oahHandler& handler,
  std::ostream&           manPageOutputStream)
    : oah2manPage (
        handler,
        manPageOutputStream)
{}

oahOah2manPage::~oahOah2manPage ()
{}

//________________________________________________________________________
void oahOah2manPage::visitStart (S_oahOptionalValuesStyleKindAtom& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> Start visiting oahOptionalValuesStyleKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;
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
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> End visiting oahOptionalValuesStyleKindAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;
  }
#endif
}
*/

} // namespace
