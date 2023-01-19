/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/

#include "mfStaticSettings.h"

#ifdef MF_HARMONIES_EXTRA_IS_ENABLED

#include <iomanip>      // std::setw, std::setprecision, ...
#include <cmath>
#include <string>

#include "oahHarmoniesExtraOah2manPage.h"

#include "oahOah.h"

namespace MusicFormats
{

//________________________________________________________________________
harmoniesExtraOah2manPage::harmoniesExtraOah2manPage (
  const S_oahHandler& handler,
  std::ostream&       manPageOutputStream)
    : oah2manPage (
        handler,
        manPageOutputStream)
{}

harmoniesExtraOah2manPage::~harmoniesExtraOah2manPage ()
{}

//________________________________________________________________________
void harmoniesExtraOah2manPage::visitStart (S_extraDisplayAllHarmoniesStructuresAtom& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> Start visiting extraDisplayAllHarmoniesStructuresAtom" <<
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

void harmoniesExtraOah2manPage::visitEnd (S_extraDisplayAllHarmoniesStructuresAtom& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> End visiting extraDisplayAllHarmoniesStructuresAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void harmoniesExtraOah2manPage::visitStart (S_extraDisplayAllHarmoniesContentsAtom& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> Start visiting extraDisplayAllHarmoniesContentsAtom" <<
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

void harmoniesExtraOah2manPage::visitEnd (S_extraDisplayAllHarmoniesContentsAtom& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> End visiting extraDisplayAllHarmoniesContentsAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void harmoniesExtraOah2manPage::visitStart (S_extraDisplayHarmonyDetailsAtom& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> Start visiting extraDisplayHarmonyDetailsAtom" <<
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

void harmoniesExtraOah2manPage::visitEnd (S_extraDisplayHarmonyDetailsAtom& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> End visiting extraDisplayHarmonyDetailsAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;
  }
#endif
}

//________________________________________________________________________
void harmoniesExtraOah2manPage::visitStart (S_extraDisplayHarmonyAnalysisAtom& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> Start visiting extraDisplayHarmonyAnalysisAtom" <<
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

void harmoniesExtraOah2manPage::visitEnd (S_extraDisplayHarmonyAnalysisAtom& elt)
{
#ifdef MF_TRACING_IS_ENABLED
  if (gGlobalOahOahGroup->getTracingOahVisitors ()) {
    gLogStream <<
      std::endl <<
      ".\\\" --> End visiting extraDisplayHarmonyAnalysisAtom" <<
      " \"" << elt->fetchNames () << "\"" <<
      std::endl;
  }
#endif
}


} // namespace


#endif
