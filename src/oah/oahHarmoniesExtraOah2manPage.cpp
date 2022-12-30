/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2022

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfEnableHarmoniesExtraIfDesired.h"

#ifdef OAH_HARMONIES_EXTRA_IS_ENABLED

#include <iomanip>      // std::setw, std::setprecision, ...
#include <cmath>
#include <string>

#include "oahHarmoniesExtraOah2manPage.h"

#include "mfEnableTracingIfDesired.h"
#ifdef OAH_TRACING_IS_ENABLED
  #include "mfTracingOah.h"
#endif


#include "oahOah.h"

namespace MusicFormats
{

//________________________________________________________________________
harmoniesExtraOah2manPage::harmoniesExtraOah2manPage (
  const S_oahHandler& handler,
  std::ostream&           manPageOutputStream)
    : oah2manPage (
        handler,
        manPageOutputStream)
{}

harmoniesExtraOah2manPage::~harmoniesExtraOah2manPage ()
{}

//________________________________________________________________________
void harmoniesExtraOah2manPage::visitStart (S_extraShowAllHarmoniesStructuresAtom& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> Start visiting extraShowAllHarmoniesStructuresAtom" <<
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

void harmoniesExtraOah2manPage::visitEnd (S_extraShowAllHarmoniesStructuresAtom& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> End visiting extraShowAllHarmoniesStructuresAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void harmoniesExtraOah2manPage::visitStart (S_extraShowAllHarmoniesContentsAtom& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> Start visiting extraShowAllHarmoniesContentsAtom" <<
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

void harmoniesExtraOah2manPage::visitEnd (S_extraShowAllHarmoniesContentsAtom& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> End visiting extraShowAllHarmoniesContentsAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void harmoniesExtraOah2manPage::visitStart (S_extraShowHarmonyDetailsAtom& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> Start visiting extraShowHarmonyDetailsAtom" <<
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

void harmoniesExtraOah2manPage::visitEnd (S_extraShowHarmonyDetailsAtom& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> End visiting extraShowHarmonyDetailsAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void harmoniesExtraOah2manPage::visitStart (S_extraShowHarmonyAnalysisAtom& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> Start visiting extraShowHarmonyAnalysisAtom" <<
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

void harmoniesExtraOah2manPage::visitEnd (S_extraShowHarmonyAnalysisAtom& elt)
{
#ifdef OAH_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> End visiting extraShowHarmonyAnalysisAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;
  }
#endif
}


} // namespace


#endif
